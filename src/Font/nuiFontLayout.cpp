/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#include "nuiFontLayout.h"
#include "nuiFontManager.h"
#include "nuiFontBase.h"
#include "nglMath.h"

extern float NUI_SCALE_FACTOR;
extern float NUI_INV_SCALE_FACTOR;

/*
 * nuiFontLayout
 */

nuiFontLayout::nuiFontLayout (nuiFontBase& rFont, float PenX, float PenY, nuiOrientation Orientation) :
  mFont(rFont), mUnderline(false), mStrikeThrough(false)
{
  mPenX = PenX;
  mPenY = PenY;
  mGlyphPrev = 0;
  mpFontPrev = NULL;
  mUseKerning = true;
  SetDownAxis(-1);
  InitMetrics();

  mOrientation = Orientation;
  SetDownAxis(1);
  mSpacesPerTab = 2;
  mWrapX = 0;
  mpCurrentWord = NULL;
  Line l = {mPenX, mPenY, 0.0f };
  mLines.push_back(l);
  mXDensity = 1.0f;
  mYDensity = 1.0f;
}

nuiFontLayout::~nuiFontLayout()
{
  mGlyphs.clear();
  for (size_t i = 0; i < mpSubstitutionFonts.size(); i++)
  {
    mpSubstitutionFonts[i]->Release();
  }
}


void nuiFontLayout::Init (float PenX, float PenY)
{
  mPenX = PenX;
  mPenY = PenY;
  mGlyphPrev = 0;
  mGlyphs.clear();
  InitMetrics();
}

void nuiFontLayout::UseKerning (bool Use)
{
  mUseKerning = Use;
}

int nuiFontLayout::GetMetrics (nuiGlyphInfo& rInfo) const
{
  int count = mGlyphs.size();

  rInfo.Index = -count; // Composite glyph convention for nuiGlyphInfo.Dump()
  if (count > 0)
  {
    rInfo.Width    = mXMax - mXMin;
    rInfo.Height   = mYMax - mYMin;
    rInfo.BearingX = mXMin;
    rInfo.BearingY = mYMax;
    rInfo.AdvanceX = mPenX;
    rInfo.AdvanceY = mPenY;
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

int nuiFontLayout::Layout (const nglString& rText, bool FinalizeLayout)
{
  int len = rText.GetLength();
  if (len == 0)
    return 0;

  nglUChar* pChars = (nglUChar*)rText.Export(sizeof(nglUChar) == 2 ? eUCS2 : eUCS4);
  uint UCount = wcslen(pChars);
  
  int indexes_max = UCount * 2 + 1; // Let's have extra space (eg. for composite glyphs)
  uint32* indexes = (uint32*) malloc(indexes_max * sizeof(uint32));

  if (!mFont.GetGlyphIndexes(pChars, UCount, indexes, indexes_max))
  {
    free(indexes);
    free(pChars);
    return -1;
  }

  int i, done = 0;
  for (i = 0; i < len; i++)
  {
    nuiGlyphInfo info;

    /* Assume this layout is useful for rendering, and ask for bitmap metrics,
     * this also act as a cache prefetch (glyph is rendered and available).
     */
    if (indexes[i] && mFont.GetGlyphInfo(info, indexes[i], nuiFontBase::eGlyphBitmap))
    {
      CallOnGlyph(&mFont, rText, i, &info);
      done++;
    }
    else
    {
      if (pChars[i] >= ' ')
      {
        bool handled = false;
        // Try to find an alternative font that contains the missing glyph:
        nuiFontBase* pFont = FindFontForMissingGlyph(&mFont, pChars[i]);
        
        if (pFont)
        {
          uint32 index = 0;
          if (pFont->GetGlyphIndexes(pChars + i, 1, &index, 1) > 0)
          {
            if (pFont->GetGlyphInfo(info, index, nuiFontBase::eGlyphBitmap))
            {
              CallOnGlyph(pFont, rText, i, &info);
              done++;
              handled = true;
            }
          }
        }
        
        if (!handled) // We haven't found any suitable font so we look for a replacement glyph:
        {
          if (mFont.GetGlyphInfo(info, '?', nuiFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, '!', nuiFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, '.', nuiFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, 0, nuiFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
        }        
      }
      else
      {
        if (mFont.GetGlyphInfo(info, 0, nuiFontBase::eGlyphBitmap))
          CallOnGlyph(&mFont, rText, i, &info);        
      }
    }
  }

  if (FinalizeLayout)
  {
    mpFontPrev = NULL;
    mGlyphPrev = 0;
    OnFinalizeLayout();
  }
  // Finalize the layout (needed to handle complex layout that needs to buffer glyphs in order to manage things such as word wrapping).

  free(indexes);
  free(pChars);
  return done;
}

const nuiGlyphLayout* nuiFontLayout::GetGlyphAt (float X, float Y) const
{
  if (X < mXMin || X > mXMax || Y < mYMin || Y > mYMax)
    return NULL; // Out of layout bounds

  GlyphList::const_iterator g;

  for (g = mGlyphs.begin(); g != mGlyphs.end(); g++)
  {
    nuiGlyphInfo info;
    const nuiGlyphLayout& glyph = *g;

    if (glyph.Index >= 0)
    {
      // Existing glyph
      if (!glyph.mpFont->GetGlyphInfo(info, glyph.Index, nuiFontBase::eGlyphBitmap))
        continue;
      
      float min = glyph.X + info.BearingX;
      float max = min + info.Width;
      if (X < min || X > max)
        continue;
      
      max = glyph.Y + info.BearingY;
      min = max - info.Height;
      if (Y < min || Y > max)
        continue;
      
      return &glyph;
    }
    else
    {
      // dummy glyph / placeholder
      float min = glyph.X;
      float max = ((g + 1) == mGlyphs.end() ? 0 : g->X);
      if (X < min || X > max)
        continue;
            
      return &glyph;
    }
  }

  return NULL;
}

uint nuiFontLayout::GetGlyphCount() const
{
  return mGlyphs.size();
}

const nuiGlyphLayout* nuiFontLayout::GetGlyph (uint Offset) const
{
  return (Offset < mGlyphs.size()) ? &(mGlyphs[Offset]): NULL;
}


/*
 * Protected interface
 */
bool nuiFontLayout::AddDummyGlyph(int32 ReferencePos, void* pUserPointer, float W, float H)
{
  nuiGlyphLayout glyph;
  
  glyph.X     = mPenX;
  glyph.Y     = mPenY;
  glyph.Pos   = ReferencePos;
  glyph.Index = -1;
  glyph.mpFont = (nuiFontBase*)pUserPointer;
  mGlyphs.push_back(glyph);
  
  mpFontPrev = NULL;
  mGlyphPrev = 0;  
  
  /* Update global layout metrics
   */
  float x, y;
  
  // Minima
  x = mPenX;
  y = mPenY;
  mXMin = MIN(mXMin, x);
  mYMin = MIN(mYMin, y);
  
  // Maxima
  x += W;
  y += H;
  mXMax = MAX(mXMax, x);
  mYMax = MAX(mYMax, y);
  
  mPenX += W;
  mPenY += H;
  return true;
}

bool nuiFontLayout::AddGlyph (nuiFontBase* pFont, float X, float Y, int Pos, nuiGlyphInfo* pGlyph)
{
  if (!pGlyph)
    return false;

  nuiGlyphLayout glyph;
  uint index = pGlyph->Index;

  glyph.X     = X;
  glyph.Y     = Y;
  glyph.Pos   = Pos;
  glyph.Index = index;
  glyph.mpFont = pFont;
  mGlyphs.push_back(glyph);

  if (pFont == mpFontPrev)
  {
    mGlyphPrev = index;
  }
  else  
  {
    mpFontPrev = pFont;
    mGlyphPrev = 0;
  }
  

  /* Update global layout metrics
   */
  float x, y;

  // Minima
  x = X + pGlyph->BearingX;
  y = Y + (pGlyph->BearingY - pGlyph->Height);
  mXMin = MIN(mXMin, x);
  mYMin = MIN(mYMin, y);

  // Maxima
  x += pGlyph->Width;
  y += pGlyph->Height;//= Y + pGlyph->BearingY;
  mXMax = MAX(mXMax, x);
  mYMax = MAX(mYMax, y);

  return true;
}

bool nuiFontLayout::GetKerning (nuiFontBase* pFont, uint Index, float& rX, float& rY)
{
  bool res = pFont->GetKerning(mGlyphPrev, Index, rX, rY);
  return (mUseKerning && mGlyphPrev && res);
}


void nuiFontLayout::InitMetrics()
{
  mXMin = mYMin = 1E32f;
  mXMax = mYMax = -1E32f;
  mAscender = mFont.GetAscender();
  mDescender = mFont.GetDescender();
}

void nuiFontLayout::CallOnGlyph (nuiFontBase* pFont, const nglString& rString, int Pos, nuiGlyphInfo* pGlyph)
{
  if (pFont != &mFont)
  {
    float tmpAsc = pFont->GetAscender();
    float tmpDesc = pFont->GetDescender();
    mAscender = MAX(mAscender, tmpAsc);
    mDescender = MAX(mDescender, tmpDesc);
  }
  
  
  //NGL_ASSERT(!(pFont->GetFamilyName() == _T("STHeiti") && Pos == 22));
  
  OnGlyph(pFont, rString, Pos, pGlyph);
}

void nuiFontLayout::SetDownAxis(float UpDown)
{
  mDownAxis = UpDown;
}

float nuiFontLayout::GetDownAxis()
{
  return mDownAxis;
}

float nuiFontLayout::GetAscender() const
{
  return mAscender;
}

float nuiFontLayout::GetDescender() const
{
  return mDescender;
}

nuiFontLayout::WordElement::WordElement(nuiGlyphInfo Glyph, nglUChar Char, int Pos, nuiFontBase* pFont)
{
  mGlyph = Glyph;
  mChar = Char;
  mPos = Pos;
  mpFont = pFont;
}

void nuiFontLayout::SetUnderline(bool set)
{
  mUnderline = set;
}

bool nuiFontLayout::GetUnderline() const
{
  return mUnderline;
}

void nuiFontLayout::SetStrikeThrough(bool set)
{
  mStrikeThrough = set;
}

bool nuiFontLayout::GetStrikeThrough() const
{
  return mStrikeThrough;
}

nuiFontLayout::WordElement::WordElement(const WordElement& rWordElement)
{
  mGlyph = rWordElement.mGlyph;
  mChar = rWordElement.mChar;
  mPos = rWordElement.mPos;
  mpFont = rWordElement.mpFont;
}


void nuiFontLayout::OnGlyph (nuiFontBase* pFont, const nglString& rString, int Pos, nuiGlyphInfo* pGlyph)
{
  nglUChar c = rString[Pos];
  if (mOrientation == nuiVertical)
  {
    nuiFontInfo info;
    pFont->GetInfo(info);
    // Handle new line control char
    if (c == _T('\n'))
    {
      if (mGlyphs.size() > 0)
        mPenY = mGlyphs[0].Y; // Go back to layout Y origin
      mPenX += info.AdvanceMaxW * 2.0f;
    }
    else if (c == _T('\t'))
    {
      mPenY += info.AdvanceMaxH * mSpacesPerTab;
    }
    else
    {
      // Add this glyph to the layout with the current position pen position
      float x = mPenX;// + info.AdvanceMaxW/2.f - pGlyph->BearingX;//pGlyph->Width/2.f;
      float y = mPenY;// - (info).BBoxMinY;
      float kx = 0, ky = 0;
      if (GetKerning(pFont, pGlyph->Index, kx, ky))
      {
        x += kx;
        y += ky;
      }
      AddGlyph(pFont, x, y, Pos, pGlyph);
      
      mPenY += mDownAxis * info.AdvanceMaxH;
    }
  }                           
  else// (orient == nuiHorizontal)
  {
    if (!(mWrapX > 0.0f)) // No X Wrapper
    {
      // Handle new line control char
      if (c == _T('\n'))
      {
        if (mGlyphs.size() > 0)
          mPenX = mGlyphs[0].X; // Go back to layout X origin
        nuiFontInfo info;
        pFont->GetInfo(info);
        mPenY += (mDownAxis * info.AdvanceMaxH) * mYDensity;
        
        Line l = {mPenX, mPenY, 0.0f };
        mLines.push_back(l);
        if (mNewLineDelegate)
          mNewLineDelegate(Pos, false);
      }
      else if (c == _T('\t'))
      {
        nuiFontInfo info;
        pFont->GetInfo(info);
        mPenX += info.AdvanceMaxW * mSpacesPerTab;
        mLines[mLines.size() - 1].mWidth += info.AdvanceMaxW * mSpacesPerTab;
      }
      else if (c < _T(' ') ||  // skip control chars (includes newline)
               !pGlyph)        // no glyph to render
      {
        mGlyphPrev = 0;
        return;
      }
      else // Add Glyph
      {
        // Add kerning vector if applicable
        float kx = 0, ky = 0;
        if (GetKerning(pFont, pGlyph->Index, kx, ky))
        {
          mPenX += kx;
          mPenY += ky;
        }
        
        // Add this glyph to the layout with the current position pen position
        AddGlyph(pFont, mPenX, mPenY, Pos, pGlyph);
        
        // Proceed with glyph advance
        mPenX += pGlyph->AdvanceX * mXDensity;
        mPenY += pGlyph->AdvanceY;
        
        mLines[mLines.size() - 1].mWidth += pGlyph->AdvanceX;
      }
    }
    else // Using Wrapper
    {
      if (!mpCurrentWord)
        mpCurrentWord = new Word;
      
      WordElement element(*pGlyph, c, Pos, pFont);
      
      // Handle new line control char and separate words:
      if ( c <= _T(' ') 
          || c == _T('.')
          || c == _T(',')
          || c == _T(';')
          || c == _T('!')
          || c == _T(':')
          || c == _T('?')
          || c == _T('*')
          || c == _T('}')
          || c == _T(']')
          )
      {
        if (mpCurrentWord->empty())
          delete mpCurrentWord;
        else
          mWords.push_back(mpCurrentWord);
        mpCurrentWord = mpCurrentWord = new Word;
        mpCurrentWord->push_back(element);
        mWords.push_back(mpCurrentWord);
        mpCurrentWord = NULL;
      }
      else
      {
        mpCurrentWord->push_back(element);
      }
    }
  }
}

void nuiFontLayout::GetWordSize(nuiRect& rRect, const nuiFontLayout::GlyphList& rGlyphs)
{
  rRect = nuiRect();
  GlyphList::const_iterator it = rGlyphs.begin();
  GlyphList::const_iterator end = rGlyphs.end();
  
  nuiRect r;
  
  nuiFontInfo finfo;
  mFont.GetInfo(finfo);
  
  while (it != end)
  {
    const nuiGlyphLayout& rGlyph(*it);
    nuiGlyphInfo info;
    rGlyph.mpFont->GetGlyphInfo(info, rGlyph.Index, nuiFontBase::eGlyphBitmap);
    nuiSize w = info.AdvanceX;
    nuiSize h = finfo.AdvanceMaxH;
    nuiSize x = rGlyph.X + info.BearingX;
    nuiSize y = rGlyph.Y - finfo.Ascender;
    
    r.Union(r, nuiRect(x, y, w, h));
    
    ++it;
  }
  
  rRect = rRect.Size();
}


void nuiFontLayout::OnFinalizeLayout()
{
  mGlyphPrev = 0;
  mGlyphPrev = 0;
  if (mWrapX > 0.0f) // X Wrapper
  {
    if (mpCurrentWord)
    {
      if (!mpCurrentWord->empty())
        mWords.push_back(mpCurrentWord);
      else
        delete mpCurrentWord;
    }
    
    mpCurrentWord = NULL;
    
    nuiFontInfo info;
    mFont.GetInfo(info);
    
    std::list<Word*>::iterator it = mWords.begin();
    std::list<Word*>::iterator end = mWords.end();
    
    
    //NGL_OUT(_T("\n\nFinalize with wrap at %f\n"), mWrapX);
    while (it != end)
    {
      Word* pWord = *it;
      
      //NGL_OUT(_T("New Word %f %f\n"), mPenX, mPenY);
      
      nglUChar c = pWord->front().mChar;
      nuiGlyphInfo* pGlyph = &pWord->front().mGlyph;
      // Handle new line control char
      if (c == _T('\n'))
      {
        //NGL_OUT(_T("EOL\n"));
        if (mGlyphs.size() > 0)
          mPenX = mGlyphs[0].X; // Go back to layout X origin
        mPenY += mDownAxis * info.AdvanceMaxH;
        mGlyphPrev = 0;
        mpFontPrev = NULL;
        
        if (mNewLineDelegate && !mNewLineDelegate(pWord->front().mPos, false))
        {
          return; // Stop adding lines
        }
        else
        {
          Line l = {mPenX, mPenY, 0.0f };
          mLines.push_back(l);
        }          
        
      }
      else if (c == _T('\t'))
      {
        //NGL_OUT(_T("TAB\n"));
        if (mPenX > 0.0f)
        {
          mPenX += info.AdvanceMaxW * mSpacesPerTab;
          mLines[mLines.size() - 1].mWidth += info.AdvanceMaxW * mSpacesPerTab;
        }
        mGlyphPrev = 0;
        mpFontPrev = NULL;
      }
      else if (c < _T(' ') ||  // skip control chars (includes newline)
               !pGlyph)        // no glyph to render
      {
        //NGL_OUT(_T("<' '\n"));
        mGlyphPrev = 0;
        mpFontPrev = NULL;
      }
      else // Add Glyph
      {
        //NGL_OUT(_T("\tNormal word\n"));
        float X = 0;
        float Y = 0;
        
        Word::iterator cit = pWord->begin();
        Word::iterator cend = pWord->end();
        
        nuiRect r;
        
        int32 pos = 0;
        int prev = mGlyphPrev;
        while (cit != cend)
        {
          WordElement& rElement(*cit);
          pos = rElement.mPos;
          c = rElement.mChar;
          pGlyph = &rElement.mGlyph;
          
          nuiFontBase* pFont = rElement.mpFont;
          
          // Add kerning vector if applicable
          float kx = 0, ky = 0;
          if (mpFontPrev && mGlyphPrev && GetKerning(pFont, pGlyph->Index, kx, ky))
          {
            X += kx;
            Y += ky;
          }
          
          // Proceed with glyph advance
          X += pGlyph->AdvanceX;
          Y += pGlyph->AdvanceY;
          
          {
            nuiSize w = pGlyph->AdvanceX;
            nuiSize h = info.AdvanceMaxH;
            nuiSize x = X + pGlyph->BearingX;
            nuiSize y = Y - info.Ascender;
            
            r.Union(r, nuiRect(x, y, w, h));
          }
          
          mGlyphPrev = pGlyph->Index;
          mpFontPrev = pFont;
          
          ++cit;
        }
        
        if (mPenX + r.GetWidth() >= mWrapX) // Do the actual wrapping:
        {
          if (mGlyphs.size() > 0)
          {
            mPenX = mGlyphs[0].X; // Go back to layout X origin
            mPenY += mDownAxis * info.AdvanceMaxH;
            
            if (mNewLineDelegate && !mNewLineDelegate(pos, false))
            {
              std::list<Word*>::iterator it = mWords.begin();
              std::list<Word*>::iterator end = mWords.end();
              
              while (it != end)
              {
                Word* pWord = *it;
                delete pWord;
                
                ++it;
              }
              
              mWords.clear();
              mpCurrentWord = NULL;
              return; // Stop adding lines
            }
            else
            {
              Line l = {mPenX, mPenY, 0.0f };
              mLines.push_back(l);
            }          
          }
          //NGL_OUT(_T("Wrapped to %f %f\n"), mPenX, mPenY);
        }
        
        // Add the glyphs to the layout:
        if (mPenX == 0.0f && c <= ' ')
        {
          // Skip the blanks that start on the beginning of the line
        }
        else
        {
          mGlyphPrev = prev;
          cit = pWord->begin();
          cend = pWord->end();
          
          while (cit != cend)
          {
            WordElement& rElement(*cit);
            c = rElement.mChar;
            pGlyph = &rElement.mGlyph;
            
            int Pos = rElement.mPos;
            //NGL_OUT(_T("\t'%c'   (%d)\n"), c, pGlyph->Index);
            
            // Add kerning vector if applicable
            nuiFontBase* pFont = rElement.mpFont;
            
            float kx = 0, ky = 0;
            if (mGlyphPrev && pFont && mpFontPrev == pFont && GetKerning(pFont, pGlyph->Index, kx, ky))
            {
              mPenX += kx;
              mPenY += ky;
            }
            
            // Add this glyph to the layout with the current position pen position
            AddGlyph(pFont, mPenX, mPenY, Pos, pGlyph);
            
            // Proceed with glyph advance
            mPenX += pGlyph->AdvanceX * mXDensity;
            mPenY += pGlyph->AdvanceY;
            
            mLines[mLines.size() - 1].mWidth += (pGlyph->AdvanceX + kx);
            
            mGlyphPrev = pGlyph->Index;
            mpFontPrev = pFont;
            
            ++cit;
          }
        }
      }
      
      ++it;
    }
  }
  
  std::list<Word*>::iterator it = mWords.begin();
  std::list<Word*>::iterator end = mWords.end();
  
  while (it != end)
  {
    Word* pWord = *it;
    delete pWord;
    
    ++it;
  }
  
  mWords.clear();
  mpCurrentWord = NULL;
}

void nuiFontLayout::SetSpacesPerTab(int count)
{
  mSpacesPerTab = count;
}

int nuiFontLayout::GetSpacesPerTab()
{
  return mSpacesPerTab;
}

void nuiFontLayout::SetWrapX(nuiSize WrapX)
{
  mWrapX = WrapX;
}

nuiSize nuiFontLayout::GetWrapX() const
{
  return mWrapX;
}

nuiRect nuiFontLayout::GetRect() const
{
  GlyphList::const_iterator it = mGlyphs.begin();
  GlyphList::const_iterator end = mGlyphs.end();
  
  nuiRect r;
  
  nuiFontInfo finfo;
  mFont.GetInfo(finfo);
  
  while (it != end)
  {
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
    
    ++it;
  }
  
  return r.Size();
}

nuiFontBase* nuiFontLayout::FindFontForMissingGlyph(nuiFontBase* pOriginalFont, nglUChar Glyph)
{
  //NGL_OUT(_T("Try to find font for char 0x%x (%c)\n"), Glyph, Glyph);
  
  //First look for the glyph in the fonts we already have:
  for (size_t i = 0; i < mpSubstitutionFonts.size(); i++)
  {
    uint index = 0;
    if (!mpSubstitutionFonts[i]->IsLastResort() && mpSubstitutionFonts[i]->GetGlyphIndexes(&Glyph, 1, &index, 1) > 0 && index)
    {
      //NGL_OUT(_T("found in previous fonts '%s' as glyph 0x%x OK\n"), mpSubstitutionFonts[i]->GetFamilyName().GetChars(), index);
      return mpSubstitutionFonts[i];
    }
  }
  
  // try to find a font with the glyph that is similar enough:
  nuiFontManager& rFontManager(nuiFontManager::GetManager());
  nuiFontRequest request(pOriginalFont);
  request.MustHaveGlyph(Glyph, 1.0f, true);
  
  std::list<nuiFontRequestResult> fonts;
  rFontManager.RequestFont(request, fonts);
  
  if (!fonts.empty())
  {
    const nuiFontRequestResult& rResult(fonts.front());
    
    nglPath path(rResult.GetPath());
    float size = pOriginalFont->GetSize();
    int face = rResult.GetFace();
    nuiFont* pFont = nuiFont::GetFont(path, size, face);
    
    NGL_OUT(_T("pFont '%p' path = '%s'   size = %f   face = %d\n"), pFont, path.GetPathName().GetChars(), size, face);
    
    uint index = 0;
    nuiFont* pTempFont = pFont;
    NGL_OUT("pTempFont %p", pTempFont);
    if (pFont->GetGlyphIndexes(&Glyph, 1, &index, 1) > 0)
    {
      pFont = pTempFont;
      NGL_OUT(_T("Found font '%s' with glyph index %d\n"), pFont->GetFamilyName().GetChars(), index);
      
      NGL_ASSERT(rResult.GetFontDesc()->HasGlyph(Glyph));
      
      mpSubstitutionFonts.push_back(pFont);
      
      return pFont;
    }
  }
  
  return NULL; // We don't have a valid font for substitution...
}

const std::vector<nuiFontLayout::Line>& nuiFontLayout::GetLines() const
{
  return mLines;
}

void nuiFontLayout::SetDensity(nuiSize X, nuiSize Y)
{
  mXDensity = X;
  mYDensity = Y;
}

nuiSize nuiFontLayout::GetDensityX() const
{
  return mXDensity;
}

nuiSize nuiFontLayout::GetDensityY() const
{
  return mYDensity;
}

void nuiFontLayout::SetNewLineDelegate(const NewLineDelegate& rDelegate)
{
  mNewLineDelegate = rDelegate;
}

