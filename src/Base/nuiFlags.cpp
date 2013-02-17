/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

// Get enum from string desc:
nuiPosition nuiGetPosition (const nglString& Position, nuiPosition Default)
{
  if (!Position.Compare(_T("Left"),false))
    return nuiLeft;
  else if (!Position.Compare(_T("Right"),false))
    return nuiRight;
  else if (!Position.Compare(_T("Top"),false))
    return nuiTop;
  else if (!Position.Compare(_T("Bottom"),false))
    return nuiBottom;
  else if (!Position.Compare(_T("TopLeft"),false))
    return nuiTopLeft;
  else if (!Position.Compare(_T("TopRight"),false))
    return nuiTopRight;
  else if (!Position.Compare(_T("BottomLeft"),false))
    return nuiBottomLeft;
  else if (!Position.Compare(_T("BottomRight"),false))
    return nuiBottomRight;
  else if (!Position.Compare(_T("Center"),false))
    return nuiCenter;
  else if (!Position.Compare(_T("Tile"),false))
    return nuiTile;
  else if (!Position.Compare(_T("Fill"),false))
    return nuiFill;
  else if (!Position.Compare(_T("FillHorizontal"),false))
    return nuiFillHorizontal;
  else if (!Position.Compare(_T("FillVertical"),false))
    return nuiFillVertical;
  else if (!Position.Compare(_T("FillLeft"),false))
    return nuiFillLeft;
  else if (!Position.Compare(_T("FillRight"),false))
    return nuiFillRight;
  else if (!Position.Compare(_T("FillTop"),false))
    return nuiFillTop;
  else if (!Position.Compare(_T("FillBottom"),false))
    return nuiFillBottom;
  else if (!Position.Compare(_T("None"),false))
    return nuiNoPosition;

  return Default;
}

nglString nuiGetPosition (nuiPosition Value)
{
  switch (Value)
  {
  case nuiLeft:
    return _T("Left");
  case nuiRight:
    return _T("Right");
  case nuiTop:
    return _T("Top");
  case nuiBottom:
    return _T("Bottom");
  case nuiFillLeft:
    return _T("FillLeft");
  case nuiFillRight:
    return _T("FillRight");
  case nuiFillTop:
    return _T("FillTop");
  case nuiFillBottom:
    return _T("FillBottom");
  case nuiTopLeft:
    return _T("TopLeft");
  case nuiTopRight:
    return _T("TopRight");
  case nuiBottomLeft:
    return _T("BottomLeft");
  case nuiBottomRight:
    return _T("BottomRight");
  case nuiCenter:
    return _T("Center");
  case nuiTile:
    return _T("Tile");
  case nuiFill:
    return _T("Fill");
  case nuiFillHorizontal:
    return _T("FillHorizontal");
  case nuiFillVertical:
    return _T("FillVertical");
  default:
    return _T("None"); // Default?
  }
}

nuiDirection nuiGetDirection (const nglString& Direction, nuiDirection Default)
{
  if (!Direction.Compare(_T("Forward"),false))
    return nuiForward;
  else if (!Direction.Compare(_T("Backward"),false))
    return nuiBackward;
  return Default;
}

nglString nuiGetDirection (nuiDirection Value)
{
  switch (Value)
  {
  case nuiForward:
    return _T("Forward");
  case nuiBackward:
    return _T("Backward");
  default:
    return _T("Forward");
  }
}

nuiAlignment nuiGetAlignment (const nglString& Alignment, nuiAlignment Default)
{
  if (!Alignment.Compare(_T("Begin"),false))
    return eBegin;
  else if (!Alignment.Compare(_T("End"),false))
    return eEnd;
  else if (!Alignment.Compare(_T("Center"),false))
    return eCenter;
  else if (!Alignment.Compare(_T("Justify"),false))
    return eJustify;
  return Default;
}

nglString nuiGetAlignment (nuiAlignment Value)
{
  switch (Value)
  {
    case eBegin:
      return _T("Begin");
    case eEnd:
      return _T("End");
    case eCenter:
      return _T("Center");
    case eJustify:
      return _T("Justify");
    default:
      return _T("Begin");
  }
}

nuiOrientation nuiGetOrientation (const nglString& Orientation, nuiOrientation Default)
{
  if (!Orientation.Compare(_T("Horizontal"),false))
    return nuiHorizontal;
  else if (!Orientation.Compare(_T("Vertical"),false))
    return nuiVertical;
  return Default;
}
  
nglString nuiGetOrientation (nuiOrientation Value)
{
  switch (Value)
  {
  case nuiHorizontal:
    return _T("Horizontal");
  case nuiVertical:
  default:
    return _T("Vertical");
  }
}
  
bool nuiGetBool (const nglString& rBool, bool Default)
{
  if (!rBool.Compare(_T("true"),false))
    return true;
  else if (!rBool.Compare(_T("false"),false))
    return false;

  return Default;
}

// Get enum from XML desc:
nuiPosition nuiGetPosition (const nuiXMLNode* pNode, const nglString& Attr, nuiPosition Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return nuiGetPosition(pNode->GetAttribute(Attr), Default);
}

nuiDirection nuiGetDirection (const nuiXMLNode* pNode, const nglString& Attr, nuiDirection Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return nuiGetDirection(pNode->GetAttribute(Attr), Default);
}

nuiOrientation nuiGetOrientation (const nuiXMLNode* pNode, const nglString& Attr, nuiOrientation Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return nuiGetOrientation(pNode->GetAttribute(Attr), Default);
}

nuiAlignment nuiGetAlignment (const nuiXMLNode* pNode, const nglString& Attr, nuiAlignment Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return nuiGetAlignment(pNode->GetAttribute(Attr), Default);
}

// Get enum from standard xml attrib:
nuiPosition nuiGetPosition (const nuiXMLNode* pNode, nuiPosition Default)
{
  if (!pNode->HasAttribute(_T("Position")))
    return Default;
  return nuiGetPosition(pNode->GetAttribute(_T("Position")),Default);
}

nuiDirection nuiGetDirection (const nuiXMLNode* pNode, nuiDirection Default)
{
  if (!pNode->HasAttribute(_T("Direction")))
    return Default;
  return nuiGetDirection(pNode->GetAttribute(_T("Direction")),Default);
}

nuiOrientation nuiGetOrientation (const nuiXMLNode* pNode, nuiOrientation Default)
{
  if (!pNode->HasAttribute(_T("Orientation")))
    return Default;
  return nuiGetOrientation(pNode->GetAttribute(_T("Orientation")),Default);
}

nuiAlignment nuiGetAlignment (const nuiXMLNode* pNode, nuiAlignment Default)
{
  if (!pNode->HasAttribute(_T("Alignment")))
    return Default;
  return nuiGetAlignment(pNode->GetAttribute(_T("Alignment")), Default);
}

// Values helpers:
bool nuiGetBool (const nuiXMLNode* pNode, const nglString& Attr, bool Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return nuiGetBool(pNode->GetAttribute(Attr),Default);
}

const nglString& nuiGetString (const nuiXMLNode* pNode, const nglString& Attr, const nglString& Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr);
}


int nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, int Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCInt();
}


int64 nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, int64 Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCInt();
}


uint nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, uint Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCInt();
}

uint64 nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, uint64 Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCInt64();
}

float nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, float Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCFloat();
}

double nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, double Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr).GetCDouble();
}

nglString nuiGetVal (const nuiXMLNode* pNode, const nglString& Attr, const nglString& Default)
{
  if (!pNode->HasAttribute(Attr))
    return Default;
  return pNode->GetAttribute(Attr);
}

