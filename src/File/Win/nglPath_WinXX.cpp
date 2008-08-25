/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nglApplication.h"
#include "nglPath.h"
#include "nglIFile.h"
#include <shlobj.h>
#include <errno.h>
#include <io.h>
#include <direct.h>

using namespace std;

int nglPath::GetChildren (list<nglPath>* pChildren) const
{
  SetError ("path", NGL_PATH_ENONE);
  int count = 0;

  const char* nodename = GetOSPathName();
  if (!nodename)
  {
    SetError ("path", NGL_PATH_EINVALID);
    return false;
  }

  char filter[MAX_PATH + 1];
  struct _finddata_t entry;
  long dir;

  strcpy (filter, nodename);
  if (filter[strlen(filter)-1] =='/')
    strcat (filter, "*");
  else
    strcat (filter, "/*");

  dir = _findfirst (filter, &entry);
  if (dir == -1L)
  {
    switch (errno)
    {
      case ENOENT:
      case EINVAL:
        SetError ("path", NGL_PATH_EEXIST); break;
    }
    return 0;
  }

  do
  {
    if (strcmp (entry.name, ".") && strcmp (entry.name, ".."))
    {
      if (pChildren)
      {
        nglPath path = *this;
        path += nglPath(entry.name);
        pChildren->push_back (path);
      }
      count++;
    }
  }
  while (_findnext (dir, &entry) == 0);

  _findclose (dir);

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
  return nglString((char*)value);
}

bool nglGetDriveInfo(char* name, nglPathVolume& rVol)
{
  UINT error_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
  rVol.Comment = "";
  rVol.Path = name;
  rVol.TheFlags = 0;
  rVol.TheType = nglPathVolume::eTypeUnknown;

  UINT type = GetDriveType(name);
  switch (type)
  {
  case DRIVE_UNKNOWN: //The drive type cannot be determined. 
    rVol.TheType = nglPathVolume::eTypeUnknown;
    break;
  case DRIVE_NO_ROOT_DIR:// The root path is invalid. For example, no volume is mounted at the path. 
    rVol.TheType = nglPathVolume::eTypeUnknown;
    break;
  case DRIVE_REMOVABLE:// The disk can be removed from the drive. 
    rVol.TheType = nglPathVolume::eTypeHD;
    rVol.TheFlags |= nglPathVolume::Removable;
    break;
  case DRIVE_FIXED:// The disk cannot be removed from the drive. 
    rVol.TheType = nglPathVolume::eTypeHD;
    rVol.TheFlags |= nglPathVolume::System;
    break;
  case DRIVE_REMOTE:// The drive is a remote (network) drive. 
    rVol.TheType = nglPathVolume::eTypeNetwork;
    break;
  case DRIVE_CDROM: // The drive is a CD-ROM drive. 
    rVol.TheType = nglPathVolume::eTypeCD;
    rVol.TheFlags |= nglPathVolume::Removable;
    break;
  case DRIVE_RAMDISK: 
    rVol.TheType = nglPathVolume::eTypeUnknown;
    break;
  }

  char lpRootPathName[MAX_PATH];
  strcpy(lpRootPathName, name);
  lpRootPathName[2] = '\\';
  char lpVolumeNameBuffer[MAX_PATH];
  DWORD nVolumeNameSize = MAX_PATH;
  DWORD VolumeSerialNumber = 0;
  DWORD MaximumComponentLength = 0;
  DWORD FileSystemFlags = 0;
  LPTSTR lpFileSystemNameBuffer = NULL;
  DWORD nFileSystemNameSize = 0;

  lpVolumeNameBuffer[0]=0;

  // Check to see if the volume is currently mounted:
  bool skip = false;
  if (rVol.TheFlags &= nglPathVolume::Removable)
  {
    HANDLE file = CreateFile(lpRootPathName, 0, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file)
    {
      DWORD BytesReturned = 0;
      if (!DeviceIoControl(file, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &BytesReturned, NULL))
        rVol.TheFlags |= nglPathVolume::Offline;
      CloseHandle(file);
    }

  }

  // Retrieve the label:
  GetVolumeInformation(lpRootPathName, lpVolumeNameBuffer, nVolumeNameSize, &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, lpFileSystemNameBuffer, nFileSystemNameSize);
  rVol.Comment = lpVolumeNameBuffer;

  SetErrorMode(error_mode);
  return type != DRIVE_NO_ROOT_DIR;
}


int nglPath::GetVolumes(list<nglPathVolume>& rVolumes, uint Flags)
{
  UINT error_mode = SetErrorMode(SEM_FAILCRITICALERRORS);
  DWORD drives = GetLogicalDrives();

  for (int i=0; i<32; i++)
  {
    if (drives & 1)
    {
      char name[4];
      name[0] = 'A' + i;
      name[1] = ':';
      name[2] = '/';
      name[3] = 0;

      nglPathVolume volume;
      if (nglGetDriveInfo(name, volume))
        rVolumes.push_back(volume);
    }

    drives >>= 1;
  }

  SetErrorMode(error_mode);
  return rVolumes.size();
}

/* Return the number of characters designing a FS root
 * Examples :
 *   0 : path is relative
 *   3 : the 'C:/' DOS drive root
 *  >3 : the '//host/' Win32 UNC
 */
int nglPath::GetRootPart() const
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
  else if (mPathName[0] == '/')
  {
    if (mPathName[1] != '/')
      return 1;
     
    // //host[/path] (network address)
    // or /volume[/path] (standard unix like path)
    int end = mPathName.Find('/', 2);
    return (end > 0) ? end : mPathName.GetLength();
  }
  return 0;
}

bool nglPath::ResolveLink()
{
  if (!GetOSPathName())
    return false;

  char tmpPath[MAX_PATH];
  if (access(GetOSPathName(), 0) == -1) 
  {
    strcpy(tmpPath, GetOSPathName());
    strcat(tmpPath,".lnk");
  }
  else 
  {
    if (GetExtension() != "lnk")
    {
      return false;
    }
    strcpy(tmpPath, GetOSPathName());
  }

  HRESULT hres;
  IShellLink *pShLink = NULL;

  wchar_t wsz[MAX_PATH];
  char resolvedPath[MAX_PATH];

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
}

