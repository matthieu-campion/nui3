/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiFontManager.h"

class nuiHTMLContext
{
public:
  nuiHTMLContext();
  nuiHTMLContext(const nuiHTMLContext& rContext);
  nuiHTMLContext& operator=(const nuiHTMLContext& rContext);
  
  float mLeftMargin;
  
  float mMaxWidth;
  
  float mVSpace;
  float mHSpace;
  
  nuiFontRequest mFont;
  bool mUnderline;
  bool mStrikeThrough;
  nuiColor mTextFgColor;
  nuiColor mTextBgColor;
};

