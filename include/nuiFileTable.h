/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFileTable_h__
#define __nuiFileTable_h__

#include "nuiTable.h"

class NUI_API nuiFileTable : public nuiTable
{
public:
  nuiFileTable(const nglPath& rPath = ePathCurrent);
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
  virtual ~nuiFileTable();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;

  void SetShowSize(bool Show);
  bool GetShowSize();
  void SetShowType(bool Show);
  bool GetShowType();
  void SetShowDirs(bool Show);
  bool GetShowDirs();
  void SetShowFiles(bool Show);
  bool GetShowFiles();

  bool Selected(const nuiEvent& rEvent); ///< Called whenever a file is double clicked.
  bool KeyDown(const nglKeyEvent& rEvent); ///< Complementing nuiTable (Backspace, etc...).

  bool Populate(const nglPath& rPath);
  nglPath GetPath();
protected:
  nuiEventSink<nuiFileTable> mFileTableSink;

  nglPath mPath;

  bool mShowFiles;
  bool mShowDirs;

  bool mShowSize;
  bool mShowType;

  int mNameColumn;
  int mSizeColumn;
  int mTypeColumn;
};

#endif // __nuiFileTable_h__
