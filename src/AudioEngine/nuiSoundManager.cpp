/*
 *  nuiSoundManager.cpp
 *  LPCTest
 *
 *  Created by  mat on 1/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "nuiSoundManager.h"
#include "nuiFileSound.h"
#include "nuiMemorySound.h"

#define STREAM_SUFFIX _T("_stream");
#define MEMORY_SUFFIX _T("_memory");

nuiSoundManager nuiSoundManager::Instance;


nuiSound* nuiSoundManager::GetSound(const nglPath& rPath, nuiSound::Type type)
{
  nglString str = GetStringID(rPath, type);
  SoundMap::iterator it = mSounds.find(str);
  if (it != mSounds.end())
  {
    nuiSound* pSound = it->second;
    return pSound;
  }
  
  nuiSound* pSound = NULL;
  
  switch (type) 
  {
    case nuiSound::eStream:
      pSound = new nuiFileSound(rPath);
      break;
      
    case nuiSound::eMemory:
      pSound = new nuiMemorySound(rPath);
      break;
      
    default:
      NGL_ASSERT(0);
      break;
  }
  
  mSounds[str] = pSound;
  return pSound;
}

nuiSoundManager::nuiSoundManager()
{
  
}

nuiSoundManager::~nuiSoundManager()
{
}

void nuiSoundManager::RemoveSound(const nglPath& rPath, nuiSound::Type type)
{
  nglString str = GetStringID(rPath, type);
  SoundMap::iterator it = mSounds.find(str);
  if (it != mSounds.end())
  {
    NGL_ASSERT(it->second->GetRefCount() == 0);
    mSounds.erase(it);
  }
}

nglString nuiSoundManager::GetStringID(const nglPath& rPath, nuiSound::Type type)
{
  nglString str = rPath.GetPathName();
  switch (type) 
  {
    case nuiSound::eStream:
      str += STREAM_SUFFIX;
      break;
      
    case nuiSound::eMemory:
      str += MEMORY_SUFFIX;
      break;
      
    default:
      break;
  }
  return str;
}