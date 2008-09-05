/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglFontCache.h
\brief Font cache helper for nglFont
*/

#ifndef __nglFontCache_h__
#define __nglFontCache_h__

//#include "nui.h"
class nglGlyphLayout;


//! Font cache helper for nglFont
/*!
A font cache implement a way to store and draw rasterized glyph images
efficiently. nglFont needs such a cache for decent performance, and
NGL comes with a default one (which implementation is private, see
nglFontTexCache in the source distribution).

This class must basically implement context management managements
similar to those of nglFont, and the crucial Draw().
*/
class NGL_API nglFontCache
{
public:
  /** @name Life cycle */
  //@{
  nglFontCache();
  virtual ~nglFontCache();

  virtual bool Init (const class nglFont* pFont) = 0;
  /*!< Attach to a font instance
    \param pFont font instance to cache

    This method is called when this cache is attached via
    nglFont::SetCache(). Global info and metrics can be fetched from
    this point to setup the cache internals. Do \e not rely on specific
    on specific rendering or size settings at this point (see SetContext()).
  */
  //@}

  /** @name Rendering context */
  //@{
  virtual void SetScale (float ScaleX, float ScaleY);
  /*!< Set pixel to screen unit ratio
    \param ScaleX abscissa scale
    \param ScaleY ordinate scale

    Called by nglFont whenever the font scaling is changed. The default
    implementation updates the \c mScaleX and \c mScaleY fields.
  */
  virtual void PushContext();  ///< Save GL context bits subject to be modified by SetContext()
  virtual void PopContext();   ///< Restore GL context saved by PushContext()
  virtual void SetContext();   ///< Set up GL context using current rendering settings
  //@}

  /** @name Rendering */
  //@{
  virtual bool Draw (const nglGlyphLayout& rGlyph) = 0;
  /*!< Draw a single glyph
    \param rGlyph glyph index and position

    This method should draw as efficiently as possible the given
    glyph (using its index and position). The glyph image can be
    retrieved via GetBitmapGlyph() and should be cached in
    the native format for the cache target. nglFontBase::GetBitmap8()
    might help.
  */
  //@}

protected:
  const nglFont* mpFont;
  float mScaleX, mScaleY;

  nglFontCache(const nglFontCache&) {} // Undefined copy constructor
};

#endif // __nglFontCache_h__
