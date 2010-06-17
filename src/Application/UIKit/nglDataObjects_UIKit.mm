/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataObjects.h"


void nglDataFilesObject::AddFile(const nglString& rPath)
{
  nglPath path(rPath);
 
  mFilePaths.push_back(path.GetPathName());
}
  

