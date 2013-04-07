/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "ngl_carbon.h"

/*
 * nglContextInfo
 */

bool nglContextInfo::Enum (uint Index, nglContextInfo& rInfo)
{
  return false;
}


/*
 * nglContext
 */
 
/*
#define NGL_CONTEXT_ENONE      0
*/
#define NGL_CONTEXT_ENOMATCH   1
#define NGL_CONTEXT_EGLCTX     2
#define NGL_CONTEXT_EBIND      3
#if NGL_CONTEXT_ELAST != 3
#error "Please sync NGL_CONTEXT_ELAST in src/core/carbon/ngl_carbon.h accordingly"
#endif

const nglChar* gpContextErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("No context match your request"),
/*  2 */ _T("GL rendering context creation failed"),
/*  3 */ _T("Couldn't bind GL context to the system window"),
         NULL
};


nglContext::nglContext()
{
  mpBundleRefOpenGL = NULL;
  mpPainter = NULL;
  mFullscreen = false;
  mValidBackBufferRequestedNotGranted = false;
  mCtx = NULL;
  mScale = 1.0f;
  mScaleInv = 1.0f;
}

nglContext::~nglContext()
{
  Destroy();
#ifndef __NOGLCONTEXT__
  if (mpBundleRefOpenGL)
    aglDellocEntryPoints();
#endif
}

bool nglContext::GetContextInfo(nglContextInfo& rInfo) const
{
  //aglDescribePixelFormat(Format, En, &value); NGL_OUT(Desc, value);
  rInfo = mContextInfo;
  return true;
}

#define DUMP_FORMAT(Desc, En) aglDescribePixelFormat(Format, En, &value); NGL_OUT(_T(Desc), value);
static void DumpFormat(AGLPixelFormat Format)
{
  GLint value;
  DUMP_FORMAT("Buffer size %d\n", AGL_BUFFER_SIZE);
  DUMP_FORMAT("Level %d\n", AGL_LEVEL);
  DUMP_FORMAT("RGBA %d\n", AGL_RGBA);
  DUMP_FORMAT("Double buffer %d\n", AGL_DOUBLEBUFFER);
  DUMP_FORMAT("Stereo %d\n", AGL_STEREO);
  DUMP_FORMAT("Aux buffers %d\n", AGL_AUX_BUFFERS);
  DUMP_FORMAT("Red %d\n", AGL_RED_SIZE);
  DUMP_FORMAT("Green size %d\n", AGL_GREEN_SIZE);
  DUMP_FORMAT("Blue size %d\n", AGL_BLUE_SIZE);
  DUMP_FORMAT("Alpha size %d\n", AGL_ALPHA_SIZE);
  DUMP_FORMAT("Depth Size %d\n", AGL_DEPTH_SIZE);
  DUMP_FORMAT("Stencil Size %d\n", AGL_STENCIL_SIZE);
  DUMP_FORMAT("Accum Red Size %d\n", AGL_ACCUM_RED_SIZE);
  DUMP_FORMAT("Accum Green Size %d\n", AGL_ACCUM_GREEN_SIZE);
  DUMP_FORMAT("Accum Blue Size %d\n", AGL_ACCUM_BLUE_SIZE);
  DUMP_FORMAT("Accum Alpha Size %d\n", AGL_ACCUM_ALPHA_SIZE);
  DUMP_FORMAT("Pixel Size %d\n", AGL_PIXEL_SIZE);
  DUMP_FORMAT("Offscreen %d\n", AGL_OFFSCREEN);
  DUMP_FORMAT("Fullscreen %d\n", AGL_FULLSCREEN);
  DUMP_FORMAT("Window %d\n", AGL_WINDOW);
  DUMP_FORMAT("Renderer ID %d\n", AGL_RENDERER_ID);
  DUMP_FORMAT("Single Renderer %d\n", AGL_SINGLE_RENDERER);
  DUMP_FORMAT("No recovery %d\n", AGL_NO_RECOVERY);
  DUMP_FORMAT("Accelerated %d\n", AGL_ACCELERATED);
  DUMP_FORMAT("Backing Store %d\n", AGL_BACKING_STORE);
  DUMP_FORMAT("Robust %d\n", AGL_ROBUST);
  DUMP_FORMAT("MP Safe %d\n", AGL_MP_SAFE);
  DUMP_FORMAT("Compliant %d\n", AGL_COMPLIANT);
  DUMP_FORMAT("MultiScreen %d\n", AGL_MULTISCREEN);
  DUMP_FORMAT("Virtual Screen %d\n", AGL_VIRTUAL_SCREEN);
}


bool nglContext::Build(WindowRef Win, const nglContextInfo& rInfo, const nglContext* pShared, bool Fullscreen)
{
  mTargetAPI = rInfo.TargetAPI;
  if (mTargetAPI != eTargetAPI_OpenGL || mTargetAPI != eTargetAPI_OpenGL2)
    return false;
  
#ifndef __NOGLCONTEXT__
  mFullscreen = Fullscreen;

  std::vector<GLint> attribs;
  attribs.push_back(AGL_RGBA);
  if (rInfo.FrameCnt != 1)
    attribs.push_back(AGL_DOUBLEBUFFER);
  
  attribs.push_back(AGL_DEPTH_SIZE);
  attribs.push_back(rInfo.DepthBits);
  
  attribs.push_back(AGL_STENCIL_SIZE);
  attribs.push_back(rInfo.StencilBits);

  if (rInfo.AuxCnt)
  {
    attribs.push_back(AGL_AUX_BUFFERS);
    attribs.push_back(rInfo.AuxCnt);
  }
  
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

  if (rInfo.AccumBitsR || rInfo.AccumBitsG || rInfo.AccumBitsB || rInfo.AccumBitsA)
  {
    attribs.push_back(AGL_ACCUM_RED_SIZE);
    attribs.push_back(rInfo.AccumBitsR);
    attribs.push_back(AGL_ACCUM_GREEN_SIZE);
    attribs.push_back(rInfo.AccumBitsG);
    attribs.push_back(AGL_ACCUM_BLUE_SIZE);
    attribs.push_back(rInfo.AccumBitsB);
    attribs.push_back(AGL_ACCUM_ALPHA_SIZE);
    attribs.push_back(rInfo.AccumBitsA);
  }

  
  if (rInfo.Stereo) 
    attribs.push_back(AGL_STEREO);
  
  attribs.push_back(AGL_MINIMUM_POLICY);
  attribs.push_back(AGL_NO_RECOVERY);
  
  if (rInfo.CopyOnSwap)
    attribs.push_back(AGL_BACKING_STORE);
  
  attribs.push_back(AGL_NONE);
     
  /* Choose pixel format */
  AGLPixelFormat Format = aglChoosePixelFormat(NULL, 0, &attribs[0]);
  
  //NGL_OUT("Pixel Format: 0x%x\n", Format);
  
  if (!Format)
  {
    if (rInfo.CopyOnSwap)
    {
      attribs[attribs.size() - 2] = AGL_NONE;
      Format = aglChoosePixelFormat(NULL, 0, &attribs[0]);
    } 

    if (!Format)
    {
      SetError(_T("context"), NGL_CONTEXT_ENOMATCH);
      return false;
    }
  }

  //DumpFormat(Format);
  
  /* Create an AGL context */
  mCtx = aglCreateContext(Format, pShared?pShared->mCtx:NULL);
  long err = aglGetError();

  GLint value;
  aglDescribePixelFormat(Format, AGL_DOUBLEBUFFER, &value);
  mContextInfo.FrameCnt = value ? 2 : 1;     ///< Number of frame buffers (two means double-buffering)

  aglDescribePixelFormat(Format, AGL_RED_SIZE, (GLint*)&mContextInfo.FrameBitsR);   ///< Bits per red component (frame buffer)
  aglDescribePixelFormat(Format, AGL_GREEN_SIZE, (GLint*)&mContextInfo.FrameBitsG);   ///< Bits per green component (frame buffer)
  aglDescribePixelFormat(Format, AGL_BLUE_SIZE, (GLint*)&mContextInfo.FrameBitsB);   ///< Bits per blue component (frame buffer)
  aglDescribePixelFormat(Format, AGL_ALPHA_SIZE, (GLint*)&mContextInfo.FrameBitsA);   ///< Bits per alpha component (frame buffer)
  aglDescribePixelFormat(Format, AGL_DEPTH_SIZE, (GLint*)&mContextInfo.DepthBits);    ///< Depth buffer resolution (ie. Z buffer, 0 means no Z buffer)
  aglDescribePixelFormat(Format, AGL_STENCIL_SIZE, (GLint*)&mContextInfo.StencilBits);  ///< Stencil buffer resolution (0 means no stencil)
  aglDescribePixelFormat(Format, AGL_ACCUM_RED_SIZE, (GLint*)&mContextInfo.AccumBitsR);   ///< Bits per red component (accumulator buffer)
  aglDescribePixelFormat(Format, AGL_ACCUM_GREEN_SIZE, (GLint*)&mContextInfo.AccumBitsG);   ///< Bits per green component (accumulator buffer)
  aglDescribePixelFormat(Format, AGL_ACCUM_BLUE_SIZE, (GLint*)&mContextInfo.AccumBitsB);   ///< Bits per blue component (accumulator buffer)
  aglDescribePixelFormat(Format, AGL_ACCUM_ALPHA_SIZE, (GLint*)&mContextInfo.AccumBitsA);   ///< Bits per alpha component (accumulator buffer)
  aglDescribePixelFormat(Format, AGL_AUX_BUFFERS, (GLint*)&mContextInfo.AuxCnt);       ///< Number of auxiliary buffers
  aglDescribePixelFormat(Format, AGL_SAMPLE_BUFFERS_ARB, (GLint*)&mContextInfo.AABufferCnt);  ///< Number of anti-aliasing buffers
  aglDescribePixelFormat(Format, AGL_SAMPLES_ARB, (GLint*)&mContextInfo.AASampleCnt);  ///< Anti-alisaing oversampling count
  aglDescribePixelFormat(Format, AGL_STEREO, &value);       ///< Stereoscopic display

  mContextInfo.Stereo = value != 0;
  mContextInfo.Offscreen = false;       ///< This context can render in memory instead of to a window. (false by default).
  mContextInfo.RenderToTexture = false; ///< This context must be able to be bound as a texture. (false by default)
  aglDescribePixelFormat(Format, AGL_BACKING_STORE, &value);  ///< This context must be able to use copy the back buffer to the front buffer instead of swaping them. (false by default) 
  mContextInfo.CopyOnSwap = value != 0;
  if (rInfo.CopyOnSwap && !mContextInfo.CopyOnSwap)
    mValidBackBufferRequestedNotGranted = true;
  
  aglDestroyPixelFormat(Format);
  if (!mCtx)
  {
    SetError(_T("context"), NGL_CONTEXT_EGLCTX);
    /*
    switch (err)
    {
    case AGL_BAD_MATCH:
      NGL_OUT("AGL Error: Bad Context Match (shared context incompatible with requested pixel format).\n");
      break;
    case AGL_BAD_CONTEXT:
      NGL_OUT("AGL Error: Bad Shared Context.\n");
      break;
    case AGL_BAD_PIXELFMT:
      NGL_OUT("AGL Error: Bad Pixel Format.\n");
      break;
    default:
      NGL_OUT("AGL Error: Unknown error\n");
      break;
    }
    */
    return false;
  }

  /* Attach the context to the window */
  if (!aglSetDrawable(mCtx, GetWindowPort (Win)))
  {
    SetError(_T("context"), NGL_CONTEXT_EBIND);
    return false;
  }

  {
    CGLError err = kCGLNoError;
    CGLContextObj ctx = CGLGetCurrentContext();
    
    // Enable the multi-threading
    //err =  CGLEnable( ctx, kCGLCEMPEngine);
    
    if (err != kCGLNoError )
    {
      // Multi-threaded execution is possibly not available
      // Insert your code to take appropriate action
    }
  }
  
  GLint vsync = rInfo.VerticalSync ? 1 : 0;
  aglSetInteger(mCtx, AGL_SWAP_INTERVAL, &vsync);
  
  InitPainter();
  MakeCurrent(Win);
#endif
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  return true;
}

bool nglContext::BuildOpenGLFromExisting(WindowRef Win, AGLContext Ctx)
{
  mTargetAPI = eTargetAPI_OpenGL;
  
  mFullscreen = false;

  mCtx = Ctx;

  InitPainter();
  MakeCurrent(Win);
  return true;
}

bool nglContext::MakeCurrent(WindowRef win) const
{
  bool res = true;
#ifndef __NOGLCONTEXT__
  if (mCtx)
    res = aglSetCurrentContext(mCtx) != 0;
  if (win)
    aglSetDrawable(mCtx, GetWindowPort(win));
  
  CGLContextObj ctx = CGLGetCurrentContext();
  CGLPixelFormatObj pixelFormat = CGLGetPixelFormat(ctx);
  //printf("cglcontext  : 0x%x / 0x%x\n", ctx, pixelFormat);
#endif
  return res;
}


const nglChar* nglContext::OnError (uint& rError) const
{
  return FetchError(gpContextErrorTable, NULL, rError);
}

// -------------------------

OSStatus nglContext::aglInitEntryPoints (void)
{
  OSStatus err = noErr;
#ifndef __CFM_CLASSIC__
  const Str255 frameworkName = "\pOpenGL.framework";
  FSRefParam fileRefParam;
  FSRef fileRef;
  CFURLRef bundleURLOpenGL;

  memset(&fileRefParam, 0, sizeof(fileRefParam));
  memset(&fileRef, 0, sizeof(fileRef));

  fileRefParam.ioNamePtr  = frameworkName;
  fileRefParam.newRef = &fileRef;

  // Frameworks directory/folder
  err = FindFolder (kSystemDomain, kFrameworksFolderType, false, &fileRefParam.ioVRefNum, (SInt32*)&fileRefParam.ioDirID);

  if (noErr != err) 
  {
    DebugStr ("\pCould not find frameworks folder");
    return err;
  }

  err = PBMakeFSRefSync (&fileRefParam); // make FSRef for folder
  if (noErr != err) 
  {
    DebugStr ("\pCould make FSref to frameworks folder");
    return err;
  }

  // create URL to folder
  bundleURLOpenGL = CFURLCreateFromFSRef (kCFAllocatorDefault,
  &fileRef);
  if (!bundleURLOpenGL) 
  {
    DebugStr ("\pCould create OpenGL Framework bundle URL");
    return paramErr;
  }

  // create ref to GL's bundle
  mpBundleRefOpenGL = CFBundleCreate (kCFAllocatorDefault,
  bundleURLOpenGL);
  if (!mpBundleRefOpenGL) 
  {
    DebugStr ("\pCould not create OpenGL Framework bundle");
    return paramErr;
  }

  CFRelease (bundleURLOpenGL); // release created bundle
  // if the code was successfully loaded, look for our function.
  if (!CFBundleLoadExecutable (mpBundleRefOpenGL)) 
  {
    DebugStr ("\pCould not load MachO executable");
    return paramErr;
  }
#endif  
  
  return err;
}

// -------------------------

void nglContext::aglDellocEntryPoints (void)
{
#ifndef __CFM_CLASSIC__
  if (mpBundleRefOpenGL != NULL) 
  {
    // unload the bundle's code.
    CFBundleUnloadExecutable (mpBundleRefOpenGL);
    CFRelease (mpBundleRefOpenGL);
    mpBundleRefOpenGL = NULL;
  }
#endif
}

// -------------------------

void* nglContext::aglGetProcAddress (const char * pszProc)
{
#ifndef __CFM_CLASSIC__
  if (!mpBundleRefOpenGL)
    aglInitEntryPoints();
  CFStringRef str = CFStringCreateWithCString (kCFAllocatorDefault, pszProc, CFStringGetSystemEncoding());
  void *fp = CFBundleGetFunctionPointerForName (mpBundleRefOpenGL, str);
  CFRelease(str);
  return fp;
#else
  return NULL;          
#endif
}



nglContext::GLExtFunc nglContext::LookupExtFunc (const char* pFuncName)
{
  return (GLExtFunc)aglGetProcAddress(pFuncName);
}


bool nglContext::Destroy()
{
#ifndef __NOGLCONTEXT__
  if (mCtx)
    aglDestroyContext(mCtx);
  mCtx = NULL;
#endif
	return true;
}
