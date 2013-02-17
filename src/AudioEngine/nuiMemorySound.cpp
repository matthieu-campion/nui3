/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiMemorySound::nuiMemorySound(const nglPath& rPath)
: mLength(0),
  mPath(rPath)
{
  mType = eMemory;
  LoadSamples();
  mID = nuiSound::GetStringID(rPath, mType);
}

nuiMemorySound::nuiMemorySound(const nglString& rSoundID, nglIStream* pStream)
: mLength(0)
{
  mType = eMemory;
  LoadSamples(pStream);
  mID = rSoundID;
}

nuiMemorySound::~nuiMemorySound()
{
  for (int32 c = 0; c < mSamples.size(); c++)
    delete[] mSamples[c];
}

bool nuiMemorySound::LoadSamples(nglIStream* pSStream)
{  
  nglIStream* pStream = pSStream;
  
  if (!mPath.Exists() && !pStream)
  {
    NGL_OUT("nuiMemorySound: file '%s' does not exist\n", mPath.GetPathName().GetChars());
    return false;
  }
  
  if (!pStream)
  {
    pStream = mPath.OpenRead();
  }

  if (!pStream)
  {
    NGL_OUT("nuiMemorySound: stream '%s' can't be open\n", mPath.GetPathName().GetChars());
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
        NGL_OUT(_T("Can't load this audio file: %s (reader can't be created)\n"), mPath.GetNodeName().GetChars());
        delete pReader;
        if (!pSStream)
          delete pStream;
        return false;
      }
    }
  }
  
  int32 length = info.GetSampleFrames();
  int32 channels = info.GetChannels();
  std::vector<void*> temp;
  for (int32 c = 0; c < channels; c++)
  {
    float* pBuffer = new float[length];
    mSamples.push_back(pBuffer);
    
    temp.push_back((void*)mSamples[c]);
  }
  
  mLength = pReader->ReadDE(temp, length, eSampleFloat32);
  delete pReader;
  if (!pSStream)
    delete pStream;
}

nuiVoice* nuiMemorySound::GetVoiceInternal()
{
  nuiVoice* pVoice = new nuiMemoryVoice(this);
  return pVoice;
}

int32 nuiMemorySound::ReadSamples(const std::vector<float*>& rBuffers, int64 position, int32 SampleFrames)
{
  if (position >= mLength)
    return 0;
  
  int32 todo = MIN(SampleFrames, mLength - position);
  for (int32 c = 0; c < rBuffers.size(); c++)
  {
    int32 inChannel = c < mSamples.size() ? c : (mSamples.size() - 1);
    memcpy(rBuffers[c], mSamples[inChannel] + position, todo * sizeof(float));
  }
  
  return todo;
}

int32 nuiMemorySound::GetSampleFrames() const
{
  return mLength;
}

int32 nuiMemorySound::GetChannels()const
{
  return mSamples.size();
}

