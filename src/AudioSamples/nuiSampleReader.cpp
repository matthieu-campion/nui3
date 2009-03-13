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
mPosition(-1)
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

uint64 nuiSampleReader::BytesToSampleFrames(uint64 inBytes) const
{
  NGL_ASSERT(mInitialized);
  uint64 outSampleFrames = inBytes * 8 / (mInfo.GetChannels() * mInfo.GetBitsPerSample());
  return outSampleFrames;
}

uint64 nuiSampleReader::SampleFramesToBytes(uint64 inSampleFrames) const
{
  NGL_ASSERT(mInitialized);
  uint64 outBytes = inSampleFrames * mInfo.GetChannels() * mInfo.GetBitsPerSample() / 8;
  return outBytes;
}
