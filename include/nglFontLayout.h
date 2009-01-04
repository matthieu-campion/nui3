/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglFontLayout.h
\brief Text layouts (see nglFontBase)
*/

#pragma once

//#include "nui.h"
#include <vector>
class nglFontBase;
class nglGlyphInfo;
class nglString;

//! Describes a glyph item positioned in a layout
class NGL_API nglGlyphLayout
{
public:
  float X;      ///< Abscissa in layout coordinate space
  float Y;      ///< Ordinate in layout coordinate space
  int32 Pos;    ///< Position of the source char in the original laid out string
  int32 Index;  ///< Glyph index in the current font if > 0, otherwise mpFont is not a font but a void* used to reference some custom data.
  nglFontBase* mpFont; ///< Pointer to font used for the rendering the glyph
};


//! Layout context (see nglFontBase)
/*!
A layout context manages the positions of a glyph collection. When Layout() is
invoked, the OnGlyph() callback is triggered for every glyph extracted from the
original user string (a plain text). This base class does a very simple layout
job as a default.

To implement a more sophisticated layout, create your own layout class as a
nglFontLayout superclass, and override the OnGlyph() method. See OnGlyph()
documentation for more info.
*/
class NGL_API nglFontLayout
{
public:
  /** @name Life cycle */
  //@{
  nglFontLayout (nglFontBase& rFont, float PenX = 0.0f, float PenY = 0.0f);
  /*!< Initialize a layout context
    \param rFont source font: we try to use this font for all the glyphs. Missing glyphs can try to use font substituion call backs.
    \param PenX initial horizontal pen position
    \param PenY initial vertical pen position

    Kerning is turned on as a default, see UseKerning().
  */
  virtual ~nglFontLayout();

  void Init (float PenX = 0.0f, float PenY = 0.0f);  ///< (re)initialize context
  //@}

  /** @name Metrics */
  //@{
  void UseKerning (bool Use = true);
  /*< Use kerning if available (turned on as a default)
   */
  int  GetMetrics (nglGlyphInfo& rInfo) const;
  /*!< Returns whole layout metrics (as a composite glyph)
    \param rInfo metrics info holder
    \return number of glyphs in the layout
  */
  //@}

  /** @name Layout */
  //@{
  int          Layout (const nglString& rText, bool FinalizeLayout = true);
  /*!< 
    \param rText string to decompose
    \return number of glyphs effectively processed

    Extract a glyph collection from \p rText and build the layout. See OnGlyph().
  */
  virtual void OnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph);
  /*!< Add a glyph to the layout
    \param pFont font that contains the glyph to render (only useful when we need font substitution)
    \param rString current laid out string
    \param Pos position of the current char in the original laid out string
    \param pGlyph glyph object, might be null if the current char maps to no glyph

    This method inserts the given glyph into the current layout. It is a simple
    implementation which supports kerning and newline (\\n). Your own layout can
    be implemented by overloading this method. Here is a trivial example :

\code
void nglFontLayout::Onglyph (const nglString& rString, int Pos, nglGlyphInfo* pGlyph)
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

    Have a look at nglFontLayout::OnGlyph() default implementation for a little
    more involved exemple (newline and kerning support).

    It might be possible that a single char decomposes to several glyphs, in
    this case OnGlyph() will be called once per output glyph, with the same
    \p Pos parameter.
  */
  virtual void OnFinalizeLayout();
  virtual nglFontBase* FindFontForMissingGlyph(nglFontBase* pOriginalFont, nglChar Glyph);
  
  uint  GetGlyphCount() const;
  const nglGlyphLayout* GetGlyph   (uint Offset) const;
  const nglGlyphLayout* GetGlyphAt (float X, float Y) const;
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
  
protected:
  typedef std::vector<nglGlyphLayout> GlyphList;

  nglFontBase&       mFont;        ///< Font source for glyphs
  float              mPenX;        ///< Current horizontal pen position
  float              mPenY;        ///< Current vertical pen position
  bool               mUseKerning;  ///< Use kerning in default layout
  uint               mGlyphPrev;   ///< Last laid out glyph (for kerning), 0 if none
  GlyphList          mGlyphs;      ///< List of glyphs
  nglFontBase*       mpFontPrev;   ///< The font used to render the last glyph (mainly used to handle kerning)

  bool AddGlyph   (nglFontBase* pFont, float X, float Y, int Pos, nglGlyphInfo* pGlyph);
  /*!< Add a localized glyph to the layout
    \param X horizontal pen position
    \param Y vertical pen position
    \param Pos glyph position in original laid out string
    \param pGlyph glyph to insert, no insertion if NULL
    \return true if the glyph was succesfully appended to the layout

    Append a glyph to the current layout with pen position (\p X, \p Y).
    A nglGlyphLayout instance is added at the end of the mGlyphs vector.
    Internally, the global layout bounding box is updated.

    The \p Pos parameter is used for reverse mapping in GetGlyphAt(),
    to recover from a glyph back to the source character in the original
    laid out string. Multiple glyphs can reverse map to the same character
    index.
  */
  bool GetKerning (nglFontBase* pFont, uint Index, float& rX, float& rY);
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

private:
  float mXMin, mXMax;
  float mYMin, mYMax;
  
  nglFontLayout(const nglFontLayout& rLayout) : mFont(rLayout.mFont)
  {
  // Undefined copy constructor (untils somone proves the default copy ctor is safe)
  }

  void InitMetrics();
  void CallOnGlyph (nglFontBase* pFont, const nglString& rString, int Pos, nglGlyphInfo* pGlyph);
};


