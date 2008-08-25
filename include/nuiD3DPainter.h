/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiD3DPainter_h__
#define __nuiD3DPainter_h__

//#include "nui.h"
#include "nuiDrawContext.h"

// Disable GL support on OSX Classic...
#ifndef _WIN32_
  #define __NUI_NO_D3D__
#endif

#ifndef __NUI_NO_D3D__

class nuiD3DPainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiD3DPainter(nglContext* pContext, const nuiRect& rRect);
  virtual ~nuiD3DPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY);
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(const nuiRenderArray& rArray);
  virtual void ClearColor();
  virtual void ClearStencil(uint8 value);
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();

  nglContext* GetContext() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

protected:
  nglContext* mpContext;

  static uint32 mActiveContexts;

  virtual void ReleaseCacheObject(void* pHandle);
  //void DrawSmallArray(const nuiRenderArray& rArray);
  void LoadCurrentMatrix();

  class TextureInfo
  {
  public:
    TextureInfo();

    bool mReload;
    IDirect3DTexture9* mpTexture;
  };
  std::map<nuiTexture*, TextureInfo> mTextures;

  GLenum GetTextureTarget(bool POT) const;
  void UploadTexture(nuiTexture* pTexture);
};
#endif //   #ifndef __NUI_NO_D3D__

#endif //__nuiD3DPainter_h__

