/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiBackgroundPane::nuiBackgroundPane(nuiBackgroundType type)
 : nuiSimpleContainer(), mType(type)
{
  SetObjectClass(_T("nuiBackgroundPane"));
  mInterceptMouse = false;
  
  mCanRespectConstraint = true;

  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);
}

nuiBackgroundPane::~nuiBackgroundPane()
{
}

nuiBackgroundType nuiBackgroundPane::GetType() const
{
  return mType;
}



bool nuiBackgroundPane::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  ClickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiBackgroundPane::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  UnclickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiBackgroundPane::MouseMoved  (nuiSize X, nuiSize Y)
{
  MovedMouse(X,Y);
  return mInterceptMouse;
}

void nuiBackgroundPane::SetInterceptMouse(bool intercept)
{
  mInterceptMouse = intercept;
}

bool nuiBackgroundPane::GetInterceptMouse()
{
  return mInterceptMouse;
}

