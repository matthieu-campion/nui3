/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglImage.h
\brief Generic multi-format image loader/exporter
*/

#ifndef __nglImage__
#define __nglImage__

#include "nui.h"
#include "nglError.h"
#include "nuiFlags.h"

class nglIStream;
class nglOStream;
class nglPath;

#if (defined _NOGFX_) || (defined _MINUI3_)
// Bare necessities extracted from <GL/gl.h>
  #define GL_COLOR_INDEX        0x1900
  #define GL_RGB                0x1907
  #define GL_RGBA               0x1908
  #define GL_BGR                0x80E0
  #define GL_BGRA               0x80E1
  #define GL_LUMINANCE          0x1909
  #define GL_LUMINANCE_ALPHA    0x190A
  #define GL_ALPHA              0x1906
#endif

#ifdef _OPENGL_ES_
  #define GL_COLOR_INDEX        0x1900
#endif

//! Internal nglImage buffer format
enum nglImageBufferFormat
{
  eImageFormatNone,  ///< Image buffer not available
  eImageFormatRaw,   ///< Plain linear image buffer (see nglImageInfo)
  eImageFormatS3TC,  ///< Buffer holds texture data in S3TC format
  eImageFormatDXTC   ///< Buffer holds texture data in DXTC format
};

//! Pixel components and bits
/*!
Describes the pixel format :
- the kind of information (palette index, RGB color space, luminance, alpha)
- the order and type of components
FIXME: don't we miss BGR and ABGR here ?
*/
enum nglImagePixelFormat
{
  eImagePixelNone,                        ///< Pixel format not available
  eImagePixelIndex = GL_COLOR_INDEX,      ///< The pixel is a palette cell index
  eImagePixelRGB   = GL_RGB,              ///< Red, green and blue
  eImagePixelRGBA  = GL_RGBA,             ///< Red, green, blue and alpha
#if (!defined NUI_IOS) && (!defined _ANDROID_)
  eImagePixelBGR   = GL_BGR,              ///< Blue, green and red
#endif
  eImagePixelLum   = GL_LUMINANCE,        ///< Luminance (gray level)
  eImagePixelAlpha = GL_ALPHA,            ///< Alpha (transparency level)
  eImagePixelLumA  = GL_LUMINANCE_ALPHA   ///< Luminance and alpha
};


//! Image description
/*!
The nglImageInfo class holds all the data and metadata (pixel format, size, etc)
of a nglImage object. You can setup a nglImageInfo of your own and build a nglImage
from it, or let nglImage do the job via a codec loader (see nglImage). Most parameters
should be straightforward for people used to pixel buffers representation.

There is two memory management modes : in managed mode, nglImageInfo 'owns' the pixel
buffer data (\a mpBuffer), and will automatically free this data when it is destroyed.
In unmanaged mode, nglImageInfo simply holds a reference to a buffer you manage
(in mpBuffer too), but will never attempt to release or re-allocate it.
*/

class NGL_API nglImageInfo
{
public:
  nglImageBufferFormat mBufferFormat;   ///< Buffer data format can be raw (user accessible) or proprietary (opaque to user, such as S3TC/DXTC)
  nglImagePixelFormat  mPixelFormat;    ///< Pixel components and respective components bit resolution
  uint32  mWidth;                       ///< Image width in pixels (0 if \a mpBuffer is NULL)
  uint32  mHeight;                      ///< Image height in pixels (0 if \a mpBuffer is NULL)
  uint32  mBitDepth;                    ///< Pixel bit depth (sum of components bit resolution, 0 if \a mpBuffer is NULL)
  uint32  mBytesPerPixel;               ///< Pixel allocation size in bytes (>= pixel bit depth, 0 if \a mpBuffer is NULL)
  uint32  mBytesPerLine;                ///< Pixel row allocation size in bytes (>= pixel size * image width, 0 if \a mpBuffer is NULL)
  bool    mPreMultAlpha;                ///< True if the buffer has it alpha value premultiplied in the color elements.
  char* mpBuffer;                       ///< Buffer data

  /** @name Life cycle */
  //@{
  nglImageInfo(uint32 width, uint32 height, uint32 bitdepth);
  /*!< Create a simple nglImageInfo with the corresponding width, height and bitdepth and allocate the buffer for these params. The buffer format is RGBA, RGB, Lum depending on the given bitdepth.
   \param width The width of the buffer
   \param height The width of the buffer
   \param bitdepth Number of bits per pixel. Can be 32 (RGBA8), 24 (RGB8), 16 (R5G5B5A1), 15 (RGB5), or 8 (Lum).
   */
  nglImageInfo (bool Managed = false);
  /*!<
    \param Managed choose memory management mode

    Please see nglImageInfo documentation for memory management.
  */
  nglImageInfo (const nglImageInfo& rInfo, bool Clone = true);
  /*!<
    \param rInfo image information source
    \param Clone if true, clones buffer data (ie. holds a managed copy). If false, only holds a reference to the source buffer data (and don't manage this memory)

    Please see nglImageInfo documentation for memory management.
  */
  ~nglImageInfo();
  /*!<
    Please see nglImageInfo documentation for memory management.
  */

  bool AllocateBuffer();
  /*!< Allocate buffer data according to image properties
    If the nglImageInfo object is in 'managed mode', free (allocated) buffer memory.

    Then allocates buffer data, ie. mBytesPerLine * mHeight bytes. The buffer is zero'ed.
    The buffer data automatically becomes managed (it will be released at destruction
    time).

    Please see nglImageInfo documentation for memory management.
  */
  //@}


protected:
  bool mOwnBuffer;  ///< Memory management mode (ie. only release mpBuffer if mOwnBuffer is true)

  void Transfert(nglImageInfo& rInfo);

  void Copy (const nglImageInfo& rInfo, bool Clone);
  /*!< Copy metadata and optionally buffer data
    \param rInfo image information source
    \param Clone if true, clones buffer data from rInfo.mpBuffer (ie. holds a managed copy). If false, only holds a reference to the source buffer data (and don't manage this memory)

    Please see nglImageInfo documentation for memory management.
    The nglImageInfo copy constructor is implemented like this :

\code
nglImageInfo::nglImageInfo (const nglImageInfo& rInfo, bool Clone)
{
  Copy (rInfo, Clone);
}
\endcode

    The nglImage::GetInfo() method only needs to copy metadata, here is its implementation :
\code
bool nglImage::GetInfo (nglImageInfo& rInfo) const
{
  rInfo.Copy (mInfo, false); // mInfo is nglImage's own image description
  return true;
}
\endcode
  */
  void ReleaseBuffer();
  /*!< Release buffer memory (only if it is managed by this instance)
    Please see nglImageInfo documentation for memory management.
  */

friend class nglImage;
};

class nglImageCodecInfo;
class nglImageCodec;


//! Generic multi-format image loader/exporter
/*!
A nglImage object is a container for image buffer data (using various pixel formats),
and also provides a plugin framework (the \a nglImageCodec codecs) to import from and
export to various file formats. Codecs can load data from a stream, making progressive
image loading possible. NGL is compiled with classic codecs (PNG, JPEG, TGA) as a
default, but you can choose to load them at run time, add your own codec, and so on.
*/
class NGL_API nglImage : public nglError
{
public:
  /** @name Life cycle */
  nglImage (nglIStream* pInput, nglImageCodec* pCodec = NULL);
  /*!< Create an image from a stream
    \param pInput input stream (feeding the codec)
    \param pCodec codec to use, NULL means auto-detection

    If auto-dection is used (\a pCodec set to NULL), nglImage will ask registered code
    to find their signature. If the signature check fails for all known codecs, codecs are
    asked to see if the stream name matches (actually check common file extensions). If
    a matching codec is still not present, an error will be set (use GetError()).

    If \a pCodec is a valid codec instance, it won't be managed, ie. it won't be destroy
    along the destruction of the nglImage object.

    When a matching codec is found, as soon as the header information is parsed, the OnInfo()
    callback will be invoked with valid data (image size, pixel format, etc). Then, OnData()
    and OnError() will be called respectively when more data was decoded or an error occured
    (interrupted stream or codec complaint for instance).
   */
   nglImage (const nglPath& rPath, nglImageCodec* pCodec = NULL );
  //@{
  /*!< Create an image from a file
    \param rPath input file
    \param pCodec codec to use, NULL means auto-detection

    If auto-dection is used (\a pCodec set to NULL), nglImage will ask registered code
    to find their signature. If the signature check fails for all known codecs, codecs are
    asked to see if the stream name matches (actually check common file extensions). If
    a matching codec is still not present, an error will be set (use GetError()).

    If \a pCodec is a valid codec instance, it won't be managed, ie. it won't be destroy
    along the destruction of the nglImage object.

    To provide a compatible behaviour with the streaming constructor of nglImage, OnInfo()
    and OnData() callbacks will be called (once, in this order). OnError() might be invoked
    as well if an error occurs.
  */
  nglImage(nglImageInfo& rInfo, nuiCopyPolicy policy = eClone);
  /*!< Create an image from a user given description
    \param rInfo image description
    \param policy if eClone, clones the buffer data and manage it. If eReference, copy the buffer pointer but do not give ownership. If eTransfert, copy the buffer pointer and give the ownership (the ownership is removed from the source)

    Create a nglImage object from a comprehensive description.

    If \a policy is eClone, nglImage makes a copy of rInfo.mpBuffer image data and owns it
    (ie. release this data memory when the nglImage object is destroyed).

    If \a policy is different from eClone nglImage only copies the rInfo.mpBuffer reference, and do \e not
    manage this memory (ie. it won't be released when the nglImage object is destroyed).
  */
  nglImage (const nglImage& rImage);
  /*!< Create an image copy from another image
    \param rImage source image

    The object will hold an exact copy of the source image description and buffer data.
    The image buffer data is cloned (and thus managed).
  */

  nglImage(const nglImage& rImage, uint32 NewWidth, uint32 NewHeight);
  /*!< Create an image copy from another image, scaling the source image to the given size
   \param rImage source image
   \param scaledWidth requested width
   \param scaledHeight requested height

   */


  virtual ~nglImage();
  //@}

  /** @name Image info helpers */
  //@{
  static bool GetImageInfo(nglImageInfo& rInfo, nglIStream* pInput, nglImageCodec* pCodec = NULL);
  static bool GetImageInfo(nglImageInfo& rInfo, const nglPath& rPath, nglImageCodec* pCodec = NULL );
  //@}

  /** @name Image description (info & data) */
  //@{
  bool  GetInfo (nglImageInfo& rInfo) const;
  /*!< Fetch image description
    \param rInfo holds image description data
    \return true if \a rInfo holds valid data, false otherwise

    Fill \a rInfo with the image description data.
  */
  nglImageBufferFormat GetBufferFormat() const;  ///< Internal buffer format. Returns eImageFormatNone if the info is not available.
  nglImagePixelFormat  GetPixelFormat() const;   ///< Internal pixel format. Returns eImagePixelNone if the info is not available.
  uint32  GetWidth() const;         ///< Image width in pixels. Returns 0 if the info is not available.
  uint32  GetHeight() const;        ///< Image height in pixels. Returns 0 if the info is not available.
  uint32  GetBitDepth() const;      ///< Pixel bit depth (sum of pixel components bit count). Returns 0 if the info is not available.
  uint32  GetPixelSize() const;     ///< Pixel allocation size in bytes (>= pixel bit depth). Returns 0 if the info is not available.
  uint32  GetBytesPerLine() const;  ///< Pixel row allocation size in bytes (>= pixel size * image width). Returns 0 if the info is not available.
  char* GetBuffer() const;        ///< Image buffer data. Returns NULL if the info is not available.

  bool IsValid() const;

  nglImageCodec* GetCodec() const;
  /*!< Fetch current codec
    \return current codec instance pointer

    If there is no codec in use (ie. import or export running), returns NULL.
  */
  //@}

  /** @name Export (save) */
  //@{
  bool Save (nglOStream* pOutput, nglImageCodec* pCodec );
  bool Save (const nglPath& rPath, nglImageCodec* pCodec );
  //@}


  nglImage* Resize(uint32 width, uint32 height);
  /*!< create a copy with a new size
   \param width new image width
   \param height new image height

   This method uses a simple bresenham algorithm. Should be improved in the future.
   */

  nglImage* Crop(uint32 x, uint32 y, uint32 width, uint32 height);
  /*!< create a copy, cropping the source
   \param x x-coord in the source image
   \param y y-coord in the source image
   \param width new image width
   \param height new image height

   return NULL if the coordinates or the new size goes outside the source image.
   */

  nglImage* Trim(int32& rXOffset, int32& rYOffset);
  /*!< create a copy, cropping the source so that no fully transparent lines or columns are left.
   \param rXOffset on output, contains the number of x pixels eaten from the left of the image during the operation.
   \param rYOffset on output, contains the number of y pixels eaten from the top of the image during the operation.

   return NULL if something bad happened.
   */

  nglImage* RotateLeft();
  /*!< create a copy, rotating the whole image to the left
   */

  nglImage* RotateRight();
  /*!< create a copy, rotating the whole image to the right
   */

  void PreMultiply(); ///< Premultiply the alpha in the image buffer
  void UnPreMultiply(); ///< Try to inverse the effect of PreMultiply.

  /** @name User callbacks */
  //@{
  virtual bool OnInfo (nglImageInfo& rInfo);
  /*!< Image description available
    \param rInfo image description

    This method is called as soon as the image description is available.
    From this point, image size is known and buffer data is properly allocated.
  */
  virtual bool OnData (float Completion);
  /*!< More data decoded
    \param Completion completion status, from 0 (nothing) to 1 (full image decoded)

    This method is called when a part of the source image data is decoded into the buffer.
  */
  virtual bool OnError();
  /*!<
    \return CanContinue Try to continue codec job when \e true, else abort

    This method is called whenever an error occurs into the codec. User can elect to
    continue (more errors can follow) when returning \e true or abort right now by
    returning \e false.
    FIXME: should retrieve error with GetCodec()->GetError()
  */
  //@}

  virtual void ReleaseBuffer();
  /*!< Wrapper to nglImageInfo Buffer Memory
    Please see nglImageInfo documentation for memory management.
  */

  friend class nglImageCodec;

  /** @name Global codec registry */
  //@{
  static bool AddCodec (nglImageCodecInfo* pCodecInfo);
  static bool DelCodec (nglImageCodecInfo* pCodecInfo);

  static nglImageCodec* CreateCodec (int32 Index);
  static nglImageCodec* CreateCodec (const nglString& rName);
  //@}

  class Color
  {
  public:
    float mRed, mGreen, mBlue, mAlpha;
  };

  Color GetPixel(uint32 X, uint32 Y) const;
  void GetPixel(uint32 X, uint32 Y, Color& rColor) const;
  void SetPixel(uint32 X, uint32 Y, const Color& rColor);

protected:
  bool OnCodecInfo (nglImageInfo& rInfo);  ///< The codec invoke this method as soon as all the image metadata is available
  bool OnCodecData (float Completion);     ///< The codec invoke this method whenever more data has been decoded to image buffer
  bool OnCodecError();                     ///< The codec invoke this method when an error occurs while encoding/decoding

  static void StaticInit();
  static void StaticExit();

  nglImageInfo mInfo;     ///< The image info.
  nglImageCodec* mpCodec; ///< The codec currently in use to load or save the image.
  bool mOwnCodec;         ///< false if the codec is supplied by user

  float mCompletion; ///< In between 0.0 and 1.0.

  virtual const nglChar* OnError (uint& rError) const;

private:
  static std::vector<nglImageCodecInfo*> *mpCodecInfos; ///< The list of image codec creator methods.
};


#endif // __nglImage_h__
