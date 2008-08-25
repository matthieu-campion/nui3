/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglKernel.h"
#include "nglMath.h"
#include "nglFont.h"
#include "nglFontLayout.h"
#include "nglFontTexCache.h"

using namespace std;


const nglChar* gpFontErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("Something bad happened"),
          NULL
};


/*
 * Life cycle
 */

nglFont::nglFont()
 : nglFontBase()
{
  Defaults();
}

nglFont::nglFont (uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer)
 : nglFontBase (pBuffer, BufferSize, Face, StaticBuffer)
{
  Defaults();
}

nglFont::nglFont (const nglPath& rFontFile, uint Face)
 : nglFontBase (rFontFile, Face)
{
  Defaults();
}

nglFont::nglFont (const nglFont& rFont)
 : nglFontBase (rFont)
{
  // Don't share the cache
  mpCache = NULL;

  mScaleX    = rFont.mScaleX;
  mScaleY    = rFont.mScaleY;
  mColor[0]  = rFont.mColor[0];
  mColor[1]  = rFont.mColor[1];
  mColor[2]  = rFont.mColor[2];
  mColor[3]  = rFont.mColor[3];
  mAlphaTest = rFont.mAlphaTest;
}

nglFont::~nglFont()
{
  CacheRelease();
}

void nglFont::Defaults()
{
  mpCache = NULL;

  SetScale(1.0f, 1.0f);
  SetColor(1.0f, 1.0f, 1.0f);
  SetAlphaTest();
}


/*
 * Settings
 */

void nglFont::SetColor (float R, float G, float B, float A)
{
  mColor[0] = R;
  mColor[1] = G;
  mColor[2] = B;
  mColor[3] = A;
}

void nglFont::SetScale (float X, float Y)
{
  mScaleX = X;
  mScaleY = Y;
  if (mpCache)
    mpCache->SetScale(X, Y);
}

void nglFont::SetAlphaTest (float Threshold)
{
  mAlphaTest = Threshold;
}


/*
 * Rendering
 */

void nglFont::PushContext()
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TEXTURE_BIT);
  if (mpCache)
    mpCache->PushContext();
}

void nglFont::PopContext()
{
  if (mpCache)
    mpCache->PopContext();
  glPopAttrib();
}

void nglFont::SetContext()
{
  glEnable(GL_TEXTURE_2D);
 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  if (mAlphaTest > .0f)
  {
    glAlphaFunc(GL_GEQUAL, mAlphaTest);
    glEnable(GL_ALPHA_TEST);
  }
  else
    glDisable(GL_ALPHA_TEST);

  glColor4fv (mColor);

  if (mpCache)
  mpCache->SetContext();
}

int nglFont::Print (float X, float Y, const nglString& rText, bool AutoContext)
{
  nglFontLayout layout(*this); // Use default layout

  layout.Layout(rText);

  return Print(X, Y, layout, AutoContext);
}

int nglFont::Print (float X, float Y, const nglFontLayout& rLayout, bool AutoContext)
{
  uint todo = rLayout.GetGlyphCount();
  if (!todo)
    return 0;

  if (AutoContext)
  {
    PushContext();
    SetContext();
  }

  uint i;
  int done = 0;

  for (i = 0; i < todo; i++)
  {
    // Fetch i-th glyph in layout
    const nglGlyphLayout* pglyph = rLayout.GetGlyph(i);
    if (!pglyph)
      break;

    nglGlyphLayout glyph;

    glyph.X     = pglyph->X + X;
    glyph.Y     = pglyph->Y + Y;
    glyph.Pos   = pglyph->Pos;
    glyph.Index = pglyph->Index;

    if (OnPrintGlyph(glyph))
      done++;
  }

  if (AutoContext)
    PopContext();

  return done;
}

bool nglFont::BitmapToTexture (const GlyphBitmap& rBitmap, GLint OffsetX, GLint OffsetY) const
{
  if (!rBitmap.pData)
    return false;

  GLsizei tex_w = rBitmap.Width;
  GLsizei tex_h = rBitmap.Height;
  if (!tex_w || !tex_h)
    return false;

  switch (rBitmap.Depth)
  {
    case 1:
    {
      /* We can't load it as a bitmap texture because it will be used as an alpha map.
       * Convert every bit to a full byte into a temporary buffer.
       */
      GlyphBitmap bmp8 = rBitmap;
      bmp8.Pitch = (tex_w + 7) & ~7; // row length in bytes, 8-bytes aligned
      bmp8.pData = (uint8*) alloca(bmp8.Pitch * tex_h);

      if (GetBitmap8(rBitmap, bmp8))
      {
        glPixelStorei(GL_UNPACK_ROW_LENGTH, bmp8.Pitch);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        glTexSubImage2D(GL_TEXTURE_2D, 0, OffsetX, OffsetY, tex_w, tex_h, GL_ALPHA, GL_UNSIGNED_BYTE, bmp8.pData);
      }
    }
    break;

    case 8:
    {
      glPixelStorei(GL_UNPACK_ROW_LENGTH, rBitmap.Pitch);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glTexSubImage2D(GL_TEXTURE_2D, 0, OffsetX, OffsetY, tex_w, tex_h, GL_ALPHA, GL_UNSIGNED_BYTE, rBitmap.pData);
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


/*
 * Rendering callbacks
 */

bool nglFont::OnPrintGlyph (const nglGlyphLayout& rGlyph)
{
  /* First try to render through cache
   */
  if (mpCache && mpCache->Draw(rGlyph))
    return true;

  /* Then fallback on default method
   */
  return OnPrintGlyphDefault(rGlyph);
}

/* OnPrintGlyphDefault
 *
 * Default displayer handler, terribly inefficient since it does not
 * use a texture object. This is used as a fallback (when no cache is
 * present). The glTextureBind() appart, it is OpenGL 1.0 compliant.
 */
bool nglFont::OnPrintGlyphDefault (const nglGlyphLayout& rGlyph)
{
  // Fetch rendered glyph
  GlyphHandle glyph = GetGlyph(rGlyph.Index, eGlyphBitmap);

  // If we don't have this glyph, assert it has not been rendered
  if (!glyph)
    return false;

  GlyphBitmap bmp;
  if (!GetGlyphBitmap(glyph, bmp))
    return false;

  // GL wants textures with power of 2 as dimensions, and a minimum of 64x64
  GLint tex_w = 1 << Log2Ceil(bmp.Width);
  GLint tex_h = 1 << Log2Ceil(bmp.Height);
  tex_w = MAX(64, tex_w);
  tex_h = MAX(64, tex_h);

  // Drop current texture binding (0 is the default target)
  glBindTexture(GL_TEXTURE_2D, 0);

  // Setup unfiltered and clamped 2D texturing
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // Allocate texture (it will be filled via means of glTexSubImage2D later)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, tex_w, tex_h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);

  // Upload bitmap to texture
  if (!BitmapToTexture(bmp, 0, 0))
    return  false;

  /* We'll draw a w*h pixels quad at x,y, and make sure mapping
   * coordinates sample only the needed portion of the texture (u,v).
   */
  float w = (float)bmp.Width;
  float h = (float)bmp.Height;
  float x = rGlyph.X + mScaleX * (float)bmp.Left;
  float y = rGlyph.Y + mScaleY * ((float)bmp.Top - h);
  float u = w / (float)tex_w;
  float v = h / (float)tex_h;
  h *= mScaleY;

  /* Here we assume the current projection maps 1 texel on 1 pixel,
   * and we round the quad origin to integer values to guarantee a
   * proper texel=pixel blitting.
   */
  x = floorf(x);
  y = floorf(y);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, v);    glVertex2f(x,   y);
  glTexCoord2f(u,    v);    glVertex2f(x+w, y);
  glTexCoord2f(u,    0.0f); glVertex2f(x+w, y+h);
  glTexCoord2f(0.0f, 0.0f); glVertex2f(x,   y+h);
  glEnd();

  return true;
}


/*
 * Cache
 */

bool nglFont::UseCache (int Hint)
{
  /* The hint of nglFontTexCache is the miminum number of glyph
   * fitting the cache. We provide an arbitrary default of 128
   */
  nglFontCache* cache = new nglFontTexCache(Hint > 0 ? Hint : 128);

  return cache && CacheInit(cache);
}

bool nglFont::SetCache (nglFontCache* pCache)
{
  return CacheInit(pCache);
}

bool nglFont::CacheInit (nglFontCache* pCache)
{
  CacheRelease();

  mpCache = pCache;
  if (!mpCache)
    return true; // User means to turn off caching, this is OK

  if (mpCache->Init(this))
  {
    mpCache->SetScale(mScaleX, mScaleY);
    return true;
  }

  return false;
}

void nglFont::CacheRelease()
{
  if (mpCache)
  {
    delete mpCache;
    mpCache = NULL;
  }
}


/*
 * Error callback
 */

const nglChar* nglFont::OnError (uint& rError) const
{
  return FetchError(gpFontErrorTable, nglFontBase::OnError(rError), rError);
}

#endif // HAVE_FREETYPE
