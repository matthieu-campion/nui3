/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define SBWIDTH 12
#define INTERLINE 4

nuiText::nuiText(const nglString& rText)
  : nuiEditText(rText)
{
  SetObjectClass(_T("nuiText"));
  SetEditable(false);
}

nuiText::~nuiText()
{
}

