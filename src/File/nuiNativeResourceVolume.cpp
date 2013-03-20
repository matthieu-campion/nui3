/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include "nuiStopWatch.h"

#if (!defined _UIKIT_)
nuiNativeResourceVolume::nuiNativeResourceVolume()
: nglVolume(_T("rsrc"), nglPath(), _T("Native resources"), nglPathVolume::ReadOnly, nglPathVolume::eTypeUnknown)
{
  //App->GetLog().SetLevel(_T("StopWatch"), 100);
  
  nuiStopWatch watch(_T("nuiNativeResourceVolume creation"));
  std::vector<nglPath> resources;
  nuiNativeResource::GetResourcesList(resources);
  watch.AddIntermediate(_T("GetResourcesList"));
                     
  //NGL_OUT(_T("Found %d resources in this executable\n"), resources.size());
  for (size_t i = 0; i < resources.size(); i++)
  {
    nglPath tmp(resources[i]);
    nglPath ref;
    while (tmp != ref && tmp != nglPath(_T("/")))
    {
      ref = tmp;
      tmp = tmp.GetParent();
			nglString nodename(ref.GetNodeName());
			if (!nodename.IsEmpty())
      {
        mItems[tmp].insert(nodename);
        //NGL_OUT(_T("mItems[%s] <- '%s'\n"), tmp.GetChars(), nodename.GetChars());
      }
		}
  }
}

nuiNativeResourceVolume::~nuiNativeResourceVolume()
{
  
}

bool nuiNativeResourceVolume::GetPathInfo(const nglPath& rPath, nglPathInfo& rInfo)
{
	nglString tmp = rPath.GetVolumeLessPath();
  nglPath path(tmp);
	tmp.TrimLeft(L'/');
	nglPath trimmed(tmp);
  
  if (!path.GetPathName().Compare(_T("/")))
  {
    // we act like this is a folder
    rInfo.Exists = true;      ///< Node existence. All other fields are invalid if set to false.
    rInfo.IsLeaf = false;      ///< Leaf (file or assimilable) or non-leaf (folder)
    rInfo.CanRead = true;     ///< True if the file (leaf) can be read or the folder (non-leaf) can be traversed and its content listed
    rInfo.CanWrite = false;    ///< True if the file (leaf) can be written to or a new node can be created in this folder (non-leaf)
    rInfo.LastAccess = 0;  ///< nglTime stamp of last access (read or exec)
    rInfo.LastMod = 0;     ///< nglTime stamp of last modification (write)
    rInfo.Size = 0;      ///< If the node is a leaf, size in bytes. If non-leaf, always zero.
    rInfo.Visible = true; ///< Always visible...
    
    return true;  
  }

  std::map<nglPath, std::set<nglString> >::const_iterator it = mItems.find(trimmed);
	
  if (it != mItems.end())
  {
    // This is a folder
    rInfo.Exists = true;      ///< Node existence. All other fields are invalid if set to false.
    rInfo.IsLeaf = false;      ///< Leaf (file or assimilable) or non-leaf (folder)
    rInfo.CanRead = true;     ///< True if the file (leaf) can be read or the folder (non-leaf) can be traversed and its content listed
    rInfo.CanWrite = false;    ///< True if the file (leaf) can be written to or a new node can be created in this folder (non-leaf)
    rInfo.LastAccess = 0;  ///< nglTime stamp of last access (read or exec)
    rInfo.LastMod = 0;     ///< nglTime stamp of last modification (write)
    rInfo.Size = 0;      ///< If the node is a leaf, size in bytes. If non-leaf, always zero.
    rInfo.Visible = true; ///< Always visible...
    
    return true;
  }
  
  nuiNativeResource* pRes = new nuiNativeResource(path);
  if (pRes && pRes->IsValid())
  {
    // This is a file
    rInfo.Exists = true;      ///< Node existence. All other fields are invalid if set to false.
    rInfo.IsLeaf = true;      ///< Leaf (file or assimilable) or non-leaf (folder)
    rInfo.CanRead = true;     ///< True if the file (leaf) can be read or the folder (non-leaf) can be traversed and its content listed
    rInfo.CanWrite = false;    ///< True if the file (leaf) can be written to or a new node can be created in this folder (non-leaf)
    rInfo.LastAccess = 0;  ///< nglTime stamp of last access (read or exec)
    rInfo.LastMod = 0;     ///< nglTime stamp of last modification (write)
    rInfo.Size = pRes->Available();      ///< If the node is a leaf, size in bytes. If non-leaf, always zero.
    rInfo.Visible = true; ///< Always visible...
    
    delete pRes;
    return true;
  }
  
  delete pRes;
  
  return false;
}

bool nuiNativeResourceVolume::MakeDirectory(const nglPath& rPath)
{
  // We can't modify this kind of volume
  return false;
}

bool nuiNativeResourceVolume::Delete(const nglPath& rPathToDelete)
{
  // We can't modify this kind of volume
  return false;
}

bool nuiNativeResourceVolume::Move(const nglPath& rSource, const nglPath& rPathTarget)
{
  // We can't modify this kind of volume
  return false;
}

nglIStream* nuiNativeResourceVolume::OpenRead(const nglPath& rPath)
{
  nglPath path(rPath.GetVolumeLessPath());
  nuiNativeResource* pRes = new nuiNativeResource(path);
  if (pRes && pRes->IsValid())
  {
    return pRes;
  }
  delete pRes;
  return NULL;
}

nglIOStream* nuiNativeResourceVolume::OpenWrite(const nglPath& rPath, bool OverWrite)
{
  // We can't modify this kind of volume
  return NULL;
}

bool nuiNativeResourceVolume::GetChildren(const nglPath& rPath, std::list<nglPath>& rChildren)
{
  nglString p(rPath.GetVolumeLessPath());
  p.TrimLeft(_T('/'));
  //wprintf(_T("trimed path '%s'\n"), p.GetChars());
  nglPath path(p);
  //wprintf(_T("GetChildren(\"%s\") [%s] [%s]\n"), rPath.GetChars(), path.GetChars(), p.GetChars());
  
  std::map<nglPath, std::set<nglString> >::const_iterator fit = mItems.find(path);
  
  if (fit == mItems.end())
  {
    // The path has to point to a folder, not a file
    return false;
  }
  
  const std::set<nglString>& rChildrenSet(fit->second);
  std::set<nglString>::const_iterator it = rChildrenSet.begin();
  std::set<nglString>::const_iterator end = rChildrenSet.end();
  
  while (it != end)
  {
    nglPath p(rPath);
    p += *it;
    rChildren.push_back(p);
    
    ++it;
  }
  return true;
}
#else
nuiNativeResourceVolume::nuiNativeResourceVolume()
: nglNativeVolume(_T("rsrc"), nglPathVolume(nuiGetNativeResourcePath(), _T("Native resources"), nglPathVolume::ReadOnly, nglPathVolume::eTypeUnknown))
{
}

nuiNativeResourceVolume::~nuiNativeResourceVolume()
{
  
}

bool nuiNativeResourceVolume::MakeDirectory(const nglPath& rPath)
{
  // We can't modify this kind of volume
  return false;
}

bool nuiNativeResourceVolume::Delete(const nglPath& rPathToDelete)
{
  // We can't modify this kind of volume
  return false;
}

bool nuiNativeResourceVolume::Move(const nglPath& rSource, const nglPath& rPathTarget)
{
  // We can't modify this kind of volume
  return false;
}

nglIOStream* nuiNativeResourceVolume::OpenWrite(const nglPath& rPath, bool OverWrite)
{
  // We can't modify this kind of volume
  return NULL;
}
#endif