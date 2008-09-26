/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiContainer.h"
#include "nuiEvent.h"
#include "nuiMouseEvent.h"

/// This widget implements a pane area. It draws a decorated background and can intercept mouse events
class NUI_API nuiBackgroundPane : public nuiSimpleContainer
{
public:
  nuiBackgroundPane();
  virtual bool Load(const nuiXMLNode* pNode); ///< Create from an XML description.
	virtual bool LoadAttributes(const nuiXMLNode* pNode);
	virtual bool LoadChildren(const nuiXMLNode* pNode);
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive = false) const;
	virtual void SerializeChildren(nuiXMLNode* pParentNode, bool Recursive = false) const; // top level children serialization
	virtual nuiXMLNode* SerializeAttributes(nuiXMLNode* pParentNode, bool Recursive = false) const;
  virtual ~nuiBackgroundPane();

  // Out going Events
  nuiMouseClicked   ClickedMouse;
  nuiMouseUnclicked UnclickedMouse;
  nuiMouseMoved     MovedMouse;

  virtual bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseMoved(nuiSize X, nuiSize Y);

  virtual void SetInterceptMouse(bool intercept=false); ///< If intercept==true then all the mouse event will be intercepted by the UserArea. By default no event is intercepted.
  virtual bool GetInterceptMouse(); ///< Return the mouse event interception state.

protected:
  bool mInterceptMouse;
};


