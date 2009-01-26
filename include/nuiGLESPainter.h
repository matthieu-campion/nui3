/*
  NUI - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 SÈbastien MÈtrot

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __nuiGLESPainter_h__
#define __nuiGLESPainter_h__

//#include "nui.h"
#include "nuiDrawContext.h"

#ifndef __NUI_NO_GLES__

class NUI_API nuiGLESPainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiGLESPainter(nglContext* pContext, const nuiRect& rRect);
  virtual ~nuiGLESPainter();
  
  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY);
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(const nuiRenderArray& rArray);
  virtual void ClearColor();
  virtual void ClearStencil(uint8 value);
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nglMatrixf& rMatrix);
  virtual void MultMatrix(const nglMatrixf& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual uint32 GetRectangleTextureSupport() const;

  virtual void CreateTexture(nuiTexture* pTexture);
  virtual void DestroyTexture(nuiTexture* pTexture);
  virtual void InvalidateTexture(nuiTexture* pTexture, bool ForceReload);
  
  virtual void CreateSurface(nuiSurface* pSurface);
  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void InvalidateSurface(nuiSurface* pSurface, bool ForceReload);

  void Enable3DMode(bool set);
  bool Get3DMode() const;

protected:
  nglContext* mpContext;

  static uint32 mActiveContexts;

  virtual void ReleaseCacheObject(void* pHandle);

  uint32 mCanRectangleTexture;
  GLenum mTextureTarget;

  GLint mOldFramebuffer, mOldRenderbuffer;
  
  void ApplyTexture(const nuiRenderState& rState, bool ForceApply);
  
  class TextureInfo
  {
  public:
    TextureInfo();
    
    bool mReload;
    GLuint mTexture;
  };
  std::map<nuiTexture*, TextureInfo> mTextures;

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

  void ApplySurface(const nuiRenderState& rState, bool ForceApply);

  GLenum GetTextureTarget(bool POT) const;

  bool m3DMode;
public:
  void UploadTexture(nuiTexture* pTexture);
};

inline void nuiCheckForGLErrors()
{
#if 1 // Globally enable/disable OpenGL error checking
#ifdef _DEBUG_
  bool error = false;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR)
  {
    switch (err)
    {
      /*
      case GL_NO_ERROR:
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, "error has been recorded. The value of this symbolic constant is guaranteed to be zero.");
      */
      break;
    case GL_INVALID_ENUM: 
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_VALUE: 
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_OPERATION:
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_OVERFLOW:
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_UNDERFLOW:
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_OUT_OF_MEMORY:
      NGL_LOG(_T("nuiGLESPainter"), NGL_LOG_ALWAYS, _T("There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."));
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

#endif //   #ifndef __NUI_NO_GLES__

#endif //__nuiGLESPainter_h__

