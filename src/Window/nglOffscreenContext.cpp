/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

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

/*!
\file  nglOffscreenContext.cpp
\brief OpenGL offscreen context management (=> pbuffer extension on most platforms)

This classes is not available if the _NOGFX_ symbol is defined.
*/

#include "nui.h"
#include "nglContext.h"
#include "nglKernel.h"
#include <vector>

#define NGL_OFFSCREEN_CONTEXT_ENOMATCH   1
#define NGL_OFFSCREEN_CONTEXT_ENOSUPPORT    2
#define NGL_OFFSCREEN_CONTEXT_EGLCTX     3
#define NGL_OFFSCREEN_CONTEXT_EBIND      4
#define NGL_OFFSCREEN_CONTEXT_ECANTSHARE      5

const nglChar* gpOffscreenContextErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("The hardware doesn't support offscreen contexts"),
/*  2 */ _T("Cound't set pixel format"),
/*  3 */ _T("GL rendering context creation failed"),
/*  4 */ _T("Couldn't bind GL context"),
/*  5 */ _T("Couldn't share GL contexts"),
         NULL
};


#ifdef _WIN32_
PFNWGLCREATEPBUFFERARBPROC    nglOffscreenContext::wglCreatePbufferARB    = NULL;
PFNWGLGETPBUFFERDCARBPROC     nglOffscreenContext::wglGetPbufferDCARB     = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC nglOffscreenContext::wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC   nglOffscreenContext::wglDestroyPbufferARB   = NULL;
PFNWGLQUERYPBUFFERARBPROC     nglOffscreenContext::wglQueryPbufferARB     = NULL;
PFNWGLBINDTEXIMAGEARBPROC     nglOffscreenContext::wglBindTexImageARB     = NULL;//    BOOL wglBindTexImageARB (HPBUFFERARB hPbuffer, int iBuffer)
PFNWGLRELEASETEXIMAGEARBPROC  nglOffscreenContext::wglReleaseTexImageARB  = NULL;//    BOOL wglReleaseTexImageARB (HPBUFFERARB hPbuffer, int iBuffer)
PFNWGLSETPBUFFERATTRIBARBPROC nglOffscreenContext::wglSetPbufferAttribARB = NULL;//    BOOL wglSetPbufferAttribARB (HPBUFFERARB hPbuffer, const int *piAttribList) 

#endif

nglOffscreenContext::nglOffscreenContext(uint32 Width, uint32 Height, bool RenderToTexture, nglContext* pShared)
{
#ifdef _WIN32_
  mPBuffer = NULL;
#endif
  mWidth = Width;
  mHeight = Height;
  nglContextInfo Info;
  Info.Offscreen = true;
  Info.RenderToTexture = RenderToTexture;
  Build(Info, pShared);
}

nglOffscreenContext::nglOffscreenContext(const nglContextInfo& rInfo, uint32 Width, uint32 Height, nglContext* pShared)
{
  mWidth = Width;
  mHeight = Height;
#ifdef _WIN32_
  mPBuffer = NULL;
#endif
  nglContextInfo Info(rInfo);
  Info.Offscreen = true;
  Build(rInfo, pShared);
}

nglOffscreenContext::~nglOffscreenContext()
{
#ifdef _WIN32_
  wglReleasePbufferDCARB(mPBuffer,mDC);
  wglDestroyPbufferARB(mPBuffer);
#endif // _WIN32_
#ifdef _CARBON_
  aglDestroyPBuffer(mPBuffer);
#endif
}


void nglOffscreenContext::BeginSession()
{
  MakeCurrent();
#ifdef _WIN32_
  std::vector<int> mAttribs;
  mAttribs.push_back(WGL_MIPMAP_LEVEL_ARB);
  mAttribs.push_back(0);

  mAttribs.push_back(0);
  wglSetPbufferAttribARB(mPBuffer, &mAttribs[0]);
#endif // _WIN32_
#ifdef _CARBON_
  bool res = true;
	MakeCurrent();  
#endif
}

bool nglOffscreenContext::MakeCurrent() const
{
  bool res = false;
#ifndef __NOGLCONTEXT__
#ifdef __APPLE__
  res = aglSetCurrentContext(mCtx) != 0;
  GLboolean result = aglSetPBuffer(mCtx, mPBuffer, 0, 0, 0);
  cgl_ctx = CGLGetCurrentContext();  
#endif
#endif
  return res;
}

void nglOffscreenContext::EndSession()
{
#ifdef _WIN32_
  wglMakeCurrent(NULL,NULL);
#endif
#ifdef _CARBON_
  #ifndef __NOGLCONTEXT__
    aglSwapBuffers(mCtx);
    aglSetCurrentContext (NULL);
#endif
#endif
}

bool nglOffscreenContext::Build(const nglContextInfo& rInfo, nglContext* pShared)
{
#ifdef _WIN32_
  if ( !wglCreatePbufferARB
    && !wglGetPbufferDCARB 
    && !wglReleasePbufferDCARB 
    && !wglDestroyPbufferARB 
    && !wglQueryPbufferARB 
    && !wglBindTexImageARB 
    && !wglReleaseTexImageARB 
    && !wglSetPbufferAttribARB)
  {
    wglCreatePbufferARB     = (PFNWGLCREATEPBUFFERARBPROC)    wglGetProcAddress("wglCreatePbufferARB");
    wglGetPbufferDCARB      = (PFNWGLGETPBUFFERDCARBPROC)     wglGetProcAddress("wglGetPbufferDCARB");
    wglReleasePbufferDCARB  = (PFNWGLRELEASEPBUFFERDCARBPROC) wglGetProcAddress("wglReleasePbufferDCARB");
    wglDestroyPbufferARB    = (PFNWGLDESTROYPBUFFERARBPROC)   wglGetProcAddress("wglDestroyPbufferARB");
    wglQueryPbufferARB      = (PFNWGLQUERYPBUFFERARBPROC)     wglGetProcAddress("wglQueryPbufferARB");
    wglBindTexImageARB      = (PFNWGLBINDTEXIMAGEARBPROC)     wglGetProcAddress("wglBindTexImageARB");
    wglReleaseTexImageARB   = (PFNWGLRELEASETEXIMAGEARBPROC)  wglGetProcAddress("wglReleaseTexImageARB");
    wglSetPbufferAttribARB  = (PFNWGLSETPBUFFERATTRIBARBPROC) wglGetProcAddress("wglSetPbufferAttribARB");

    if ( !wglCreatePbufferARB 
      || !wglGetPbufferDCARB 
      || !wglReleasePbufferDCARB 
      || !wglDestroyPbufferARB  
      || !wglQueryPbufferARB  
      || !wglBindTexImageARB 
      || !wglReleaseTexImageARB 
      || !wglSetPbufferAttribARB)
    {
      SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_ENOSUPPORT);
      return false;
    }
  }

  int pixelformat = rInfo.GetPFD(App->GetDC());
  if (pixelformat == 0) 
  {
    SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_ENOMATCH);
    return false; //throw("wglChoosePixelFormatARB() failed");
  }

  int pattrib[] = { WGL_TEXTURE_FORMAT_ARB,
                    rInfo.FrameBitsA != 0 ? WGL_TEXTURE_RGBA_ARB : WGL_TEXTURE_RGB_ARB,
                    WGL_TEXTURE_TARGET_ARB,
                    WGL_TEXTURE_2D_ARB,
                    0
                    };
  if (!rInfo.RenderToTexture)
    pattrib[0] = 0;

  mPBuffer = wglCreatePbufferARB(App->GetDC(),pixelformat,mWidth,mHeight,pattrib);
  if (!mPBuffer)
  {
    SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_EGLCTX);
    return false; //throw("wglCreatePbufferARB() failed");
  }

  mDC = wglGetPbufferDCARB(mPBuffer);
  if(!mDC) 
  {
    SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_EGLCTX);
    return false; //throw("wglGetPbufferDCARB() failed");
  }

  mRC = wglCreateContext(mDC);
  if(!mRC) 
  {
    SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_EGLCTX);
    return false; //throw("wglCreateContext() failed");
  }

  // Query the actual size of the pbuffer that was created:
  wglQueryPbufferARB(mPBuffer, WGL_PBUFFER_WIDTH_ARB, (int*)&mWidth);
  wglQueryPbufferARB(mPBuffer, WGL_PBUFFER_HEIGHT_ARB, (int*)&mHeight);

  if (pShared)
  {
    if (!ShareWith(pShared))
    {
      SetError("offscreen_context", NGL_OFFSCREEN_CONTEXT_ECANTSHARE);
      return false; //throw("wglShareLists() failed");
    }
  }
#endif // _WIN32_
#ifdef _CARBON_
  std::vector<GLint> attribs;
  attribs.push_back(AGL_RGBA);
/*
  if (rInfo.FrameCnt == 2)
    attribs.push_back(AGL_DOUBLEBUFFER);
*/
  
  attribs.push_back(AGL_PBUFFER);
  
  attribs.push_back(AGL_DEPTH_SIZE);
  attribs.push_back(rInfo.DepthBits);

  attribs.push_back(AGL_STENCIL_SIZE);
  attribs.push_back(rInfo.StencilBits);

/*
  if (rInfo.AuxCnt)
  {
    attribs.push_back(AGL_AUX_BUFFERS);
    attribs.push_back(rInfo.AuxCnt);
  }
  */
  
  if (rInfo.AABufferCnt)
  {
    attribs.push_back(AGL_SAMPLE_BUFFERS_ARB);
    attribs.push_back(rInfo.AABufferCnt);
    attribs.push_back(AGL_SAMPLES_ARB);
    attribs.push_back(rInfo.AASampleCnt);
  }

  attribs.push_back(AGL_RED_SIZE);
  attribs.push_back(rInfo.FrameBitsR);
  attribs.push_back(AGL_GREEN_SIZE);
  attribs.push_back(rInfo.FrameBitsG);
  attribs.push_back(AGL_BLUE_SIZE);
  attribs.push_back(rInfo.FrameBitsB);
  attribs.push_back(AGL_ALPHA_SIZE);
  attribs.push_back(rInfo.FrameBitsA);
  attribs.push_back(AGL_PIXEL_SIZE);
  attribs.push_back(rInfo.FrameBitsR + rInfo.FrameBitsG + rInfo.FrameBitsB + rInfo.FrameBitsA);

  attribs.push_back(AGL_ACCUM_RED_SIZE);
  attribs.push_back(rInfo.AccumBitsR);
  attribs.push_back(AGL_ACCUM_GREEN_SIZE);
  attribs.push_back(rInfo.AccumBitsG);
  attribs.push_back(AGL_ACCUM_BLUE_SIZE);
  attribs.push_back(rInfo.AccumBitsB);
  attribs.push_back(AGL_ACCUM_ALPHA_SIZE);
  attribs.push_back(rInfo.AccumBitsA);

  if (rInfo.Stereo) 
    attribs.push_back(AGL_STEREO);

  attribs.push_back(AGL_MAXIMUM_POLICY);

  attribs.push_back(AGL_NONE);
  /* Choose pixel format */
  
#ifdef __NGL_MACHO__
  GDHandle gdhDisplay;
  CGDirectDisplayID DisplayID = CGMainDisplayID();
  DMGetGDeviceByDisplayID ( (DisplayIDType)DisplayID, &gdhDisplay, false);
  AGLPixelFormat Format = aglChoosePixelFormat(&gdhDisplay, 1, &attribs[0]);
#else
  AGLPixelFormat Format = aglChoosePixelFormat(NULL, 0, &attribs[0]);
#endif

  mCtx = aglCreateContext(Format, pShared?pShared->mCtx:NULL);
  if (!mCtx)
    return false;
  aglDestroyPixelFormat(Format);
  
  GLenum internalFormat = rInfo.FrameBitsA != 0 ? GL_RGBA : GL_RGB;
                    
  GLboolean result = aglCreatePBuffer(mWidth, mHeight, GL_TEXTURE_2D, internalFormat, 0, &mPBuffer);
  if (result == GL_FALSE) 
    return false; 
  {
    GLenum target, internalFormat;
    GLint max_level;
  
    // Query the actual size of the pbuffer that was created:
    aglDescribePBuffer(mPBuffer, (GLint*)&mWidth, (GLint*)&mHeight, &target, &internalFormat, &max_level);
  }

#endif // _CARBON


  return true;
}

bool nglOffscreenContext::BindTexture()
{
#ifdef _WIN32_
  return 0 != wglBindTexImageARB(mPBuffer, WGL_FRONT_LEFT_ARB);
#endif // _WIN32_
#ifdef _CARBON_
  GLboolean result = aglTexImagePBuffer(aglGetCurrentContext(), mPBuffer, GL_FRONT_LEFT);
  return result;
#endif // _CARBON_
  return false;

}

bool nglOffscreenContext::UnbindTexture()
{
#ifdef _WIN32_
  return 0 != wglReleaseTexImageARB(mPBuffer, WGL_FRONT_LEFT_ARB);
#endif // _WIN32_
#ifdef _CARBON_
  return true;
#endif // _CARBON_
  return false;
}

