/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglIMemory.h"



class nuiNativeResource : public nglIMemory
{
public:
  nuiNativeResource(const nglPath& rPath);
  ~nuiNativeResource();
  
  bool IsValid() const;

  static bool GetResourcesList(std::vector<nglPath>& rResources);

private:
  
  bool mValid; 
};
