/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSound.h"

nuiSound::nuiSound(const nglPath& rPath)
: mInPath(rPath),
  mType(eUnknown)
{
}
  
nuiSound::~nuiSound()
{
  nuiSoundManager::Instance.RemoveSound(mInPath, mType);
  NGL_OUT(_T("nuiSound destr. '%ls'\n"), mInPath.GetPathName().GetChars());
}

const nglPath& nuiSound::GetPath() const
{
  return mInPath;
}