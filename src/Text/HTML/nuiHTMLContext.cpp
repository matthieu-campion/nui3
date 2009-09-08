/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiHTMLContext.h"

///////////class nuiHTMLContext
nuiHTMLContext::nuiHTMLContext()
{
  mLeftMargin = 0;
  
  mMaxWidth = 640;
  
  mVSpace = 0;
  mHSpace = 8;
  
  mFont.SetName(_T("arial"), 0.8f);
  mFont.SetGenericName(_T("sans-serif"), 0.8f);
  mFont.SetStyle(_T("regular"), 0.5f);
  mFont.SetProportionnal(1.0f);
  
  mFont.SetScalable(1.0f);
  mFont.MustHaveSize(12.0f, 1.0f);
  mFont.SetItalic(false, 1.0f);
  mFont.SetBold(false, 1.0f);
  mUnderline = false;
  mStrikeThrough = false;
  mTextFgColor = nuiColor(0,0,0,255);
  mTextBgColor = nuiColor(0,0,0,0);
}

nuiHTMLContext::nuiHTMLContext(const nuiHTMLContext& rContext)
: mLeftMargin(rContext.mLeftMargin),
  mMaxWidth(rContext.mMaxWidth),
  mVSpace(rContext.mVSpace),
  mHSpace(rContext.mHSpace),
  mFont(rContext.mFont),
  mUnderline(rContext.mUnderline),
  mStrikeThrough(rContext.mStrikeThrough),
  mTextFgColor(rContext.mTextFgColor),
  mTextBgColor(rContext.mTextBgColor)
{
}

nuiHTMLContext& nuiHTMLContext::operator=(const nuiHTMLContext& rContext)
{
  mLeftMargin = rContext.mLeftMargin;
  
  mMaxWidth = rContext.mMaxWidth;
  
  mVSpace = rContext.mVSpace;
  mHSpace = rContext.mHSpace;
  
  mFont = rContext.mFont;
  mUnderline = rContext.mUnderline;
  mStrikeThrough = rContext.mStrikeThrough;
  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;

  return *this;
}


