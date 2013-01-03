/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiImage_h__
#define __nuiImage_h__

//#include "nui.h"
#include "nglImage.h"
#include "nuiTexture.h"
#include "nuiWidget.h"
#include "nuiRect.h"
#include "nuiColor.h"
#include "nuiDrawContext.h"

/// This class implements a basic image widget.
class NUI_API nuiImage : public nuiWidget
{
public:
  explicit nuiImage (nuiTexture* pTexture = NULL, bool TextureAlreadyAcquired = false); ///< Create an image from an allready existing texture object.
  explicit nuiImage (nglIStream* pInput, nglImageCodec* pCodec = NULL); ///< Create an image from an input stream and a codec.  If \param pCodec is NULL all codecs will be tried on the image.
  nuiImage (const nglPath& rPath, nglImageCodec* pCodec = NULL ); ///< Create an image from a path and a codec. If \param pCodec is NULL all codecs will be tried on the image.
  explicit nuiImage (nglImageInfo& rInfo, bool Clone = true); ///< Create an image from an nglImageInfo structure. If \param is true then the image buffer will be cloned, otherwise it will be deleted with the nuiImage.
  explicit nuiImage (const nglImage& rImage); ///< Create an image by copying an existing nglImage.
  explicit nuiImage (nglImage* pImage, bool OwnImage); ///< Create an image from an existing nglImage. If \param OwnImage the nglImage object will be deleted with the nuiImage.
  
  void InitAttributes();
  
  const nglPath& GetTexturePath();
  void SetTexturePath(const nglPath& rTexturePath);
  void SetTexture(nuiTexture* pTex);

  void SetTextureRect(const nuiRect& rRect);
  const nuiRect& GetTextureRect() const;
  void ResetTextureRect();
  
  virtual ~nuiImage();

  virtual void ForceReload(); ///< This method deletes the texture associated with the nuiImage thus forcing its recreation at the next rendertime.
  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  
  void SetBlendFunc(nuiBlendFunc Func);
  virtual nuiBlendFunc GetBlendFunc(); ///< Get the current blending function to use while rendering to compose the image with the background. The default setting is to alpha blend the image with the current background.

  virtual void SetUseAlpha(bool Alpha); ///< If \param Alpha is true the the rendering of the image will enable alpha blending. Alpha blending is enabled by default if an nuiImage contains an alpha channel.
  virtual bool GetUseAlpha(); ///< Return true if the image rendering uses alpha blending.
  virtual nglImage* GetImage(); ///< Return a pointer to the nglImage contained in this object's nuiTexture.
  virtual nuiTexture* GetTexture(); ///< Return a pointer to the nuiTexture contained in this object.

  void SetColor(const nuiColor& rColor);
  const nuiColor& GetColor() const;
  
protected:
  nuiTexture* mpTexture;
  nuiRect mTextureRect;
  nglPath mTexturePath;
  bool mUseAlpha;

  nuiBlendFunc mBlendFunc;
  
  nuiColor mColor;
};

#endif // __nuiImage_h__
