/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nglString.h"
#include "nuiObject.h"

enum ElementType
{
  eElementFrame=0,
  eElementWidget
};

class ElementDesc : nuiObject
{
public:
  ElementDesc();
  virtual ~ElementDesc();
  
  void SetName(const nglString& rString);
  const nglString& GetName() const;
  
  void SetType(ElementType type);
  ElementType GetType() const;
  
  const nuiXMLNode* GetXML() const;
  void SetXML(const nuiXMLNode* pNode);
  
  nuiSimpleEventSource<0> CommitChanges;
	
private:
  nglString mName;
  ElementType mType;
  const nuiXMLNode* mpXML;
};
