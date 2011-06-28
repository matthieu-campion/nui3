/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiTextLayout.h"

nuiTextStyle::nuiTextStyle()
: mpFont(NULL),
  mDensityX(1.0f),
  mDensityY(1.0f),
  mSpacesPerTab(2),
  mColor(0, 0, 0),
  mUnderline(false),
  mStrikeThrough(false),
  mBaseline(nuiTextBaselineNormal),
  mJustify(false),
  mDirection(nuiLeftToRight)
{
  
}

nuiTextStyle::nuiTextStyle(const nuiTextStyle& rStyle)
: mpFont(rStyle.mpFont),
  mDensityX(rStyle.mDensityX),
  mDensityY(rStyle.mDensityY),
  mSpacesPerTab(rStyle.mSpacesPerTab),
  mColor(rStyle.mColor),
  mUnderline(rStyle.mUnderline),
  mStrikeThrough(rStyle.mStrikeThrough),
  mBaseline(rStyle.mBaseline),
  mJustify(rStyle.mJustify),
  mDirection(rStyle.mDirection)
{
  if (mpFont)
    mpFont->Acquire();
}

nuiTextStyle::~nuiTextStyle()
{
  if (mpFont)
  {
    mpFont->CheckValid();
    mpFont->Release();
  }
}


nuiTextStyle& nuiTextStyle::operator =(const nuiTextStyle& rStyle)
{
  if (rStyle.mpFont)
    rStyle.mpFont->Acquire();
  if (mpFont)
    mpFont->Release();
  
  mpFont = (rStyle.mpFont);
  mDensityX = (rStyle.mDensityX);
  mDensityY = (rStyle.mDensityY);
  mSpacesPerTab = (rStyle.mSpacesPerTab);
  mColor = (rStyle.mColor);
  mUnderline = (rStyle.mUnderline);
  mStrikeThrough = (rStyle.mStrikeThrough);
  mBaseline = (rStyle.mBaseline);
  mJustify = (rStyle.mJustify);
  mDirection = (rStyle.mDirection);
  
  return *this;
}


void nuiTextStyle::SetFont(nuiFont* pFont)
{
  if (pFont)
    pFont->Acquire();
  if (mpFont)
    mpFont->Release();
  
  mpFont = pFont;
}

nuiFont* nuiTextStyle::GetFont() const
{
  return mpFont;
}

void nuiTextStyle::SetDensity(nuiSize X, nuiSize Y)
{
  mDensityX = X;
  mDensityY = Y;
}

nuiSize nuiTextStyle::GetDensityX() const
{
  return mDensityX;
}

nuiSize nuiTextStyle::GetDensityY() const
{
  return mDensityY;
}

void nuiTextStyle::SetSpacesPerTab(int count)
{
  mSpacesPerTab = count;
}

int32 nuiTextStyle::GetSpacesPerTab()
{
  return mSpacesPerTab;
}

void nuiTextStyle::SetUnderline(bool set)
{
  mUnderline = set;
}

bool nuiTextStyle::GetUnderline() const
{
  return mUnderline;
}

void nuiTextStyle::SetStrikeThrough(bool set)
{
  mStrikeThrough = set;
}

bool nuiTextStyle::GetStrikeThrough() const
{
  return mStrikeThrough;
}

void nuiTextStyle::SetColor(const nuiColor& rColor)
{
  mColor = rColor;
}

const nuiColor& nuiTextStyle::GetColor() const
{
  return mColor;
}

void nuiTextStyle::SetBaseline(nuiTextBaseline set)
{
  mBaseline = set;
}

nuiTextBaseline nuiTextStyle::GetBaseline() const
{
  return mBaseline;
}

void nuiTextStyle::SetDirection(nuiTextDirection set)
{
  mDirection = set;
}

nuiTextDirection nuiTextStyle::GetDirection() const
{
  return mDirection;
}

void nuiTextStyle::SetJustify(bool set)
{
  mJustify = set;
}

bool nuiTextStyle::GetJustify() const
{
  return mJustify;
}

