/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nglString.h"
#include "nuiApplication.h"
#include "nuiText.h"
#include "nuiDrawContext.h"
#include "nuiScrollBar.h"

#include "nuiMainWindow.h"

#include <math.h>

#define SBWIDTH 12
#define INTERLINE 4

nuiText::nuiText(const nglString& rText)
  : nuiEditText(rText)
{
  SetObjectClass(_T("nuiText"));
  SetEditable(false);
}

bool nuiText::Load(const nuiXMLNode* pNode)
{
  nuiEditText::Load(pNode);
  SetObjectClass(_T("nuiText"));
  SetEditable(false);
  //Inherits all from nuiEditText...
  
  return true;
}

nuiText::~nuiText()
{
}

