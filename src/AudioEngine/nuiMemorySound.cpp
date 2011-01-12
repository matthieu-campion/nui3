/*
 *  nuiMemorySound.cpp
 *  nui3
 *
 *  Created by  mat on 1/12/11.
 *  Copyright 2011 libNUI. All rights reserved.
 *
 */

#include "nuiMemorySound.h"
#include "nuiWaveReader.h"
#include "nuiAiffReader.h"
#include "nuiAudioDecoder.h"

#include "nuiMemoryVoice.h"

nuiMemorySound::nuiMemorySound(const nglPath& rPath)
: nuiSound(rPath),
  mLength(0)
{
  mType = eMemory;
  LoadSamples();
}

nuiMemorySound::~nuiMemorySound()
{
  for (uint32 c = 0; c < mSamples.size(); c++)
    delete[] mSamples[c];
}

bool nuiMemorySound::LoadSamples()
{
  if (!mInPath.Exists())
    return false;
  
  nglIStream* pStream = mInPath.OpenRead();
  if (!pStream)
    return false;
  
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
        NGL_OUT(_T("Can't load this audio file: %ls (reader can't be created)\n"), mInPath.GetNodeName().GetChars());
        delete pReader;
        delete pStream;
        return false;
      }
    }
  }
  
  uint32 length = info.GetSampleFrames();
  uint32 channels = info.GetChannels();
  std::vector<void*> temp;
  for (uint32 c = 0; c < channels; c++)
  {
    float* pBuffer = new float[length];
    mSamples.push_back(pBuffer);
    
    temp.push_back((void*)mSamples[c]);
  }
  
  mLength = pReader->ReadDE(temp, length, eSampleFloat32);
}

nuiVoice* nuiMemorySound::GetVoice()
{
  nuiVoice* pVoice = new nuiMemoryVoice(this);
  return pVoice;
}

uint32 nuiMemorySound::ReadSamples(const std::vector<float*>& rBuffers, int64 position, uint32 SampleFrames)
{
  if (position >= mLength)
    return 0;
  
  uint32 todo = MIN(SampleFrames, mLength - position);
  for (uint32 c = 0; c < rBuffers.size(); c++)
  {
    uint32 inChannel = c < mSamples.size() ? c : (mSamples.size() - 1);
    memcpy(rBuffers[c], mSamples[inChannel] + position, todo * sizeof(float));
  }
  
  return todo;
}

uint32 nuiMemorySound::GetSampleFrames() const
{
  return mLength;
}

uint32 nuiMemorySound::GetChannels()const
{
  return mSamples.size();
}

