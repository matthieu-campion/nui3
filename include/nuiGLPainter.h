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
  virtual void DrawArray(const nuiRenderArray& rArray);
  virtual void ClearColor();
  virtual void ClearStencil(uint8 value);
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
  virtual void SetSurface(nuiSurface* pSurface);

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);

  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);
  
protected:
  nglContext* mpContext;

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
  GLint mOldFramebuffer, mOldRenderbuffer;

  bool CheckFramebufferStatus();
  int32 GetAngle() const;
  int32 GetCurrentWidth() const;
  int32 GetCurrentHeight() const;
  void SetViewport();
};

inline void nuiCheckForGLErrors()
{
#if 0 // Globally enable/disable OpenGL error checking
#ifdef _DEBUG_
  bool error = false;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR)
  {
    switch (err)
    {
      /*
      case GL_NO_ERROR:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, "error has been recorded. The value of this symbolic constant is guaranteed to be zero.");
      */
      break;
    case GL_INVALID_ENUM: 
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_VALUE: 
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_OPERATION:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_OVERFLOW:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_UNDERFLOW:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_OUT_OF_MEMORY:
      NGL_LOG(_T("nuiGLPainter"), NGL_LOG_ALWAYS, _T("There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."));
      break;
    }
#ifdef _WIN32_
    __asm int 3;
#else
    NGL_ASSERT(0);
#endif
    err = glGetError();
  }
#endif
#endif
}

#endif //   #ifndef __NUI_NO_GL__

#endif //__nuiGLPainter_h__

