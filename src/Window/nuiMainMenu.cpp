/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


//*****************************************************************************************
//
// nuiMainMenuItem
//
//*****************************************************************************************



bool nuiMainMenuItem::IsEnabled()
{
  return mEnabled;
}

bool nuiMainMenuItem::IsChecked()
{
  return mChecked;
}


//*****************************************************************************************
//
// nuiMainMenu
//
//*****************************************************************************************


nuiMainMenu::nuiMainMenu(nuiMainMenuItem* pRoot)
: mEventSink(this)
{
  mpRoot = NULL;
  mpPrivate = NULL;
  SetRoot(pRoot);
}

void nuiMainMenu::SetRoot(nuiMainMenuItem* pRoot)
{
  if (pRoot)
  {
    mpRoot = pRoot;
    mEventSink.Connect(pRoot->ChildAdded, &nuiMainMenu::OnItemAdded);
    
    Init();
  }  
}


// static
uint32 nuiMainMenu::GetRefCount()
{
  return mRefCount;
}
uint32 nuiMainMenu::mRefCount = 0;

