/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiFlags_h__
#define __nuiFlags_h__

//#include "nui.h"
#include "nglString.h"
class nuiXMLNode;

// Generic definitions that can be very useful for widgets:
enum nuiOrientation
{
  nuiHorizontal = 0,
  nuiVertical
};

enum nuiDirection
{
  nuiForward = 0,
  nuiBackward
};

enum nuiPosition
{
  nuiNoPosition = 0,
  nuiLeft = 1,
  nuiRight = 2,
  nuiTop = 3,
  nuiBottom = 4,
  nuiTopLeft = 5,
  nuiTopRight = 6,
  nuiBottomLeft = 7,
  nuiBottomRight = 8,
  nuiCenter = 9,
  nuiTile = 10,  ///< This position is not handled by most widgets. 
  nuiFill = 11,
  nuiFillHorizontal = 12,
  nuiFillVertical = 13,
  nuiFillLeft = 14,
  nuiFillRight = 15,
  nuiFillTop = 16,
  nuiFillBottom = 17
};

enum nuiLineJoin
{
  nuiLineJoinMiter = 0,
  nuiLineJoinRound,
  nuiLineJoinBevel
};

enum nuiLineCap
{
  nuiLineCapBut = 0,
  nuiLineCapRound,
  nuiLineCapSquare
};


enum nuiCopyPolicy
{
  eClone=0,
  eReference,
  eTransfert
};


enum nuiExpandMode
{
  nuiExpandFixed,
  nuiExpandGrow,
  nuiExpandShrink,
  nuiExpandShrinkAndGrow
};



// This is just plain ugly; i know...
typedef uint nuiWindowFlags;



/// Useful conversion functions:
nuiPosition     nuiGetPosition    (const nglString Position, nuiPosition Default = nuiNoPosition);
nuiDirection    nuiGetDirection   (const nglString Direction, nuiDirection Default = nuiForward);
nuiOrientation  nuiGetOrientation (const nglString Orientation, nuiOrientation Default = nuiVertical);

nglString nuiGetPosition (nuiPosition Value);
nglString nuiGetDirection (nuiDirection Value);
nglString nuiGetOrientation (nuiOrientation Value);

bool nuiGetBool (const nglString& rBool, bool Default = false);

// Get enum from XML desc:
nuiPosition     nuiGetPosition    (const nuiXMLNode* pNode, const nglString Attr, nuiPosition Default = nuiCenter);
nuiDirection    nuiGetDirection   (const nuiXMLNode* pNode, const nglString Attr, nuiDirection Default = nuiForward);
nuiOrientation  nuiGetOrientation (const nuiXMLNode* pNode, const nglString Attr, nuiOrientation Default = nuiVertical);

// Get enum from xml desc attrib with standard node:
nuiPosition     nuiGetPosition    (const nuiXMLNode* pNode, nuiPosition Default = nuiCenter);
nuiDirection    nuiGetDirection   (const nuiXMLNode* pNode, nuiDirection Default = nuiForward);
nuiOrientation  nuiGetOrientation (const nuiXMLNode* pNode, nuiOrientation Default = nuiVertical);

bool nuiGetBool (const nuiXMLNode* pNode, const nglString Attr, bool Default = false);
nglString nuiGetString (const nuiXMLNode* pNode, const nglString Attr, const nglString Default = nglString(""));

int     nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, int     Default = 0);
int64   nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, int64   Default = 0);
uint    nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, uint    Default = 0);
uint64  nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, uint64  Default = 0);
float   nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, float   Default = 0);
double  nuiGetVal (const nuiXMLNode* pNode, const nglString Attr, double  Default = 0);

#endif // __nuiFlags_h__
