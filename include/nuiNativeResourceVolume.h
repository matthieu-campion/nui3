
/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglVolume.h"

#pragma once

#if (!defined _UIKIT_)
class nuiNativeResourceVolume : public nglVolume
{
public:
  nuiNativeResourceVolume();
  virtual ~nuiNativeResourceVolume();
  
  virtual bool GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo);
  virtual bool MakeDirectory(const nglPath& rPath);
  virtual bool Delete(const nglPath& rPathToDelete);
  virtual bool Move(const nglPath& rSource, const nglPath& rPathTarget);
  virtual nglIStream* OpenRead(const nglPath& rPath);
  virtual nglIOStream* OpenWrite(const nglPath& rPath, bool OverWrite);
  virtual bool GetChildren(const nglPath& rPath, std::list<nglPath>& pChildren);
  
private:
  std::map<nglPath, std::set<nglString> > mItems;
};
#else
// Optimized version for UIKIT
#include "nglNativeVolume.h"

class nuiNativeResourceVolume : public nglNativeVolume
{
public:
  nuiNativeResourceVolume();
  virtual ~nuiNativeResourceVolume();
  
  virtual bool MakeDirectory(const nglPath& rPath);
  virtual bool Delete(const nglPath& rPathToDelete);
  virtual bool Move(const nglPath& rSource, const nglPath& rPathTarget);
  virtual nglIOStream* OpenWrite(const nglPath& rPath, bool OverWrite);
};
#endif