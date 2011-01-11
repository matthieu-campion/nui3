/*
 *  nuiSoundManager.cpp
 *  LPCTest
 *
 *  Created by  mat on 1/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "nuiSoundManager.h"

nuiSoundManager nuiSoundManager::Instance;


nuiSound* nuiSoundManager::GetSound(const nglPath& rPath)
{
  SoundMap::iterator it = mSounds.find(rPath);
  if (it != mSounds.end())
  {
    nuiSound* pSound = it->second;
    return pSound;
  }
  
  nuiSound* pSound = new nuiSound(rPath);
  mSounds[rPath] = pSound;
  return pSound;
}

nuiSoundManager::nuiSoundManager()
{
  
}

nuiSoundManager::~nuiSoundManager()
{
}

void nuiSoundManager::RemoveSound(const nglPath& rPath)
{
  SoundMap::iterator it = mSounds.find(rPath);
  if (it != mSounds.end())
  {
    NGL_ASSERT(it->second->GetRefCount() == 0);
    mSounds.erase(it);
  }
}