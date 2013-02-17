/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nglTime          nglPath::mMimeTypeStamp = nglTime();
nglPath::MimeMap nglPath::mMimeType;

#ifdef _CARBON_
bool nglPath::ResolveLink()
{
  if (!mPathName.IsNull())
    return false;

  bool res = false;

  FSRef ref;
  std::string str(mPathName.GetStdString());
  OSStatus status = FSPathMakeRef((const UInt8*)str.c_str(), &ref, NULL);
  if (status == noErr)
  {
    Boolean targetIsAFolder;
    Boolean wasAliased;

		OSErr err = ::FSResolveAliasFileWithMountFlags(&ref,  true, &targetIsAFolder, &wasAliased, 0);
    if ( (err == noErr) && (wasAliased) )
    {
      char buffer[1024];
      FSRefMakePath(&ref, (UInt8*)buffer, 1024);
      InternalSetPath(buffer);
      res = true;
    }
  }
  return res;
}

void nglPathVolume::UpdateVolumes(std::list<nglPathVolume>& rVolumes)
{
  OSErr    result;
  uint volumeIndex = 1;

  /* Call FSGetVolumeInfo in loop to get all volumes starting with the first */
  do
  {
    FSVolumeRefNum refnum = 0;
    result = FSGetVolumeInfo(kFSInvalidVolumeRefNum, volumeIndex, &refnum, kFSVolInfoNone, NULL, NULL, NULL);
    if ( noErr == result )
    {
      // Found a new volume.
      AddVolume(rVolumes, refnum);
      ++volumeIndex;
    }
  } while ( noErr == result );
}

static OSErr nglFSGetVolParms(
              FSVolumeRefNum volRefNum,
              UInt32 bufferSize,
              GetVolParmsInfoBuffer *volParmsInfo,
              UInt32 *actualInfoSize)
{
  OSErr      result;
  HParamBlockRec  pb;

  pb.ioParam.ioNamePtr = NULL;
  pb.ioParam.ioVRefNum = volRefNum;
  pb.ioParam.ioBuffer = (Ptr)volParmsInfo;
  pb.ioParam.ioReqCount = (SInt32)bufferSize;
  result = PBHGetVolParmsSync(&pb);

  /* return number of bytes the file system returned in volParmsInfo buffer */
  *actualInfoSize = (UInt32)pb.ioParam.ioActCount;

  return ( result );
}

#define GetVolParmsInfoExtendedAttributes(volParms) \
(((volParms)->vMVersion >= 3) ? (volParms)->vMExtendedAttributes : 0)

/* attribute bits supported by all versions of GetVolParmsInfoBuffer */
#define VolIsNetworkVolume(volParms) \
((volParms)->vMServerAdr != 0)
#define VolHasLimitFCBs(volParms) \
(((volParms)->vMAttrib & (1L << bLimitFCBs)) != 0)
#define VolHasLocalWList(volParms) \
(((volParms)->vMAttrib & (1L << bLocalWList)) != 0)
#define VolHasNoMiniFndr(volParms) \
(((volParms)->vMAttrib & (1L << bNoMiniFndr)) != 0)
#define VolHasNoVNEdit(volParms) \
(((volParms)->vMAttrib & (1L << bNoVNEdit)) != 0)
#define VolHasNoLclSync(volParms) \
(((volParms)->vMAttrib & (1L << bNoLclSync)) != 0)
#define VolHasTrshOffLine(volParms) \
(((volParms)->vMAttrib & (1L << bTrshOffLine)) != 0)
#define VolHasNoSwitchTo(volParms) \
(((volParms)->vMAttrib & (1L << bNoSwitchTo)) != 0)
#define VolHasNoDeskItems(volParms) \
(((volParms)->vMAttrib & (1L << bNoDeskItems)) != 0)
#define VolHasNoBootBlks(volParms) \
(((volParms)->vMAttrib & (1L << bNoBootBlks)) != 0)
#define VolHasAccessCntl(volParms) \
(((volParms)->vMAttrib & (1L << bAccessCntl)) != 0)
#define VolHasNoSysDir(volParms) \
(((volParms)->vMAttrib & (1L << bNoSysDir)) != 0)
#define VolHasExtFSVol(volParms) \
(((volParms)->vMAttrib & (1L << bHasExtFSVol)) != 0)
#define VolHasOpenDeny(volParms) \
(((volParms)->vMAttrib & (1L << bHasOpenDeny)) != 0)
#define VolHasCopyFile(volParms) \
(((volParms)->vMAttrib & (1L << bHasCopyFile)) != 0)
#define VolHasMoveRename(volParms) \
(((volParms)->vMAttrib & (1L << bHasMoveRename)) != 0)
#define VolHasDesktopMgr(volParms) \
(((volParms)->vMAttrib & (1L << bHasDesktopMgr)) != 0)
#define VolHasShortName(volParms) \
(((volParms)->vMAttrib & (1L << bHasShortName)) != 0)
#define VolHasFolderLock(volParms) \
(((volParms)->vMAttrib & (1L << bHasFolderLock)) != 0)
#define VolHasPersonalAccessPrivileges(volParms) \
(((volParms)->vMAttrib & (1L << bHasPersonalAccessPrivileges)) != 0)
#define VolHasUserGroupList(volParms) \
(((volParms)->vMAttrib & (1L << bHasUserGroupList)) != 0)
#define VolHasCatSearch(volParms) \
(((volParms)->vMAttrib & (1L << bHasCatSearch)) != 0)
#define VolHasFileIDs(volParms) \
(((volParms)->vMAttrib & (1L << bHasFileIDs)) != 0)
#define VolHasBTreeMgr(volParms) \
(((volParms)->vMAttrib & (1L << bHasBTreeMgr)) != 0)
#define VolHasBlankAccessPrivileges(volParms) \
(((volParms)->vMAttrib & (1L << bHasBlankAccessPrivileges)) != 0)
#define VolSupportsAsyncRequests(volParms) \
(((volParms)->vMAttrib & (1L << bSupportsAsyncRequests)) != 0)
#define VolSupportsTrashVolumeCache(volParms) \
(((volParms)->vMAttrib & (1L << bSupportsTrashVolumeCache)) != 0)

/* attribute bits supported by version 3 and greater versions of GetVolParmsInfoBuffer */
#define VolIsEjectable(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bIsEjectable)) != 0)
#define VolSupportsHFSPlusAPIs(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsHFSPlusAPIs)) != 0)
#define VolSupportsFSCatalogSearch(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsFSCatalogSearch)) != 0)
#define VolSupportsFSExchangeObjects(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsFSExchangeObjects)) != 0)
#define VolSupports2TBFiles(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupports2TBFiles)) != 0)
#define VolSupportsLongNames(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsLongNames)) != 0)
#define VolSupportsMultiScriptNames(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsMultiScriptNames)) != 0)
#define VolSupportsNamedForks(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsNamedForks)) != 0)
#define VolSupportsSubtreeIterators(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsSubtreeIterators)) != 0)
#define VolL2PCanMapFileBlocks(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bL2PCanMapFileBlocks)) != 0)
#define VolParentModDateChanges(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bParentModDateChanges)) != 0)
#define VolAncestorModDateChanges(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bAncestorModDateChanges)) != 0)
#define VolSupportsSymbolicLinks(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bSupportsSymbolicLinks)) != 0)
#define VolIsAutoMounted(volParms) \
((GetVolParmsInfoExtendedAttributes(volParms) & (1L << bIsAutoMounted)) != 0)

static void nglSetPathVolumeFromRefNum(int32 volnum, nglPathVolume& rPath)
{
  // Get the volume Name:
  FSVolumeInfo info;
  HFSUniStr255 volumeName;
  FSRef root;

  FSGetVolumeInfo(volnum, 0, NULL, kFSVolInfoNone, &info, &volumeName, &root);

  // Fetch the name:

  rPath.mComment.Import((char*)volumeName.unicode, volumeName.length*2, eUCS2);
	// Then the root path:
	char path[1024];
  FSRefMakePath(&root, (UInt8*)path, 1024);
	rPath.mPath = path;

	// Get other infos:
	GetVolParmsInfoBuffer buf;
  UInt32 s = sizeof(buf);
	nglFSGetVolParms(volnum, s, &buf, &s);

  rPath.mType = nglPathVolume::eTypeHD;
  if (VolIsNetworkVolume(&buf))
	  rPath.mType = nglPathVolume::eTypeNetwork;

  rPath.mFlags = 0;
  if (VolIsEjectable(&buf))
    rPath.mFlags |= nglPathVolume::Removable;
}



nglPathVolume nglPathVolume::AddVolume(std::list<nglPathVolume>& rVolumes, int32 volnum)
{

  std::list<nglPathVolume>::iterator it = rVolumes.begin();
  std::list<nglPathVolume>::iterator end = rVolumes.end();

  while (it != end)
  {
    const nglPathVolume& vol(*it);

    // If the volume is already in the list, let's return it to the caller.
    if (vol.mRefNum == volnum)
      return vol;

    ++it;
  }


  nglPathVolume vol;
  nglSetPathVolumeFromRefNum(volnum, vol);
  vol.mRefNum = volnum;
  rVolumes.push_back(vol);

  //printf("Added volume '%s' from '%s'.\n", vol.Comment.GetChars(), vol.Path.GetChars());
  return vol;
}

nglPathVolume nglPathVolume::DelVolume(std::list<nglPathVolume>& rVolumes, int32 volnum)
{
  std::list<nglPathVolume>::iterator it = rVolumes.begin();
  std::list<nglPathVolume>::iterator end = rVolumes.end();

  while (it != end)
  {
    nglPathVolume& vol(*it);

    // If the volume is already in the list, let's return it to the caller.
    if (vol.mRefNum == volnum)
    {
      nglPathVolume v(vol);
      rVolumes.erase(it);
      //printf("Removed volume '%s' from '%s'.\n", v.Comment.GetChars(), v.Path.GetChars());
      return v;
    }

    ++it;
  }

  //printf("Could not found removed volume in list.\n");
  return nglPathVolume();
}
#else
bool nglPath::ResolveLink()
{
  //#TODO implement UIKIT & COCOA versions
  return false;
}
#endif // _CARBON_

#ifdef _COCOA_
#import <Cocoa/Cocoa.h>
void nglPathVolume::UpdateVolumes(std::list<nglPathVolume>& rVolumes)
{
  NSArray* keys = [NSArray arrayWithObjects:
                   NSURLVolumeNameKey,
                   NSURLVolumeIsLocalKey,
                   NSURLVolumeIsEjectableKey,
                   NSURLVolumeIsReadOnlyKey,
                   NSURLVolumeIsBrowsableKey,
                   nil];
  
  NSArray* urls = [[NSFileManager defaultManager] mountedVolumeURLsIncludingResourceValuesForKeys:keys options:0];
  for (NSURL *url in urls)
  {
    NSError* error;
    NSNumber* isLocal;
    NSNumber* isEjectable;
    NSNumber* isReadOnly;
    NSNumber* isBrowsable;
    NSString* volumeName;
    
    if (![url getResourceValue:&isBrowsable forKey:NSURLVolumeIsBrowsableKey error:&error] || ![isBrowsable boolValue])
      continue;
    if (![url getResourceValue:&volumeName forKey:NSURLVolumeNameKey error:&error])
      continue;
    if (![url getResourceValue:&isLocal forKey:NSURLVolumeIsEjectableKey error:&error])
      continue;
    if (![url getResourceValue:&isEjectable forKey:NSURLVolumeIsEjectableKey error:&error])
      continue;
    if (![url getResourceValue:&isReadOnly forKey:NSURLVolumeIsReadOnlyKey error:&error])
      continue;
    
    nglPathVolume v;
    v.mPath = nglPath([[url path] UTF8String]);
    if (v.mPath == nglPath(_T("/"))) // Skip '/' ...
      continue;
    
    // Fill type
    v.mType = nglPathVolume::eTypeHD;
    if (![isLocal boolValue])
      v.mType = nglPathVolume::eTypeNetwork;
    
    // Fill flags
    v.mFlags = 0;
    if ([isEjectable boolValue])
      v.mFlags |= nglPathVolume::Removable;
    if ([isReadOnly boolValue])
      v.mFlags |= nglPathVolume::ReadOnly;
    
    rVolumes.push_back(v);
  }
}
#endif


/// deprecated
int nglPath::GetChildren (std::list<nglPath>* pChildren) const
{
  if (!pChildren)
    return -1;
  return GetChildren(*pChildren);
}

int nglPath::GetChildren (std::list<nglPath>& rChildren) const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
      return pVolume->GetChildren(*this, rChildren);
  }

  int count = 0;

  if (mPathName.IsNull())
  {
    return -1;
  }

  std::string str(mPathName.GetStdString());
  const char* nodename = str.c_str();

  DIR* dir;
  dir = opendir (nodename);
  if (dir == NULL)
  {
//    switch (errno)
//    {
//      case EACCES:  SetError ("path", NGL_PATH_EACCESS); break;
//      case ENOENT:  SetError ("path", NGL_PATH_EEXIST); break;
//      case EMFILE:  SetError ("path", NGL_PATH_EPROCMAX); break;
//      case ENFILE:  SetError ("path", NGL_PATH_ESYSMAX); break;
//      case ENOTDIR: SetError ("path", NGL_PATH_ELEAF); break;
//    }
    return -1;
  }

  struct dirent* entry;

  while ((entry = readdir (dir)))
  {
    if (strcmp (entry->d_name, ".") && strcmp (entry->d_name, ".."))
    {
      //printf("nglpath - base = '%s' - child = '%s'\n", GetChars(), entry->d_name);
      nglPath path = *this;
      path += nglPath(entry->d_name);
      rChildren.push_back (path);

      count++;
    }
  }
  closedir (dir);

  return count;
}


/*
 * This is rather Linux code here, warning other Unices !
 */
#if (defined _CARBON_)
static nglString GetMimeType(const nglString& extension)
{
  CFStringRef mimeType = NULL;
  CFStringRef ext = extension.ToCFString();

  CFStringRef UTI = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, ext, NULL);
  CFRelease(ext);
  if (!UTI)
    return nglString::Null;

  mimeType = UTTypeCopyPreferredTagWithClass(UTI, kUTTagClassMIMEType);
  if (!mimeType) // check for edge case
  {
    return nglString::Null;
  }

  CFRelease(UTI);
  nglString mime(mimeType);
  CFRelease(mimeType);
  return mime;
}
#endif

nglString nglPath::GetMimeType() const
{

  nglString ext = GetExtension();
  ext.ToLower();
  if (ext.IsEmpty())
    return nglString::Empty;

#if (defined _CARBON_)
  {
    nglString mime = ::GetMimeType(ext);
    if (!mime.IsNull())
      return mime;
  }
#endif


  nglPathInfo info;
  nglPath path("/etc/mime.types");

  path.GetInfo(info);
  if (info.Exists && (info.LastMod < mMimeTypeStamp))
  {
    mMimeType.clear();
    mMimeTypeStamp = info.LastMod;

    nglString line;
    nglIFile file(path);

    while (file.ReadLine(line))
    {
      if (line[0] == _T('#')) continue; // Pass over comments

      uint i;
      std::vector<nglString> tokens;
      if (line.Tokenize(tokens, _T(" \t\n")) < 2)
        continue;

      for (i = 1; i < tokens.size(); i++)
      {
        tokens[i].ToLower();
        mMimeType[tokens[i]] = tokens[0];
      }
    }
  }

  nglString& result = mMimeType[ext];
  return result.IsNull() ? nglString::Empty : result;
}

static const nglChar* _intrank(int x)
{
  switch (x)
  {
    case 1 : return _T("st");
    case 2 : return _T("nd");
    case 3 : return _T("rd");
    default: return _T("th");
  }
}

bool nglPath_IsRO(nglString& rOptions)
{
  std::vector<nglString> tokens;
  std::vector<nglString>::iterator i;

  rOptions.Tokenize(tokens, _T(','));
  for (i = tokens.begin(); i != tokens.end(); i++)
    if ((*i) == _T("ro")) return true;

  return false;
}

bool nglPath_SetVolume(nglPathVolume& rVolume,
                       nglString& rMPoint,
                       nglString& rDevice,
                       nglString& rFSType,
                       nglString& rOptions)
{
  rVolume.mPath     = nglPath(rMPoint);
  rVolume.mFlags = nglPathVolume::Offline;
  rVolume.mType  = nglPathVolume::eTypeUnknown;

  if ((rMPoint == _T("none")) ||
      (rMPoint == _T("/")) ||
      (rMPoint == _T("/boot")) ||
      (rMPoint == _T("/usr")) ||
      (rMPoint == _T("/usr/local")) ||
      (rMPoint == _T("/var")) ||
      (rMPoint == _T("/tmp")) ||
      (rMPoint == _T("/home")))
  {
    rVolume.mFlags |= nglPathVolume::System;
  }

  if (!rDevice.Compare(_T("/dev/hd"), 0, 7))
  {
    int controler = (rDevice[7] - 'a') / 2 + 1;
    int channel = (rDevice[7] - 'a') % 2;
    int partition = rDevice[8] - '0';
    rVolume.mComment.Format(_T("%s on %d%s partition (%s on %d%s IDE controler)"),
      rFSType.GetChars(),
      partition, _intrank(partition),
      channel ? _T("slave") : _T("master"),
      controler, _intrank(controler));
  }
  else
  if (!rDevice.Compare(_T("/dev/sd"), 0, 7))
  {
    int dev = rDevice[7] - 'a';
    int partition = rDevice[8] - '0';
    rVolume.mComment.Format(_T("%s on %d%s partition (%d%s SCSI device)"),
      rFSType.GetChars(),
      partition, _intrank(partition),
      dev, _intrank(dev));
    rVolume.mType = nglPathVolume::eTypeHD;
  }
  else
  if ((!rDevice.Compare(_T("/dev/sr"), 0, 7)) ||
      (!rDevice.Compare(_T("/dev/sg"), 0, 7)))
  {
    int dev = rDevice[7] - '0' + 1;
    rVolume.mComment.Format(_T("%s (%d%s SCSI device)"),
      rFSType.GetChars(),
      dev, _intrank(dev));
      rVolume.mFlags |= nglPathVolume::Removable;
      rVolume.mType = nglPathVolume::eTypeCD;
  }
  else
  if (!rDevice.Compare(_T("/dev/fd"), 0, 7))
  {
    int dev = rDevice[7] - '0' + 1;
    rVolume.mComment.Format(_T("%d%s floppy"), dev, _intrank(dev));
    rVolume.mFlags |= nglPathVolume::Removable;
    rVolume.mType = nglPathVolume::eTypeFloppy;
  }

  if (rFSType == _T("smbfs"))
  {
    rVolume.mComment.Format(_T("%s (SMB)"), rDevice.GetChars());
    rVolume.mType = nglPathVolume::eTypeNetwork;
  }
  else
  if (rFSType == _T("nfs"))
  {
    rVolume.mComment.Format(_T("%s (NFS)"), rDevice.GetChars());
    rVolume.mType = nglPathVolume::eTypeNetwork;
  }
  else
  if ((rFSType == _T("proc")) ||
      (rFSType == _T("devfs")) ||
      (rFSType == _T("usbdevfs")) ||
      (rFSType == _T("devpts")))
  {
    rVolume.mComment =  rFSType;
    rVolume.mFlags |= nglPathVolume::System;
  }

  if (nglPath_IsRO(rOptions))
    rVolume.mFlags |= nglPathVolume::ReadOnly;

  return true;
}

uint64 nglPath::GetVolumes(std::list<nglPathVolume>& rVolumes, uint64 Flags)
{
#if (defined _CARBON_ || defined _COCOA_)
  nglPathVolume::UpdateVolumes(rVolumes);
#else
	#define DEVICE tokens[0]
  #define MPOINT tokens[1]
  #define FSTYPE tokens[2]

  // Parse /etc/fstab to list all potential mount points
  nglString line;
  nglIFile fstab("/etc/fstab");
  if (fstab.GetState() == eStreamError)
    return 0;

  while (fstab.ReadLine(line))
  {
    if (line[0] == _T('#'))
      continue;

    std::vector<nglString> tokens;
    nglPathVolume vol;

    line.Tokenize(tokens, _T(" \t"));
    if (tokens.size() < 4)
    	continue;

	if (tokens.size() && nglPath_SetVolume(vol, tokens[1], tokens[0], tokens[2], tokens[3]))
	  rVolumes.push_front(vol);
  }

  // Parse /proc/mounts to list all mounted items
  nglIFile mounts("/proc/mounts");
  if (mounts.GetState() == eStreamError)
    return rVolumes.size();

  while (mounts.ReadLine(line))
  {
    std::vector<nglString> tokens;
    std::list<nglPathVolume>::iterator i;
    nglPathVolume newvol;
    nglPathVolume* vol;

    line.Tokenize(tokens, _T(" \t"));

    for (i = rVolumes.begin(); (i != rVolumes.end()) && ((*i).mPath != tokens[1]); i++);

    if (i != rVolumes.end())
      vol = &(*i); // Already in list
    else
    {
      // Create new entry
      if (tokens.size() && nglPath_SetVolume(newvol, tokens[1], tokens[0], tokens[2], tokens[3]))
        rVolumes.push_front(newvol);
      vol = &newvol;
    }

    vol->mFlags &= ~nglPathVolume::Offline;
    if (nglPath_IsRO(tokens[3]))
      vol->mFlags |= nglPathVolume::ReadOnly;
  }
#endif

  nglVolume::GetVolumes(rVolumes, Flags);

  return rVolumes.size();
}

/* Return the number of characters designing a FS root
 * Examples :
 *   0 : path is relative
 *   1 : the '/' (Unix root)
 */
//int nglPath::GetRootPart() const
//{
//  return (mPathName[0] == _T('/')) ? 1 : 0;
//}
