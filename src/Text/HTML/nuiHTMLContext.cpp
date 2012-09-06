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
  mMaxWidth = 640;
  
  mVSpace = 0;
  
  mFont.SetName(_T("arial"), 0.8f);
  mFont.SetGenericName(_T("sans-serif"), 0.8f);
  mFont.SetStyle(_T("regular"), 0.5f);
  mFont.SetProportionnal(1.0f);
  
  mFont.SetScalable(1.0f);
  mFont.MustHaveSize(12.0f, 1.0f);
  mFont.SetItalic(false, 1.0f);
  mFont.SetBold(false, 1.0f);
  mpFont = NULL;
  mUnderline = false;
  mStrikeThrough = false;
  mTextFgColor = nuiColor(0,0,0,255);
  mTextBgColor = nuiColor(0,0,0,0);
  mLinkColor = nuiColor(64, 64, 200);

  mAlignHorizontal = eBegin;
  mAlignVertical = eBegin;
  
  UpdateFont();
}

nuiHTMLContext::nuiHTMLContext(const nuiHTMLContext& rContext)
: mMaxWidth(rContext.mMaxWidth),
  mVSpace(rContext.mVSpace),
  mHSpace(rContext.mHSpace),
  mFont(rContext.mFont),
  mpFont(rContext.mpFont),
  mUnderline(rContext.mUnderline),
  mStrikeThrough(rContext.mStrikeThrough),
  mTextFgColor(rContext.mTextFgColor),
  mTextBgColor(rContext.mTextBgColor),
  mLinkColor(rContext.mLinkColor),
  mAlignHorizontal(rContext.mAlignHorizontal),
  mAlignVertical(rContext.mAlignVertical),
  mpStyleSheets(rContext.mpStyleSheets)
{
  if (mpFont)
    mpFont->Acquire();
}

nuiHTMLContext& nuiHTMLContext::operator=(const nuiHTMLContext& rContext)
{
  mMaxWidth = rContext.mMaxWidth;
  
  mVSpace = rContext.mVSpace;
  mHSpace = rContext.mHSpace;
  
  mFont = rContext.mFont;
  mpFont = rContext.mpFont;
  if (mpFont)
    mpFont->Acquire();
  
  mUnderline = rContext.mUnderline;
  mStrikeThrough = rContext.mStrikeThrough;
  mTextFgColor = rContext.mTextFgColor;
  mTextBgColor = rContext.mTextBgColor;
  mLinkColor = rContext.mLinkColor;
  
  mAlignHorizontal = rContext.mAlignHorizontal;
  mAlignVertical = rContext.mAlignVertical;

  mpStyleSheets = rContext.mpStyleSheets;

  return *this;
}

nuiHTMLContext::~nuiHTMLContext()
{
  if (mpFont)
    mpFont->Release();
}

void nuiHTMLContext::UpdateFont()
{
  nuiFont* pOld = mpFont;
  
  mpFont = nuiFontManager::GetManager().GetFont(mFont);
  NGL_ASSERT(mpFont);
  nuiGlyphInfo ginfo;
  mpFont->GetGlyphInfo(ginfo, mpFont->GetGlyphIndex(_T(' ')), nuiFontBase::eGlyphBitmap);
  mHSpace = ginfo.AdvanceX;
  
  if (pOld)
    pOld->Release();
}


