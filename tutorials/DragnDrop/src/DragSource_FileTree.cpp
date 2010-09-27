/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "DragSource_FileTree.h"
#include "Application.h"
#include "DropDestination_Container.h"


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
  
  mEventSink.Connect(pFileTree->Activated, &DragFileTree::OnFileActivated, (void*)pFileTree);
}


DragFileTree::~DragFileTree()
{

}


void DragFileTree::OnFileActivated(const nuiEvent& rEvent)
{
  nuiFileTree* pFileTree = (nuiFileTree*)rEvent.mpUser;
  
  nglPath selectedPath = pFileTree->GetPath();
  nglString msg;
  msg.Format(_T("\nthe file '%ls' has been activated!\n"), selectedPath.GetChars());
  GetDropContainer()->Log(msg);
  
  // do what u want with the selected file here...
  
  rEvent.Cancel();
}
