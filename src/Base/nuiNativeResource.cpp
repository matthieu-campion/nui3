/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot

 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"


#ifdef _WIN32_
# include "nglIMemory.h"
#endif // _WIN32_

#ifdef _ANDROID_
#include "nglIMemory.h"
#include "android/asset_manager.h"
#include <android_native_app_glue.h>
#endif // _ANDROID_

#if (defined _CARBON_) || (defined _UIKIT_) || (defined _COCOA_)
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

#include <dlfcn.h>
nglPath nuiGetNativeResourcePath()
{
  CFBundleRef bundle = NULL;

#ifndef _UIKIT_
  Dl_info inf;
  dladdr((void*)nuiGetNativeResourcePath, &inf);

  const char* imagename = inf.dli_fname;
  /* get the bundle of a header */
  if (imagename)
    bundle = _CFXBundleCreateFromImageName (NULL, imagename);
#endif

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

#if defined _LINUX_ || defined _MINUI3_
nglPath nuiGetNativeResourcePath()
{
  nglString ResourcePathName(getenv("NUI_RESOURCE_PATH"));
  if (ResourcePathName.IsEmpty())
    ResourcePathName = nglString("/usr/share/");
  if( ResourcePathName.GetLastChar() == _T('/'))
    ResourcePathName += App->GetName();
  else
    ResourcePathName += _T('/')+App->GetName();
  nglPath p(ResourcePathName);
  if (!p.Exists() || p.IsLeaf())
  {
    p = nglPath(ePathApp);
    NGL_OUT(_T("application path: %s\n"), p.GetChars());
    p = p.GetParent();
    NGL_OUT(_T("application folder: %s\n"), p.GetChars());
    p = p + nglPath(_T("resources"));
    NGL_OUT(_T("Couldn't find global resource path, looking for it next to the application: %s\n"), p.GetChars());
  }
  NGL_OUT(_T("NUI_RESOURCE_PATH: %s\n"), p.GetChars());
  return p;
}
#endif //_LINUX_

#if defined _ANDROID_
nglPath nuiGetNativeResourcePath()
{
  nglPath p("");
  return p;
}
#endif

nuiNativeResource::nuiNativeResource(const nglPath& rPath)
: nglIStream(),
  mpIStream(NULL)
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
  mpIStream = new nglIMemory((char*)pData, datasize, true);

  UnlockResource(GlobalHandle);

  mValid = true;
#endif
#if defined _CARBON_ || defined _UIKIT_ || defined _COCOA_ || defined _LINUX_
  nglPath resourcePath(nuiGetNativeResourcePath());
  resourcePath += rPath;

  if (resourcePath.Exists())
  {
    mpIStream = resourcePath.OpenRead();
    if (!mpIStream)
      NGL_OUT(_T("nuiNativeResource opening failed with path '%s'\n"), resourcePath.GetChars());
    NGL_ASSERT(mpIStream);
    mValid = true;
  }
#endif // _CARBON_ || _UIKIT_ || _COCOA_ || _LINUX_

#ifdef _ANDROID_
  nglString dir = rPath.GetParentName();
  nglString file = rPath.GetNodeName();

  dir.TrimLeft(_T("/"));

  char* dirChar = dir.Export();
  char* fileChar = file.Export();
  if (!dirChar || !fileChar)
  {
    return;
  }

  AAssetManager* pManager = App->GetAndroidApp()->activity->assetManager;
  AAssetDir* pDir = AAssetManager_openDir(pManager, dirChar);
  AAsset* pAsset = NULL;
  if (pDir)
  {
    const char* f = AAssetDir_getNextFileName(pDir);
    while (f && !mValid)
    {
      if (strcmp(f, fileChar) == 0)
      {
        nglString resource;
        if (dir.IsEmpty())
          resource = file;
        else
          resource = dir + nglString(_T("/")) + file;
        char* resourceChar = resource.Export();
        pAsset = AAssetManager_open(pManager, resourceChar, AASSET_MODE_BUFFER);
        free(resourceChar);
        if (pAsset)
        {
          const void* pBuffer = AAsset_getBuffer(pAsset);
          uint32 datasize = AAsset_getLength(pAsset);
          if (pBuffer && datasize != 0)
          {
            char* pData = new char[datasize];
            memcpy(pData, pBuffer, datasize);
            mpIStream = new nglIMemory(pData, datasize, true/*own buffer*/);
            mValid = true;
          }
        }
      }
      f = AAssetDir_getNextFileName(pDir);
    }
  }

  if (pAsset)
    AAsset_close(pAsset);
  if (pDir)
    AAssetDir_close(pDir);

  free(dirChar);
  free(fileChar);
#endif // _ANDROID_
}

nuiNativeResource::~nuiNativeResource()
{
  delete mpIStream;
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
#elif defined (_ANDROID_)
bool nuiNativeResource::GetResourcesList(std::vector<nglPath>& rResources)
{
  nuiNativeResource resourcesList(_T("resource.android"));
  nglString line;
  int64 read = resourcesList.ReadLine(line);
  while (read)
  {
    rResources.push_back(line);
    read = resourcesList.ReadLine(line);
  }
  return true;
}
#else
void RecurseChildren(std::vector<nglPath>& rResources, nglPath ResPath, nglPath BasePath)
{
//wprintf(_T("Enum resources BasePath '%s'\n"), BasePath.GetChars());
  if (BasePath.IsLeaf())
  {
    nglString str(ResPath.GetPathName());
    //wprintf(_T("Added '%s'\n"), str.GetChars());
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
  RecurseChildren(rResources, nglPath(), nuiGetNativeResourcePath());

  return true;
}
#endif

/*
 * Wrapping the internal IStream
 */

nglStreamState nuiNativeResource::GetState() const
{
  return mpIStream->GetState();
}
nglFileOffset nuiNativeResource::GetPos() const
{
  return mpIStream->GetPos();
}
nglFileOffset nuiNativeResource::SetPos (nglFileOffset Where, nglStreamWhence Whence)
{
  return mpIStream->SetPos(Where, Whence);
}
nglFileSize nuiNativeResource::Available (uint WordSize)
{
  return mpIStream->Available(WordSize);
}
int64 nuiNativeResource::ReadUInt8  (uint8*  pData, int64 Count)
{
  return mpIStream->ReadUInt8(pData, Count);
}
int64 nuiNativeResource::ReadUInt16 (uint16* pData, int64 Count)
{
  return mpIStream->ReadUInt16(pData, Count);
}
int64 nuiNativeResource::ReadUInt32 (uint32* pData, int64 Count)
{
  return mpIStream->ReadUInt32(pData, Count);
}
int64 nuiNativeResource::ReadUInt64 (uint64* pData, int64 Count)
{
  return mpIStream->ReadUInt64(pData, Count);
}
int64 nuiNativeResource::ReadInt8   (int8*  pData, int64 Count)
{
  return mpIStream->ReadInt8(pData, Count);
}
int64 nuiNativeResource::ReadInt16  (int16* pData, int64 Count)
{
  return mpIStream->ReadInt16(pData, Count);
}
int64 nuiNativeResource::ReadInt32  (int32* pData, int64 Count)
{
  return mpIStream->ReadInt32(pData, Count);
}
int64 nuiNativeResource::ReadInt64  (int64* pData, int64 Count)
{
  return mpIStream->ReadInt64(pData, Count);
}
int64 nuiNativeResource::ReadFloat  (float*  pData, int64 Count)
{
  return mpIStream->ReadFloat(pData, Count);
}
int64 nuiNativeResource::ReadDouble (double* pData, int64 Count)
{
  return mpIStream->ReadDouble(pData, Count);
}
int64 nuiNativeResource::ReadLine   (nglString& rLine, nglTextFormat* pFormat)
{
  return mpIStream->ReadLine(rLine, pFormat);
}
int64 nuiNativeResource::Read (void* pData, int64 WordCount, uint WordSize)
{
  return mpIStream->Read(pData, WordCount, WordSize);
}
int64 nuiNativeResource::Peek (void* pData, int64 WordCount, uint WordSize)
{
  return mpIStream->Peek(pData, WordCount, WordSize);
}

void nuiNativeResource::SetEndian(nglEndian Endian)
{
  mpIStream->SetEndian(Endian);
  nglIStream::SetEndian(Endian);
}

bool nuiNativeResource::SetTextEncoding(nglTextEncoding Encoding)
{
  mpIStream->SetTextEncoding(Encoding);
  return nglIStream::SetTextEncoding(Encoding);
}

void nuiNativeResource::SetTextFormat(nglTextFormat Format)
{
  mpIStream->SetTextFormat(Format);
  nglIStream::SetTextFormat(Format);
}

