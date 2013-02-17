/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nglVolume::nglVolume(const nglString& rVolumeName, const nglPath& rPath, const nglString& rComment, nglPathVolume::VolumeFlags Flags, nglPathVolume::MediaType rType)
{
  mName = rVolumeName;
  mInfos.mPath = rPath;
  mInfos.mComment = rComment;
  mInfos.mFlags = Flags;
  mInfos.mType = rType;
}

nglVolume::~nglVolume()
{
}

void nglVolume::UnmountAll()
{
  nglCriticalSectionGuard guard(mCS);
  
  VolumeMap::iterator it;
  for(it  = mVolumes.begin(); it != mVolumes.end();)
  {
	  delete (*it).second;
	  mVolumes.erase(it++);
  }
}

const nglPathVolume& nglVolume::GetVolumeInfo() const
{
  return mInfos;
}

const nglString& nglVolume::GetName() const
{
  return mName;
}

void nglVolume::SetName(const nglString& rName)
{
  mName = rName;
}

bool nglVolume::IsValid() const
{
  return mValid;
}

void nglVolume::SetValid(bool set)
{
  mValid = set;
}

///////////////////////////////////
// Static methods:
void nglVolume::Mount(nglVolume* pVolume)
{
  nglCriticalSectionGuard guard(mCS);
  nglString name(pVolume->GetName());
  
  VolumeMap::const_iterator it = mVolumes.find(name);
  uint32 i = 2;
  while (it != mVolumes.end())
  {
    name.CFormat(_T("%s (%d)"), pVolume->GetName().GetChars(), i);
    i++;
    it = mVolumes.find(name);
  }
  pVolume->SetName(name);
  
  mVolumes[name] = pVolume;
}

void nglVolume::Unmount(nglVolume* pVolume)
{
  nglCriticalSectionGuard guard(mCS);
  
  VolumeMap::iterator it = mVolumes.find(pVolume->GetName());
  if (it == mVolumes.end())
    return;
  mVolumes.erase(it);
  delete pVolume;
}

nglVolume* nglVolume::GetVolume(const nglString& rName)
{
  nglCriticalSectionGuard guard(mCS);
  VolumeMap::const_iterator it = mVolumes.find(rName);
  if (it == mVolumes.end())
    return NULL;
  
  return it->second;
}

void nglVolume::GetVolumes(std::list<nglPathVolume>& rVolumes, uint64 Flags)
{
  nglCriticalSectionGuard guard(mCS);
  
  VolumeMap::const_iterator it = mVolumes.begin();
  VolumeMap::const_iterator end = mVolumes.end();

  while (it != end)
  {
    nglVolume* pVolume = it->second;
    if (pVolume->GetVolumeInfo().mFlags == Flags)
      rVolumes.push_back(pVolume->GetVolumeInfo());
    
    ++it;
  }
}


nglVolume::VolumeMap nglVolume::mVolumes;
nglCriticalSection nglVolume::mCS(_T("nglVolumeCS"));

/// deprecated
bool nglVolume::GetChildren(const nglPath& rPath, std::list<nglPath>* pChildren)
{
  return GetChildren(rPath, *pChildren);
}

