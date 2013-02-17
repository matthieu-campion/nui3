/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"



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
  };
  
  ~nuiMainMenuItemPrivate()
  {
    
  };
  
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
  }
  
  if (pParent)
    mIsBeingDeleted = true;
  
  delete mpPrivate;
}


void nuiMainMenuItem::SetEnabled(bool set)
{
  mEnabled = set;
}



void nuiMainMenuItem::SetChecked(bool set)
{
  mChecked = set;
  
  // send event
  if (mChecked)
    Checked();
  else
    Unchecked();
}



void nuiMainMenuItem::SetText(const nglString& rText)
{
}

void nuiMainMenuItem::MakeSubMenu(uint32 unused)
{
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
  
  std::list<nglWindow*> mWindows;
};


nuiMainMenuPrivate::nuiMainMenuPrivate()
{
}

nuiMainMenuPrivate::~nuiMainMenuPrivate()
{
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
  rEvent.Cancel();
}



void nuiMainMenu::InsertMenu(nuiMainMenuItem* pItem)
{
}



void nuiMainMenu::InsertItem(nuiMainMenuItem* pParent, nuiMainMenuItem* pItem)
{
}


void nuiMainMenu::SetItemText(nuiMainMenuItem* pItem, const nglString& rText)
{
}

