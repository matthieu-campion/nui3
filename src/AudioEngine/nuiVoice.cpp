/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


nuiVoice::nuiVoice(nuiSound* pSound)
: mpSound(pSound),
  mPlay(true),
  mDone(false),
  mLoop(false),
  mMute(false),
  mGain(1.f),
  mPan(0),
  mPosition(0),
  mFadingIn(false),
  mFadeInPosition(0),
  mFadeInLength(0),
  mFadingOut(false),
  mFadeOutPosition(0),
  mFadeOutLength(0)
{
  if (SetObjectClass(_T("nuiVoice")))
    InitAttributes();
  
  if (mpSound)
    mpSound->Acquire();
}

nuiVoice::nuiVoice(const nuiVoice& rVoice)
: mpSound(NULL)
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
  
  mPlay = rVoice.mPlay;
  mDone = rVoice.mDone;
  mLoop = rVoice.mLoop;
  mMute = rVoice.mMute;
  mGain = rVoice.mGain;
  mPan = rVoice.mPan;
  mPosition = rVoice.mPosition;
  
  return *this;
}

void nuiVoice::InitAttributes()
{
  nuiAttribute<float>* pGainAttrib = new nuiAttribute<float>
  (nglString(_T("gain")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::GetGain),
   nuiMakeDelegate(this, &nuiVoice::SetGain));  
  AddAttribute(pGainAttrib);
  
  nuiAttribute<float>* pGainDbAttrib = new nuiAttribute<float>
  (nglString(_T("gainDb")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::GetGainDb),
   nuiMakeDelegate(this, &nuiVoice::SetGainDb));  
  AddAttribute(pGainDbAttrib);
  
  nuiAttribute<bool>* pMuteAttrib = new nuiAttribute<bool>
  (nglString(_T("mute")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::IsMute),
   nuiMakeDelegate(this, &nuiVoice::SetMute));
  AddAttribute(pMuteAttrib);
  
  nuiAttribute<float>* pPanAttrib = new nuiAttribute<float>
  (nglString(_T("pan")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::GetPan),
   nuiMakeDelegate(this, &nuiVoice::SetPan));  
  AddAttribute(pPanAttrib);
  
  nuiAttribute<bool>* pPlayAttrib = new nuiAttribute<bool>
  (nglString(_T("play")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::IsPlaying),
   nuiMakeDelegate(this, &nuiVoice::SetPlay));
  AddAttribute(pPlayAttrib);
  
  nuiAttribute<bool>* pLoopAttrib = new nuiAttribute<bool>
  (nglString(_T("loop")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::IsLooping),
   nuiMakeDelegate(this, &nuiVoice::SetLoop));
  AddAttribute(pLoopAttrib);
  
  nuiAttribute<int64>* pPosAttrib = new nuiAttribute<int64>
  (nglString(_T("position")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiVoice::GetPosition),
   nuiMakeDelegate(this, &nuiVoice::SetPosition));
  AddAttribute(pPosAttrib);
}



bool nuiVoice::IsDone() const
{
  return mDone;
}

void nuiVoice::Process(const std::vector<float*>& rOutput, int32 SampleFrames)
{
  int32 done = 0;
  std::vector<float*> output;
  for (int c = 0; c < rOutput.size(); c++)
    output.push_back(rOutput[c]);
  
  while (done < SampleFrames)
  {
    int64 nextevent = std::numeric_limits<int64>::max();
    int64 pos = GetPosition();
    {
      nglCriticalSectionGuard guard(mEventCs);
      while (!mEvents.empty() && mEvents.front().mPosition == pos)
      {
        const nuiVoiceEvent& event(mEvents.front());
        switch (event.mType)
        {
        case nuiVoiceEvent::FadeOut:
          if (event.mParam > 0)
            FadeOut(event.mParam);
          break;
        case nuiVoiceEvent::FadeIn:
          if (event.mParam > 0)
            FadeIn(event.mParam);
          break;
        }

        mEvents.erase(mEvents.begin());
      }
      
      if (!mEvents.empty())
        nextevent = mEvents.front().mPosition;
    }
    
    const int64 nexteventwait = nextevent - pos;
    const int64 todo = MIN(SampleFrames, nexteventwait);
    
    
    
    ProcessInternal(rOutput, todo);
    for (int c = 0; c < output.size(); c++)
      output[c] += todo;

    done += todo;
  }
}

void nuiVoice::ProcessInternal(const std::vector<float*>& rOutput, int32 SampleFrames)
{
  if (!mpSound || !mPlay || !IsValid())
    return;
  
  int32 outChannels = rOutput.size();
  int32 inChannels = GetChannels();
  
  // if the input and output have not the same channel configuration, we only support mono input with stereo output
  if ((inChannels != outChannels) && (inChannels != 1 || outChannels != 2))
  {
    NGL_OUT(_T("channel format  unsupported: in = %d  out = %d\n"), inChannels, outChannels);
    NGL_ASSERT(0);
  }
  
  // create temp buffers
  std::vector<float*> buffers;
  for (int32 c = 0; c < inChannels; c++)
  {
    float* pTemp = new float[SampleFrames];
    memset(pTemp, 0, SampleFrames * sizeof(float));
    buffers.push_back(pTemp);
  }
  
  nglCriticalSectionGuard guard(mCs);
  
  // fill temp buffers with data from the reader
  int32 done = 0;
  int32 toread = SampleFrames;
  while (toread && !mDone)
  {    
    std::vector<float*> temp;
    for (int32 c = 0; c < inChannels; c++)
      temp.push_back(buffers[c] + done);
   
    int32 read = ReadSamples(temp, mPosition, toread);
    
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
    int32 todo = MIN(SampleFrames, mFadeInLength - mFadeInPosition);
    
    float* pFade = new float[todo];
    float* pTempFade = pFade;
    for (int32 i = 0; i < todo; i++)
      *pTempFade++ = (float)(mFadeInPosition + i) / (float)mFadeInLength;
    
    for (int32 c = 0; c < inChannels; c++)
    {
      float* pDst = buffers[c];
      pTempFade = pFade;
      for (int32 i = 0; i < todo; i++)
        *pDst++ *= *pTempFade++;
    }
    
    delete[] pFade;
    
    mFadeInPosition += todo;
    if (mFadeInPosition == mFadeInLength)
    {
      mFadingIn = false;
      mFadeInPosition = 0;
    }
  }
  else if (mFadingOut)
  {
    int32 todo = MIN(SampleFrames, mFadeOutLength - mFadeOutPosition);
    
    float* pFade = new float[todo];
    float* pTempFade = pFade;
    for (int32 i = 0; i < todo; i++)
      *pTempFade++ = 1.f - ((float)(mFadeOutPosition + i) / (float)mFadeOutLength);
    
    for (int32 c = 0; c < inChannels; c++)
    {
      float* pDst = buffers[c];
      pTempFade = pFade;
      for (int32 i = 0; i < todo; i++)
        *pDst++ *= *pTempFade++;
    }
    
    delete[] pFade;
    
    mFadeOutPosition += todo;
    if (mFadeOutPosition == mFadeOutLength)
    {
      mFadingOut = false;
      mFadeOutPosition = 0;
      mPlay = false;
      
      for (int32 c = 0; c < inChannels; c++)
      {
        float* pDst = buffers[c];
        for (int32 i = todo; i < SampleFrames; i++)
          pDst[i] = 0;
      }
    }
  }
  
  
  // copy temp buffers to output and apply gain
  if (!mMute && mGain > 0.f)
  {
    float pan = mPan;
    pan = MIN(pan, 1.0);
    pan = MAX(pan, -1.0);
    float panLeft = MIN(1.0, 1.0 - pan);
    float panRight = MIN(1.0, 1.0 + pan);
    for (int32 c= 0; c < outChannels; c++)
    {
      float mult = mGain * (c == 0 ? panLeft : panRight);
      float* pSrc = (inChannels == outChannels) ? buffers[c] : buffers[0]; // mono input signal: use first channel
      float* pDst = rOutput[c];
      for (int32 i = 0; i < SampleFrames; i++)
        *pDst++ += (*pSrc++) * mult;
    }
  }
  
  for (int32 c = 0; c < buffers.size(); c++)
    delete[] buffers[c];
  
}

void nuiVoice::Play()
{
  SetPlay(true);
}

void nuiVoice::Pause()
{
  SetPlay(false);
}

bool nuiVoice::IsPlaying() const
{
  return mPlay;
}

void nuiVoice::SetPlay(bool play)
{
  nglCriticalSectionGuard guard(mCs);
  mPlay = play;
}

void nuiVoice::FadeIn(int32 length)
{
  if (mPlay)
    return;
  
  nglCriticalSectionGuard guard(mCs);
  mFadingIn = true;
  mFadingOut = false;
  
  mFadeInLength = length;
  mFadeInPosition = 0;
  mPlay = true;
}

void nuiVoice::FadeOut(int32 length)
{
  if (!mPlay)
    return;
  
  nglCriticalSectionGuard guard(mCs);
  mFadingOut = true;
  mFadingIn = false;
  
  mFadeOutLength = length;
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

int64 nuiVoice::GetPosition() const
{
  return mPosition;
}

void nuiVoice::SetPosition(int64 position)
{
  nglCriticalSectionGuard guard(mCs);
  SetPositionInternal(position);
  mPosition = position;
}

void nuiVoice::SetPositionInternal(int64 position)
{
  
}

float nuiVoice::GetGainDb() const
{
  float Db = GainToDB(GetGain());
  return Db;
}

void nuiVoice::SetGainDb(float Db)
{
  SetGain(DBToGain(Db));
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

void nuiVoice::PostEvent(const nuiVoiceEvent& rEvent)
{
  nglCriticalSectionGuard guard(mEventCs);
  mEvents.push_back(rEvent);
}

nuiVoiceEvent::nuiVoiceEvent(Type EventType, int64 position, int64 param)
: mPosition(position), mType(EventType), mParam(param)
{
}

