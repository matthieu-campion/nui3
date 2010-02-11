/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglFontBase.h
\brief Font abstraction (see nglFont for display)
*/

#pragma once

//#include "nui.h"
#include "nglError.h"
#include "nglPath.h"
#include "nuiPanose.h"

class nglStringConv;


//! Font units
/*!
Different font units used for nglFontBase metrics management methods.
*/
enum nglFontUnit
{
  eFontUnitEM,     ///< Native font unit (used by the font designer)
  eFontUnitPoint,  ///< Point unit. A point unit is device independent, and represents 1/72 inch (0.35 mm)
  eFontUnitPixel   ///< Pixel (or texel) unit
};


//! Font descriptor
/*!
*/
class NGL_API nglFontDesc
{
public:
  nglString Name;
  bool      IsBold;
  bool      IsMonospace;
  bool      IsItalic;
  uint      PixelSize;

  nglFontDesc(); // default font ?
  nglFontDesc(const nglFontDesc& rDesc);

  nglPath GetPath() const;

  static bool Find(const nglFontDesc& rDesc);

private:
  nglPath mPath;
};


//! Font information
/*!
If the font has a fixed size (ie. 'bitmap' type), the only valid global metrics
are the bitmap size (see nglFontBase::GetSize). All metrics fields (from the bounding
box to the underline position) have no meaning and should not be used.

If the font is scalable (ie. 'vectorial' type), global metrics are available.
They are given either in pixels (using the current font size) or in font units,
as requested by the user while invoking nglFontBase::GetInfo().

See nglFontBase::GetInfo() for more informations. See also the insightfull introduction
to digital typography on FreeType's site : http://www.freetype.org/freetype2/docs/glyphs/ .
*/
class NGL_API nglFontInfo
{
public:
  const class nglFontBase* pFont;  ///< Back reference to the described font object

  nglString   FamilyName;   ///< Family name (Helvetica, Times, Courier, etc)
  nglString   StyleName;    ///< Style name (Italic, Bold, Stroke, etc)
  bool        IsScalable;   ///< If true, font is freely scalable. If false only bitmap data is available.
  bool        IsBold;       ///< True if the font is tagged as 'bold' (typographer hint)
  bool        IsMonospace;  ///< True if the font is tagged as 'fixed-pitch'
  bool        IsItalic;     ///< True if the font is tagged as 'italic' (typographer hint)
  uint        GlyphCount;   ///< Total number of available glyphs
  nglFontUnit Unit;         ///< Unit of global metrics (if any)
  uint        FaceCount;    ///< The number of Faces in the font file
  
  /** @name Global metrics (only relevant for scalable fonts) */
  //@{
  float BBoxMinX;        ///< Global bounding box, lower X coordinate
  float BBoxMinY;        ///< Global bounding box, lower Y coordinate
  float BBoxMaxX;        ///< Global bounding box, upper X coordinate
  float BBoxMaxY;        ///< Global bounding box, upper Y coordinate
  float Ascender;        ///< Ascender
  float Descender;       ///< Descender
  float Height;          ///< Height
  float AdvanceMaxW;     ///< Maximum horizontal advance
  float AdvanceMaxH;     ///< Maximum vertical advance
  float UnderlinePos;    ///< Underline Y coordinate
  float UnderlineThick;  ///< Underline thickness
  //@}

  nglFontInfo();
  virtual ~nglFontInfo();
  void Dump (uint Level = 0) const;  ///< Dumps informations to the application log using \p Level verbosity
};


//! Glyph metrics
/*!
All glyph metrics are given in pixels, they depend on the current font size and
resolution. See :

- nglFontBase::GetGlyphInfo() for more informations
- the insightfull introduction to digital typography on FreeType's site :
  http://www.freetype.org/freetype2/docs/glyphs/
- the font/metrics example bundled with the regular NGL distribution
*/
class NGL_API nglGlyphInfo
{
public:
  int   Index;     ///< Glyph index in font face
  float Width;     ///< Bounding box width
  float Height;    ///< Bounding box height
  float BearingX;  ///< X bearing
  float BearingY;  ///< Y bearing
  float AdvanceX;  ///< Horizontal advance
  float AdvanceY;  ///< Vertical advance

  nglGlyphInfo();
  virtual ~nglGlyphInfo();
  void Dump (uint Level = 0) const;  ///< Dumps informations to the application log using \p Level verbosity
};


// Errors
#define NGL_FONT_ENONE  0  ///< No error
#define NGL_FONT_EINIT  1  ///< FreeType library initialisation failed
#define NGL_FONT_ELOAD  2  ///< Couldn't load font ressource


//! Base font class
/*!
This base font class does not handle final rendering. It is intended to support all
font services but rendering. Specific rendering methods are implemented atop this
class using some advanced and specialized services (see the protected interface).

This class implementation is based on FreeType ( http://www.freetype.org ). It will
load fonts of various formats (see nglFontBase()), expose global font as well as
glyph specific informations and metrics, control scalable fonts rasterizing
(hinting, anti-aliasing), manage charmaps and synthesize missing ones, calculate
string layout (no BiDi or vertical support for now) and somehow contribute to the
world's entropy.

Although this class is intented to handle transparently both scalable and bitmap
fonts, there are some major design differences you ought to know :

- bitmap fonts often come bundled in a font file as a collection of glyphs from
a given font family, a given style (bold, italic, etc) and a given size. In the
rare event where the font file embeds multiple sizes, SetSize() will still let you
choose between them. Bitmap fonts do not need rendering, thus SetRenderMode()
will be ignored. Bitmap fonts do not define their <em>EM square</em>, thus operations
using the \a eFontUnitPoint unit will fail or return zero. Finally, most bitmap
fonts do not have global metrics (see nglFontInfo and GetHeight()).

- scalable fonts often come bundled in a font file as a collection of glyphs from
a given font family and a given style (bold, italic, etc). If pre-rendered glyphs
are available (called <em>embedded bitmaps</em>), they are automatically used when the
font size matches. Otherwise those glyphs will have to be rasterized in place :
this is controlled by SetRenderMode().
*/
class NGL_API nglFontBase : public nglError
{
public:
  typedef class GlyphData* GlyphHandle;

  //! Glyph fetching mode. See GetGlyphInfo() and GetGlyph()
  enum GlyphType
  {
    eGlyphNative,   ///< Fetch native format (bitmap for fixed sizes fonts, outlines for scalable fonts)
    eGlyphOutline,  ///< Fetch as outlines (only works for scalable fonts)
    eGlyphBitmap    ///< Fetch as a bitmap (always possible, might imply rendering)
  };

  //! Description of a glyph bitmap representation
  class GlyphBitmap
  {
  public:
    uint   Width;   ///< Width in pixels
    uint   Height;  ///< Height in pixels
    int    Top;     ///< Vertical distance of bitmap topmost pixel from glyph origin
    int    Left;    ///< Horizontal distance of bitmap leftmost pixel from glyph origin
    uint8* pData;   ///< Raw pixel buffer (see \a Depth and \a Pitch)
    uint8  Depth;   ///< Bit depth of pixel, currently 1 or 8
    uint   Pitch;   ///< Number of bytes between successive lines of pixels
  };

  typedef uint RenderMode;  ///< Rendering flags. See SetRenderMode() and GetRenderMode()

  static const RenderMode AntiAliasing;  ///< Use anti-aliasing. See SetRenderMode()
  static const RenderMode Hinting;       ///< Interpret font hints. See SetRenderMode()

  static float DefaultPixelSize;  ///< Default size (in pixels) for scalable fonts

  /** @name Life cycle */
  //@{
  nglFontBase ();
  /*!< Create default font
    Create a font from an embedded description. It implements all ASCII characters
   using a monospace, lightly serifed, legible scalable font.
  */

  nglFontBase(uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer);
  /*!< Create font from memory.
    Create a font from a memory buffer.
  */

  nglFontBase (const nglPath& rFontFile, uint Face = 0);
  /*!< Create a font from a file
    \param rFontFile path to font file
    \param Face face index for multi-face fonts

    This will load a font file using any of the FreeType's supported file formats,
    including TrueType, PostScript Type1, Type42, PCF and Windows' FON (among others).
  */
  nglFontBase (const nglFontBase& rFont);
  /*!< Copy a font
    \param rFont source font

    Make a copy of the font ressource. Current metrics are also copied (size, resolution
    and kerning). The charmap is also preserved.

    \e Note : this copy constructor is lightweight due to the cache infrastructure.
  */
  virtual ~nglFontBase();
  /*!< Release the font ressource
  */
  //@}

  /** @name Global font info & metrics */
  //@{
  bool      GetInfo (nglFontInfo& rInfo, nglFontUnit Unit = eFontUnitPixel) const;
  /*!< Fetch font informations and global metrics
    \param rInfo information receptacle
    \param Unit unit used in metric fields
    \return \p rInfo contains valid informations if true

    Return a wealth of information about this font face. See nglFontInfo.
  */

  nglString GetFamilyName() const;  ///< Returns font family (ie. Helvetica, Geneva, etc)
  nglString GetStyleName() const;   ///< Returns font style (ie. Bold, Italic, Condensed, etc)
  bool      IsScalable() const;     ///< Returns wether the font is scalable or not (ie. bitmap based)
  bool      IsBold() const;         ///< Returns font weight, ie. \e bold or \e regular
  bool      IsMonospace() const;    ///< Returns true if the face contains only fixed-pitch glyphs
  bool      IsItalic() const;       ///< Returns font slant, ie. \e italic or \e regular
  float     GetHeight (nglFontUnit Unit = eFontUnitPixel, float DefaultSpacing = 1.f) const;
  float     GetAscender(nglFontUnit Unit = eFontUnitPixel) const;
  float     GetDescender(nglFontUnit Unit = eFontUnitPixel) const;
  uint      GetFaceCount() const;
  bool      IsLastResort() const; ///< This returns true if this font is considered as a fallback last resort.
  /*!< Return font height
    \param Unit output unit
    \param DefaultSpacing user specified interline spacing (or <em>line gap</em>) when
           the font does not define one using \p Unit as input unit)
    \return font height in \p Unit or zero when not applicable

    The font height is the distance that should separate two successive (horizontal) baselines.

    Most scalable fonts define their font height, a value specifically choosen by the font
    designer. In this case, the \p DefaultSpacing value is ignored and the original font
    height is returned.

    Most bitmap fonts do not have a defined font height. If such a value is not present
    in the font ressource, the font height is computed as the sum of the current font size
    and the \p DefaultSpacing parameter. An interline spacing of 1 pixel is pretty common
    in this case.

    Please note that the \e Height field of nglFontInfo only returns the designer's font
    height when it's defined, zero otherwise. GetHeight() is a higher level method which
    helps you to support fonts which do not define their height (mostly bitmap fonts).
  */
  uint      GetGlyphCount() const;  ///< Returns the number of available glyphs
  void      GetGlyphs(std::set<nglChar>& rGlyphs) const; ///< Return all the available charcodes in the current charmap.

  // Unit conversion methods
  float     EMToPixel   (float Value) const;  ///< Convert from EM to pixels (undefined for bitmap fonts)
  float     EMToPoint   (float Value) const;  ///< Convert from EM to points (undefined for bitmap fonts)
  float     PixelToPoint(float Value) const;  ///< Convert from pixels to points (font size independent)
  float     PointToPixel(float Value) const;  ///< Convert from points to pixels (font size independent)

  const nuiFontPanoseBytes& GetPanoseBytes() const;
  /*!< Return the panose (opentype font features) or this font. See http://www.w3.org/Fonts/Panose/pan2.html to learn about the panose standard.
   \return a reference to the panose structure for this font
   */
  
  //@}
  bool HasPanoseInfo() const; ///< Returns true if this font containst panose info bytes.
  
  /** @name Glyph metrics */
  //@{
  float GetSize (nglFontUnit Unit = eFontUnitPixel) const;  ///< Returns current font size using \p Unit

  float GetResolution () const;  ///< Get current resolution in DPI (dots per inch)
  void  SetResolution (float Resolution);
  /*!< Set current resolution
    \param Resolution new resolution in DPI (dots per inch)

    The resolution is the link between a pixel (being a screen pixel or a printer's
    ink bubble) and its physical size. A \e dot is a physical measure which is equal
    to a 72th of an inch (0.35 mm). This parameter is used when input or output
    metric values are given using \a eFontUnitPoint. The default is 72dpi (1 pixel = 1 dot).

    You can fetch and use the physical resolution of the display device from a nglWindow
    instance with nglWindow::GetResolution().
  */

  bool  GetMetrics (nglGlyphInfo& rInfo, nglChar Char, GlyphType Type = eGlyphBitmap) const;
  /*!< Fetch glyph metrics
    \param rInfo information receptacle
    \param Char character code of the inspected glyph
    \param Type compute metrics from outline (if available) or bitmap
    \return \p rInfo contains valid informations if true

    This is a frontend to GetGlyphInfo(), but with a character code (with
    respect to the curretn charmap) instead of a raw glyph index.
  */
  virtual bool GetGlyphInfo (nglGlyphInfo& rInfo, uint Index, GlyphType Type) const;
  /*!< Fetch a glyph description from a a given glyph index
    \param rInfo information receptacle
    \param Index glyph index
    \param Type compute metrics from outline (if available) or bitmap
    \return \p rInfo contains valid informations if true

    Metrics can be computed against outline (only available for scalable fonts),
    or from their bitmap representation. Due to the hinting and rounding process,
    the metrics may differ between the outline and bitmap computations. You should
    use the same glyph type for layout computing and rendering.

    If \p Char does not have a corresponding glyph, returns false (see SetCharMap()).
    Output metrics are always in pixels. See nglGlyphInfo for more info.

    To retrieve string metrics, use nglFontLayout::GetMetrics().
  */
  bool GetKerning (uint Left, uint Right, float& rX, float& rY) const;
  /*!< Fetch kerning values from a glyph pair
    \param Left Left glyph index
    \param Right Right glyph index
    \param rX horizontal advance return
    \param rY vertical advance return
    \return true if \p rX and \p rY represent a valid kerning vector

    This methods work on raw glyph indexes because it is expected to be called
    from a layout implementation (see nglFontLayout), where indexes are proper
    glyph handlers.
  */
  //@}

  /** @name Character maps */
  //@{
  int            GetCharMapCount() const;   ///< Returns the number of supported charmaps
  int            GetCharMap() const;
  /*!< Return the current charmap
    \return current charmap index, between 0 and GetCharMapCount()-1, or -1 if
            no charmap is selected
  */
  bool           SetCharMap (int Index);
  /*!< Select current charmap
    \param Index charmap index, between 0 and GetCharMapCount()-1

    If the font embeds several charmaps, you can choose one by yourself. At creation
    time, nglFontBase will search for a Unicode charmap, then fallback to the first
    available charmap, and finaly select none if none is available.
  */
  const nglChar* GetCharMapName() const;           ///< Get current charmap human readable name
  const nglChar* GetCharMapName(int Index) const;  ///< Get charmap \p Index human readable name

  int GetGlyphIndexes (const nglChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const;
  /*!< Retrieve a range of glyph indexes from a nglChar array (encoding aware)
    \param pSource nglChar source array (zero terminal is not handled)
    \param SourceLength \p pSource array length in nglChars
    \param pIndexes target glyph indexes array
    \param IndexesLength \p pIndexes array length (in uints)
    \return number of nglChars/glyph indexes converted, -1 if a fatal error occured

    This method retrieves glyph indexes from nglChars using the current active charmap.
  */

  int32 GetGlyphIndex(nglChar Source) const;
  /*!< Retrieve a glyph indexes from a nglChar
   \param Source nglChar source
   \return glyph indexes converted, -1 if a fatal error occured
   
   This method retrieves a glyph index from an nglChar using the current active charmap.
   */
  //@}

  void GetEncodings(std::set<nglTextEncoding>& rEncodings);
  void GetSizes(std::set<int32>& rSizes);
  
  /** @name Rendering/rasterizing (scalable fonts only) */
  //@{
  RenderMode   GetRenderMode () const;  ///< Get current rendering mode
  virtual bool SetRenderMode (RenderMode Mode);
  /*!< Set rendering mode
    \param Mode a bitmask of RenderMode constants
    \return true if the rendering mode was succesfully applied

    The rendering mode affects the way scalable fonts are rendered as bitmap.

    \a AntiAliasing will use an 8bit alpha channel to represent smooth edges. You can
    apply this mask to a texture or a plain color, blend this on the target surface,
    and thus obtain smoothed font contours. \e Anti-aliasing is turned on as a default
    for scalable fonts.

    Most scalable fonts are hinted : the font designer added some information to help
    the renderer keep some font features consistant, such as stem width or spacing,
    serif control, and so on. These hints are especially important (and well tuned)
    for small sizes like 10-20 pixels. \e Hinting is turned on as a default.
  */
  //@}

  /** @name Glyph metrics, contour and bitmap retrieval */
  //@{
  GlyphHandle GetGlyph (uint Index, GlyphType Type) const;
  /*!< Fetch a glyph description from a a given glyph index
    \param Index glyph index
    \param Type extraction mode (see GlyphType)
    \return a GlyphHandle of the type \p Type, NULL if an error occured

    For scalable fonts, using the eGlyphBitmap type implies rendering.
    Rendering is controled by the current render mode (see SetRenderMode()).

    GetGlyph() can return monochrome (1 bit) or graylevel (8 bit) bitmaps,
    check the \e pixel_mode field. See also GetBitmap8().
  */
  GlyphType GetGlyphType (GlyphHandle Glyph) const;
  /*!< Returns the type (outline vs. bitmap) of a glyph handle
    \param Glyph glyph handle
    \return glyph type
  */
  bool GetGlyphBitmap (GlyphHandle Glyph, GlyphBitmap& rBitmap) const;
  /*!< Retrieve bitmap info from a glyph handle
    \param Glyph glyph handle
    \param rBitmap bitmap info holder
    \return true if \p rBitmap contains valid informations
  */
  bool GetBitmap8 (const GlyphBitmap& rSrc, GlyphBitmap& rDst) const;
  /*!< Convert a 1-bit monochrome bitmap to a 8-bit one
    \param rSrc source bitmap, if not monochrome will fail
    \param rDst destination bitmap, must have its \a pData buffer allocated
    \return true if the conversion was succesfull

    GetBitmap8() can convert monochrome bitmaps into graylevel ones, simply
    expanding a set bit as a full byte (255).
  */
  //@}

protected:
  virtual const nglChar* OnError (uint& rError) const; // see nglError

  bool  SetSize (float Size, nglFontUnit Unit = eFontUnitPixel);
  /*!< Set current font size
    \param Size new font size, using \p Unit
    \param Unit input unit
    \return true if the new font size \p Size was succesfully applied

    Changing the current font size for a scalable font is always possible, but is restricted
    to some discrete values for bitmap fonts. Bitmap fonts can embed several glyph sizes,
    they are described in pixels and can be selected using this method.
  */

private:
  typedef void* FaceID;                 // This will internally be a FTC_FaceID

  static const uint32   mBitLUT[16];    // bitmap to alphamap helper table

  class FaceHandle*     mpFace;         // Opaque handle to FreeType's face handle
  int                   mCharMap;       // Selected charmap (index)
  nglStringConv*        mpConv;         // Conversion context
  float                 mSize;          // Current font size in pixels
  float                 mResolution;    // Current resolution in dpi
  RenderMode            mRenderMode;    // Antialiasing/hinting, also encoded into mFTDesc.type

  /* Some global info (cached here for performance)
   */
  nglString             mFamilyName;
  nglString             mStyleName;
  float                 mUnitsPerEM;
  float                 mGlobalHeight;
 
  nuiFontPanoseBytes    mPanoseBytes;
  bool                  mHasPanoseInfo;
  bool                  mLastResort;
  
  /* Init/setup
   */
  void Defaults();
  bool Init();                                  // Mandatory init for all instances
  bool Load (const nglPath& rPath, uint Face);  // Create font object from font file
  bool Load (const uint8* pBase, int32 Size, uint Face, bool StaticBuffer); // Create font object from memory
  bool Load (FaceID ID);                        // Clone font object from font ID (font already 'installed')
  bool LoadFinish();                            // Setup some defaults (size, charmap, rendering)

  /* Internal callbacks
   */
  static void OnExit();

  friend class nglFontInfo;
};

