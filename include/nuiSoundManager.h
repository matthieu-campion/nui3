/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSound.h"

class nuiSynthSound;

class nuiSoundManager
{
public:  
  static nuiSoundManager Instance;
  
  nuiSound* GetSound(const nglPath& rPath, nuiSound::Type type = nuiSound::eStream);
  nuiSynthSound* GetSynthSound();
  
private:
  friend class nuiSound;
  nuiSoundManager();
  virtual ~nuiSoundManager();
  
  void RemoveSound(nuiSound* pSound);
  void RemoveSound(const nglPath& rPath, nuiSound::Type type); ///< Remove the given sound from the manager without deleting it (this method is called when the Sound instance is destroyed)
  nglString GetStringID(const nglPath& rPath, nuiSound::Type type);
  
  typedef std::map<nglString, nuiSound*> SoundMap;
  SoundMap mSounds;
  
  std::list<nuiSynthSound*> mSynthSounds;
  
};