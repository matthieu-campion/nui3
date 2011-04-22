//
//  nuiTextLayout.cpp
//  TextEdit
//
//  Created by Sébastien Métrot on 4/21/11.
//  Copyright 2011 MXP4. All rights reserved.
//

#include "nuiTextLayout.h"
#include "nuiFontManager.h"

void TextLayoutTest(const nglString& txt)
{
  nuiFont* pFont = nuiFont::GetFont(12);
  nuiTextLayout layout(pFont);
  layout.LayoutText(txt);
  
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
  for (uint32 i = 0; i < mpRuns.size(); i++)
    mpRuns[i]->Release();
}

/** @name Drawing the Line */
//@{
void nuiTextLine::Draw(nuiDrawContext* pContext)
{
}
//@}

const std::vector<nuiTextRun*>& nuiTextLine::GetGlyphRuns() const
{
  return mpRuns;
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
  mpRuns.push_back(pRun);
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

  for (uint32 i = 0; i < mpLines.size(); i++)
    mpLines[i]->Release();
}

bool nuiTextLayout::LayoutText(const nglString& rString)
{
  // General algorithm:
  // 1. Split text into paragraphs (LayoutText)
  // 2. Split paragraphs into ranges (LayoutParagraph)
  // 3. Split ranges into fonts
  // 4. Split ranges into lines / words if needed

  int32 start = 0;
  int32 position = 0;
  int32 len = rString.GetLength();
  while (position < len)
  {
    // Scan through the text and look for end of line markers
    nglUChar ch = rString.GetNextUChar(position);
    if (ch == '\n')
    {
      // Found a paragraph
      LayoutParagraph(rString, start, position - start);
      start = position;
    }
  }
  
  return true;
}

bool nuiTextLayout::LayoutParagraph(const nglString& rString, int32 start, int32 length)
{
  printf("new paragraph: %d + %d\n", start, length);

  nuiTextLine* pLine = new nuiTextLine(0, 0);
  AddLine(pLine);
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(rString, ranges, nuiST_ScriptChange, start, length);
  
  
  nuiTextRangeList::iterator it = ranges.begin();
  nuiTextRangeList::iterator end = ranges.end();
  uint32 i = 0;
  uint32 pos = 0;
  while (it != end)
  {
    const nuiTextRange& range(*it);
    uint32 len = range.mLength;
    printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());

    std::vector<nglUChar> codepoints;
    {
      int32 pos = start;
      while (pos < start + length)
      {
        nglUChar ch = rString.GetNextUChar(pos);
        
        codepoints.push_back(ch);
      }
    }
    
    nuiFont* pFont = FindBestFont(codepoints);
    nuiTextRun* pRun = new nuiTextRun(pFont, rString, pos, len);
    pLine->AddRun(pRun);
    
    
    pos += len;
    ++i;
    ++it;
  }
  
  return true;
}

nuiFont* nuiTextLayout::FindBestFont(const std::vector<nglUChar>& rCodepoints)
{
  // Create a set of the code points we need:
  std::set<nglUChar> points;
  points.insert(rCodepoints.begin(), rCodepoints.end());
  nuiFont* pFont = NULL;

  // Try the fonts we already use
  for (uint32 i = 0; i < mpFonts.size(); i++)
  {
    pFont = mpFonts[i];

    std::set<nglUChar>::iterator it = points.begin();
    std::set<nglUChar>::iterator end = points.end();
    
    int32 index = 1;
    while (it != end && index > 0)
    {
      nglUChar ch = *it;
      printf("%d ", ch);
      index = pFont->GetGlyphIndex(*it);
      ++it;
    }      
    
    if (it == end) // We found a font that works
      continue;
    
    pFont = NULL;
  }
  
  if (!pFont)
  {
    // We haven't found a font that contains the glyphs we need
    nuiFontRequest request(mpFonts.front());
    request.MustHaveGlyphs(points, 10.f, true);
    
    pFont = nuiFontManager::GetManager().GetFont(request);
  }
  
  if (pFont)
    printf("\t\tbest font found: %s (%s)\n", pFont->GetFamilyName().GetChars(), pFont->GetStyleName().GetChars());
  else
    printf("\n\nNo font found matching these unicode points\n");
  
  return pFont;
}

void nuiTextLayout::SetJustification(bool set)
{
  mJustify = set;
}

bool nuiTextLayout::GetJustification() const
{
  return mJustify;
}

void nuiTextLayout::SetFlush(float set)
{
  mFlush = set;
}

float nuiTextLayout::GetFlush() const
{
  return mFlush;
}

void nuiTextLayout::AddLine(nuiTextLine* pLine)
{
  mpLines.push_back(pLine);
}


