/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglPath.h"
#include "nglFile.h"
#include "nglCriticalSection.h"
#include "nuiRefCount.h"

class nglIStream;
class nglIOStream;

class nglVolume : public nuiRefCount
{
public:
  nglVolume(const nglString& rVolumeName, const nglPath& rPath, const nglString& rComment, nglPathVolume::VolumeFlags Flags, nglPathVolume::MediaType rType);
  virtual ~nglVolume();
  
  bool IsValid() const;
  const nglString& GetName() const;
  const nglPathVolume& GetVolumeInfo() const;
  
  virtual bool GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo) = 0;
  virtual bool MakeDirectory(const nglPath& rPath) = 0;
  virtual bool Delete(const nglPath& rPathToDelete) = 0;
  virtual bool Move(const nglPath& rSource, const nglPath& rPathTarget) = 0;
  virtual nglIStream* OpenRead(const nglPath& rPath) = 0;
  virtual nglIOStream* OpenWrite(const nglPath& rPath, bool OverWrite) = 0;
  virtual bool GetChildren(const nglPath& rPath, std::list<nglPath>* pChildren) = 0;
  
  static void Mount(nglVolume* pVolume);
  static void Unmount(nglVolume* pVolume);
  static void UnmountAll();
  static nglVolume* GetVolume(const nglString& rName);

  static void GetVolumes(std::list<nglPathVolume>& rVolumes, uint64 Flags);

protected:
  friend class nglPath;
  
  void SetName(const nglString& rName);
  
  nglString mName;
  nglPathVolume mInfos;
  
  void SetValid(bool set);
  
  bool mValid;

  typedef std::map<nglString, nglVolume*> VolumeMap;
  static VolumeMap mVolumes;  
  static nglCriticalSection mCS;
};
