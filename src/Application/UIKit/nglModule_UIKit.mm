/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


/*
 * Loading & unloading
 */

bool nglModule::Load()
{
  return false;
}

bool nglModule::Unload()
{
  return false;
}


/*
 * Symbol retrieval
 */

void* nglModule::GetSymbol(const char* pName)
{
  return NULL;
}
