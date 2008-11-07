#include "nui.h"
#include "nglKernel.h"
#include "nglContext.h"


#include <UIKit/UIKit.h>
#include "nglWindow_UIKit.h"

#include <OpenGLES/EAGL.h>
//#include <OpenGLES/EAGLDisplay.h>
#include <OpenGLES/EAGLDrawable.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <QuartzCore/QuartzCore.h>

#include "ngl_uikit.h"

//#define _DEBUG_CONTEXT_
/*
 * nglContextInfo
 */

bool nglContextInfo::Enum(uint Index, nglContextInfo& rInfo)
{
/*
  if (!nglContext::mEGLInitialized)
    return false;
  EGLDisplay display = eglGetDisplay(0);

  EGLint numConfigs;
  if (!eglGetConfigs(display, 0, 0, &numConfigs))
    return false;
  NGL_OUT(_T("%d egl configs\n"), numConfigs);
  if (Index >= numConfigs)
    return false;

  const uint maxConf = (Index+1);
  EGLConfig configs[maxConf];
  eglGetConfigs(display, configs, maxConf, &numConfigs);

  EGLint redBits;
  EGLint greenBits;
  EGLint blueBits;
  EGLint alphaBits;

  EGLint bufferSize;
  EGLint depthSize;
  EGLint stencilSize;

  EGLint caveat;
  EGLint native;

  EGLint level;
  EGLint surfaceType;
  EGLint visualType;
  EGLint visualID;

  eglGetConfigAttrib(display, configs[Index], EGL_RED_SIZE,    &redBits);
  eglGetConfigAttrib(display, configs[Index], EGL_GREEN_SIZE,  &greenBits);
  eglGetConfigAttrib(display, configs[Index], EGL_BLUE_SIZE,   &blueBits);
  eglGetConfigAttrib(display, configs[Index], EGL_ALPHA_SIZE,  &alphaBits);

  eglGetConfigAttrib(display, configs[Index], EGL_BUFFER_SIZE, &bufferSize);
  eglGetConfigAttrib(display, configs[Index], EGL_DEPTH_SIZE,  &depthSize);
  eglGetConfigAttrib(display, configs[Index], EGL_STENCIL_SIZE, &stencilSize);
  eglGetConfigAttrib(display, configs[Index], EGL_CONFIG_CAVEAT, &caveat);
  eglGetConfigAttrib(display, configs[Index], EGL_NATIVE_RENDERABLE, &native);

  eglGetConfigAttrib(display, configs[Index], EGL_LEVEL, &level);
  eglGetConfigAttrib(display, configs[Index], EGL_SURFACE_TYPE, &surfaceType);
  eglGetConfigAttrib(display, configs[Index], EGL_NATIVE_VISUAL_TYPE, &visualType);
  eglGetConfigAttrib(display, configs[Index], EGL_NATIVE_VISUAL_ID, &visualID);
  
  rInfo.FrameCnt=2; ///< Number of frame buffers (two means double-buffering)
  rInfo.FrameBitsR=redBits;
  rInfo.FrameBitsG=greenBits;
  rInfo.FrameBitsB=blueBits;
  rInfo.FrameBitsA=alphaBits;
  rInfo.DepthBits=depthSize;
  rInfo.StencilBits=stencilSize;
*/
/*
  rInfo.AccumBitsR=0;
  rInfo.AccumBitsG=0;
  rInfo.AccumBitsB=0;
  rInfo.AccumBitsA=0;
  rInfo.AuxCnt=0;
  rInfo.AABufferCnt=0;
  rInfo.AASampleCnt=0;
  rInfo.Stereo=0;
  rInfo.Offscreen=0;
  rInfo.RenderToTexture=0;
  rInfo.CopyOnSwap=0;
*/
  rInfo.TargetAPI = eTargetAPI_OpenGLES;
/*
  NGL_OUT(
    _T("config %d: rgba=%d:%d:%d:%d(%d), ds=%d:%d, caveat=%04x, native=%d, level=%d, surface=%04x, visual=%04x, id=%d\n"),
    Index, redBits, greenBits, blueBits, alphaBits, bufferSize, depthSize, stencilSize, caveat, native, level, surfaceType, visualType, visualID
  );
*/
  return true;
}

#define GET_GL_ERROR(str)\
{\
  GLenum __error = glGetError();\
  if(__error) {\
    str.CFormat(_T("OpenGL error 0x%04X in %s\n"), __error, __FUNCTION__);\
  }\
  else {\
    str = _T("No Error\n");\
  }\
}

const nglChar* gpContextErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("No context match your request"),
/*  2 */ _T("GL rendering context creation failed"),
/*  3 */ _T("Couldn't bind GL context to the system window"),
         NULL
};

#if 0
const nglChar* gpEAGLErrorTable[] =
{
/* kEAGLErrorSuccess */           _T("No error"),
/* kEAGLErrorInvalidParameter */  _T("Function parameter is not legal"),
/* kEAGLErrorInvalidOperation */  _T("Requested operation is not legal"),
/* kEAGLErrorOutOfMemory */       _T("Implementation ran out of memory"),
                                  NULL
};
#endif

/*
 * nglContext
 */

nglContext::nglContext()
{
  mpContext = NULL;
}

nglContext::~nglContext()
{
  Destroy();
}

bool nglContext::GetContextInfo(nglContextInfo& rInfo) const
{
  rInfo = mContextInfo;
  return true;
}

bool nglContext::Build(const nglContextInfo& rInfo, const nglContext* pShared, bool FullScreen)
{
  mTargetAPI = rInfo.TargetAPI;
  if (mTargetAPI != eTargetAPI_OpenGLES)
    return false;

#ifndef __NOGLCONTEXT__

  mFullscreen = FullScreen;
//  uint32 frameBits = (rInfo.FrameBitsR + rInfo.FrameBitsG + rInfo.FrameBitsB + rInfo.FrameBitsA);

///< defaults
  mDepthFormat = 0;//rInfo.DepthBits;
  mPixelFormat = GL_RGB565_OES;
	NSString* pixelFormat = kEAGLColorFormatRGB565;

///< Choose our pixel format

  if (rInfo.FrameBitsR == 8 &&
      rInfo.FrameBitsG == 8 &&
      rInfo.FrameBitsB == 8 &&
      rInfo.FrameBitsA == 8)
  {
    mPixelFormat = GL_RGBA8_OES;
		pixelFormat = kEAGLColorFormatRGBA8;
  }
  else if (rInfo.FrameBitsR == 5 &&
           rInfo.FrameBitsG == 6 &&
           rInfo.FrameBitsB == 5 &&
           rInfo.FrameBitsA == 0)
  {
    mPixelFormat = GL_RGB565_OES;
		pixelFormat = kEAGLColorFormatRGB565;
  }
  else
  {
    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("could not find any acceptable Pixel Format compatible with: RGBA:%d:%d:%d:%dn"), 
            rInfo.FrameBitsR, rInfo.FrameBitsG, rInfo.FrameBitsB, rInfo.FrameBitsA);
    Destroy();
    return false;
  }

  NGL_LOG(_T("context"), NGL_LOG_INFO, _T("choosing GLES Pixel Format [%ls] compatible with: RGBA:%d:%d:%d:%d\n"),
          pixelFormat == kEAGLColorFormatRGBA8 ? _T("kEAGLColorFormatRGBA8") : _T("kEAGLColorFormatRGB565"),
          rInfo.FrameBitsR, rInfo.FrameBitsG, rInfo.FrameBitsB, rInfo.FrameBitsA);

  mpEAGLPixelFormat = pixelFormat;

///< Create our context

  mpContext = NULL;
  if (pShared)
  {
    mpContext = [[EAGLContext alloc]  initWithAPI: kEAGLRenderingAPIOpenGLES1
                                      sharegroup:  [(EAGLContext*)pShared->mpContext sharegroup]];
  }
  else
  {
    mpContext = [[EAGLContext alloc]  initWithAPI: kEAGLRenderingAPIOpenGLES1];
  }
  NGL_ASSERT(mpContext);
  if (!mpContext)
  {
    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("[EAGLContext %ls] Failed to Create context %ls\n"),
            pShared ? _T("initWithAPI:sharegroup:") : _T("initWithAPI:"), _T("Failed"));
    Destroy();
    return false;
  }

  if (!InternalMakeCurrent(mpContext))
  {
    Destroy();
    return false;
  }

#endif
  return true;
}

bool nglContext::InternalMakeCurrent(void* pContext) const
{
#ifndef __NOGLCONTEXT__

  if ( ![EAGLContext setCurrentContext: (EAGLContext*)pContext] )
  {
    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("EAGLContext setCurrentContext: %p: [error]\n"), pContext);
    return false;
  }

# ifdef _DEBUG_CONTEXT_
  NGL_LOG(_T("context"), NGL_LOG_INFO, _T("EAGLContext setCurrentContext: %p: [succeeded]\n"), pContext);
# endif

#endif
  return true;
}

const nglChar* nglContext::OnError (uint& rError) const
{
  return FetchError(gpContextErrorTable, NULL, rError);
}

// -------------------------
// TODO: Query Exts
// -------------------------

nglContext::GLExtFunc nglContext::LookupExtFunc (const char* pFuncName)
{
  return NULL;//(GLExtFunc)eaglGetProcAddress(pFuncName);
}

bool nglContext::Destroy()
{
#ifndef __NOGLCONTEXT__
  InternalMakeCurrent(NULL);
  if (mpContext)
  {
    [(EAGLContext*)mpContext release];
  }
  
  mpContext = NULL;
#endif
	return true;
}
