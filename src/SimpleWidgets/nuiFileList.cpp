/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiFileList.h"
#include "nuiLabel.h"
#include "nuiXML.h"


nuiFileList::nuiFileList(const nglPath& rPath)
  : nuiList(nuiVertical),
    mFileListSink(this)
{
  SetObjectClass(_T("nuiFileList"));
  nuiLabel* pLabel = new nuiLabel(_T(".."));
  pLabel->SetProperty(_T("Path"), rPath.GetParent().GetAbsolutePath().GetPathName());

  Populate(rPath);
  mFileListSink.Connect(Activated, &nuiFileList::Selected, this); 
}

bool nuiFileList::Load(const nuiXMLNode* pNode)
{
  nuiList::Load(pNode);
  SetObjectClass(_T("nuiFileList"));
  nglString path;
  path = nuiGetString(pNode, _T("Path"), _T("."));
  if (path.GetRight(2) == _T("/.") || path.GetRight(2) == _T("\\."))
    path.DeleteRight(2);
  else if (path == _T("."))
    path = nglPath(ePathCurrent).GetAbsolutePath().GetPathName();

  nuiLabel* pLabel = new nuiLabel(_T(".."));
  pLabel->SetProperty(_T("Path"),nglPath(path).GetParent().GetAbsolutePath().GetPathName());

  Populate(nglPath(path));
  mFileListSink.Connect(Activated, &nuiFileList::Selected, this); 
  
  return true;
}

nuiXMLNode* nuiFileList::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiList::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
  pNode->SetAttribute(_T("Path"),GetProperty(_T("Path")));
  return pNode;
}

nuiFileList::~nuiFileList()
{
}

bool nuiFileList::Selected(const nuiEvent& rEvent)
{
  nuiWidgetList Items;
//  NGL_OUT(_T("Selected!\n"));
  if (GetSelected(Items))
  {
    nuiWidgetPtr pItem = Items.front();
    if (pItem)
    {
      nglPath path(pItem->GetProperty(_T("Path")));
      if (!path.IsLeaf())
      {
//        NGL_OUT(_T("Exploring %ls\n"),path.GetAbsolutePath().GetChars());
        Clear();
        nuiLabel* pLabel = new nuiLabel(_T(".."));
        pLabel->SetProperty(_T("Path"),path.GetParent().GetAbsolutePath().GetPathName());
      
        Populate(path);
      }
//      else
//      {
//        NGL_OUT(_T("'%ls' is a file:\nSize: %d\nType: %ls\n\n"),path.GetAbsolutePath().GetChars(),path.GetSize(),path.GetMimeType().GetChars());
//      }
    }
//    else
//      NGL_OUT(_T("Selected item is NULL!!! (?)\n"));
  }
//  else
//    NGL_OUT(_T("?!? No item selected!?!\n"));

  return false;
}


