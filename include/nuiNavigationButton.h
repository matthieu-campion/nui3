/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#pragma once
#include "nui.h"
#include "nuiNavigationBar.h"

enum nuiBarButtonSystemItem
{
  eBarButtonSystemItemDone,
  eBarButtonSystemItemCancel,
  eBarButtonSystemItemEdit,
  eBarButtonSystemItemSave,
  eBarButtonSystemItemAdd,
  eBarButtonSystemItemFlexibleSpace,
  eBarButtonSystemItemFixedSpace,
  eBarButtonSystemItemCompose,
  eBarButtonSystemItemReply,
  eBarButtonSystemItemAction,
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
  eBarButtonSystemItemUndo,
  eBarButtonSystemItemRedo,
  eBarButtonSystemItemPageCurl
};



class nuiNavigationButton : public nuiButton
{
public:
  nuiNavigationButton(nuiBarButtonSystemItem systemItem);
  nuiNavigationButton(const nglString& rText);
  nuiNavigationButton(nuiTexture* pIconTexture);
  nuiNavigationButton();
  virtual ~nuiNavigationButton();
  
private:
  
  
  
};






