/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiFileSound.h"
#include "nuiFileVoice.h"

nuiFileSound::nuiFileSound(const nglPath& rPath)
: nuiSound(rPath)
{
  mType = eStream;
}
nuiFileSound::~nuiFileSound()
{
  
}

nuiVoice* nuiFileSound::GetVoice()
{
  nuiVoice* pVoice = new nuiFileVoice(this);
  return pVoice;
}