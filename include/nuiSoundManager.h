/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiSound;

class nuiSoundManager
{
public:
  
  static nuiSoundManager Instance;
  
  nuiSound* GetSound(const nglPath& rPath);
  
private:
  friend class nuiSound;
  nuiSoundManager();
  virtual ~nuiSoundManager();
  
  void RemoveSound(const nglPath& rPath); ///< Remove the given sound from the manager without deleting it (this method is called when the Sound instance is destroyed)
  
  
  typedef std::map<nglPath, nuiSound*> SoundMap;
  SoundMap mSounds;
  
};