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
nuiTextLayout::nuiTextLayout(nuiFont* pFont, nuiOrientation Orientation)
:
mXMin(0), mXMax(0), mYMin(0), mYMax(0),
mAscender(0),
mDescender(0),
mOrientation(Orientation)
{
  mStyle.SetFont(pFont);
}

nuiTextLayout::nuiTextLayout(const nuiTextStyle& rStyle, nuiOrientation Orientation)
:
mXMin(0), mXMax(0), mYMin(0), mYMax(0),
mAscender(0),
mDescender(0),
mStyle(rStyle),
mOrientation(Orientation)
{
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

bool nuiTextLayout::Layout(const nglString& rString)
{
  // Transform the string in a vector of nglUChar, also keep the offsets from the original chars to the nglUChar and vice versa
  int32 len = rString.GetLength();
  int32 i = 0;
  
  //printf("layout ");
  while (i < len)
  {
    nglUChar ch = rString.GetNextUChar(i);
    //printf("'%c' (%d) ", (char)ch, ch);
    mUnicode.push_back(ch);
    mOffsetInString.push_back(i);
    mOffsetInUnicode.push_back(mUnicode.size() - 1);
  }
  
  //printf("\n");
  
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
      //printf("Paragraph %d -> %d (%d chars)\n", start, position, position - start);
      LayoutParagraph(start, position - start); // Eat the \n char
      start = position + 1;
    }
    position++;
  }
  
  if (start < position)
  {
    //printf("last Paragraph %d -> %d (%d chars)\n", start, position, position - start);
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
      //printf("%s -> ", nuiGetUnicodeScriptName(it->first).GetChars());
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
      
      FontSet[it->first] = pFont;
      
      //printf("%s\n", pFont->GetFamilyName().GetChars());
      
      ++it;
    }
  }

  i = 0;
  nuiRect rect;
  float PenX = 0;
  float PenY = 0;
  // Assign the correct font to each run
  for (uint32 p = 0; p < mpParagraphs.size(); p++)
  {
    Paragraph* pParagraph = mpParagraphs[p];
    for (uint32 l = 0; l < pParagraph->size(); l++)
    {
      nuiTextLine* pLine = (*pParagraph)[l];
      PenX = 0;
      float x = 0;
      float y = 0;
      for (uint32 r = 0; r < pLine->GetRunCount(); r++)
      { 
        nuiTextRun* pRun = pLine->GetRun(r);
        nuiFont* pFont = FontSet[pRun->GetScript()];
        if (!pRun->IsDummy())
        {
          // Only shape real runs.
          pRun->SetFont(pFont);
          pFont->Shape(pRun);

          nuiFontInfo finfo;
          pFont->GetInfo(finfo);
          
          
          // Prepare glyphs:
          std::vector<nuiTextGlyph>& rGlyphs(pRun->GetGlyphs());
          for (int32 g = 0; g < rGlyphs.size(); g++)
          {
            nuiTextGlyph& rGlyph(rGlyphs.at(g));
            
            pFont->PrepareGlyph(PenX + x, PenY + y, rGlyph);
            
            const nuiSize W = rGlyph.AdvanceX;
            //    nuiSize h = finfo.AdvanceMaxH;
            const nuiSize X = rGlyph.mX + rGlyph.BearingX;
            const nuiSize Y = rGlyph.mY - finfo.Ascender;
            const nuiSize H = finfo.Height;
            
            nuiRect rr(rect);
            rect.Union(rr, nuiRect(X, Y, W, H));
          }
        }

        
        x += pRun->GetAdvanceX();
        //y += pRun->GetAdvanceY();


        //printf("\trange %d <%d.%d.%d> (%d - %d) (%s --> %s / %s) (advance: %f / %f)\n", i, p, l, r, pRun->GetPosition(), pRun->GetLength(), nuiGetUnicodeScriptName(pRun->GetScript()).GetChars(), pFont->GetFamilyName().GetChars(), pFont->GetStyleName().GetChars(), pRun->GetAdvanceX(), pRun->GetAdvanceY());

        i++;
      }
      PenY += pLine->GetAdvanceY();
    }
  }
  
  nuiTextLine* pFirstLine = NULL;
  if (GetParagraphCount() > 0)
    if (GetLineCount(0) > 0)
      pFirstLine = GetLine(0, 0);
  
  if (pFirstLine)
    mAscender = pFirstLine->GetAscender();
    
  nuiTextLine* pLastLine = NULL;
  if (GetParagraphCount() > 0)
    if (GetLineCount(GetParagraphCount() - 1) > 0)
      pLastLine = GetLine(GetParagraphCount() - 1, GetLineCount(GetParagraphCount() - 1) - 1);
  
  if (pLastLine)
    mDescender = pLastLine->GetDescender();


  mXMin = rect.Left();
  mXMax = rect.Right();
  mYMin = rect.Top();
  mYMax = rect.Bottom();

  mCharsets.clear();
  return true;
}

bool Split(nglUChar previousch, nglUChar ch, int32 index)
{
  if (previousch && ((previousch < 32) != (ch < 32)))
    return true;
  return false;
}

bool nuiTextLayout::LayoutParagraph(int32 start, int32 length)
{
  //printf("new paragraph: %d + %d\n", start, length);

  float spacewidth = 0;
  {
    nuiFont* pFont = mStyle.GetFont();
    nuiGlyphInfo glyphinfo;
    uint32 space = pFont->GetGlyphIndex(32);
    pFont->GetGlyphInfo(glyphinfo, space, nuiFontBase::eGlyphNative);
    spacewidth = glyphinfo.AdvanceX;
  }
  
  mpParagraphs.push_back(new Paragraph());

  nuiTextLine* pLine = new nuiTextLine(*this, 0, 0);
  mpParagraphs.back()->push_back(pLine);
  
  // Split the paragraph into ranges:
  nuiTextRangeList ranges;
  nuiSplitText(mUnicode, ranges, nuiST_ScriptChange, start, length, &Split);

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
      //printf("\trange %d (%d - %d) (%s - %s)\n", i, pos, len, nuiGetUnicodeScriptName(range.mScript).GetChars(), nuiGetUnicodeRangeName(range.mRange).GetChars());
      nglUChar ch = mUnicode[pos];
      
      nuiTextRun* pRun = NULL;
      
      if (ch < 32)
      {
        int32 tabs = 0;
        for (uint32 i = pos; i < pos + range.mLength; i++)
        {
          if (mUnicode[i] == 9)
            tabs++;
        }
        
        if (tabs > 0)
        {
          pRun = new nuiTextRun(*this, pos, len, spacewidth * (float)tabs, 0.0f);
        }
      }
      else
      {
        pRun = new nuiTextRun(*this, range.mScript, pos, len, mStyle);
      }
      
      if (pRun)
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
  int32 count = 0;
  // Iterate runs:
  for (int32 p = 0; p < GetParagraphCount(); p++)
  {
    for (int32 l = 0; l < GetLineCount(p); l++)
    {
      nuiTextLine* pLine = GetLine(p, l);
      
      count += pLine->GetGlyphCount();
    }
  }
  
  return count;
}

const nuiTextGlyph* nuiTextLayout::GetGlyph(int32 Offset) const
{
  for (int32 p = 0; p < GetParagraphCount(); p++)
  {
    for (int32 l = 0; l < GetLineCount(p); l++)
    {
      nuiTextLine* pLine = GetLine(p, l);
      
      int32 s = pLine->GetGlyphCount();

      if (Offset < s)
      {
        return pLine->GetGlyph(Offset);
      }

      Offset -= s;
    }
  }
  
  return NULL;
}

const nuiTextGlyph* nuiTextLayout::GetGlyphAt(float X, float Y) const
{
  for (int32 p = 0; p < GetParagraphCount(); p++)
  {
    for (int32 l = 0; l < GetLineCount(p); l++)
    {
      nuiTextLine* pLine = GetLine(p, l);
      const nuiTextGlyph* pGlyph = pLine->GetGlyphAt(X, Y);

      if (pGlyph)
        return pGlyph;
    }
  }
  
  return NULL;
}

nuiRect nuiTextLayout::GetRect() const
{
  nuiRect r(mXMin, mYMin, mXMax, mYMax, false);
  return r.Size();
}

void nuiTextLayout::SetWrapX(nuiSize WrapX)
{
  
}

nuiSize nuiTextLayout::GetWrapX() const
{
  return 0;
}


bool nuiTextLayout::PrintGlyphs(nuiDrawContext *pContext, float X, float Y, const std::map<nuiTexture*, std::vector<nuiTextGlyph*> >& rGlyphs, bool AlignGlyphPixels) const
{
  std::map<nuiTexture*, std::vector<nuiTextGlyph*> >::const_iterator it = rGlyphs.begin();
  std::map<nuiTexture*, std::vector<nuiTextGlyph*> >::const_iterator end = rGlyphs.end();
  
  bool texturing = pContext->GetState().mTexturing;
  nuiTexture* pOldTexture = pContext->GetTexture();
  if (pOldTexture)
    pOldTexture->Acquire();
  
  pContext->EnableTexturing(true);
  
  const float f = nuiGetScaleFactor();
  const float i_f = nuiGetInvScaleFactor();
  
  while (it != end)
  {
    nuiTexture* pTexture = it->first;
    pContext->SetTexture(pTexture);
    int size = (int)it->second.size();
    int i;
    
    nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLES);
    pArray->EnableArray(nuiRenderArray::eVertex);
    pArray->EnableArray(nuiRenderArray::eTexCoord);
    pArray->Reserve(6 * size);
    
    for (i = 0; i < size; i++)
    {
      const nuiRect& rDest = it->second[i]->mDestRect;
      const nuiRect& rSource = it->second[i]->mSourceRect;
      
      nuiSize x1,y1,x2,y2;
      nuiSize tx,ty,tw,th;
      
      x1 = rDest.mLeft + X;
      y1 = rDest.mTop + Y;
      x2 = rDest.mRight + X;
      y2 = rDest.mBottom + Y;

      if (AlignGlyphPixels)
      {
        x1 = ToNearest(x1 * f) * i_f;
        y1 = ToNearest(y1 * f) * i_f;
        x2 = ToNearest(x2 * f) * i_f;
        y2 = ToNearest(y2 * f) * i_f;
      }
      
      tx = rSource.mLeft;
      ty = rSource.mTop;
      tw = rSource.mRight;
      th = rSource.mBottom;
      
      pTexture->ImageToTextureCoord(tx, ty);
      pTexture->ImageToTextureCoord(tw,th);
      
      ///////////////////////////////////////////
      pArray->SetVertex(x1, y1);
      pArray->SetTexCoords(tx, ty);
      pArray->PushVertex();
      
      pArray->SetVertex(x2, y1);
      pArray->SetTexCoords(tw, ty);
      pArray->PushVertex();
      
      pArray->SetVertex(x2, y2);
      pArray->SetTexCoords(tw, th);
      pArray->PushVertex();
      
      ///////////////////////////////////////////
      pArray->SetVertex(x1, y1);
      pArray->SetTexCoords(tx, ty);
      pArray->PushVertex();
      
      pArray->SetVertex(x2, y2);
      pArray->SetTexCoords(tw, th);
      pArray->PushVertex();
      
      pArray->SetVertex(x1, y2);
      pArray->SetTexCoords(tx, th);
      pArray->PushVertex();
    }
    
    //nglString str = pArray->Dump();
    //NGL_OUT("%s", str.GetChars());
    pContext->DrawArray(pArray);
    
    ++it;
  }
  
  pContext->EnableTexturing(texturing);
  pContext->SetTexture(pOldTexture);
  if (pOldTexture)
    pOldTexture->Release();
  
  return true;
}

void nuiTextLayout::Print(nuiDrawContext* pContext, float X, float Y, bool AlignGlyphPixels) const
{
  bool blendsaved = pContext->GetState().mBlending;
  bool texturesaved = pContext->GetState().mTexturing;
  
  pContext->EnableBlending(true);
  pContext->EnableTexturing(true);
  
  nuiColor SavedColor = pContext->GetFillColor();
  nuiColor oldcolor(pContext->GetStrokeColor());
  pContext->SetStrokeColor(pContext->GetTextColor());
  pContext->SetFillColor(pContext->GetTextColor());
  pContext->SetBlendFunc(nuiBlendTransp);
  
  std::map<nuiTexture*, std::vector<nuiTextGlyph*> > Glyphs;
  
  float x = X;
  float y = Y;
  
  // Iterate runs:
  for (int32 p = 0; p < GetParagraphCount(); p++)
  {
    for (int32 l = 0; l < GetLineCount(p); l++)
    {
      x = X;
      nuiTextLine* pLine = GetLine(p, l);
      for (int32 r = 0; r < pLine->GetRunCount(); r++)
      {
        nuiTextRun* pRun = pLine->GetRun(r);
        std::vector<nuiTextGlyph>& rGlyphs(pRun->GetGlyphs());
        nuiFont* pFont = pRun->GetFont();
        
        for (int32 g = 0; g < rGlyphs.size(); g++)
        {
          nuiTextGlyph& rGlyph(rGlyphs.at(g));
          Glyphs[rGlyph.mpTexture].push_back(&rGlyph);
        }
        
        // Draw underlines and strike through if needed
        if (pRun->GetUnderline() || pRun->GetStrikeThrough())
        {
          nuiFontInfo info;
          pFont->GetInfo(info);
          float thickness = ToNearest(info.UnderlineThick);
          pContext->SetLineWidth(thickness);
          
          const float x1 = x;
          const float x2 = x + pRun->GetAdvanceX();
          
          if (pRun->GetUnderline())
          {
            const float pos = -info.UnderlinePos;
            const float _y = ToNearest(y + pos);
            if (x1 != x2)
              pContext->DrawLine(x1, _y, x2, _y);
          }
          
          if (pRun->GetStrikeThrough())
          {
            const float pos = -info.Ascender * .4f;
            const float _y = ToNearest(y + pos);
            if (x1 != x2)
              pContext->DrawLine(x1, _y, x2, _y);
          }
        }
        
        x += pRun->GetAdvanceX();
      }
      y += pLine->GetAdvanceY();
    }
  }
  
  PrintGlyphs(pContext, X, Y, Glyphs, AlignGlyphPixels);
  
  
  pContext->EnableBlending(blendsaved);
  pContext->EnableTexturing(texturesaved);
  
  pContext->SetFillColor(SavedColor);
  pContext->SetStrokeColor(oldcolor);
}

void nuiTextLayout::GetLines(std::vector<uint>& rLines) const
{
  for (int32 p = 0; p < GetParagraphCount(); p++)
  {
    for (int32 l = 0; l < GetLineCount(p); l++)
    {
      nuiTextLine* pLine = GetLine(p, l);
      rLines.push_back(pLine->GetRun(0)->GetPosition());
    }
  }
}

void nuiTextLayout::SetUnderline(bool set)
{
  mStyle.SetUnderline(set);
}

bool nuiTextLayout::GetUnderline() const
{
  return mStyle.GetUnderline();
}

void nuiTextLayout::SetStrikeThrough(bool set)
{
  mStyle.SetStrikeThrough(set);
}

bool nuiTextLayout::GetStrikeThrough() const
{
  return mStyle.GetStrikeThrough();
}





