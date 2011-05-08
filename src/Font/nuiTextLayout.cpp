/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nuiTextLayout.h"
#include "nuiFontManager.h"

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
nuiTextRun::nuiTextRun(nuiUnicodeScript script, const nglString& rString, int32 Position, int32 Length)
: mpFont(NULL),
  mString(rString),
  mPosition(Position),
  mLength(Length),
  mScript(script),
  mAdvanceX(0),
  mAdvanceY(0)
{
}

nuiTextRun::~nuiTextRun()
{
  mpFont->Release();
}

const std::vector<nuiTextGlyph>& nuiTextRun::GetGlyphs() const
{
  return mGlyphs;
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

nuiFont* nuiTextRun::GetFont() const
{
  return mpFont;
}


int32 nuiTextRun::GetPosition() const
{
  return mPosition;
}

int32 nuiTextRun::GetLength() const
{
  return mLength;
}

const nglString& nuiTextRun::GetString() const
{
  return mString;
}

float nuiTextRun::GetAdvanceX() const
{
  return mAdvanceX;
}

float nuiTextRun::GetAdvanceY() const
{
  return mAdvanceY;
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
  pRun->Acquire();
  mpRuns.push_back(pRun);
}

int32 nuiTextLine::GetRunCount() const
{
  return mpRuns.size();
}

nuiTextRun* nuiTextLine::GetRun(int32 index) const
{
  return mpRuns[index];
}

float nuiTextLine::GetAdvanceY() const
{
  float y = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    y = MAX(y, mpRuns[i]->GetAdvanceY());
  }

  return y;
}

float nuiTextLine::GetAdvanceX() const
{
  float x = 0;
  for (uint32 i = 0; i < mpRuns.size(); i++)
  {
    x += mpRuns[i]->GetAdvanceX();
  }
  
  return x;
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
      //printf("%s -> ", nuiGetUnicodeScriptName(it->first).GetChars());
      const std::set<nglUChar>& charset(it->second);
      nuiFont* pFont = NULL;
      // First try the requested font
      {
        std::set<nglUChar>::const_iterator it = charset.begin();
        std::set<nglUChar>::const_iterator end = charset.end();
        
        while (it != end && mpFont->GetGlyphIndex(*it) > 0)
          ++it;
        
        // If all the glyphs are available in the font we're done...
        if (it == end)
          pFont = mpFont;
        else
        {
          //printf("[couldn't find glyph %d '%c' in requested font] ", *it, *it);
        }
      }

      // If the requested font doesn't work, try to find one that does:
      if (!pFont)
      {
        nuiFontRequest request(mpFont);
        request.MustHaveGlyphs(charset, 10);
        pFont = nuiFontManager::GetManager().GetFont(request);
      }
      
      FontSet[it->first] = pFont;
      
      //printf("%s\n", pFont->GetFamilyName().GetChars());
      
      ++it;
    }
  }

  uint32 i = 0;
  // Assign the correct font to each run
  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      for (uint32 r = 0; r < pLine->GetRunCount(); r++)
      { 
        nuiTextRun* pRun = pLine->GetRun(r);
        nuiFont* pFont = FontSet[pRun->GetScript()];
        pRun->SetFont(pFont);
        pFont->Shape(pRun);
        
        //printf("\trange %d <%d.%d.%d> (%d - %d) (%s --> %s) (advance: %f / %f)\n", i, p, l, r, pRun->GetPosition(), pRun->GetLength(), nuiGetUnicodeScriptName(pRun->GetScript()).GetChars(), pFont->GetFamilyName().GetChars(), pRun->GetAdvanceX(), pRun->GetAdvanceY());

        
        i++;
      }
    }
  }
  
  
  mCharsets.clear();
  return true;
}

bool nuiTextLayout::LayoutParagraph(const nglString& rString, int32 start, int32 length)
{
  //printf("new paragraph: %d + %d\n", start, length);

  mpParagraphs.push_back(new Paragraph());

  nuiTextLine* pLine = new nuiTextLine(0, 0);
  mpParagraphs.back()->push_back(pLine);
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(rString, ranges, nuiST_ScriptChange | nuiST_DirectionChange, start, length);

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
          nglUChar ch = rString.GetNextUChar(pos);
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
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
         
      nuiTextRun* pRun = new nuiTextRun(range.mScript, rString, pos, len);
      pLine->AddRun(pRun);
      
      
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


