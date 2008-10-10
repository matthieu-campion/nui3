/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiD3DPainter.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nglMatrix.h"
#include "AAPrimitives.h"
#include "nuiShader.h"
#include "nuiTexture.h"
#include "AAPrimitives.h"


#define NGL_OUT(a)
//#define NGL_OUT(a) wprintf(a)
//#define NUI_PROFILE_DIRECTX
#define DIRECTX_FULL_LOG

//profiling--------------------------------
#ifdef NUI_PROFILE_DIRECTX
LARGE_INTEGER start[100];
double cumul[100];
WCHAR strOut[512];
#define PROFILE_CHRONO_IN(a) QueryPerformanceCounter(&start[a])
#define PROFILE_CHRONO_RESET(a) cumul[a] = 0
#define PROFILE_CHRONO_OUT(a) {LARGE_INTEGER stop,freq;QueryPerformanceCounter(&stop);QueryPerformanceFrequency(&freq);stop.QuadPart -=start[a].QuadPart;cumul[a] += (double)((stop.QuadPart*1000000)/freq.QuadPart);}
#define PROFILE_CHRONO_LOG(a,str) wsprintf(strOut, L"%s chrono(%d) = %d ms \n", _T(str), a, (int)(cumul[a]));NGL_OUT(strOut);
int drawArrayCount = 0;
int setStateCount = 0;
#else
#define PROFILE_CHRONO_IN(a) 
#define PROFILE_CHRONO_RESET(a)
#define PROFILE_CHRONO_OUT(a) 
#define PROFILE_CHRONO_LOG(a,str)

#endif
//-----------------------------------------

#ifndef __NUI_NO_D3D__
#define NUI_RETURN_IF_RENDERING_DISABLED
#define NUI_USE_MULTISAMPLE_AA
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6
 
#define DIRECTX_FULL_LOG

#define NUI_VERTEXBUFFER_MAXSIZE 16384

//***************************
//jonathan - TO DO LIST
//non native directx primitives support : quads, quad strips, polygons
//rArray.UseGLAATexture - glAA !
//lost device, recreation(CTRL-ALT-SUPPR ...) - device release
//textures : 15 and 16 bits formats
//optimisation (DrawPrimitiveUP ? renderstates ? ...)
//***************************

struct NuiD3DVertex
{
    FLOAT    x, y, z;
    D3DCOLOR diffuse;
    FLOAT    tu, tv;
};


class HRESULTChecker
{
public:
  inline HRESULTChecker(HRESULT hr = S_OK)
  {
    mRes = hr;
    CheckForD3DErrors(mRes);
  }

  inline ~HRESULTChecker()
  {
  }

  inline const HRESULT& operator=(HRESULT hr)
  {
    mRes = hr;
    CheckForD3DErrors(mRes);
    return mRes;
  }

  operator HRESULT()
  {
    return mRes;
  }

  inline operator HRESULT() const
  {
    return mRes;
  }

  inline void CheckForD3DErrors(HRESULT hr)
  {
#if 1 // Globally enable/disable Direct3D error checking
#ifdef _DEBUG_
    bool error = false;
    switch (hr)
    {
      case D3D_OK:
        break;
      case D3DERR_WRONGTEXTUREFORMAT:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Wrong Texture format."));
        break;
      case D3DERR_UNSUPPORTEDCOLOROPERATION:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported color operation."));
        break;
      case D3DERR_UNSUPPORTEDCOLORARG:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported color arguments."));
        break;
      case D3DERR_UNSUPPORTEDALPHAOPERATION:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported alpha operation."));
        break;
      case D3DERR_UNSUPPORTEDALPHAARG:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported alpha arguments."));
        break;
      case D3DERR_TOOMANYOPERATIONS:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Too many operations."));
        break;
      case D3DERR_CONFLICTINGTEXTUREFILTER:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Conflicting texture filters."));
        break;
      case D3DERR_UNSUPPORTEDFACTORVALUE:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported factor value."));
        break;
      case D3DERR_CONFLICTINGRENDERSTATE:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Conflicting render state."));
        break;
      case D3DERR_UNSUPPORTEDTEXTUREFILTER:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Unsupported texture filter."));
        break;
      case D3DERR_CONFLICTINGTEXTUREPALETTE:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Conflicting texture palette."));
        break;
      case D3DERR_DRIVERINTERNALERROR:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Driver internal error."));
        break;
      case D3DERR_NOTFOUND:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Not found."));
        break;
      case D3DERR_MOREDATA:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : More data."));
        break;
      case D3DERR_DEVICELOST:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Device lost."));
        break;
      case D3DERR_DEVICENOTRESET:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Device not reset."));
        break;
      case D3DERR_NOTAVAILABLE:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Not avaiable."));
        break;
      case D3DERR_OUTOFVIDEOMEMORY:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Out of video memory."));
        break;
      case D3DERR_INVALIDDEVICE:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Invalid device."));
        break;
      case D3DERR_INVALIDCALL:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Invalid call."));
        break;
      case D3DERR_DRIVERINVALIDCALL:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Driver invalid call."));
        break;
      case D3DERR_WASSTILLDRAWING:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Was still drawing."));
        break;
//#FIXME
#ifdef D3DERR_DEVICEREMOVED
      case D3DERR_DEVICEREMOVED:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Device removed."));
        break;
#endif
//#FIXME
#ifdef D3DERR_DEVICEHUNG
      case D3DERR_DEVICEHUNG:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("D3D Error : Device hung."));
        break;
#endif
      default:
        NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("Unknown Direct3D Error."));
        break;

        /* non géré
        #define D3DOK_NOAUTOGEN                         MAKE_D3DSTATUS(2159)
#define S_NOT_RESIDENT                          MAKE_D3DSTATUS(2165)
#define S_RESIDENT_IN_SHARED_MEMORY             MAKE_D3DSTATUS(2166)
#define S_PRESENT_MODE_CHANGED                  MAKE_D3DSTATUS(2167)
#define S_PRESENT_OCCLUDED                      MAKE_D3DSTATUS(2168)
        */
    }
    if (hr != S_OK)
      __asm int 3;

#endif
#endif
  }

private:
  HRESULT mRes;
};

uint32 nuiD3DPainter::mActiveContexts = 0;

static void ConvertMatrix(D3DMATRIX& rResult, const nuiMatrix& rSource)
{
  for (int j = 0; j < 4; j++)
    for (int i = 0; i < 4; i++)
      rResult.m[i][j] = rSource(i,j);
}

nuiD3DPainter::nuiD3DPainter(nglContext* pContext, const nuiRect& rRect)
: nuiPainter(rRect, pContext)
{
  mpVB = NULL;
  mnCurrentVBOffset = 0;
  mCanRectangleTexture = 1; //FIXME
  mnBatchCurrentVBOffset = 0;
  mnBatchCurrentVBSize = 0;
  if (!mActiveContexts)
  {
  }
 
  mActiveContexts++;

  mpContext = pContext;
  if (mpContext)
  {
    //mpContext->BeginSession();
  }
   AddNeedTextureBackingStore(); 
}


nuiD3DPainter::~nuiD3DPainter()
{
  mActiveContexts--;
  if (mActiveContexts == 0)
    glAAExit(); //FIX ME
}


void nuiD3DPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  PROFILE_CHRONO_IN(0);
#ifdef NUI_PROFILE_DIRECTX
  drawArrayCount = 0;
  setStateCount = 0;
#endif
  HRESULTChecker hr = S_OK;
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  //mpContext->BeginSession();
  NUI_RETURN_IF_RENDERING_DISABLED;

  CreateDeviceObjects(); //FIXME

  nuiPainter::StartRendering(ClipOffsetX, ClipOffsetY);
  //NGL_OUT(_T("StartRendering (%d x %d)\n"), mWidth, mHeight);
  D3DVIEWPORT9 vp;
  vp.X = 0;
  vp.Y = 0;
  vp.Width = mWidth;
  vp.Height = mHeight;
  vp.MinZ = 0.0f;
  vp.MaxZ = 1.0f;
  hr = pDev->SetViewport(&vp);

  D3DMATRIX projection;
  D3DMATRIX tr;
  D3DMATRIX id;

  nuiMatrix m, m0, m1;
  ConvertMatrix(id, m);
  m0.SetTranslation(-1.0f, 1.0f, 0.0f);
  m0.Transpose();
  m1.SetScaling(2.0f / (float)mWidth, -2.0f / (float)mHeight, 1.0f);
  m = m1 * m0;
  ConvertMatrix(projection, m);
  
  hr = pDev->SetTransform(D3DTS_PROJECTION, &projection);
  hr = pDev->SetTransform(D3DTS_WORLD, &id);
  hr = pDev->SetTransform(D3DTS_VIEW, &id);

  hr = pDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  hr = pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
  hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  hr = pDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  hr = pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  hr = pDev->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255)); //@@@
  hr = pDev->SetRenderState(D3DRS_LIGHTING,FALSE); 
  hr = pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

  hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE); //D3DTOP_SELECTARG1//D3DTOP_ADD //D3DTOP_MODULATE
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); //ADD : black SUBTRACT : texte n'apparait pas
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE); //@@@
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE); //@@@
  //FIXME
  if (mState.mTexturing)
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
  else
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1); //D3DTOP_SELECTARG1
  hr = pDev->SetStreamSource( 0, mpVB, 0, sizeof(NuiD3DVertex) );
}



void nuiGetBlendFuncFactorsD3D(nuiBlendFunc Func, DWORD& src, DWORD& dst)
{
  GLenum sfactor, dfactor;
  switch (Func)
  {
  default:
  case nuiBlendSource:        sfactor = D3DBLEND_ONE;                   dfactor = D3DBLEND_ZERO;                 break;
  case nuiBlendTransp:        sfactor = D3DBLEND_SRCALPHA;              dfactor = D3DBLEND_INVSRCALPHA;  break;
  case nuiBlendClear:         sfactor = D3DBLEND_ZERO;                  dfactor = D3DBLEND_ZERO;                 break;
  case nuiBlendDest:          sfactor = D3DBLEND_ZERO;                  dfactor = D3DBLEND_ONE;                  break;
  case nuiBlendOver:          sfactor = D3DBLEND_ONE;                   dfactor = D3DBLEND_INVSRCALPHA;  break;
  case nuiBlendOverRev:       sfactor = D3DBLEND_INVDESTALPHA;          dfactor = D3DBLEND_ONE;                  break;
  case nuiBlendIn:            sfactor = D3DBLEND_DESTALPHA;             dfactor = D3DBLEND_ZERO;                 break;
  case nuiBlendInRev:         sfactor = D3DBLEND_ZERO;                  dfactor = D3DBLEND_SRCALPHA;            break;
  case nuiBlendOut:           sfactor = D3DBLEND_INVDESTALPHA;          dfactor = D3DBLEND_ZERO;                 break;
  case nuiBlendOutRev:        sfactor = D3DBLEND_ZERO;                  dfactor = D3DBLEND_INVSRCALPHA;  break;
  case nuiBlendTop:           sfactor = D3DBLEND_DESTALPHA;             dfactor = D3DBLEND_INVSRCALPHA;  break;
  case nuiBlendTopRev:        sfactor = D3DBLEND_INVDESTALPHA;          dfactor = D3DBLEND_SRCALPHA;            break;
  case nuiBlendXOR:           sfactor = D3DBLEND_INVDESTALPHA;          dfactor = D3DBLEND_INVSRCALPHA;  break;
  case nuiBlendAdd:           sfactor = D3DBLEND_ONE;                   dfactor = D3DBLEND_ONE;                  break;
  case nuiBlendSaturate:      sfactor = D3DBLEND_SRCALPHASAT;           dfactor = D3DBLEND_ONE;                  break;

  case nuiBlendTranspClear:   sfactor = D3DBLEND_SRCALPHA;             dfactor = D3DBLEND_ZERO;                 break;
  case nuiBlendTranspInRev:   sfactor = D3DBLEND_SRCALPHA;             dfactor = D3DBLEND_SRCALPHA;            break;
  case nuiBlendTranspAdd:     sfactor = D3DBLEND_SRCALPHA;             dfactor = D3DBLEND_ONE;                  break;
  }
  src = sfactor;
  dst = dfactor;
}




D3DTEXTUREFILTERTYPE nuiGetTextureFilteringTypeD3D(GLuint Filter)
{
  //return D3DTEXF_POINT;
	switch (Filter)
	{
	case GL_NEAREST:
		return D3DTEXF_POINT;
		break;
	case GL_LINEAR:
		return D3DTEXF_LINEAR;
		break;
	default:
		return D3DTEXF_NONE;
		break;
	}
}




D3DTEXTUREADDRESS nuiGetTextureAdressModeD3D(GLuint AdressMode)
{
	switch (AdressMode)
	{
	case GL_CLAMP:
		return D3DTADDRESS_CLAMP;
		break;
	case GL_REPEAT:
		return D3DTADDRESS_WRAP;
		break;
  case GL_CLAMP_TO_EDGE:
    return D3DTADDRESS_BORDER;
	default:
		return D3DTADDRESS_CLAMP;
		break;
	}
}






void nuiD3DPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  //return;
  //NGL_OUT(_T("--------------"));
  PROFILE_CHRONO_IN(1);
#ifdef NUI_PROFILE_DIRECTX
  setStateCount ++;
#endif
  //NGL_OUT(_T("SetState() "));
  NUI_RETURN_IF_RENDERING_DISABLED;
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  //CheckForGLErrors();

  //----------------------------
  //blending -------------------
  //----------------------------
  //#ifdef BLENDING_ENABLED
  if (ForceApply || mState.mBlending != rState.mBlending)
  {
    NGL_OUT(_T("Blending Value %d "), mState.mBlending);
    mState.mBlending = rState.mBlending;
    if (mState.mBlending)
    {
      hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      if (ForceApply || mState.mBlendFunc != rState.mBlendFunc)
      {
        mState.mBlendFunc = rState.mBlendFunc;
        DWORD src, dst;
        nuiGetBlendFuncFactorsD3D(rState.mBlendFunc, src, dst);
        hr = pDev->SetRenderState(D3DRS_SRCBLEND, src);
        hr = pDev->SetRenderState(D3DRS_DESTBLEND, dst);
      }
    }
    else
    {
      hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE); 
    }
  }
  //#endif

  //----------------------------
  //stencil test ---------------
  //----------------------------
//#ifdef STENCIL_ENABLED
  if (ForceApply || mState.mStencilMode != rState.mStencilMode || mState.mStencilValue != rState.mStencilValue)
  {
    NGL_OUT(_T("Stencil mode"));
    mState.mStencilMode = rState.mStencilMode;
    mState.mStencilValue = rState.mStencilValue;
    switch (mState.mStencilMode)
    {
    case nuiIgnoreStencil:
      NGL_OUT(_T("nuiIgnoreStencil Value %d "), mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		  hr = pDev->SetRenderState(D3DRS_STENCILMASK, 0);
		  hr = pDev->SetRenderState(D3DRS_STENCILREF, mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NEVER);
      hr = pDev->SetRenderState(D3DRS_STENCILWRITEMASK, ~0);
      hr = pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
      hr = pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
      hr = pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
      break;

    case nuiReadStencil:
      NGL_OUT(_T("nuiReadStencil Value %d "), mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		  hr = pDev->SetRenderState(D3DRS_STENCILMASK, 0);
		  hr = pDev->SetRenderState(D3DRS_STENCILREF, mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
      hr = pDev->SetRenderState(D3DRS_STENCILWRITEMASK, ~0);
      hr = pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
      hr = pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
      hr = pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
      break;
    
    case nuiAddToStencil:
      NGL_OUT(_T("nuiAddToStencil Value %d "), mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		  hr = pDev->SetRenderState(D3DRS_STENCILMASK, ~0);
		  hr = pDev->SetRenderState(D3DRS_STENCILREF, mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
      hr = pDev->SetRenderState(D3DRS_STENCILWRITEMASK, ~0);
      hr = pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
      hr = pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
      hr = pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
      break;
    
    case nuiBlendToStencil:
      NGL_OUT(_T("nuiBlendToStencil Value %d "), mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		  hr = pDev->SetRenderState(D3DRS_STENCILMASK, ~0);
		  hr = pDev->SetRenderState(D3DRS_STENCILREF, mState.mStencilValue);
		  hr = pDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); 
      hr = pDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_INCR);
      hr = pDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
      hr = pDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
  }
//#endif
  
  //----------------------------
  //shaders (not used) ---------
  //----------------------------
  if (ForceApply || mState.mpShader != rState.mpShader)
  {
    NGL_ASSERT(0);
  }

  // 2D Textures:
  ApplyTexture(rState, ForceApply);

  //----------------------------
  //rendering color buffer -----
  //----------------------------
//#ifdef COLORBUFFER_ENABLED
  if (ForceApply || mState.mColorBuffer != rState.mColorBuffer)
  {
	  NGL_OUT(_T("ColorBuffer Value %d "), mState.mColorBuffer);
	  mState.mColorBuffer = rState.mColorBuffer;
    if (mState.mColorBuffer == GL_TRUE)
	  {
      //ARVB bitmask 
		  hr = pDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0F); //0x0F
	  }
	  else
	  {
		  hr = pDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	  }
  }
//#endif
  //----------------------------
  //SCISSOR CLIPPING -----------
  //----------------------------
#define SCISSORCLIPPING_ENABLED
#ifdef SCISSORCLIPPING_ENABLED

  if (mClip.mEnabled)    
  {
    //NGL_OUT(_T("Clipping mode"));
    //PROFILE_CHRONO_IN(22);
    hr = pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    nuiRect clip(mClip);
    clip.Move(-mClipOffsetX, -mClipOffsetY);
    RECT r = {ToBelow(clip.Left()),
      ToBelow(clip.Top()),
      ToBelow(clip.Left() + clip.GetWidth()),
      ToBelow(clip.Top() + clip.GetHeight())};
    hr = pDev->SetScissorRect(&r);
    //PROFILE_CHRONO_OUT(22);
    //NGL_OUT(_T("Clip {%f, %f, %f, %f} "), mClip.mLeft, mClip.mTop, mClip.GetWidth(), mClip.GetHeight());
  }
  else
  {
    //PROFILE_CHRONO_IN(22);
    pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
    //PROFILE_CHRONO_OUT(22);
  }
#endif
  //----------------------------
  //colors ---------------------
  //----------------------------
  mState.mClearColor = rState.mClearColor;
  mState.mStrokeColor = rState.mStrokeColor;
  mState.mFillColor = rState.mFillColor;

  if (mState.mTexturing)
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
  else
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE); //SELECT1
  
  PROFILE_CHRONO_OUT(1);
  //NGL_OUT(_T("--------------"));
}





void nuiD3DPainter::CreateDeviceObjects()
{
  //création des objets qui doivent être créés/recréés avec le device
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  if (!mpVB)
  {
    int size = sizeof(NuiD3DVertex);
    hr = pDev->CreateVertexBuffer(NUI_VERTEXBUFFER_MAXSIZE * sizeof(NuiD3DVertex), D3DUSAGE_DYNAMIC, 
      D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &mpVB, NULL );
#ifdef NUI_PROFILE_DIRECTX
    for (int i=0; i<100; ++i)
      PROFILE_CHRONO_RESET(i);
#endif
  }
}


void nuiD3DPainter::ReleaseDeviceObjects()
{
  //PROFILE_CHRONO_IN(8);
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  //release all textures
  for (std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.begin(); it != mTextures.end() ; ++it)
  {
    it->second.mpTexture->Release();
  }
  mTextures.clear();

  if (mpVB)
  {
    mpVB->Release();
    mpVB = NULL;

  }
  //PROFILE_CHRONO_OUT(8, "ReleaseDeviceObjects");
}


void nuiD3DPainter::SetSize(uint32 w, uint32 h)
{
  //PROFILE_CHRONO_IN(7);
  NUI_RETURN_IF_RENDERING_DISABLED;
  mWidth = w;
  mHeight = h;

  //in case of a size change we have to :
  //release all graphic objects (textures)
  //reset the device
  //textures will be recreated later (on demand)
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  ReleaseDeviceObjects();
  /*
  D3DVIEWPORT9 viewport;

	viewport.X      = 0;
	viewport.Y      = 0;
	viewport.Width  = w;
	viewport.Height = h;
	viewport.MinZ   = 0.0f;
	viewport.MaxZ   = 1.0f;

	HRESULT hr = pDev->SetViewport(&viewport);
  int a=0;
  */
  
  D3DDEVICE_CREATION_PARAMETERS creationParameters;
  ZeroMemory(&creationParameters,sizeof(creationParameters));
  pDev->GetCreationParameters(&creationParameters);

  D3DPRESENT_PARAMETERS presParams;
  ZeroMemory(&presParams,sizeof(presParams));

  presParams.BackBufferCount = 1;
  presParams.Windowed = TRUE;
  //if (rInfo.CopyOnSwap)
  presParams.SwapEffect = D3DSWAPEFFECT_COPY;
  //else
  //presParams.SwapEffect = D3DSWAPEFFECT_FLIP;
  
  presParams.BackBufferFormat = D3DFMT_UNKNOWN;
  presParams.MultiSampleType = D3DMULTISAMPLE_NONE;//;//(D3DMULTISAMPLE_TYPE)rInfo.AASampleCnt;
  
  presParams.EnableAutoDepthStencil = TRUE;
  presParams.AutoDepthStencilFormat = D3DFMT_D24S8;
  //presParams.Flags = D3DPRESENTFLAG_DEVICECLIP;
  presParams.hDeviceWindow = creationParameters.hFocusWindow;//hwnd;
  presParams.BackBufferFormat = D3DFMT_X8R8G8B8;
  presParams.BackBufferWidth = w;
  presParams.BackBufferHeight = h;
  
  //reset the device
  //this succeeds only if all ressources are released before
  hr = pDev->Reset(&presParams); 

  //FIXME
  switch(pDev->TestCooperativeLevel())
  {
    case D3DERR_DEVICELOST: 
      return;// false;
      break;
    case D3DERR_DEVICENOTRESET:
    {
      if(FAILED(pDev->Reset(&presParams)))
      {
        //FIXME : lost device ?
        //MessageBox(NULL, _T("Reset() failed!"), _T("CheckDevice()"), MB_OK);
        return;//false;
      }
    }
      break;
    default:
      break;
  }  
  //PROFILE_CHRONO_OUT(7, "SetSize()");
}



  


void nuiD3DPainter::ClearColor()
{
  //PROFILE_CHRONO_IN(6);
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();

  float alpha = mState.mClearColor.Alpha() * 255.f;
  float red = mState.mClearColor.Red() * 255.f;
  float green = mState.mClearColor.Green() * 255.f;
  float blue = mState.mClearColor.Blue() * 255.f;
  uint8 a = (uint8)alpha; 
  uint8 r = (uint8)red;
  uint8 g = (uint8)green; 
  uint8 b = (uint8)blue;

  //clear only the clip area
  D3DRECT rectClear;
  rectClear.x1 = mClip.Left()-mClipOffsetX;
  rectClear.x2 = mClip.Right()-mClipOffsetX;
  rectClear.y1 = mClip.Top()-mClipOffsetY;
  rectClear.y2 = mClip.Bottom()-mClipOffsetY;
  
  //FIXME
  //HRESULTChecker hr = pDev->Clear(1, &rectClear, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0);
  HRESULTChecker hr = pDev->Clear(1, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0);
 
  NGL_OUT(_T("D3D::Clear()\n"));
  ::Beep(440,1);
  //PROFILE_CHRONO_OUT(6, "Clear color");
}





void nuiD3DPainter::ClearStencil(uint8 value)
{
  //PROFILE_CHRONO_IN(5);
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;

  //clear only the clip area
  D3DRECT rectClear;
  rectClear.x1 = mClip.Left()-mClipOffsetX;
  rectClear.x2 = mClip.Right()-mClipOffsetX;
  rectClear.y1 = mClip.Top()-mClipOffsetY;
  rectClear.y2 = mClip.Bottom()-mClipOffsetY;
  
  hr = pDev->SetRenderState(D3DRS_STENCILMASK, ~0); 
  hr = pDev->Clear(1, &rectClear, D3DCLEAR_STENCIL, 0, 1.0f, value);
  hr = pDev->SetRenderState(D3DRS_STENCILMASK, 0);
  //PROFILE_CHRONO_OUT(5, "Clear Stencil");
}

/*

void nuiD3DPainter::DrawPrimitiveBatch()
{
  mnBatchCurrentVBOffset += mnBatchCurrentVBSize;
  mnBatchCurrentVBSize = 0;
  if (mnBatchCurrentVBOffset >= NUI_VERTEXBUFFER_MAXSIZE)
    mnBatchCurrentVBOffset = 0;
}
*/


void nuiD3DPainter::DrawArray(const nuiRenderArray& rArray)
{
  //NGL_OUT(_T("--------------"));
#ifdef NUI_PROFILE_DIRECTX
  drawArrayCount ++;
#endif
  PROFILE_CHRONO_IN(2);
  HRESULTChecker hr = S_OK;
  //NGL_OUT(_T("DrawArray() "));

  if (!mEnableDrawArray)
    return;
  /*
  PROFILE_CHRONO_IN(18);
  //nuiRenderArray array(rArray);
  PROFILE_CHRONO_OUT(18, "ARRAY COPY");
  */

  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  mRenderOperations++;
  mBatches++;
  uint32 size = rArray.GetSize();
  if (!size)
    return;

  if (mClip.GetWidth() <= 0 || mClip.GetHeight() <= 0)
    return;

  mVertices += size;
  //PROFILE_CHRONO_IN(18);
  NUI_RETURN_IF_RENDERING_DISABLED;

  {  //#FIXME Set up Antialiasing texture
//     if (rArray.UseGLAATexture())
//     {
//       //NGL_OUT(_T("GLAA"));
//       //return; //@@@     
//       //int a=0;
//       //pDev->SetTexture()
//       /*
//       if (!mState.mBlending)
//       //if (1)
//       {
//         //glEnable(GL_BLEND);
//         hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//       }
//       if (mState.mBlendFunc != nuiBlendTransp) 
//       //if (1)
//       {
//         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//         hr = pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//         hr = pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//       }
// 
//       //glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);
//       */
//       /*
//       glMatrixMode(GL_TEXTURE);
//       glPushMatrix();
//       glLoadIdentity();
//       glMatrixMode(GL_MODELVIEW);
//       glPushMatrix();
//       glTranslatef(0.5f, 0.5f, 0);
//       */
//      
//     }
//     else
    if (!mState.mTexturing)
    {
      
      //glPushMatrix();
      //glTranslatef(0.5f, 0.5f, 0);
      //nuiCheckForGLErrors();
      
      int a=0;
    }
  }

    //PROFILE_CHRONO_OUT(18, "INTERNAL RENDER STATE");    
    
    if (rArray.IsArrayEnabled(nuiRenderArray::eTexCoord))
    {
      //NGL_OUT(_T("rArray.UseGLAATexture() "));
      int a=0;
      /*
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(array.GetTexCoordElements(), GL_FLOAT, 0, &array.GetTexCoords()[0]);
      CheckForGLErrors();
      */
    }
    else
    {
      int a=0;
      NGL_OUT(_T("TEXTCOORD ARRAY NOT ENABLED"));
    }
    /*
    else
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      */

    if (rArray.IsArrayEnabled(nuiRenderArray::eNormal))
    {
      //NGL_OUT(_T("ENORMAL ARRAY "));
      int a=0;
      /*
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, 0, &array.GetNormals()[0]);
      CheckForGLErrors();
      */
    }
    /*
    else
      glDisableClientState(GL_NORMAL_ARRAY);
      */

    /*
    if (array.IsArrayEnabled(nuiRenderArray::eEdgeFlag))
    {
      NGL_OUT(_T("EEDGEFLAG ARRAY "));
      int a=0;
      //this->ClearColor();
      //return;
      
    }
    */
    /*
    CheckForGLErrors();
    glDrawArrays(array.GetMode(), 0, size);
    CheckForGLErrors();
    */
  /*}*/

  //hr = pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);//D3DTOP_MODULATE
  //D3DTOP_ADD
  //D3DTOP_MODULATE

  //GL_TRIANGLE_STRIP OK

  //JT
  
  /*
  if (array.GetMode() != GL_TRIANGLES)
    return;
  if (size != 30)
    return;
    */
    
  //if (size != 42) //54 60 108
  //  return;
    
  //if (size > 42)
  //  return;
  //NuiD3DVertex* pBuffer = new NuiD3DVertex[size];
//NGL_OUT(_T("Apply Target: 0x%x\n"), target);

  //PROFILE_CHRONO_IN(18);
  D3DPRIMITIVETYPE primtype = D3DPT_POINTLIST;
  int primitivecount = 0;
  nuiColor c;
  switch (rArray.GetMode())
  {
  case GL_POINTS:
    primtype = D3DPT_POINTLIST;
    c = mState.mStrokeColor;
    primitivecount = size;
    //NGL_OUT(_T("Primitive : Point list %d "), size);
    break;

  case GL_LINES:
    primtype = D3DPT_LINELIST;
    c = mState.mStrokeColor;
    primitivecount = size/2;
    //NGL_OUT(_T("Primitive : Line list %d "), size);
    break;

  case GL_LINE_LOOP:
    //NGL_ASSERT(0);
    //return;
    primtype = D3DPT_LINESTRIP; //#FIXME : close the loop 
    c = mState.mStrokeColor;
    primitivecount = size-1;
    //NGL_OUT(_T("Primitive : Line loop %d "), size);
    break;

  case GL_LINE_STRIP:
    primtype = D3DPT_LINESTRIP;
    c = mState.mStrokeColor;
    primitivecount = size-1;
    //NGL_OUT(_T("Primitive : Line strip %d "), size);
    break;

  case GL_TRIANGLES:
    primtype = D3DPT_TRIANGLELIST;
    c = mState.mFillColor;
    primitivecount = size/3;
    //NGL_OUT(_T("Primitive : Triangle list %d "), size);
    break;

  case GL_TRIANGLE_STRIP:
    primtype = D3DPT_TRIANGLESTRIP;
    c = mState.mFillColor;
    primitivecount = size-2;
    //NGL_OUT(_T("Primitive : Triangle strip %d "), size);
    break;

  case GL_TRIANGLE_FAN:
    primtype = D3DPT_TRIANGLEFAN;
    c = mState.mFillColor;
    primitivecount = size-1;
    //NGL_OUT(_T("Primitive : Triangle fan %d "), size);
    break;

  case GL_QUADS:
    NGL_OUT(_T("Primitive : Quads %d "), size);
    NGL_ASSERT(0);
    break;

  case GL_QUAD_STRIP:
    NGL_OUT(_T("Primitive : Quad strip %d "), size);
    NGL_ASSERT(0);
    break;

  case GL_POLYGON:
    NGL_OUT(_T("Primitive : Polygon %d "), size);
    NGL_ASSERT(0);
    break;
  default:
    NGL_OUT(_T("ERROR : Unknown Primitive !"));
    NGL_ASSERT(0);
    break;
  }

  
  //PROFILE_CHRONO_OUT(18, "PRIMITIVE SELECTION" );
  //PROFILE_CHRONO_IN(18);
  //hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA((uint8)(c.Red()*255.0f), (uint8)(c.Green()*255.0f), (uint8)(c.Blue()*255.0f), (uint8)(c.Alpha()*255.0f)));
  //hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA(255, 0, 0, 255)); 

  const DWORD VertexFVF =	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  hr = pDev->SetFVF(VertexFVF);

  // We need to swap the bytes in the render array because D3D uses ARGB and OpenGL wants RGBA...
  
  //FIXME : permuter les couleurs plus tard
#ifdef FIXME
  if (rArray.IsArrayEnabled(nuiRenderArray::eColor))
  {
    //NGL_OUT(_T("ECOLOR ARRAY --------- "));
    for (uint i = 0; i < size; i++)
    {
      nuiRenderArray::Vertex& v(rArray.GetVertices()[i]);
      const uint8 R = v.mB;
      const uint8 G = v.mG;
      const uint8 B = v.mR;
      const uint8 A = v.mA;
      v.mR = R;
      v.mG = G;
      v.mB = B;
      v.mA = A;
    }
  }
  else
  {
    const uint8 R = /*ToBelow*/(c.Blue()  * 255.0);
    const uint8 G = /*ToBelow*/(c.Green() * 255.0);
    const uint8 B = /*ToBelow*/(c.Red()   * 255.0);
    const uint8 A = /*ToBelow*/(c.Alpha() * 255.0);

    //NGL_OUT(_T("RGBA %d %d %d %d"), R,   G, B, A);
    for (uint i = 0; i < size; i++)
    {
      nuiRenderArray::Vertex& v(rArray.GetVertices()[i]);

      v.mR = R;//R;
      v.mG = G;
      v.mB = B;
      v.mA = A;//A;

    }
    //pDev->SetRenderState(D3DRS_TEXTUREFACTOR,RGB(R,G,B));
  }
#endif

  if (rArray.IsArrayEnabled(nuiRenderArray::eColor))
  {
    NGL_OUT(_T("COLOR ARRAY ENABLED"));
  }
  else
  {
    NGL_OUT(_T("COLOR ARRAY NOT ENABLED"));
    const uint8 R = /*ToBelow*/(c.Blue()  * 255.0);
    const uint8 G = /*ToBelow*/(c.Green() * 255.0);
    const uint8 B = /*ToBelow*/(c.Red()   * 255.0);
    const uint8 A = /*ToBelow*/(c.Alpha() * 255.0);

    //pDev->SetRenderState(D3DRS_TEXTUREFACTOR,RGB(R,G,B));
  }

  //PROFILE_CHRONO_OUT(18, "COLOR PERMUTATION" );
  // #FIXME 
  UINT stride = sizeof(nuiRenderArray::Vertex);
  void* pData = (void*)&rArray.GetVertices()[0];
  //pData = pBuffer;
  //stride = sizeof(NuiD3DVertex);
  //stride = 6*4;
 
  //int a=rand() % 10;
  //if (a == 0)

 //#define DRAW_1
#ifdef DRAW_1
  hr = pDev->DrawPrimitiveUP(primtype, primitivecount, pData, stride);				//draw triangle ( NEW )

#else
  //on copie les datas dans le vertex buffer
  NuiD3DVertex* pVertices = NULL;
  DWORD nSectionSize = size; //vertices count
  DWORD flagLock = 0;
  if ((mnCurrentVBOffset + nSectionSize) <= NUI_VERTEXBUFFER_MAXSIZE)
  {
    flagLock=D3DLOCK_NOOVERWRITE;
  }
  else
  {
    //overflow
    flagLock = D3DLOCK_DISCARD;
    mnCurrentVBOffset = 0;
  }
  //PROFILE_CHRONO_IN(18);
  hr = mpVB->Lock( mnCurrentVBOffset * sizeof(NuiD3DVertex), // Offset to lock
                   nSectionSize * sizeof(NuiD3DVertex),    // Size to lock
                   (void**) &pVertices, 
                   flagLock);
  //PROFILE_CHRONO_OUT(18, "VERTEX BUFFER LOCK");
  //PROFILE_CHRONO_IN(18);

  const uint8 R = /*ToBelow*/(c.Blue()  * 255.0);
  const uint8 G = /*ToBelow*/(c.Green() * 255.0);
  const uint8 B = /*ToBelow*/(c.Red()   * 255.0);
  const uint8 A = /*ToBelow*/(c.Alpha() * 255.0);

  //copie des vertices plus rapide
  //memcpy(pVertices, pData, nSectionSize*sizeof(NuiD3DVertex));
  
  for (int i=0; i<nSectionSize; ++i)
  {
    //copy vertex by vertex
    //memcpy(pVertices+i, (char*)pData+(i*stride), sizeof(NuiD3DVertex));
    NuiD3DVertex* pDst = pVertices+i;
    //bof mais les structures sont compatibles
    NuiD3DVertex* pSrc = (NuiD3DVertex*)((char*)pData+(i*stride));
    pDst->x = pSrc->x;
    pDst->y = pSrc->y;
    pDst->z = pSrc->z;
    
    //if (rArray.IsArrayEnabled(nuiRenderArray::eTexCoord))
   // {
      pDst->tu = pSrc->tu;
      pDst->tv = pSrc->tv;
   // }
    /*
    else
    {
      pDst->tu = (float)(rand()%100)/100.0f;
      pDst->tv = (float)(rand()%100)/100.0f;
      pDst->tu = pSrc->tu*2;
      pDst->tv = pSrc->tv*2;
    }
    */
    
    //pDst->tu = (float)(rand()%100)/100.0f;
    //pDst->tv = (float)(rand()%100)/100.0f;
    
    if (rArray.IsArrayEnabled(nuiRenderArray::eColor))
    {
      ((char*)&pDst->diffuse)[0] = ((char*)&pSrc->diffuse)[2];
      ((char*)&pDst->diffuse)[1] = ((char*)&pSrc->diffuse)[1];
      ((char*)&pDst->diffuse)[2] = ((char*)&pSrc->diffuse)[0];
      ((char*)&pDst->diffuse)[3] = ((char*)&pSrc->diffuse)[3];
    }
    else
    {
      ((char*)&pDst->diffuse)[0] = R;
      ((char*)&pDst->diffuse)[1] = G;
      ((char*)&pDst->diffuse)[2] = B;
      ((char*)&pDst->diffuse)[3] = A;
    }

    
  }
  
  //PROFILE_CHRONO_OUT(18, "MEMCPY VERTICES");
  //memcpy(pVertices, pData, nSectionSize * sizeof(NuiD3DVertex));
  //unlock
  //PROFILE_CHRONO_IN(19);
  hr = mpVB->Unlock();
  //PROFILE_CHRONO_OUT(19, "VERTEX BUFFER UNLOCK");
  //
  //PROFILE_CHRONO_IN(20);
  hr = pDev->DrawPrimitive(primtype, mnCurrentVBOffset, primitivecount);
  //PROFILE_CHRONO_OUT(20, "DRAW PRIMITIVE");
  mnCurrentVBOffset += nSectionSize;

#endif
  if (mState.mAntialiasing)
  {

    //NGL_OUT(_T("ANTIALIASING ENABLED --------- "));
/*
#ifdef NUI_USE_MULTISAMPLE_AA
    glDisable(GL_MULTISAMPLE_ARB);
#else
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif
    CheckForGLErrors();
*/
    NGL_ASSERT(0);
  }
  else
  {
/*
    if (!mState.mTexturing)
    {
      glPopMatrix();
    }
    CheckForGLErrors();
*/
  }

/*
  glColor3f(1.0f, 1.0f, 1.0f);5
  CheckForGLErrors();
*/
//  pDev->SetVertexData4ub(D3DVSDE_DIFFUSE, 255, 255, 255, 255);
  PROFILE_CHRONO_OUT(2);
  //NGL_OUT(_T("--------------"));
}









void nuiD3DPainter::BeginSession()
{
#ifdef DIRECTX_FULL_LOG
  NGL_OUT(_T("BeginSession"));
#endif
/*
  CheckForGLErrors();
*/
  //mpContext->BeginSession();
/*
  CheckForGLErrors();
*/ 
}

void nuiD3DPainter::EndSession()
{
  
  // Bleh!
#ifdef DIRECTX_FULL_LOG
  NGL_OUT(_T("EndSession"));
#endif
  PROFILE_CHRONO_OUT(0);
  PROFILE_CHRONO_LOG(0, "******3D FRAME : start rendering->end session");
  PROFILE_CHRONO_RESET(0);

  PROFILE_CHRONO_LOG(1, "Render states");
  PROFILE_CHRONO_RESET(1);

  PROFILE_CHRONO_LOG(2, "Render array");
  PROFILE_CHRONO_RESET(2);
#ifdef NUI_PROFILE_DIRECTX
  NGL_OUT(_T("DRAWARRAY COUNT : %d"), drawArrayCount);
  NGL_OUT(_T("SETSTATE COUNT : %d"), setStateCount);
#endif
}

void nuiD3DPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::LoadMatrix(rMatrix);
/*
  glLoadMatrixf(rMatrix.Array);
  CheckForGLErrors();
*/
  LoadCurrentMatrix();
}

void nuiD3DPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  nuiPainter::MultMatrix(rMatrix);
/*
  glMultMatrixf(rMatrix.Array);
  CheckForGLErrors();
*/
  LoadCurrentMatrix();
}

void nuiD3DPainter::PushMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  nuiPainter::PushMatrix();
/*
  glPushMatrix();
  CheckForGLErrors();
*/
}

void nuiD3DPainter::PopMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  nuiPainter::PopMatrix();
  LoadCurrentMatrix();
}

void nuiD3DPainter::LoadCurrentMatrix()
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  D3DMATRIX m;
  nuiMatrix rM(mMatrixStack.top());
  rM.Transpose();
  ConvertMatrix(m, rM);
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  pDev->SetTransform(D3DTS_WORLD, &m);
}

void nuiD3DPainter::ReleaseCacheObject(void* pHandle)
{
  //?
}


nglContext* nuiD3DPainter::GetContext() const
{
  return mpContext;
}

nuiD3DPainter::TextureInfo::TextureInfo()
{
  mReload = false;
  mpTexture = NULL;
}

void nuiD3DPainter::CreateTexture(nuiTexture* pTexture)
{

}

void nuiD3DPainter::UploadTexture(nuiTexture* pTexture)
{
	//PROFILE_CHRONO_IN(4);
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  float Width = pTexture->GetWidth();
  float Height = pTexture->GetHeight();
  GLenum target = GetTextureTarget(pTexture->IsPowerOfTwo());
  bool changedctx = false;

  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    it = mTextures.insert(mTextures.begin(), std::pair<nuiTexture*, TextureInfo>(pTexture, TextureInfo()));
  NGL_ASSERT(it != mTextures.end());

  TextureInfo& info(it->second);

  if (!pTexture->IsPowerOfTwo())
  {
    switch (GetRectangleTextureSupport())
    {
    case 0:
      Width = pTexture->GetWidthPOT();
      Height = pTexture->GetHeightPOT();
      break;
    case 1:
    case 2:
      break;
    }
  }

  //NGL_OUT(_T("Apply Target: 0x%x\n"), target);
  nglImage* pImage = pTexture->GetImage();
  {
    bool firstload = false;
    bool reload = info.mReload;
    if (!pImage || !pImage->GetPixelSize())
      return;

    uint i;

    int bytes = 0;

		D3DFORMAT d3dTextureFormat = D3DFMT_UNKNOWN;
		nglImageBufferFormat bufferFormat = pImage->GetBufferFormat();
    nglImagePixelFormat pixelFormat = pImage->GetPixelFormat();
    uint type = pImage->GetBitDepth();

    /*
    if (type == 32)
	  { 
      return;
    }
    */
    if (type == 8)
	  {  
		  bytes = 1;
      if (pixelFormat == eImagePixelIndex)
      {
        d3dTextureFormat = D3DFMT_P8;
      }
      else if (pixelFormat == eImagePixelLum)
      {
        d3dTextureFormat = D3DFMT_L8;
      }
      else if (pixelFormat == eImagePixelAlpha)
      {
        d3dTextureFormat = D3DFMT_A8;
      }
      else
      {
        NGL_ASSERT(0);
      }
	  }
    else if (type==16 || type==15)
	  {  
		  bytes = 2;
		  d3dTextureFormat = D3DFMT_A1R5G5B5;
	  }
    else if (type==24)
	  {
		  bytes = 3;
      if (pixelFormat == eImagePixelRGB)
      {
        d3dTextureFormat = D3DFMT_X8R8G8B8;
      }
      else
      {
        NGL_ASSERT(0);
      }
	  }
    else if (type==32)
	  {
        bytes = 4;
        if (pixelFormat == eImagePixelRGBA)
        {
          d3dTextureFormat = D3DFMT_A8R8G8B8;
        }
        else
        {
          NGL_ASSERT(0);
        }
	  }

    if (info.mpTexture == NULL)
    { 
      NGL_OUT(_T("CreateTexture format %d\n"), d3dTextureFormat);
      //we have to (re)create the texture
      //D3DPOOL_MANAGED ne fonctionne pas avec D3DUSAGE_DYNAMIC ?
	    //FIXME : créer la texture uniquement la première fois 
      //- les fois suivantes, on accède à la texture existante 
      //(équivalent glTexImage2D et glTexSubImage2D)
      //
      /*
      if (d3dTextureFormat == D3DFMT_R8G8B8)
      {
        d3dTextureFormat = D3DFMT_A8R8G8B8;
      }
      */
      hr = pDev->CreateTexture(Width, Height, 1, D3DUSAGE_DYNAMIC, d3dTextureFormat, D3DPOOL_DEFAULT, &info.mpTexture, NULL);
      firstload = true;
      reload = true;
    }

    if (reload)
    {
      //we have to reload texture content
      D3DLOCKED_RECT lockedRect;
		  hr = info.mpTexture->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
	    char* p = (char*) lockedRect.pBits;
	    int tex_width = lockedRect.Pitch/4;
		  int pixelSize = pImage->GetPixelSize();
		  char* pBits = pImage->GetBuffer();
		  if (pBits && p)
		  {	
			  if (d3dTextureFormat == D3DFMT_A8R8G8B8)
			  {
				  int dst, src;
				  for (int h=0;h<Height ; ++h)
				  {
					  for (int w=0 ; w<Width ; ++w)
					  {
						  dst = w+h*tex_width;
						  src = w + h*Width;
						  //p[dst*4+0] = pBits[src*4+1];
						  //p[dst*4+1] = pBits[src*4+2];
						  //p[dst*4+2] = pBits[src*4+3];
						  //p[dst*4+3] = pBits[src*4+0];

//#ifdef OLD
              p[dst*4+0] = pBits[src*4+2];
						  p[dst*4+1] = pBits[src*4+1];
						  p[dst*4+2] = pBits[src*4+0];
						  p[dst*4+3] = pBits[src*4+3];
//#endif
               
              /*
              p[dst*4+0] = 0;
						  p[dst*4+1] = 0;
						  p[dst*4+2] = 0;
						  p[dst*4+3] = pBits[src*4+0];
              */
              /*
              p[dst*4+0] = pBits[(src*4+1];
						  p[dst*4+1] = pBits[src*4+2];
						  p[dst*4+2] = pBits[src*4+3];
						  p[dst*4+3] = pBits[src*4+0];
              */
              
              //rand()%255
              /*
              p[dst*4+0] = rand()%255;
						  p[dst*4+1] = rand()%255;
						  p[dst*4+2] = rand()%255;
						  p[dst*4+3] = rand()%255;
              */
              
              
              /*
              const uint8 R = v.mB;
        const uint8 G = v.mG;
        const uint8 B = v.mR;
        const uint8 A = v.mA;
        */  
              //p[dst*4+0] = 0; //B
						  //p[dst*4+1] = 0; //green
						  //p[dst*4+2] = 255; //R
						  //p[dst*4+3] = 200; //A
              
					  }
            
				  }
        
			}
			else
			{
				//memset(p, 0xFF, Height*Width*bytes);
				memcpy(p, pImage->GetBuffer(), Height*Width*bytes);
      }
			info.mpTexture->UnlockRect(0);
			}
      else
      {
        int a=0;
      }



      GLuint pixelformat = pImage->GetPixelFormat();

//       {
//         glTexImage2D
//           (  
//           target,
//           0,
//           pImage->GetPixelFormat(),
//           (int)Width,
//           (int)Height,
//           0,
//           pixelformat,
//           type,
//           pBuffer
//           );
//         nuiCheckForGLErrors();
//       }

      info.mReload = false;
      
      /*
      GLbyte* pBuffer = NULL;
      bool allocated = false;

      pBuffer = (GLbyte*)pImage->GetBuffer();

      if (!GetRectangleTextureSupport())
      {
        if (!pTexture->IsPowerOfTwo())
        {        
          pBuffer = (GLbyte*)malloc( (uint)(Width * Height * pImage->GetPixelSize()));
          if (!pBuffer) 
            return;
          allocated = true;
          memset(pBuffer,0, (uint)(Width*Height * pImage->GetPixelSize()));

          for (i=0; i < pImage->GetHeight(); i++)
          {

            memcpy
              (
              pBuffer + (uint)(Width * i * pImage->GetPixelSize()), 
              pImage->GetBuffer() + pImage->GetBytesPerLine()*i, 
              pImage->GetBytesPerLine()
              );
          }
        }
        
      }
      */
	
	  //création de la texture D3D


      //if (allocated)
      //  free(pBuffer);
    }
  }
  //FIXME : à globaliser ?
	hr = pDev->SetSamplerState(0, D3DSAMP_MINFILTER, nuiGetTextureFilteringTypeD3D(pTexture->GetMinFilter()));
	hr = pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, nuiGetTextureFilteringTypeD3D(pTexture->GetMagFilter()));
	hr = pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, nuiGetTextureAdressModeD3D(pTexture->GetWrapS()));
	hr = pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, nuiGetTextureAdressModeD3D(pTexture->GetWrapT()));
	
  //TO DO : env mode ?
//   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
//   nuiCheckForGLErrors();
// 
	//?
//   if (changedctx)
//   {
//     mpContext->BeginSession();
//     nuiCheckForGLErrors();
//     glBindTexture(target, info.mpTexture);
//     nuiCheckForGLErrors();
//     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
//     nuiCheckForGLErrors();
//   }
// 
//   glMatrixMode(GL_TEXTURE);
//   nuiCheckForGLErrors();
//   glLoadIdentity();
//   nuiCheckForGLErrors();
// 
//   uint32 rectangle = GetRectangleTextureSupport();
//   nuiCheckForGLErrors();
//   if (rectangle != 1)
//   {
//     double rx = pTexture->GetWidth() / Width;
//     double ry = pTexture->GetHeight() / Height;
//     if (target == GL_TEXTURE_RECTANGLE_ARB)
//     {
//       rx *= pTexture->GetWidth();
//       ry *= pTexture->GetHeight();
//     }
// 
//     glScaled(rx, ry, 1);
//     nuiCheckForGLErrors();
//   }
// 
//   glMatrixMode(GL_MODELVIEW);
//   nuiCheckForGLErrors();

  //PROFILE_CHRONO_OUT(4, "UploadTexture()");
}



void nuiD3DPainter::DestroyTexture(nuiTexture* pTexture)
{
  //PROFILE_CHRONO_IN(11);
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return; //not found
  NGL_ASSERT(it != mTextures.end());
  TextureInfo& info(it->second);
  info.mpTexture->Release();
  mTextures.erase(it);
  //PROFILE_CHRONO_OUT(11, "DestroyTexture()");
}



void nuiD3DPainter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{
  //PROFILE_CHRONO_IN(12);
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return; //not found
  NGL_ASSERT(it != mTextures.end());
  TextureInfo& info(it->second);
  info.mReload = true;
  if (!ForceReload && info.mpTexture != NULL)
  {
//     glDeleteTextures(1, &info.mTexture);
    info.mpTexture->Release();
    info.mpTexture = NULL;
  }
  //PROFILE_CHRONO_OUT(12, "InvalidateTexture()");
}

GLenum nuiD3DPainter::GetTextureTarget(bool POT) const
{
  return GL_TEXTURE_2D; //FIXME
}

void nuiD3DPainter::ApplyTexture(const nuiRenderState& rState, bool ForceApply)
{
	LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
	/*
	
	if (ForceApply || mState.mTexturing != rState.mTexturing)
	  {
		mState.mTexturing = rState.mTexturing;
		if (mState.mTexturing)
		  pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		else
		  pDev->SetTexture(0, NULL);
	  }

	  if (mState.mpTexture != rState.mpTexture || (mState.mpTexture && !mState.mpTexture->IsUptoDate()))
	  { 
		if (mState.mpTexture)
		{
		  // FIXME!!! mState.mpTexture->UnapplyD3D(this);
		  mState.mpTexture->Release();
		}
	    
		mState.mpTexture = rState.mpTexture ;
	    
		if (mState.mpTexture)
		{
		  mState.mpTexture->Acquire();
		  std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
		  //if (it)
		  {
			  IDirect3DTexture9* pTexture = it->second.mpTexture;
				pDev->SetTexture(0, pTexture);
				//it->second.mTexture
		  }
		  

		  // FIXME!!! mState.mpTexture->ApplyD3D(this);
		  //mState.mpTexture->ApplyD3D(this);
		  
		  //if (mState.mpShader)
		  //  mState.mpShader->SetTexture2D(0, mState.mpTexture->GetGLTexture());
		}
	  }
	  */



	
	
	// 2D Textures: 
  std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
  bool uptodate = (it == mTextures.end()) ? false : ( !it->second.mReload && it->second.mpTexture != NULL );
  if (ForceApply || (mState.mpTexture != rState.mpTexture) || (mState.mpTexture && !uptodate))
  { 
    //GLenum intarget = 0;
    //GLenum outtarget = 0;

    /*
    if (mState.mpTexture)
    {      
      //outtarget = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());
	  mState.mpTexture->Release();
      //mState.mpTexture->UnapplyGL(this); #TODO Un apply the texture
      //nuiCheckForGLErrors();
      //mState.mpTexture->Release();
      //nuiCheckForGLErrors();
    }
    */

    mState.mpTexture = rState.mpTexture ;

    if (mState.mpTexture)
    {
      //intarget = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());

      mState.mpTexture->Acquire();
      
      UploadTexture(mState.mpTexture);
	  std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
	  IDirect3DTexture9* pTexture = it->second.mpTexture;
	  hr = pDev->SetTexture(0, it->second.mpTexture);
    

      //nuiCheckForGLErrors();
      /*
	  if (mState.mpShader)
      {
        std::map<nuiTexture*, TextureInfo>::const_iterator it = mTextures.find(rState.mpTexture);
        mState.mpShader->SetTexture2D(0, it->second.mTexture);
      }
      nuiCheckForGLErrors();
	  */
    }
    else
    {
      hr = pDev->SetTexture(0, NULL); //disable texturing
    }

    //NGL_OUT(_T("Change texture from 0x%x to 0x%x\n"), outtarget, intarget);
/*
    mTextureTarget = intarget;
    if (intarget != outtarget)
    {
      // Texture Target has changed
      if (outtarget)
      {
        glDisable(outtarget);
        nuiCheckForGLErrors();
      }
      //NGL_OUT(_T("disable outtarget\n"));
      if (intarget && mState.mTexturing && mState.mpTexture)
      {
        mState.mTexturing = rState.mTexturing;
        //NGL_OUT(_T("enable intarget\n"));
        glEnable(intarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      // Texture Target have not changed     
      if (mState.mTexturing != rState.mTexturing) // Have texture on/off changed?
      {
        // Should enable or disable texturing
        mState.mTexturing = rState.mTexturing;
        if (mState.mTexturing)
        {
          glEnable(mTextureTarget);
          nuiCheckForGLErrors();
        }
        else
        {
          glDisable(mTextureTarget);
          nuiCheckForGLErrors();
        }
      }
    }
  }

  if (ForceApply || (mState.mTexturing != rState.mTexturing))
  {
    // Texture have not changed, but texturing may have been enabled / disabled
    mState.mTexturing = rState.mTexturing;

    GLenum target = 0;
    if (mState.mpTexture)
    {
      target = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());

      if (target && mState.mTexturing)
      {
        //NGL_OUT(_T("Enable 0x%x\n"), target);
        glEnable(mTextureTarget);
        nuiCheckForGLErrors();
      }
      else
      {
        //NGL_OUT(_T("Disable 0x%x\n"), target);
        glDisable(mTextureTarget);
        nuiCheckForGLErrors();
      }
    }
    else
    {
      //NGL_OUT(_T("Disable 0x%x\n"), target);
      if (mTextureTarget)
        glDisable(mTextureTarget);
      nuiCheckForGLErrors();
    }
	*/
  } 
}

uint32 nuiD3DPainter::GetRectangleTextureSupport() const
{
  return mCanRectangleTexture;
}

#endif //   #ifndef __NUI_NO_D3D__