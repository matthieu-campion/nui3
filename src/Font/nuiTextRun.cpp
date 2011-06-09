/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiFontManager.h"
#include "nuiTextLayout.h"

void TextLayoutTest(const nglString& txt)
{
  nuiFontRequest request;
  request.MustHaveSize(14, 2);
  request.SetName("Times", 2);
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(request);
  //printf("Requested font: %s\n", pFont->GetFamilyName().GetChars());
  nuiTextLayout* layout = new nuiTextLayout(pFont);
  layout->LayoutText(txt);
  delete layout;
}


/////////////
nuiTextRun::nuiTextRun(const nuiTextLayout& rLayout, nuiUnicodeScript script, int32 Position, int32 Length, const nuiTextStyle& rStyle)
: mLayout(rLayout),
mPosition(Position),
mLength(Length),
mScript(script),
mAdvanceX(0),
mAdvanceY(0),
mUnderline(false),
mStrikeThrough(false),
mStyle(rStyle)
{
}

nuiTextRun::~nuiTextRun()
{
}

const std::vector<nuiTextGlyph>& nuiTextRun::GetGlyphs() const
{
  return mGlyphs;
}

void nuiTextRun::SetFont(nuiFont* pFont)
{
  mStyle.SetFont(pFont);
}

nuiUnicodeScript nuiTextRun::GetScript() const
{
  return mScript;
}

nuiFont* nuiTextRun::GetFont() const
{
  return mStyle.GetFont();
}


int32 nuiTextRun::GetPosition() const
{
  return mPosition;
}

int32 nuiTextRun::GetLength() const
{
  return mLength;
}

float nuiTextRun::GetAdvanceX() const
{
  return mAdvanceX;
}

float nuiTextRun::GetAdvanceY() const
{
  return mAdvanceY;
}

float nuiTextRun::GetAscender() const
{
  return mStyle.GetFont()->GetAscender();
}

float nuiTextRun::GetDescender() const
{
  return mStyle.GetFont()->GetDescender();
}



const nglUChar* nuiTextRun::GetUnicodeChars() const
{
  return mLayout.GetUnicodeChars() + mPosition;
}

void nuiTextRun::SetUnderline(bool set)
{
  mUnderline = set;
}

bool nuiTextRun::GetUnderline() const
{
  return mUnderline;
}

void nuiTextRun::SetStrikeThrough(bool set)
{
  mStrikeThrough = set;
}

bool nuiTextRun::GetStrikeThrough() const
{
  return mStrikeThrough;
}

nuiRect nuiTextRun::GetRect() const
{
  nuiFontInfo finfo;
  mStyle.GetFont()->GetInfo(finfo);
  /*
   const nuiGlyphLayout& rGlyph(*it);
   nuiGlyphInfo info;
   rGlyph.mpFont->GetGlyphInfo(info, rGlyph.Index, nuiFontBase::eGlyphBitmap);
   nuiSize w = info.AdvanceX;
   //    nuiSize h = finfo.AdvanceMaxH;
   nuiSize x = rGlyph.X + info.BearingX;
   nuiSize y = rGlyph.Y - finfo.Ascender;
   nuiSize h = finfo.Height;
   
   nuiRect rr(r);
   r.Union(rr, nuiRect(x, y, w, h));
   */
  
}

