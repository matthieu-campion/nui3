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
  mpUIWindow = NULL;
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
  mDepthFormat = 0;
  mPixelFormat = GL_RGB565_OES;
	NSString* pixelFormat = kEAGLColorFormatRGB565;

///< Choose our pixel format
  if (  rInfo.FrameBitsR == 8 && rInfo.FrameBitsG == 8 && rInfo.FrameBitsB == 8 && rInfo.FrameBitsA == 8 &&
            rInfo.DepthBits == 0)
  {
    mPixelFormat = GL_RGBA8_OES;
		pixelFormat = kEAGLColorFormatRGBA8;
    mDepthFormat = 0;
  }
  else if (rInfo.FrameBitsR == 5 && rInfo.FrameBitsG == 6 && rInfo.FrameBitsB == 5 && rInfo.FrameBitsA == 0 &&
            rInfo.DepthBits == 0)
  {
    mPixelFormat = GL_RGB565_OES;
		pixelFormat = kEAGLColorFormatRGB565;
    mDepthFormat = 0;
  }
  else
  {
    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("could not find any acceptable Pixel Format compatible with: RGBA:%d:%d:%d:%d Depth:%d\n"), 
            rInfo.FrameBitsR, rInfo.FrameBitsG, rInfo.FrameBitsB, rInfo.FrameBitsA, rInfo.DepthBits);
    Destroy();
    return false;
  }

  NGL_LOG(_T("context"), NGL_LOG_INFO, _T("choosing GLES Pixel Format [%ls] compatible with: RGBA:%d:%d:%d:%d Depth:%d\n"),
          pixelFormat == kEAGLColorFormatRGBA8 ? _T("kEAGLColorFormatRGBA8") : _T("kEAGLColorFormatRGB565"),
          rInfo.FrameBitsR, rInfo.FrameBitsG, rInfo.FrameBitsB, rInfo.FrameBitsA, rInfo.DepthBits);


///< Set our layer
  NGL_ASSERT(mpUIWindow);
  CGRect rect = [(nglUIWindow*)mpUIWindow frame];
  mWidth  = rect.size.width;
  mHeight = rect.size.height;
  CAEAGLLayer* pLayer = (CAEAGLLayer*)[(nglUIWindow*)mpUIWindow layer];
  NGL_ASSERT(pLayer);  
	//pLayer.opaque = YES;
  BOOL retainBacking = rInfo.Offscreen ? YES : NO;
	[pLayer setDrawableProperties: 
    [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithBool:retainBacking],
                                                kEAGLDrawablePropertyRetainedBacking,
                                                pixelFormat,
                                                kEAGLDrawablePropertyColorFormat,
                                                nil
    ]
  ];
  
///< Create our context
  mpContext = NULL;
  if (pShared) {
    mpContext = [[EAGLContext alloc]  initWithAPI: kEAGLRenderingAPIOpenGLES1
                                      sharegroup:  [(EAGLContext*)pShared->mpContext sharegroup]];
  }
  else {
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

  if (!InternalMakeCurrent(mpContext)) {
    Destroy();
    return false;
  }
	
  CGSize newSize;
	newSize = [pLayer bounds].size;
	newSize.width = roundf(newSize.width);
	newSize.height = roundf(newSize.height);

	mWidth = newSize.width;
  mHeight = newSize.height;

	GLuint oldRenderbuffer;
	GLuint oldFramebuffer;
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	glGenRenderbuffersOES(1, &mRenderBuffer);
	glGenFramebuffersOES(1, &mFrameBuffer);
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderBuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFrameBuffer);
		
	[(EAGLContext*)mpContext renderbufferStorage: GL_RENDERBUFFER_OES fromDrawable: pLayer];  

	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, mRenderBuffer);

	if (mDepthFormat)
  {
		glGenRenderbuffersOES(1, &mDepthBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, mDepthBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, mDepthFormat, newSize.width, newSize.height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mDepthBuffer);
	}
	
	if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		NGL_OUT(_T("Error: failed to make complete framebuffer object\n"));
		return false;
	}

//  glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);

//  [(EAGLContext*) mpContext presentRenderbuffer:GL_RENDERBUFFER_OES];

/*
	if ( [(EAGLContext*) mpContext presentRenderbuffer:GL_RENDERBUFFER_OES] == NO )
	{
		NGL_OUT(_T("Error: Can't set Render Buffer Storage\n"));
		glDeleteRenderbuffersOES(1, &mRenderBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_BINDING_OES, oldRenderbuffer);
    Destroy();
		return false;
	}
*/

//nglString errorStr;
//GET_GL_ERROR(errorStr)

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


bool nglContext::InternalSwapBuffers() const
{
  bool res = true;

#ifndef __NOGLCONTEXT__

  EAGLContext* pContext = [EAGLContext currentContext];	

	if (pContext != mpContext) {
    if ( !InternalMakeCurrent(mpContext) ) {
      return false;
    }
  }

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRenderBuffer);	
	if ( ![(EAGLContext*)mpContext presentRenderbuffer: GL_RENDERBUFFER_OES] )
  {
		printf("Failed to swap renderbuffer in %s\n", __FUNCTION__);
    res = false;
  }

	if (pContext != mpContext) {
    InternalMakeCurrent((void*)pContext);
  }
  
#endif

  return res;
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
