/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"

//#define AUDIO_LOG
//#define AUDIO_PROFILE




nuiAudioEngine::nuiAudioEngine(double SampleRate, int32 BufferSize, ChannelConfig inputConfig)
: mSampleRate(SampleRate),
  mBufferSize(BufferSize),
  mInputDelegateSet(false),
  mOutputDelegateSet(false),
  mGain(1.f),
  mMute(false),
  mPan(0),
  mPlaying(true),
  mCs(_T("nuiAudioEngineCriticalSection"))
{  
  if (SetObjectClass(_T("nuiAudioEngine")))
    InitAttributes();
  
  mpOutAudioDevice = NULL;
  mpInAudioDevice = NULL;
  AudioInit(inputConfig);
}

nuiAudioEngine::~nuiAudioEngine()
{  
  delete mpOutAudioDevice;
  delete mpInAudioDevice;
  
  for (int32 i = 0; i < mVoices.size(); i++)
    mVoices[i]->Release();
  
}

void nuiAudioEngine::InitAttributes()
{ 
  nuiAttribute<float>* pGainAttrib = new nuiAttribute<float>
  (nglString(_T("gain")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiAudioEngine::GetGain),
   nuiMakeDelegate(this, &nuiAudioEngine::SetGain));  
  AddAttribute(pGainAttrib);
  
  nuiAttribute<float>* pGainDbAttrib = new nuiAttribute<float>
               (nglString(_T("gainDb")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiAudioEngine::GetGainDb),
                nuiMakeDelegate(this, &nuiAudioEngine::SetGainDb));  
  AddAttribute(pGainDbAttrib);
  
  nuiAttribute<bool>* pMuteAttrib = new nuiAttribute<bool>
               (nglString(_T("mute")), nuiUnitCustom,
                nuiMakeDelegate(this, &nuiAudioEngine::IsMuted),
                nuiMakeDelegate(this, &nuiAudioEngine::SetMute));
  AddAttribute(pMuteAttrib);
  
  nuiAttribute<float>* pPanAttrib = new nuiAttribute<float>
  (nglString(_T("pan")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiAudioEngine::GetPan),
   nuiMakeDelegate(this, &nuiAudioEngine::SetPan));  
  AddAttribute(pPanAttrib);
  
  nuiAttribute<bool>* pPlayAttrib = new nuiAttribute<bool>
  (nglString(_T("play")), nuiUnitCustom,
   nuiMakeDelegate(this, &nuiAudioEngine::IsPlaying),
   nuiMakeDelegate(this, &nuiAudioEngine::SetPlay));
  AddAttribute(pPlayAttrib);
}


bool nuiAudioEngine::AudioInit(ChannelConfig inputConfig)
{
  mpOutAudioDevice = nuiAudioDeviceManager::Get().GetDefaultOutputDevice();

  NGL_OUT(_T("Default output: %s\n"), mpOutAudioDevice->GetName().GetChars());

  bool res = ActivateOutputDevice();
  
  if (inputConfig == eNone)
    return res;
  
  mpInAudioDevice = nuiAudioDeviceManager::Get().GetDefaultInputDevice();
  res &= ActivateInputDevice(inputConfig);
  
  return res;
}





void nuiAudioEngine::DeactivateOutputDevice()
{
  mpOutAudioDevice->Close();
}

void nuiAudioEngine::DeactivateInputDevice()
{
  mpInAudioDevice->Close();
}



bool nuiAudioEngine::ActivateOutputDevice()
{
  std::vector<int32> InputChannels;
  std::vector<int32> OutputChannels;
  OutputChannels.push_back(0);
  OutputChannels.push_back(1);
  bool res = mpOutAudioDevice->Open(InputChannels, OutputChannels, mSampleRate, mBufferSize, nuiMakeDelegate(this, &nuiAudioEngine::ProcessAudioOutput));
  return res;
}



bool nuiAudioEngine::ActivateInputDevice(ChannelConfig inputConfig)
{
  std::vector<int32> InputChannels;
  std::vector<int32> OutputChannels;
 
  if (inputConfig == eMono)
    InputChannels.push_back(0);
  else if (inputConfig == eStereo)
  {
    InputChannels.push_back(0);
    InputChannels.push_back(1);
  }
  
  bool res = mpInAudioDevice->Open(InputChannels, OutputChannels, mSampleRate, mBufferSize, nuiMakeDelegate(this, &nuiAudioEngine::ProcessAudioInput));
  return res;
}


double nuiAudioEngine::GetSampleRate() const
{
  return mSampleRate;
}

int32 nuiAudioEngine::GetBufferSize() const
{
  return mBufferSize;
}

void nuiAudioEngine::SetInputProcessDelegate(const nuiAudioEngine::InputDelegate& rDelegate)
{
  mInputDelegateSet = true;
  mInputDelegate = rDelegate;
}

void nuiAudioEngine::SetOutputProcessDelegate(const nuiAudioEngine::OutputDelegate& rDelegate)
{
  mOutputDelegateSet = true;
  mOutputDelegate = rDelegate;
}

void nuiAudioEngine::UnsetInputProcessDelegate()
{
  mInputDelegateSet = false;
}

void nuiAudioEngine::UnsetOutputProcessDelegate()
{
  mOutputDelegateSet = false;
}


void nuiAudioEngine::ProcessAudioOutput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, int32 SampleFrames)
{
#ifdef AUDIO_PROFILE
  double beginTime = nglTime();
  {
#endif

  std::vector<nuiVoice*> voicesToAdd;
  std::vector<nuiVoice*> voicesToRemove;
  mCs.Lock();
  voicesToAdd = mAddVoicesQueue;
  mAddVoicesQueue.clear();
  
  voicesToRemove = mRemoveVoicesQueue;
  mRemoveVoicesQueue.clear();
  mCs.Unlock();
  
  for (int32 i = 0; i < voicesToRemove.size(); i++)
  {
    nuiVoice* pVoiceToRemove = voicesToRemove[i];
    std::vector<nuiVoice*>::iterator it = mVoices.begin();
    std::vector<nuiVoice*>::iterator end = mVoices.end();
    while (it != end)
    {
      nuiVoice* pCurrent = *it;
      if (pCurrent == pVoiceToRemove)
      {
        mVoices.erase(it);
        pCurrent->Release();
        break;
      }
      ++it;
    }
  }
  
  for (int32 i = 0; i < voicesToAdd.size(); i++)
    mVoices.push_back(voicesToAdd[i]);
  
  int32 channels = rOutput.size();
  for (int32 c = 0; c < channels; c++)
  {
    memset(&rOutput[c][0], 0, sizeof(float) * SampleFrames);
  }
    
  
  if (!mPlaying)
    return;
  
  std::vector<float*> buffers;
  for (int32 c = 0; c < channels; c++)
  {
    float* pBuffer = new float[SampleFrames];
    memset(pBuffer, 0, SampleFrames * sizeof(float));
    buffers.push_back(pBuffer);
  }
    
  
  for (int32 i = 0 ; i < mVoices.size(); i++)
  {
    nuiVoice* pVoice = mVoices[i];
    pVoice->Process(buffers, SampleFrames);
  }
  
  if (!mMute && mGain > 0)
  {
    float pan = mPan;
    pan = MIN(pan, 1.0);
    pan = MAX(pan, -1.0);
    float panLeft = MIN(1.0, 1.0 - pan);
    float panRight = MIN(1.0, 1.0 + pan);
    for (int32 c = 0; c < channels; c++)
    {
      float* pDst = rOutput[c];
      float* pSrc = buffers[c];
      float mult = mGain * (c == 0 ? panLeft : panRight);
      for (int32 i = 0; i < SampleFrames; i++)
        *pDst++ += (*pSrc++) * mult;
    }
  }
    
  if (mOutputDelegateSet)
    mOutputDelegate(rOutput, SampleFrames);
  

  std::vector<nuiVoice*>::iterator it = mVoices.begin();
  int32 index = 0;
  while (it != mVoices.end())
  {
    nuiVoice* pVoice = *it;
    if (pVoice->IsDone() == pVoice->GetRefCount() == 1)
    {
      mVoices.erase(it);
      it = mVoices.begin() + index;
      pVoice->Release();
      continue;
    }
      
    ++it;
    ++index;
  }
  
#ifdef AUDIO_LOG
  nglPath path(ePathUserDesktop);
  path+= nglString(_T("AudioLog"));
  nglOFile* pFile = new nglOFile(path, eOFileAppend);
  if (pFile)
  {
    for (int32 c =  0; c < rOutput.size(); c++)
    {
      for (int32 i = 0; i < SampleFrames; i++)
      {
        pFile->WriteFloat(rOutput[c] + i, 1);
      }
    }
    delete pFile;
  }
#endif
  
  
  for (int32 c = 0; c < channels; c++)
    delete[] buffers[c];

    
#ifdef AUDIO_PROFILE
    }
    double endTime = nglTime();
    double diff = endTime - beginTime;
    NGL_OUT(_T("AUDIO_PROFILE [%.2f] : %.3fms\n"), endTime, diff * 1000.f);
#endif
    
   
}

void nuiAudioEngine::ProcessAudioInput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, int32 SampleFrames)
{
  if (mInputDelegateSet)
    mInputDelegate(rInput, SampleFrames);
}

nuiVoice* nuiAudioEngine::PlaySound(const nglPath& path, nuiSound::Type type)
{
  nuiSound* pSound = nuiSoundManager::Instance.GetSound(path, type);  
  nuiVoice* pVoice = PlaySound(pSound);
  
  return pVoice;
}

nuiVoice* nuiAudioEngine::PlaySound(nuiSound* pSound)
{
  nuiVoice* pVoice = pSound->GetVoice();
  
  nglCriticalSectionGuard guard(mCs);
  pVoice->Acquire();
  mAddVoicesQueue.push_back(pVoice);
  return pVoice;
}

void nuiAudioEngine::StopSound(nuiVoice* pVoice)
{
  nglCriticalSectionGuard guard(mCs);
  
  mRemoveVoicesQueue.push_back(pVoice);
}






float nuiAudioEngine::GetGain()
{
  return mGain;
}

void nuiAudioEngine::SetGain(float gain)
{
  mGain = gain;
}

float nuiAudioEngine::GetGainDb()
{
  float db = GainToDB(GetGain());
  return db;
}

void nuiAudioEngine::SetGainDb(float Db)
{
  SetGain(DBToGain(Db));
}

bool nuiAudioEngine::IsMuted()
{
  return mMute;
}

void nuiAudioEngine::SetMute(bool mute)
{
  mMute = mute;
}

float nuiAudioEngine::GetPan()
{
  return mPan;
}

void nuiAudioEngine::SetPan(float pan)
{
  mPan = pan;
}

bool nuiAudioEngine::IsPlaying()
{
  return mPlaying;
}

void nuiAudioEngine::SetPlay(bool play)
{
  mPlaying = play;
}

void nuiAudioEngine::Play()
{
  SetPlay(true);
}

void nuiAudioEngine::Pause()
{
  SetPlay(false);
}
