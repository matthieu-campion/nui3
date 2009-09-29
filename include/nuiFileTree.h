/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nuiFileSelector.h"
#include "nuiTreeView.h"
#include "nglVolume.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiList.h"
#include "nuiSplitter.h"
#include "nglDragAndDropObjects.h"

/*
NOTE ABOUT DECORATIONS:

Object names have been assigned to the nuiFileSelector children, 
in order to get an easy way to set a decoration within a css file.
You just have to use the following labels as object names in your css file.

"nuiFileTree"
"nuiFileTree::VolumeLabel"
"nuiFileTree::FolderLabel"
"nuiFileTree::FileLabel"
"nuiFileTree::VolumeIcon"
"nuiFileTree::FolderIcon"
"nuiFileTree::FileIcon"
"nuiFileTree::FileIcon::[file extension]"
 
*/

#define ROOTPATH_ALLVOLUMES _T("*:")


class nuiFileTree : public nuiFileSelectorBase
{
public:
  
  nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const nglString& rFilter = _T("*"), bool showHiddenFiles = false);
  nuiFileTree(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles = false);
  virtual ~nuiFileTree();
  
  bool SetPath(const nglPath& rPath);      ///< Change current Path.
  bool SetRootPath(const nglPath& rPath);      ///< Change current root
  nglPath GetPath() const;          ///< Retrieve current Path
  nglPath GetRootPath() const;          ///< Retrieve current root.
  const nglPath& GetActivatedPath();

  virtual nuiTreeNode* GetNewNode(const nglPath& rPath);
  
  virtual nglDragAndDrop* OnDragStartDelegate(nuiTreeNode* pNode);
  virtual void OnDragRequestDataDelegate(nuiTreeNode* pNode, nglDragAndDrop* pDragObj, const nglString& rMimeType);
  virtual void OnDragStopDelegate(nuiTreeNode* pNode, bool canceled);
  
  nuiColor GetHandleColor();
  void SetHandleColor(nuiColor rColor);

  nuiScrollBar* GetScrollBar(nuiOrientation Orientation);
  
  nuiSimpleEventSource<nuiWidgetActivated> PathChanged;      ///< Event triggered when the user is navigating around the file system.
  nuiSimpleEventSource<nuiWidgetActivated> SelectionChanged; 
  nuiSimpleEventSource<nuiWidgetActivated> Activated; 
  
private:

  void Init(const nglPath& rPath, const nglPath& rRootPath, const std::list<nglString>& rFilters, bool showHiddenFiles);

  void InitAttributes();
  
  void AddTree(const nglPath& rPath, bool Opened);

  nglString GetFileInfo(const nglPath& rPath);
  void FormatFileSize(nuiSize size, nglString& str);
  
  bool OnNodeActivated(const nuiEvent& rEvent);
  bool OnGotoParentFolder(const nuiEvent& rEvent);
  
  bool isRoot(const nglPath& rPath);
  
  nuiVBox* mpFileBox;
  nuiTreeView* mpTreeView;
  nuiTreeNode* mpTreeRoot;
  std::map<nglPath, nuiTreeNode*> mTrees; // rootpath => treeview
  nuiTreeNode* mpCurrentTree;
  nglPath mRootPath;
  nglPath mActivatedPath;
  std::stack<nglPath> mWalkthrough;
  nuiScrollView* mpScrollView;
  
  nglString mPath;
  nuiEventSink<nuiFileTree> mEventSink;
  
};


