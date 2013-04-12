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
  nuiD3DPainter(nglContext* pContext);
  virtual ~nuiD3DPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
  void         DrawPrimitiveBatch();
  virtual void Clear(bool color, bool depth, bool stencil);
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix);
  virtual void MultProjectionMatrix(const nuiMatrix& rMatrix);
  virtual void PushProjectionMatrix();
  virtual void PopProjectionMatrix();

  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);

  virtual void CreateDeviceObjects();
  virtual void ReleaseDeviceObjects();

  virtual uint32 GetRectangleTextureSupport() const;

  nglContext* GetContext() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

  void        ApplyTextureFiltering(LPDIRECT3DDEVICE9 pDev, GLuint minfilter, GLuint magfilter);
  bool        IsTextureFormatSupported(LPDIRECT3DDEVICE9 pDev, D3DFORMAT format);
  void        SetDefaultRenderStates(LPDIRECT3DDEVICE9 pDev);

protected:
  void SetSurface(nuiSurface* pSurface);

  nglContext* mpContext;

  static uint32 mActiveContexts;

  void ApplyTexture(const nuiRenderState& rState, bool ForceApply);

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

  LPDIRECT3DVERTEXBUFFER9 mpVB;
  DWORD                   mnCurrentVBOffset;
  DWORD                   mnCurrentVBSize;

  DWORD                   mnBatchCurrentVBOffset;
  DWORD                   mnBatchCurrentVBSize;

  bool                    bInitialized;


  GLenum GetTextureTarget(bool POT) const;
  void UploadTexture(nuiTexture* pTexture);

  uint32 mCanRectangleTexture;

  // Surfaces:
  class FramebufferInfo
  {
  public:
    FramebufferInfo();
    
    bool mReload;
    LPDIRECT3DTEXTURE9 mpRenderTexture;
    LPDIRECT3DSURFACE9 mpRenderSurface;
  };
  std::map<nuiSurface*, FramebufferInfo> mFramebuffers;
  LPDIRECT3DSURFACE9 mpBackBuffer;
  void SetViewport();
};
#endif //   #ifndef __NUI_NO_D3D__

#endif //__nuiD3DPainter_h__

