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

#include "nui.h"
#include "nglKernel.h"
#include "nglContext.h"

#include "ngl_unix.h"


/*
#define NGL_CONTEXT_ENONE      0
*/
#define NGL_CONTEXT_ENOX       1
#define NGL_CONTEXT_ENOGLX     2
#define NGL_CONTEXT_ENOMATCH   3
#define NGL_CONTEXT_EGLCTX     4
#define NGL_CONTEXT_EBIND      5


#if NGL_CONTEXT_ELAST != 5
#error "Please sync NGL_CONTEXT_ELAST in src/core/unix/ngl_unix.h accordingly"
#endif

const nglChar* gpContextErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("No X connection"),
/*  2 */ _T("GLX extension not found"),
/*  3 */ _T("No visual match your request"),
/*  4 */ _T("GL rendering context creation failed"),
/*  5 */ _T("Couldn't bind GL context to the system window"),
         NULL
};


/*
 * nglContextInfo
 */

XVisualInfo* nglContextInfo::mpXVisualInfoList   = NULL;
uint         nglContextInfo::mXVisualInfoListCnt = 0;


bool nglContextInfo::Enum (uint Index, nglContextInfo& rInfo)
{
  if (!Init() ||
      Index >= mXVisualInfoListCnt)
    return false;

  rInfo = nglContextInfo((Display*)App->GetDisplay(), &(mpXVisualInfoList[Index]));
  return true;
}


bool nglContextInfo::Init()
{
  if (!mpXVisualInfoList)
  {
    Display* display = (Display*)App->GetDisplay();
    XVisualInfo ref;
    long mask;
    int n;

    ref.screen = DefaultScreen (display);
    ref.c_class = TrueColor;
    mask = VisualScreenMask | VisualClassMask;
    mpXVisualInfoList = XGetVisualInfo (display, mask, &ref, &n);
    mXVisualInfoListCnt = n;

    App->AddExit(Exit);
  }

  return mpXVisualInfoList != NULL;
}

void nglContextInfo::Exit()
{
  if (mpXVisualInfoList)
    XFree(mpXVisualInfoList);
}


#define GLX_GET(attrib, value) value = glXGetConfig(pDisplay, pXVisualInfo, attrib, &x) ? 0 : x;

nglContextInfo::nglContextInfo (Display* pDisplay, XVisualInfo* pXVisualInfo)
{
  int x, temp;

  mpXVisualInfo = pXVisualInfo;

  GLX_GET(GLX_DOUBLEBUFFER, temp)
  FrameCnt = temp ? 2 : 1;
  GLX_GET(GLX_RED_SIZE,         FrameBitsR)
  GLX_GET(GLX_GREEN_SIZE,       FrameBitsG)
  GLX_GET(GLX_BLUE_SIZE,        FrameBitsB)
  GLX_GET(GLX_ALPHA_SIZE,       FrameBitsA)
  GLX_GET(GLX_DEPTH_SIZE,       DepthBits)
  GLX_GET(GLX_STENCIL_SIZE,     StencilBits)
  GLX_GET(GLX_ACCUM_RED_SIZE,   AccumBitsR)
  GLX_GET(GLX_ACCUM_GREEN_SIZE, AccumBitsG)
  GLX_GET(GLX_ACCUM_BLUE_SIZE,  AccumBitsB)
  GLX_GET(GLX_ACCUM_ALPHA_SIZE, AccumBitsA)
  GLX_GET(GLX_AUX_BUFFERS, AuxCnt)
  // FIXME: implement multisample
  AABufferCnt = 0;
  AASampleCnt = 0;
  GLX_GET(GLX_STEREO, temp)
  Stereo = (temp);
}


#define ATTRIB_MAX 64
#define ATTRIB_PUSH1(a)   { if (pos >= ATTRIB_MAX) return NULL; attrib[pos++] = a; }
#define ATTRIB_PUSH2(a,b) { if (pos+1 >= ATTRIB_MAX) return NULL; attrib[pos++] = a; attrib[pos++] = b; }

XVisualInfo* nglContextInfo::GetXVisualInfo(Display* pDisplay, int Screen) const
{
  if (mpXVisualInfo)
  {
    /* This one was fetched via Enum().
     * Give a copy of this visual, using an exact XGetVisualInfo match.
     */
    int n;
    return XGetVisualInfo(pDisplay, VisualAllMask, mpXVisualInfo, &n);
  }
  else
  {
    /* Try to match the context description with an existing visual.
     * glXChooseVisual will do the job for us.
     */
    int pos = 0;
    int attrib[ATTRIB_MAX];

    ATTRIB_PUSH1(GLX_RGBA)

    if (FrameCnt > 1) ATTRIB_PUSH1(GLX_DOUBLEBUFFER)

    ATTRIB_PUSH2(GLX_RED_SIZE,   FrameBitsR)
    ATTRIB_PUSH2(GLX_GREEN_SIZE, FrameBitsG)
    ATTRIB_PUSH2(GLX_BLUE_SIZE,  FrameBitsB)
    if (FrameBitsA)
      ATTRIB_PUSH2(GLX_ALPHA_SIZE,  FrameBitsA)

    if (DepthBits)
      ATTRIB_PUSH2(GLX_DEPTH_SIZE, DepthBits)

    if (StencilBits)
      ATTRIB_PUSH2(GLX_STENCIL_SIZE, StencilBits)

    ATTRIB_PUSH2(GLX_ACCUM_RED_SIZE,   AccumBitsR)
    ATTRIB_PUSH2(GLX_ACCUM_GREEN_SIZE, AccumBitsG)
    ATTRIB_PUSH2(GLX_ACCUM_BLUE_SIZE,  AccumBitsB)
    if (AccumBitsA)
      ATTRIB_PUSH2(GLX_ACCUM_ALPHA_SIZE,  AccumBitsA)

    if (AuxCnt)
      ATTRIB_PUSH2(GLX_AUX_BUFFERS, AuxCnt)

    // FIXME: implement multisample

    if (Stereo)
      ATTRIB_PUSH1(GLX_STEREO)

    ATTRIB_PUSH1(None)

    return glXChooseVisual (pDisplay, Screen, attrib);
  }
}



/*
 * nglContext
 */

nglContext::nglContext()
{
  mpDisplay     = (Display*)App->GetDisplay();
  mpXVisualInfo = NULL;
  mpXVisual     = NULL;
  mGlxCtx       = NULL;
  mpPainter     = NULL;
  mDepth        = 0;

  if (!mpDisplay)
  {
    SetError (_T("context"), NGL_CONTEXT_ENOX);
    return;
  }

  // Check GLX extension presence
  if (!glXQueryExtension(mpDisplay, &mGlxErrorBase, &mGlxEventBase))
  {
    SetError (_T("context"), NGL_CONTEXT_ENOGLX);
    return;
  }
}

nglContext::~nglContext()
{
  if (mpDisplay && mGlxCtx)
    glXDestroyContext (mpDisplay, mGlxCtx);
  if (mpXVisualInfo)
    XFree(mpXVisualInfo);
}

bool nglContext::GetContextInfo(nglContextInfo& rInfo) const
{
  if (!mpDisplay || !mpXVisualInfo)
    return false;

  rInfo = nglContextInfo(mpDisplay, mpXVisualInfo);
  return true;
}


bool nglContext::Build(int Screen, const nglContextInfo& rInfo, const nglContext* pShared)
{
  if (!mpDisplay ||  // No X connection
      mGlxCtx)       // We already have a context
    return false;

#ifdef _DEBUG_
  /* Get GLX extension info
   */
  {
    const char *srv_vendor  = glXQueryServerString(mpDisplay, Screen, GLX_VENDOR);
    const char *srv_version = glXQueryServerString(mpDisplay, Screen, GLX_VERSION);
    const char *cli_vendor  = glXGetClientString(mpDisplay, GLX_VENDOR);
    const char *cli_version = glXGetClientString(mpDisplay, GLX_VERSION);

    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("server: GLX %s (%s)"), srv_version, srv_vendor);
    NGL_LOG(_T("context"), NGL_LOG_INFO, _T("client: GLX %s (%s)"), cli_version, cli_vendor);
  }
#endif // _DEBUG_

  /* Find a matching XVisualInfo
   */
  mpXVisualInfo = rInfo.GetXVisualInfo(mpDisplay, Screen);
  if (!mpXVisualInfo)
  {
    SetError (_T("context"), NGL_CONTEXT_ENOMATCH);
    return false;
  }
  mpXVisual = mpXVisualInfo->visual;
  mDepth    = mpXVisualInfo->depth;

  /* Create GLX context
   */
  mGlxCtx = glXCreateContext(mpDisplay, mpXVisualInfo, pShared ? pShared->mGlxCtx : NULL, True);
  if (mGlxCtx == NULL)
  {
    SetError (_T("context"), NGL_CONTEXT_EGLCTX);
    return false;
  }

  InitPainter();

  return true;
}

bool nglContext::BuildOpenGLFromExisting(GLXContext ctx)
{
  mGlxCtx = ctx;
  InitPainter();

  return true;
}

bool nglContext::MakeCurrent(Window Win) const
{
  if (!mpDisplay || !Win || !mGlxCtx)
    return false;

  if (glXMakeCurrent(mpDisplay, Win, mGlxCtx) == False)
  {
    SetError (_T("context"), NGL_CONTEXT_EBIND);
    return false;
  }

  return true;
}

nglContext::GLExtFunc nglContext::LookupExtFunc (const char* pFuncName)
{
//#ifdef GLX_VERSION_1_4
//  return (GLExtFunc)glXGetProcAddress((const GLubyte*)pFuncName);
//#else
  return (GLExtFunc)glXGetProcAddressARB((const GLubyte*)pFuncName);
//#endif
}


const nglChar* nglContext::OnError (uint& rError) const
{
  return FetchError(gpContextErrorTable, NULL, rError);
}
