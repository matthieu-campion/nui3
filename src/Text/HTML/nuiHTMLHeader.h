/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"
#include "nuiHTMLBox.h"

class nuiHTMLHeader : public nuiHTMLBox
{
public:
  nuiHTMLHeader(nuiHTMLNode* pNode, nuiHTMLNode* pAnchor);
  virtual ~nuiHTMLHeader();
  
  virtual void Layout(nuiHTMLContext& rContext);
  
};

