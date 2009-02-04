/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "DragSource_FileTree.h"



DragFileTree::DragFileTree()
: nuiSimpleContainer(), mEventSink(this)
{
  SetObjectName(_T("DragFileTree"));
  
  mpView = new nuiScrollView(true, true);
  AddChild(mpView);
  
  std::list<nglString> filters;
  filters.push_back(_T("*"));
  
  nuiFileTree* pFileTree = new nuiFileTree(nglPath(ePathUser), ROOTPATH_ALLVOLUMES, filters, false);
  mpView->AddChild(pFileTree);
}


DragFileTree::~DragFileTree()
{

}


