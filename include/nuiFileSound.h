/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once
#include "nui.h"

class nuiFileSound : public nuiSound
{
public:
  friend class nuiSoundManager;
  
  virtual nuiVoice* GetVoice();
  
protected:
  nuiFileSound(const nglPath& rPath);
  virtual ~nuiFileSound();
  
};