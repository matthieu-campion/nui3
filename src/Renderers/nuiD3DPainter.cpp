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
      /*
      case GL_NO_ERROR:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, "error has been recorded. The value of this symbolic constant is guaranteed to be zero.");
      */
      break;
    case GL_INVALID_ENUM: 
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("An unacceptable value is specified for an enumerated argument. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_VALUE: 
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("A numeric argument is out of range. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_INVALID_OPERATION:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("The specified operation is not allowed in the current state. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_OVERFLOW:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack overflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_STACK_UNDERFLOW:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("This function would cause a stack underflow. The offending function is ignored, having no side effect other than to set the error flag."));
      break;
    case GL_OUT_OF_MEMORY:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("There is not enough memory left to execute the function. The state of OpenGL is undefined, except for the state of the error flags, after this error is recorded."));
      break;
    case S_OK:
      break;
    default:
      NGL_LOG(_T("nuiD3DPainter"), NGL_LOG_ALWAYS, _T("Unknown Direct3D Error."));
      break;
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
}

nuiD3DPainter::~nuiD3DPainter()
{
  mActiveContexts--;
  if (mActiveContexts == 0)
    glAAExit();
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
  hr = pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
  hr = pDev->SetRenderState(D3DRS_STENCILENABLE, false);
  hr = pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
  hr = pDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
  hr = pDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  hr = pDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

/*
void nuiD3DPainter::BeginSession()
{ 
  mpContext->BeginSession();
}


void nuiD3DPainter::StopRendering()
{
  //glFlush();

}

*/

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



void nuiD3DPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  NUI_RETURN_IF_RENDERING_DISABLED;
  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();

  //CheckForGLErrors();

  // blending
  if (ForceApply || mState.mBlending != rState.mBlending)
  {
    mState.mBlending = rState.mBlending;
    if (mState.mBlending)
    {
      //glEnable(GL_BLEND);
      pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

      if (ForceApply || mState.mBlendFunc != rState.mBlendFunc)
      {
        mState.mBlendFunc = rState.mBlendFunc;
        DWORD src, dst;
        nuiGetBlendFuncFactorsD3D(rState.mBlendFunc, src, dst);

        pDev->SetRenderState(D3DRS_SRCBLEND, src);
        pDev->SetRenderState(D3DRS_DESTBLEND, dst);
      }
    }
    else
    {
      pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    }
  }

  // Stencil test:
/* 
  //#TODO
  if (ForceApply || mState.mStencilMode != rState.mStencilMode || mState.mStencilValue != rState.mStencilValue)
  {
    mState.mStencilMode = rState.mStencilMode;
    mState.mStencilValue = rState.mStencilValue;
    switch (mState.mStencilMode)
    {
    case nuiIgnoreStencil:
      //NGL_OUT(_T("nuiIgnoreStencil Value %d\n"), mState.mStencilValue);
      glDisable(GL_STENCIL_TEST);
      glStencilMask(0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      break;
    case nuiReadStencil:
      //NGL_OUT(_T("nuiReadStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(0);
      glStencilFunc(GL_EQUAL, mState.mStencilValue, 255);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      break;
    case nuiAddToStencil:
      //NGL_OUT(_T("nuiAddToStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(~0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      break;
    case nuiBlendToStencil:
      //NGL_OUT(_T("nuiBlendToStencil Value %d\n"), mState.mStencilValue);
      glEnable(GL_STENCIL_TEST);
      glStencilMask(~0);
      glStencilFunc(GL_ALWAYS, mState.mStencilValue, 255);
      glStencilOp(GL_INCR, GL_INCR, GL_INCR);
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
    CheckForGLErrors();
  }
*/

  // We don't care about the font in the lower layer of rendering
  //nuiFont* mpFont;
  // 

/*
  //#TODO
  if (ForceApply || mState.mpShader != rState.mpShader)
  {
    if (mState.mpShader)
      mState.mpShader->UnBind();

    mState.mpShader = rState.mpShader; 

    if (mState.mpShader)
      mState.mpShader->Bind();
    CheckForGLErrors();
  }
*/

  // 2D Textures:
  //#TODO
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

  // Rendering buffers:
/*
  //#TODO
  if (ForceApply || mState.mColorBuffer != rState.mColorBuffer)
  {
    mState.mColorBuffer = rState.mColorBuffer;
    GLboolean m = mState.mColorBuffer?GL_TRUE:GL_FALSE;
    glColorMask(m, m, m, m);
    CheckForGLErrors();
  }
*/

  if (mClip.mEnabled)    
  {
    pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
    nuiRect clip(mClip);
    clip.Move(-mClipOffsetX, -mClipOffsetY);

    RECT r = {ToBelow(clip.Left()),
      ToBelow(clip.Top()),
      ToBelow(clip.Left() + clip.GetWidth()),
      ToBelow(clip.Top() + clip.GetHeight())};
    pDev->SetScissorRect(&r);

    //NGL_OUT(_T("Clip {%f, %f, %f, %f}\n"), mClip.mLeft, mClip.mTop, mClip.GetWidth(), mClip.GetHeight());
  }
  else
    pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, false);

  mState.mClearColor = rState.mClearColor;
  mState.mStrokeColor = rState.mStrokeColor;
  mState.mFillColor = rState.mFillColor;


  pDev->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
  pDev->SetRenderState(D3DRS_LIGHTING,false);
  pDev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

  HRESULTChecker hr = S_OK;

  hr = pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  hr = pDev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
}

void nuiD3DPainter::SetSize(uint32 w, uint32 h)
{
  NUI_RETURN_IF_RENDERING_DISABLED;

  mWidth = w;
  mHeight = h;
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

  HRESULTChecker hr = pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(a, r, g, b), 1.0f, 0);

  //NGL_OUT(_T("D3D::Clear()\n"));
}

void nuiD3DPainter::ClearStencil(uint8 value)
{
  return;
  mRenderOperations++;
  NUI_RETURN_IF_RENDERING_DISABLED;

  LPDIRECT3DDEVICE9 pDev = mpContext->GetDirect3DDevice();
  HRESULTChecker hr = pDev->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, value);
}



void nuiD3DPainter::DrawArray(const nuiRenderArray& rArray)
{
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

/*
  {  //#FIXME Set up Antialiasing texture
    if (rArray.UseGLAATexture())
    {
      pDev->SetTexture()
      pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

      if (!mState.mBlending)
        glEnable(GL_BLEND);
      if (mState.mBlendFunc != nuiBlendTransp)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glBindTexture(GL_TEXTURE_2D, glAA_texture[0]);

      glMatrixMode(GL_TEXTURE);
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode(GL_MODELVIEW);

      glPushMatrix();
      glTranslatef(0.5f, 0.5f, 0);
    }
    else if (!mState.mTexturing)
    {
      glPushMatrix();
      glTranslatef(0.5f, 0.5f, 0);
      nuiCheckForGLErrors();
    }
  }
*/


  /*
    if (array.IsArrayEnabled(nuiRenderArray::eTexCoord))
    {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(array.GetTexCoordElements(), GL_FLOAT, 0, &array.GetTexCoords()[0]);
      CheckForGLErrors();
    }
    else
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if (array.IsArrayEnabled(nuiRenderArray::eNormal))
    {
      glEnableClientState(GL_NORMAL_ARRAY);
      glNormalPointer(GL_FLOAT, 0, &array.GetNormals()[0]);
      CheckForGLErrors();
    }
    else
      glDisableClientState(GL_NORMAL_ARRAY);

    if (array.IsArrayEnabled(nuiRenderArray::eEdgeFlag))
    {
      glEnableClientState(GL_EDGE_FLAG_ARRAY);
      glEdgeFlagPointer(0, &array.GetEdgeFlags()[0]);
      CheckForGLErrors();
    }
    else
      glDisableClientState(GL_EDGE_FLAG_ARRAY);

    CheckForGLErrors();
    glDrawArrays(array.GetMode(), 0, size);
    CheckForGLErrors();
  }
*/

  HRESULTChecker hr = S_OK;

  hr = pDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

  D3DPRIMITIVETYPE primtype = D3DPT_POINTLIST;
  nuiColor c;
  switch (array.GetMode())
  {
  case GL_POINTS:
    primtype = D3DPT_POINTLIST;
    c = mState.mStrokeColor;
    break;
  case GL_LINES:
    primtype = D3DPT_LINELIST;
    c = mState.mStrokeColor;
    size /= 2;
    break;
  case GL_LINE_LOOP:
    primtype = D3DPT_LINELIST; //#FIXME
    c = mState.mStrokeColor;
    size /= 2;
    break;
  case GL_LINE_STRIP:
    primtype = D3DPT_LINESTRIP;
    c = mState.mStrokeColor;
    size -= 1;
    break;

  case GL_TRIANGLES:
    primtype = D3DPT_TRIANGLELIST;
    c = mState.mFillColor;
    size /= 3;
    break;
  case GL_TRIANGLE_STRIP:
    primtype = D3DPT_TRIANGLESTRIP;
    c = mState.mFillColor;
    size -= 2;
    break;
  case GL_TRIANGLE_FAN:
    primtype = D3DPT_TRIANGLEFAN;
    c = mState.mFillColor;
    size -= 1;
    break;
  case GL_QUADS:
  case GL_QUAD_STRIP:
  case GL_POLYGON:
    NGL_ASSERT(0);
    break;
  }

  hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA((uint8)(c.Red()*255.0f), (uint8)(c.Green()*255.0f), (uint8)(c.Blue()*255.0f), (uint8)(c.Alpha()*255.0f)));
  //hr = pDev->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_RGBA(255, 0, 0, 255));

  const DWORD VertexFVF =	D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
  hr = pDev->SetFVF(VertexFVF);

  // We need to swap the bytes in the render array because D3D uses ARGB and OpenGL wants RGBA...
  if (array.IsArrayEnabled(nuiRenderArray::eColor))
  {
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
    const uint8 R = ToBelow(c.Blue()  * 255.0);
    const uint8 G = ToBelow(c.Green() * 255.0);
    const uint8 B = ToBelow(c.Red()   * 255.0);
    const uint8 A = ToBelow(c.Alpha() * 255.0);

    for (uint i = 0; i < size; i++)
    {
      nuiRenderArray::Vertex& v(array.GetVertices()[i]);

      v.mR = R;
      v.mG = G;
      v.mB = B;
      v.mA = A;
    }
  }

  // #FIXME 
  hr = pDev->DrawPrimitiveUP(primtype, size, &array.GetVertices()[0], sizeof(nuiRenderArray::Vertex));				//draw triangle ( NEW )


  if (mState.mAntialiasing)
  {
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
  glColor3f(1.0f, 1.0f, 1.0f);
  CheckForGLErrors();
*/
//  pDev->SetVertexData4ub(D3DVSDE_DIFFUSE, 255, 255, 255, 255);

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

    int type = pImage->GetBitDepth();

    uint i;
    if (info.mpTexture == NULL)
    { 

//       glGenTextures(1, &info.mpTexture);
      firstload = true;
      reload = true;
    }

//     glBindTexture(target, info.mpTexture);
//     nuiCheckForGLErrors();

    if (reload)
    {
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

      if (type == 8)
        type=GL_UNSIGNED_BYTE;
      else if (type==16 || type==15)
        type=GL_UNSIGNED_SHORT_5_5_5_1;
      else if (type==24)
        type=GL_UNSIGNED_BYTE;
      else if (type==32)
        type=GL_UNSIGNED_BYTE;

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

      if (allocated)
        free(pBuffer);
    }
  }

//   glTexParameteri(target, GL_TEXTURE_MIN_FILTER, pTexture->GetMinFilter());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_MAG_FILTER, pTexture->GetMagFilter());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_WRAP_S, pTexture->GetWrapS());
//   nuiCheckForGLErrors();
//   glTexParameteri(target, GL_TEXTURE_WRAP_T, pTexture->GetWrapT());
//   nuiCheckForGLErrors();
// 
//   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, pTexture->GetEnvMode());
//   nuiCheckForGLErrors();
// 
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
    return;
  NGL_ASSERT(it != mTextures.end());

  TextureInfo& info(it->second);

  info.mpTexture->Release();
  mTextures.erase(it);
}

void nuiD3DPainter::InvalidateTexture(nuiTexture* pTexture, bool ForceReload)
{
  std::map<nuiTexture*, TextureInfo>::iterator it = mTextures.find(pTexture);
  if (it == mTextures.end())
    return;
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
  return GL_TEXTURE_2D;
}


#endif //   #ifndef __NUI_NO_D3D__
