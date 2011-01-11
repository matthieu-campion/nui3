/*
 *  nuiAudioEngine.cpp
 *
 *  Copyright 2010 blumug. All rights reserved.
 *
 */


#include "nuiAudioEngine.h"


nuiAudioEngine::nuiAudioEngine(double SampleRate, uint32 BufferSize)
: mSampleRate(SampleRate),
  mBufferSize(BufferSize),
  mGain(1.f),
  mMute(false),
  mCs(_T("nuiAudioEngineCriticalSection"))
{  
  if (SetObjectClass(_T("nuiAudioEngine")))
  {
    InitAttributes();
  }
  
  mpOutAudioDevice = NULL;
  AudioInit();
}

nuiAudioEngine::~nuiAudioEngine()
{  
  delete mpOutAudioDevice;
  
  for (uint32 i = 0; i < mVoices.size(); i++)
    mVoices[i]->Release();
  
}

void nuiAudioEngine::InitAttributes()
{  
//  nuiAttribute<float>* pGainAttrib = new nuiAttribute<float>
//               (nglString(_T("gain")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiAudioEngine::GetGainDb),
//                nuiMakeDelegate(this, &nuiAudioEngine::SetGainDb));  
//  AddAttribute(pGainAttrib);
//  
//  nuiAttribute<bool>* pMuteAttrib = new nuiAttribute<bool>
//               (nglString(_T("mute")), nuiUnitCustom,
//                nuiMakeDelegate(this, &nuiAudioEngine::IsMute),
//                nuiMakeDelegate(this, &nuiAudioEngine::SetMute));
//  AddAttribute(pMuteAttrib);
}


bool nuiAudioEngine::AudioInit()
{
  mpOutAudioDevice = nuiAudioDeviceManager::Get().GetDefaultOutputDevice();

  NGL_OUT(_T("Default output: %ls\n"), mpOutAudioDevice->GetName().GetChars());

  std::vector<uint32> InputChannels;
  std::vector<uint32> OutputChannels;
  OutputChannels.push_back(0);
  OutputChannels.push_back(1);
  
  bool res = mpOutAudioDevice->Open(InputChannels, OutputChannels, mSampleRate, mBufferSize, nuiMakeDelegate(this, &nuiAudioEngine::ProcessAudioOutput));
  return res;
}


void nuiAudioEngine::ProcessAudioOutput(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames)
{
  std::vector<nuiVoice*> voicesToAdd;
  std::vector<nuiVoice*> voicesToRemove;
  mCs.Lock();
  voicesToAdd = mAddVoicesQueue;
  mAddVoicesQueue.clear();
  
  voicesToRemove = mRemoveVoicesQueue;
  mRemoveVoicesQueue.clear();
  mCs.Unlock();
  
  for (uint32 i = 0; i < voicesToRemove.size(); i++)
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
    }
  }
  
  for (uint32 i = 0; i < voicesToAdd.size(); i++)
    mVoices.push_back(voicesToAdd[i]);
  
  
  uint32 i = 0;
  for (i = 0; i < rOutput.size(); i++)
    memset(&rOutput[i][0], 0, sizeof(float) * SampleFrames);
  
  for (uint32 i = 0 ; i < mVoices.size(); i++)
  {
    nuiVoice* pVoice = mVoices[i];
    pVoice->Process(rOutput, SampleFrames);
  }

  std::vector<nuiVoice*>::iterator it = mVoices.begin();
  std::vector<nuiVoice*>::iterator end = mVoices.end();
  while (it != end)
  {
    nuiVoice* pVoice = *it;
    if (pVoice->IsDone() == pVoice->GetRefCount() == 1)
    {
      mVoices.erase(it++);
      pVoice->Release();
      continue;
    }
      
    ++it;
  }
}

nuiVoice* nuiAudioEngine::PlaySound(const nglPath& path)
{
  nuiSound* pSound = nuiSoundManager::Instance.GetSound(path);  
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
