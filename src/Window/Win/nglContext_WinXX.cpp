/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglKernel.h"
#include "nglContext.h"
#include "ngl_winXX.h"


/*
#define NGL_CONTEXT_ENONE      0
*/
#define NGL_CONTEXT_ENOMATCH   1
#define NGL_CONTEXT_ESETPFD    2
#define NGL_CONTEXT_EGLCTX     3
#define NGL_CONTEXT_EBIND      4
#if NGL_CONTEXT_ELAST != 4
#error "Please sync NGL_CONTEXT_ELAST in src/core/win32/ngl_win32.h accordingly"
#endif

const nglChar* gpContextErrorTable[] =
{
  /*  0 */ _T("No error"),
  /*  1 */ _T("No visual match your request"),
  /*  2 */ _T("Cound't set pixel format"),
  /*  3 */ _T("GL rendering context creation failed"),
  /*  4 */ _T("Couldn't bind GL context to the system window"),
  NULL
};


/*
* nglContextInfo
*/

nglContextInfo::nglContextInfo (HDC hDC, int PFD)
{
  TargetAPI = eTargetAPI_OpenGL;
  mPFD = 0;

  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  if (DescribePixelFormat(hDC, PFD, pfd.nSize, &pfd) == 0)
    return;

  HWND tmpWin = CreateWindowEx(
    WS_EX_APPWINDOW,
    _T("STATIC"),
    _T("Dummy Tmp Window from NGL"),
    WS_POPUP,
    0, 0, 64, 64,
    NULL,
    NULL,
    NULL,
    NULL);
  NGL_OUT(_T("Create temp gl context window hwnd = 0x%x"), tmpWin);

  HDC tmpDC = GetDC(tmpWin);
  SetPixelFormat(tmpDC, PFD, &pfd);
  HGLRC rc = wglCreateContext(tmpDC);
  wglMakeCurrent(tmpDC, rc);

  PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
  PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT = NULL;
  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");

  PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
  PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = NULL;
  PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = NULL;
  wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
  wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
  wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");

  mPFD        = PFD;

  if ((wglGetPixelFormatAttribivARB && wglGetPixelFormatAttribfvARB))
  {
    std::vector<int> queries;
    std::vector<int> values;
    queries.push_back(WGL_NUMBER_PIXEL_FORMATS_ARB);
    queries.push_back(WGL_DRAW_TO_WINDOW_ARB);
    queries.push_back(WGL_DRAW_TO_BITMAP_ARB);
    queries.push_back(WGL_ACCELERATION_ARB);
    queries.push_back(WGL_NEED_PALETTE_ARB);
    queries.push_back(WGL_NEED_SYSTEM_PALETTE_ARB);
    queries.push_back(WGL_SWAP_LAYER_BUFFERS_ARB);
    queries.push_back(WGL_SWAP_METHOD_ARB);
    queries.push_back(WGL_NUMBER_OVERLAYS_ARB);
    queries.push_back(WGL_NUMBER_UNDERLAYS_ARB);
    queries.push_back(WGL_TRANSPARENT_ARB);
    queries.push_back(WGL_TRANSPARENT_RED_VALUE_ARB);
    queries.push_back(WGL_TRANSPARENT_GREEN_VALUE_ARB);
    queries.push_back(WGL_TRANSPARENT_BLUE_VALUE_ARB);
    queries.push_back(WGL_TRANSPARENT_ALPHA_VALUE_ARB);
    queries.push_back(WGL_TRANSPARENT_INDEX_VALUE_ARB);
    queries.push_back(WGL_SHARE_DEPTH_ARB);
    queries.push_back(WGL_SHARE_STENCIL_ARB);
    queries.push_back(WGL_SHARE_ACCUM_ARB);
    queries.push_back(WGL_SUPPORT_GDI_ARB);
    queries.push_back(WGL_SUPPORT_OPENGL_ARB);
    queries.push_back(WGL_DOUBLE_BUFFER_ARB);
    queries.push_back(WGL_STEREO_ARB);
    queries.push_back(WGL_PIXEL_TYPE_ARB);
    queries.push_back(WGL_COLOR_BITS_ARB);
    queries.push_back(WGL_RED_BITS_ARB);
    queries.push_back(WGL_RED_SHIFT_ARB);
    queries.push_back(WGL_GREEN_BITS_ARB);
    queries.push_back(WGL_GREEN_SHIFT_ARB);
    queries.push_back(WGL_BLUE_BITS_ARB);
    queries.push_back(WGL_BLUE_SHIFT_ARB);
    queries.push_back(WGL_ALPHA_BITS_ARB);
    queries.push_back(WGL_ALPHA_SHIFT_ARB);
    queries.push_back(WGL_ACCUM_BITS_ARB);
    queries.push_back(WGL_ACCUM_RED_BITS_ARB);
    queries.push_back(WGL_ACCUM_GREEN_BITS_ARB);
    queries.push_back(WGL_ACCUM_BLUE_BITS_ARB);
    queries.push_back(WGL_ACCUM_ALPHA_BITS_ARB);
    queries.push_back(WGL_DEPTH_BITS_ARB);
    queries.push_back(WGL_STENCIL_BITS_ARB);
    queries.push_back(WGL_AUX_BUFFERS_ARB);

    queries.push_back(WGL_DRAW_TO_PBUFFER_ARB);
    queries.push_back(WGL_BIND_TO_TEXTURE_RGBA_ARB);
    queries.push_back(WGL_BIND_TO_TEXTURE_RGB_ARB);
    queries.push_back(WGL_SAMPLE_BUFFERS_ARB);
    queries.push_back(WGL_SAMPLES_ARB);
    queries.push_back(WGL_SWAP_METHOD_ARB);

    values.resize(queries.size());
    wglGetPixelFormatAttribivARB(hDC, mPFD, 0, queries.size(), &queries[0], &values[0]);

    // 0  queries.push_back(WGL_NUMBER_PIXEL_FORMATS_ARB);
    // 1  queries.push_back(WGL_DRAW_TO_WINDOW_ARB);
    // 2  queries.push_back(WGL_DRAW_TO_BITMAP_ARB);
    // 3  queries.push_back(WGL_ACCELERATION_ARB);
    // 4  queries.push_back(WGL_NEED_PALETTE_ARB);
    // 5  queries.push_back(WGL_NEED_SYSTEM_PALETTE_ARB);
    // 6  queries.push_back(WGL_SWAP_LAYER_BUFFERS_ARB);
    // 7  queries.push_back(WGL_SWAP_METHOD_ARB);
    // 8  queries.push_back(WGL_NUMBER_OVERLAYS_ARB);
    // 9  queries.push_back(WGL_NUMBER_UNDERLAYS_ARB);
    // 10 queries.push_back(WGL_TRANSPARENT_ARB);
    // 11 queries.push_back(WGL_TRANSPARENT_RED_VALUE_ARB);
    // 12 queries.push_back(WGL_TRANSPARENT_GREEN_VALUE_ARB);
    // 13 queries.push_back(WGL_TRANSPARENT_BLUE_VALUE_ARB);
    // 14 queries.push_back(WGL_TRANSPARENT_ALPHA_VALUE_ARB);
    // 15 queries.push_back(WGL_TRANSPARENT_INDEX_VALUE_ARB);
    // 16 queries.push_back(WGL_SHARE_DEPTH_ARB);
    // 17 queries.push_back(WGL_SHARE_STENCIL_ARB);
    // 18 queries.push_back(WGL_SHARE_ACCUM_ARB);
    // 19 queries.push_back(WGL_SUPPORT_GDI_ARB);
    // 20 queries.push_back(WGL_SUPPORT_OPENGL_ARB);
    FrameCnt = values[21]?2:1; // 21 queries.push_back(WGL_DOUBLE_BUFFER_ARB);
    Stereo = values[22]!=0;// 22 queries.push_back(WGL_STEREO_ARB);
    // 23 queries.push_back(WGL_PIXEL_TYPE_ARB);
    // 24 queries.push_back(WGL_COLOR_BITS_ARB);
    FrameBitsR = values[25]; // 25 queries.push_back(WGL_RED_BITS_ARB);
    // 26 queries.push_back(WGL_RED_SHIFT_ARB);
    FrameBitsG = values[27]; // 27 queries.push_back(WGL_GREEN_BITS_ARB);
    // 28 queries.push_back(WGL_GREEN_SHIFT_ARB);
    FrameBitsB = values[29]; // 29 queries.push_back(WGL_BLUE_BITS_ARB);
    // 30 queries.push_back(WGL_BLUE_SHIFT_ARB);
    FrameBitsA = values[31]; // 31 queries.push_back(WGL_ALPHA_BITS_ARB);
    // 32 queries.push_back(WGL_ALPHA_SHIFT_ARB);
    // 33 queries.push_back(WGL_ACCUM_BITS_ARB);
    AccumBitsR = values[34]; // 34 queries.push_back(WGL_ACCUM_RED_BITS_ARB);
    AccumBitsG = values[35]; // 35 queries.push_back(WGL_ACCUM_GREEN_BITS_ARB);
    AccumBitsB = values[36]; // 36 queries.push_back(WGL_ACCUM_BLUE_BITS_ARB);
    AccumBitsA = values[37]; // 37 queries.push_back(WGL_ACCUM_ALPHA_BITS_ARB);
    DepthBits = values[38]; // 38 queries.push_back(WGL_DEPTH_BITS_ARB);
    StencilBits = values[39]; // 39 queries.push_back(WGL_STENCIL_BITS_ARB);
    AuxCnt = values[40]; // 40 queries.push_back(WGL_AUX_BUFFERS_ARB);

    Offscreen = values[41] != 0; // 41 queries.push_back(WGL_DRAW_TO_PBUFFER_ARB);
    RenderToTexture = values[42] || values[43]; // 42 queries.push_back(WGL_BIND_TO_TEXTURE_RGBA_ARB);
    // 43 queries.push_back(WGL_BIND_TO_TEXTURE_RGB_ARB);
    AABufferCnt = values[44]; // 44 queries.push_back(WGL_SAMPLE_BUFFERS_ARB);
    AASampleCnt = values[45]; // 45 queries.push_back(WGL_SAMPLES_ARB);
    CopyOnSwap = (WGL_SWAP_COPY_ARB == values[46]); // 46 queries.push_back(WGL_SWAP_METHOD_ARB);
  }
  else
  {
    FrameCnt    = (pfd.dwFlags & PFD_DOUBLEBUFFER) ? 2 : 1;
    FrameBitsR  = pfd.cRedBits;
    FrameBitsG  = pfd.cGreenBits;
    FrameBitsB  = pfd.cBlueBits;
    FrameBitsA  = pfd.cAlphaBits;
    DepthBits   = pfd.cDepthBits;
    StencilBits = pfd.cStencilBits;
    AccumBitsR  = pfd.cAccumRedBits;
    AccumBitsG  = pfd.cAccumGreenBits;
    AccumBitsB  = pfd.cAccumBlueBits;
    AccumBitsA  = pfd.cAccumAlphaBits;
    AuxCnt      = pfd.cAuxBuffers;
    AABufferCnt = 0; // FIXME
    AASampleCnt = 0;
    Stereo      = (pfd.dwFlags & PFD_STEREO) ? true : false;
    Offscreen   = false;
    RenderToTexture  = false;
    CopyOnSwap = (pfd.dwFlags & PFD_SWAP_COPY) ? true : false;
  }

  char* vendor = (char*)glGetString(GL_VENDOR);
  if (vendor && !strcmp(vendor, "Matrox Graphics Inc."))
  {
    CopyOnSwap = true;
  }

  if (wglGetSwapIntervalEXT)
  {
    VerticalSync = wglGetSwapIntervalEXT() != 0;
  }

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(rc);
  ReleaseDC(tmpWin, tmpDC);
  DestroyWindow(tmpWin);
}

bool nglContextInfo::Enum (uint Index, nglContextInfo& rInfo)
{
  // We don't have device info, we use the default one
  HDC hDC = GetDC(NULL);

  nglContextInfo info(hDC, 1 + Index);
  if (info.mPFD != 0)
    rInfo = info;

  ReleaseDC(NULL, hDC);

  return info.mPFD != 0;
}

bool wglIsExtensionSupported(const char *extension)
{
  const size_t extlen = strlen(extension);
  const char *supported = NULL;

  // Try To Use wglGetExtensionStringARB On Current DC, If Possible
  PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

  if (wglGetExtString)
    supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

  // If That Failed, Try Standard Opengl Extensions String
  if (supported == NULL)
    supported = (char*)glGetString(GL_EXTENSIONS);

  // If That Failed Too, Must Be No Extensions Supported
  if (supported == NULL)
    return false;

  // Begin Examination At Start Of String, Increment By 1 On False Match
  for (const char* p = supported; ; p++)
  {
    // Advance p Up To The Next Possible Match
    p = strstr(p, extension);

    if (p == NULL)
      return false;            // No Match

    // Make Sure That Match Is At The Start Of The String Or That
    // The Previous Char Is A Space, Or Else We Could Accidentally
    // Match "wglFunkywglExtension" With "wglExtension"

    // Also, Make Sure That The Following Character Is Space Or NULL
    // Or Else "wglExtensionTwo" Might Match "wglExtension"
    if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
      return true;            // Match
  }
}

int nglContextInfo::GetPFD(HDC hDC) const
{
  if (mPFD)
  {
    /* This one was fetched by Enum()
    * This is a little shortsighted since Enum() works on the default device :
    * we should check if mDC == hDC
    */
    return mPFD;
  }
  else
  {
    bool _CopyOnSwap = CopyOnSwap;
    uint _AABufferCnt = AABufferCnt;
    uint _AASampleCnt = AASampleCnt;
    int res = 0;
    int format = -1;
    bool stop = false;

    // Create a dummy context to be able to query ARB's wglChoosePixelFormatARB
    PIXELFORMATDESCRIPTOR pfd;
    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion        = 1;
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;
    pfd.iPixelType      = PFD_TYPE_RGBA;
    pfd.cColorBits      = 0; // FIXME
    pfd.cRedBits        = 0;
    pfd.cRedShift       = 0;
    pfd.cGreenBits      = 0;
    pfd.cGreenShift     = 0;
    pfd.cBlueBits       = 0;
    pfd.cBlueShift      = 0;
    pfd.cAlphaBits      = 0;
    pfd.cAlphaShift     = 0;
    pfd.cAccumBits      = 0;
    pfd.cAccumRedBits   = 0;
    pfd.cAccumGreenBits = 0;
    pfd.cAccumBlueBits  = 0;
    pfd.cAccumAlphaBits = 0;
    pfd.cDepthBits      = DepthBits;
    pfd.cStencilBits    = StencilBits;
    pfd.cAuxBuffers     = 0;
    pfd.iLayerType      = 0;
    pfd.bReserved       = 0;
    pfd.dwLayerMask     = 0;
    pfd.dwVisibleMask   = 0;
    pfd.dwDamageMask    = 0;

    WNDCLASS wc;

    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = App->GetHInstance();
    wc.hIcon         = LoadIcon( App->GetHInstance(), _T("0") );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.lpszMenuName  = NULL;
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;
    wc.hbrBackground = NULL;
    wc.lpfnWndProc   = &::DefWindowProc;
    wc.lpszClassName = NGL_CONTEXT_CLASS;
    if (!RegisterClass( &wc ))
    {
      DWORD err = GetLastError();
      LPVOID lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
      NGL_OUT(_T("context error: %s"), lpMsgBuf);
      LocalFree(lpMsgBuf);

      NGL_DEBUG( OutputDebugString(_T("NGL: error: unable to create ") NGL_CONTEXT_CLASS _T(" window class\n")); )
        return false;
    }

    HWND tmpWin = CreateWindowEx(
      WS_EX_APPWINDOW,
      NGL_CONTEXT_CLASS,
      _T("Dummy Tmp Window from NGL"),
      WS_POPUP,
      0, 0, 64, 64,
      NULL,
      NULL,
      NULL,
      NULL);
    NGL_OUT(_T("Create tmp gl ctx window hwnd = 0x%x"), tmpWin);

    HDC tmpDC = GetDC(tmpWin);
    int pf = ChoosePixelFormat(tmpDC, &pfd);
    res = SetPixelFormat(tmpDC, pf, &pfd);
    HGLRC rc = wglCreateContext(tmpDC);
    res = wglMakeCurrent(tmpDC, rc);


//     res = wglMakeCurrent(NULL, NULL);
//     res = wglDeleteContext(rc);
// 
//     res = ReleaseDC(tmpWin, tmpDC);
//     res = DestroyWindow(tmpWin);


    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
    wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

    for ( res = 0; !res && !stop; )
    {
      if (wglChoosePixelFormatARB) 
      {
        std::vector<int> attrib;
        attrib.push_back(WGL_SUPPORT_OPENGL_ARB);
        attrib.push_back(GL_TRUE);

        attrib.push_back(WGL_PIXEL_TYPE_ARB);
        attrib.push_back(WGL_TYPE_RGBA_ARB);

        attrib.push_back(WGL_ACCELERATION_ARB);
        attrib.push_back(WGL_FULL_ACCELERATION_ARB);

        /// Disabled this as the latest versions of Parallels virtualization software breaks with it.
        //  attrib.push_back(WGL_COLOR_BITS_ARB);
        //  attrib.push_back(FrameBitsR + FrameBitsG + FrameBitsB);
        //  

        attrib.push_back(WGL_RED_BITS_ARB);
        attrib.push_back(FrameBitsR);
        attrib.push_back(WGL_GREEN_BITS_ARB);
        attrib.push_back(FrameBitsG);
        attrib.push_back(WGL_BLUE_BITS_ARB);
        attrib.push_back(FrameBitsB);

        if(FrameBitsA) 
        {
          attrib.push_back(WGL_ALPHA_BITS_ARB);
          attrib.push_back(FrameBitsA);
        }

        if (AccumBitsR || AccumBitsG || AccumBitsB || AccumBitsA) 
        {
          attrib.push_back(WGL_ACCUM_RED_BITS_ARB);
          attrib.push_back(AccumBitsR);
          attrib.push_back(WGL_ACCUM_GREEN_BITS_ARB);
          attrib.push_back(AccumBitsG);
          attrib.push_back(WGL_ACCUM_BLUE_BITS_ARB);
          attrib.push_back(AccumBitsB);
          //if(AccumBitsA) 
          {
            attrib.push_back(WGL_ACCUM_ALPHA_BITS_ARB);
            attrib.push_back(AccumBitsA);
          }
        }

        if (DepthBits) 
        {
          attrib.push_back(WGL_DEPTH_BITS_ARB);
          attrib.push_back(DepthBits);
        }

        if (StencilBits) 
        {
          attrib.push_back(WGL_STENCIL_BITS_ARB);
          attrib.push_back(StencilBits);
        }

        if (Offscreen)
        {
          attrib.push_back(WGL_DRAW_TO_PBUFFER_ARB);
          attrib.push_back(GL_TRUE);

          if (RenderToTexture)
          {
            attrib.push_back(FrameBitsA?WGL_BIND_TO_TEXTURE_RGBA_ARB:WGL_BIND_TO_TEXTURE_RGB_ARB);
            attrib.push_back(GL_TRUE);
          }
        }
        else
        {
          attrib.push_back(WGL_DRAW_TO_WINDOW_ARB);
          attrib.push_back(GL_TRUE);

          if (FrameCnt > 1)
          {
            attrib.push_back(WGL_DOUBLE_BUFFER_ARB);
            attrib.push_back(GL_TRUE);
          }
        }

        if (_AABufferCnt)
        {
          attrib.push_back(WGL_SAMPLE_BUFFERS_ARB);
          attrib.push_back(_AABufferCnt);
          attrib.push_back(WGL_SAMPLES_ARB);
          attrib.push_back(_AASampleCnt);
        }

        if (FrameCnt > 1 && !Offscreen && _CopyOnSwap)
        {
          //glAddSwapHintRectWIN
          attrib.push_back(WGL_SWAP_METHOD_ARB);
          attrib.push_back(WGL_SWAP_COPY_ARB);
        }

        attrib.push_back(0);

        UINT count = 16;

        int formats[16];
        res = wglChoosePixelFormatARB(hDC, &attrib[0], NULL, 16, formats, &count) && count;
        format = formats[0];
      }
      else
      {
        // Old drivers that doesn't support wglChoosePixelFormatARB...
        PIXELFORMATDESCRIPTOR pfd;

        pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion        = 1;
        pfd.dwFlags         = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;
        if (FrameCnt > 1) pfd.dwFlags |= PFD_DOUBLEBUFFER;
        if (Stereo)       pfd.dwFlags |= PFD_STEREO;
        if (_CopyOnSwap) pfd.dwFlags |=  PFD_SWAP_COPY;
        pfd.iPixelType      = PFD_TYPE_RGBA;
        pfd.cColorBits      = FrameBitsA + FrameBitsB + FrameBitsG + FrameBitsR; // FIXME
        pfd.cRedBits        = FrameBitsR;
        pfd.cRedShift       = 0;
        pfd.cGreenBits      = FrameBitsG;
        pfd.cGreenShift     = 0;
        pfd.cBlueBits       = FrameBitsB;
        pfd.cBlueShift      = 0;
        pfd.cAlphaBits      = FrameBitsA;
        pfd.cAlphaShift     = 0;
        pfd.cAccumBits      = AccumBitsA + AccumBitsB + AccumBitsG + AccumBitsR;
        pfd.cAccumRedBits   = AccumBitsR;
        pfd.cAccumGreenBits = AccumBitsG;
        pfd.cAccumBlueBits  = AccumBitsB;
        pfd.cAccumAlphaBits = AccumBitsA;
        pfd.cDepthBits      = DepthBits;
        pfd.cStencilBits    = StencilBits;
        pfd.cAuxBuffers     = AuxCnt;
        pfd.iLayerType      = 0;
        pfd.bReserved       = 0;
        pfd.dwLayerMask     = 0;
        pfd.dwVisibleMask   = 0;
        pfd.dwDamageMask    = 0;

        format = ChoosePixelFormat(hDC, &pfd);
        res = format != 0;
      }

      if (!res)
      {
        format = -1;

        if (_AASampleCnt)
        {
          _AASampleCnt--;
        }

        if (!_AASampleCnt && _AABufferCnt)
        {
          _AABufferCnt--;
          _AASampleCnt = AASampleCnt;
        }
        else if (_CopyOnSwap)
        {
          _CopyOnSwap = false;
        }
        else
          stop = true;
      }

      // Release the dummy context
    }

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(rc);
    ReleaseDC(tmpWin, tmpDC);
    DestroyWindow(tmpWin);

    UnregisterClass(NGL_CONTEXT_CLASS, App->GetHInstance());
    return format;
  }
}


/*
* nglContext
*/

nglContext::nglContext()
{
  mTargetAPI = eTargetAPI_None;
  mpDirect3DDevice = NULL;
  mDC = NULL;
  mRC = NULL;
  mCtxWnd = NULL;
  mpPainter = NULL;
  mValidBackBufferRequestedNotGranted = false;
}

nglContext::~nglContext()
{
  wglMakeCurrent(NULL, NULL);
  if (mRC)
    wglDeleteContext(mRC);
  if (mDC)
    ReleaseDC(mCtxWnd, mDC);
  if (mpDirect3DDevice)
    mpDirect3DDevice->Release();
}


bool nglContext::GetContextInfo(nglContextInfo& rInfo) const
{
  if (!mDC)
    return false;

  int pfd = GetPixelFormat(mDC);
  if (pfd == 0)
    return false;

  rInfo = nglContextInfo(mDC, pfd);
  MakeCurrent();
  return true;
}

bool nglContext::Build(HWND hwnd, const nglContextInfo& rInfo, const nglContext* pShared)
{
  mTargetAPI = rInfo.TargetAPI;

  switch (mTargetAPI)
  {
  case eTargetAPI_OpenGL:
    return BuildOpenGL(hwnd, rInfo, pShared);
    break;
  case eTargetAPI_Direct3D:
    return BuildDirect3D(hwnd, rInfo, pShared);
    break;
  case eTargetAPI_None:
    return BuildNone(hwnd, rInfo, pShared);
    break;

  default:
    NGL_ASSERT(0);
    break;
  }

  InitPainter();
  return false;
}

bool nglContext::BuildNone(HWND hwnd, const nglContextInfo& rInfo, const nglContext* pShared)
{
  // ???
  return true;
}

bool nglContext::BuildDirect3D(HWND hwnd, const nglContextInfo& rInfo, const nglContext* pShared)
{
  // ???
  LPDIRECT3D9 pDirect3D = App->GetDirect3D();

  if (!pDirect3D)
    return false;

  D3DPRESENT_PARAMETERS presParams;
  ZeroMemory(&presParams,sizeof(presParams));

  //presParams.BackBufferFormat = D3DFMT_UNKNOWN;
  presParams.BackBufferFormat = D3DFMT_X8R8G8B8;
  presParams.BackBufferCount = 1;
  presParams.Windowed = TRUE;

  presParams.MultiSampleType = D3DMULTISAMPLE_NONE;//(D3DMULTISAMPLE_TYPE)rInfo.AASampleCnt;

  if (rInfo.CopyOnSwap)
    presParams.SwapEffect = D3DSWAPEFFECT_COPY;
  else
    presParams.SwapEffect = D3DSWAPEFFECT_FLIP;

  presParams.hDeviceWindow = hwnd;

  presParams.EnableAutoDepthStencil = TRUE;
  presParams.AutoDepthStencilFormat = D3DFMT_D24S8;

  presParams.Flags = D3DPRESENTFLAG_DEVICECLIP;

  HRESULT hr = pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &presParams, &mpDirect3DDevice);
  if (FAILED(hr))
    hr = pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &presParams, &mpDirect3DDevice);
  if (FAILED(hr))
    hr = pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presParams, &mpDirect3DDevice);

  if (FAILED(hr))
  {
    switch (hr)
    {
    case D3DERR_DEVICELOST:
      NGL_OUT(_T("D3DERR_DEVICELOST"));
      break;
    case D3DERR_INVALIDCALL:
      NGL_OUT(_T("D3DERR_INVALIDCALL"));
      break;
    case D3DERR_NOTAVAILABLE:
      NGL_OUT(_T("D3DERR_NOTAVAILABLE"));
      break;
    case D3DERR_OUTOFVIDEOMEMORY:
      NGL_OUT(_T("D3DERR_OUTOFVIDEOMEMORY"));
      break;
    }
    return false;  
  }

  switch(mpDirect3DDevice->TestCooperativeLevel())
  {
  case D3DERR_DEVICELOST: 
    return false;
    break;
  case D3DERR_DEVICENOTRESET:
    {
      if(FAILED(mpDirect3DDevice->Reset(&presParams)))
      {
        MessageBox(NULL, _T("Reset() failed!"), _T("CheckDevice()"), MB_OK);
        return false;
      }
    }
    break;
  default:
    break;
  }


  hr = mpDirect3DDevice->BeginScene();
  hr = mpDirect3DDevice->Clear(0,NULL,D3DCLEAR_TARGET /*| D3DCLEAR_ZBUFFER*/, D3DCOLOR_XRGB(0,0,0),1.0f,0);
  hr = mpDirect3DDevice->EndScene();
  hr = mpDirect3DDevice->Present(NULL, NULL, NULL, NULL);
  return true;
}

bool nglContext::BuildOpenGL(HWND hWnd, const nglContextInfo& rInfo, const nglContext* pShared)
{
  if (mRC)
    return false; // We already have a context

  mDC = GetDC(hWnd);
  mCtxWnd = hWnd; // We keep a reference for ReleaseDC() but we don't own it

  int pfd_index = rInfo.GetPFD(mDC);
  nglContextInfo TmpInfo(mDC, pfd_index);
  mValidBackBufferRequestedNotGranted = rInfo.CopyOnSwap && !TmpInfo.CopyOnSwap;
  if (pfd_index < 0)
  {
    SetError(_T("context"), NGL_CONTEXT_ENOMATCH);
    return false;
  }

  PIXELFORMATDESCRIPTOR pfd_dummy;
  pfd_dummy.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  if (!SetPixelFormat(mDC, pfd_index, &pfd_dummy))
  {
    SetError(_T("context"), NGL_CONTEXT_ESETPFD);
    return false;
  }

  mRC = wglCreateContext(mDC);
  NGL_OUT(_T("DC = 0x%x / RC = 0x%x\n"), mDC, mRC);
  if (!mRC)
  {
    SetError(_T("context"), NGL_CONTEXT_EGLCTX);
    return false;
  }

  if (pShared)
  {
    if (!wglShareLists(pShared->mRC, mRC))
    {
      SetError (_T("context"), NGL_CONTEXT_EGLCTX);
      return false;
    }
  }


  return true;
}

bool nglContext::BuildOpenGLFromExisting(HWND hWnd, HGLRC rc)
{
  if (mRC)
    return false; // We already have a context

  if (hWnd)
  {
    mDC = GetDC(hWnd);
  }
  else
  {
    mDC = NULL;
  }
  mCtxWnd = hWnd; // We keep a reference for ReleaseDC() but we don't own it
  mValidBackBufferRequestedNotGranted = false;
  mRC = rc;
  return true;
}

void NGL_API nglCheckForGLErrors();

bool nglContext::MakeCurrent() const
{
  if (mTargetAPI == eTargetAPI_OpenGL)
  {
    if (!mCtxWnd || !mDC || !mRC)
      return false;

    DWORD err = GetLastError();
    /*
    if (err)
    {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    NGL_OUT(_T("context error (before make current): %s (mDC = 0x%x / mRC = 0x%x)"), lpMsgBuf, mDC, mRC);
    LocalFree(lpMsgBuf);
    }
    */
    if (wglGetCurrentContext() != mRC)
    {
      if (!wglMakeCurrent(mDC, mRC))
      {
        SetError(_T("context"), NGL_CONTEXT_EBIND);
        DWORD err = GetLastError();
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
        NGL_OUT(_T("context error: %s (mDC = 0x%x / mRC = 0x%x)"), lpMsgBuf, mDC, mRC);
        LocalFree(lpMsgBuf);
        return false;
      }
    }
    nglCheckForGLErrors();
  }

  return true;
}

nglContext::GLExtFunc nglContext::LookupExtFunc (const char* pFuncName)
{
  if (mTargetAPI == eTargetAPI_OpenGL)
    return (GLExtFunc)wglGetProcAddress((LPCSTR)pFuncName);
  return NULL;
}


const nglChar* nglContext::OnError (uint& rError) const
{
  return FetchError(gpContextErrorTable, NULL, rError);
}

bool nglContext::ShareWith(nglContext* pShared)
{
  return 0 != wglShareLists(pShared->mRC, mRC);
}

LPDIRECT3DDEVICE9 nglContext::GetDirect3DDevice()
{
  return mpDirect3DDevice;
}
