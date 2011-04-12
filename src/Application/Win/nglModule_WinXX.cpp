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

  const nglChar* filename = mPath.GetPathName().GetChars();
  if (!filename || !*filename)
  {
    SetError(_T("module"), NGL_MODULE_EPATH);
    return false;
  }

  #define BUFFER_LEN 1023
  DWORD len;
  nglChar buffer[BUFFER_LEN+1];
  nglChar* slash = NULL;

  buffer[BUFFER_LEN] = 0;
  nglString fullpath;

  len = SearchPath (NULL, filename, NULL, BUFFER_LEN, buffer, &slash);
  if (slash)
    *slash = 0;
  fullpath = len > 0 ? buffer : _T("?");
  NGL_DEBUG( NGL_LOG(_T("module"), NGL_LOG_INFO, _T("loading '%s' from folder '%s'"), mPath.GetChars(), fullpath.GetChars()); )

  mHandle = LoadLibrary(filename);
  if (!mHandle)
  {
    SetError(_T("module"), NGL_MODULE_ELOAD);

    nglString errormsg;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, 
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      buffer,
      BUFFER_LEN,
      NULL);
    errormsg = buffer;
    NGL_LOG(_T("module"), NGL_LOG_ERROR, _T("error: %s"), buffer);

    return false;
  }

  return true;
}

bool nglModule::Unload()
{
  if (!mHandle)
    return false;

  return FreeLibrary(mHandle) ? true : false;
}


/*
 * Symbol retrieval
 */

void* nglModule::GetSymbol(const char* pName)
{
  if (!mHandle || !pName)
    return NULL;

  return GetProcAddress(mHandle, pName);
}
