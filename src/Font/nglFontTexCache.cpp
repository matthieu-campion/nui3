/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include NGL_CONFIG_H

#ifdef HAVE_FREETYPE

#include "nglKernel.h"
#include "nglFontTexCache.h"
#include "nglFontLayout.h"
#include "nglMath.h"

using namespace std;


/*
 * nglFontTexCache
 */

nglFontTexCache::nglFontTexCache (int GlyphCnt)
{
  mGlyphCntHint = GlyphCnt;
  mpFont = NULL;
  mGlyphCnt = 0;
  mTexture = 0;
}

nglFontTexCache::~nglFontTexCache()
{
  uint i;

  for (i = 0; i < mGlyphLUT.size(); i++)
  {
    Glyph* glyph = mGlyphLUT[i];

    if (glyph)
      delete glyph;
  }
  mGlyphLUT.clear();

  if (mTexture)
    glDeleteTextures (1, &mTexture);
}


bool nglFontTexCache::Init (const nglFont* pFont)
{
  mpFont = pFont;
  mGlyphCnt = 0;

  NGL_LOG(_T("font"), NGL_LOG_INFO, _T("nglFontTexCache: Init: trying to fit %d glyphs"), mGlyphCntHint);

  /* Find font's maximum bounding box (in pixels)
   */
  nglFontInfo info;
  if (!mpFont->GetInfo(info, eFontUnitPixel))
    return false;

  uint bbox_w, bbox_h;
  if (info.IsScalable)
  {
    // Only scalable provide their (precomputed) global bouding box
    bbox_w = (uint)ceilf(info.BBoxMaxX - info.BBoxMinY);
    bbox_h = (uint)ceilf(info.BBoxMaxY - info.BBoxMinY);
  }
  else
  {
    // Let's cheat for bitmap fonts : pretend height * height will do
    bbox_w = bbox_h = (uint)ceilf(info.Height);
  }
  NGL_LOG(_T("font"), NGL_LOG_INFO+1, _T("  font bbox is %dx%d"), bbox_w, bbox_h);

  /* Try yo find out optimal texture height
   */
  GLint tex_max;
  glGetIntegerv (GL_MAX_TEXTURE_SIZE, &tex_max);

  uint min = 6;
  uint max = Log2Floor(tex_max);
  NGL_LOG(_T("font"), NGL_LOG_INFO+2, _T("  tex range : %d..%d texels"), 1<<min, 1<<max);

  uint tile_w = 0, tile_h = 0, glyph_cnt = 0;
  uint i, j;
  NGL_LOG(_T("font"), NGL_LOG_INFO+2, _T("  matching tiles :"));

  /* i = log2(texture size in texels)
   */
  for (i = 2*min; i <= 2*max; i++)
  {
    float waste = 1.1f;
    bool fit = false;
    /* We search all decompositions with the form 2^j * 2^(i-j) [= 2^i]
     * j = [min..max]
     *   = eliminate symetric solutions (A*B = B*A) with j <= min+(i-2*min)/max
     *   = also prevent cases where 2^i * 2^(i-j), i-j > max
     */
    for (j = (i-min <= max) ? min : (i-max); j <= min + (i-2*min)/2; j++)
    {
      // Evaluate a w*h texture : how many glyph's bboxes can we fit in ?
      uint w = 1<<j, h = 1<<(i-j);

      int cnt = (w / bbox_w) * (h / bbox_h);
      if (cnt < mGlyphCntHint) continue;

      // Check if GL is ready to allocate such a texture
      GLsizei allocated;
      glTexImage2D(GL_PROXY_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
      glGetTexLevelParameteriv(GL_PROXY_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &allocated);
      if (allocated == 0) continue;

      float wx = (float)(w % bbox_w);
      float wy = (float)(h % bbox_h);
      float wtex = ((float)w - wx)*wy + (float)h*wx;
      float r = wtex / (float)(1<<i);
      NGL_LOG(_T("font"), NGL_LOG_INFO+2, _T("   %5d KB : %4dx%4d, %4d glyphs, waste %3.0f KTexels (%.1f%%)"), (1<<i)>>10, w, h, cnt, wtex/1024.0f, r*100.0f);

      fit = true;
      if (r < waste)
      {
        // Keep the less wasteful
        tile_w = w;
        tile_h = h;
        glyph_cnt = cnt;
        waste = r;
      }
    }
    if (fit) break;
  }
  if (tile_w == 0)
  {
    NGL_LOG(_T("font"), NGL_LOG_INFO+1, _T("  no match"));
    return false;
  }
  NGL_LOG(_T("font"), NGL_LOG_INFO+1, _T("  selecting %dx%d (fitting %d glyphs)"), tile_w, tile_h, glyph_cnt);

  /* Real job begins here !
   */
  mGlyphMax = glyph_cnt;
  mTexW   = tile_w;
  mTexH   = tile_h;
  mGlyphW = bbox_w;
  mGlyphH = bbox_h;
  mGlyphU = (GLfloat)mGlyphW / (GLfloat)mTexW;
  mGlyphV = (GLfloat)mGlyphH / (GLfloat)mTexH;

  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_ALPHA, mTexW, mTexH, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR)
    NGL_LOG(_T("font"), NGL_LOG_INFO+1, _T("  glTexImage2D: failed (GL_ERR: %d)"), err);

  return (err == GL_NO_ERROR);
}

bool nglFontTexCache::Draw (const nglGlyphLayout& rGlyph)
{
  Glyph* glyph = GetGlyph(rGlyph.Index);
  if (!glyph)
    return false;

  mModelView.Elt.M14 = mX0 + rGlyph.X + mScaleX * glyph->mBearingX;
  mModelView.Elt.M24 = mY0 + rGlyph.Y - mScaleY * glyph->mBearingY;
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(mModelView.Array);

  return glyph->Draw();
}

void nglFontTexCache::PushContext()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  glGetFloatv(GL_MODELVIEW_MATRIX, mModelView.Array);
  mX0 = mModelView.Elt.M14;
  mY0 = mModelView.Elt.M24;
}

void nglFontTexCache::PopContext()
{
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void nglFontTexCache::SetContext()
{
}


/* GetGlyph
 *
 * Search the glyph in the LUT :
 *  - if found, return it
 *  - else see if there's room to fit a new one
 *    - if ok, build the new Glyph object, insert in LUT and in texture, then return it
 *    - otherwise bail out
 * Returns NULL if cache full or other oddity happened
 */
nglFontTexCache::Glyph* nglFontTexCache::GetGlyph (uint Index)
{
  Glyph* glyph = LUTLookup(Index);

  if (glyph)
  {
    glBindTexture(GL_TEXTURE_2D, mTexture);
    return glyph; // Cache hit
  }

  // Glyph not in cache, check for room to insert one
  if (mGlyphCnt >= mGlyphMax)
    return NULL;

  // Render offscreen glyph
  nglFontBase::GlyphHandle hglyph = mpFont->GetGlyph(Index, nglFontBase::eGlyphBitmap);
  if (!hglyph)
    return NULL;

  nglFontBase::GlyphBitmap bmp;
  if (!mpFont->GetGlyphBitmap(hglyph, bmp))
    return NULL;

  // Compute location of glyph in texture
  GLint gpl = mTexW / mGlyphW;  // glyph per line
  GLint x_pos = (mGlyphCnt % gpl) * mGlyphW;
  GLint y_pos = (mGlyphCnt / gpl) * mGlyphH;

  // Insert into texture
  glBindTexture(GL_TEXTURE_2D, mTexture);
  if (!mpFont->BitmapToTexture(bmp, x_pos, y_pos))
    return NULL;

  // Build Glyph object
  glyph = new Glyph(bmp, x_pos, y_pos, mTexW, mTexH);

  // Insert in LUT
  if (!LUTInsert(Index, glyph))
  {
    NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("nglFontTexCache::GetGlyph: LUTInsert() failed"));
    delete glyph;
    return NULL;
  }
  mGlyphCnt++;

//  NGL_OUT(_T("nglFontTexCache::GetGlyph: LUTInsert(Index:%d [%d,%d]): size=%d\n"), Index, x_pos, y_pos, mGlyphCnt);
  // Hurra
  return glyph;
}

/* LUTInsert
 *
 * Insert pGlyph instance into the cache LUT. If the LUT already held
 * a glyph object for the same Index, the insertion fails.
 */
bool nglFontTexCache::LUTInsert (uint Index, Glyph* pGlyph)
{
  if (Index >= mGlyphLUT.size())
    mGlyphLUT.resize(Index + 1, NULL);
  else
  {
    Glyph* glyph = mGlyphLUT[Index];

    if (glyph)
      return false;
  }

  mGlyphLUT[Index] = pGlyph;
  return true;
}

/* LUTLookup
 *
 * Search for glyph object with Index reference in the cache LUT.
 * Returns NULL if not found.
 */
nglFontTexCache::Glyph* nglFontTexCache::LUTLookup (uint Index)
{
  Glyph* glyph;

  if (Index < mGlyphLUT.size() &&
      (glyph = mGlyphLUT[Index]) != NULL)
    return glyph;

  return NULL;
}



/*
 * Glyph
 */

nglFontTexCache::Glyph::Glyph (nglFontBase::GlyphBitmap& rBitmap, GLint X, GLint Y, GLint TexW, GLint TexH)
{
  mBearingX = (float)rBitmap.Left;
  mBearingY = (float)(rBitmap.Height - rBitmap.Top);
  mList = glGenLists(1);
  if (!mList)
  {
    NGL_DEBUG( NGL_LOG(_T("font"), NGL_LOG_WARNING, _T("nglFontTexCache::Glyph: could not create display list")); )
    return;
  }

  float tex_w = (float)TexW;
  float tex_h = (float)TexH;
  float w = (float)rBitmap.Width;
  float h = (float)rBitmap.Height;
  float u1 = (float)X / tex_w;
  float v1 = (float)Y / tex_h;
  float u2 = u1 + w / tex_w;
  float v2 = v1 + h / tex_h;

  glNewList(mList, GL_COMPILE);
  {
    glBegin(GL_QUADS);

    glTexCoord2f(u1, v1);
    glVertex2f(0.f, h);

    glTexCoord2f(u2, v1);
    glVertex2f(w, h);

    glTexCoord2f(u2, v2);
    glVertex2f(w, 0.f);

    glTexCoord2f(u1, v2);
    glVertex2f(0.f, 0.f);
 
    glEnd();
  }
  glEndList();
}

nglFontTexCache::Glyph::~Glyph()
{
  if (mList)
    glDeleteLists(mList, 1);
}

bool nglFontTexCache::Glyph::Draw () const
{
  if (!mList)
    return false;

  glCallList(mList);

  return true;
}

#endif // HAVE_FREETYPE
