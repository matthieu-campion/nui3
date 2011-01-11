/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiSampleReader.h"

class nuiVoice;
class nuiSampleReader;

class nuiSound : public nuiRefCount
{
public:
  nuiSound(const nglPath& rPath);
  virtual ~nuiSound();
  
  const nglPath& GetPath() const;
  
  virtual nuiVoice* GetVoice(); // get an alredy acquired Voice object
  
private:
    nglPath mInPath;
};

