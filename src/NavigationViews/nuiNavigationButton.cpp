/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"


nuiNavigationButton::nuiNavigationButton(nuiBarButtonSystemItem systemItem)
: nuiButton()
{
  nglString text;
  SetObjectClass(_T("nuiNavigationButton"));
  //LBDEBUG TODO
  
  switch (systemItem)
  {
    case eBarButtonSystemItemDone: text = _T("Done"); break;
    case eBarButtonSystemItemCancel: text = _T("Cancel"); break;
    case eBarButtonSystemItemEdit: text = _T("Edit"); break;
    case eBarButtonSystemItemSave: text = _T("Save"); break;
    case eBarButtonSystemItemAdd: text = _T("Add"); break;
    case eBarButtonSystemItemReply: text = _T("Reply"); break;
    case eBarButtonSystemItemUndo: text = _T("Undo"); break;
    case eBarButtonSystemItemRedo: text = _T("Redo"); break;
  }
  
  if (!text.IsEmpty())
  {
    nuiLabel* pLabel = new nuiLabel(text);
    AddChild(pLabel);
    return;
  }
  
//  switch (systemItem)
//  {
////    case eBarButtonSystemItemFlexibleSpace,
////    case eBarButtonSystemItemFixedSpace,
////      eBarButtonSystemItemPageCurl
//    case eBarButtonSystemItemCompose : break;
//    case eBarButtonSystemItemAction : break; 
//    case eBarButtonSystemItemOrganize : break;
//    case eBarButtonSystemItemBookmarks : break;
//    case eBarButtonSystemItemSearch : break;
//    case eBarButtonSystemItemRefresh : break;
//    case eBarButtonSystemItemStop : break;
//    case eBarButtonSystemItemCamera : break;
//    case eBarButtonSystemItemTrash : break;
//    case eBarButtonSystemItemPlay : break;
//    case eBarButtonSystemItemPause : break;
//    case eBarButtonSystemItemRewind : break;
//    case eBarButtonSystemItemFastForward : break;
//
//  }
}


nuiNavigationButton::nuiNavigationButton()
: nuiButton()
{
  SetObjectClass(_T("nuiNavigationButton"));
}

nuiNavigationButton::nuiNavigationButton(const nglString& rText)
: nuiButton()
{
  SetObjectClass(_T("nuiNavigationButton"));
  nuiLabel* pLabel = new nuiLabel(rText);
  AddChild(pLabel);
}


nuiNavigationButton::nuiNavigationButton(nuiTexture* pIconTexture)
: nuiButton()
{
  SetObjectClass(_T("nuiNavigationButton"));
  AddChild(new nuiImage(pIconTexture));
}



// virtual 
nuiNavigationButton::~nuiNavigationButton()
{

}


  
