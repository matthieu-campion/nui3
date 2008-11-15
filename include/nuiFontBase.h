/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nglFontBase.h"
#include "nglFontLayout.h"
#include "nuiRect.h"

#include <vector>
#include <map>

class nglPath;
//class nglFont;
class nuiTexture;
class nuiDrawContext;
class nuiTexture;
class nuiFont;

class NUI_API nuiGlyphLayout : public nglGlyphLayout
{
public:
  nuiTexture* mpTexture;
  nuiRect mDestRect;
  nuiRect mSourceRect;
};

class NUI_API nuiFontLayout : public nglFontLayout
{
public:
  nuiFontLayout(nglFontBase& rFont, float PenX = 0.0f, float PenY = 0.0f, nuiOrientation Orientation = nuiHorizontal);
  virtual ~nuiFontLayout();

  virtual void OnGlyph(nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph);

  void SetSpacesPerTab(uint count);
  uint GetSpacesPerTab();

  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
  virtual void OnFinalizeLayout();

  void SetWrapX(nuiSize WrapX);
  nuiSize GetWrapX() const;

  void SetDensity(nuiSize X, nuiSize Y);
  nuiSize GetDensityX() const;
  nuiSize GetDensityY() const;

  nuiRect GetRect() const;
  nglFontBase* FindFontForMissingGlyph(nglFontBase* pOriginalFont, nglChar Glyph);
  
  struct Line
  {
    float mX;
    float mY;
    float mWidth;
  };
  
  const std::vector<Line>& GetLines() const;
protected:
  uint mSpacesPerTab;
  nuiOrientation mOrientation;
  nuiSize mWrapX;
  class NUI_API WordElement
  {
  public:
    WordElement();
    WordElement(const WordElement& rWordElement);

    nglGlyphInfo mGlyph;
    nglChar mChar;
    int mPos;
    nglFontBase* mpFont;
  };
  typedef std::list<WordElement> Word;
  std::list<Word*> mWords;
  Word* mpCurrentWord;
  void GetWordSize(nuiRect& rRect, const GlyphList& rGlyphs);
  
  std::vector<nuiFont*> mpSubstitutionFonts;

  std::vector<Line> mLines;
  bool mUnderline;
  bool mStrikeThrough;
  float mXDensity;
  float mYDensity;
};

class NUI_API nuiFontBase : public nglFontBase
{

public:
  nuiFontBase();
  nuiFontBase(uint8* pBuffer, uint BufferSize, uint Face, bool StaticBuffer);
  nuiFontBase(const nglPath& rFontFile, uint Face);
  nuiFontBase(const nuiFontBase& rFont);

  virtual ~nuiFontBase();

  void SetAlphaTest(float Threshold = 0.01f);


  int  Print (nuiDrawContext *pContext, float X, float Y, const nglString& rText);

  int  Print (nuiDrawContext *pContext, float X, float Y, const nuiFontLayout& rLayout);

  int  GetTextSize (float& X, float& Y, const nglChar* pText); ///< Calculate the bounding of the string in texels and returns it in X & Y.
  int  GetTextPos (float X, const nglChar* pText); ///< Calculate the bounding of the char pos in the given text where the pixel (X,?) lies and returns it.


protected:
  GLclampf mAlphaTest;  ///< Alpha test threshold as set by SetAlphaTest()
  const nglChar* OnError (uint& rError) const;

private:
  bool PrintGlyph (nuiDrawContext *pContext, const nglGlyphLayout& rGlyph);
  bool PrintGlyphs(nuiDrawContext *pContext, const std::map<nuiTexture*, std::vector<nuiGlyphLayout> >& rGlyphs);
  bool PrepareGlyph(nuiDrawContext *pContext, nuiGlyphLayout& rGlyph);
  class NUI_API GlyphLocation
  {
  public:
    GlyphLocation();
    GlyphLocation(uint OffsetX, uint OffsetY, uint Width, uint Height, uint OffsetTexture);
    ~GlyphLocation();

  public:
    uint mOffsetX, mOffsetY, mWidth, mHeight, mOffsetTexture;
  };

  typedef std::map<uint, GlyphLocation> GlyphLocationLookupTable;
  typedef std::map<uint, GlyphLocation>::iterator GlyphLocationLookupTableIterator;
  typedef std::vector<nuiTexture *> Textures;

  GlyphLocationLookupTable mGlyphLocationLookupTable;
  Textures mTextures;

  uint mCurrentX;
  uint mRowMaxWidth;
  uint mCurrentY;
  uint mCurrentTexture;

private:

  nuiTexture *AllocateTexture(uint size);
  bool CopyBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY);

  void Blit8BitsBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY);

  void GetCacheGlyph(uint Index, nuiFontBase::GlyphLocation &rGlyphLocation);
  void AddCacheGlyph(uint Index, nuiFontBase::GlyphLocation &rGlyphLocation);
  bool FindNextGlyphLocation(uint Width, uint Height, uint &rOffsetX, uint &rOffsetY);
  void Defaults();
   
  static const unsigned int TEXTURE_SIZE = 256;

};

