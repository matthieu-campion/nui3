/*
 *  AudioEngine.cpp
 *  AudioPlayer
 *
 *  Created by mat on 9/24/09.
 *  Copyright 2009 Mxp4. All rights reserved.
 *
 */

#include "AudioEngine.h"


AudioEngine::AudioEngine(double SampleRate, uint32 BufferSize)
: mpAudioDevice(NULL),
  mpIStream(NULL),
  mpReader(NULL),
  mPlaying(false),
  mSampleRate(SampleRate),
  mBufferSize(BufferSize),
  mGain(1.f),
  mMute(false),
  mCs(_T("AudioEngineCriticalSection"))
{  
  if (SetObjectClass(_T("AudioEngine")))
  {
    InitAttributes();
  }
  
  // init audio device
  nuiAudioDeviceManager& rManager = nuiAudioDeviceManager::Get();
  mpAudioDevice = rManager.GetDefaultOutputDevice();
  
  std::vector<uint32> InputChannels;  
  std::vector<uint32> OutputChannels;
  OutputChannels.push_back(0);
  OutputChannels.push_back(1);
  
  mpAudioDevice->Open(InputChannels, OutputChannels, SampleRate, BufferSize, nuiMakeDelegate(this, &AudioEngine::Process));
}

AudioEngine::~AudioEngine()
{
  if (mpAudioDevice)
    delete mpAudioDevice;
  
  if (mpIStream)
    delete mpIStream;
  if (mpReader)
    delete mpReader;
}

void AudioEngine::InitAttributes()
{
  AddAttribute(new nuiAttribute<uint64>
               (nglString(_T("position")), nuiUnitCustom,
                nuiMakeDelegate(this, &AudioEngine::GetPosition),
                nuiMakeDelegate(this, &AudioEngine::SetPosition)));
  
  AddAttribute(new nuiAttribute<uint64>
               (nglString(_T("length")), nuiUnitCustom,
                nuiMakeDelegate(this, &AudioEngine::GetSampleFrames)));
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("gain")), nuiUnitRangeKnob,
                nuiMakeDelegate(this, &AudioEngine::GetGain),
                nuiMakeDelegate(this, &AudioEngine::SetGain)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("mute")), nuiUnitOnOff,
                nuiMakeDelegate(this, &AudioEngine::IsMute),
                nuiMakeDelegate(this, &AudioEngine::SetMute)));
}

void AudioEngine::Process(const std::vector<const float*>& rInput, const std::vector<float*>& rOutput, uint32 SampleFrames)
{
  uint32 channels = rOutput.size();
  
  if (!mPlaying || !mpReader)
  {
    for (uint32 c = 0; c < channels; c++)
    {
      memset(rOutput[c], 0, SampleFrames * sizeof(float));
    }
    return;
  }
    
  
  std::vector<float*> buffers;
  for (uint32 c = 0; c < channels; c++)
  {
    buffers.push_back(new float[SampleFrames]);
    memset(buffers[c], 0, SampleFrames * sizeof(float));
  }
  
  uint32 SrcChannels = mInfo.GetChannels();
  float* pTemp = NULL;
  
  std::vector<void*> temp;
  
  if (channels == SrcChannels)
  {
    for (uint32 c = 0; c < channels; c++)
    {
      temp.push_back((void*)(buffers[c]));
    }
  }
  else if (channels == 2 && SrcChannels == 1)
  {
    pTemp = new float[SampleFrames];
    temp.push_back((void*)pTemp);
  }
  else
  {
    NGL_ASSERT(0);
  }
  
  nglCriticalSectionGuard guard(mCs);
  uint32 read = mpReader->ReadDE(temp, SampleFrames, eSampleFloat32);
  if (read == 0)
    NGL_OUT(_T("audio file (%ls) finished\n"), mInPath.GetNodeName().GetChars());
  
  if (pTemp)
  {
    for (uint32 c = 0; c < channels; c++)
    {
      memcpy(buffers[c], pTemp, SampleFrames * sizeof(float));
    }
    delete pTemp;
  }
  
  if (!mMute && mGain >= 0.f)
  {
    for (uint32 c= 0; c < channels; c++)
    {
      for (uint32 i = 0; i < SampleFrames; i++)
      {
        rOutput[c][i] = buffers[c][i] * mGain;
      }
    }
  }
  else
  {
    for (uint32 c = 0; c < channels; c++)
    {
      memset(rOutput[c], 0, SampleFrames * sizeof(float));
    }
  }
}

bool AudioEngine::LoadFile(nglPath FilePath)
{
  if (!FilePath.Exists())
  {
    NGL_OUT(_T("Can't load this audio file: %ls\n"), FilePath.GetNodeName().GetChars());
    return false;
  }
  
  nglIStream* pStream = FilePath.OpenRead();
  if (!pStream)
  {
    NGL_OUT(_T("Can't load this audio file: %ls\n"), FilePath.GetNodeName().GetChars());
    return false;
  }
  
  nuiSampleReader* pReader = new nuiAudioDecoder(*pStream);
  nuiSampleInfo info;
  bool valid = pReader->GetInfo(info);
  if (!valid)
  {
    NGL_OUT(_T("Can't load this audio file: %ls\n"), FilePath.GetNodeName().GetChars());
    delete pReader;
    delete pStream;
    return false;
  }
  
  nglCriticalSectionGuard guard(mCs);
  
  if (mpIStream)
    delete mpIStream;
  if (mpReader)
    delete mpReader;
  
  mpIStream = pStream;
  mpReader = pReader;  
  mInPath = FilePath;
  mInfo = info;
  NGL_OUT(_T("audio file loaded: %ls\n"), mInPath.GetNodeName().GetChars());
  return true;
}

void AudioEngine::Play()
{
  if (!mpIStream)
    mPlaying = false;
  else
    mPlaying = true;
}

void AudioEngine::Pause()
{
  mPlaying = false;
}

bool AudioEngine::IsPlaying() const
{
  return mPlaying;
}

uint64 AudioEngine::GetSampleFrames() const
{
  if (!mpReader)
    return 0;
  
  uint32 sampleframes = mInfo.GetSampleFrames();
  return sampleframes;
}

uint64 AudioEngine::GetPosition() const
{
  if (!mpReader)
  {
    return 0;
  }
  uint32 pos = mpReader->GetPosition();
  return pos;
}

void AudioEngine::SetPosition(uint64 position)
{
  nglCriticalSectionGuard guard(mCs);
  mpReader->SetPosition(position);
}

double AudioEngine::GetSampleRate() const
{
  return mSampleRate;
}

float AudioEngine::GetGain()
{
  return mGain;
}

void AudioEngine::SetGain(float gain)
{
  mGain = gain;
}

bool AudioEngine::IsMute()
{
  return mMute;
}

void AudioEngine::SetMute(bool mute)
{
  mMute = mute;
}