/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
#include "nui.h"
#include "nuiSampleReader.h"
#include "nuiChunksDefinitions.h"

nuiSampleReader::nuiSampleReader(nglIStream& rStream):
mrStream(rStream),
mInitialized(false),
mPosition(0)
{
}

nuiSampleReader::nuiSampleReader(const nuiSampleReader& rReader, nglIStream& rStream):
mrStream(rStream),
mInitialized(rReader.mInitialized),
mPosition(rReader.mPosition),
mInfo(rReader.mInfo)
{
}

nuiSampleReader::~nuiSampleReader()
{
}

bool nuiSampleReader::Init()
{
  mInitialized = ReadInfo();
  if (mInitialized)
    SetPosition(0);
  
  return mInitialized;
}

uint32 nuiSampleReader::GetPosition() const
{
  return mPosition;
}

void nuiSampleReader::SetPosition(uint32 position)
{
  NGL_ASSERT(mInitialized);
  mPosition = position;
}

bool nuiSampleReader::GetInfo(nuiSampleInfo& rInfo) const
{
  if (!mInitialized)
  {
    if (!const_cast<nuiSampleReader*>(this)->Init()) // This is a hack but we have to do it if we want to keep on doing lazy inits
      return false;
  }
  
  rInfo = mInfo;
  return true;
}

bool nuiSampleReader::BytesToSampleFrames(uint64 inBytes, uint64& outSampleFrames) const
{
  NGL_ASSERT(mInitialized);
  outSampleFrames = inBytes * 8 / (mInfo.GetChannels() * mInfo.GetBitsPerSample());
  return true;
}

bool nuiSampleReader::SampleFramesToBytes(uint64 inSampleFrames, uint64& outBytes) const
{
  NGL_ASSERT(mInitialized);
  outBytes = inSampleFrames * mInfo.GetChannels() * mInfo.GetBitsPerSample() / 8;
  return true;
}
