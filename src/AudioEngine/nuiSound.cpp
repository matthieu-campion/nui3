/*
 *  nuiSound.cpp
 *
 *  Copyright 2010 blumug. All rights reserved.
 *
 */

#include "nuiSound.h"

nuiSound::nuiSound(const nglPath& rPath)
{
  mInPath = rPath;
}
  
nuiSound::~nuiSound()
{
  nuiSoundManager::Instance.RemoveSound(mInPath);
  NGL_OUT(_T("nuiSound destr. '%ls'\n"), mInPath.GetPathName().GetChars());
}

const nglPath& nuiSound::GetPath() const
{
  return mInPath;
}

nuiVoice* nuiSound::GetVoice()
{
  nuiVoice* pVoice = new nuiVoice(this);
  return pVoice;
}