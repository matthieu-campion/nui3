/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nuiFontLayout.h
\brief Text layouts (see nuiFontBase)
*/

#pragma once

#include "nui.h"
#include "nuiFontBase.h"

//! Describes a glyph item positioned in a layout
class NGL_API nuiGlyphLayout
{
public:
  float X;      ///< Abscissa in layout coordinate space
  float Y;      ///< Ordinate in layout coordinate space
  int32 Pos;    ///< Position of the source char in the original laid out string
  int32 Index;  ///< Glyph index in the current font if > 0, otherwise mpFont is not a font but a void* used to reference some custom data.
  nuiFontBase* mpFont; ///< Pointer to font used for the rendering the glyph
  nuiTexture* mpTexture;
  nuiRect mDestRect;
  nuiRect mSourceRect;
};

//! Layout context (see nuiFontBase)
/*!
A layout context manages the positions of a glyph collection. When Layout() is
invoked, the OnGlyph() callback is triggered for every glyph extracted from the
original user string (a plain text). This base class does a very simple layout
job as a default.

To implement a more sophisticated layout, create your own layout class as a
nuiFontLayout superclass, and override the OnGlyph() method. See OnGlyph()
documentation for more info.
*/
class NGL_API nuiFontLayout
{
public:
  /** @name Life cycle */
  //@{
  nuiFontLayout(nuiFontBase& rFont, float PenX = 0.0f, float PenY = 0.0f, nuiOrientation Orientation = nuiHorizontal);
  /*!< Initialize a layout context
    \param rFont source font: we try to use this font for all the glyphs. Missing glyphs can try to use font substituion call backs.
    \param PenX initial horizontal pen position
    \param PenY initial vertical pen position
    \param Orientation are the glyphs to be positionned horizontally (which is the default for most languages) or verticaly (possible for some languages like Chinese and Japanese)

    Kerning is turned on as a default, see UseKerning().
  */
  virtual ~nuiFontLayout();

  void Init (float PenX = 0.0f, float PenY = 0.0f);  ///< (re)initialize context
  //@}

  /** @name Metrics */
  //@{
  void UseKerning (bool Use = true);
  /*< Use kerning if available (turned on as a default)
   */
  int  GetMetrics (nuiGlyphInfo& rInfo) const;
  /*!< Returns whole layout metrics (as a composite glyph)
    \param rInfo metrics info holder
    \return number of glyphs in the layout
  */
  float GetAscender() const;
  float GetDescender() const;
  
  //@}

  /** @name Layout */
  //@{
  int          Layout (const nglString& rText, bool FinalizeLayout = true);
  /*!< 
    \param rText string to decompose
    \return number of glyphs effectively processed

    Extract a glyph collection from \p rText and build the layout. See OnGlyph().
  */
  virtual void OnGlyph (nuiFontBase* pFont, const nglString& rString, int Pos, nuiGlyphInfo* pGlyph);
  /*!< Add a glyph to the layout
    \param pFont font that contains the glyph to render (only useful when we need font substitution)
    \param rString current laid out string
    \param Pos position of the current char in the original laid out string
    \param pGlyph glyph object, might be null if the current char maps to no glyph

    This method inserts the given glyph into the current layout. It is a simple
    implementation which supports kerning and newline (\\n). Your own layout can
    be implemented by overloading this method. Here is a trivial example :

\code
void nuiFontLayout::Onglyph (const nglString& rString, int Pos, nuiGlyphInfo* pGlyph)
{
  if (!pGlyph) // No glyph available for this Char
    return;

  // Add this glyph to the layout with the current position pen position
  AddGlyph(mPenX, mPenY, Pos, pGlyph);

  // Proceed with glyph advance
  mPenX += pGlyph->AdvanceX;
  mPenY += pGlyph->AdvanceY;
}
\endcode

    Have a look at nuiFontLayout::OnGlyph() default implementation for a little
    more involved exemple (newline and kerning support).

    It might be possible that a single char decomposes to several glyphs, in
    this case OnGlyph() will be called once per output glyph, with the same
    \p Pos parameter.
  */
  virtual void OnFinalizeLayout();
  virtual nuiFontBase* FindFontForMissingGlyph(nuiFontBase* pOriginalFont, nglUChar Glyph);
  
  uint  GetGlyphCount() const;
  const nuiGlyphLayout* GetGlyph   (uint Offset) const;
  const nuiGlyphLayout* GetGlyphAt (float X, float Y) const;
  /*!< Identify a glyph at given coordinates
    \param X abscissa in layout coordinate space
    \param Y ordinate in layout coordinate space
    \return located glyph instance, NULL if none match

    This method searches if a glyph's bounding box in the layout encloses the
    given coordinates. If such a glyph is found, its localized instance is
    returned.
  */
  //@}

  void SetDownAxis(float UpDown);
  float GetDownAxis();

  bool AddDummyGlyph(int32 ReferencePos, void* pUserPointer, float W, float H);
  
  void SetSpacesPerTab(int count);
  int GetSpacesPerTab();
  
  void SetUnderline(bool set);
  bool GetUnderline() const;
  void SetStrikeThrough(bool set);
  bool GetStrikeThrough() const;
  
  void SetWrapX(nuiSize WrapX);
  nuiSize GetWrapX() const;
  
  void SetDensity(nuiSize X, nuiSize Y);
  nuiSize GetDensityX() const;
  nuiSize GetDensityY() const;
  
  nuiRect GetRect() const;
  
  struct Line
  {
    float mX;
    float mY;
    float mWidth;
  };
  
  typedef nuiFastDelegate2<int32 /* positions */, bool /* true if wrapped */, bool /*result*/>  NewLineDelegate;
  void SetNewLineDelegate(const NewLineDelegate& rDelegate);
  const std::vector<Line>& GetLines() const;
  
protected:
  typedef std::vector<nuiGlyphLayout> GlyphList;

  nuiFontBase&       mFont;        ///< Font source for glyphs
  float              mPenX;        ///< Current horizontal pen position
  float              mPenY;        ///< Current vertical pen position
  bool               mUseKerning;  ///< Use kerning in default layout
  uint               mGlyphPrev;   ///< Last laid out glyph (for kerning), 0 if none
  GlyphList          mGlyphs;      ///< List of glyphs
  nuiFontBase*       mpFontPrev;   ///< The font used to render the last glyph (mainly used to handle kerning)

  bool AddGlyph   (nuiFontBase* pFont, float X, float Y, int Pos, nuiGlyphInfo* pGlyph);
  /*!< Add a localized glyph to the layout
    \param X horizontal pen position
    \param Y vertical pen position
    \param Pos glyph position in original laid out string
    \param pGlyph glyph to insert, no insertion if NULL
    \return true if the glyph was succesfully appended to the layout

    Append a glyph to the current layout with pen position (\p X, \p Y).
    A nuiGlyphLayout instance is added at the end of the mGlyphs vector.
    Internally, the global layout bounding box is updated.

    The \p Pos parameter is used for reverse mapping in GetGlyphAt(),
    to recover from a glyph back to the source character in the original
    laid out string. Multiple glyphs can reverse map to the same character
    index.
  */
  bool GetKerning (nuiFontBase* pFont, uint Index, float& rX, float& rY);
  /*!< Fetch kerning with previous glyph
    \param pFont the font that contains the glyph
    \param Index current glyph (right glyph of the kerning pair)
    \param rX horizontal kerning value
    \param rY vertical kerning value
    \return true if both \p rX and \p rY are valid

    The left glyph is defined by mGlyphPrev and only valid when non zero
    (thus the OnGlyph() might prevent kerning with the next glyph by resetting
    mGlyphPrev).
  */

  float mDownAxis;

  float mAscender;
  float mDescender;

  int mSpacesPerTab;
  nuiOrientation mOrientation;
  nuiSize mWrapX;
  class NUI_API WordElement
  {
  public:
    WordElement(nuiGlyphInfo Glyph, nglUChar Char, int Pos, nuiFontBase* pFont);
    WordElement(const WordElement& rWordElement);
    
    nuiGlyphInfo mGlyph;
    nglUChar mChar;
    int mPos;
    nuiFontBase* mpFont;
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
  NewLineDelegate mNewLineDelegate;

private:
  float mXMin, mXMax;
  float mYMin, mYMax;
  
  nuiFontLayout(const nuiFontLayout& rLayout) : mFont(rLayout.mFont)
  {
  // Undefined copy constructor (untils somone proves the default copy ctor is safe)
  }

  void InitMetrics();
  void CallOnGlyph (nuiFontBase* pFont, const nglString& rString, int Pos, nuiGlyphInfo* pGlyph);
};


