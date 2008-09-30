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

#ifndef __NUI_NO_D3D__
#define NUI_RETURN_IF_RENDERING_DISABLED
#define NUI_USE_MULTISAMPLE_AA
//#define NUI_USE_GL_VERTEX_BUFFER
#define NUI_COMPLEX_SHAPE_THRESHOLD 6
 

//***************************
//jonathan - TO DO LIST
//non native directx primitives support : quads, quad strips, polygons
//rArray.UseGLAATexture - glAA !
//lost device, recreation(CTRL-ALT-SUPPR ...) - device release
//textures : 15 and 16 bits formats
//optimisation (DrawPrimitiveUP ? renderstates ? ...)
//
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
  HRESULTChecker hr = S_OK;
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  //mpContext->BeginSession();
  NUI_RETURN_IF_RENDERING_DISABLED;
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
/*
  //#FIXME: recreate this code without D3DX in order to minimize static deps on the DX DLLs...
  D3DXMatrixIdentity(&id);
  D3DXMatrixTranslation(&tr, -1.0f, 1.0f, 0.0f);
  D3DXMatrixScaling(&sc, 2.0f/(float)mWidth, -2.0f/(float)mHeight, 1.0f);
  projection = sc * tr;
  hr = pDev->SetTransform(D3DTS_PROJECTION, &projection);
*/
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
  
  //hr = pDev->SetRenderState( D3DRS_COLORVERTEX, TRUE );
  //hr = pDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );


  //test JT
  //hr = pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
/*
  glMatrixMode(GL_PROJECTION);
  glViewport(0,0, mWidth, mHeight);
  CheckForGLErrors();
  glLoadIdentity();
  //  glScalef (1, -1, 1);
  glTranslatef(-1.0f, 1.0f, 0.0f );
  glScalef (2.0f/(float)mWidth, -2.0f/(float)mHeight, 1.0f);
  CheckForGLErrors();

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  CheckForGLErrors();
*/
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
  NGL_OUT(_T("SetState() "));
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
      //glEnable(GL_BLEND);
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
    mState.mStencilMode = rState.mStencilMode;
    mState.mStencilValue = rState.mStencilValue;
    switch (mState.mStencilMode)
    {
    case nuiIgnoreStencil:
      NGL_OUT(_T("nuiIgnoreStencil Value %d "), mState.mStencilValue);
		  //glDisable(GL_STENCIL_TEST); //disabling
		  //glStencilMask(0);           //comparison mask
		  //glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255); //test function
		  //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //sfail, spass zfail, spass zpass
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
      //glEnable(GL_STENCIL_TEST);
      //glStencilMask(0);
      //glStencilFunc(GL_EQUAL, mState.mStencilValue, 255);
      //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
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
      //glEnable(GL_STENCIL_TEST);
      //glStencilMask(~0);
      //glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      //glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
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
      //glEnable(GL_STENCIL_TEST);
      //glStencilMask(~0);
      //glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      //glStencilOp(GL_INCR, GL_INCR, GL_INCR);
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
    //CheckForGLErrors();
  }
//#endif
  // We don't care about the font in the lower layer of rendering
  //nuiFont* mpFont;

  //----------------------------
  //shaders (not used) ---------
  //----------------------------
  if (ForceApply || mState.mpShader != rState.mpShader)
  {
    NGL_ASSERT(0);
  }


  // 2D Textures:
  //#TODO
  /*
  //déplacé dans ApplyTexture
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
      // FIXME!!! mState.mpTexture->ApplyD3D(this);
      //if (mState.mpShader)
      //  mState.mpShader->SetTexture2D(0, mState.mpTexture->GetGLTexture());
    }
  }
  */

  ApplyTexture(rState, ForceApply);

  //----------------------------
  //rendering color buffer -----
  //----------------------------
//#ifdef COLORBUFFER_ENABLED
  if (ForceApply || mState.mColorBuffer != rState.mColorBuffer)
  {
	  NGL_OUT(_T("ColorBuffer Value %d "), mState.mColorBuffer);
    /*
	  mState.mColorBuffer = rState.mColorBuffer;
    GLboolean m = mState.mColorBuffer?GL_TRUE:GL_FALSE;
    glColorMask(m, m, m, m);
    CheckForGLErrors();
	  */
	  mState.mColorBuffer = rState.mColorBuffer;
    if (mState.mColorBuffer == GL_TRUE)
	  {
      //ARVB mask 
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
//#ifdef SCISSORCLIPPING_ENABLED
  if (mClip.mEnabled)    
  {
    
    hr = pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    nuiRect clip(mClip);
    clip.Move(-mClipOffsetX, -mClipOffsetY);
    RECT r = {ToBelow(clip.Left()),
      ToBelow(clip.Top()),
      ToBelow(clip.Left() + clip.GetWidth()),
      ToBelow(clip.Top() + clip.GetHeight())};
    hr = pDev->SetScissorRect(&r);
    NGL_OUT(_T("Clip {%f, %f, %f, %f} "), mClip.mLeft, mClip.mTop, mClip.GetWidth(), mClip.GetHeight());
  }
  else
  {
    pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

  }
//#endif
  //----------------------------
  //colors ---------------------
  //----------------------------
  mState.mClearColor = rState.mClearColor;
  mState.mStrokeColor = rState.mStrokeColor;
  mState.mFillColor = rState.mFillColor;
  
  hr = pDev->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255)); //@@@
  hr = pDev->SetRenderState(D3DRS_LIGHTING,FALSE); 
  hr = pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

  //hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1); //D3DTOP_ADD //D3DTOP_MODULATE
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
  //hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1); //@@@
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); //ADD : black SUBTRACT : texte n'apparait pas
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE); //@@@
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE); //@@@
  if (mState.mTexturing)
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
  else
    hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    
  NGL_OUT(_T("\n"));
}







void nuiD3DPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  mWidth = w;
  mHeight = h;

  //in case of a size change we have to :
  //release all graphic objects (textures)
  //reset the device
  //textures will be recreated later (on demand)
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = S_OK;
  //release all textures
  for (std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.begin(); it != mTextures.end() ; ++it)
  {
    it->second.mpTexture->Release();
  }
  mTextures.clear();
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
}



  


void nuiD3DPainter::ClearColor()
{
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
   
  HRESULTChecker hr = pDev->Clear(1, &rectClear, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0);
 
  NGL_OUT(_T("D3D::Clear()\n"));
}





void nuiD3DPainter::ClearStencil(uint8 value)
{
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
  
  //glStencilMask(~0);
  hr = pDev->SetRenderState(D3DRS_STENCILMASK, ~0); 
  hr = pDev->Clear(1, &rectClear, D3DCLEAR_STENCIL, 0, 1.0f, value);
  //glStencilMask(0);
  hr = pDev->SetRenderState(D3DRS_STENCILMASK, 0);
  
}





void nuiD3DPainter::DrawArray(const nuiRenderArray& rArray)
{
  HRESULTChecker hr = S_OK;
  NGL_OUT(_T("DrawArray() "));

  if (!mEnableDrawArray)
    return;
  
  nuiRenderArray array(rArray);

  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  mRenderOperations++;
  mBatches++;
  uint32 size = array.GetSize();
  if (!size)
    return;

  if (mClip.GetWidth() <= 0 || mClip.GetHeight() <= 0)
    return;

  mVertices += size;

  NUI_RETURN_IF_RENDERING_DISABLED;

  {  //#FIXME Set up Antialiasing texture
    if (rArray.UseGLAATexture())
    {
      NGL_OUT(_T("rArray.UseGLAATexture() "));
      //return; //@@@     
      //int a=0;
      //pDev->SetTexture()
      if (!mState.mBlending)
      //if (1)
      {
        //glEnable(GL_BLEND);
        hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      }
      if (mState.mBlendFunc != nuiBlendTransp) 
      //if (1)
      {
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        hr = pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        hr = pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
      }

      //glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);

      /*
      glMatrixMode(GL_TEXTURE);
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glTranslatef(0.5f, 0.5f, 0);
      */
     
    }
    else if (!mState.mTexturing)
    {
      
      //glPushMatrix();
      //glTranslatef(0.5f, 0.5f, 0);
      //nuiCheckForGLErrors();
      
      int a=0;
    }
  }



    
    if (array.IsArrayEnabled(nuiRenderArray::eTexCoord))
    {
      NGL_OUT(_T("rArray.UseGLAATexture() "));
      int a=0;
      /*
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(array.GetTexCoordElements(), GL_FLOAT, 0, &array.GetTexCoords()[0]);
      CheckForGLErrors();
      */
    }
    /*
    else
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      */

    if (array.IsArrayEnabled(nuiRenderArray::eNormal))
    {
      NGL_OUT(_T("ENORMAL ARRAY "));
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
  D3DPRIMITIVETYPE primtype = D3DPT_POINTLIST;
  int primitivecount = 0;
  nuiColor c;
  switch (array.GetMode())
  {
  case GL_POINTS:
    primtype = D3DPT_POINTLIST;
    c = mState.mStrokeColor;
    primitivecount = size;
    NGL_OUT(_T("Primitive : Point list %d "), size);
    break;

  case GL_LINES:
    primtype = D3DPT_LINELIST;
    c = mState.mStrokeColor;
    primitivecount = size/2;
    NGL_OUT(_T("Primitive : Line list %d "), size);
    break;

  case GL_LINE_LOOP:
    //NGL_ASSERT(0);
    //return;
    primtype = D3DPT_LINESTRIP; //#FIXME : close the loop 
    c = mState.mStrokeColor;
    primitivecount = size-1;
    NGL_OUT(_T("Primitive : Line loop %d "), size);
    break;

  case GL_LINE_STRIP:
    primtype = D3DPT_LINESTRIP;
    c = mState.mStrokeColor;
    primitivecount = size-1;
    NGL_OUT(_T("Primitive : Line strip %d "), size);
    break;

  case GL_TRIANGLES:
    primtype = D3DPT_TRIANGLELIST;
    c = mState.mFillColor;
    primitivecount = size/3;
    NGL_OUT(_T("Primitive : Triangle list %d "), size);
    break;

  case GL_TRIANGLE_STRIP:
    primtype = D3DPT_TRIANGLESTRIP;
    c = mState.mFillColor;
    primitivecount = size-2;
    NGL_OUT(_T("Primitive : Triangle strip %d "), size);
    break;

  case GL_TRIANGLE_FAN:
    primtype = D3DPT_TRIANGLEFAN;
    c = mState.mFillColor;
    primitivecount = size-1;
    NGL_OUT(_T("Primitive : Triangle fan %d "), size);
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

  
  
  //c.Set(1, 0, 0, 0.0);
  //
  /*
  if (c.Red() != c.Blue())
  {
    int a=0;
  }
  else
    return;
    */
    
    
  
  //hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA((uint8)(c.Red()*255.0f), (uint8)(c.Green()*255.0f), (uint8)(c.Blue()*255.0f), (uint8)(c.Alpha()*255.0f)));
  //hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA(255, 0, 0, 255)); 

  const DWORD VertexFVF =	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  hr = pDev->SetFVF(VertexFVF);

  // We need to swap the bytes in the render array because D3D uses ARGB and OpenGL wants RGBA...
  if (array.IsArrayEnabled(nuiRenderArray::eColor))
  {
    NGL_OUT(_T("ECOLOR ARRAY --------- "));
    for (uint i = 0; i < size; i++)
    {
      nuiRenderArray::Vertex& v(array.GetVertices()[i]);
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
    //if (c.Red() == 1)
    //  int a=0;
    const uint8 R = /*ToBelow*/(c.Blue()  * 255.0);
    const uint8 G = /*ToBelow*/(c.Green() * 255.0);
    const uint8 B = /*ToBelow*/(c.Red()   * 255.0);
    const uint8 A = /*ToBelow*/(c.Alpha() * 255.0);

    NGL_OUT(_T("RGBA %d %d %d %d"), R, G, B, A);
    for (uint i = 0; i < size; i++)
    {
      nuiRenderArray::Vertex& v(array.GetVertices()[i]);

      v.mR = R;//R;
      v.mG = G;
      v.mB = B;
      v.mA = A;//A;

    }
    //pDev->SetRenderState(D3DRS_TEXTUREFACTOR,RGB(R,G,B));
  }

  
  // #FIXME 
  UINT stride = sizeof(nuiRenderArray::Vertex);
  void* pData = &array.GetVertices()[0];
  //pData = pBuffer;
  //stride = sizeof(NuiD3DVertex);
  //stride = 6*4;
  
  // test material
  
  
/*
  D3DMATERIAL9 materialWhite;
	materialWhite.Diffuse.r = 0.0f;
	materialWhite.Diffuse.g = 0.0f;
	materialWhite.Diffuse.b = 0.0f;
	materialWhite.Diffuse.a = 0.0f;
	materialWhite.Ambient.r = 0.0f;
	materialWhite.Ambient.g = 0.0f;
	materialWhite.Ambient.b = 0.0f;
	materialWhite.Ambient.a = 0.0;
	materialWhite.Emissive.r = 0.0;
	materialWhite.Emissive.g = 1.0f;
	materialWhite.Emissive.b = 0.0;
  materialWhite.Emissive.a = 1.0;
	materialWhite.Specular.r = 0.0f;
	materialWhite.Specular.g = 0.0f;
	materialWhite.Specular.b = 0.0f;
	materialWhite.Specular.a = 0.0;
*/
	//hr = pDev->SetMaterial( &materialWhite );

  
  //int a=rand() % 10;
  //if (a == 0)
  hr = pDev->DrawPrimitiveUP(primtype, primitivecount, pData, stride);				//draw triangle ( NEW )

  //delete[] pBuffer;
  if (mState.mAntialiasing)
  {

    NGL_OUT(_T("ANTIALIASING ENABLED --------- "));
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
  NGL_OUT(_T("\n"));
}









void nuiD3DPainter::BeginSession()
{
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
        d3dTextureFormat = D3DFMT_R8G8B8;
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
      //we have to (re)create the texture
      //D3DPOOL_MANAGED ne fonctionne pas avec D3DUSAGE_DYNAMIC ?
	    //FIXME : créer la texture uniquement la première fois 
      //- les fois suivantes, on accède à la texture existante 
      //(équivalent glTexImage2D et glTexSubImage2D)
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
		  if (!pBits)
		  int a=0;
		  if (pBits)
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

						  
              p[dst*4+0] = pBits[src*4+2];
						  p[dst*4+1] = pBits[src*4+1];
						  p[dst*4+2] = pBits[src*4+0];
						  p[dst*4+3] = pBits[src*4+3];
              
              /*
              p[dst*4+0] = pBits[src*4+3];
						  p[dst*4+1] = pBits[src*4+2];
						  p[dst*4+2] = pBits[src*4+1];
						  p[dst*4+3] = pBits[src*4+0];
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


//       glPixelStorei(GL_UNPACK_ALIGNMENT,1);

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
	hr = pDev->SetSamplerState(0, D3DSAMP_MINFILTER, nuiGetTextureFilteringTypeD3D(pTexture->GetMinFilter()));
	hr = pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, nuiGetTextureFilteringTypeD3D(pTexture->GetMagFilter()));
	hr = pDev->SetSamplerState(0, D3DSAMP_ADDRESSU, nuiGetTextureAdressModeD3D(pTexture->GetWrapS()));
	hr = pDev->SetSamplerState(0, D3DSAMP_ADDRESSV, nuiGetTextureAdressModeD3D(pTexture->GetWrapT()));
  //D3DSAMP_MIPFILTER
//   glTexParameteri(target, GL_TEXTURE_MIN_FILTER, pTexture->GetMinFilter());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_MAG_FILTER, pTexture->GetMagFilter());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_WRAP_S, pTexture->GetWrapS());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_WRAP_T, pTexture->GetWrapT());
//   nuiCheckForGLErrors();
// 
	//TO DO : env mode
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

}



void nuiD3DPainter::DestroyTexture(nuiTexture* pTexture)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return; //not found
  NGL_ASSERT(it != mTextures.end());
  TextureInfo& info(it->second);
  info.mpTexture->Release();
  mTextures.erase(it);
}



void nuiD3DPainter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{
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

    if (mState.mpTexture)
    {      
      //outtarget = GetTextureTarget(mState.mpTexture->IsPowerOfTwo());
	  mState.mpTexture->Release();
      //mState.mpTexture->UnapplyGL(this); #TODO Un apply the texture
      //nuiCheckForGLErrors();
      //mState.mpTexture->Release();
      //nuiCheckForGLErrors();
    }

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




#endif //   #ifndef __NUI_NO_D3D__
