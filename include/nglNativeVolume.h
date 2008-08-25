/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglVolume.h"

class nglNativeVolume : public nglVolume
{
public:
  nglNativeVolume(const nglString& rVolumeName, const nglPathVolume& rPathVolume);
  nglNativeVolume(const nglString& rVolumeName, const nglPath& rSourcePath);
  virtual ~nglNativeVolume();
  
  virtual bool GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo);
  virtual bool MakeDirectory(const nglPath& rPath);
  virtual bool Delete(const nglPath& rPathToDelete);
  virtual bool Move(const nglPath& rSource, const nglPath& rPathTarget);
  virtual nglIStream* OpenRead(const nglPath& rPath);
  virtual nglIOStream* OpenWrite(const nglPath& rPath, bool OverWrite);
  virtual bool GetChildren(const nglPath& rPath, std::list<nglPath>* pChildren);
};
