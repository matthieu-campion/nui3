/*
 *  nuiAudioDecoder.cpp
 *  nui3
 *
 *  Created by mat on 12/18/08.
 *  Copyright 2008 Mxp4. All rights reserved.
 *
 */

#include "nuiAudioDecoder.h"

nuiAudioDecoder::nuiAudioDecoder(nglIStream& rStream) :
  mInitialized(false),
  mrStream(rStream),
  mPosition(0),
  mpPrivate(NULL)
{
  mInitialized = Init();
}

nuiAudioDecoder::~nuiAudioDecoder()
{
  Clear();
}


bool nuiAudioDecoder::GetInfo(nuiSampleInfo& rInfo) const
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return false;
  
  rInfo = mInfo;
  return true;
}

const uint64 nuiAudioDecoder::GetPosition() const
{
  return mPosition;
}

bool nuiAudioDecoder::SetPosition(uint64 pos)
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return false;
 
  if (Seek(pos))
  {
    mPosition = pos;
    return true;
  }
  else
    return false;
}

bool nuiAudioDecoder::BytesToSampleFrames(uint64 inBytes, uint64& outSampleFrames) const
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return false;
  
  outSampleFrames = inBytes * 8 / (mInfo.GetChannels() * mInfo.GetBitsPerSample());
  return true;
}

bool nuiAudioDecoder::SampleFramesToBytes(uint64 inSampleFrames, uint64& outBytes) const
{
  NGL_ASSERT(mInitialized);
  if (!mInitialized)
    return false;
  
  outBytes = inSampleFrames * mInfo.GetChannels() * mInfo.GetBitsPerSample() / 8;
  return true;
}