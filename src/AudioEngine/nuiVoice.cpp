/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiVoice.h"

#include "nuiWaveReader.h"
#include "nuiAiffReader.h"
#include "nuiAudioDecoder.h"

#define FADE_LENGTH 44100

nuiVoice::nuiVoice(nuiSound* pSound)
: mpSound(pSound),
  mpStream(NULL),
  mpReader(NULL),
  mPlay(true),
  mDone(false),
  mLoop(false),
  mMute(false),
  mGain(1.f),
  mPan(0),
  mPosition(0),
  mFadingIn(false),
  mFadeInPosition(0),
  mFadingOut(false),
  mFadeOutPosition(0)
{
  if (mpSound)
    mpSound->Acquire();
  Load();
}

nuiVoice::nuiVoice(const nuiVoice& rVoice)
: mpSound(NULL),
  mpStream(NULL),
  mpReader(NULL)
{
  *this = rVoice;
}

nuiVoice::~nuiVoice()
{
  if (mpSound)
    mpSound->Release();
}

nuiVoice& nuiVoice::operator=(const nuiVoice& rVoice)
{
  if (mpSound)
    mpSound->Release();
  
  mpSound = rVoice.mpSound;
  mpSound->Acquire();
  Load();
  
  mPlay = rVoice.mPlay;
  mDone = rVoice.mDone;
  mLoop = rVoice.mLoop;
  mMute = rVoice.mMute;
  mGain = rVoice.mGain;
  mPan = rVoice.mPan;
  mPosition = rVoice.mPosition;
  
  return *this;
}

bool nuiVoice::Load()
{
  if (mpStream)
    delete mpStream;
  if (mpReader)
    delete mpReader;
  mpStream = NULL;
  mpReader = NULL;
  
  if (!mpSound)
    return false;
  
  nglPath path = mpSound->GetPath();
  
  if (!path.Exists())
  {
    NGL_OUT(_T("Can't load this audio file: %ls (file does not exist)\n"), path.GetNodeName().GetChars());
    return false;
  }
  
  nglIStream* pStream = path.OpenRead();
  if (!pStream)
  {
    NGL_OUT(_T("Can't load this audio file: %ls (stream can't be open)\n"), path.GetNodeName().GetChars());
    return false;
  }
  
  nuiSampleReader* pReader = NULL;
  nuiSampleInfo info;
  
  pReader = new nuiWaveReader(*pStream);
  if (!pReader->GetInfo(info))
  {
    delete pReader;
    pReader = new nuiAiffReader(*pStream);
    if (!pReader->GetInfo(info))
    {
      delete pReader;
      pReader = new nuiAudioDecoder(*pStream);
      if (!pReader->GetInfo(info))
      {
        NGL_OUT(_T("Can't load this audio file: %ls (reader can't be created)\n"), path.GetNodeName().GetChars());
        delete pReader;
        delete pStream;
        return false;
      }
    }
  }

  mpStream = pStream;
  mpReader = pReader;
  mInfo = info;
  NGL_OUT(_T("audio file loaded: %ls\n"), path.GetNodeName().GetChars());
  return true;
}

bool nuiVoice::IsValid() const
{
  return mpSound && mpStream && mpReader;
}

bool nuiVoice::IsDone() const
{
  return mDone;
}

void nuiVoice::Process(const std::vector<float*>& rOutput, uint32 SampleFrames)
{
  if (!mpSound || !mPlay || !IsValid())
    return;
  
  uint32 outChannels = rOutput.size();
  uint32 inChannels = mInfo.GetChannels();
  
  // if the input and output have not the same channel configuration, we only support mono input with stereo output
  if ((inChannels != outChannels) && (inChannels != 1 || outChannels != 2))
  {
    NGL_OUT(_T("channel format  unsupported: in = %d  out = %d\n"), inChannels, outChannels);
    NGL_ASSERT(0);
  }
  
  // create temp buffers
  std::vector<float*> buffers;
  for (uint32 c = 0; c < inChannels; c++)
  {
    float* pTemp = new float[SampleFrames];
    memset(pTemp, 0, SampleFrames * sizeof(float));
    buffers.push_back(pTemp);
  }
  
  nglCriticalSectionGuard guard(mCs);
  
  // fill temp buffers with data from the reader
  uint32 done = 0;
  uint32 toread = SampleFrames;
  while (toread && !mDone)
  {    
    std::vector<void*> temp;
    for (uint32 c = 0; c < inChannels; c++)
      temp.push_back(buffers[c] + done);
   
    NGL_ASSERT(mPosition <= (int64)GetSampleFrames());
    uint32 todo = MIN(toread, (int64)GetSampleFrames() - mPosition);
    mpReader->SetPosition(mPosition);
    uint32 read = mpReader->ReadDE(temp, todo, eSampleFloat32);
    
    mPosition += read;
    done += read;
    toread -= read;
    
    if (read == 0)
    {
      if (mLoop)
        mPosition = 0;
      else 
        mDone = true;
    }
  }
  
  // Fades
  if (mFadingIn)
  {
    uint32 todo = MIN(SampleFrames, FADE_LENGTH - mFadeInPosition);
    
    float* pFade = new float[todo];
    float* pTempFade = pFade;
    for (uint32 i = 0; i < todo; i++)
      *pTempFade++ = (float)(mFadeInPosition + i) / (float)FADE_LENGTH;
    
    for (uint32 c = 0; c < outChannels; c++)
    {
      float* pDst = buffers[c];
      pTempFade = pFade;
      for (uint32 i = 0; i < todo; i++)
        *pDst++ *= *pTempFade++;
    }
    
    delete[] pFade;
    
    mFadeInPosition += todo;
    if (mFadeInPosition == FADE_LENGTH)
    {
      mFadingIn = false;
      mFadeInPosition = 0;
    }
  }
  else if (mFadingOut)
  {
    uint32 todo = MIN(SampleFrames, FADE_LENGTH - mFadeOutPosition);
    
    float* pFade = new float[todo];
    float* pTempFade = pFade;
    for (uint32 i = 0; i < todo; i++)
      *pTempFade++ = 1.f - ((float)(mFadeOutPosition + i) / (float)FADE_LENGTH);
    
    for (uint32 c = 0; c < outChannels; c++)
    {
      float* pDst = buffers[c];
      pTempFade = pFade;
      for (uint32 i = 0; i < todo; i++)
        *pDst++ *= *pTempFade++;
    }
    
    delete[] pFade;
    
    mFadeOutPosition += todo;
    if (mFadeOutPosition == FADE_LENGTH)
    {
      mFadingOut = false;
      mFadeOutPosition = 0;
      mPlay = false;
      
      for (uint32 c = 0; c < outChannels; c++)
      {
        float* pDst = buffers[c];
        for (uint32 i = todo; i < SampleFrames; i++)
          pDst[i] = 0;
      }
    }
  }
  
  
  // copy temp buffers to output and apply gain
  if (!mMute && mGain >= 0.f)
  {
    float pan = mPan;
    pan = MIN(pan, 1.0);
    pan = MAX(pan, -1.0);
    float panLeft = MIN(1.0, 1.0 - pan);
    float panRight = MIN(1.0, 1.0 + pan);
    for (uint32 c= 0; c < outChannels; c++)
    {
      float mult = mGain * (c == 0 ? panLeft : panRight);
      float* pSrc = (inChannels == outChannels) ? buffers[c] : buffers[0]; // mono input signal: use first channel
      float* pDst = rOutput[c];
      for (uint32 i = 0; i < SampleFrames; i++)
        *pDst++ += (*pSrc++) * mult;
    }
  }
  
  for (uint32 c = 0; c < buffers.size(); c++)
    delete[] buffers[c];
  
}

void nuiVoice::Play()
{
  nglCriticalSectionGuard guard(mCs);
  mPlay = true;
}

void nuiVoice::Pause()
{
  nglCriticalSectionGuard guard(mCs);
  mPlay = false;
}

bool nuiVoice::IsPlaying() const
{
  return mPlay;
}

void nuiVoice::FadeIn()
{
  if (mPlay)
    return;
  
  nglCriticalSectionGuard guard(mCs);
  mFadingIn = true;
  mFadingOut = false;
  
  mFadeInPosition = 0;
  mPlay = true;
}

void nuiVoice::FadeOut()
{
  if (!mPlay)
    return;
  
  nglCriticalSectionGuard guard(mCs);
  mFadingOut = true;
  mFadingIn = false;
  
  mFadeOutPosition = 0;
}

void nuiVoice::SetLoop(bool loop)
{
  nglCriticalSectionGuard guard(mCs);
  mLoop = loop;
}

bool nuiVoice::IsLooping()
{
  return mLoop;
}

uint64 nuiVoice::GetSampleFrames() const
{
  return mInfo.GetSampleFrames();
}

int64 nuiVoice::GetPosition() const
{
  return mPosition;
}

void nuiVoice::SetPosition(int64 position)
{
  nglCriticalSectionGuard guard(mCs);
  mPosition = position;
  if (mpReader)
    mpReader->SetPosition(mPosition);
}

float nuiVoice::GetGainDb() const
{
  float Db = GainToDB(mGain);
  return Db;
}

void nuiVoice::SetGainDb(float Db)
{
  nglCriticalSectionGuard guard(mCs);
  mGain = DBToGain(Db);
}

float nuiVoice::GetGain() const
{
  return mGain;
}

void nuiVoice::SetGain(float gain)
{
  nglCriticalSectionGuard guard(mCs);
  mGain = gain;
}

bool nuiVoice::IsMute() const
{
  return mMute;
}

void nuiVoice::SetMute(bool mute)
{
  nglCriticalSectionGuard guard(mCs);
  mMute = mute;
}


float nuiVoice::GetPan() const
{
  return mPan;
}

void nuiVoice::SetPan(float pan)
{
  mPan = pan;
}