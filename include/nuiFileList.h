/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFileList_h__
#define __nuiFileList_h__

#include "nuiList.h"

/// This class implements a simple list of file or directory that a user can easily browse. 
class NUI_API nuiFileList : public nuiList
{
public:
  nuiFileList(const nglPath& rPath = ePathCurrent);
  virtual ~nuiFileList();

  void Selected(const nuiEvent& rEvent); ///< Called whenever a file is double clicked.

protected:
  nuiEventSink<nuiFileList> mFileListSink;
};

#endif // __nuiFileList_h__
