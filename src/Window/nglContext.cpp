/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"
#include "glext/ngl_glext_table.h"

/*
 * nglContextInfo
 */

nglContextInfo::nglContextInfo()
{
  nglVideoMode mode;

  TargetAPI = eTargetAPI_OpenGL;

  FrameCnt    = 2;
  switch (mode.GetDepth())
  {
    case 15:
      FrameBitsR  = 5;
      FrameBitsG  = 5;
      FrameBitsB  = 5;
      break;

    case 16:
      FrameBitsR  = 5;
      FrameBitsG  = 6;
      FrameBitsB  = 5;
      break;

    case 24:
    case 32:
    default:
      FrameBitsR  = 8;
      FrameBitsG  = 8;
      FrameBitsB  = 8;
      break;
  }
  FrameBitsA  = 0;
  DepthBits   = 16;
  StencilBits = 8;
  AccumBitsR  = 0;
  AccumBitsG  = 0;
  AccumBitsB  = 0;
  AccumBitsA  = 0;
  AuxCnt      = 0;
  AABufferCnt = 0;
  AASampleCnt = 0;
  Stereo      = false;
  Offscreen   = false;
  RenderToTexture = false;
  CopyOnSwap = false;
  VerticalSync = true;
#ifdef _WIN32_
  mPFD = 0;
#endif
#ifdef _X11_
  mpXVisualInfo = NULL;
#endif
}

nglContextInfo::nglContextInfo(const nglContextInfo& rInfo)
{
  FrameCnt        = rInfo.FrameCnt;
  FrameBitsR      = rInfo.FrameBitsR;
  FrameBitsG      = rInfo.FrameBitsG;
  FrameBitsB      = rInfo.FrameBitsB;
  FrameBitsA      = rInfo.FrameBitsA;
  DepthBits       = rInfo.DepthBits;
  StencilBits     = rInfo.StencilBits;
  AccumBitsR      = rInfo.AccumBitsR;
  AccumBitsG      = rInfo.AccumBitsG;
  AccumBitsB      = rInfo.AccumBitsB;
  AccumBitsA      = rInfo.AccumBitsA;
  AuxCnt          = rInfo.AuxCnt;
  AABufferCnt     = rInfo.AABufferCnt;
  AASampleCnt     = rInfo.AASampleCnt;
  Stereo          = rInfo.Stereo;
  Offscreen       = rInfo.Offscreen;
  RenderToTexture = rInfo.RenderToTexture;
  CopyOnSwap      = rInfo.CopyOnSwap;
  VerticalSync    = rInfo.VerticalSync;

#ifdef _WIN32_
  mPFD = 0;
#endif
#ifdef _X11_
  mpXVisualInfo = NULL;
#endif
}


void nglContextInfo::Dump(uint Level) const
{
  const nglChar* human_readable[5] = { _T("none"), _T("single"), _T("double"), _T("triple"), _T(">3 (waw!)") };
  uint fbcount = (FrameCnt <= 4) ? FrameCnt : 4;

  NGL_LOG(_T("context"), Level, _T("GL Context description :"));
  NGL_LOG(_T("context"), Level, _T("  Frame buffer : %s"), human_readable[fbcount]);
  NGL_LOG(_T("context"), Level, _T("  Frame bits   : %d:%d:%d:%d\n"), FrameBitsR, FrameBitsG, FrameBitsB, FrameBitsA);
  NGL_LOG(_T("context"), Level, _T("  Depth bits   : %d\n"), DepthBits);
  NGL_LOG(_T("context"), Level, _T("  Stencil bits : %d\n"), StencilBits);
  NGL_LOG(_T("context"), Level, _T("  Accum bits   : %d:%d:%d:%d\n"), AccumBitsR, AccumBitsG, AccumBitsB, AccumBitsA);
  NGL_LOG(_T("context"), Level, _T("  Aux buffer   : %d\n"), AuxCnt);
  NGL_LOG(_T("context"), Level, _T("  Multisample  : %d buffer%s, %d sample%s\n"), AABufferCnt, PLURAL(AABufferCnt), AASampleCnt, PLURAL(AASampleCnt));
  NGL_LOG(_T("context"), Level, _T("  Stereo       : %s\n"), YESNO(Stereo));
  NGL_LOG(_T("context"), Level, _T("  Offscreen    : %s\n"), YESNO(Offscreen));
  NGL_LOG(_T("context"), Level, _T("  Copy On Swap : %s\n"), YESNO(CopyOnSwap));
  NGL_LOG(_T("context"), Level, _T("  Vertical Sync: %s\n"), YESNO(VerticalSync));
  NGL_LOG(_T("context"), Level, _T("  CopyOnSwap  : %s\n"), YESNO(CopyOnSwap));
  NGL_LOG(_T("context"), Level, _T("  VerticalSync: %s\n"), YESNO(VerticalSync));
}


/*
 * nglContext
 */
void nglContext::OnRescale(float NewScale)
{
  // Do Nothing by default
}


void nglContext::CallOnRescale(float NewScale)
{
  OnRescale(mScale);
  mScale = NewScale;
  mScaleInv = 1.0 / NewScale;
}

float nglContext::GetScale() const
{
  return mScale;
}

float nglContext::GetScaleInv() const
{
  return mScaleInv;
}


bool nglContext::CheckExtension (const nglChar* pExtName)
{
  if (!pExtName)
    return false;

  MakeCurrent();
  
  nglString temp(pExtName);
  char* extname = temp.Export();

  int extname_l = strlen(extname);
  const char* ext0 = (const char*)glGetString(GL_EXTENSIONS);
  const char* ext = ext0;
  bool success = false;

  while (!success && (ext = strstr(ext, extname)))
  {
    success = (ext == ext0 || ext[-1] == ' '); // Check previous separator
    ext += extname_l;
    success = success && (*ext == 0 || *ext == ' '); // Check next separator
  }

  if (success || !strncmp(extname, "GL_VERSION_1_", 13) || !strncmp(extname, "GL_VERSION_2_", 13))
  {
    success = InitExtension(pExtName);
#ifdef _DEBUG_
    if (!success)
      NGL_LOG(_T("context"), NGL_LOG_WARNING, _T("'%s' extension setup failed"), pExtName);
  }
  else
  {
    NGL_LOG(_T("context"), NGL_LOG_DEBUG, _T("'%s' extension not found"), pExtName);
#endif
  }

  if (extname)
    free (extname);

  return success;
}

/* These tables are defined in glext/ngl_glext_table.cpp
 */
extern GLExtNameInfo gpGLExtensionNameTable[];
extern GLExtFuncInfo gpGLExtensionFuncTable[];

bool nglContext::InitExtension (const nglChar* pExtName)
{
  const GLExtNameInfo* ext_info;
  const GLExtFuncInfo* func_info;
  uint i;

  nglString ExtName(pExtName);
  std::string extname(ExtName.GetStdString());

  /* First lookup extension record block
   */
  for (i = 0; (ext_info = &(gpGLExtensionNameTable[i])),
              ext_info->pName && strcmp(ext_info->pName, extname.c_str()); i++);

  if (!ext_info->pName)
  /* Two solutions here
   *  1.- this extensions has no associated functions (only macros)
   *  2.- we don't have it yet in our registry
   * We're working hard so 1 is the good answer, so Init() actually succeeded
   */
    return true;

  /* Then initialize each extension's function
   */
  uint last = gpGLExtensionNameTable[i+1].Index;
  for (i = ext_info->Index; i < last; i++)
  {
    func_info = &gpGLExtensionFuncTable[i];

    this->*(func_info->pFunc) = LookupExtFunc(func_info->pName);
  }

  return true;
}

void nglContext::Dump(uint Level) const
{
  nglContextInfo info;

  if (!GetContextInfo(info)) return;
  info.Dump(Level);
  nglString version((const char*)glGetString(GL_VERSION));
  nglString renderer((const char*)glGetString(GL_RENDERER));
  nglString vendor((const char*)glGetString(GL_VENDOR));
  nglString exts((const char*)glGetString(GL_EXTENSIONS));
#if (!defined _UIKIT_) && (!defined _ANDROID_)
  nglString sl((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
  
  NGL_LOG(_T("context"), Level, _T("  OpenGL Version: %s"), version.GetChars());
  NGL_LOG(_T("context"), Level, _T("  Renderer      : %s"),        renderer.GetChars());
  NGL_LOG(_T("context"), Level, _T("  Vendor        : %s"),        vendor.GetChars());
#if (!defined _UIKIT_) && (!defined _ANDROID_)
  NGL_LOG(_T("context"), Level, _T("  GLSL version  : %s"),        sl.GetChars());
#endif
  NGL_LOG(_T("context"), Level, _T("  Extensions    :"));
  
  std::vector<nglString> tokens;
  exts.Tokenize(tokens);
  for (int32 i = 0; i < tokens.size(); i++)
  {
    NGL_LOG(_T("context"), Level, _T("    %3d %s"), i, tokens[i].GetChars());
  }
  
}

void nglContext::InitPainter()
{
  mpPainter = NULL;
  switch (mTargetAPI)
  {
#ifndef __NUI_NO_GL__
    case eOpenGL:
      mpPainter = new nuiGLPainter(this);
      break;
    case eOpenGL2:
      mpPainter = new nuiGL2Painter(this);
      break;
#endif
#ifndef __NUI_NO_D3D__
    case eDirect3D:
      mpPainter = new nuiD3DPainter(this);
      break;
#endif
#ifndef __NUI_NO_SOFTWARE__
    case eNone:
      mpPainter = new nuiSoftwarePainter(this);
      break;
#endif
  }
}

nuiPainter* nglContext::GetPainter() const
{
  return mpPainter;
}


