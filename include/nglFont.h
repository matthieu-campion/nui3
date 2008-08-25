/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglFont.h
\brief Font renderer (2D, OpenGL based)
*/

#ifndef __nglFont_h__
#define __nglFont_h__

//#include "nui.h"
#include "nglFontBase.h"
class nglFontLayout;
class nglGlyphLayout;
class nglFontCache;


//! Font renderer
/*!
This class implements a 2D font renderer, using the nglFontBase core services.

It provides both very portable and very optimized  backends. You won't get
decent performance until you call SetCache() and understand how the cache
mechanism works.

See also nglFontLayout, for both layout and performance issues.
*/
class NGL_API nglFont : public nglFontBase
{
public:
  /** @name Life cycle */
  //@{
  nglFont ();
  nglFont (uint8* pBuffer, uint32 BufferSize, uint Face, bool StaticBuffer);
  /*!< Create font from memory.
    Create a font from a memory buffer.
  */
  nglFont (const nglPath& rFontFile, uint Face);
  nglFont (const nglFont& rFont);
  /*!< Copy a font
    \param rFont source font

    Copies all attributes (scaling, color, alpha test), but do not share
    the cache (ie. the copy comes with no cache as a default).

    See also nglFontBase copy constructor.
  */
  virtual ~nglFont();
  //@}

  /** @name Rendering settings */
  //@{
  void SetScale     (float X, float Y);
  /*!< Install default cache
    \param X abscissa unit to pixel ratio
    \param Y ordinate unnit to pixel ratio

    nglFont is tuned for correct and UI-class rendering, making anti-aliasing and
    hinting first-class citizens. The output quality for small fonts (< 20 pixels)
    will only be satisfying if nglFont knows how to map a texel to a pixel. In
    other words, if the current projection matrix is setup to an orthonormal
    projection wich maps one GL unit to one pixel size. If this is not the case,
    you should provide the scaling ratios with SetScale().

    SetScale() can also be used with negative values, for instance to support
    direct referentials. As a default, scaling is set to (1, 1) and the OpenGL
    convention (X from left to right, Y from bottom to top) is expected. If
    you want the classical '2D convention' (Y axis from top to bottom), call
    SetScale(1, -1).
  */
  void SetColor     (float R, float G, float B, float A = 1.0f);
  /*!< Set font color (including transparency)

    Note that the current color is only set current to the rendering context
    when SetContext() is called.
  */
  void SetAlphaTest (float Threshold = 0.01f);
  /*!< Set alpha test threshold
    \param Threshold alpha culling threshold in the 0..1 range, 0 turns it off

    Alpha test culls fragments which have an alpha value lower than a given
    \p Threshold. This optimization can be very efficient for glyphs since
    most pixels of a glyph bitmap are fully transparent. If you set the
    threshold to high, you will hurt anti-aliasing without gaining performance
    at all. A value between 1/100 and 5/100 is recomended.

    Note that alpha test is only setup within the current rendering context
    when SetContext() is called.
  */
  //@}

  /** @name Rendering context */
  //@{
  virtual void PushContext();  ///< Save GL context bits subject to be modified by SetContext()
  virtual void PopContext();   ///< Restore GL context saved by PushContext()
  virtual void SetContext();   ///< Set up GL context using current rendering settings
  //@}

  /** @name Rendering */
  //@{
  int  Print (float X, float Y, const nglString& rText, bool AutoContext = false);
  /*!< Print a text string
    \param X pen origin abscissa
    \param Y pen origin ordinate
    \param rText text string to display
    \param AutoContext if true, take care of saving, setting and then restoring
           the GL context

    This is a variant of the other Print() method, which invokes the default
    layout class. It is equivalent to :

\code
  nglFontLayout layout(this);
  layout.Layout(rText);
  Print(X, Y, layout, AutoContext);
\endcode
  */
  int  Print (float X, float Y, const nglFontLayout& rLayout, bool AutoContext = false);
  /*!< Print a text string
    \param X pen origin abscissa
    \param Y pen origin ordinate
    \param rLayout layout to display
    \param AutoContext if true, take care of saving, setting and then restoring
           the GL context

    Render a prepared layout. The pen origin is \e not the top left corner of
    the first glyph, see about glyph metrics and bearing in nglGlyphInfo.

    The \p AutoContext code is equivalent to :

\code
  if (AutoContext)
  {
    PushContext();
    SetContext();
  }

  ... layout rendering happens here ...

  if (AutoContext)
    PopContext();
\endcode
  */
  //@}

  /** @name Cache */
  //@{
  bool UseCache (int Hint = 0);
  /*!< Install default cache
    \param Hint

    Install the default cache. This cache creates a single texture object,
    where glyph bitmaps are stored in cells from a regular grid. Upon
    initialization, the cache will search for the smallest and more
    efficient texture size fitting at least \p Hint cells. If your
    application supports nglApplication::ParseDefaultArgs(), you can call
    it with <pre>--log-level font:debug</pre> to check the allocation
    algorithm decision. Here is an example with a \p Hint of 32 :

\code
Font: nglFontTexCache: Init: trying to fit 32 glyphs
Font:   font bbox is 25x20
Font:   tex range : 64..4096 texels
Font:   matching tiles :
Font:       32 KB :   64x 512,   50 glyphs, waste   8 KTexels (23.7%)
Font:       32 KB :  128x 256,   60 glyphs, waste   3 KTexels (8.4%)
Font:   selecting 128x256 (fitting 60 glyphs)
\endcode

    When this cache is installed, SetContext() implies setting up the
    first texture unit to the cache texture. Thus you should try to
    draw as much text using the same font as possible to spare on
    texture unit context switches.
  */
  bool SetCache (nglFontCache* pCache);
  /*!< Install user defined cache
    \param pCache cache instance
    \return true if cache installed

    From this point, \p pCache's life cycle is managed by the nglFont
    object. It will be released either when SetCache() is called again
    (with \p pCache set to NULL or another valid instance), either when
    the nglFont object is destroyed.

    The nglFontBase::Init() is immediatly called on \p pCache, giving
    it a chance to tune with respect to the current nglFont object.
  */
  //@}

  /** @name Rendering helper */
  //@{
  bool BitmapToTexture (const GlyphBitmap& rBitmap, GLint OffsetX, GLint OffsetY) const;
  /*!<
    \param rBitmap source bitmap (monochrome or grayscale)
    \param OffsetX X offset in current texture (in texel)
    \param OffsetY Y offset in current texture (in texel)

    This method transfers a glyph bitmap (1-bit mono or 8-bit grays) to the current
    2D texture object at position [\p OffsetX, \p OffsetY] (in texels). The
    destination texture format must be 8-bit single alpha (or luminance) channel.
    The bitmap will be uploaded via means of glTexSubImage2D. You can create
    the destination texture beforehand with :

\code
glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, NULL);
\endcode

    In this example, \p width and \p height should represent a valid texture size, and
    accomodate the bitmap (which dimensions are \p pBitmap->width * \p pBitmap->rows).
  */
  //@}

protected:
  float    mScaleX;     ///< X scale as set by SetScale()
  float    mScaleY;     ///< Y scale as set by SetScale()
  float    mColor[4];   ///< Color as set by SetColor()
  GLclampf mAlphaTest;  ///< Alpha test threshold as set by SetAlphaTest()

  /** @name Rendering callbacks */
  //@{
  virtual bool OnPrintGlyph (const nglGlyphLayout& rGlyph);
  /*!<
    \param rGlyph laid out glyph to render
    \return true if the glyph was succesfully rendered

    This callback is invoked by Print() for every glyph from a layout object.
    This method must return false if the glyph could not be rendered (for any
    reason), true otherwise.

    The nglFont implementation provides a default implementation which can
    use plain texturing (see OnPrintGlyphDefault()) as well as a cache based
    on texture objects (see UseCache()).
  */
  bool OnPrintGlyphDefault (const nglGlyphLayout& rGlyph);
  /*!<
    \param rGlyph laid out glyph to render
    \return true if the glyph was succesfully rendered

    This is the default glyph rendering method which is called when there
    is no cache to handle the job, or the cache could not handle it. It
    uploads the glyph and draws it via a texture quad. It does not use
    texture objects, hence it is very slow, but it is expected to work
    even on very restricted OpenGL implementations.
  */
  //@}

protected:
  const nglChar* OnError (uint& rError) const;

private:
  nglFontCache* mpCache;

  void Defaults();
  bool CacheInit (nglFontCache* pCache);
  void CacheRelease();
};

#endif // __nglFont_h__
