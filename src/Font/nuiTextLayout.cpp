/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiTextLayout.h"
#include "nuiFontManager.h"
#include "ucdata.h"


/////////////////
// nuiTextLayout
nuiTextLayout::nuiTextLayout(nuiFont* pFont)
:
  mXMin(0), mXMax(0), mYMin(0), mYMax(0),
  mAscender(0),
  mDescender(0)
{
  mStyle.SetFont(pFont);
}

nuiTextLayout::~nuiTextLayout()
{
  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      delete pLine;
    }
    
    delete pParagraph;
  }
}

bool nuiTextLayout::LayoutText(const nglString& rString)
{
  // Transform the string in a vector of nglUChar, also keep the offsets from the original chars to the nglUChar and vice versa
  int32 len = rString.GetLength();
  int32 i = 0;
  while (i < len)
  {
    nglUChar ch = rString.GetNextUChar(i);
    mUnicode.push_back(ch);
    mOffsetInString.push_back(i);
    mOffsetInUnicode.push_back(mUnicode.size() - 1);
  }

  // General algorithm:
  // 1. Split text into paragraphs (LayoutText)
  // 2. Split paragraphs into ranges (LayoutParagraph)
  // 3. Split ranges into fonts
  // 4. Split ranges into lines / words if needed

  int32 start = 0;
  int32 position = 0;
  int32 count = mUnicode.size();
  while (position < count)
  {
    // Scan through the text and look for end of line markers
    nglUChar ch = mUnicode[position];
    if (ch == '\n' || ch == 0xb || ch == 0x2028 || ch == 0x2029)
    {
      // Found a paragraph
      LayoutParagraph(start, position - start); // Eat the \n char
      position++;
      start = position;
    }
    position++;
  }
  
  if (start != position - start)
  {
    LayoutParagraph(start, position - start); // Eat the \n char
    start = position;
  }

  mAscender = 0;
  mDescender = 0;
  
  // Find the needed fonts for each script:
  std::map<nuiUnicodeScript, nuiFont*> FontSet;
  {
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator it = mCharsets.begin();
    std::map<nuiUnicodeScript, std::set<nglUChar> >::iterator end = mCharsets.end();
    while (it != end)
    {
      printf("%s -> ", nuiGetUnicodeScriptName(it->first).GetChars());
      const std::set<nglUChar>& charset(it->second);
      nuiFont* pFont = NULL;
      // First try the requested font
      {
        std::set<nglUChar>::const_iterator it = charset.begin();
        std::set<nglUChar>::const_iterator end = charset.end();
        
        while (it != end && mStyle.GetFont()->GetGlyphIndex(*it) > 0)
          ++it;
        
        // If all the glyphs are available in the font we're done...
        if (it == end)
          pFont = mStyle.GetFont();
        else
        {
          //printf("[couldn't find glyph %d '%c' in requested font] ", *it, *it);
        }
      }

      // If the requested font doesn't work, try to find one that does:
      if (!pFont)
      {
        nuiFontRequest request(mStyle.GetFont());
        request.MustHaveGlyphs(charset, 500, false);
        pFont = nuiFontManager::GetManager().GetFont(request);
      }
      
      mAscender = MAX(mAscender, pFont->GetAscender());
      mDescender = MAX(mDescender, pFont->GetDescender());
      FontSet[it->first] = pFont;
      
      printf("%s\n", pFont->GetFamilyName().GetChars());
      
      ++it;
    }
  }

  i = 0;
  // Assign the correct font to each run
  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      float x = 0;
      float y = 0;
      for (uint32 r = 0; r < pLine->GetRunCount(); r++)
      { 
        nuiTextRun* pRun = pLine->GetRun(r);
        nuiFont* pFont = FontSet[pRun->GetScript()];
        pRun->SetFont(pFont);
        pFont->Shape(pRun);
        
        x += pRun->GetAdvanceX();
        y += pRun->GetAdvanceY();

        mXMin = MIN(mXMin, x);
        mXMax = MAX(mXMax, x);
        mYMin = MIN(mYMin, y);
        mYMax = MAX(mYMax, y);
        
        //printf("\trange %d <%d.%d.%d> (%d - %d) (%s --> %s / %s) (advance: %f / %f)\n", i, p, l, r, pRun->GetPosition(), pRun->GetLength(), nuiGetUnicodeScriptName(pRun->GetScript()).GetChars(), pFont->GetFamilyName().GetChars(), pFont->GetStyleName().GetChars(), pRun->GetAdvanceX(), pRun->GetAdvanceY());

        i++;
      }
    }
  }
  
  
  mCharsets.clear();
  return true;
}

bool nuiTextLayout::LayoutParagraph(int32 start, int32 length)
{
  //printf("new paragraph: %d + %d\n", start, length);

  mpParagraphs.push_back(new Paragraph());

  nuiTextLine* pLine = new nuiTextLine(*this, 0, 0);
  mpParagraphs.back()->push_back(pLine);
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(mUnicode, ranges, nuiST_ScriptChange, start, length);

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    int32 origin = start;
    int32 i = 0;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      int32 len = range.mLength;
      int32 pos = origin;
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
      
      std::set<nglUChar>& charset(mCharsets[range.mScript]);
      {
        while (pos < origin + len)
        {
          nglUChar ch = mUnicode[pos++];
          if (ucisprint(ch))
            charset.insert(ch);
        }
      }

      origin += len;
      ++it;
      i++;
    }
  }
  

  {
    nuiTextRangeList::iterator it = ranges.begin();
    nuiTextRangeList::iterator end = ranges.end();
    uint32 i = 0;
    uint32 pos = start;
    while (it != end)
    {
      const nuiTextRange& range(*it);
      uint32 len = range.mLength;
      printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
         
      nuiTextRun* pRun = new nuiTextRun(*this, range.mScript, pos, len, mStyle);
      pLine->AddRun(pRun);
      
      
      pos += len;
      ++i;
      ++it;
    }
  }
  
  return true;
}

int32 nuiTextLayout::GetParagraphCount() const
{
  return mpParagraphs.size();
}

int32 nuiTextLayout::GetLineCount(int32 Paragraph) const
{
  return mpParagraphs[Paragraph]->size();
}

int32 nuiTextLayout::GetRunCount(int32 Paragraph, int32 Line) const
{
  return GetLine(Paragraph, Line)->GetRunCount();
}

nuiTextLine* nuiTextLayout::GetLine(int32 Paragraph, int32 Line) const
{
  return (*mpParagraphs[Paragraph])[Line];
}

nuiTextRun*  nuiTextLayout::GetRun(int32 Paragraph, int32 Line, int32 Run) const
{
  return GetLine(Paragraph, Line)->GetRun(Run);
}

const nglUChar* nuiTextLayout::GetUnicodeChars() const
{
  return &mUnicode[0];
}

///////////////////////// Old nuiFontLayout interface
int32 nuiTextLayout::GetMetrics(nuiGlyphInfo& rInfo) const
{
  int32 count = GetGlyphCount();
  rInfo.Index = -count;
  if (count > 0)
  {
    rInfo.Width    = mXMax - mXMin;
    rInfo.Height   = mYMax - mYMin;
    rInfo.BearingX = mXMin;
    rInfo.BearingY = mYMax;
    
    float penx = 0;
    float peny = 0;
    
    if (!mpParagraphs.empty())
    {
      Paragraph* pLastParagraph = mpParagraphs.back();
      if (!pLastParagraph->empty())
      {
        nuiTextLine* pLastLine = pLastParagraph->back();
        penx = pLastLine->GetAdvanceX();
        peny = pLastLine->GetAdvanceY();
      }
      
      
    }
    
    rInfo.AdvanceX = penx;
    rInfo.AdvanceY = peny;
  }
  else
  {
    rInfo.Width    = 0.0f;
    rInfo.Height   = 0.0f;
    rInfo.BearingX = 0.0f;
    rInfo.BearingY = 0.0f;
    rInfo.AdvanceX = 0.0f;
    rInfo.AdvanceY = 0.0f;
  }
  return count;
}

float nuiTextLayout::GetAscender() const
{
  return mAscender;
}

float nuiTextLayout::GetDescender() const
{
  return mDescender;
}

int32 nuiTextLayout::GetGlyphCount() const
{
  return 0;
}

const nuiGlyphLayout* nuiTextLayout::GetGlyph(uint Offset) const
{
  return NULL;
}

const nuiGlyphLayout* nuiTextLayout::GetGlyphAt(float X, float Y) const
{
  return NULL;
}

nuiRect nuiTextLayout::GetRect() const
{
  nuiRect r;
  
  
  
  return r.Size();
}

void nuiTextLayout::SetWrapX(nuiSize WrapX)
{
  
}

nuiSize nuiTextLayout::GetWrapX() const
{
  return 0;
}


