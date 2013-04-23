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

class nuiGLDebugGuard
{
public:
  nuiGLDebugGuard(const nglString& rString)
  {
#ifdef DEBUG
    glPushGroupMarkerEXT(0, rString.GetChars());
#endif
  }

  nuiGLDebugGuard(const char* pString)
  {
#ifdef DEBUG
    glPushGroupMarkerEXT(0, pString);
#endif
  }

  ~nuiGLDebugGuard()
  {
#ifdef DEBUG
    glPopGroupMarkerEXT();
#endif
  }
};

class NUI_API nuiGLPainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiGLPainter(nglContext* pContext);
  virtual ~nuiGLPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
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
  virtual uint32 GetRectangleTextureSupport() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);
  
protected:
  virtual void ResetOpenGLState();
  void SetSurface(nuiSurface* pSurface);
  nglContext* mpContext;

  void ApplyState(const nuiRenderState& rState, bool ForceApply);
  nuiRenderState mFinalState;
  bool mForceApply;
  static uint32 mActiveContexts;

  virtual void ReleaseCacheObject(void* pHandle);

  uint32 mCanRectangleTexture;
  GLenum mTextureTarget;

  void ApplyTexture(const nuiRenderState& rState, bool ForceApply, int slot);
  
  class TextureInfo
  {
  public:
    TextureInfo();
    
    bool mReload;
    GLint mTexture;
  };
  std::map<nuiTexture*, TextureInfo> mTextures;

  GLenum GetTextureTarget(bool POT) const;
  void UploadTexture(nuiTexture* pTexture, int slot);

  class FramebufferInfo
  {
  public:
    FramebufferInfo();
    
    bool mReload;
    GLint mFramebuffer;
    GLint mTexture; ///< the framebuffer can be used to render to a texture
    GLint mRenderbuffer; ///< or a render buffer
    GLint mDepthbuffer;
    GLint mStencilbuffer;
  };
  std::map<nuiSurface*, FramebufferInfo> mFramebuffers;
  GLint mDefaultFramebuffer, mDefaultRenderbuffer;

  bool CheckFramebufferStatus();
  virtual void SetViewport();
  
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

  // Local cache:
  bool mClientVertex;
  bool mClientColor;
  bool mClientTexCoord;
  bool mMatrixChanged;
  float mR;
  float mG;
  float mB;
  float mA;
  GLenum mTexEnvMode;
  
  uint32 mViewPort[4];

  bool mUseShaders;

  // Only used for shaders:
  nglVector2f mTextureTranslate;
  nglVector2f mTextureScale;
};

bool nuiCheckForGLErrorsReal();
#define nuiCheckForGLErrors() { NGL_ASSERT(nuiCheckForGLErrorsReal()); }

#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGLPainter_h__

