/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "ElementDesc.h"

ElementDesc::ElementDesc()
{
  mpXML = NULL;
}

ElementDesc::~ElementDesc()
{
  delete mpXML;
}

void ElementDesc::SetName(const nglString& rString)
{
  mName = rString;
  CommitChanges();
}

const nglString& ElementDesc::GetName() const
{
  return mName;
}

void ElementDesc::SetType(ElementType type)
{
  mType = type;	
}

ElementType ElementDesc::GetType() const
{
  return mType;
}

const nuiXMLNode* ElementDesc::GetXML() const
{
  return mpXML;
}

void ElementDesc::SetXML(const nuiXMLNode* pNode)
{
  delete mpXML;
  mpXML = pNode;
}





