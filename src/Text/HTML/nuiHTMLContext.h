/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiFontManager.h"

class nuiCSSStyleSheet;

class nuiHTMLContext
{
public:
  nuiHTMLContext();
  nuiHTMLContext(const nuiHTMLContext& rContext);
  nuiHTMLContext& operator=(const nuiHTMLContext& rContext);
  ~nuiHTMLContext();
  
  void UpdateFont();
    
  float mMaxWidth;
  
  float mVSpace;
  float mHSpace;
  
  nuiFontRequest mFont; ///< If you change this object, you MUST update mpFont too (just call UpdateFont())!
  nuiFont* mpFont;
  bool mUnderline;
  bool mStrikeThrough;
  nuiColor mTextFgColor;
  nuiColor mTextBgColor;
  nuiColor mLinkColor;
  
  nuiAlignment mAlignHorizontal;
  nuiAlignment mAlignVertical;
  
  std::vector<const nuiCSSStyleSheet*> mpStyleSheets;
};

