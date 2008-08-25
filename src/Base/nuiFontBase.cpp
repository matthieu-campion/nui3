/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"

#include "nuiFontBase.h"

#include "nglKernel.h"
#include "nglMath.h"
#include "nuiTexture.h"

#include "nglFontLayout.h"
#include "nglImage.h"

#include "nuiDrawContext.h"
#include "nglBitmapTools.h"

#include "nuiFontManager.h"

#include "harfbuzz.h"

using namespace std;


static const nglChar* gpFontErrorTable[] =
{
  /*  0 */ _T("No error"),
  /*  1 */ _T("Something bad happened"),
  NULL
};

//class nuiFontLayout : public nglFontLayout
nuiFontLayout::nuiFontLayout(nglFontBase& rFont, float PenX , float PenY, nuiOrientation Orientation)
  : nglFontLayout(rFont, PenX, PenY)
{
  mOrientation = Orientation;
  SetDownAxis(1);
  mSpacesPerTab = 2;
  mWrapX = 0;
  mpCurrentWord = NULL;
}

nuiFontLayout::~nuiFontLayout()
{
  for (size_t i = 0; i < mpSubstitutionFonts.size(); i++)
  {
    mpSubstitutionFonts[i]->Release();
  }
}

nuiFontLayout::WordElement::WordElement()
{
  //mGlyph;
  mChar = 0;
  mPos = -1;
}

nuiFontLayout::WordElement::WordElement(const WordElement& rWordElement)
{
  mGlyph = rWordElement.mGlyph;
  mChar = rWordElement.mChar;
  mPos = rWordElement.mPos;
  mpFont = rWordElement.mpFont;
}


void nuiFontLayout::OnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph)
{
  nglChar c = rString[Pos];
  if (mOrientation == nuiVertical)
  {
    nglFontInfo info;
    pFont->GetInfo(info);
    // Handle new line control char
    if (c == _T('\n'))
    {
      if (mGlyphs.size() > 0)
        mPenY = mGlyphs[0].Y; // Go back to layout Y origin
      mPenX += info.AdvanceMaxW*2;
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
        nglFontInfo info;
        pFont->GetInfo(info);
        mPenY += mDownAxis * info.AdvanceMaxH;
      }
      else if (c == _T('\t'))
      {
        nglFontInfo info;
        pFont->GetInfo(info);
        mPenX += info.AdvanceMaxW * mSpacesPerTab;
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
        mPenX += pGlyph->AdvanceX;
        mPenY += pGlyph->AdvanceY;
      }
    }
    else // Using Wrapper
    {
      if (!mpCurrentWord)
        mpCurrentWord = new Word;

      WordElement element;
      element.mChar = c;
      element.mGlyph = *pGlyph;
      element.mPos = Pos;
      element.mpFont = pFont;

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
  nuiFontLayout::GlyphList::const_iterator it = rGlyphs.begin();
  nuiFontLayout::GlyphList::const_iterator end = rGlyphs.end();

  nuiRect r;

  nglFontInfo finfo;
  mFont.GetInfo(finfo);

  while (it != end)
  {
    const nglGlyphLayout& rGlyph(*it);
    nglGlyphInfo info;
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

    nglFontInfo info;
    mFont.GetInfo(info);
    
    std::list<Word*>::iterator it = mWords.begin();
    std::list<Word*>::iterator end = mWords.end();


    //NGL_OUT(_T("\n\nFinalize with wrap at %f\n"), mWrapX);
    while (it != end)
    {
      Word* pWord = *it;

      //NGL_OUT(_T("New Word %f %f\n"), mPenX, mPenY);

      nglChar c = pWord->front().mChar;
      nglGlyphInfo* pGlyph = &pWord->front().mGlyph;
      // Handle new line control char
      if (c == _T('\n'))
      {
        //NGL_OUT(_T("EOL\n"));
        if (mGlyphs.size() > 0)
          mPenX = mGlyphs[0].X; // Go back to layout X origin
        mPenY += mDownAxis * info.AdvanceMaxH;
        mGlyphPrev = 0;
        mpFontPrev = NULL;
      }
      else if (c == _T('\t'))
      {
        //NGL_OUT(_T("TAB\n"));
        if (mPenX > 0.0f)
          mPenX += info.AdvanceMaxW * mSpacesPerTab;
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

        int prev = mGlyphPrev;
        while (cit != cend)
        {
          WordElement& rElement(*cit);
          c = rElement.mChar;
          pGlyph = &rElement.mGlyph;
          
          nglFontBase* pFont = rElement.mpFont;

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
            nglFontBase* pFont = rElement.mpFont;
            
            float kx = 0, ky = 0;
            if (mGlyphPrev && pFont && mpFontPrev == pFont && GetKerning(pFont, pGlyph->Index, kx, ky))
            {
              mPenX += kx;
              mPenY += ky;
            }

            // Add this glyph to the layout with the current position pen position
            AddGlyph(pFont, mPenX, mPenY, Pos, pGlyph);
            //AddGlyph(X, Y, Pos, pGlyph);

            // Proceed with glyph advance
            mPenX += pGlyph->AdvanceX;
            mPenY += pGlyph->AdvanceY;

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

void nuiFontLayout::SetSpacesPerTab(uint count)
{
  mSpacesPerTab = count;
}

uint nuiFontLayout::GetSpacesPerTab()
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

  nglFontInfo finfo;
  mFont.GetInfo(finfo);

  while (it != end)
  {
    const nglGlyphLayout& rGlyph(*it);
    nglGlyphInfo info;
    rGlyph.mpFont->GetGlyphInfo(info, rGlyph.Index, nuiFontBase::eGlyphBitmap);
    nuiSize w = info.AdvanceX;
//    nuiSize h = finfo.AdvanceMaxH;
    nuiSize x = rGlyph.X + info.BearingX;
    nuiSize y = rGlyph.Y;
    nuiSize h = finfo.Height;

    nuiRect rr(r);
    r.Union(rr, nuiRect(x, y, w, h));

    ++it;
  }

  return r.Size();
}

nglFontBase* nuiFontLayout::FindFontForMissingGlyph(nglFontBase* pOriginalFont, nglChar Glyph)
{
  //NGL_OUT(_T("Try to find font for char 0x%x (%c)\n"), Glyph, Glyph);
  
  //First look for the glyph in the fonts we already have:
  for (size_t i = 0; i < mpSubstitutionFonts.size(); i++)
  {
    uint index = 0;
    if (mpSubstitutionFonts[i]->GetGlyphIndexes(&Glyph, 1, &index, 1) > 0 && index)
    {
      //NGL_OUT(_T("found in previous fonts '%ls' as glyph 0x%x OK\n"), mpSubstitutionFonts[i]->GetFamilyName().GetChars(), index);
      return mpSubstitutionFonts[i];
    }
  }
  
  // try to find a font with the glyph that is similar enough:
  nuiFontManager& rFontManager(nuiFontManager::GetManager());
  nuiFontRequest request;
  request.SetName(pOriginalFont->GetFamilyName(), 3.0f);
  request.SetStyle(pOriginalFont->GetStyleName(), 1.5f);
  request.SetItalic(pOriginalFont->IsItalic(), 1.0f);
  request.SetBold(pOriginalFont->IsBold(), 1.0f);
  if (pOriginalFont->IsScalable())
    request.SetScalable(10.f, true);
  if (pOriginalFont->IsMonospace())
    request.SetMonospace(1.0f);
  request.MustHaveSize(pOriginalFont->GetSize(), 1.0f);

  request.MustHaveGlyph(Glyph, 1.0f, true);
  
  std::list<nuiFontRequestResult> fonts;
  rFontManager.RequestFont(request, fonts);
  
  if (!fonts.empty())
  {
    const nuiFontRequestResult& rResult(fonts.front());
        
    nuiFont* pFont = nuiFont::GetFont(rResult.GetPath(), pOriginalFont->GetSize(), rResult.GetFace());
    
    uint index = 0;
    if (pFont->GetGlyphIndexes(&Glyph, 1, &index, 1) > 0)
    {
      //NGL_OUT(_T("Found font '%ls' with glyph index %d\n"), pFont->GetFamilyName().GetChars(), index);
      
      NGL_ASSERT(rResult.GetFontDesc()->HasGlyph(Glyph));
      
      mpSubstitutionFonts.push_back(pFont);
      
      return pFont;
    }
  }
  
  return NULL; // We don't have a valid font for substitution...
}


/////////////////////////////////////////////
nuiFontBase::nuiFontBase()
: nglFontBase()
{
  Defaults();
}

nuiFontBase::nuiFontBase(uint8* pBuffer, uint BufferSize, uint Face, bool StaticBuffer)
: nglFontBase(pBuffer, BufferSize, Face, StaticBuffer)
{
  Defaults();
}

nuiFontBase::nuiFontBase (const nglPath& rFontFile, uint Face)
: nglFontBase (rFontFile, Face)
{
  Defaults();
}

nuiFontBase::nuiFontBase (const nuiFontBase& rFont)
: nglFontBase (rFont)
{
  mAlphaTest = rFont.mAlphaTest;
}

nuiFontBase::~nuiFontBase()
{
  Textures::iterator it;
  Textures::iterator end = mTextures.end();
  for(it = mTextures.begin(); it != end; ++it)
    (*it)->Release();
  mTextures.clear();
  mGlyphLocationLookupTable.clear();
}

void nuiFontBase::Defaults()
{
  SetAlphaTest();

  mCurrentX = 0;
  mCurrentY = 0;
  mCurrentTexture = 0;

  mRowMaxWidth = 0;

  nuiTexture *texture = AllocateTexture(nuiFontBase::TEXTURE_SIZE);
  mTextures.push_back(texture);
}

nuiTexture *nuiFontBase::AllocateTexture(uint size)
{
  nglImageInfo ImageInfo(false);
  ImageInfo.mBufferFormat = eImageFormatRaw;
  ImageInfo.mPixelFormat = eImagePixelAlpha;
  nglFontInfo FontInfo;
  GetInfo(FontInfo);
  ImageInfo.mWidth = size;
  ImageInfo.mHeight = size;
  ImageInfo.mBitDepth = 8;
  ImageInfo.mBytesPerPixel = 1;
  ImageInfo.mBytesPerLine = ImageInfo.mWidth * ImageInfo.mBytesPerPixel;
  ImageInfo.mpBuffer = NULL;

  ImageInfo.AllocateBuffer();

  nuiTexture *pTexture = nuiTexture::GetTexture(ImageInfo, true);

  pTexture->SetEnvMode(GL_BLEND);
  pTexture->SetMinFilter(GL_NEAREST);
  pTexture->SetMagFilter(GL_NEAREST);
//  pTexture->SetMinFilter(GL_LINEAR);
//  pTexture->SetMagFilter(GL_LINEAR);
  pTexture->SetWrapS(GL_CLAMP);
  pTexture->SetWrapT(GL_CLAMP);

  return pTexture;
}

void nuiFontBase::Blit8BitsBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY)
{
  nglImage *Image = pTexture->GetImage();
  char *pBuffer = Image->GetBuffer();
  uint32 x,y;

  uint32 w = pTexture->GetWidth();
  
  for (y = 0; y < rBitmap.Height; y++)
  {
    uint32 DstOffset = OffsetX + (y + OffsetY )* w;
    uint32 SrcOffset = y * rBitmap.Pitch;

    uint8* pSrc = rBitmap.pData + SrcOffset;
    uint8* pDst = (uint8*)pBuffer + DstOffset;

    for (x = 0; x < rBitmap.Width; x++) 
    {
      *pDst++ = *pSrc++;
    }
  }
  pTexture->ForceReload();
}

bool nuiFontBase::CopyBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY)
{

  switch (rBitmap.Depth)
  {
  case 1:
    {
      /* 
      * We can't load it as a bitmap texture because it will be used as an alpha map.
      * Convert every bit to a full byte into a temporary buffer.
      */
      GlyphBitmap bmp8 = rBitmap;
      bmp8.Pitch = (rBitmap.Width + 7) & ~7; // row length in bytes, 8-bytes aligned
      bmp8.pData = new uint8[bmp8.Pitch * rBitmap.Height];

      if (GetBitmap8(rBitmap, bmp8))
      {
        Blit8BitsBitmapToTexture(bmp8, pTexture, OffsetX, OffsetY);
      }
      delete bmp8.pData;
    }
    break;

  case 8:
    {
      Blit8BitsBitmapToTexture(rBitmap, pTexture, OffsetX, OffsetY);
    }
    break;

  default:
    {
      NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("nglFont: unsupported input bitmap pixel depth (%d)"), rBitmap.Depth); )
        return false;
    }
  }
  return true;
}

bool nuiFontBase::FindNextGlyphLocation(uint Width, uint Height, uint &rOffsetX, uint &rOffsetY)
{
  if (Width < (TEXTURE_SIZE - rOffsetX) )
  {
    if (Height < (TEXTURE_SIZE - rOffsetY) )
    {
      // there is enough space available
      mRowMaxWidth = max(mRowMaxWidth, Width);
      return true; 
    }
    rOffsetX += mRowMaxWidth;
    rOffsetY = 0;     
    if (Width < (TEXTURE_SIZE - rOffsetX) )
    {
      if (Height < (TEXTURE_SIZE - rOffsetY) )
      {
        mRowMaxWidth = Width;
        return true;
      }
      else // should move to another texture
      {
        return false;
      }
    }
  }
  return false; // Move to another texture, this on is full...
}

uint32 POT(uint32 i)
{
  uint32 t = 1;
  while (t < i)
  {
    t *= 2;
  }
  
  return t;
}

void nuiFontBase::AddCacheGlyph(uint Index, nuiFontBase::GlyphLocation &rGlyphLocation)
{
  // Fetch rendered glyph
  GlyphHandle glyph = GetGlyph(Index, eGlyphBitmap);
  GlyphBitmap bmp;
  GetGlyphBitmap(glyph, bmp);

  uint OffsetX = mCurrentX;
  uint OffsetY = mCurrentY;

  if (FindNextGlyphLocation(bmp.Width, bmp.Height, OffsetX, OffsetY)) 
  {
    nuiTexture *pCurrentTexture = mTextures[mCurrentTexture];

    rGlyphLocation = GlyphLocation (OffsetX, OffsetY, bmp.Width, bmp.Height, mCurrentTexture);
    mGlyphLocationLookupTable.insert(std::pair<uint, nuiFontBase::GlyphLocation>(Index, rGlyphLocation));

    CopyBitmapToTexture(bmp, pCurrentTexture, OffsetX, OffsetY);

    mCurrentX = OffsetX;
    mCurrentY = OffsetY + bmp.Height;
  }
  else
  {
    mCurrentTexture++;
    nuiTexture *texture = AllocateTexture(MAX(TEXTURE_SIZE, POT(MAX(bmp.Width, bmp.Height))));
    mTextures.push_back(texture);

    rGlyphLocation = GlyphLocation (0, 0, bmp.Width, bmp.Height, mCurrentTexture);
    mGlyphLocationLookupTable.insert(std::pair<uint, nuiFontBase::GlyphLocation>(Index, rGlyphLocation));

    CopyBitmapToTexture(bmp, texture, 0, 0);

    mCurrentX = 0;
    mCurrentY = bmp.Height;
    mRowMaxWidth = bmp.Width;

  }
}

void nuiFontBase::GetCacheGlyph(uint Index, nuiFontBase::GlyphLocation &rGlyphLocation)
{
  GlyphLocationLookupTableIterator iterator = mGlyphLocationLookupTable.find(Index);
  if (iterator == mGlyphLocationLookupTable.end())
  {
    AddCacheGlyph(Index, rGlyphLocation); 
  }
  else
  {
    rGlyphLocation = iterator->second;
  }
}

void nuiFontBase::SetAlphaTest (float Threshold)
{
  mAlphaTest = Threshold;
}

int nuiFontBase::Print (nuiDrawContext *pContext, float X, float Y, const nglString& rText)
{
  nuiFontLayout layout(*this);

  layout.Layout(rText);

  nglFontInfo info;
  GetInfo(info);

  int c = 0;
  int lines = 0;
  while (c >= 0)
  {
    c = rText.Find('\n',c);
    if (c!=-1)
      c++;
    lines++;
  }

  Y -= info.Height * (lines-1);
  return Print(pContext, X, Y, layout);
}

int nuiFontBase::Print (nuiDrawContext *pContext, float X, float Y, const nuiFontLayout& rLayout)
{
  uint todo = rLayout.GetGlyphCount();
  if (!todo)
    return 0;
  uint i;
  int done = 0;

  bool blendsaved = pContext->GetState().mBlending;
  bool texturesaved = pContext->GetState().mTexturing;

  pContext->EnableBlending(true);
  pContext->EnableTexturing(true);

  nuiColor SavedColor = pContext->GetFillColor();
  pContext->SetFillColor(pContext->GetTextColor());
  pContext->SetBlendFunc(nuiBlendTransp);

  std::map<nuiTexture*, std::vector<nuiGlyphLayout> > Glyphs;
  for (i = 0; i < todo; i++)
  {
    // Fetch i-th glyph in layout
    const nglGlyphLayout* pglyph = rLayout.GetGlyph(i);
    if (!pglyph)
      break;

    nuiGlyphLayout glyph;

    glyph.X     = pglyph->X + X;
    glyph.Y     = pglyph->Y + Y;
    glyph.Pos   = pglyph->Pos;
    glyph.Index = pglyph->Index;

    if (((nuiFontBase*)pglyph->mpFont)->PrepareGlyph(pContext, glyph))
      done++;

    Glyphs[glyph.mpTexture].push_back(glyph);
  }

  PrintGlyphs(pContext, Glyphs);

  pContext->EnableBlending(blendsaved);
  pContext->EnableTexturing(texturesaved);

  pContext->SetFillColor(SavedColor);

  return done;
}


bool nuiFontBase::PrintGlyph (nuiDrawContext *pContext, const nglGlyphLayout& rGlyph)
{
  // Fetch rendered glyph
  GlyphHandle glyph = rGlyph.mpFont->GetGlyph(rGlyph.Index, eGlyphBitmap);

  // If we don't have this glyph, assert it has not been rendered
  if (!glyph)
    return false;

  GlyphBitmap bmp;
  if (!rGlyph.mpFont->GetGlyphBitmap(glyph, bmp))
    return false;

  nuiFontBase::GlyphLocation GlyphLocation;
  ((nuiFontBase*)rGlyph.mpFont)->GetCacheGlyph(rGlyph.Index, GlyphLocation);

  int w = GlyphLocation.mWidth;
  int h = GlyphLocation.mHeight;

  int x = (int)floorf(rGlyph.X + bmp.Left);
  int y = (int)floorf(rGlyph.Y - bmp.Top);

  nuiTexture *texture;
  texture = mTextures[GlyphLocation.mOffsetTexture];

  pContext->SetTexture(texture);

  nuiRect DestRect((nuiSize)x, (nuiSize)y, (nuiSize)w, (nuiSize)h);
  nuiRect SourceRect((nuiSize)GlyphLocation.mOffsetX, (nuiSize)GlyphLocation.mOffsetY, (nuiSize)w, (nuiSize)h);

  pContext->DrawImage(DestRect, SourceRect);

  return true;
}

bool nuiFontBase::PrepareGlyph (nuiDrawContext *pContext, nuiGlyphLayout& rGlyph)
{
  // Fetch rendered glyph
  GlyphHandle glyph = GetGlyph(rGlyph.Index, eGlyphBitmap);

  // If we don't have this glyph, assert it has not been rendered
  if (!glyph)
    return false;

  GlyphBitmap bmp;
  if (!GetGlyphBitmap(glyph, bmp))
    return false;

  nuiFontBase::GlyphLocation GlyphLocation;
  GetCacheGlyph(rGlyph.Index, GlyphLocation);

  int w = GlyphLocation.mWidth;
  int h = GlyphLocation.mHeight;

  int x = (int)floorf(rGlyph.X + bmp.Left);
  int y = (int)floorf(rGlyph.Y - bmp.Top);

  rGlyph.mpTexture = mTextures[GlyphLocation.mOffsetTexture];

  rGlyph.mDestRect.Set((nuiSize)x, (nuiSize)y, (nuiSize)w, (nuiSize)h);
  rGlyph.mSourceRect.Set((nuiSize)GlyphLocation.mOffsetX, (nuiSize)GlyphLocation.mOffsetY, (nuiSize)w, (nuiSize)h);

  return true;
}


int nuiFontBase::GetTextSize (float& X, float& Y, const nglChar* pText)
{

  nuiFontLayout Layout(*this);

  int done = Layout.Layout(pText);

  nglGlyphInfo GlyphInfo;
  Layout.GetMetrics(GlyphInfo);

  Y = MAX(GlyphInfo.Height, GlyphInfo.AdvanceY);
  X = MAX(GlyphInfo.Width,  GlyphInfo.AdvanceX);

  return done;

}

int nuiFontBase::GetTextPos (float x, const nglChar* pText)
{
  nuiFontLayout Layout(*this);

  int done =  Layout.Layout(pText);

  if (done == -1) 
    return -1;

  if (!done)
  {
    return 0;
  }

  nglGlyphInfo Info;
  Layout.GetMetrics(Info);
  
  if (!Layout.GetGlyphCount())
    return 0;

  if (x > Layout.GetGlyph(done-1)->X) 
  {
    return (done-1);
  }

  for (int i = 0; i < done; i++) 
  {
    const nglGlyphLayout * pGlyphLayout = Layout.GetGlyph(i);

    if (x <= (pGlyphLayout->X))
    {
      return i;
    }
  }
  return done;
}

const nglChar* nuiFontBase::OnError (uint& rError) const
{
  return FetchError(gpFontErrorTable, nglFontBase::OnError(rError), rError);
}

nuiFontBase::GlyphLocation::GlyphLocation()
{
}

nuiFontBase::GlyphLocation::GlyphLocation(uint OffsetX, uint OffsetY, uint Width, uint Height, uint OffsetTexture)
: mOffsetX(OffsetX), mOffsetY(OffsetY), mWidth(Width), mHeight(Height), mOffsetTexture(OffsetTexture)
{
}

nuiFontBase::GlyphLocation::~GlyphLocation()
{
}

bool nuiFontBase::PrintGlyphs(nuiDrawContext *pContext, const std::map<nuiTexture*, std::vector<nuiGlyphLayout> >& rGlyphs)
{
  std::map<nuiTexture*, std::vector<nuiGlyphLayout> >::const_iterator it = rGlyphs.begin();
  std::map<nuiTexture*, std::vector<nuiGlyphLayout> >::const_iterator end = rGlyphs.end();

  bool texturing = pContext->GetState().mTexturing;
  nuiTexture* pOldTexture = pContext->GetTexture();
  if (pOldTexture)
    pOldTexture->Acquire();

  pContext->EnableTexturing(true);

  while (it != end)
  {
    nuiTexture* pTexture = it->first;
    pContext->SetTexture(pTexture);
    uint size = (uint)it->second.size();
    uint i;

    if (1)
    {
      nuiRenderArray array(GL_TRIANGLES);
      array.EnableArray(nuiRenderArray::eVertex);
      array.EnableArray(nuiRenderArray::eTexCoord);
      for (i = 0; i < size; i++)
      {
        const nuiRect& rDest = it->second[i].mDestRect;
        const nuiRect& rSource = it->second[i].mSourceRect;

        nuiSize tx,ty,tw,th;

        tx = rSource.mLeft;
        ty = rSource.mTop;
        tw = rSource.mRight;
        th = rSource.mBottom;

        pTexture->ImageToTextureCoord(tx, ty);
        pTexture->ImageToTextureCoord(tw,th);

        ///////////////////////////////////////////
        array.SetVertex(rDest.mLeft, rDest.mTop);
        array.SetTexCoords(tx, ty);
        array.PushVertex();
        
        array.SetVertex(rDest.mRight, rDest.mTop);
        array.SetTexCoords(tw, ty);
        array.PushVertex();
        
        array.SetVertex(rDest.mRight, rDest.mBottom);
        array.SetTexCoords(tw, th);
        array.PushVertex();

        ///////////////////////////////////////////
        array.SetVertex(rDest.mLeft, rDest.mTop);
        array.SetTexCoords(tx, ty);
        array.PushVertex();

        array.SetVertex(rDest.mRight, rDest.mBottom);
        array.SetTexCoords(tw, th);
        array.PushVertex();

        array.SetVertex(rDest.mLeft, rDest.mBottom);
        array.SetTexCoords(tx, th);
        array.PushVertex();
      }

      pContext->DrawArray(array);
    }
    else
    {
      for (i = 0; i != size; i++)
      {
        pContext->DrawImage(it->second[i].mDestRect, it->second[i].mSourceRect);
      }
    }

    ++it;
  }

  pContext->EnableTexturing(texturing);
  pContext->SetTexture(pOldTexture);
  if (pOldTexture)
    pOldTexture->Release();

  return true;
}
