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
  
  enum Type 
  {
    eStream = 0,
    eMemory,
    eUnknown
  };
  
  const nglPath& GetPath() const;
  
  virtual nuiVoice* GetVoice() = 0;
  
protected:
  nuiSound(const nglPath& rPath);
  virtual ~nuiSound();
  
  nglPath mInPath;
  Type mType;  
};

