//
//  nuiTextLayout.cpp
//  TextEdit
//
//  Created by Sébastien Métrot on 4/21/11.
//  Copyright 2011 MXP4. All rights reserved.
//

#include "nuiTextLayout.h"

void TextLayoutTest(const nglString& txt)
{
  nuiTextRangeList ranges;
  nuiSplitText(txt, ranges, nuiST_ScriptChange);
  
  nuiTextRangeList::iterator it = ranges.begin();
  nuiTextRangeList::iterator end = ranges.end();
  uint32 i = 0;
  uint32 pos = 0;
  while (it != end)
  {
    const nuiTextRange& range(*it);
    uint32 len = range.mLength;
    printf("range %d (%d - %d) (%s - %s)\n%s\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars(), txt.Extract(pos, len).GetChars());
    
    pos += len;
    ++i;
    ++it;
  }
  
}


/////////////
nuiTextRun::nuiTextRun(nuiFont* pFont, const nglString& rString, int32 Position, int32 Length)
: mpFont(pFont),
  mString(rString),
  mPosition(Position),
  mLength(Length)
{
}

void nuiTextRun::AddGlyph(nuiFont* pFont, int32 Index, float X, float Y)
{
  nuiGlyphLayout glyph;
  glyph.X = X;
  glyph.Y = Y;
  pFont->PrepareGlyph(Index, glyph, true);
  mGlyphs.push_back(glyph);
}

////////////
//nuiTextLine
nuiTextLine::nuiTextLine(float X, float Y)
{
}

nuiTextLine::~nuiTextLine()
{
}

/** @name Drawing the Line */
//@{
void nuiTextLine::Draw(nuiDrawContext* pContext)
{
}
//@}

const std::vector<nuiTextRun*>& nuiTextLine::GetGlyphRuns() const
{
  return mRuns;
}

float nuiTextLine::GetX() const
{
  return mX;
}

float nuiTextLine::GetY() const
{
  return mY;
}

void nuiTextLine::SetPosition(float X, float Y)
{
  mX = X;
  mY = Y;
}

void nuiTextLine::AddRun(nuiTextRun* pRun)
{
  mRuns.push_back(pRun);
}


/////////////////
// nuiTextLayout
nuiTextLayout::nuiTextLayout(nuiFont* pFont)
{
  pFont->Acquire();
  mpFonts.push_back(pFont);
}

nuiTextLayout::~nuiTextLayout()
{
  for (uint32 i = 0; i < mpFonts.size(); i++)
    mpFonts[i]->Release();
}

