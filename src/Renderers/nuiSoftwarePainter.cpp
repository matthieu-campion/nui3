/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#include "nui.h"

#ifndef _CARBON_
#define NUI_USE_BGRA
#define AGG_BGRA32
#else
#define NUI_USE_BGRA
#define AGG_BGRA32
#endif

#ifdef _NGL_X86_
//#define NUI_USE_MMX
//#define NUI_USE_SSE
#endif

#include "nuiFixedPoint.h"
#include "nuiVertex.h"
#include "nuiPixelBlender.h"
#include "nuiRasterizer.h"

/*
Shark result: 
 27.0%	27.0%	DrawHLine<nuiPixelBlender_Transp, nuiVertex<nuiSolidColor> >(nuiVertex<nuiSolidColor>&, nuiVertex<nuiSolidColor>&)	
 18.4%	18.4%	DrawHLine<nuiPixelBlender_Transp, nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGB24>, nuiGouraudColor> > >(nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGB24>, nuiGouraudColor> >&, nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGB24>, nuiGouraudColor> >&)	
 13.3%	13.3%	DrawHLine<nuiPixelBlender_Transp, nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGBA32>, nuiGouraudColor> > >(nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGBA32>, nuiGouraudColor> >&, nuiVertex<nuiModulatedColor<nuiTexelColor<nuiTexelAccessor_RGBA32>, nuiGouraudColor> >&)	
 4.8%	4.8%	DrawHLine<nuiPixelBlender_Transp, nuiVertex<nuiGouraudColor> >(nuiVertex<nuiGouraudColor>&, nuiVertex<nuiGouraudColor>&)	
 4.1%	4.1%	DrawLine<nuiPixelBlender_Transp, nuiVertex<nuiGouraudColor> >(nuiVertex<nuiGouraudColor> const&, nuiVertex<nuiGouraudColor> const&)	
 
 
 */


//template <>
//class nuiVertex< nuiModulatedColor< nuiTexelColor<nuiTexelAccessor_RGB24>, nuiGouraudColor> >
//{
//public:
//  void DrawHLine<class PixelBlender>(uint32* pBuffer, nuiVertex< nuiModulatedColor< nuiTexelColor<nuiTexelAccessor_RGB24>, nuiGouraudColor> >& v0, int32 width)
//  {
//    NGL_ASSERT(width > 0);
//    
//    if (PixelBlender::CanOptimize())
//    {
//      if (IsStable())
//      {
//        uint32 color = v0.GetColor();
//        for (uint i = 0; i < width; i++)
//          pBuffer[i] = color;
//      }
//      else
//      {
//        while (width > 0)
//        {
//          *pBuffer = v0.GetColor();
//          pBuffer++;
//          v0.AddValue(*this);
//          width--;
//        }
//      }
//    }
//    else
//    {
//      if (IsStable())
//      {
//        uint32 color = v0.GetColor();
//        while (width > 0)
//        {
//          PixelBlender::Blend(*pBuffer, color);
//          pBuffer++;
//          width--;
//        }
//      }
//      else
//      {
//        while (width > 0)
//        {
//          PixelBlender::Blend(*pBuffer, v0.GetColor());
//          pBuffer++;
//          v0.AddValue(*this);
//          width--;
//        }
//      }
//    }
//  }
//  
//};
//


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


nuiSoftwarePainter::nuiSoftwarePainter(nglContext* pContext)
: nuiPainter(pContext)
{
  mWidth = 0;
  mHeight = 0;
  mpRasterizer = new nuiRasterizer(mWidth, mHeight);
  AddNeedTextureBackingStore();
}

nuiSoftwarePainter::~nuiSoftwarePainter()
{
  DelNeedTextureBackingStore();
}


void nuiSoftwarePainter::SetSize(uint sizex, uint sizey)
{
  mWidth = sizex;
  mHeight = sizey;
  mpRasterizer->Resize(mWidth, mHeight);
}

void nuiSoftwarePainter::StartRendering()
{
  nuiPainter::StartRendering();
}

void nuiSoftwarePainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  mState = rState;

  int32 x = MIN(ToNearest(mClip.Left()), (int32)mWidth);
  int32 y = MIN(ToNearest(mClip.Top()), (int32)mHeight);
  int32 xt = MIN(ToNearest(mClip.Right()), (int32)mWidth);
  int32 yt = MIN(ToNearest(mClip.Bottom()), (int32)mHeight);
  
  x = MAX(0, x);
  y = MAX(0, y);
  xt = MAX(0, xt);
  yt = MAX(0, yt);
  
  if (mClip.mEnabled)
    mpRasterizer->SetClipRect(x, y, xt, yt);
  else
    mpRasterizer->SetClipRect(0,0, mWidth, mHeight);  
}

void nuiSoftwarePainter::DrawArray(nuiRenderArray* pArray)
{
  if (!mEnableDrawArray)
  {
    pArray->Release();
    return;
  }
  
  switch (pArray->GetMode())
  {
  case GL_POINTS:
    //NGL_OUT(_T("GL_POINTS Not Implemented\n"));
    break;
  case GL_LINES:
    DrawLines(pArray);
    break;
  case GL_LINE_STRIP:
    DrawLineStrip(pArray);
    break;
  case GL_LINE_LOOP:
    DrawLineLoop(pArray);
    break;
  case GL_TRIANGLES:
    DrawTriangles(pArray);
    break;
  case GL_TRIANGLE_FAN:
    DrawTrianglesFan(pArray);
    break;
  case GL_TRIANGLE_STRIP:
    DrawTrianglesStrip(pArray);
    break;
//  case GL_QUADS:
//    DrawQuads(pArray);
//    break;
//  case GL_QUAD_STRIP:
//    DrawQuadStrip(pArray);
//    break;
//  case GL_POLYGON:
//    //NGL_OUT(_T("GL_POLYGON Not Implemented\n"));
//    break;
  }

  pArray->Release();
}

void nuiSoftwarePainter::ClearColor()
{
  uint32 col = NUI_RGBA_F(mState.mClearColor.Red(), mState.mClearColor.Green(), mState.mClearColor.Blue(), mState.mClearColor.Alpha());
  mpRasterizer->ClearColor(col);

}

void nuiSoftwarePainter::BeginSession()
{
}

void nuiSoftwarePainter::EndSession()
{
}

void nuiSoftwarePainter::ReleaseCacheObject(void* pHandle)
{

}



void nuiSoftwarePainter::DrawLines(const nuiRenderArray* pArray)
{
  int32 count = pArray->GetSize() / 2;
  for (int32 i = 0; i < count; i++)
  {
    int32 ii = i << 1;
    DrawLine(pArray, ii, ii+1);
  }
}

void nuiSoftwarePainter::DrawLineStrip(const nuiRenderArray* pArray)
{
  int32 count = pArray->GetSize() - 1;
  for (int32 i = 0; i < count; i++)
  {
    DrawLine(pArray, i, i + 1);
  }
}

void nuiSoftwarePainter::DrawLineLoop(const nuiRenderArray* pArray)
{
  int32 s = pArray->GetSize();
  int32 count = s / 2;

  for (int32 i = 0; i < count; i++)
  {
    int32 ii = i << 1;
    DrawLine(pArray, ii, (ii + 1) % s);
  }
}

// DrawTriangles (GL_TRIANGLES)
void nuiSoftwarePainter::DrawTriangles(const nuiRenderArray* pArray)
{
  int32 i;
  int32 count = pArray->GetSize() / 3;
  for (i = 0; i < count; i++)
  {
    uint32 ii = i *3;
    DrawTriangle(pArray, ii, ii+1, ii+2);
  }
}

// DrawTrianglesFan (GL_TRIANGLE_FAN)
void nuiSoftwarePainter::DrawTrianglesFan(const nuiRenderArray* pArray)
{
  int32 i;
  int32 count = pArray->GetSize() - 1;
  for (i = 1; i < count; i++)
  {
    DrawTriangle(pArray, 0, i, i + 1);
  }
}

// DrawTrianglesStrip (GL_TRIANGLE_STRIP)
void nuiSoftwarePainter::DrawTrianglesStrip(const nuiRenderArray* pArray)
{
  int32 i;
  int32 count = pArray->GetSize() - 2;
  for (i = 0; i < count; i++)
  {
    if (i & 1)
      DrawTriangle(pArray, i, i + 1, i + 2);
    else
      DrawTriangle(pArray, i + 1, i, i + 2);
  }
}

// DrawQuads (GL_QUADS)
void nuiSoftwarePainter::DrawQuads(const nuiRenderArray* pArray)
{
  int32 i;
  int32 count = pArray->GetSize() / 4;
  for (i = 0; i < count; i++)
  {
    int32 ii = i << 2;

    // Is the quad a special case?
    const std::vector<nuiRenderArray::Vertex>& rCoords = pArray->GetVertices();

    float x0 = rCoords[ii].mX, y0 = rCoords[ii].mY;
    float x1 = rCoords[(ii+1)].mX, y1 = rCoords[(ii+1)].mY;
    float x2 = rCoords[(ii+2)].mX, y2 = rCoords[(ii+2)].mY;
    float x3 = rCoords[(ii+3)].mX, y3 = rCoords[(ii+3)].mY;

    if (x0 == x3 && x1 == x2 && y0 == y1 && y2 == y3)
    {
      // This is an axis aligned rectangle
      DrawRectangle(pArray, ii, ii+1, ii+2, ii+3);
    }
    else
    {
      // This is not a special quad, draw two triangles:
      DrawTriangle(pArray, ii, ii+1, ii+2);
      DrawTriangle(pArray, ii, ii+2, ii+3);
    }
  }
}

// DrawQuads (GL_QUADS)
void nuiSoftwarePainter::DrawQuadStrip(const nuiRenderArray* pArray)
{
  int32 i;
  int32 count = (pArray->GetSize() - 2) / 2;
  for (i = 0; i < count; i++)
  {
    int32 ii = i << 1;

    // Is the quad a special case?
    const std::vector<nuiRenderArray::Vertex>& rCoords = pArray->GetVertices();

    float x0 = rCoords[ii].mX, y0 = rCoords[ii+1].mY;
    float x1 = rCoords[(ii+1)].mX, y1 = rCoords[(ii+1)].mY;
    float x2 = rCoords[(ii+2)].mX, y2 = rCoords[(ii+2)].mY;
    float x3 = rCoords[(ii+3)].mX, y3 = rCoords[(ii+3)].mY;
    if (x0 == x3 && x1 == x2 && y0 == y1 && y2 == y3)
    {
      // This is an axis aligned rectangle
      DrawRectangle(pArray, ii, ii+1, ii+3, ii+2);
    }
    else
    {
      // This is not a special quad, draw two triangles:
      DrawTriangle(pArray, ii, ii+1, ii+2);
      DrawTriangle(pArray, ii+1, ii+3, ii+2);
    }
  }
}

void nuiSoftwarePainter::DrawLine(const nuiRenderArray* pArray, int p1, int p2)
{
  // Prepare the line points:

  // Coordinates:
  const std::vector<nuiRenderArray::Vertex>& rVertices(pArray->GetVertices());

  const float xbias = 0;
  const float ybias = 0;

  float x1 = rVertices[p1].mX, y1 = rVertices[p1].mY;
  float x2 = rVertices[p2].mX, y2 = rVertices[p2].mY;

  nuiVector vec1(x1,y1, 0.0f);
  nuiVector vec2(x2,y2, 0.0f);

  vec1 = mMatrixStack.top() * vec1;
  vec2 = mMatrixStack.top() * vec2;

  x1 = vec1[0] + xbias; y1 = vec1[1] + ybias;
  x2 = vec2[0] + xbias; y2 = vec2[1] + ybias;

  // Vertices Colors:
  nuiColor c1, c2;

  if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    c1 = nuiColor(rVertices[p1].mR, rVertices[p1].mG, rVertices[p1].mB, rVertices[p1].mA);
    c2 = nuiColor(rVertices[p2].mR, rVertices[p2].mG, rVertices[p2].mB, rVertices[p2].mA);
  }
  else
  {
    c1 = c2 = mState.mFillColor;
  }

  // Texture coords:
  float u1 = 0, v1 = 0;
  float u2 = 0, v2 = 0;

  if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    u1 = rVertices[p1].mTX;
    v1 = rVertices[p1].mTY;
    u2 = rVertices[p2].mTX;
    v2 = rVertices[p2].mTY;
  }
  
  
  if (mState.mpTexture && mState.mTexturing)
  {
    nuiTexture* pTexture = mState.mpTexture;
    int32 width = pTexture->GetImage()->GetWidth();
    int32 height = pTexture->GetImage()->GetHeight();
    
    u1 *= width;
    v1 *= height;
    u2 *= width;
    v2 *= height;
    
    
    //    if (c1 == c2 && c1 == c3)
    { // One Color:
      
#define VERTEX(Y, NUM) nuiVertex<nuiModulatedColor<nuiTexelColor<Y>, nuiGouraudColor> > \
(x##NUM, y##NUM, \
nuiModulatedColor<nuiTexelColor<Y>, nuiGouraudColor>(nuiTexelColor<Y>(pTexture, u##NUM, v##NUM), nuiGouraudColor(c##NUM)))
      
#define RASTERIZE(X, Y) \
mpRasterizer->DrawLine<X>(VERTEX(Y, 1), VERTEX(Y, 2));
      
#define RASTERIZERS(X) \
case eImagePixelRGB:\
RASTERIZE(X, nuiTexelAccessor_RGB24);\
break;\
case eImagePixelRGBA:\
RASTERIZE(X, nuiTexelAccessor_RGBA32);\
break;\
case eImagePixelLum:\
RASTERIZE(X, nuiTexelAccessor_Lum);\
break;\
case eImagePixelAlpha:\
RASTERIZE(X, nuiTexelAccessor_Alpha);\
break;\
case eImagePixelLumA:\
RASTERIZE(X, nuiTexelAccessor_LumA);\
break; \
case eImagePixelNone: break; \
case eImagePixelIndex: break; 

      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Transp);
            default:
              break;
          }
          break;
        case nuiBlendTranspAdd:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_TranspAdd);
            default:
              break;
          }
          break;
        case nuiBlendSource:
        default:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Copy);
            default:
              break;
          }
          break;
      }
    }
    //    else
    //    {
    //      switch (mState.mBlendFunc)
    //      {
    //        case nuiBlendTransp:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                             nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                             nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //        case nuiBlendTranspAdd:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                                nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                                nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //        case nuiBlendSource:
    //        default:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                           nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                           nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //      }
    //    }
#undef RASTERIZE
#undef RASTERIZERS
  }
  else
  {
    if (c1 == c2)
    { // One Color:
      const uint32 col = NUI_RGBA_F(c1.Red(), c1.Green(), c1.Blue(), c1.Alpha());
      
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          mpRasterizer->DrawLine<nuiPixelBlender_Transp>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col));
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawLine<nuiPixelBlender_TranspAdd>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col));
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawLine<nuiPixelBlender_Copy>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col));
          break;
      }
    }
    else
    {
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          mpRasterizer->DrawLine<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                             nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2))
                                                             );
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawLine<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                                nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2))
                                                                );
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawLine<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                           nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2))
                                                           );
          break;
      }
    }
  }
}


void nuiSoftwarePainter::DrawTriangle(const nuiRenderArray* pArray, int p1, int p2, int p3)
{
  // Prepare the triangle points:

  // Coordinates:
  const std::vector<nuiRenderArray::Vertex>& rVertices(pArray->GetVertices());

  float x1 = rVertices[p1].mX, y1 = rVertices[p1].mY;
  float x2 = rVertices[p2].mX, y2 = rVertices[p2].mY;
  float x3 = rVertices[p3].mX, y3 = rVertices[p3].mY;

  nuiVector vec1(x1,y1, 0.0f);
  nuiVector vec2(x2,y2, 0.0f);
  nuiVector vec3(x3,y3, 0.0f);

  vec1 = mMatrixStack.top() * vec1;
  vec2 = mMatrixStack.top() * vec2;
  vec3 = mMatrixStack.top() * vec3;

  x1 = vec1[0]; y1 = vec1[1];
  x2 = vec2[0]; y2 = vec2[1];
  x3 = vec3[0]; y3 = vec3[1];

  // Vertice Colors:
  nuiColor c1, c2, c3;

  if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    c1 = nuiColor(rVertices[p1].mR, rVertices[p1].mG, rVertices[p1].mB, rVertices[p1].mA);
    c2 = nuiColor(rVertices[p2].mR, rVertices[p2].mG, rVertices[p2].mB, rVertices[p2].mA);
    c3 = nuiColor(rVertices[p3].mR, rVertices[p3].mG, rVertices[p3].mB, rVertices[p3].mA);
  }
  else
  {
    switch (pArray->GetMode())
    {
      case GL_POINTS:
      case GL_LINES:
      case GL_LINE_LOOP:
      case GL_LINE_STRIP:
        c1 = c2 = c3 = mState.mStrokeColor;
        break;
        
      case GL_TRIANGLES:
      case GL_TRIANGLE_STRIP:
      case GL_TRIANGLE_FAN:
//      case GL_QUADS:
//      case GL_QUAD_STRIP:
//      case GL_POLYGON:
        c1 = c2 = c3 = mState.mFillColor;
        break;
    }
  }

  // Texture coords:
  float u1 = 0, v1 = 0;
  float u2 = 0, v2 = 0;
  float u3 = 0, v3 = 0;

  if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    u1 = rVertices[p1].mTX;
    v1 = rVertices[p1].mTY;
    u2 = rVertices[p2].mTX;
    v2 = rVertices[p2].mTY;
    u3 = rVertices[p3].mTX;
    v3 = rVertices[p3].mTY;
  }

  if (mState.mpTexture && mState.mTexturing)
  {
    nuiTexture* pTexture = mState.mpTexture;
    int32 width = pTexture->GetImage()->GetWidth();
    int32 height = pTexture->GetImage()->GetHeight();

    u1 *= width;
    v1 *= height;
    u2 *= width;
    v2 *= height;
    u3 *= width;
    v3 *= height;
    
    
    //    if (c1 == c2 && c1 == c3)
    { // One Color:
#define VERTEX(Y, NUM) nuiVertex<nuiModulatedColor<nuiTexelColor<Y>, nuiGouraudColor> > \
      (x##NUM, y##NUM, \
      nuiModulatedColor<nuiTexelColor<Y>, nuiGouraudColor>(nuiTexelColor<Y>(pTexture, u##NUM, v##NUM), nuiGouraudColor(c##NUM)))

#define RASTERIZE(X, Y) \
  mpRasterizer->DrawTriangle<X>( \
    VERTEX(Y, 1), \
    VERTEX(Y, 2), \
    VERTEX(Y, 3) \
    );
      
#define RASTERIZERS(X) \
    case eImagePixelRGB:\
      RASTERIZE(X, nuiTexelAccessor_RGB24);\
      break;\
    case eImagePixelRGBA:\
      RASTERIZE(X, nuiTexelAccessor_RGBA32);\
      break;\
    case eImagePixelLum:\
      RASTERIZE(X, nuiTexelAccessor_Lum);\
      break;\
    case eImagePixelAlpha:\
      RASTERIZE(X, nuiTexelAccessor_Alpha);\
      break;\
    case eImagePixelLumA:\
      RASTERIZE(X, nuiTexelAccessor_LumA);\
      break; \
	case eImagePixelNone: break; \
	case eImagePixelIndex: break;
      
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Transp);
            default:
              break;
          }
          break;
        case nuiBlendTranspAdd:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_TranspAdd);
            default:
              break;
          }
          break;
        case nuiBlendSource:
        default:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Copy);
            default:
              break;
          }
          break;
      }
    }
//    else
//    {
//      switch (mState.mBlendFunc)
//      {
//        case nuiBlendTransp:
//          mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
//                                                             nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
//                                                             nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
//          break;
//        case nuiBlendTranspAdd:
//          mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
//                                                                nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
//                                                                nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
//          break;
//        case nuiBlendSource:
//        default:
//          mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
//                                                           nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
//                                                           nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
//          break;
//      }
//    }
#undef RASTERIZE
#undef RASTERIZERS
  }
  else
  {
    if (c1 == c2 && c1 == c3)
    { // One Color:
      const uint32 col = NUI_RGBA_F(c1.Red(), c1.Green(), c1.Blue(), c1.Alpha());
      
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          if (c1.Alpha() < 1.0f)
            mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col));
          else
            mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col));
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col));
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col));
          break;
      }
    }
    else
    {
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          if ((c1.Alpha() < 1.0f) && (c2.Alpha() < 1.0f) && (c3.Alpha() < 1.0f))
          {
            mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                               nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                               nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
          }
          else
          {
            mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                               nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                               nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
          }
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                                nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                                nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                           nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                           nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
          break;
      }
    }
  }
}

void nuiSoftwarePainter::DrawRectangle(const nuiRenderArray* pArray, int p1, int p2, int p3, int p4)
{
  // Coordinates:
  const std::vector<nuiRenderArray::Vertex>& rVertices(pArray->GetVertices());

  float bias = 0.0f;

//  if (!mState.mAntialiasing && !mState.mTexturing)
//    bias = 0.5f;

  float x1 = rVertices[p1].mX, y1 = rVertices[p1].mY;
  float x2 = rVertices[p2].mX, y2 = rVertices[p2].mY;
  float x3 = rVertices[p3].mX, y3 = rVertices[p3].mY;
  float x4 = rVertices[p4].mX, y4 = rVertices[p4].mY;

  nuiVector vec1(x1,y1, 0.0f);
  nuiVector vec2(x2,y2, 0.0f);
  nuiVector vec3(x3,y3, 0.0f);
  nuiVector vec4(x4,y4, 0.0f);

  vec1 = mMatrixStack.top() * vec1;
  vec2 = mMatrixStack.top() * vec2;
  vec3 = mMatrixStack.top() * vec3;
  vec4 = mMatrixStack.top() * vec4;

  x1 = vec1[0] + bias; y1 = vec1[1] + bias;
  x2 = vec2[0] + bias; y2 = vec2[1] + bias;
  x3 = vec3[0] + bias; y3 = vec3[1] + bias;
  x4 = vec4[0] + bias; y4 = vec4[1] + bias;

  // Vertice Colors:
  nuiColor c1, c2, c3, c4;

  if (pArray->IsArrayEnabled(nuiRenderArray::eColor))
  {
    c1 = nuiColor(rVertices[p1].mR, rVertices[p1].mG, rVertices[p1].mB, rVertices[p1].mA);
    c2 = nuiColor(rVertices[p2].mR, rVertices[p2].mG, rVertices[p2].mB, rVertices[p2].mA);
    c3 = nuiColor(rVertices[p3].mR, rVertices[p3].mG, rVertices[p3].mB, rVertices[p3].mA);
    c4 = nuiColor(rVertices[p4].mR, rVertices[p4].mG, rVertices[p4].mB, rVertices[p4].mA);
  }
  else
  {
    c1 = c2 = c3 = c4 = mState.mFillColor;
  }

  // Texture coords:
  float u1 = 0, v1 = 0;
  float u2 = 0, v2 = 0;
  float u3 = 0, v3 = 0;
  float u4 = 0, v4 = 0;

  if (pArray->IsArrayEnabled(nuiRenderArray::eTexCoord))
  {
    u1 = rVertices[p1].mTX;
    v1 = rVertices[p1].mTY;
    u2 = rVertices[p2].mTX;
    v2 = rVertices[p2].mTY;
    u3 = rVertices[p3].mTX;
    v3 = rVertices[p3].mTY;
    u4 = rVertices[p4].mTX;
    v4 = rVertices[p4].mTY;
  }

  
  if (mState.mpTexture && mState.mTexturing)
  {
    nuiTexture* pTexture = mState.mpTexture;
    int32 width = pTexture->GetImage()->GetWidth();
    int32 height = pTexture->GetImage()->GetHeight();
    
    u1 *= width;
    v1 *= height;
    u2 *= width;
    v2 *= height;
    u3 *= width;
    v3 *= height;
    u4 *= width;
    v4 *= height;
    
    
    //    if (c1 == c2 && c1 == c3)
    { // One Color:
#define RASTERIZE(X, Y) \
  mpRasterizer->DrawRectangle<X>( \
    VERTEX(Y, 1), \
    VERTEX(Y, 2), \
    VERTEX(Y, 3), \
    VERTEX(Y, 4) \
    );
      
#define RASTERIZERS(X) \
  case eImagePixelRGB:\
  RASTERIZE(X, nuiTexelAccessor_RGB24);\
  break;\
  case eImagePixelRGBA:\
  RASTERIZE(X, nuiTexelAccessor_RGBA32);\
  break;\
  case eImagePixelLum:\
  RASTERIZE(X, nuiTexelAccessor_Lum);\
  break;\
  case eImagePixelAlpha:\
  RASTERIZE(X, nuiTexelAccessor_Alpha);\
  break;\
  case eImagePixelLumA:\
  RASTERIZE(X, nuiTexelAccessor_LumA);\
  break; \
  case eImagePixelNone: break; \
  case eImagePixelIndex: break;

      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Transp);
            default:
              break;
          }
          break;
        case nuiBlendTranspAdd:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_TranspAdd);
            default:
              break;
          }
          break;
        case nuiBlendSource:
        default:
          switch (mState.mpTexture->GetImage()->GetPixelFormat())
          {
            RASTERIZERS(nuiPixelBlender_Copy);
            default:
              break;
          }
          break;
      }
    }
    //    else
    //    {
    //      switch (mState.mBlendFunc)
    //      {
    //        case nuiBlendTransp:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                             nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                             nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //        case nuiBlendTranspAdd:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                                nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                                nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //        case nuiBlendSource:
    //        default:
    //          mpRasterizer->DrawTriangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
    //                                                           nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
    //                                                           nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)));
    //          break;
    //      }
    //    }
#undef RASTERIZE
#undef RASTERIZERS
  }
  else
  {
    if (c1 == c2 && c1 == c3 && c1 == c4)
    { // One Color:
      const uint32 col = NUI_RGBA_F(c1.Red(), c1.Green(), c1.Blue(), c1.Alpha());
      
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          mpRasterizer->DrawRectangle<nuiPixelBlender_Transp>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col), nuiVertex_Solid(x4, y4, col));
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawRectangle<nuiPixelBlender_TranspAdd>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col), nuiVertex_Solid(x4, y4, col));
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawRectangle<nuiPixelBlender_Copy>(nuiVertex_Solid(x1, y1, col), nuiVertex_Solid(x2, y2, col), nuiVertex_Solid(x3, y3, col), nuiVertex_Solid(x4, y4, col));
          break;
      }
    }
    else
    {
      switch (mState.mBlendFunc)
      {
        case nuiBlendTransp:
          mpRasterizer->DrawRectangle<nuiPixelBlender_Transp>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                              nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                              nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)),
                                                              nuiVertex_Gouraud(x4, y4, nuiGouraudColor(c4)));
          break;
        case nuiBlendTranspAdd:
          mpRasterizer->DrawRectangle<nuiPixelBlender_TranspAdd>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                                 nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                                 nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)),
                                                                 nuiVertex_Gouraud(x4, y4, nuiGouraudColor(c4)));
          break;
        case nuiBlendSource:
        default:
          mpRasterizer->DrawRectangle<nuiPixelBlender_Copy>(nuiVertex_Gouraud(x1, y1, nuiGouraudColor(c1)),
                                                            nuiVertex_Gouraud(x2, y2, nuiGouraudColor(c2)),
                                                            nuiVertex_Gouraud(x3, y3, nuiGouraudColor(c3)),
                                                            nuiVertex_Gouraud(x4, y4, nuiGouraudColor(c4)));
          break;
      }
    }
  }
}


void nuiSoftwarePainter::Display(nglWindow* pWindow, const nuiRect& rRect)
{
  if (!pWindow)
    return;
  const nglWindow::OSInfo* pInfo = pWindow->GetOSInfo();
  
  NGL_ASSERT(pInfo);

  if (!pInfo)
    return;

  //TestAgg((char*)&mBuffer[0], mWidth, mHeight);
  
  int32 x, y, w, h;
  x = ToBelow(rRect.Left());
  y = ToBelow(rRect.Top());
  w = ToBelow(rRect.GetWidth());
  h = ToBelow(rRect.GetHeight());
  int32 offset = (x + mWidth * y);
  
#ifdef _WIN32_
  HDC hdc = GetDC(pInfo->GLWindowHandle);

  BITMAPV5HEADER bminfo;

  // BITMAPV5HEADER
  bminfo.bV5Size = sizeof(BITMAPV5HEADER);
  bminfo.bV5Width = (LONG)mWidth;
  bminfo.bV5Height = -(LONG)mHeight;
  bminfo.bV5Planes = 1;
  bminfo.bV5BitCount = 32;
  bminfo.bV5Compression = BI_RGB;
  bminfo.bV5SizeImage = 0;
  bminfo.bV5XPelsPerMeter = 0;
  bminfo.bV5YPelsPerMeter = 0;
  bminfo.bV5ClrUsed = 0;
  bminfo.bV5ClrImportant =0;

  bminfo.bV5RedMask = 0;
  bminfo.bV5GreenMask = 0;
  bminfo.bV5BlueMask = 0;
  bminfo.bV5AlphaMask = 0;

  bminfo.bV5CSType = LCS_WINDOWS_COLOR_SPACE;
  bminfo.bV5Endpoints.ciexyzRed.ciexyzX = 0;
  bminfo.bV5Endpoints.ciexyzRed.ciexyzY = 0;
  bminfo.bV5Endpoints.ciexyzRed.ciexyzZ = 0;
  bminfo.bV5Endpoints.ciexyzGreen.ciexyzX = 0;
  bminfo.bV5Endpoints.ciexyzGreen.ciexyzY = 0;
  bminfo.bV5Endpoints.ciexyzGreen.ciexyzZ = 0;
  bminfo.bV5Endpoints.ciexyzBlue.ciexyzX = 0;
  bminfo.bV5Endpoints.ciexyzBlue.ciexyzY = 0;
  bminfo.bV5Endpoints.ciexyzBlue.ciexyzZ = 0;
  bminfo.bV5GammaRed = 0;
  bminfo.bV5GammaGreen = 0;
  bminfo.bV5GammaBlue = 0;
  bminfo.bV5Intent = LCS_GM_IMAGES;
  bminfo.bV5ProfileData = 0;
  bminfo.bV5ProfileSize = 0;

  bminfo.bV5Reserved = 0;

  RECT r;
  GetClientRect(pInfo->GLWindowHandle, &r);

  r.right = (LONG)mWidth;
  r.bottom = (LONG)mHeight;

  uint32* pBuffer = mpRasterizer->GetBuffer();
/*
  for (uint32 i = 0; i < mWidth * mHeight; i++)
    pBuffer[i] = rand();
*/

  SetDIBitsToDevice(
    hdc,              // handle to DC
    x,                // x-coord of destination upper-left corner
    y,                // y-coord of destination upper-left corner
    w,          // width of destination rectangle
    h,         // height of destination rectangle
    x,                // x-coord of source upper-left corner
    mHeight - h - y,                // y-coord of source upper-left corner

    0,                // first scan line
    (LONG)mHeight,    // number of scan lines

    pBuffer,          // bitmap bits
    (const BITMAPINFO*)&bminfo,          // bitmap data
    DIB_RGB_COLORS    // usage options
    );

  ReleaseDC(pInfo->GLWindowHandle, hdc);

#elif (defined _CARBON_)

  WindowRef win = pInfo->WindowHandle;
  if (!win)
    win = pInfo->Parent;
  
  //CGContextRef myMemoryContext;
  CGColorSpaceRef cspace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
  CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, mpRasterizer->GetBuffer() + offset, mWidth * h * 4, NULL);
  
  CGImageRef img = CGImageCreate(
                                 w,
                                 h,
                                 8,
                                 32,
                                 mWidth * 4,
                                 cspace,
                                 kCGBitmapByteOrder32Host | kCGImageAlphaNoneSkipFirst,
                                 provider,
                                 NULL,
                                 0,
                                 kCGRenderingIntentDefault
                                 );
  
  CGContextRef myContext;
  
  SetPortWindowPort (win);// 1
  QDBeginCGContext (GetWindowPort (win), &myContext);

  CGRect rect = CGRectMake(x, mHeight - h - y, w, h);
  CGContextDrawImage(myContext, rect, img);
  
  QDEndCGContext (GetWindowPort(win), &myContext);  
          
#endif
}

