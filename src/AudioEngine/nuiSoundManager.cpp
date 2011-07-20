/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSoundManager.h"
#include "nuiFileSound.h"
#include "nuiMemorySound.h"
#include "nuiSynthSound.h"

nuiSoundManager nuiSoundManager::Instance;


nuiSound* nuiSoundManager::GetSound(const nglPath& rPath, nuiSound::Type type)
{
  nglCriticalSectionGuard gaurd(mCS);
  nglString ID = nuiSound::GetStringID(rPath, type);
  SoundMap::iterator it = mSounds.find(ID);
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
  
  mSounds[ID] = pSound;
  return pSound;
}

nuiSound* nuiSoundManager::GetSound(const nglString& rSoundID, nglIStream* pStream)
{
  nglCriticalSectionGuard gaurd(mCS);
  nglString ID = rSoundID;
  SoundMap::iterator it = mSounds.find(ID);
  if (it != mSounds.end())
  {
    nuiSound* pSound = it->second;
    delete pStream;
    return pSound;
  }
  
  nuiSound* pSound = NULL;
  pSound = new nuiMemorySound(rSoundID, pStream);

  mSounds[ID] = pSound;
  return pSound;
}


nuiSynthSound* nuiSoundManager::GetSynthSound()
{
  nglCriticalSectionGuard gaurd(mCS);
  nuiSynthSound* pSound = new nuiSynthSound();
  nglString ID = pSound->GetID();
  mSounds[ID] = pSound;
  
  return pSound;
}

nuiSoundManager::nuiSoundManager()
{
  
}

nuiSoundManager::~nuiSoundManager()
{
}

void nuiSoundManager::RemoveSound(nuiSound* pSound)
{
  NGL_ASSERT(pSound->GetRefCount() == 0); // RemoveSound is called by nuiSound destructor 
  
  nglCriticalSectionGuard guard(mCS);
  nglString ID = pSound->GetID();
  SoundMap::iterator it = mSounds.find(ID);
  if (it != mSounds.end())
  {
    mSounds.erase(it);
  }
}
