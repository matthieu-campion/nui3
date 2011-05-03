/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiNavigationButton.h"



nuiNavigationButton::nuiNavigationButton(nuiBarButtonSystemItem systemItem)
: nuiButton()
{
  nglString text;
  
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
  
  if (!text.Empty())
  {
    nuiLabel* pLabel = new nuiLabel(text);
    AddChild(pLabel);
    return;
  }
  
  switch (systemItem)
  {
//    case eBarButtonSystemItemFlexibleSpace,
//    case eBarButtonSystemItemFixedSpace,
    case eBarButtonSystemItemCompose : 
      eBarButtonSystemItemAction,
      
      
      ,
      ,
      ,
      ,
      ,
      ,
      ,
      eBarButtonSystemItemOrganize,
      eBarButtonSystemItemBookmarks,
      eBarButtonSystemItemSearch,
      eBarButtonSystemItemRefresh,
      eBarButtonSystemItemStop,
      eBarButtonSystemItemCamera,
      eBarButtonSystemItemTrash,
      eBarButtonSystemItemPlay,
      eBarButtonSystemItemPause,
      eBarButtonSystemItemRewind,
      eBarButtonSystemItemFastForward,
      ,
      ,
      eBarButtonSystemItemPageCurl

  }
}


nuiNavigationButton::nuiNavigationButton(const nglString& rText)
: nuiButton()
{
  nuiLabel* pLabel = new nuiLabel(rText);
  AddChild(pLabel);
 
}


nuiNavigationButton::nuiNavigationButton(const nglPath& rIconTexture)
: nuiButton()
{
  nuiTexture* pTex = nuiTexture::GetTexture(rIconTexture);
  AddChild(new nuiImage(pTex));
}



// virtual 
nuiNavigationButton::~nuiNavigationButton()
{

}


  
