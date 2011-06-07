/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglString.h"
#include "nglDataObjects.h"
#include <map>
#include <list>







void nglDataFilesObject::AddFile(const nglString& rPath)
{
  nglPath path(rPath);

// LBDEBUG FIXME see forward into the PromiseFiles question
//  if (GetNativeType() == kDragFlavorTypePromiseHFS)
//  {
//    nglPath folderPath(ePathUser);
//    char str[1024];
//    int32 len = 1024;
//    int32 start = 0;
//    folderPath.GetPathName().Export(start, str, len, eUTF8);
//    str[folderPath.GetPathName().GetLength()] = 0;
//    
//    FSRef destFolder;
//    OSStatus err = FSPathMakeRef(reinterpret_cast<const UInt8*>(str), &destFolder, NULL);
//    NGL_ASSERT(err == noErr);
//    
//    // replace tmp path with new one
//    nglPath newPath = folderPath + path.GetNodeName();
//    path = newPath.GetChars();
//    
//    FSRef fsref;
//    len = 1024;
//    start = 0;
//    path.GetPathName().Export(start, str, len, eUTF8);
//    str[path.GetPathName().GetLength()] = 0;
//    err = FSPathMakeRef(reinterpret_cast<const UInt8*>(str), &fsref, NULL);
//    NGL_ASSERT(err == noErr);
//    err = FSCopyObjectSync(&fsref, &destFolder, NULL, NULL, 0);
//    NGL_ASSERT(err == noErr);
//  }
  
  mFilePaths.push_back(path.GetPathName());
}
  

