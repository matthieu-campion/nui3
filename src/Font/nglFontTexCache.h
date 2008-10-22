/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglFontTexCache.h
\brief A simple texture based cache for nglFont (internal)
*/

#pragma once

//#include "nui.h"
#include "nglFont.h"
#include "nglMatrix.h"
#include "nglFontCache.h"

class nglFontTexCache : public nglFontCache
{
public:
  nglFontTexCache(int GlyphCnt);
  virtual ~nglFontTexCache();

  bool Init (const nglFont* pFont);
  bool Draw (const nglGlyphLayout& rGlyph);

  void PushContext();
  void PopContext();
  void SetContext();

private:
  class Glyph
  {
  public:
    Glyph(nglFontBase::GlyphBitmap& rBitmap, GLint X, GLint Y, GLint TexW, GLint TexH);
    ~Glyph();

    bool Draw() const;

  public:
    float  mBearingX;
    float  mBearingY;
    GLuint mList;
  };
  typedef std::vector<Glyph*> GlyphLUTType;

  GlyphLUTType    mGlyphLUT;        // glyph lookup table
  int             mGlyphCntHint;    // initial hint for cache init (from ctor)
  GLuint          mTexture;         // texture name
  GLint           mTexW, mTexH;     // texture size
  uint            mGlyphCnt;        // current number of glyphs in cache (<= mGlyphMax)
  uint            mGlyphMax;        // number of glyph that fits the cache
  uint            mGlyphW, mGlyphH; // glyph size (pixels)
  GLfloat         mGlyphU, mGlyphV; // glyph size (in texture coordinates relative to texture)
  nuiMatrix      mModelView;
  GLfloat         mX0, mY0;

  Glyph* GetGlyph  (uint Index);
  bool   LUTInsert (uint Index, Glyph* pGlyph);
  Glyph* LUTLookup (uint Index);
};

