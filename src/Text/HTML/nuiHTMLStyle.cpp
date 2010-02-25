/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */


#include "nui.h"
#include "nuiHTMLStyle.h"

/// class nuiHTMLStyleValueBase : public nuiObject
nuiHTMLStyleValueBase::nuiHTMLStyleValueBase(bool use)
: mUse(use)
{
  if (SetObjectClass(_T("nuiHTMLStyleValueBase")))
  {
    
  }
}

nuiHTMLStyleValueBase::~nuiHTMLStyleValueBase()
{
}

bool nuiHTMLStyleValueBase::Use() const
{
  return mUse;
}

void nuiHTMLStyleValueBase::Use(bool set)
{
  mUse = set;
}

/// class nuiHTMLStyle
nuiHTMLStyle::nuiHTMLStyle()
{
  nuiHTMLStyle::Init();
}

nuiHTMLStyle::nuiHTMLStyle(const nuiHTMLStyle& rStyle)
{
  nuiHTMLStyle::Init();
  
  CopyFrom(rStyle);
}

nuiHTMLStyle::~nuiHTMLStyle()
{
  
}

void nuiHTMLStyle::Init()
{
  if (SetObjectClass(_T("nuiHTMLStyle")))
  {
    // Init Attributes:
  }
}

#define COPY(X) X = rStyle.X;

void nuiHTMLStyle::CopyFrom(const nuiHTMLStyle& rStyle)
{
  COPY(mBackgroundColor);
  COPY(mBorderColor);
  COPY(mColor);
  
  COPY(mBorderSize);
  
  COPY(mFontBold);
  COPY(mFontItalic);
  COPY(mFontUnderline);
  COPY(mFontStrike);
  COPY(mMonospace);
  COPY(mFontSize);
  COPY(mFontFamilyName);
  COPY(mFontGenericName);
  COPY(mFontStyle);
}

#undef COPY

nuiHTMLStyle& nuiHTMLStyle::operator=(const nuiHTMLStyle& rStyle)
{
  CopyFrom(rStyle);
}


