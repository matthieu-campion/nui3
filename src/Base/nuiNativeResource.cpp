/*
 *  nuiNativeResource.cpp
 *  nui3
 *
 *  Created by Sebastien Metrot on 20/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "nui.h"
#include "nuiNativeResource.h"


#ifdef _WIN32_
#endif // _WIN32_
#if (defined _CARBON_) || (defined _UIKIT_)
extern "C"
{
#include <mach-o/dyld.h>
#include <mach-o/ldsyms.h>
}

static CFBundleRef _CFXBundleCreateFromImageName(CFAllocatorRef allocator, const char* image_name);
static CFBundleRef _CFXBundleCreateFromImageName(CFAllocatorRef allocator, const char* image_name)
{
  CFURLRef myBundleExecutableURL = CFURLCreateFromFileSystemRepresentation(allocator, (const unsigned char*)image_name, strlen (image_name), false);
  if (myBundleExecutableURL == 0)
    return 0;
  
  CFURLRef myBundleContentsMacOSURL = CFURLCreateCopyDeletingLastPathComponent(allocator, myBundleExecutableURL); // Delete Versions/Current/Executable
  CFRelease(myBundleExecutableURL);
  if (myBundleContentsMacOSURL == 0)
    return 0;
  
  CFURLRef myBundleContentsURL = CFURLCreateCopyDeletingLastPathComponent(allocator, myBundleContentsMacOSURL); // Delete Current
  CFRelease(myBundleContentsMacOSURL);
  if (myBundleContentsURL == 0)
    return 0;
  
  CFURLRef theBundleURL = CFURLCreateCopyDeletingLastPathComponent(allocator, myBundleContentsURL); // Delete Versions
  CFRelease(myBundleContentsURL);
  if (theBundleURL == 0)
    return 0;
  
  CFBundleRef result = CFBundleCreate(allocator, theBundleURL);
  CFRelease(theBundleURL);
  
  return result;
}

static nglPath GetResourcePath()
{
  const mach_header* header = &_mh_execute_header;
  
  const char* imagename = 0;
  /* determine the image name */
  int cnt = _dyld_image_count();
  for (int idx1 = 1; idx1 < cnt; idx1++) 
  {
    if (_dyld_get_image_header(idx1) == header)
    {
      imagename = _dyld_get_image_name(idx1);
      break;
    }
  }
  /* get the bundle of a header */
  CFBundleRef bundle = NULL;
  if (imagename)
    bundle = _CFXBundleCreateFromImageName (NULL, imagename);
  
  if (!bundle)
    bundle = CFBundleGetMainBundle();
  
  CFURLRef resURLref = CFBundleCopyResourcesDirectoryURL(bundle);
  char buffer[2048];
  Boolean res = CFURLGetFileSystemRepresentation(resURLref, true, (UInt8*)buffer, 2048);
  CFRelease(resURLref);
  NGL_ASSERT(res);
  
  return nglPath(nglString(buffer));
}
#endif // _CARBON_
#ifdef _LINUX_
static nglPath GetResourcePath()
{
	nglString ResourcePathName(getenv("NUI_RESOURCE_PATH"));
	if (ResourcePathName.IsEmpty())
		ResourcePathName = nglString("/usr/share/");
	if( ResourcePathName.GetLastChar() == _T('/'))
		ResourcePathName += App->GetName();
	else
		ResourcePathName += _T('/')+App->GetName();
	return nglPath(ResourcePathName);
}
#endif //_LINUX_


nuiNativeResource::nuiNativeResource(const nglPath& rPath)
: nglIMemory(NULL, 0)
{
  mValid = false;
#ifdef _WIN32_
  nglString str(rPath.GetPathName());
  str.TrimLeft(_T('/'));
  str.ToUpper();
  HRSRC Resource = FindResource(App->GetHInstance(), str.GetChars(), _T("NUI_RESOURCE"));
  
  if (Resource == NULL)
  {
    return;
  }
  
  DWORD size = SizeofResource(App->GetHInstance(), Resource);
  HGLOBAL GlobalHandle = LoadResource(App->GetHInstance(), Resource);
  
  if (GlobalHandle == NULL)
  {
    return;
  }
  
  void* pBuffer = LockResource(GlobalHandle);
  
  uint32 datasize = size;
  char* pData = new char[datasize];
  memcpy(pData, pBuffer, size);
  
  // Init the nglIMemory we inherit from...
  mpBuffer = (char*)pData;
  mSize = datasize;
  mOffset = 0;
  
  UnlockResource(GlobalHandle);
  
  mValid = true;
#endif
#if defined _CARBON_ || defined _UIKIT_ || defined _LINUX_
  nglPath ResourcePath(GetResourcePath());
  ResourcePath += rPath;
  nglIFile File(ResourcePath, false);
  mValid = File.Open();
  
  uint32 datasize = File.Available();
  char* pData = new char[datasize];
  File.Read(pData, datasize, 1);
  
  mpBuffer = pData;
  mSize = datasize;
#endif // _CARBON_ || _UIKIT_ || _LINUX_
}

nuiNativeResource::~nuiNativeResource()
{
  delete[] mpBuffer;
}

bool nuiNativeResource::IsValid() const
{
  return mValid;
}


#ifdef _WIN32_
BOOL CALLBACK ResEnumerator(HMODULE hModule, LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam)
{
  std::vector<nglPath>* pResources = (std::vector<nglPath>*)lParam;
  nglString str(lpszName);
  str.ToLower();
  pResources->push_back(str);
  return TRUE;
}

bool nuiNativeResource::GetResourcesList(std::vector<nglPath>& rResources)
{
  return EnumResourceNames(App->GetHInstance(), _T("NUI_RESOURCE"), &::ResEnumerator, (LONG_PTR)&rResources) != FALSE;
}
#else
void RecurseChildren(std::vector<nglPath>& rResources, nglPath ResPath, nglPath BasePath)
{
//wprintf(_T("Enum resources BasePath '%ls'\n"), BasePath.GetChars());
  if (BasePath.IsLeaf())
  {
    nglString str(ResPath.GetPathName());
    //wprintf(_T("Added '%ls'\n"), str.GetChars());
    rResources.push_back(str);
    return;
  }
  
  std::list<nglPath> children;
  BasePath.GetChildren(&children);
  
  std::list<nglPath>::const_iterator it = children.begin();
  std::list<nglPath>::const_iterator end = children.end();
  
  while (it != end)
  {
    nglPath p(*it);
    nglString nodename(p.GetNodeName());
    nglPath child(ResPath);
    if (child.GetPathName().IsEmpty())
      child = nodename;
    else
      child += nodename;
    
    RecurseChildren(rResources, child, p);
    
    ++it;
  }
}


bool nuiNativeResource::GetResourcesList(std::vector<nglPath>& rResources)
{
  RecurseChildren(rResources, nglPath(), GetResourcePath());
  
  return true;
}
#endif
