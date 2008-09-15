/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiTree.h"


enum nuiMainMenuItemType
{
  eItemString=0,
  eItemCheck,
  eItemSeparator
};

class nuiMainMenuItemPrivate;
class nuiMainMenu;




class nuiMainMenuItem : public nuiValueTree<nglString>
{
  friend class nuiMainMenu;
  
public:
  
  nuiMainMenuItem(const nglString& rLabel, nuiMainMenuItemType type = eItemString);
  virtual ~nuiMainMenuItem();
  
  nuiMainMenuItemType GetType() { return mType; }
  
  void SetEnabled(bool set);
  bool IsEnabled();

  void SetChecked(bool set);
  bool IsChecked();
  
  void SetText(const nglString& rText);
  
  nuiSimpleEventSource<0> Activated;
  nuiSimpleEventSource<0> Checked;
  nuiSimpleEventSource<0> Unchecked;
    
protected:

  nuiMainMenuItemPrivate* mpPrivate;

private:
  
  void MakeSubMenu(uint32 subMenuID);
  void SetMenuEnabled(bool set);
  
  nuiMainMenuItemType mType;
  bool mChecked;
  bool mEnabled;
  bool mIsBeingDeleted;
};






class nuiMainMenuPrivate;




// create a menu bar
class nuiMainMenu
{
  friend class nglWindow;
  
public:
  
  nuiMainMenu(nuiMainMenuItem* pRoot);
  virtual ~nuiMainMenu();
  
  void SetRoot(nuiMainMenuItem* pRoot);
  
  static uint32 GetRefCount();

protected:
  
  bool RegisterFromWindow(nglWindow* pWindow); // don't use this method. call nuiMainWindow::SetMainMenu
  bool UnregisterFromWindow(nglWindow* pWindow);
private:
  
  void Init();
  
  // internal events for system-side implementation
  bool OnItemAdded(const nuiEvent& rEvent);
  
  void InsertMenu(nuiMainMenuItem* pItem);
  void InsertItem(nuiMainMenuItem* pParent, nuiMainMenuItem* pItem);
  
  
  nuiMainMenuItem* mpRoot;
  nuiMainMenuPrivate* mpPrivate;
  
  nuiEventSink<nuiMainMenu> mEventSink;
  
  static uint32 mRefCount; ///< nb of nuiMainMenu
  
};

