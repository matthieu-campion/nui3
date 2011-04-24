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
  nuiFontRequest request;
  request.MustHaveSize(14, 2);
  request.SetName("Times", 2);
  nuiFont* pFont = nuiFontManager::GetManager().GetFont(request);
  printf("Requested font: %s\n", pFont->GetFamilyName().GetChars());
  nuiTextLayout layout(pFont);
  layout.LayoutText(txt);
  
}


/////////////
nuiTextRun::nuiTextRun(nuiUnicodeScript script, const nglString& rString, int32 Position, int32 Length)
: mpFont(NULL),
  mString(rString),
  mPosition(Position),
  mLength(Length),
  mScript(script)
{
}

nuiTextRun::~nuiTextRun()
{
  mpFont->Release();
}

void nuiTextRun::SetFont(nuiFont* pFont)
{
  NGL_ASSERT(!mpFont);
  pFont->Acquire();
  mpFont = pFont;
}

nuiUnicodeScript nuiTextRun::GetScript() const
{
  return mScript;
}


void nuiTextRun::AddGlyph(int32 Index, float X, float Y)
{
  nuiGlyphLayout glyph;
  glyph.X = X;
  glyph.Y = Y;
  mpFont->PrepareGlyph(Index, glyph, true);
  mGlyphs.push_back(glyph);
}

int32 nuiTextRun::GetPosition() const
{
  return mPosition;
}

int32 nuiTextRun::GetLength() const
{
  return mLength;
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

uint32 nuiTextLine::size() const
{
  return mpRuns.size();
}

nuiTextRun* nuiTextLine::operator[](uint32 index) const
{
  return mpRuns[index];
}


/////////////////
// nuiTextLayout
nuiTextLayout::nuiTextLayout(nuiFont* pFont)
{
  pFont->Acquire();
  mpFont = pFont;
}

nuiTextLayout::~nuiTextLayout()
{
  mpFont->Release();

  for (uint32 p = 0; p < mParagraphs.size(); p++)
  {
    for (uint32 l = 0; l < mParagraphs[p].size(); l++)
    {
      for (uint32 r = 0; r < mParagraphs[p][l].size(); r++)
      { 
        mParagraphs[p][l][r]->Release();
      }
    }
  }
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
  
  // Find the needed fonts for each script:
  std::map<nuiUnicodeScript, nuiFont*> FontSet;
  {
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator it = mCharsets.begin();
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator end = mCharsets.end();
    while (it != end)
    {
      const std::set<nglUChar>& charset(it->second);
      nuiFontRequest request(mpFont);
      request.MustHaveGlyphs(charset, 10);
      nuiFont* pFont = nuiFontManager::GetManager().GetFont(request);
      FontSet[it->first] = pFont;
      
      printf("%s -> %s\n", nuiGetUnicodeScriptName(it->first).GetChars(), pFont->GetFamilyName().GetChars());
      
      ++it;
    }
  }

  uint32 i = 0;
  // Assign the correct font to each run
  for (uint32 p = 0; p < mParagraphs.size(); p++)
  {
    for (uint32 l = 0; l < mParagraphs[p].size(); l++)
    {
      for (uint32 r = 0; r < mParagraphs[p][l].size(); r++)
      {
        nuiTextRun* pRun = mParagraphs[p][l][r];
        nuiFont* pFont = FontSet[pRun->GetScript()];
        pRun->SetFont(pFont);
        
        printf("\trange %d <%d.%d.%d> (%d - %d) (%s --> %s)\n", i, p, l, r, pRun->GetPosition(), pRun->GetLength(), nuiGetUnicodeScriptName(pRun->GetScript()).GetChars(), pFont->GetFamilyName().GetChars());

        i++;
      }
    }
  }
  
  
  mCharsets.clear();
  return true;
}

bool nuiTextLayout::LayoutParagraph(const nglString& rString, int32 start, int32 length)
{
  printf("new paragraph: %d + %d\n", start, length);

  mParagraphs.push_back(Paragraph());

  mParagraphs.back().push_back(nuiTextLine(0, 0));
  nuiTextLine& rLine = mParagraphs.back().back();
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(rString, ranges, nuiST_ScriptChange, start, length);

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    uint32 pos = 0;
    int32 i = 0;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      uint32 len = range.mLength;
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
      
      std::set<nglUChar>& charset(mCharsets[range.mScript]);
      {
        int32 pos = start;
        while (pos < start + length)
        {
          nglUChar ch = rString.GetNextUChar(pos);
          
          charset.insert(ch);
        }
      }

      pos += len;
      ++it;
    }
  }
  

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    uint32 i = 0;
    uint32 pos = 0;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      uint32 len = range.mLength;
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
         
      nuiTextRun* pRun = new nuiTextRun(range.mScript, rString, pos, len);
      rLine.AddRun(pRun);
      
      
      pos += len;
      ++i;
      ++it;
    }
  }
  
  return true;
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


