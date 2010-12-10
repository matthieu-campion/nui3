/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiMetaPainter.h"
#include "nuiGLPainter.h"
#include "nuiD3DPainter.h"
#include "nuiSoftwarePainter.h"
#include "nuiWidget.h"
#include "nuiTheme.h"
#include "nuiContour.h"
#include "nuiTessellator.h"
#include "nuiOutliner.h"
#include "nuiTexture.h"
#include "nuiSurface.h"

extern float NUI_SCALE_FACTOR;
extern float NUI_INV_SCALE_FACTOR;


/****************************************************************************
 *
 * Constructor / Destructor
 *
 ****************************************************************************/
nuiDrawContext::nuiDrawContext(const nuiRect& rRect)
{
  mWidth = rRect.GetWidth();
  mHeight = rRect.GetHeight();

  mClipOffsetX = mClipOffsetY = 0;

  mPermitAntialising = true;

  mpPainter = NULL;
  mpOldPainter = NULL;
  mpAATexture = nuiTexture::GetAATexture();
  
  mStateChanges = 1;
}

nuiDrawContext::~nuiDrawContext()
{
  SetTexture(NULL);
  SetFont(NULL);
  if (mpAATexture)
    mpAATexture->Release();

  delete mpPainter;
  mpPainter = NULL;
  mpOldPainter = NULL;
  while (!mpRenderStateStack.empty())
  {
    PopState();
  }
}

void nuiDrawContext::StartRendering()
{ 
  mpPainter->StartRendering(); 
}

void nuiDrawContext::BeginSession()
{
  mpPainter->BeginSession();
}

void nuiDrawContext::EndSession()
{
  mpPainter->EndSession();
}

void nuiDrawContext::StopRendering()
{
  SetTexture(NULL);
}


void nuiDrawContext::SetPainter(nuiPainter* pPainter)
{
  mpPainter = pPainter;
}

nuiPainter* nuiDrawContext::GetPainter() const
{
  return mpPainter;
}

void nuiDrawContext::SetState(const nuiRenderState& rState)
{
  mpPainter->SetState(rState);
}

void nuiDrawContext::AddBreakPoint()
{
  mpPainter->AddBreakPoint();
}

/****************************************************************************
 *
 * Clipping Support
 *
 ****************************************************************************/

// Clipping using Scissor :
void nuiDrawContext::PushClipping()
{
  mpPainter->PushClipping();
}

void nuiDrawContext::PopClipping()
{
  mpPainter->PopClipping();
}

bool nuiDrawContext::Clip(const nuiRect& rRect)
{
  nuiRect r;
  mpPainter->GetClipRect(r, true);
//  if (r == rRect)
//    return true;
  
  mpPainter->Clip(rRect);
  mStateChanges++;
  return true;
}

bool nuiDrawContext::ResetClipRect()
{
  mpPainter->ResetClipRect();
  mStateChanges++;
  return true;
}

bool nuiDrawContext::EnableClipping(bool set)
{
//  nuiRect r;
//  if (mpPainter->GetClipRect(r, false) == set)
//    return true;
  
  mpPainter->EnableClipping(set);
  mStateChanges++;
  return true;
}

bool nuiDrawContext::GetClipRect(nuiRect& rRect, bool LocalRect) const
{
  return mpPainter->GetClipRect(rRect, LocalRect);
}

/****************************************************************************
 *
 * Render state manipulation
 *
 ****************************************************************************/
void nuiDrawContext::PushState()
{
  nuiRenderState *pState = new nuiRenderState(mCurrentState);
  mpRenderStateStack.push(pState);
  //mStateChanges++;
}

void nuiDrawContext::PopState()
{
  nuiRenderState* pState = mpRenderStateStack.top();
  if (!(*pState == mCurrentState))
  {
    mStateChanges++;
    mCurrentState = *pState;
  }
  mpRenderStateStack.pop();
  delete pState;
}

const nuiRenderState& nuiDrawContext::GetState() const
{
  return mCurrentState;
}

bool nuiDrawContext::ResetState()
{
  static nuiRenderState Dummy;
  if (!(Dummy == mCurrentState))
  {
    mStateChanges++;
    mCurrentState = Dummy;
  }
  
  return true;
}

void nuiDrawContext::EnableBlending(bool val)       
{
  if (mCurrentState.mBlending != val)
  {
    mCurrentState.mBlending = val; 
    mStateChanges++;
  }
}

void nuiDrawContext::EnableTexturing(bool val)      
{ 
  if (mCurrentState.mTexturing != val)
  {
    mCurrentState.mTexturing = val;
    mStateChanges++;
  }
}

#if 0
void nuiGetBlendFuncFactors(nuiBlendFunc Func, GLenum& src, GLenum& dst)
{
  GLenum sfactor, dfactor;
  switch (Func)
  {
  default:
  case nuiBlendSource:        sfactor = GL_ONE;                   dfactor = GL_ZERO;                 break;
  case nuiBlendTransp:        sfactor = GL_SRC_ALPHA;             dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
  case nuiBlendClear:         sfactor = GL_ZERO;                  dfactor = GL_ZERO;                 break;
  case nuiBlendDest:          sfactor = GL_ZERO;                  dfactor = GL_ONE;                  break;
  case nuiBlendOver:          sfactor = GL_ONE;                   dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
  case nuiBlendOverRev:       sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_ONE;                  break;
  case nuiBlendIn:            sfactor = GL_DST_ALPHA;             dfactor = GL_ZERO;                 break;
  case nuiBlendInRev:         sfactor = GL_ZERO;                  dfactor = GL_SRC_ALPHA;            break;
  case nuiBlendOut:           sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_ZERO;                 break;
  case nuiBlendOutRev:        sfactor = GL_ZERO;                  dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
  case nuiBlendTop:           sfactor = GL_DST_ALPHA;             dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
  case nuiBlendTopRev:        sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_SRC_ALPHA;            break;
  case nuiBlendXOR:           sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
  case nuiBlendAdd:           sfactor = GL_ONE;                   dfactor = GL_ONE;                  break;
  case nuiBlendSaturate:      sfactor = GL_SRC_ALPHA_SATURATE;    dfactor = GL_ONE;                  break;

  case nuiBlendTranspClear:   sfactor = GL_SRC_ALPHA;             dfactor = GL_ZERO;                 break;
  case nuiBlendTranspInRev:   sfactor = GL_SRC_ALPHA;             dfactor = GL_SRC_ALPHA;            break;
  case nuiBlendTranspAdd:     sfactor = GL_SRC_ALPHA;             dfactor = GL_ONE;                  break;
  }
  src = sfactor;
  dst = dfactor;
}
#endif

// Premultiplied Alpha Blending rules:
void nuiGetBlendFuncFactors(nuiBlendFunc Func, GLenum& src, GLenum& dst)
{
  GLenum sfactor, dfactor;
  switch (Func)
  {
    default:
    case nuiBlendSource:        sfactor = GL_ONE;                   dfactor = GL_ZERO;                 break;
    case nuiBlendTransp:        sfactor = GL_ONE;                   dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
    case nuiBlendClear:         sfactor = GL_ZERO;                  dfactor = GL_ZERO;                 break;
    case nuiBlendDest:          sfactor = GL_ZERO;                  dfactor = GL_ONE;                  break;
    case nuiBlendOver:          sfactor = GL_ONE;                   dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
    case nuiBlendOverRev:       sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_ONE;                  break;
    case nuiBlendIn:            sfactor = GL_DST_ALPHA;             dfactor = GL_ONE;                  break;
    case nuiBlendInRev:         sfactor = GL_ZERO;                  dfactor = GL_SRC_ALPHA;            break;
    case nuiBlendOut:           sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_ZERO;                 break;
    case nuiBlendOutRev:        sfactor = GL_ZERO;                  dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
    case nuiBlendTop:           sfactor = GL_DST_ALPHA;             dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
    case nuiBlendTopRev:        sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_SRC_ALPHA;            break;
    case nuiBlendXOR:           sfactor = GL_ONE_MINUS_DST_ALPHA;   dfactor = GL_SRC_ALPHA;            break;
    case nuiBlendAdd:           sfactor = GL_ONE;                   dfactor = GL_ONE;                  break;
//    case nuiBlendSaturate:      sfactor = GL_SRC_ALPHA_SATURATE;    dfactor = GL_ONE;                  break;
    case nuiBlendSaturate:      sfactor = GL_DST_COLOR;             dfactor = GL_ONE_MINUS_SRC_ALPHA;  break;
      
    case nuiBlendTranspClear:   sfactor = GL_SRC_ALPHA;             dfactor = GL_ZERO;                 break;
    case nuiBlendTranspInRev:   sfactor = GL_SRC_ALPHA;             dfactor = GL_SRC_ALPHA;            break;
    case nuiBlendTranspAdd:     sfactor = GL_SRC_ALPHA;             dfactor = GL_ONE;                  break;
  }
  src = sfactor;
  dst = dfactor;
}


void nuiDrawContext::SetBlendFunc(nuiBlendFunc Func)
{
  if (mCurrentState.mBlendFunc != Func)
  {
    mCurrentState.mBlendFunc = Func;
    mStateChanges++;
  }
}

/****************************************************************************
 *
 * Texture manipulation
 *
 ****************************************************************************/

void nuiDrawContext::SetTexture (nuiTexture* pTex) 
{
  nuiTexture* pOld = mCurrentState.mpTexture;
  if (pTex == pOld)
    return;
  
  mCurrentState.mpTexture = pTex ;
  if (pTex)
    pTex->Acquire();
  if (pOld)
    pOld->Release();
  mStateChanges++;
}

bool nuiDrawContext::IsTextureCurrent(nuiTexture* pTex) const
{ 
  return mCurrentState.mpTexture == pTex;
}

nuiTexture* nuiDrawContext::GetTexture() const
{ 
  return mCurrentState.mpTexture; 
}

/****************************************************************************
 *
 * Brush manipulation
 *
 ****************************************************************************/

void nuiDrawContext::SetFillColor(const nuiColor& rColor)
{
  if (!(mCurrentState.mFillColor == rColor))
  {
    mCurrentState.mFillColor = rColor;
    mStateChanges++;
  }
}

void nuiDrawContext::SetStrokeColor(const nuiColor& rColor)
{
  if (!(mCurrentState.mStrokeColor == rColor))
  {
    mCurrentState.mStrokeColor = rColor;
    mStateChanges++;
  }
}

const nuiColor& nuiDrawContext::GetFillColor() const
{
  return mCurrentState.mFillColor;
}

const nuiColor& nuiDrawContext::GetStrokeColor() const
{
  return mCurrentState.mStrokeColor;
}

void nuiDrawContext::SetLineWidth(nuiSize Width)
{
  if (Width < 0.0)
    Width = -Width;
  if (Width == 0.0f)
    Width = 0.0001f;
  mCurrentState.mLineWidth = Width;
//  mStateChanges++;
}

void nuiDrawContext::EnableAntialiasing(bool set)
{
  if (mCurrentState.mAntialiasing != set)
  {
    mCurrentState.mAntialiasing = set;
    mStateChanges++;
  }
}

bool nuiDrawContext::GetAntialiasing() const
{
  return mCurrentState.mAntialiasing;
}

void nuiDrawContext::SetWinding(nuiShape::Winding Rule)
{
  mCurrentState.mWinding = Rule;
//  mStateChanges++;
}

nuiShape::Winding nuiDrawContext::GetWinding() const
{
  return mCurrentState.mWinding;
}

/****************************************************************************
 *
 * Drawing function
 *
 ****************************************************************************/
void nuiDrawContext::SetClearColor(const nuiColor& ClearColor)
{
  if (!(mCurrentState.mClearColor == ClearColor))
  {
    mCurrentState.mClearColor = ClearColor;
    mStateChanges++;
  }
}

void nuiDrawContext::Clear() 
{ 
  if (mStateChanges)
    mpPainter->SetState(mCurrentState);
  mStateChanges = 0;
  mpPainter->ClearColor(); 
}

void nuiDrawContext::DrawShape(nuiShape* pShape, nuiShapeMode Mode, float Quality)
{
  NGL_ASSERT(pShape != NULL);
  PushState();
  switch (Mode)
  {
  case eStrokeShape:
    {
      nuiRenderObject* pObject = pShape->Outline(Quality, mCurrentState.mLineWidth, mCurrentState.mLineJoin, mCurrentState.mLineCap);
      SetFillColor(GetStrokeColor());
      SetTexture(mpAATexture);
      EnableTexturing(true);
      EnableBlending(true);
      SetBlendFunc(nuiBlendTransp);//GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      DrawObject(*pObject);
      delete pObject;
    }
    break;
  case eFillShape:
    {
      nuiTessellator* pTess = new nuiTessellator(pShape);
      pTess->SetFill(true);
      nuiRenderObject* pObject = pTess->Generate(Quality);
      DrawObject(*pObject);
      delete pObject;
      delete pTess;
    }
    break;
  case eStrokeAndFillShape:
    {
      {
        nuiRenderObject* pObject = pShape->Fill(Quality);
        DrawObject(*pObject);
        delete pObject;
      }

      {
        nuiRenderObject* pObject = pShape->Outline(Quality, mCurrentState.mLineWidth, mCurrentState.mLineJoin, mCurrentState.mLineCap);
        SetFillColor(GetStrokeColor());
        SetTexture(mpAATexture);
        EnableTexturing(true);
        EnableBlending(true);
        SetBlendFunc(nuiBlendTransp);//GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        DrawObject(*pObject);
        delete pObject;
      }
    }
    break;
  case eDefault: //?
    break;
  }
  PopState();
}



/****************************************************************************
 *
 * Font manipulation
 *
 ****************************************************************************/

bool nuiDrawContext::SetFont (nuiFont* pFont, bool AlreadyAcquired)
{
  nuiFont* pOld = mCurrentState.mpFont;
  if (pOld == pFont)
    return true;
  
  mCurrentState.mpFont = pFont;
  if (mCurrentState.mpFont && !AlreadyAcquired)
    mCurrentState.mpFont->Acquire();
  if (pOld)
    pOld->Release();
  return true;
}

nuiFont* nuiDrawContext::GetFont() const
{
  return mCurrentState.mpFont;
}

void nuiDrawContext::SetTextColor(const nuiColor& rColor)
{
  mCurrentState.mTextColor = rColor;
}

nuiColor nuiDrawContext::GetTextColor() const
{
  return mCurrentState.mTextColor;
}

void nuiDrawContext::DrawText(nuiSize x, nuiSize y, const nglString& rString, bool AlignGlyphPixels)
{
  mCurrentState.mpFont->Print(this,x,y,rString, AlignGlyphPixels);
}

void nuiDrawContext::DrawText(nuiSize x, nuiSize y, const nuiFontLayout& rLayout, bool AlignGlyphPixels)
{
  mCurrentState.mpFont->Print(this,x,y,rLayout, AlignGlyphPixels);
}

void nuiDrawContext::PermitAntialiasing(bool Set)
{
  mPermitAntialising = Set;
}

bool nuiDrawContext::IsAntialiasingPermited() const
{
  return mPermitAntialising;
}

void nuiDrawContext::DrawImage(const nuiRect& rDest, const nuiRect& rSource)
{
  DrawImageQuad(rDest.mLeft, rDest.mTop, 
                rDest.mRight, rDest.mTop, 
                rDest.mRight, rDest.mBottom, 
                rDest.mLeft, rDest.mBottom, 
                rSource);
}

void nuiDrawContext::DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource)
{
  bool texturing = mCurrentState.mTexturing;
  if (!texturing)
    EnableTexturing(true);

  nuiSize tx,ty,tw,th;

  tx = rSource.mLeft;
  ty = rSource.mTop;
  tw = rSource.mRight;
  th = rSource.mBottom;

  mCurrentState.mpTexture->ImageToTextureCoord(tx, ty);
  mCurrentState.mpTexture->ImageToTextureCoord(tw, th);

  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
  pArray->Reserve(4);
  pArray->EnableArray(nuiRenderArray::eVertex, true);
  pArray->EnableArray(nuiRenderArray::eTexCoord, true);
  pArray->EnableArray(nuiRenderArray::eColor, true);

  // 1
  pArray->SetTexCoords(tx,ty); 
  pArray->SetVertex(x0, y0);
  pArray->SetColor(mCurrentState.mFillColor);
  pArray->PushVertex();

  pArray->SetTexCoords(tw,ty); 
  pArray->SetVertex(x1, y1);
  pArray->PushVertex();

  pArray->SetTexCoords(tx,th); 
  pArray->SetVertex(x3, y3);
  pArray->PushVertex();

  // 2
  pArray->SetTexCoords(tw,th); 
  pArray->SetVertex(x2, y2);
  pArray->PushVertex();

  DrawArray(pArray);

  if (!texturing)
    EnableTexturing(texturing);
}

void nuiDrawContext::DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, const nuiVector2& rP1, const nuiVector2& rP2)
{
  DrawGradient(rGradient, rEnclosingRect, rP1[0], rP1[1], rP2[0], rP2[1]);
}

static void IntersectLines(float x0,float y0,float x1,float y1, 
                           float x2,float y2,float x3,float y3,
                           float& xi,float& yi)
{
  x1 += x0;
  y1 += y0;
  x3 += x2;
  y3 += y2;

  float a1,b1,c1, // constants of linear equations
    a2,b2,c2,
    det_inv,  // the inverse of the determinant of the coefficient
    m1,m2;    // the slopes of each line

  // compute slopes, note the kludge for infinity, however, this will
  // be close enough

  if ((x1-x0)!=0)
    m1 = (y1-y0)/(x1-x0);
  else
    m1 = (float)1e+10;   // close enough to infinity

  if ((x3-x2)!=0)
    m2 = (y3-y2)/(x3-x2);
  else
    m2 = (float)1e+10;   // close enough to infinity

  // compute constants
  a1 = m1;
  a2 = m2;

  b1 = -1;
  b2 = -1;

  c1 = (y0-m1*x0);
  c2 = (y2-m2*x2);

  // compute the inverse of the determinate

  if (m1 >= (float)1e+10 || m1 >= (float)1e+10)
    det_inv = 0;
  else
    det_inv = 1/(a1*b2 - a2*b1);

  // use Kramers rule to compute xi and yi

  xi=((b1*c2 - b2*c1)*det_inv);
  yi=((a2*c1 - a1*c2)*det_inv);

}


void nuiDrawContext::DrawGradient(const nuiGradient& rGradient, const nuiRect& rEnclosingRect, nuiSize x1, nuiSize y1, nuiSize x2, nuiSize y2)
{
  nuiVector2 vec(x2 - x1, y2 - y1);
  nuiVector2 para(-vec[1], vec[0]);
  nuiVector2 vec1(vec);
  nuiVector2 para1(para);
  vec1.Normalize();
  para1.Normalize();

  // What Quadrant are we in?:
  //         |
  //     a   |   b
  //         |
  //  ----------------
  //         |
  //     c   |   d
  //         |
  float xa, xb, xc, xd;
  float ya, yb, yc, yd;
  float x, y;
  float xp, yp;
  float xx, yy;
  float xxp, yyp;

  xa = xc = rEnclosingRect.Left();
  xb = xd = rEnclosingRect.Right();
  ya = yb = rEnclosingRect.Top();
  yc = yd = rEnclosingRect.Bottom();

  if (x1 < x2)
  {
    // Go from a to d or c to b
    if (y1 == y2)
    {
      x  = xa; y  = ya;
      xp = xc; yp = yc;
      xx = xd; yy = yd;
      xxp= xb; yyp= yb;
    }
    else if (y1 < y2)
    {
      // a to d
      IntersectLines(xa,ya, para1[0], para1[1], xb, yb, vec1[0], vec1[1], x, y);
      IntersectLines(xa,ya, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xp, yp);
      IntersectLines(xd,yd, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xx, yy);
      IntersectLines(xd,yd, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xxp, yyp);
    }
    else
    {
      // c to d
      IntersectLines(xc,yc, para1[0], para1[1], xa, ya, vec1[0], vec1[1], x, y);
      IntersectLines(xc,yc, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xp, yp);
      IntersectLines(xb,yb, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xx, yy);
      IntersectLines(xb,yb, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xxp, yyp);
    }
  }
  else
  {
    if (y1 == y2)
    {
      x  = xd; y  = yd;
      xp = xb; yp = yb;
      xx = xa; yy = ya;
      xxp= xc; yyp= yc;
    }
    else if (y1 < y2)
    {
      // b to c
      IntersectLines(xb,yb, para1[0], para1[1], xd, yd, vec1[0], vec1[1], x, y);
      IntersectLines(xb,yb, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xp, yp);
      IntersectLines(xc,yc, para1[0], para1[1], xa, ya, vec1[0], vec1[1], xx, yy);
      IntersectLines(xc,yc, para1[0], para1[1], xd, yd, vec1[0], vec1[1], xxp, yyp);
    }
    else
    {
      // d to a
      IntersectLines(xd,yd, para1[0], para1[1], xc, yc, vec1[0], vec1[1], x, y);
      IntersectLines(xd,yd, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xp, yp);
      IntersectLines(xa,ya, para1[0], para1[1], xb, yb, vec1[0], vec1[1], xx, yy);
      IntersectLines(xa,ya, para1[0], para1[1], xc, yc, vec1[0], vec1[1], xxp, yyp);
    }
  }

  float startx,starty;
  float startxp,startyp;
  float stopx,stopy;
  float stopxp,stopyp;

  if (y1 != y2)
  {
    IntersectLines(x1, y1, para1[0], para1[1], x,  y,  vec1[0], vec1[1], startx,  starty);
    IntersectLines(x1, y1, para1[0], para1[1], xp, yp, vec1[0], vec1[1], startxp, startyp);
    IntersectLines(x2, y2, para1[0], para1[1], x,  y,  vec1[0], vec1[1], stopx,   stopy);
    IntersectLines(x2, y2, para1[0], para1[1], xp, yp, vec1[0], vec1[1], stopxp,  stopyp);
  }
  else
  {
    startx  = x1; starty  = y;
    startxp = x1; startyp = yp;
    stopx   = x2; stopy   = y;
    stopxp  = x2; stopyp  = yp;
  }

  nuiGradientStopList::const_iterator it = rGradient.GetStopList().begin();
  nuiGradientStopList::const_iterator end = rGradient.GetStopList().end();

  float px1, py1;
  float px2, py2;

  PushClipping();
  Clip(rEnclosingRect);
  EnableClipping(true);

  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
  pArray->EnableArray(nuiRenderArray::eVertex);
  pArray->EnableArray(nuiRenderArray::eColor);
  
  //  nuiRenderArray Array(GL_LINES);
//  pArray->SetVertexElements(3);
//  pArray->SetColorElements(4);

  nuiColor col = it->second;
  pArray->SetVertex(x, y);
  pArray->SetColor(col);
  pArray->PushVertex();
  pArray->SetVertex(xp, yp);
  pArray->PushVertex();

  for ( ; it != end; ++it)
  {
    float r = it->first;
    float rm = 1.0f - r;
    px1 = startx * rm + stopx * r;
    py1 = starty * rm + stopy * r;
    px2 = startxp * rm + stopxp * r;
    py2 = startyp * rm + stopyp * r;

    col = it->second;
    pArray->SetColor(col);
    pArray->SetVertex(px2, py2);
    pArray->PushVertex();
    pArray->SetVertex(px1, py1);
    pArray->PushVertex();
  }

  pArray->SetVertex(xx, yy);
  pArray->PushVertex();
  pArray->SetVertex(xxp, yyp);
  pArray->PushVertex();

  DrawArray(pArray);

  PopClipping();
}

static void nuiDrawRect(const nuiRect& out, nuiRenderArray& rArray)
{
  rArray.SetMode(GL_TRIANGLE_STRIP);
  rArray.Reserve(8);
  nuiRect in(out);
  in.Grow(-NUI_INV_SCALE_FACTOR, -NUI_INV_SCALE_FACTOR);
  
  rArray.SetVertex(out.Left(), out.Top()); rArray.PushVertex();
  rArray.SetVertex(in.Left(), in.Top()); rArray.PushVertex();

  rArray.SetVertex(out.Right(), out.Top()); rArray.PushVertex();
  rArray.SetVertex(in.Right(), in.Top()); rArray.PushVertex();
  
  rArray.SetVertex(out.Right(), out.Bottom()); rArray.PushVertex();
  rArray.SetVertex(in.Right(), in.Bottom()); rArray.PushVertex();
  
  rArray.SetVertex(out.Left(), out.Bottom()); rArray.PushVertex();
  rArray.SetVertex(in.Left(), in.Bottom()); rArray.PushVertex();

  rArray.SetVertex(out.Left(), out.Top()); rArray.PushVertex();
  rArray.SetVertex(in.Left(), in.Top()); rArray.PushVertex();
}

void nuiDrawContext::DrawRect(const nuiRect& rRect, nuiShapeMode Mode)
{

  if (Mode == eStrokeAndFillShape || Mode == eStrokeShape)
  {
    nuiRect rect(rRect);

    GLenum mode = GL_LINE_LOOP;

    if (rect.mRight - rect.mLeft <= 1.0f)
    {
      mode = GL_TRIANGLE_STRIP;
    }
    else
    {
      rect.mRight -= 1.0f;
    }
    
    if (rect.mBottom - rect.mTop <= 1.0f)
    {
      mode = GL_TRIANGLE_STRIP;
    }
    else
    {
      rect.mBottom -= 1.0f;
    }

    // Draw the stroke in all cases:
    if (mode == GL_TRIANGLE_STRIP)
    {
      nuiRenderArray* pStrokeArray = new nuiRenderArray(mode);
      pStrokeArray->EnableArray(nuiRenderArray::eVertex, true);
      pStrokeArray->EnableArray(nuiRenderArray::eColor, true);
      pStrokeArray->Reserve(4);
      
      pStrokeArray->SetColor(mCurrentState.mStrokeColor);
      pStrokeArray->SetVertex(rect.mLeft, rect.mTop);
      pStrokeArray->PushVertex();
      pStrokeArray->SetVertex(rect.mRight, rect.mTop);
      pStrokeArray->PushVertex();
      pStrokeArray->SetVertex(rect.mLeft, rect.mBottom);
      pStrokeArray->PushVertex();

      pStrokeArray->SetVertex(rect.mRight, rect.mBottom);
      pStrokeArray->PushVertex();

      DrawArray(pStrokeArray);
    }
    else
    {
      nuiRenderArray* pStrokeArray = new nuiRenderArray(mode);
      if (NUI_SCALE_FACTOR != 1.0f)
      {
        nuiDrawRect(rRect, *pStrokeArray);
      }
      else
      {
        pStrokeArray->EnableArray(nuiRenderArray::eVertex, true);
        pStrokeArray->EnableArray(nuiRenderArray::eColor, true);
        pStrokeArray->Reserve(4);
        
        pStrokeArray->SetColor(mCurrentState.mStrokeColor);
        pStrokeArray->SetVertex(rect.mLeft, rect.mTop);
        pStrokeArray->PushVertex();
        
        pStrokeArray->SetVertex(rect.mRight, rect.mTop);
        pStrokeArray->PushVertex();
        
        pStrokeArray->SetVertex(rect.mRight, rect.mBottom);
        pStrokeArray->PushVertex();
        
        pStrokeArray->SetVertex(rect.mLeft, rect.mBottom);
        pStrokeArray->PushVertex();
      }

      DrawArray(pStrokeArray);
    }
  }

  if (Mode == eStrokeAndFillShape)
  {
    if ((rRect.mRight - rRect.mLeft <= 2.0f) || (rRect.mBottom - rRect.mTop <= 2.0f))
      return;

    nuiRect rect(rRect);
    // Draw the filled part:
    nuiRenderArray* pFillArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
    pFillArray->EnableArray(nuiRenderArray::eVertex, true);
    pFillArray->EnableArray(nuiRenderArray::eColor, true);
    pFillArray->Reserve(4);
    
    pFillArray->SetColor(mCurrentState.mFillColor);
    pFillArray->SetVertex(rect.mLeft+1, rect.mTop+1);
    pFillArray->PushVertex();

    pFillArray->SetVertex(rect.mRight-1, rect.mTop+1);
    pFillArray->PushVertex();

    pFillArray->SetVertex(rect.mLeft+1, rect.mBottom-1);
    pFillArray->PushVertex();
    
    pFillArray->SetVertex(rect.mRight-1, rect.mBottom-1);
    pFillArray->PushVertex();
    
    DrawArray(pFillArray);
  }
  else if (Mode == eFillShape)
  {
    nuiRect rect(rRect);
    //rect.Move(0,-.5); // Adjust to have a correct position on ATI cards, this should work on nvidia too
    // Draw the filled rectangle:
    nuiRenderArray* pFillArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
    pFillArray->EnableArray(nuiRenderArray::eVertex, true);
    pFillArray->EnableArray(nuiRenderArray::eColor, true);
    pFillArray->Reserve(4);

    pFillArray->SetColor(mCurrentState.mFillColor);
    pFillArray->SetVertex(rect.mLeft, rect.mTop);
    pFillArray->PushVertex();

    pFillArray->SetVertex(rect.mRight, rect.mTop);
    pFillArray->PushVertex();

    pFillArray->SetVertex(rect.mLeft, rect.mBottom);
    pFillArray->PushVertex();
    
    pFillArray->SetVertex(rect.mRight, rect.mBottom);
    pFillArray->PushVertex();

    DrawArray(pFillArray);
  }
}

void nuiDrawContext::DrawLine(float x1, float y1, float x2, float y2)
{
  if (x1 == x2 && y1 == y2)
  {
    DrawPoint(x1, y1);
  }
  else
  {
    nuiShape shp;
    shp.LineTo(nuiPoint(x1, y1));
    shp.LineTo(nuiPoint(x2, y2));
    DrawShape(&shp, eStrokeShape);
  }
}

void nuiDrawContext::DrawPoint(float x, float y)
{
  nuiShape shp;
  shp.AddCircle(x, y, mCurrentState.mLineWidth);
  nuiColor col(mCurrentState.mFillColor);
  SetFillColor(mCurrentState.mStrokeColor);
  DrawShape(&shp, eFillShape);
  SetFillColor(col);
}

void nuiDrawContext::DrawPoint(const nuiVector2& rPoint)
{
  DrawPoint(rPoint[0], rPoint[1]);
}

void nuiDrawContext::DrawArray(nuiRenderArray* pArray)
{
  uint32 size = pArray->GetSize();
  if (!size)
  {
    pArray->Release();
    return;
  }

#ifdef _DEBUG_
  {
    // Error checking:
    if (!pArray->GetIndexArrayCount())
    {
      switch (pArray->GetMode())
      {
        case GL_POINTS:
          NGL_ASSERT(size);
          break;
        case GL_LINES:
          NGL_ASSERT(!(size & 1));
          break;
        case GL_LINE_LOOP:
          NGL_ASSERT(size > 1);
          break;
        case GL_LINE_STRIP:
          NGL_ASSERT(size > 1);
          break;
        case GL_TRIANGLES:
          NGL_ASSERT(size > 2 && !(size % 3));
          break;
        case GL_TRIANGLE_STRIP:
          NGL_ASSERT(size > 2);
          break;
        case GL_TRIANGLE_FAN:
          NGL_ASSERT(size > 2);
          break;
#ifndef _OPENGL_ES_
        case GL_QUADS:
          NGL_ASSERT(size > 3 && !(size % 4));
          break;
        case GL_QUAD_STRIP:
          NGL_ASSERT(size > 3);
          break;
        case GL_POLYGON:
          NGL_ASSERT(size > 3);
          break;
#endif
      }
    }
  }
#endif

  if (mStateChanges)
    mpPainter->SetState(mCurrentState);
  mStateChanges = 0;

  mpPainter->DrawArray(pArray);
}

void nuiDrawContext::DrawObject(const nuiRenderObject& rObject)
{
  uint32 count = rObject.GetSize();
  for (uint32 i = 0; i < count; i++)
  {
    DrawArray(rObject.GetArray(i));
  }
}

void nuiDrawContext::Translate(const nuiVector& Vector)
{
  Translate(Vector[0],Vector[1]);
}

void nuiDrawContext::PushMatrix()
{
  mpPainter->PushMatrix();
}

void nuiDrawContext::PopMatrix()
{
  mpPainter->PopMatrix();
}

void nuiDrawContext::LoadMatrix(const nuiMatrix& Matrix)
{
  mpPainter->LoadMatrix(Matrix);
}

void nuiDrawContext::MultMatrix(const nuiMatrix& Matrix)
{
  if (Matrix.IsIdentity())
    return;
  mpPainter->MultMatrix(Matrix);
}

void nuiDrawContext::Translate(nuiSize X, nuiSize Y, nuiSize Z)
{
  if (X == 0 && Y == 0 && Z == 0)
    return;
  
  nuiMatrix m;
  m.SetTranslation(X, Y, Z);
  MultMatrix(m);
}

void nuiDrawContext::Scale(nuiSize X, nuiSize Y, nuiSize Z)
{
  if (X == 1.0 && Y == 1.0 && Z == 1.0)
    return;
  
  nuiMatrix m;
  m.SetScaling(X, Y, Z);
  MultMatrix(m);
}

void nuiDrawContext::LoadIdentity()
{
  nuiMatrix m;
  LoadMatrix(m);
}

void nuiDrawContext::GetMatrix(nuiMatrix& rMatrix) const
{
  rMatrix = mpPainter->GetMatrix();
}

const nuiMatrix& nuiDrawContext::GetMatrix() const
{
  return mpPainter->GetMatrix();
}

//////// Projection matrix:

void nuiDrawContext::PushProjectionMatrix()
{
  mpPainter->PushProjectionMatrix();
}

void nuiDrawContext::PopProjectionMatrix()
{
  mpPainter->PopProjectionMatrix();
}

void nuiDrawContext::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& Matrix)
{
  mpPainter->LoadProjectionMatrix(rViewport, Matrix);
}

void nuiDrawContext::MultProjectionMatrix(const nuiMatrix& Matrix)
{
  if (Matrix.IsIdentity())
    return;
  mpPainter->MultProjectionMatrix(Matrix);
}

void nuiDrawContext::LoadProjectionIdentity()
{
  nuiMatrix m;
  LoadProjectionMatrix(nuiRect(0.0f, 0.0f, mWidth, mHeight), m);
}

void nuiDrawContext::GetProjectionMatrix(nuiMatrix& rMatrix) const
{
  rMatrix = mpPainter->GetProjectionMatrix();
}

const nuiMatrix& nuiDrawContext::GetProjectionMatrix() const
{
  return mpPainter->GetProjectionMatrix();
}

void nuiDrawContext::Set2DProjectionMatrix(const nuiRect& rRect)
{
  //printf("Set2DProjectionMatrix: %ls\n", rRect.GetValue().GetChars());
  nuiMatrix m;
  m.Translate(-1.0f, 1.0f, 0.0f);
  m.Scale(2.0f/rRect.GetWidth(), -2.0f/rRect.GetHeight(), 1.0f);
  LoadProjectionMatrix(rRect, m);
}

void nuiDrawContext::SetPerspectiveProjectionMatrix(const nuiRect& rRect, float FovY, float Aspect, float Near, float Far)
{
  nuiMatrix m;
  m.SetPerspective(FovY, Aspect, Near, Far);
  LoadProjectionMatrix(rRect, m);
}

void nuiDrawContext::SetOrthoProjectionMatrix(const nuiRect& rRect, float Left, float Right, float Bottom, float Top, float Near, float Far)
{
  nuiMatrix m;
  m.SetOrtho(Left, Right, Bottom, Top, Near, Far);
  m.Translate(-1.0f, 1.0f, 0.0f);
  LoadProjectionMatrix(rRect, m);
}

void nuiDrawContext::SetFrustumProjectionMatrix(const nuiRect& rRect, float Left, float Right, float Bottom, float Top, float Near, float Far)
{
  nuiMatrix m;
  m.SetFrustum(Left, Right, Bottom, Top, Near, Far);
  LoadProjectionMatrix(rRect, m);
}

///////
void nuiDrawContext::EnableColorBuffer(bool set)
{
  if (mCurrentState.mColorBuffer != set)
  {
    mCurrentState.mColorBuffer = set;
    mStateChanges++;
  }
}

uint32 nuiDrawContext::GetClipStackSize() const
{
  return mpPainter->GetClipStackSize();
}

#define SHADE_ALPHA 0.3f

void nuiDrawContext::DrawShade(const nuiRect& rSourceRect, const nuiRect& rShadeRect, const nuiColor& rTint)
{
  bool texturing = mCurrentState.mTexturing;
  bool blending = mCurrentState.mBlending;
  nuiBlendFunc blendfunc;
  blendfunc = mCurrentState.mBlendFunc;

  if (!blending)
    EnableBlending(true);
  if (blendfunc != nuiBlendTransp)
    SetBlendFunc(nuiBlendTransp);

  nuiSize ShadeSize = rSourceRect.mLeft - rShadeRect.mLeft;

  nuiTexture* pShade = ::nuiTexture::GetTexture(nglString(_T("NUI_Shade_LUT")));

  if (!pShade)
  {
    // Left shadow
    const uint32 size = 16;
    uint8 pLUT[size * 4];
    uint i;
    for (i = 0; i<size; i++)
    {
      pLUT[0+(i*4)] = 0;
      pLUT[1+(i*4)] = 0;
      pLUT[2+(i*4)] = 0;
      float p = (float)i * (255.0f / (float)size);
      pLUT[3+(i*4)] = ToBelow(p);
    }

    nglImageInfo info(false);
    info.mBitDepth = 32;
    info.mBufferFormat = eImageFormatRaw;
    info.mBytesPerLine = size * 4;
    info.mBytesPerPixel = 4;
    info.mHeight = 1;
    info.mWidth = size;
    info.mpBuffer = (char*)pLUT;
    info.mPixelFormat = eImagePixelRGBA;

    pShade = nuiTexture::GetTexture(info, true);
    pShade->SetSource(_T("NUI_Shade_LUT"));

    NGL_ASSERT(pShade);

    pShade->SetMinFilter(GL_LINEAR);
    pShade->SetMagFilter(GL_LINEAR);
#ifndef _OPENGL_ES_
    pShade->SetWrapS(GL_CLAMP);
    pShade->SetWrapT(GL_CLAMP);
#else
    pShade->SetWrapS(GL_CLAMP_TO_EDGE);
    pShade->SetWrapT(GL_CLAMP_TO_EDGE);  
#endif
    pShade->SetEnvMode(GL_MODULATE);
  }

  nuiColor transp(rTint);
  nuiColor opaque(rTint);
  transp.Multiply(0.0f);
  opaque.Multiply(SHADE_ALPHA);

  if (!texturing)
    EnableTexturing(true);
  
  SetTexture(pShade);

  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLES);
  pArray->EnableArray(nuiRenderArray::eVertex);
  pArray->EnableArray(nuiRenderArray::eColor);
  pArray->EnableArray(nuiRenderArray::eTexCoord);
  pArray->Reserve(42);

  // Top Left:
  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  ///
  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetTexCoords(0, 0);
  pArray->SetColor(opaque);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  // Left
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom);
  pArray->PushVertex();

  ///
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  // Left Corner:
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  ///
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft-ShadeSize,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  // bottom shadow
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  ///
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mLeft,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  // Right Corner
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  ///
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom+ShadeSize);
  pArray->PushVertex();

  // Right
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();
  
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();
  
  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  ///
  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mBottom);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mBottom);
  pArray->PushVertex();

  // Top Right
  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(transp);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  ///
  pArray->SetColor(transp);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mTop);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(0, 0);
  pArray->SetVertex(rSourceRect.mRight+ShadeSize,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();

  pArray->SetColor(opaque);
  pArray->SetTexCoords(1, 0);
  pArray->SetVertex(rSourceRect.mRight,rSourceRect.mTop+ShadeSize);
  pArray->PushVertex();


  DrawArray(pArray);

  if (!texturing)
    EnableTexturing(false);

  if (!blending)
    EnableBlending(blending);
  
  if (blendfunc != nuiBlendTransp)
    SetBlendFunc(blendfunc);
}


nuiDrawContext *nuiDrawContext::CreateDrawContext(const nuiRect& rRect, nuiRenderer Renderer, nglContext* pContext)
{
  nuiDrawContext* pC = new nuiDrawContext(rRect);
  nuiPainter* pPainter = NULL;
  switch (Renderer)
  {
#ifndef __NUI_NO_GL__
  case eOpenGL:
    pPainter = new nuiGLPainter(pContext, rRect);
    break;
#endif
#ifndef __NUI_NO_D3D__
  case eDirect3D:
    pPainter = new nuiD3DPainter(pContext, rRect);
    break;
#endif
#ifndef __NUI_NO_SOFTWARE__
  case eSoftware:
    pPainter = new nuiSoftwarePainter(rRect, pContext);
    break;
#endif
  case eMeta:
    pPainter = new nuiMetaPainter(rRect, pContext);
    break;
  }
  pC->SetPainter(pPainter);
  if (pContext)
    pContext->BeginSession();
  pC->Set2DProjectionMatrix(rRect);
  if (pContext)
    pContext->EndSession();
  return pC;
}


void nuiDrawContext::SetSize(uint w, uint h)
{
  mpPainter->SetSize(w, h);
}

int nuiDrawContext::GetWidth() const
{
  return (int)mWidth;
}

int nuiDrawContext::GetHeight() const
{
  return (int)mHeight;
}
