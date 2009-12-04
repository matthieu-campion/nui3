/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglFontLayout.h"
#include "nglFontBase.h"
#include "nglMath.h"


/*
 * nglFontLayout
 */

nglFontLayout::nglFontLayout (nglFontBase& rFont, float PenX, float PenY) :
  mFont(rFont)
{
  mPenX = PenX;
  mPenY = PenY;
  mGlyphPrev = 0;
  mpFontPrev = NULL;
  mUseKerning = true;
  SetDownAxis(-1);
  InitMetrics();
}

nglFontLayout::~nglFontLayout()
{
  mGlyphs.clear();
}


void nglFontLayout::Init (float PenX, float PenY)
{
  mPenX = PenX;
  mPenY = PenY;
  mGlyphPrev = 0;
  mGlyphs.clear();
  InitMetrics();
}

void nglFontLayout::UseKerning (bool Use)
{
  mUseKerning = Use;
}

int nglFontLayout::GetMetrics (nglGlyphInfo& rInfo) const
{
  int count = mGlyphs.size();

  rInfo.Index = -count; // Composite glyph convention for nglGlyphInfo.Dump()
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

int nglFontLayout::Layout (const nglString& rText, bool FinalizeLayout)
{
  int len = rText.GetLength();
  if (len == 0)
    return 0;

  int indexes_max = len * 2 + 1; // Let's have extra space (eg. for composite glyphs)
  uint32* indexes = (uint32*) malloc(indexes_max * sizeof(uint32));

  if (!mFont.GetGlyphIndexes(rText.GetChars(), len, indexes, indexes_max))
  {
    free(indexes);
    return -1;
  }

  int i, done = 0;
  for (i = 0; i < len; i++)
  {
    nglGlyphInfo info;

    /* Assume this layout is useful for rendering, and ask for bitmap metrics,
     * this also act as a cache prefetch (glyph is rendered and available).
     */
    if (indexes[i] && mFont.GetGlyphInfo(info, indexes[i], nglFontBase::eGlyphBitmap))
    {
      CallOnGlyph(&mFont, rText, i, &info);
      done++;
    }
    else
    {
      if (rText[i] >= ' ')
      {
        bool handled = false;
        // Try to find an alternative font that contains the missing glyph:
        nglFontBase* pFont = FindFontForMissingGlyph(&mFont, rText[i]);
        
        if (pFont)
        {
          uint32 index = 0;
          const nglChar* pStr = rText.GetChars();
          if (pFont->GetGlyphIndexes(pStr + i, 1, &index, 1) > 0)
          {
            if (pFont->GetGlyphInfo(info, index, nglFontBase::eGlyphBitmap))
            {
              CallOnGlyph(pFont, rText, i, &info);
              done++;
              handled = true;
            }
          }
        }
        
        if (!handled) // We haven't found any suitable font so we look for a replacement glyph:
        {
          if (mFont.GetGlyphInfo(info, '?', nglFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, '!', nglFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, '.', nglFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
          else if (mFont.GetGlyphInfo(info, 0, nglFontBase::eGlyphBitmap))
            CallOnGlyph(&mFont, rText, i, &info);
        }        
      }
      else
      {
        if (mFont.GetGlyphInfo(info, 0, nglFontBase::eGlyphBitmap))
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

  return done;
}

void nglFontLayout::OnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph)
{
  nglChar c = rString[Pos];

  // Handle new line control char
  if (c == _T('\n'))
  {
    if (mGlyphs.size() > 0)
      mPenX = mGlyphs[0].X; // Go back to layout X origin

    mPenY += mDownAxis * pFont->GetHeight(eFontUnitPixel);
  }

  if (c < _T(' ') ||  // skip control chars (includes newline)
      !pGlyph)        // no glyph to render
  {
    mGlyphPrev = 0;
    return;
  }

  // Add kerning vector if applicable
  float kx, ky;
  if (GetKerning(pFont, pGlyph->Index, kx, ky))
  {
    mPenX += kx;
    mPenY += ky;
  }

  // Add this glyph to the layout with the current position pen position
  AddGlyph(pFont, mPenX, mPenY, Pos, pGlyph);

  // Proceed with glyph advance
  mPenX += pGlyph->AdvanceX;
  mPenY += pGlyph->AdvanceY;
}

void nglFontLayout::OnFinalizeLayout ()
{
  // Do nothing by default
}

const nglGlyphLayout* nglFontLayout::GetGlyphAt (float X, float Y) const
{
  if (X < mXMin || X > mXMax || Y < mYMin || Y > mYMax)
    return NULL; // Out of layout bounds

  GlyphList::const_iterator g;

  for (g = mGlyphs.begin(); g != mGlyphs.end(); g++)
  {
    nglGlyphInfo info;
    const nglGlyphLayout& glyph = *g;

    if (glyph.Index >= 0)
    {
      // Existing glyph
      if (!glyph.mpFont->GetGlyphInfo(info, glyph.Index, nglFontBase::eGlyphBitmap))
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

uint nglFontLayout::GetGlyphCount() const
{
  return mGlyphs.size();
}

const nglGlyphLayout* nglFontLayout::GetGlyph (uint Offset) const
{
  return (Offset < mGlyphs.size()) ? &(mGlyphs[Offset]): NULL;
}


/*
 * Protected interface
 */
bool nglFontLayout::AddDummyGlyph(int32 ReferencePos, void* pUserPointer, float W, float H)
{
  nglGlyphLayout glyph;
  
  glyph.X     = mPenX;
  glyph.Y     = mPenY;
  glyph.Pos   = ReferencePos;
  glyph.Index = -1;
  glyph.mpFont = (nglFontBase*)pUserPointer;
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

bool nglFontLayout::AddGlyph (nglFontBase* pFont, float X, float Y, int Pos, nglGlyphInfo* pGlyph)
{
  if (!pGlyph)
    return false;

  nglGlyphLayout glyph;
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
  y = Y + pGlyph->BearingY - pGlyph->Height;
  mXMin = MIN(mXMin, x);
  mYMin = MIN(mYMin, y);

  // Maxima
  x += pGlyph->Width;
  y += pGlyph->Height;//= Y + pGlyph->BearingY;
  mXMax = MAX(mXMax, x);
  mYMax = MAX(mYMax, y);

  return true;
}

bool nglFontLayout::GetKerning (nglFontBase* pFont, uint Index, float& rX, float& rY)
{
  return (mUseKerning &&
          mGlyphPrev &&
          pFont->GetKerning(mGlyphPrev, Index, rX, rY));
}


void nglFontLayout::InitMetrics()
{
  mXMin = mYMin = 1E32f;
  mXMax = mYMax = -1E32f;
  mAscender = mFont.GetAscender();
  mDescender = mFont.GetDescender();
}

void nglFontLayout::CallOnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph)
{
  if (pFont != &mFont)
  {
    float tmpAsc = pFont->GetAscender();
    float tmpDesc = pFont->GetDescender();
    mAscender = MAX(mAscender, tmpAsc);
    mDescender = MAX(mDescender, tmpDesc);
  }
  
  OnGlyph(pFont, rString, Pos, pGlyph);
}

nglFontBase* nglFontLayout::FindFontForMissingGlyph(nglFontBase* pOriginalFont, nglChar Glyph)
{
  return NULL; // We don't use font substitution by default
}


void nglFontLayout::SetDownAxis(float UpDown)
{
  mDownAxis = UpDown;
}

float nglFontLayout::GetDownAxis()
{
  return mDownAxis;
}

float nglFontLayout::GetAscender() const
{
  return mAscender;
}

float nglFontLayout::GetDescender() const
{
  return mDescender;
}


#endif // HAVE_FREETYPE
