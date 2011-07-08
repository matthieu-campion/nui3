/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nuiRect.h"
#include "nuiPanose.h"

class nglPath;
class nuiTexture;
class nuiDrawContext;
class nuiTexture;
class nuiFont;
class nuiTextLayout;
class nuiTextLine;
#include "nuiTextRun.h"
class nuiRenderArray;

//! Font units
/*!
 Different font units used for nuiFontBase metrics management methods.
 */
enum nuiFontUnit
{
  eFontUnitEM,     ///< Native font unit (used by the font designer)
  eFontUnitPoint,  ///< Point unit. A point unit is device independent, and represents 1/72 inch (0.35 mm)
  eFontUnitPixel   ///< Pixel (or texel) unit
};


//! Font descriptor
/*!
 */
class nuiFontDesc
{
public:
  nuiFontDesc(const nglPath& rPath, int32 Face);
  nuiFontDesc(nglIStream& rStream);
  ~nuiFontDesc();
  
  const nglPath& GetPath() const;
  bool CheckPath();
  const nglString& GetName() const;
  const nglString& GetStyle() const;
  
  int32 GetFace() const;
  bool GetBold() const;
  bool GetItalic() const;
  bool GetMonospace() const;
  bool GetScalable() const;
  
  bool HasEncoding(nglTextEncoding Encoding) const;
  bool HasGlyph(nglUChar Glyph) const;
  bool HasSize(int32 Size) const;
  
  const std::set<nglTextEncoding>&  GetEncodings() const;
  const std::set<int32>&            GetSizes() const;
  
  const nuiFontPanoseBytes& GetPanoseBytes() const; 
  
  bool IsValid() const;
  
  bool Save(nglOStream& rStream);
  bool Load(nglIStream& rStream);
  
private:
  bool mValid;
  nglPath   mPath;
  nglString mName;
  nglString mStyle;
  int32     mFace;
  
  bool mBold;
  bool mItalic;
  bool mMonospace;
  bool mScalable;
  std::set<nglTextEncoding> mEncodings;
  typedef std::pair<nglUChar, nglUChar> GlyphRange;
  std::vector<GlyphRange> mGlyphs;
  std::set<int32> mSizes;
  
  nuiFontPanoseBytes        mPanoseBytes;
  
  friend class nuiFontBase;
};



//! Font information
/*!
 If the font has a fixed size (ie. 'bitmap' type), the only valid global metrics
 are the bitmap size (see nuiFontBase::GetSize). All metrics fields (from the bounding
 box to the underline position) have no meaning and should not be used.
 
 If the font is scalable (ie. 'vectorial' type), global metrics are available.
 They are given either in pixels (using the current font size) or in font units,
 as requested by the user while invoking nuiFontBase::GetInfo().
 
 See nuiFontBase::GetInfo() for more informations. See also the insightfull introduction
 to digital typography on FreeType's site : http://www.freetype.org/freetype2/docs/glyphs/ .
 */
class NGL_API nuiFontInfo
{
public:
  const class nuiFontBase* pFont;  ///< Back reference to the described font object
  
  nglString   FamilyName;   ///< Family name (Helvetica, Times, Courier, etc)
  nglString   StyleName;    ///< Style name (Italic, Bold, Stroke, etc)
  bool        IsScalable;   ///< If true, font is freely scalable. If false only bitmap data is available.
  bool        IsBold;       ///< True if the font is tagged as 'bold' (typographer hint)
  bool        IsMonospace;  ///< True if the font is tagged as 'fixed-pitch'
  bool        IsItalic;     ///< True if the font is tagged as 'italic' (typographer hint)
  uint        GlyphCount;   ///< Total number of available glyphs
  nuiFontUnit Unit;         ///< Unit of global metrics (if any)
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
  
  nuiFontInfo();
  virtual ~nuiFontInfo();
  void Dump (uint Level = 0) const;  ///< Dumps informations to the application log using \p Level verbosity
};


class NUI_API nuiFontBase: public nuiObject
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
  nuiFontBase (float Size = 14.0f);
  /*!< Create default font
   Create a font from an embedded description. It implements all ASCII characters
   using a monospace, lightly serifed, legible scalable font.
   \param Size size of the font
   */
  
  nuiFontBase(uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer, float Size = 14.0f);
  /*!< Create font from memory.
   Create a font from a memory buffer.
   */
  
  nuiFontBase (const nglPath& rFontFile, uint Face = 0, float Size = 14.0);
  /*!< Create a font from a file
   \param rFontFile path to font file
   \param Face face index for multi-face fonts
   \param Size size of the font
   
   This will load a font file using any of the FreeType's supported file formats,
   including TrueType, PostScript Type1, Type42, PCF and Windows' FON (among others).
   */
  nuiFontBase (const nuiFontBase& rFont);
  /*!< Copy a font
   \param rFont source font
   
   Make a copy of the font ressource. Current metrics are also copied (size, resolution
   and kerning). The charmap is also preserved.
   
   \e Note : this copy constructor is lightweight due to the cache infrastructure.
   */
  virtual ~nuiFontBase();
  /*!< Release the font ressource
   */
  //@}
  
  bool IsValid() const;
  
  /** @name Global font info & metrics */
  //@{
  bool      GetInfo (nuiFontInfo& rInfo, nuiFontUnit Unit = eFontUnitPixel) const;
  /*!< Fetch font informations and global metrics
   \param rInfo information receptacle
   \param Unit unit used in metric fields
   \return \p rInfo contains valid informations if true
   
   Return a wealth of information about this font face. See nuiFontInfo.
   */
  
  nglString GetFamilyName() const;  ///< Returns font family (ie. Helvetica, Geneva, etc)
  nglString GetStyleName() const;   ///< Returns font style (ie. Bold, Italic, Condensed, etc)
  bool      IsScalable() const;     ///< Returns wether the font is scalable or not (ie. bitmap based)
  bool      IsBold() const;         ///< Returns font weight, ie. \e bold or \e regular
  bool      IsMonospace() const;    ///< Returns true if the face contains only fixed-pitch glyphs
  bool      IsItalic() const;       ///< Returns font slant, ie. \e italic or \e regular
  float     GetHeight (nuiFontUnit Unit = eFontUnitPixel, float DefaultSpacing = 1.f) const;
  float     GetAscender(nuiFontUnit Unit = eFontUnitPixel) const;
  float     GetDescender(nuiFontUnit Unit = eFontUnitPixel) const;
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
   
   Please note that the \e Height field of nuiFontInfo only returns the designer's font
   height when it's defined, zero otherwise. GetHeight() is a higher level method which
   helps you to support fonts which do not define their height (mostly bitmap fonts).
   */
  uint      GetGlyphCount() const;  ///< Returns the number of available glyphs
  void      GetGlyphs(std::set<nglUChar>& rGlyphs) const; ///< Return all the available charcodes in the current charmap.
  
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
  float GetSize (nuiFontUnit Unit = eFontUnitPixel) const;  ///< Returns current font size using \p Unit
  
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
  
  bool  GetMetrics (nuiGlyphInfo& rInfo, nglUChar Char, GlyphType Type = eGlyphBitmap) const;
  /*!< Fetch glyph metrics
   \param rInfo information receptacle
   \param Char character code of the inspected glyph
   \param Type compute metrics from outline (if available) or bitmap
   \return \p rInfo contains valid informations if true
   
   This is a frontend to GetGlyphInfo(), but with a character code (with
   respect to the curretn charmap) instead of a raw glyph index.
   */
  bool GetGlyphInfo (nuiGlyphInfo& rInfo, uint Index, GlyphType Type) const;
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
   Output metrics are always in pixels. See nuiGlyphInfo for more info.
   
   To retrieve string metrics, use nuiTextLayout::GetMetrics().
   */
  bool GetKerning (uint Left, uint Right, float& rX, float& rY) const;
  /*!< Fetch kerning values from a glyph pair
   \param Left Left glyph index
   \param Right Right glyph index
   \param rX horizontal advance return
   \param rY vertical advance return
   \return true if \p rX and \p rY represent a valid kerning vector
   
   This methods work on raw glyph indexes because it is expected to be called
   from a layout implementation (see nuiTextLayout), where indexes are proper
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
   time, nuiFontBase will search for a Unicode charmap, then fallback to the first
   available charmap, and finaly select none if none is available.
   */
  const nglChar* GetCharMapName() const;           ///< Get current charmap human readable name
  const nglChar* GetCharMapName(int Index) const;  ///< Get charmap \p Index human readable name
  
  int GetGlyphIndexes (const nglChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const;
  int GetGlyphIndexes (const nglUChar* pSource, int SourceLength, uint* pIndexes, int IndexesLength) const;
  /*!< Retrieve a range of glyph indexes from a nglChar array (encoding aware)
   \param pSource nglChar source array (zero terminal is not handled)
   \param SourceLength \p pSource array length in nglChars
   \param pIndexes target glyph indexes array
   \param IndexesLength \p pIndexes array length (in uints)
   \return number of nglChars/glyph indexes converted, -1 if a fatal error occured
   
   This method retrieves glyph indexes from nglChars using the current active charmap.
   */
  
  int32 GetGlyphIndex(nglUChar Source, nglUChar VariationSelector = 0) const;
  /*!< Retrieve a glyph indexes from a nglUChar
   \param Source nglUChar source
   \return glyph indexes converted, -1 if a fatal error occured
   
   This method retrieves a glyph index from an nglUChar using the current active charmap.
   */
  //@}
  
  void GetEncodings(std::set<nglTextEncoding>& rEncodings);
  void GetSizes(std::set<int32>& rSizes);
  
  /** @name Rendering/rasterizing (scalable fonts only) */
  //@{
  RenderMode   GetRenderMode () const;  ///< Get current rendering mode
  bool SetRenderMode (RenderMode Mode);
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

  
  void SetAlphaTest(float Threshold = 0.01f);


  void Print(nuiDrawContext *pContext, float X, float Y, const nglString& rText, bool AlignGlyphPixels = true);
  
  bool PrepareGlyph(float X, float Y, nuiTextGlyph& rGlyph);
  
  void Shape(nuiTextRun* pRun);
protected:
  GLclampf mAlphaTest;  ///< Alpha test threshold as set by SetAlphaTest()
  
  bool  SetSize (float Size, nuiFontUnit Unit = eFontUnitPixel);
  /*!< Set current font size
   \param Size new font size, using \p Unit
   \param Unit input unit
   \return true if the new font size \p Size was succesfully applied
   
   Changing the current font size for a scalable font is always possible, but is restricted
   to some discrete values for bitmap fonts. Bitmap fonts can embed several glyph sizes,
   they are described in pixels and can be selected using this method.
   */
  

private:
  class NUI_API GlyphLocation
  {
  public:
    GlyphLocation();
    GlyphLocation(int OffsetX, int OffsetY, int Width, int Height, int OffsetTexture);
    ~GlyphLocation();

  public:
    int mOffsetX, mOffsetY, mWidth, mHeight, mOffsetTexture;
  };

  typedef std::map<int, GlyphLocation> GlyphLocationLookupTable;
  typedef std::map<int, GlyphLocation>::iterator GlyphLocationLookupTableIterator;
  typedef std::vector<nuiTexture *> Textures;

  GlyphLocationLookupTable mGlyphLocationLookupTable;
  Textures mTextures;

  int mCurrentX;
  int mRowMaxWidth;
  int mCurrentY;
  int mCurrentTexture;

  nuiTexture *AllocateTexture(int size);
  bool CopyBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY);

  void Blit8BitsBitmapToTexture(const GlyphBitmap &rBitmap, nuiTexture *pTexture, unsigned int OffsetX, unsigned int OffsetY);

  void GetCacheGlyph(int Index, nuiFontBase::GlyphLocation &rGlyphLocation);
  void AddCacheGlyph(int Index, nuiFontBase::GlyphLocation &rGlyphLocation);
  bool FindNextGlyphLocation(int Width, int Height, int &rOffsetX, int &rOffsetY);
  void Defaults();
   
  static const int TEXTURE_SIZE = 128;

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
  bool                  mValid;
  
  /* Init/setup
   */
  static bool Init();                                  // Mandatory init for all instances
  bool Load (const nglPath& rPath, uint Face, float Size);  // Create font object from font file
  bool Load (const uint8* pBase, int32 BufferSize, uint Face, bool StaticBuffer, float Size); // Create font object from memory
  bool Load (FaceID ID, float Size);                        // Clone font object from font ID (font already 'installed')
  bool LoadFinish(float Size);                            // Setup some defaults (size, charmap, rendering)
  
  /* Internal callbacks
   */
  static void OnExit();
  
  friend class nuiFontInfo;
  friend class nuiFontManager;
};

