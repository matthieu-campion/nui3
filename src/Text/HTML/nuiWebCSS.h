/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"

class nuiParser;

class nuiWebCSS
{
  nuiWebCSS();
  ~nuiWebCSS();
  
  bool Load(nglIStream* pStream, const nglString& rSourceURL);
  bool Load(const nglString& rString);
  
protected:
  nuiParser* mpParser;
  
  bool ParseBody();
};