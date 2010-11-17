/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiGLPainter_h__
#define __nuiGLPainter_h__

//#include "nui.h"
#include "nuiDrawContext.h"

// Disable GL support on OSX Classic...
#ifdef __NGL_CFM__
//  #define __NUI_NO_GL__
#endif

#ifndef __NUI_NO_GL__

class NUI_API nuiGLPainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiGLPainter(nglContext* pContext, const nuiRect& rRect);
  virtual ~nuiGLPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
  virtual void ClearColor();
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
  virtual uint32 GetRectangleTextureSupport() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);
  
protected:
  void SetSurface(nuiSurface* pSurface);
  nglContext* mpContext;

  void ApplyState(const nuiRenderState& rState, bool ForceApply);
  nuiRenderState mStateAccum;
  bool mForceApply;
  static uint32 mActiveContexts;

  virtual void ReleaseCacheObject(void* pHandle);

  uint32 mCanRectangleTexture;
  GLenum mTextureTarget;

  void ApplyTexture(const nuiRenderState& rState, bool ForceApply);
  
  class TextureInfo
  {
  public:
    TextureInfo();
    
    bool mReload;
    GLuint mTexture;
  };
  std::map<nuiTexture*, TextureInfo> mTextures;

  GLenum GetTextureTarget(bool POT) const;
  void UploadTexture(nuiTexture* pTexture);

  class FramebufferInfo
  {
  public:
    FramebufferInfo();
    
    bool mReload;
    GLuint mFramebuffer;
    GLuint mTexture; ///< the framebuffer can be used to render to a texture
    GLuint mRenderbuffer; ///< or a render buffer
    GLuint mDepthbuffer;
    GLuint mStencilbuffer;
  };
  std::map<nuiSurface*, FramebufferInfo> mFramebuffers;
  GLint mDefaultFramebuffer, mDefaultRenderbuffer;

  bool CheckFramebufferStatus();
  void SetViewport();
  
  int32 mScissorX;
  int32 mScissorY;
  int32 mScissorW;
  int32 mScissorH;
  bool mScissorOn;

  void BlendFuncSeparate(GLenum src, GLenum dst, GLenum srcalpha = GL_ONE, GLenum dstalpha = GL_ONE);
  bool mTwoPassBlend;
  GLenum mSrcColor;
  GLenum mDstColor;
  GLenum mSrcAlpha;
  GLenum mDstAlpha;
};

void nuiCheckForGLErrors();

#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGLPainter_h__

