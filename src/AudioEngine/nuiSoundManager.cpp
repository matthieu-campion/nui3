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

nuiSynthSound* nuiSoundManager::GetSynthSound()
{
  nuiSynthSound* pSound = new nuiSynthSound();
  mSynthSounds.push_back(pSound);
  
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
  
  if (pSound->mType == nuiSound::eStream)
    RemoveSound(((nuiFileSound*)pSound)->mPath, pSound->mType);
  else if (pSound->mType == nuiSound::eMemory)
    RemoveSound(((nuiMemorySound*)pSound)->mPath, pSound->mType);
  else if (pSound->mType == nuiSound::eSynth)
    mSynthSounds.remove((nuiSynthSound*)pSound);
    
}

void nuiSoundManager::RemoveSound(const nglPath& rPath, nuiSound::Type type)
{
  nglString str = GetStringID(rPath, type);
  SoundMap::iterator it = mSounds.find(str);
  if (it != mSounds.end())
  {
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
      NGL_ASSERT(0);
      break;
  }
  return str;
}