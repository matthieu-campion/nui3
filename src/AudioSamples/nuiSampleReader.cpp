/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */
 
#include "nui.h"

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

int64 nuiSampleReader::GetPosition() const
{
  return mPosition;
}

void nuiSampleReader::SetPosition(int64 position)
{
  NGL_ASSERT(mInitialized);
  mPosition = MIN(position, mInfo.GetSampleFrames() - 1);
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

int64 nuiSampleReader::BytesToSampleFrames(int64 inBytes) const
{
  NGL_ASSERT(mInitialized);
  int64 outSampleFrames = inBytes * 8 / (mInfo.GetChannels() * mInfo.GetBitsPerSample());
  return outSampleFrames;
}

int64 nuiSampleReader::SampleFramesToBytes(int64 inSampleFrames) const
{
  NGL_ASSERT(mInitialized);
  int64 outBytes = inSampleFrames * mInfo.GetChannels() * mInfo.GetBitsPerSample() / 8;
  return outBytes;
}
