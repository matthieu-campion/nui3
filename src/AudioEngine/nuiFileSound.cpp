/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiFileSound::nuiFileSound(const nglPath& rPath)
: mPath(rPath)
{
  mType = eStream;
  mID = nuiSound::GetStringID(rPath, mType);
}
nuiFileSound::~nuiFileSound()
{
  
}

nuiVoice* nuiFileSound::GetVoiceInternal()
{
  nuiVoice* pVoice = new nuiFileVoice(this);
  return pVoice;
}

const nglPath& nuiFileSound::GetPath() const
{
  return mPath;
}