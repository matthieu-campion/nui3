/*
 NUI - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 SÈbastien MÈtrot

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// #FIXME TO DO

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
  };

  ~nuiMainMenuItemPrivate()
  {

  };

  bool mIsMenu;       ///< true if the item is a menu
  bool mIsSubMenu;    ///< true if the item is a submenu

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
  delete mpPrivate;
}


void nuiMainMenuItem::SetEnabled(bool set)
{
}



void nuiMainMenuItem::SetChecked(bool set)
{
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
};


nuiMainMenuPrivate::nuiMainMenuPrivate()
{
}

nuiMainMenuPrivate::~nuiMainMenuPrivate()
{
}


bool nuiMainMenuPrivate::IsWindowRegistered(nglWindow* pWindow)
{
  return false;
}

void nuiMainMenuPrivate::RegisterWindow(nglWindow* pWindow)
{
}

void nuiMainMenuPrivate::UnregisterWindow(nglWindow* pWindow)
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

bool nuiMainMenu::RegisterFromWindow(nglWindow* pWindow)
{
//#TODO implement native menus on unix
return false;
//	if (!pWindow)
//	    return false;
//
//	  if (mpPrivate->IsWindowRegistered(pWindow))
//	    return false;
//
//	  mpPrivate->RegisterWindow(pWindow);
//
//	  return true;
}

bool nuiMainMenu::UnregisterFromWindow(nglWindow* pWindow)
{
	if (!pWindow)
	    return false;

	  mpPrivate->UnregisterWindow(pWindow);

	  return true;
}

nuiMainMenu::~nuiMainMenu()
{
  mRefCount--;
  delete mpPrivate;
  delete mpRoot;
}


// an item has been added in the widget tree. Do the system-side of the operation (add a menu or add a menu item)
void nuiMainMenu::OnItemAdded(const nuiEvent& rEvent)
{
  //FIXME
}



void nuiMainMenu::InsertMenu(nuiMainMenuItem* pItem)
{
//FIXME
}



void nuiMainMenu::InsertItem(nuiMainMenuItem* pParent, nuiMainMenuItem* pItem)
{
//FIXME
}



void nuiMainMenu::SetItemText(nuiMainMenuItem* pItem, const nglString& rText)
{
  //FIXME
}
