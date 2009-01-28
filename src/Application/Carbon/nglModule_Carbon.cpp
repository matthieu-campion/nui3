/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglModule.h"


/*
 * Loading & unloading
 */

bool nglModule::Load()
{
  if (mHandle)
    return false;
  
  bool result = false;
  
  nglString PathNglString = mPath.GetPathName();
  CFStringRef PathString = PathNglString.ToCFString();
  CFURLRef Url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, PathString, kCFURLPOSIXPathStyle, true);
  
	if (Url)
  {
		mHandle = CFBundleCreate(kCFAllocatorDefault, Url);
		if (!mHandle) 
    {
			result = false;
		}
		else 
    {
			result = true;
		}
	}
  
  if (PathString)
    CFRelease(PathString);
  if (Url)
    CFRelease(Url);
  
  return result;
}

bool nglModule::Unload()
{
  if (!mHandle)
    return false;
  
  CFRelease(mHandle);
  mHandle = NULL;

  return true;
}


/*
 * Symbol retrieval
 */

void* nglModule::GetSymbol(const char* pName)
{
  if (!mHandle || !pName)
    return NULL;

  void* pResult = NULL;
  
  CFStringRef functionNameStringRef = CFStringCreateWithCString(kCFAllocatorDefault, pName, kCFStringEncodingUTF8);
  NGL_ASSERT(functionNameStringRef);
  
  pResult = CFBundleGetFunctionPointerForName(mHandle, functionNameStringRef);
  if (!pResult)
  {
    pResult = CFBundleGetDataPointerForName(mHandle, functionNameStringRef);
  }
  
  return pResult;
}
