/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#define DEBUG_NGLPATH 0

#include "nui.h"

#if (defined _UIKIT_) || (defined _COCOA_)
#include "Cocoa/nglPath_Cocoa.h"
#endif

using namespace std;

#ifndef MIN
	#define MIN(X,Y) ((X<Y)?X:Y)
#endif

#ifndef MAX
	#define MAX(X,Y) ((X>Y)?X:Y)
#endif


using namespace std;


#ifndef _T
#define _T(X) X
#endif

const nglChar nglPath::PortableCharset[] = _T("/.abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-_");

const nglPathVolume::VolumeFlags nglPathVolume::All        = 0xFFFFFFFF;
const nglPathVolume::VolumeFlags nglPathVolume::ReadOnly   = (1 << 0);
const nglPathVolume::VolumeFlags nglPathVolume::Removable  = (1 << 1);
const nglPathVolume::VolumeFlags nglPathVolume::Offline    = (1 << 2);
const nglPathVolume::VolumeFlags nglPathVolume::System     = (1 << 3);


nglPathVolume::nglPathVolume(const nglPath& rPath, const nglString& rComment, nglPathVolume::VolumeFlags Flags, nglPathVolume::MediaType Type)
{
	mPath = rPath;      ///< nglPath to access this volume
	mComment = rComment;   ///< Comment (volume label, share comment, etc)
	mFlags = Flags;  ///< Various properties
	mType = Type;   ///< Volume media type
#ifdef _CARBON_
	mRefNum = 0;
#endif
}


nglPathVolume::nglPathVolume(const nglPathVolume& rPathVolume)
{
	*this = rPathVolume;
}

nglPathVolume::~nglPathVolume()
{
	//
}

nglPathVolume& nglPathVolume::operator=(const nglPathVolume& rPathVolume)
{
	mPath = rPathVolume.mPath;      ///< nglPath to access this volume
	mComment = rPathVolume.mComment;   ///< Comment (volume label, share comment, etc)
	mFlags = rPathVolume.mFlags;  ///< Various properties
	mType = rPathVolume.mType;   ///< Volume media type
#ifdef _CARBON_
	mRefNum = rPathVolume.mRefNum;
#endif
	return *this;
}


/*
* Life cycle
*/

nglPath::nglPath()
{
}

nglPath::nglPath (const nglChar* pPathName)
{
	InternalSetPath(pPathName);
}

nglPath::nglPath (const nglString& rPathName)
{
	InternalSetPath(rPathName.GetChars());
}

nglPath::nglPath (nglPathBase Base)
{
	switch (Base)
	{
		case ePathCurrent:
			{
				char buffer[PATH_MAX + 1];

				#ifdef WINCE
				buffer[0] = '/';
				buffer[1] = '\0';
				#else
				getcwd (buffer, PATH_MAX);
				#endif
				//       if (errno == ERANGE)
				//         NGL_LOG("path", NGL_LOG_ERROR, _T("nglPath(ePathCurrent): path might have been truncated"));
				buffer[PATH_MAX] = '\0';
				InternalSetPath(buffer);
			}
			break;

		case ePathUser:
			{
				#ifdef WINCE
				InternalSetPath("/");
				#elif defined _WIN32_
				InternalSetPath(getenv("USERPROFILE"));
				#else
				InternalSetPath(getenv("HOME"));
				#endif
			}
			break;

    case ePathTemp:
      {
        #ifdef WINCE
        InternalSetPath("/");
        #elif defined _WIN32_
        InternalSetPath(getenv("TEMP"));
        #elif defined(_UIKIT_)
        InternalSetPath(nuiCocoaGetPath_Temp());  
        #elif defined (_ANDROID_)
        InternalSetPath("/sdcard");
        #else
        InternalSetPath(_T("/tmp"));
        #endif
      }
      break;
      
		case ePathApp:
			{
				#ifdef _WIN32_
				TCHAR moduleFilename[MAX_PATH];

				HMODULE hModule = ::GetModuleHandle(NULL);
				if (0 == ::GetModuleFileName (hModule, &moduleFilename[0], MAX_PATH))
					throw exception();

				// set the path of the master program
				mPathName = moduleFilename;
				mPathName.Replace (L'\\', L'/');
				#endif
				#ifdef _CARBON_
						
					CFBundleRef appBdl = CFBundleGetMainBundle();									// get application bundle
					CFURLRef	appURL = CFBundleCopyBundleURL(appBdl);							// get url from application bundle
					CFStringRef appURLStr = CFURLCopyFileSystemPath(appURL, kCFURLPOSIXPathStyle); // url to string
					
          nglString p(appURLStr);
					InternalSetPath(nglPath(p));
					
					CFRelease (appURLStr);
					CFRelease (appURL);
					
								
				#endif
				#ifdef _LINUX_
					InternalSetPath (App->GetPath());
				#endif


			}
			break;
		case ePathUserAppSettings:
			{
				#ifdef WINCE
				InternalSetPath("/");
				#elif defined _WIN32_
				TCHAR szAppData[MAX_PATH];
				if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, szAppData))) 
				{
					InternalSetPath(szAppData);
				}
        #elif defined(_UIKIT_)
				InternalSetPath(nuiCocoaGetPath_AppSettings());
				#elif defined(__MACHO__)
				InternalSetPath(nglPath(getenv("HOME")) + nglPath("Library/Application Support"));
				#else
				InternalSetPath(getenv("HOME"));
				#endif
			}
			break;
		case ePathUserDocuments:
			{
				#ifdef WINCE
				InternalSetPath("/");
				#elif defined _WIN32_
				TCHAR szAppData[MAX_PATH];

				if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, szAppData))) 
				{
					InternalSetPath(szAppData);
				}
        #elif defined(_UIKIT_)
				InternalSetPath(nuiCocoaGetPath_UserDocuments());
        #elif defined(__MACHO__)
				InternalSetPath(nglPath(getenv("HOME")) + nglPath("Documents"));
				#else
				InternalSetPath(getenv("HOME"));
				#endif
			}
			break;
		case ePathUserPreferences:
      {
        #ifdef WINCE
        InternalSetPath("/");
        #elif defined _WIN32_
        TCHAR szAppData[MAX_PATH];
        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL, 0, szAppData))) 
        {
            InternalSetPath(szAppData);
        }
        #elif defined(_UIKIT_)
        InternalSetPath(nuiCocoaGetPath_UserPreferences());
        #elif defined(__MACHO__)
        InternalSetPath(nglPath(getenv("HOME")) + nglPath("Library/Preferences"));
        #else
        InternalSetPath(getenv("HOME"));
        #endif
      }
      break;
    case ePathUserDesktop:
      {
        #ifdef WINCE
        InternalSetPath("/");
        #elif defined _WIN32_
        TCHAR szAppData[MAX_PATH];

        if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOP|CSIDL_FLAG_CREATE, NULL, 0, szAppData))) 
        {
        InternalSetPath(szAppData);
        }
        #elif defined(_UIKIT_)
        InternalSetPath(nuiCocoaGetPath_UserDesktop());
        #elif defined(__MACHO__)
        InternalSetPath(nglPath(getenv("HOME")) + nglPath("Desktop"));
        #else
        InternalSetPath(getenv("HOME"));
        #endif
      }
      break;
	}
}

nglPath::nglPath (const nglPath& rPath)
{
	mPathName = rPath.mPathName;
}

nglPath::~nglPath()
{
}


/*
* Node operations
*/

//#define NGL_PATH_DEBUG

bool nglPath::Delete(bool Recurse) const
{
	nglPathInfo info;
	if (!GetInfo(info))
		return false;

	if (Recurse && info.Exists && !info.IsLeaf)
	{
		list<nglPath> children;

		if (GetChildren(&children) > 0)
		{
			list<nglPath>::iterator child;

			for (child = children.begin(); child != children.end(); ++child)
			{
				nglPath node = *child;
				if (!node.Delete(true)) return false;
			}
		}
		children.clear();
		Delete(false);
	}
	else
	{
    nglString volume = GetVolumeName();
    if (!volume.IsEmpty())
    {
      nglVolume* pVolume = nglVolume::GetVolume(volume);
      if (pVolume)
        return pVolume->Delete(*this);
    }
    
		int error = 0;
		if (!info.IsLeaf) 
		{
			#ifdef WINCE
			RemoveDirectory(mPathName.GetChars());
			#else
			error = rmdir(mPathName.GetStdString().c_str());
			#endif
		}
		else
		{
			#ifdef WINCE
			DeleteFile(mPathName.GetChars());
			#else
			error = remove(mPathName.GetStdString().c_str());
			#endif
		}

		if (error == -1)
		{
			return false;
		}
	}
	return true;
}

bool FindRec(const nglPath &path, nglPath &currentPath, const nglString &FileName)
{
	nglPathInfo info;
	if (!path.GetInfo(info))
    return false;

	if (!info.IsLeaf)
	{
		std::list<nglPath> list;
		path.GetChildren(&list);
		std::list<nglPath>::const_iterator end = list.end();
		for (std::list<nglPath>::const_iterator it = list.begin(); it != end; ++it)
		{
			const nglPath &child = *it;
			bool res = FindRec(child, currentPath, FileName);
			if (res)
				return true;
		}
	}
	else
	{
		if (path.GetNodeName() == FileName)
		{
			currentPath = path;
			return true;
		}
		return false;
	}
	return false;
}

nglPath nglPath::Find(const nglString &FileName) const
{
	nglPath root(*this);
	nglPath resPath;
	FindRec(root, resPath, FileName);
	return resPath;
}


bool nglPath::Move(const nglPath& PathTarget)
{	
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
      return pVolume->Move(*this, PathTarget);
  }
  
#ifdef WINCE
	BOOL	result = MoveFile(mPathName.GetChars(), PathTarget.GetPathName().GetChars());
	return (result!=0);
#else
	std::string		oldname = mPathName.GetStdString();
	std::string		newname = PathTarget.GetPathName().GetStdString();
	int retval = rename(oldname.c_str(), newname.c_str());

	if( retval != 0 ) 
	{
		switch(retval)
		{
			case ENOENT:
				//SetError("path", NGL_PATH_EBADPATH);
				break;
			case EACCES:
				//SetError("path", NGL_PATH_EACCESS);
				break;
		}
		return false;
	}
	return true;
#endif
}

bool nglPath::Copy(const nglPath& PathTarget) const
{
  nglIStream* pInStream = OpenRead();
  if (pInStream == NULL)
  {
    return false;
  }
  
  nglIOStream* pOutStream = PathTarget.OpenWrite();
  if (pOutStream == NULL)
  {
    delete pInStream;
    return false;
  }
  
  nglFileSize available = pInStream->Available();
  int64 piped = pInStream->PipeTo(*pOutStream);
  
  delete pInStream;
  delete pOutStream;
  
  return (piped == available);
}

bool nglPath::Create(bool Recurse) const
{
	//SetError("path", NGL_PATH_ENONE);

	if (Recurse)
	{
		int32 len = mPathName.GetLength();
		int32 slash = (mPathName.GetChar(0) == '/') ? 0 : -1;

		while (slash < len)
		{
			slash = mPathName.Find(L'/', slash + 1);
			if (slash == -1)
				slash = len;
			nglPath node(mPathName.GetLeft(slash));

			if(slash>=1 && mPathName[slash-1]==L':')	continue;	// skip drives

			nglPathInfo info;
			node.GetInfo (info);

			if (info.Exists)
			{
				if (info.IsLeaf)
				{
					//SetError("path", NGL_PATH_EBADPATH);
					return false;
				}
			}
			else
			{
				if (!node.Create(false))
				{
					//SetError("path", node.GetError());
					return false;
				}
			}
		}
	}
	else
	{
		if (mPathName.IsEmpty())
		{
			//SetError("path", NGL_PATH_EINVALID);
			return false;
		}

    nglString volume = GetVolumeName();
    if (!volume.IsEmpty())
    {
      nglVolume* pVolume = nglVolume::GetVolume(volume);
      if (pVolume)
        return pVolume->MakeDirectory(*this);
    }

#ifdef WINCE
		BOOL res = CreateDirectory(mPathName.GetChars(),NULL);
		return (res!=0);
#elif defined _WIN32_
		if (mkdir (mPathName.GetStdString().c_str()) == -1)
#else  // Serious OS
		if (mkdir (mPathName.GetStdString().c_str(), 0755) == -1)
#endif // _WIN32_
		{
			/*
			switch (errno)
			{
			case EPERM:
			case EACCES:
			case EROFS:
			//SetError("path", NGL_PATH_EACCESS); break;
			case ENOENT:
			case ENOTDIR:
			//SetError("path", NGL_PATH_EBADPATH); break;
			case ENAMETOOLONG:
			//SetError("path", NGL_PATH_ETOOLONG); break;
			case ENOSPC:
			//SetError("path", NGL_PATH_ENOSPACE); break;
			}
			*/
			return false;
		}
	}
	return true;
}


/*
* nglPath information
*/

#define DEBUG_CANONIZE(x)

bool nglPath::Canonize()
{
	nglString canon;
	int32 len = mPathName.GetLength();
	int32 root_part = GetRootPart();
	int32 last_slash = root_part;
	int32 slash = 0;

	canon = mPathName.GetLeft(root_part);
	while (slash < len)
	{
		slash = mPathName.Find(_T('/'), last_slash);
		if (slash == - 1)
			slash = len;

		if (((slash - last_slash) == 1) && (mPathName.GetChar(last_slash) == _T('.')))
		{
			// Ignore '.'
		}
		else
			if (((slash - last_slash) == 2) && (!mPathName.Compare(_T(".."), last_slash, 2)))
			{
				// Interpret '..'
				int32 prev_slash = canon.FindLast(_T('/'));
				if (prev_slash < root_part)
					prev_slash = root_part;

				if (!canon.IsEmpty() && canon.Compare(_T(".."), canon.GetLength() - 2, 2))
					canon.Delete(prev_slash);
				else
				{
					if (canon.GetLength() > root_part)
						canon += _T('/');
					canon += _T("..");
				}
			}
			else
			{
				// Simply append path node
				nglString node = mPathName.Extract(last_slash, (slash - last_slash));
				if (canon.GetLength() > root_part)
					canon += _T('/');
				canon += node;
			}

			last_slash = slash + 1;
	}

	mPathName = canon;
	return true;
}

const nglString& nglPath::GetPathName() const
{
	return mPathName;
}

nglString nglPath::GetNodeName() const
{
	return mPathName.Extract ((mPathName.FindLast(_T('/')) + 1));
}

nglString nglPath::GetExtension() const
{
	nglString node = GetNodeName();
	int32 dot = node.FindLast(_T('.'));

	if (dot != -1)
	{
		node.DeleteLeft(dot + 1);
		return node;
	}
	else
		return nglString::Empty;
}

nglString nglPath::GetRemovedExtension() const
{
	nglString	result = mPathName;
	int32	dot = result.FindLast(_T('.'));
	if (dot == -1)
    return result;

	result.DeleteRight(result.GetLength()-dot);
	return result;
}

void nglPath::SetExtension(const nglString& rExtension)
{
  int32 dot = mPathName.FindLast(_T('.'));
  if (dot != -1)
  {
    mPathName.DeleteRight(mPathName.GetLength() - dot);
  }

  mPathName.Append('.').Append(rExtension);
}

nglString nglPath::GetParentName() const
{
	int32 rootpart = GetRootPart();
	/*
	if (rootpart == mPathName.GetLength())
	return mPathName;
	*/

	int32 slash = mPathName.FindLast(_T('/'));
	if (slash > 0)
		return mPathName.GetLeft(((slash < rootpart) ? rootpart : slash)); // Make sure we keep at least the root part
	else
		return mPathName.GetLeft(rootpart);
}

nglPath nglPath::GetParent() const
{
	return nglPath (GetParentName());
}

void nglPath::Split(std::vector<nglString>& rElements)
{
  rElements.clear();
  if (IsAbsolute())
    rElements.push_back(_T("/"));
  mPathName.Tokenize(rElements);
}

/*
* Node (file/folder) information
*/

bool nglIsFileVisible(const nglString& rPathName)
{
#if (defined _CARBON_ || defined _COCOA_)
  FSRef ref;
  OSStatus err = FSPathMakeRefWithOptions((const UInt8*) rPathName.GetStdString(eUTF8).c_str(), kFSPathMakeRefDoNotFollowLeafSymlink, &ref, 0);
  if (err == noErr)
  {
    LSItemInfoRecord info;
    err = LSCopyItemInfoForRef(&ref, kLSRequestBasicFlagsOnly, &info);
    if (err == noErr)
    {
      return ((info.flags & kLSItemInfoIsInvisible) == 0);
    }
  }
  return true;
#else
  return true;
#endif
}


bool nglPath::GetInfo(nglPathInfo& rInfo) const
{
	if (mPathName.IsEmpty())
	{
		return false;
	}

  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
      return pVolume->GetPathInfo(*this, rInfo);
  }
  

#ifdef _WIN32_
	WIN32_FILE_ATTRIBUTE_DATA	dataInfo;

	BOOL	result = GetFileAttributesEx(mPathName.GetChars(), GetFileExInfoStandard, &dataInfo);
	if (result==0)
	{
    rInfo.Exists=false;
    return false;
  }

  SYSTEMTIME lastAccess; 
  BOOL res = FileTimeToSystemTime(&dataInfo.ftLastAccessTime, &lastAccess);
  SystemTimeToTzSpecificLocalTime(NULL, &lastAccess, &lastAccess);
  NGL_ASSERT(res);

  SYSTEMTIME lastMod; 
  res = FileTimeToSystemTime(&dataInfo.ftLastWriteTime, &lastMod);
  SystemTimeToTzSpecificLocalTime(NULL, &lastMod, &lastMod);
  NGL_ASSERT(res);

  nglTimeInfo accessTimeInfo;
  accessTimeInfo.Seconds = lastAccess.wSecond;
  accessTimeInfo.Minutes = lastAccess.wMinute;
  accessTimeInfo.Hours = lastAccess.wHour;
  accessTimeInfo.Day = lastAccess.wDay;
  accessTimeInfo.Month = lastAccess.wMonth; ///< 1..12 (unlike system's gmtime, 0..11)
  accessTimeInfo.Year = lastAccess.wYear - 1900; 
  accessTimeInfo.WeekDay = lastAccess.wDayOfWeek;
  accessTimeInfo.DST = -1; // <=> information not available


  nglTimeInfo modTimeInfo;
  modTimeInfo.Seconds = lastMod.wSecond;
  modTimeInfo.Minutes = lastMod.wMinute;
  modTimeInfo.Hours = lastMod.wHour;
  modTimeInfo.Day = lastMod.wDay;
  modTimeInfo.Month = lastMod.wMonth; ///< 1..12 (unlike system's gmtime, 0..11)
  modTimeInfo.Year = lastMod.wYear - 1900; 
  modTimeInfo.WeekDay = lastMod.wDayOfWeek;
  modTimeInfo.DST = -1; // <=> information not available

  // output debug
/*
wchar_t prout[1024];
  swprintf(&prout[0], 1024,_T("\n\nFILE '%s'\naccess seconds %d  Minutes %d   Hours %d   Day %d   Month %d   Year %d   WeekDay %d   DST %d\n"),
    GetChars(), accessTimeInfo.Seconds, accessTimeInfo.Minutes, accessTimeInfo.Hours, accessTimeInfo.Day, accessTimeInfo.Month, accessTimeInfo.Year, accessTimeInfo.WeekDay, accessTimeInfo.DST);
 
  OutputDebugString(&prout[0]);

  swprintf(&prout[0], 1024,_T("mod seconds %d  Minutes %d   Hours %d   Day %d   Month %d   Year %d   WeekDay %d   DST %d\n"),
    modTimeInfo.Seconds, modTimeInfo.Minutes, modTimeInfo.Hours, modTimeInfo.Day, modTimeInfo.Month, modTimeInfo.Year, modTimeInfo.WeekDay, modTimeInfo.DST);
 
  OutputDebugString(&prout[0]);
*/



	rInfo.Exists	 = true;
	rInfo.IsLeaf     = (dataInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0;
	rInfo.LastAccess = nglTime(accessTimeInfo);
	rInfo.LastMod    = nglTime(modTimeInfo);
	rInfo.Size       = rInfo.IsLeaf ? ((((int64)dataInfo.nFileSizeHigh)<<32) + ((int64)dataInfo.nFileSizeLow)) : 0;
	rInfo.CanRead    = true; 
	rInfo.CanWrite   = (dataInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY)==0;
  rInfo.Visible    = nglIsFileVisible(mPathName);
 	return true;

#else
  if ((mPathName.GetLeft(4) == _T("/net")) || (mPathName.GetLeft(4) == _T("/home")))
  {
    rInfo.Exists	 = false;
    rInfo.IsLeaf   = true;
    return false;
  }
  
  struct stat info;
  std::string tmp(mPathName.GetStdString());
  if (stat(tmp.c_str(), &info) == -1)
  {
    switch (errno)
    {
      case ENOTDIR:
        ////SetError("path", NGL_PATH_EBADPATH);
        break;
      case EACCES :
        ////SetError("path", NGL_PATH_EACCESS);
        break;
    }
    rInfo.Exists = false;
    return false;
  }
  rInfo.Exists     = true;
#ifdef _WIN32_
  rInfo.IsLeaf     = !(_S_IFDIR & info.st_mode);
#else
  rInfo.IsLeaf     = !S_ISDIR(info.st_mode);
#endif
  rInfo.LastAccess = (nglTime)(double)info.st_atime;
  rInfo.LastMod    = (nglTime)(double)info.st_mtime;
  rInfo.Size       = rInfo.IsLeaf ? info.st_size : 0;
  rInfo.CanRead    = (access(mPathName.GetStdString().c_str(), R_OK) != -1);
  rInfo.CanWrite   = (access(mPathName.GetStdString().c_str(), W_OK) != -1);
  rInfo.Visible    = nglIsFileVisible(mPathName);
  return true;
  
  
#endif
  
}

bool nglPath::Exists() const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglPathInfo infos;
    return GetInfo(infos) && infos.Exists;
  }
  
#ifdef WINCE
	return (!mPathName.IsEmpty() && (GetFileAttributes(mPathName.GetChars()) != 0xFFFFFFFF));
#else
	return (!mPathName.IsEmpty() && (access(mPathName.GetStdString().c_str(), F_OK) != -1));
#endif
}

bool nglPath::CanRead() const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglPathInfo infos;
    return GetInfo(infos) && infos.CanRead;
  }
  
#ifdef WINCE
	return (!mPathName.IsEmpty() && (GetFileAttributes(mPathName.GetChars()) != 0xFFFFFFFF));
#else
	return (!mPathName.IsEmpty() && (access(mPathName.GetStdString().c_str(), R_OK) != -1));
#endif
}

bool nglPath::CanWrite() const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglPathInfo infos;
    return GetInfo(infos) && infos.CanWrite;
  }

#ifdef WINCE
	return (!mPathName.IsEmpty() && ((GetFileAttributes(mPathName.GetChars())&FILE_ATTRIBUTE_READONLY)==0));
#else
	return (!mPathName.IsEmpty() && (access(mPathName.GetStdString().c_str(), W_OK) != -1));
#endif
}

bool nglPath::IsVisible() const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglPathInfo infos;
    return GetInfo(infos) && infos.Visible;
  }

  return nglIsFileVisible(mPathName);
  
//#FIXME Clean dead code:  
//#ifdef WINCE
//	return (!mPathName.IsEmpty() && ((GetFileAttributes(mPathName.GetChars())&FILE_ATTRIBUTE_VISIBLE)!=1));
//#else
//  nglString nodename(GetNodeName());
//	return nodename.IsEmpty() ? false : nodename[0]==_T('.');
//#endif
}

bool nglPath::IsBundle() const
{
  nglPath p(*this);
  p += nglPath(_T("contents/Info.plist"));
  nglPathInfo info;
  p.GetInfo(info);
  return info.Exists && info.IsLeaf;
}


bool nglPath::IsLeaf() const
{
	nglPathInfo info;

	return (GetInfo(info) && info.IsLeaf);
}

nglTime nglPath::GetLastAccess() const
{
	nglPathInfo info;

	return GetInfo(info) ? nglTime(0) : info.LastAccess;
}

nglTime nglPath::GetLastMod() const
{
	nglPathInfo info;

	if (GetInfo(info))
		return info.LastMod;
	return nglTime(0);
}

nglFileSize nglPath::GetSize() const
{
	nglPathInfo info;

	if (GetInfo(info))
		return info.Size;
	return 0;
}

const nglChar* nglPath::GetChars() const
{
	return mPathName.GetChars();
}

nglPath::operator const nglChar*() const
{
	return mPathName.GetChars();
}

int32 nglPath::GetChildrenTree(std::list<nglPath>& pChildren) const
{
  std::list<nglPath> local;
  uint32 count = GetChildren(local);
  
  std::list<nglPath>::const_iterator it = local.begin();
  std::list<nglPath>::const_iterator end = local.end();
  
  while (it != end)
  {
    const nglPath& rP(*it);
    pChildren.push_back(rP);
    
    if (!rP.IsLeaf())
      count += rP.GetChildrenTree(pChildren);
    
    ++it;
  }
  
  return count;
}



/*
* Special paths
*/

bool nglPath::IsAbsolute() const
{
	return (GetRootPart() > 0);
}

nglPath nglPath::GetAbsolutePath() const
{
	nglPath abs;

	if (IsAbsolute())
		abs = *this;
	else
	{
		abs = nglPath(ePathCurrent);
		abs += *this;
	}
	abs.Canonize();
	return abs;
}


/*
* Operators
*/

const nglPath& nglPath::operator=(const nglChar* pSource)
{
	InternalSetPath(pSource);
	return *this;
}

const nglPath& nglPath::operator=(const nglString& rSource)
{
	InternalSetPath(rSource.GetChars());
	return *this;
}

const nglPath& nglPath::operator=(const nglPath& rSource)
{
	mPathName = rSource.mPathName;
	return *this;
}


const nglPath& nglPath::operator+=(const nglPath& rAppend)
{
	nglString temp = mPathName;

	if ((temp.GetLastChar() != _T('/')) && (rAppend.mPathName.GetChar(0) != L'/'))
		temp += _T('/');
	// remove the last '/' from the source if the path we append already begin with a '/'
	if ((temp.GetLastChar() == _T('/')) && (rAppend.mPathName.GetChar(0) == L'/'))
		temp.DeleteRight(1);

	temp += rAppend.mPathName;

	InternalSetPath(temp.GetChars());
	return *this;
}

const nglPath& nglPath::operator+=(const nglString& rAppend)
{
	nglPath temp(rAppend);
	*this += temp;

	return *this;
}

const nglPath& nglPath::operator+=(const nglChar* pAppend)
{
	nglPath temp(pAppend);
	*this += temp;
  
	return *this;
}

NGL_API nglPath operator+ (const nglPath& rPath, const nglPath& rAppend)
{
	nglPath result(rPath);

	result += rAppend;
	return result;
}

NGL_API nglPath operator+ (const nglPath& rPath, const nglString& rAppend)
{
	nglPath result(rPath);

	result += rAppend;
	return result;
}

NGL_API bool operator==(const nglPath& rLeft, const nglPath& rRight)
{
#if defined(_UNIX_) || defined(_BEOS_)
	return rLeft.GetPathName() == rRight.GetPathName();
#else
	// Win32 (FAT, VFAT, NTFS)
	// MacOS (HFS, HFS+)
	return !rLeft.GetPathName().Compare(rRight.GetPathName(), false);
#endif
}

NGL_API bool operator==(const nglPath& rLeft, const nglString& rRight)
{
#if defined(_UNIX_) || defined(_BEOS_)
	return rLeft.GetPathName() == rRight;
#else
	return !rLeft.GetPathName().Compare(rRight, false);
#endif
}

NGL_API bool operator==(const nglString& rLeft, const nglPath& rRight)
{
#ifdef _WIN32_
	return !rLeft.Compare(rRight.GetPathName(), false);
#else
	return rLeft == rRight.GetPathName();
#endif // _WIN32_
}


NGL_API bool operator!=(const nglPath& rLeft, const nglPath& rRight)
{
	return !(rLeft == rRight);
}

NGL_API bool operator!=(const nglPath& rLeft, const nglString& rRight)
{
	return !(rLeft == rRight);
}

NGL_API bool operator!=(const nglString& rLeft, const nglPath& rRight)
{
	return !(rLeft == rRight);
}


/*
bool nglPath::ValidateChars()
{
int i, len;

len = mPathName.GetLength();
for (i = 0; i < len; i++)
if (strchr (ValidChars, mPathName[i]) == NULL)
{
NGL_DEBUG( NGL_LOG("path", NGL_LOG_WARNING, _T("Warning: path contains non portable chars (%s)"), (nglChar*)mPathName); )
return false;
}
return true;
}
*/

bool nglPath::InternalSetPath(const nglChar* pPath)
{
	if (!pPath)
	{
		return false;
	}

	// Everybody to the '/' (slash) convention
	mPathName.Copy(pPath);
	mPathName.Replace(_T('\\'), _T('/'));    // Win32 separator

#if (defined _WIN32_) && !(defined WINCE)
	if (mPathName.GetLength() > 1 && mPathName[0] == L'/' && mPathName[2] == L':') // /c:/xxx
		mPathName.TrimLeft('/');

	if (mPathName.GetLength() > 1 && mPathName[0] == L'/' && mPathName[1] == L'/') // //servername/xxx
  {
		mPathName[0] = mPathName[1] == L'\\';
  }
  
	if (!mPathName.IsEmpty())
	{
		int drive = mPathName[0];
		drive = (drive <= 'Z') ? (drive - 'A' + 1) : (drive - 'a' + 1);

		if ((drive >= 0) &&
			(drive <= 25) &&
			(mPathName[1] == _T(':')) &&
			((mPathName.GetLength() == 2) || (mPathName[2] != L'/')))
		{
			mPathName+= _T("/");
			/*      
			// Disabled by MeeLoo: this part is hacky at best and it really prevents to make any sense in a GUI because then Win32 path are NEVER behaving in the same way as other pathes and forces so many kludges...
			// Ackward X:[file.abc] where you should read X:<X drive own current path>[/file.abc]
			nglChar buffer[MAX_PATH];

			#ifdef USE_WCHAR
			if (_wgetdcwd(drive, buffer, MAX_PATH))
			#else
			if (_getdcwd(drive, buffer, MAX_PATH))
			#endif
			{
			nglString dpath(buffer);

			dpath.Replace (_T('\\'), _T('/'));    // Win32 separator
			dpath += _T('/');
			dpath += mPathName.Extract(2);

			mPathName.Copy(dpath);
			}
			*/
		}
	}
#endif

	// Trim trailing '/' beside root name portion
	int32 rootpart = GetRootPart();
	int32 i = mPathName.GetLength() - 1;
	if (rootpart ? i > rootpart : i > 1)
		mPathName.TrimRight(_T('/'));

#if DEBUG_NGLPATH
  NGL_OUT(_T("[nglPath::InternalSetPath] '%s' [Exists %d]\n"), mPathName.GetChars(), Exists());
  //NGL_ASSERT(Exists());
#endif
	return true;
}

#ifdef _WIN32_

/// deprecated
int32 nglPath::GetChildren(list<nglPath>* pChildren) const
{
  if (!pChildren)
    return -1;
  
  return GetChildren(*pChildren);
}

int32 nglPath::GetChildren(list<nglPath>& rChildren) const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
      return pVolume->GetChildren(*this, rChildren);
  }
  
  
	int count = 0;

	if (mPathName.IsEmpty())
	{
		return false;
	}

//	#ifdef WINCE

	nglChar	filter[MAX_PATH + 1];

	strcpy(filter, mPathName.GetChars());
	if (filter[strlen(filter)-1] =='/')
		strcat(filter, "*");
	else
    strcat(filter, "/*");

	WIN32_FIND_DATA		findData;

	HANDLE	dir = FindFirstFile(filter, &findData);
	if(dir==INVALID_HANDLE_VALUE)	return 0;

	do
	{
		if(strcmp(findData.cFileName, ".") && strcmp(findData.cFileName, ".."))
		{
      nglPath	path = *this;
      path += nglPath(findData.cFileName);

      rChildren.push_back(path);
			count++;
		}
	} while(FindNextFile(dir,&findData)!=0);

	FindClose(dir);


	return count;
}


nglString nglPath::GetMimeType() const
{
	unsigned long size = 1025;
	unsigned char value[1025];
	HKEY key;
	unsigned long type = REG_SZ;
	nglString path(_T(".")+GetExtension());
	RegOpenKeyEx(HKEY_CLASSES_ROOT,path.GetChars(),0,KEY_READ,&key);
	if (RegQueryValueEx(
		key,
		_T("Content Type"),
		NULL,
		&type,
		value,
		&size)!= ERROR_SUCCESS || type!=REG_SZ)
	{
		RegCloseKey(key);
		return nglString(_T(""));;
	}
	RegCloseKey(key);
	return nglString((nglChar*)value);
}

#ifdef WINCE
uint64 nglPath::GetVolumes(list<nglPathVolume>& rVolumes, uint64 Flags)
{
	nglPathVolume	pathvolume;
	pathvolume.mPath = "/";
	rVolumes.push_back(pathvolume);
	return rVolumes.size();
}
#else
bool nglGetDriveInfo(nglChar* name, nglPathVolume& rVol)
{
	rVol.mComment = "";
	rVol.mPath = name;
	rVol.mFlags = 0;
	rVol.mType = nglPathVolume::eTypeUnknown;

	UINT type = GetDriveType(name);
	switch (type)
	{
	case DRIVE_UNKNOWN: //The drive type cannot be determined. 
		rVol.mType = nglPathVolume::eTypeUnknown;
		break;
	case DRIVE_NO_ROOT_DIR:// The root path is invalid. For example, no volume is mounted at the path. 
		rVol.mType = nglPathVolume::eTypeUnknown;
		break;
	case DRIVE_REMOVABLE:// The disk can be removed from the drive. 
		rVol.mType = nglPathVolume::eTypeHD;
		rVol.mFlags |= nglPathVolume::Removable;
		break;
	case DRIVE_FIXED:// The disk cannot be removed from the drive. 
		rVol.mType = nglPathVolume::eTypeHD;
		rVol.mFlags |= nglPathVolume::System;
		break;
	case DRIVE_REMOTE:// The drive is a remote (network) drive. 
		rVol.mType = nglPathVolume::eTypeNetwork;
		break;
	case DRIVE_CDROM: // The drive is a CD-ROM drive. 
		rVol.mType = nglPathVolume::eTypeCD;
		rVol.mFlags |= nglPathVolume::Removable;
		break;
	case DRIVE_RAMDISK: 
		rVol.mType = nglPathVolume::eTypeUnknown;
		break;
	}

	nglString RootPathName(name);
	RootPathName.SetChar('\\', 2);
	nglChar lpVolumeNameBuffer[MAX_PATH];
	DWORD nVolumeNameSize = MAX_PATH;
	DWORD VolumeSerialNumber = 0;
	DWORD MaximumComponentLength = 0;
	DWORD FileSystemFlags = 0;
	LPTSTR lpFileSystemNameBuffer = NULL;
	DWORD nFileSystemNameSize = 0;

	lpVolumeNameBuffer[0]=0;

	// Check to see if the volume is currently mounted:
	bool skip = false;
	if (rVol.mFlags &= nglPathVolume::Removable)
	{
		HANDLE file = CreateFile(RootPathName.GetChars(), 0, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file)
		{
			DWORD BytesReturned = 0;
			if (!DeviceIoControl(file, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &BytesReturned, NULL))
				rVol.mFlags |= nglPathVolume::Offline;
			CloseHandle(file);
		}

	}

	// Retrieve the label:
	GetVolumeInformation(RootPathName.GetChars(), lpVolumeNameBuffer, nVolumeNameSize, &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize);
	if (*lpVolumeNameBuffer)
  {
    rVol.mComment = lpVolumeNameBuffer;
    rVol.mComment += _T(" (");
    rVol.mComment += RootPathName;
    rVol.mComment += _T(")");
  }
  else
  {
    rVol.mComment = RootPathName;
  }

	return type != DRIVE_NO_ROOT_DIR;
}

uint64 nglPath::GetVolumes(list<nglPathVolume>& rVolumes, uint64 Flags)
{
	DWORD drives = GetLogicalDrives();

  //MS-DOS drives
	for (int i=0; i<32; i++)
	{
		if (drives & 1)
		{
			nglChar name[4];
			name[0] = L'A' + i;
			name[1] = L':';
			name[2] = L'\\';
			name[3] = 0;

			nglPathVolume volume;
			if (nglGetDriveInfo(name, volume))
				rVolumes.push_back(volume);
		}

		drives >>= 1;
	}
  
  nglVolume::GetVolumes(rVolumes, Flags);
  
	return rVolumes.size();
}
#endif

bool nglPath::ResolveLink()
{
#ifdef WINCE
	return true;
#else
	if (mPathName.IsEmpty())
		return false;

	char tmpPath[MAX_PATH];
	std::string pn(mPathName.GetStdString());
	if (access(pn.c_str(), 0) == -1) 
	{
		strcpy(tmpPath, pn.c_str());
		strcat(tmpPath,".lnk");
	}
	else 
	{
		if (GetExtension() != "lnk")
		{
			return false;
		}
		strcpy(tmpPath, pn.c_str());
	}

	HRESULT hres;
	IShellLink *pShLink = NULL;

	wchar_t wsz[MAX_PATH];
	nglChar resolvedPath[MAX_PATH];

	CoInitialize(NULL);

	bool res = false;

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&pShLink);

	if (SUCCEEDED(hres))
	{
		IPersistFile *ppf;

		// The IShellLink Interface supports the IPersistFile
		// interface. Get an interface pointer to it.
		hres = pShLink->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf);
		if (SUCCEEDED(hres))
		{
			// Convert the given link name string to a wide character string.
			MultiByteToWideChar(CP_ACP, 0, tmpPath, -1, wsz, MAX_PATH);

			// Load the file.
			hres = ppf->Load(wsz, STGM_READ);
			if (SUCCEEDED(hres))
			{
				// Resolve the link by calling the Resolve() interface function.
				// This enables us to find the file the link points to even if
				// it has been moved or renamed.
				hres = pShLink->Resolve(NULL, SLR_ANY_MATCH | SLR_NO_UI);

				if (SUCCEEDED(hres))
				{
					WIN32_FIND_DATA winFindData;
					// Get the path of the file the link points to.
					hres = pShLink->GetPath(resolvedPath, MAX_PATH, &winFindData, SLGP_UNCPRIORITY);
					if (SUCCEEDED(hres))
					{
						InternalSetPath(resolvedPath);
						res = true;
					}
				}
			}
			ppf->Release();
		}
		pShLink->Release();
	}
	return res;
#endif
}

#endif

/* Return the number of characters designing a FS root
 * Examples :
 *   0 : path is relative
 *   3 : the 'C:/' DOS drive root
 *  >3 : the '//host/' Win32 UNC
 */
int32 nglPath::GetRootPart() const
{
	nglChar drive = mPathName[0];
	if ((((drive >= _T('A')) && (drive <= _T('Z'))) ||
       ((drive >= _T('a')) && (drive <= _T('z')))) &&
      (mPathName[1] == _T(':')))
	{
		// [A-Za-z]: (drive specification)
		//NGL_ASSERTR(mPathName[2] == _T('/'), 3);
		return MIN(3, mPathName.GetLength());
	}
	else if (mPathName[0] == _T('/'))
	{
		if (mPathName[1] != _T('/'))
			return 1;
    
		// //host[/path] (network address)
		// or /volume[/path] (standard unix like path)
		int32 end = mPathName.Find(_T('/'), 2);
		return ((end > 0) ? end : mPathName.GetLength());
	}
  
  // Find the volume name:
  int col = mPathName.Find(_T(':'), 0, true);
  int slash = mPathName.Find(_T('/'), 0, true);

  // Not a volume
  if (col < 0 || slash < 0)
    return 0;
  
  if (col < slash)
    return MIN(slash + 1, mPathName.GetLength());
  
	return 0;
}


nglString nglPath::GetVolumeName() const
{
  int32 rootpart = GetRootPart();
  if (rootpart)
  {
    nglString name(mPathName.Extract(0, rootpart));
    name.Trim(_T('/'));
    name.Trim(_T(':'));
    //wprintf(_T("GetVolumeName(\"%s\") -> \"%s\"\n"), GetChars(), name.GetChars());
    return name;
  }
  
  return nglString::Empty;
}

nglString nglPath::GetVolumeLessPath() const
{
  int32 rootpart = GetRootPart();
  if (rootpart)
  {
    while (rootpart && mPathName[rootpart] != nglChar('/'))
      rootpart--;
    
    return mPathName.Extract(rootpart);
  }
  
  return nglString::Empty;
}

bool nglPath::MakeRelativeTo(const nglPath& rOriginal)
{ 
  if (rOriginal.GetPathName().IsEmpty())
  {
    return IsAbsolute();
  }
     
  if (!IsAbsolute() || !rOriginal.IsAbsolute())
    return false;
  
  nglString PathName;
  nglString RootName;
  
  std::vector<nglString> Tokens;
  mPathName.Tokenize(Tokens, nglChar('/'));
  std::vector<nglString> RootTokens;
  rOriginal.mPathName.Tokenize(RootTokens, nglChar('/'));
  
  uint32 NumTokens     = (uint32)Tokens.size();
  uint32 NumRootTokens = (uint32)RootTokens.size();
  
  if (rOriginal.IsLeaf())
    NumRootTokens--;
  
  uint32 min = (uint32)MIN(RootTokens.size(), Tokens.size());
  uint32 common = 0;
  while (common < min && RootTokens[common] == Tokens[common])
    common++;
  
  if (!common) // not the same Volume, can't be relative
    return false;
  
  nglPath RelativePath(_T("./"));
  while (NumRootTokens > common)
  {
    RelativePath += nglPath(_T("../"));
    NumRootTokens--;
  }
  // now that we are common, insert tokens until we reach the Path
  while (common < NumTokens)
    RelativePath += Tokens[common++];
  
  nglString tmp(RelativePath.GetPathName());
  RelativePath = tmp.Extract(2);
  *this = RelativePath;
  
  return true;
}


nglIStream* nglPath::OpenRead() const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
    {
      return pVolume->OpenRead(*this);
    }
  }
  
  nglIFile* pFile = new nglIFile(*this);
  if (pFile && !pFile->IsOpen())
  {
    delete pFile;
    pFile = NULL;
  }
  
  return pFile;
}

nglIOStream* nglPath::OpenWrite(bool OverWrite) const
{
  nglString volume = GetVolumeName();
  if (!volume.IsEmpty())
  {
    nglVolume* pVolume = nglVolume::GetVolume(volume);
    if (pVolume)
      return pVolume->OpenWrite(*this, OverWrite);
  }


  nglIOFile* pFile = new nglIOFile(*this, OverWrite ? eOFileCreate : eOFileModify);
  if (pFile && !pFile->IsOpen())
  {
    delete pFile;
    pFile = NULL;
  }
  
  return pFile;
}

bool operator<(const nglPath& rLeft, const nglPath& rRight)
{
  return nglComparePath(rLeft, rRight);
}

bool nglComparePath(const nglPath& rLeft, const nglPath& rRight)
{
#ifdef WIN32
  return nglCompareStringsNoCase(rLeft.GetPathName(), rRight.GetPathName());
#else
  return nglCompareStrings(rLeft.GetPathName(), rRight.GetPathName());
#endif
}

bool nglCompareNaturalPath(const nglPath& rLeft, const nglPath& rRight)
{
#ifdef WIN32
  return nglCompareNaturalStringsNoCase(rLeft.GetPathName(), rRight.GetPathName());
#else
  return nglCompareNaturalStrings(rLeft.GetPathName(), rRight.GetPathName());
#endif
}


