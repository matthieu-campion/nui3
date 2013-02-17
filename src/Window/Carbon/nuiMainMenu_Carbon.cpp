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
    mIsMenu    = false;
    mIsSubMenu = false;
    mpMenuRef = NULL;
    mMenuID = 0;
    
  };
  
  ~nuiMainMenuItemPrivate()
  {
    
  };
  
  bool mIsMenu;       ///< true if the item is a menu
  bool mIsSubMenu;    ///< true if the item is a submenu
  MenuRef mpMenuRef;  ///< the associated MenuRef if the item is a menu or a submenu
  MenuID mMenuID;     ///< the associated MenuID if the item is a menu or a submenu
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
      DeleteMenu(mpPrivate->mMenuID);
    else
    {
      uint32 index = ComputeIndexInParent()+1; // apple's index is one-based
      DeleteMenuItem(pParent->mpPrivate->mpMenuRef, index);
    }
  }

  if (pParent)
    mIsBeingDeleted = true;
  
  delete mpPrivate;
}
 

void nuiMainMenuItem::SetEnabled(bool set)
{
  mEnabled = set;

  if (mpPrivate->mIsMenu || mpPrivate->mIsSubMenu)
  {
    if (set)
      EnableAllMenuItems(mpPrivate->mpMenuRef);
    else
      DisableAllMenuItems(mpPrivate->mpMenuRef);
  }
  else
  {
    nuiMainMenuItem* pParent = (nuiMainMenuItem*)GetParent();
    uint32 index = ComputeIndexInParent()+1; // apple item's index is one-base

    if (set)
      EnableMenuItem(pParent->mpPrivate->mpMenuRef, index);
    else
      DisableMenuItem(pParent->mpPrivate->mpMenuRef, index);
  }
}


void nuiMainMenuItem::SetMenuEnabled(bool set)
{
  
  
  
}


void nuiMainMenuItem::SetChecked(bool set)
{
  if (mpPrivate->mIsMenu || mpPrivate->mIsSubMenu)
    return;
  
  mChecked = set;
 
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)GetParent();
  uint32 index = ComputeIndexInParent()+1; // apple item's index is one-base
  
  CheckMenuItem(pParent->mpPrivate->mpMenuRef, index, mChecked);

  // send event
  if (mChecked)
    Checked();
  else
    Unchecked();
}

void nuiMainMenuItem::SetText(const nglString& rText)
{  
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)GetParent();
  int32 index = ComputeIndexInParent()+1; // apple item's index is one-base
  
  SetMenuItemTextWithCFString(pParent->mpPrivate->mpMenuRef, index, rText.ToCFString());
}

void nuiMainMenuItem::MakeSubMenu(uint32 subMenuID)
{
  int32 index = ComputeIndexInParent()+1; // apple item's index is one-base
  
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)GetParent();
  
  // first, delete the existing system item
  DeleteMenuItem(pParent->mpPrivate->mpMenuRef, index);
  
  // then, replace it by a submenu
  const nglString& rItemLabel = GetElement();
  
  mpPrivate->mIsSubMenu = true;
  mpPrivate->mMenuID = subMenuID;
  
  CreateNewMenu(mpPrivate->mMenuID, 0, &mpPrivate->mpMenuRef);
  
  CFStringRef str = rItemLabel.ToCFString();
  SetMenuTitleWithCFString(mpPrivate->mpMenuRef, str);
  CFRelease(str);    
  
  MenuRef rootMenu = AcquireRootMenu(); 
  SetRootMenu(pParent->mpPrivate->mpMenuRef);
  InsertMenu(mpPrivate->mpMenuRef, index-1);  
  SetRootMenu(rootMenu); // pop back information
  
  // re-apply the enabled state
  SetEnabled(mEnabled);
  
}



//*****************************************************************************************
//
// nuiMainMenuPrivate
//
//*****************************************************************************************

pascal OSStatus MainMenuEventHandler(EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);


class nuiMainMenuPrivate
{
  friend class nuiMainMenu;
  
protected:

  nuiMainMenuPrivate()
  {
    mEventHandlerRef = NULL;
    mEventHandlerUPP = NULL;
    
    mUniqueID = 128;
    
    EventTypeSpec menuEvents[]={ { kEventClassCommand, kEventCommandProcess} };
    
    mEventHandlerUPP = NewEventHandlerUPP(::MainMenuEventHandler);
    OSErr err = InstallApplicationEventHandler(mEventHandlerUPP, 
                                               GetEventTypeCount(menuEvents),
                                               menuEvents, 
                                               (void*) this, 

                                               &mEventHandlerRef);  

  
  }
  
  ~nuiMainMenuPrivate()
  {
    
  }
  
  bool IsCommandRegistered(uint32 commandID)
  {
    std::set<uint32>::iterator it = mCommands.find(commandID);
    return (it != mCommands.end());
  }
  
  void RegisterCommand(uint32 commandID)
  {
    mCommands.insert(commandID);
  }
  
  void UnregisterCommand(uint32 commandID)
  {
    std::set<uint32>::iterator it = mCommands.find(commandID);
    if (it == mCommands.end())
      return;
    
    mCommands.erase(it);
  }
  
private:

  uint32 mUniqueID;
  friend pascal OSStatus MainMenuEventHandler(EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData);
  EventHandlerRef   mEventHandlerRef;
  EventHandlerProcPtr mEventHandlerUPP;
  std::set<uint32> mCommands;
};



pascal OSStatus MainMenuEventHandler(EventHandlerCallRef eventHandlerCallRef, EventRef eventRef, void* userData)
{
  //  wprintf(_T("MainMenuEventHandler 0x%x\n"), userData);
  
  nuiMainMenuPrivate* pPrivate = (nuiMainMenuPrivate*)userData;
  if (!pPrivate)
    return noErr;
  
  UInt32 eventKind = GetEventKind(eventRef);
  UInt32 eventClass = GetEventClass (eventRef);
  
  switch (eventClass)
  {
    case kEventClassCommand:
    {
      HICommand command;
      GetEventParameter(eventRef, kEventParamDirectObject, typeHICommand, 
                        NULL, sizeof(HICommand), NULL, &command);
      
      switch (eventKind)
      {
        case kEventCommandProcess:
        {
          // note : Apple's advice for a commandID is a four-char code with a uppercase letter ('Copy', 'Edit', etc...).
          //
          // But this idea doesn't fit much well with dynamically generated menus (which is our purpose here...).
          // Instead of generating weird and abstract four-char codes and linking them to our commands,
          // we chose to use our objects' pointers as command ID. This way, getting our objects from the command ID is easy.
          //
          // All we have to do : check the commandID is one of our commands. If it's not, we just dispatch the event to the default event handler.
          
          if (!pPrivate->IsCommandRegistered(command.commandID))
            return CallNextEventHandler(eventHandlerCallRef, eventRef);
          
          // retrieve the associated item from the command ID
          nuiMainMenuItem* pItem = (nuiMainMenuItem*)command.commandID;
          
          // check/uncheck the item if the check flag has been set
          if (pItem->GetType() == eItemCheck)
            pItem->SetChecked(!pItem->IsChecked());
          
          //NGL_OUT(_T("Command Processed: 0x%x\n"), pItem);
          
          // and send the item activation signal
          pItem->Activated();
        }
          break;
          
        default:
        {
          return CallNextEventHandler(eventHandlerCallRef, eventRef);
        }
      }
    }
  }
  return noErr;
}




//*****************************************************************************************
//
// nuiMainMenu
//
//*****************************************************************************************

void nuiMainMenu::Init()
{
  mRefCount++;

  // a single menu per session on Carbon (on Carbon, the menu is common for all windows)
  if (mRefCount > 1)
    return;
    
  mpPrivate = new nuiMainMenuPrivate();
}

bool nuiMainMenu::RegisterFromWindow(nglWindow* pWindow)
{
  // carbon don't do anything for now, since the menu is global for the application
  return true;
}

bool nuiMainMenu::UnregisterFromWindow(nglWindow* pWindow)
{
  // carbon don't do anything for now, since the menu is global for the application
  return true;
}




nuiMainMenu::~nuiMainMenu()
{
  mRefCount--;
  
  // don't delete anything if this is not the first instance
  if (mRefCount != 0)
    return;
  
  delete mpPrivate;

  // and now, 'cause nuiMainMenu is not a widget, we have to do the house cleaning ourselves
  delete mpRoot;
}




// an item has been added in the widget tree. Do the system-side of the operation (add a menu or add a menu item)
void nuiMainMenu::OnItemAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiMainMenuItem* pParent = (nuiMainMenuItem*)pTreeEvent->mpParent;
  nuiMainMenuItem* pItem = (nuiMainMenuItem*)pTreeEvent->mpChild;
  
  if (pParent == mpRoot)
  {
    InsertMenu(pItem);
  }
  else
  {
    if (pParent->mpPrivate->mIsMenu)
    {
      InsertItem(pParent, pItem);
      mpPrivate->RegisterCommand((uint32)pItem);
    }
    else 
    {
      if (!pParent->mpPrivate->mIsSubMenu)
      {
        int32 newID = mpPrivate->mUniqueID;
        mpPrivate->mUniqueID++;
        pParent->MakeSubMenu(newID);
        mpPrivate->UnregisterCommand((uint32)pParent);
      }
      
      InsertItem(pParent, pItem);
      mpPrivate->RegisterCommand((uint32)pItem);
    }
  }
  
  rEvent.Cancel();
}


 
void nuiMainMenu::InsertMenu(nuiMainMenuItem* pItem)
{
  const nglString& rItemLabel = pItem->GetElement();

  pItem->mpPrivate->mMenuID = mpPrivate->mUniqueID;
  mpPrivate->mUniqueID++;
  
  pItem->mpPrivate->mIsMenu = true;
  
  CreateNewMenu(pItem->mpPrivate->mMenuID, 0, &(pItem->mpPrivate->mpMenuRef));

  MenuRef menuRef = pItem->mpPrivate->mpMenuRef;

  
  CFStringRef str = rItemLabel.ToCFString();
  SetMenuTitleWithCFString(menuRef, str);
  CFRelease(str);    
  
  ::InsertMenu(menuRef, 0);  
}



void nuiMainMenu::InsertItem(nuiMainMenuItem* pParent, nuiMainMenuItem* pItem)
{
  const nglString& rItemLabel = pItem->GetElement();
  MenuRef menuRef = pParent->mpPrivate->mpMenuRef;

  uint32 index = pItem->ComputeIndexInParent()+1; // Carbon item's index is one-based

  // insert the menu item 
  CFStringRef str = rItemLabel.ToCFString();

  if (pItem->mType == eItemSeparator)
  {
    InsertMenuItemTextWithCFString(menuRef, str, index-1/*previous index*/, kMenuItemAttrSeparator/* attributes */, 0);    
  }
  else
  {
    // use the nuiMainMenuItem pointer as the command ID, it'll be easy to retrieve the object in the event handler then
    MenuCommand command = (MenuCommand)pItem;    
    InsertMenuItemTextWithCFString(menuRef, str, index-1/*previous index*/, 0/* attributes */, command);
  }
  CFRelease(str); 
  
  // hack for Carbon implementation : 'til we find out how disable a whole menu, we have to use "DisableAllMenuItems" as a "DisableMenu" function, and therefore, we have to force this state after each item insertion
  if (!pParent->mEnabled)
    pParent->SetEnabled(false);
}



void nuiMainMenu::SetItemText(nuiMainMenuItem* pItem, const nglString& rText)
{
  if (pItem->GetType() != eItemString)
    return;
  
  pItem->SetText(rText);
}

  




