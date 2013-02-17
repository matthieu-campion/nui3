/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

const nglChar* gpModuleErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Invalid path"),
/*  2 */ _T("File not found or corrupted"),
         NULL
};


/*
 * Life cycle
 */

nglModule::nglModule(const nglPath& rPath)
{
  mPath = rPath;
  mHandle = NULL;
}

nglModule::~nglModule()
{
  Unload();
}

bool nglModule::IsLoaded()
{
  return (mHandle != NULL);
}


/*
 * Error callback
 */

const nglChar* nglModule::OnError (uint& rError) const
{
  return FetchError(gpModuleErrorTable, NULL, rError);
}
