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
    NGL_LOG(_T("module"), NGL_LOG_ERROR, _T("error: %ls"), buffer);

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
