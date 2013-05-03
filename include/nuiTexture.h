/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTexture_h__
#define __nuiTexture_h__

//#include "nui.h"
#include "nglImageCodec.h"
#include "nuiObject.h"

class nuiTexture;
class nuiPainter;
class nuiTextureCache;
class nuiSurface;

typedef std::map<nglString, nuiTexture*, nglString::LessFunctor> nuiTextureMap;
typedef std::set<nuiTextureCache*> nuiTextureCacheSet;

/// This class implements a basic image widget.
class NUI_API nuiTexture : public nuiObject
{
public:
  // Constructors and destructors are protected!
  static nuiTexture* GetTexture(nglIStream* pInput, nglImageCodec* pCodec = NULL); ///< Create an image from an input stream and a codec.  If \param pCodec is NULL all codecs will be tried on the image.
  static nuiTexture* GetTexture(const nglPath& rPath, nglImageCodec* pCodec = NULL ); ///< Create an image from a path and a codec. If \param pCodec is NULL all codecs will be tried on the image.
  static nuiTexture* GetTexture(nglImageInfo& rInfo, bool Clone = true); ///< Create an image from an nglImageInfo structure. If \param is true then the image buffer will be cloned, otherwise it will be deleted with the nuiTexture.
  static nuiTexture* GetTexture(const nglImage& rImage); ///< Create an image by copying an existing nglImage.
  static nuiTexture* GetTexture(nglImage* pImage, bool OwnImage); ///< Create an image from an existing nglImage. If \param OwnImage the nglImage object will be deleted with the nuiTexture.
  static nuiTexture* GetTexture(const nglString& rName); ///< Get a texture from its ID only
  static nuiTexture* GetTexture(const nglChar* pName); ///< Get a texture from its ID only
  static nuiTexture* GetAATexture(); ///< Returns an antialiasing texture for use with AAPrimitives.cpp
  static nuiTexture* BindTexture(GLuint TextureID, GLenum Target); ///< Returns a texture that will use an existing OpenGL Texture.
  static nuiTexture* CreateTextureProxy(const nglString& rName, const nglString& rSourceTextureID, const nuiRect& rProxyRect, bool RotatedToTheRight); ///< Create a proxy texture that is at subtexture in an atlas.
  static bool CreateAtlasFromPath(const nglPath& rPath, int32 MaxTextureSize, int32 ForceAtlasSize, bool Trim);
  
  static void ClearAll();
  static void ForceReloadAll(bool Rebind = false);

  void ForceReload(bool Rebind = false); ///< This method deletes the texture assiciated with the nuiTexture thus forcing its recreation at the next rendertime. If Rebind == false then we consider that the native (GL) texture was lost because the context/window have been destroyed and we have to completely recreate the texture.
  void ResetForceReload();
  void ImageToTextureCoord(nuiSize& x, nuiSize& y) const; ///< Transform the x,y point in the coordinates of the image to the coordinates of the texture. 
  void TextureToImageCoord(nuiSize& x, nuiSize& y) const; ///< Transform the x,y point in the coordinates of the texture to the coordinates of the image. 
  void ImageToTextureCoord(nuiAltSize& x, nuiAltSize& y) const;
  void TextureToImageCoord(nuiAltSize& x, nuiAltSize& y) const;
  void ImageToTextureCoord(nuiRect& rRect) const; ///< Transform the rRect rectangle in the coordinates of the image to the coordinates of the texture. 
  void TextureToImageCoord(nuiRect& rRect) const; ///< Transform the rRect rectangle in the coordinates of the texture to the coordinates of the image. 

  nglImage* GetImage() const; ///< Return a pointer to the nglImage contained in this object.
  void      ReleaseBuffer(); ///< Release the image source

  nuiSurface* GetSurface() const; ///< Return a pointer to the nuiSurface contained in this object.

  GLuint GetMinFilter() const; ///< Get the maximization filter
  GLuint GetMagFilter() const; ///< Get the minimization filter
  GLuint GetWrapS() const;     ///< Get the S coord wrap rule.
  GLuint GetWrapT() const;     ///< Get the T coord wrap rule.
  GLuint GetEnvMode() const;   ///< Get the environement mode.

  uint32 GetWidth() const;
  uint32 GetHeight() const;
  uint32 GetUnscaledWidth() const;
  uint32 GetUnscaledHeight() const;
  uint32 GetWidthPOT() const; ///< Returns the Unscaled Width of the texture rounded to the nearest bigger power of two.
  uint32 GetHeightPOT() const; ///< Returns the Unscaled Height of the texture rounded to the nearest bigger power of two.

  float GetScale() const;
  void SetScale(float scale);
  
  void SetMinFilter(GLuint Filter);  ///< Set the maximization filter
  void SetMagFilter(GLuint Filter);  ///< Set the minimization filter
  void SetWrapS(GLuint WrapS);       ///< Set the S coord wrap rule.
  void SetWrapT(GLuint WrapT);       ///< Set the T coord wrap rule.
  void SetEnvMode(GLuint Mode);      ///< Set the environement mode.
  void EnableAutoMipMap(bool Set);   ///< Enable the automatic creation of mip maps from the original texture.
  bool GetAutoMipMap() const;
  
  void SetRetainBuffer(bool Retain); ///< Set the nglImage destroying switch upon uploading to OpenGL

  bool SetSource(const nglString& rName); ///< Set the image source. Permits to set a global name for an image source instead of the automatically assigned one.
  nglString GetSource() const; ///< Retreive the source name of this texture.

  static void SetSharedContext(nglContext* pContext); ///< This method sets the shared context used to store texture over multiple renderers. It should be only called once per application.
  
  bool IsUptoDate() const { return !mForceReload; }
  bool IsPowerOfTwo() const; // Returns true if the width and the height of the texture are powers of two.
  bool IsBufferRetained() const { return mRetainBuffer; } ///< returns true if the texture doesn't try to destroy the image source upon uploading to OpenGL

  bool IsValid() const;
  
  static const nuiTextureMap& Enum();
  static nuiSimpleEventSource<0> TexturesChanged;
  
  
  static void AddCache(nuiTextureCache* pCache);
  static void DelCache(nuiTextureCache* pCache);
  
  static void RetainBuffers(bool Set);
  static void InitTextures();

  
  nglImagePixelFormat GetPixelFormat() const;
  
  GLuint GetTextureID() const;
  GLenum GetTarget() const;
  
  void SetTextureIdAndTarget(GLuint textureID, GLenum target);
  
  nuiTexture* GetProxyTexture() const;
  const nuiRect& GetProxyRect() const;
  
protected:
  friend class nuiSurface;
  static nuiTexture* GetTexture(nuiSurface* pSurface); ///< Create a texture from an existing nuiSurface.
  nuiTexture(nglIStream* pInput, nglImageCodec* pCodec = NULL); ///< Create an image from an input stream and a codec.  If \param pCodec is NULL all codecs will be tried on the image.
  nuiTexture(const nglPath& rPath, nglImageCodec* pCodec = NULL ); ///< Create an image from a path and a codec. If \param pCodec is NULL all codecs will be tried on the image.
  nuiTexture(nglImageInfo& rInfo, bool Clone = true); ///< Create an image from an nglImageInfo structure. If \param is true then the image buffer will be cloned, otherwise it will be deleted with the nuiTexture.
  nuiTexture(const nglImage& rImage); ///< Create an image by copying an existing nglImage.
  nuiTexture(nglImage* pImage, bool OwnImage); ///< Create an image from an existing nglImage. If \param OwnImage the nglImage object will be deleted with the nuiTexture.
  nuiTexture(nuiSurface* pSurface); ///< Create an image for rendering to surface
  nuiTexture(GLuint TextureID, GLenum Target); ///< Create an nuiTexture from an existing OpenGL texture ID
  nuiTexture(const nglString& rName, const nglString& rSourceTextureID, const nuiRect& rProxyRect, bool RotatedToTheRight); ///< Create a proxy texture that is at subtexture in an atlas.
  
  virtual ~nuiTexture();
  void Init();
  void InitAttributes();

  nglImage* mpImage;
  bool mOwnImage;

  nuiSurface* mpSurface;
  void ResizeSurface(int32 w, int32 h);

  nuiSize mRealWidth;
  nuiSize mRealHeight;
  nuiSize mRealWidthPOT;
  nuiSize mRealHeightPOT;
  bool mForceReload;
  bool mRetainBuffer;

  GLuint mMinFilter;
  GLuint mMagFilter;
  GLuint mWrapS;
  GLuint mWrapT;
  GLuint mEnvMode;
  bool mAutoMipMap;
  
  /// Beware, those two members are only used when a texture is created from an existing Texture ID/Target pair.
  GLuint mTextureID;
  GLenum mTarget;
  /// Beware, the two members above are only used when a texture is created from an existing Texture ID/Target pair.
  
  nuiRect mProxyRect;
  nuiTexture* mpProxyTexture;
  bool mRotated;
  
  float mScale;

  nglImagePixelFormat mPixelFormat;
  
  static nglContext* mpSharedContext;
  static nuiTextureMap mpTextures;
  static bool mRetainBuffers;
};



#endif // __nuiTexture_h__
