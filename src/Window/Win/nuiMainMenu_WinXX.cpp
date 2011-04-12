/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiMainMenu.h"



//*****************************************************************************************
//
// nuiMainMenuItemPrivate
//
//*****************************************************************************************

class nuiMainMenuItemPrivate
{
  friend class nuiMainMenuItem;
  friend class nuiMainMenu;
  
protected:
  nuiMainMenuItemPrivate()
  {
    mIsMenu    = false;
    mIsSubMenu = false;
    mhParentMenu = 0;
    mhMenu = 0;
    
  };
  
  ~nuiMainMenuItemPrivate()
  {
    
  };
  
  bool mIsMenu;       ///< true if the item is a menu
  bool mIsSubMenu;    ///< true if the item is a submenu
  HMENU mhParentMenu; ///< in our tree scheme, the menubar is not a parent of a menu, but we still need to have access to its handle, so let's store it right here
  HMENU mhMenu;       ///< the menu handle for a menu or a submenu
  
};


//*****************************************************************************************
//
// nuiMainMenuItem
//
//*****************************************************************************************

nuiMainMenuItem::nuiMainMenuItem(const nglString& rLabel, nuiMainMenuItemType type)
: nuiValueTree<nglString>(rLabel)
{
  mType = type;
  mEnabled = true;
  mChecked = false;
  mIsBeingDeleted = false;
  
  mpPrivate = new nuiMainMenuItemPrivate();
}


nuiMainMenuItem::~nuiMainMenuItem()
{
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)GetParent();
  if (pParent && !pParent->mIsBeingDeleted)
  {
    if (mpPrivate->mIsMenu)
      DestroyMenu(mpPrivate->mhMenu);
    else
    {
      uint32 index = ComputeIndexInParent();
      DeleteMenu(mpPrivate->mhParentMenu, index, MF_BYPOSITION);
    }
  }

  if (pParent)
    mIsBeingDeleted = true;
  
  delete mpPrivate;
}


void nuiMainMenuItem::SetEnabled(bool set)
{
  mEnabled = set;
  
  UINT flag = (set)? MF_ENABLED : MF_GRAYED;

  if (mpPrivate->mIsMenu || mpPrivate->mIsSubMenu)
    EnableMenuItem(mpPrivate->mhParentMenu, (UINT)mpPrivate->mhMenu, flag);
  else
    EnableMenuItem(mpPrivate->mhParentMenu, (UINT)this, flag);
}



void nuiMainMenuItem::SetChecked(bool set)
{
  if (mpPrivate->mIsMenu || mpPrivate->mIsSubMenu)
    return;

  mChecked = set;

  UINT flag = (set)? MF_CHECKED : MF_UNCHECKED;
    
  CheckMenuItem(mpPrivate->mhParentMenu, (UINT)this, flag);

  // send event
  if (mChecked)
    Checked();
  else
    Unchecked();
}



void nuiMainMenuItem::SetText(const nglString& rText)
{
   uint32 flags = MF_BYCOMMAND | MF_STRING;

   if (mChecked)
     flags |= MF_CHECKED;
   else
     flags |= MF_UNCHECKED;
 
   if (mEnabled)
     flags |= MF_ENABLED;
   else
     flags |= MF_GRAYED;
 
   bool success = ModifyMenu(mpPrivate->mhParentMenu, (UINT)this, flags, (UINT)this, rText.GetChars());
 
   //if (!success)
   //{
   //  int error = GetLastError();
   //  NGL_OUT(nglString(error).GetChars());
   //}
}

void nuiMainMenuItem::MakeSubMenu(uint32 unused)
{
  int32 index = ComputeIndexInParent();
  
  // first, delete the existing system item
  DeleteMenu(mpPrivate->mhParentMenu, (UINT)this, MF_BYCOMMAND);
  
  // then, replace it by a submenu
  const nglString& rItemLabel = GetElement();

  mpPrivate->mIsSubMenu = true;
  mpPrivate->mhMenu = CreatePopupMenu();
  InsertMenu(mpPrivate->mhParentMenu, index, MF_BYPOSITION|MF_STRING|MF_POPUP, (UINT)mpPrivate->mhMenu, rItemLabel.GetChars());

  // re-apply the enabled state
  SetEnabled(mEnabled);  
}


//*****************************************************************************************
//
// nuiMainMenuPrivate
//
//*****************************************************************************************


class nuiMainMenuPrivate
{
  friend class nuiMainMenu;
  
protected:

  nuiMainMenuPrivate();
  ~nuiMainMenuPrivate();

  bool IsWindowRegistered(nglWindow* pWindow);
  void RegisterWindow(nglWindow* pWindow);
  void UnregisterWindow(nglWindow* pWindow);

  void Draw();

  HMENU mhMenu;
  std::list<nglWindow*> mWindows;
};


nuiMainMenuPrivate::nuiMainMenuPrivate()
{
  mhMenu = CreateMenu();
}

nuiMainMenuPrivate::~nuiMainMenuPrivate()
{
  DestroyMenu(mhMenu);

}


bool nuiMainMenuPrivate::IsWindowRegistered(nglWindow* pWindow)
{
  std::list<nglWindow*>::iterator it;
  for (it = mWindows.begin(); it != mWindows.end(); ++it)
  {
    nglWindow* pnglWindow = *it;
    if (pnglWindow == pWindow)
      return true;
  }
  return false;
}

void nuiMainMenuPrivate::RegisterWindow(nglWindow* pWindow)
{
  mWindows.push_back(pWindow);
  HWND hWindow = pWindow->GetOSInfo()->WindowHandle;
  SetMenu(hWindow, mhMenu);
}

void nuiMainMenuPrivate::UnregisterWindow(nglWindow* pWindow)
{
  std::list<nglWindow*>::iterator it;
  for (it = mWindows.begin(); it != mWindows.end(); ++it)
  {
    nglWindow* pnglWindow = *it;
    if (pnglWindow == pWindow)
    {
      mWindows.erase(it);
      return;
    }
  }
}

void nuiMainMenuPrivate::Draw()
{
  std::list<nglWindow*>::iterator it;
  for (it = mWindows.begin(); it != mWindows.end(); ++it)
  {
    nglWindow* pnglWindow = *it;
    HWND hWindow = pnglWindow->GetOSInfo()->WindowHandle;
    DrawMenuBar(hWindow);
  }

}


//*****************************************************************************************
//
// nuiMainMenu
//
//*****************************************************************************************


void nuiMainMenu::Init()
{  
  mRefCount++;

  mpPrivate = new nuiMainMenuPrivate();
}


nuiMainMenu::~nuiMainMenu()
{
  mRefCount--;
  delete mpPrivate;
  delete mpRoot;
}

bool nuiMainMenu::RegisterFromWindow(nglWindow* pWindow)
{
  if (!pWindow)
    return false;

  if (mpPrivate->IsWindowRegistered(pWindow))
    return false;

  mpPrivate->RegisterWindow(pWindow);

  return true;
}

bool nuiMainMenu::UnregisterFromWindow(nglWindow* pWindow)
{
  if (!pWindow)
    return false;

  mpPrivate->UnregisterWindow(pWindow);

  return true;
}


void nuiMainMenu::OnItemAdded(const nuiEvent& rEvent)
{
  // an item has been added to the tree
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)pTreeEvent->mpParent;
  nuiMainMenuItem* pItem = (nuiMainMenuItem*)pTreeEvent->mpChild;
  
  if (pParent == mpRoot)
    InsertMenu(pItem);
  else
    if (pParent->mpPrivate->mIsMenu)
      InsertItem(pParent, pItem);
    else 
    {
      if (!pParent->mpPrivate->mIsSubMenu)
        pParent->MakeSubMenu(0/*unused*/);
      
      InsertItem(pParent, pItem);
    }

  mpPrivate->Draw();

  rEvent.Cancel();
}



void nuiMainMenu::InsertMenu(nuiMainMenuItem* pItem)
{
  const nglString& rItemLabel = pItem->GetElement();

  // create a menu in the menu bar
  HMENU hMenu = CreatePopupMenu();
  AppendMenu(mpPrivate->mhMenu, MF_STRING|MF_POPUP, (UINT)hMenu, rItemLabel.GetChars());

  // gives the item a token : the hMenu handle
  pItem->mpPrivate->mIsMenu = true;
  pItem->mpPrivate->mhMenu = hMenu;
  pItem->mpPrivate->mhParentMenu = mpPrivate->mhMenu;
}



void nuiMainMenu::InsertItem(nuiMainMenuItem* pParent, nuiMainMenuItem* pItem)
{
  const nglString& rItemLabel = pItem->GetElement();

  // create an item in a menu
  pItem->mpPrivate->mhParentMenu = pParent->mpPrivate->mhMenu;

  if (pItem->mType == eItemSeparator)
    AppendMenu(pItem->mpPrivate->mhParentMenu, MF_SEPARATOR, (UINT)pItem, rItemLabel.GetChars());
  else
    AppendMenu(pItem->mpPrivate->mhParentMenu, MF_STRING, (UINT)pItem, rItemLabel.GetChars());
}


void nuiMainMenu::SetItemText(nuiMainMenuItem* pItem, const nglString& rText)
{
  if (pItem->GetType() != eItemString)
    return;

  pItem->SetText(rText);
  mpPrivate->Draw();
}
