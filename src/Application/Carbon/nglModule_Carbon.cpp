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
  NGL_LOG(_T("module"), NGL_LOG_WARNING, _T("Load('%ls'): not implemented"), mPath.GetChars());
  return false;
}

bool nglModule::Unload()
{
  NGL_LOG(_T("module"), NGL_LOG_WARNING, _T("Unload('%ls'): not implemented"), mPath.GetChars());
  return false;
}


/*
 * Symbol retrieval
 */

void* nglModule::GetSymbol(const char* pName)
{
  NGL_LOG(_T("module"), NGL_LOG_WARNING, _T("GetSymbol('%ls:%ls'): not implemented"), mPath.GetChars(), pName);
  return NULL;
}
