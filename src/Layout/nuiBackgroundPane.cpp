/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiApplication.h"
#include "nuiBackgroundPane.h"
#include "nuiXML.h"

nuiBackgroundPane::nuiBackgroundPane()
 : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiBackgroundPane"));
  mInterceptMouse = false;
  
  mCanRespectConstraint = true;

  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);
}

bool nuiBackgroundPane::Load(const nuiXMLNode* pNode)
{
	LoadAttributes(pNode);
	LoadChildren(pNode);

  mInterceptMouse = nuiGetBool ( pNode, _T("InterceptMouse"), false);
  mCanRespectConstraint = true;
  
  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);

  return true;
}


bool nuiBackgroundPane::LoadAttributes(const nuiXMLNode* pNode)
{
	return nuiSimpleContainer::LoadAttributes(pNode);
}

bool nuiBackgroundPane::LoadChildren(const nuiXMLNode* pNode)
{
	return nuiSimpleContainer::LoadChildren(pNode);
}


nuiXMLNode* nuiBackgroundPane::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
	nuiXMLNode* paneNode = SerializeAttributes(pParentNode, Recursive);
	SerializeChildren(paneNode, Recursive);
  
  return paneNode;
}


void nuiBackgroundPane::SerializeChildren(nuiXMLNode* pParentNode, bool Recursive) const
{
	nuiContainer::SerializeChildren(pParentNode, Recursive);
}


nuiXMLNode* nuiBackgroundPane::SerializeAttributes(nuiXMLNode* pParentNode, bool Recursive) const
{
	return nuiContainer::SerializeAttributes(pParentNode, Recursive);
}



nuiBackgroundPane::~nuiBackgroundPane()
{
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

