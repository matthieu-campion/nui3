/*
NUI - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sébastien Métrot

This library is free software{} you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation{} either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY{} without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library{} if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nuiMemoryDrawContext.h"
#include "nuiGLPainter.h"

#include "agg.h"

//#define NO_SOFT_BLENDING
#ifndef __CARBON__
#define NUI_USE_BGRA
#endif // __CARBON__ 

#if (BYTE_ORDER == LITTLE_ENDIAN)
#define SPAN_TYPE agg::span_bgra32
#elif (BYTE_ORDER == BIG_ENDIAN)
#define SPAN_TYPE agg::span_rgba32
#endif

#include "nuiContour.h"

#if 0

class nuiAGGPrivate
{
public:
  nuiAGGPrivate(nuiMemoryDrawContext* pContext)
  {
    void* pImage = pContext->GetImageBuffer();
    uint32 Width = pContext->GetWidth(), Height = pContext->GetHeight();

    // Create the rendering buffer 
    mpRenderingBuffer = new agg::rendering_buffer((unsigned char*)pImage, Width, Height, Width * 4);

    // Create the renderer and the rasterizer
    mpRenderer = new agg::renderer<SPAN_TYPE>(*mpRenderingBuffer);
    mpRasterizer = new agg::rasterizer();

    // Setup the rasterizer
    mpRasterizer->gamma(1.0);
    mpRasterizer->filling_rule(agg::fill_even_odd);
  }

  virtual ~nuiAGGPrivate()
  {
    delete mpRenderingBuffer;
    delete mpRenderer;
    delete mpRasterizer;
  }

  agg::rendering_buffer * mpRenderingBuffer;
  agg::renderer<SPAN_TYPE>* mpRenderer;
  agg::rasterizer* mpRasterizer;
};

nuiMemoryDrawContext::nuiMemoryDrawContext(const nuiRect& rRect)
: nuiDrawContext(rRect)
{
  mpImage = NULL;
  mpLineBuffer = NULL;
  nuiSize w,h;
  w = mWidth;
  h = mHeight;
  mWidth = 0;
  mHeight = 0;

  mpTriangleMin = NULL;
  mpTriangleMax = NULL;
  mpColorMin = NULL;
  mpColorMax = NULL;

  mpAGGPrivate = NULL;

  SetSize(ToZero(w),ToZero(h));

  mMatrixStack.push(nglMatrixf());
}

nuiMemoryDrawContext::~nuiMemoryDrawContext()
{
  if (mpImage)
    free (mpImage);
  if (mpLineBuffer)
    free(mpLineBuffer);

  if (mpTriangleMin)
    free (mpTriangleMin);
  if (mpTriangleMax)
    free (mpTriangleMax);
  if (mpColorMin)
    free (mpColorMin);
  if (mpColorMax)
    free (mpColorMax);

  if (mpAGGPrivate)
    delete mpAGGPrivate;
}

void nuiMemoryDrawContext::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  mClipOffsetX = ClipOffsetX;
  mClipOffsetY = ClipOffsetY;
}

void nuiMemoryDrawContext::StopRendering()
{
}


/****************************************************************************
*
* Clipping Support
*
****************************************************************************/
void nuiMemoryDrawContext::ActivateClipper()
{
  mTransformedClippingRect = mClippingRect;
}

/****************************************************************************
*
* Render state manipulation
*
****************************************************************************/
void nuiMemoryDrawContext::RetrieveState()
{
}
/****************************************************************************
*
* Texture manipulation
*
****************************************************************************/

bool nuiMemoryDrawContext::SetTexture(nuiTexture* pTex) 
{ 
  if ( mCurrentState.mpTexture != pTex ) 
    mCurrentState.mpTexture = pTex ;
  mCurrentState.mGroup1Changed = true;
  return true;
}

/****************************************************************************
*
* Helper function
*
****************************************************************************/
void nuiMemoryDrawContext::Save(const nglPath& rPath, const nglString& rImageType)
{
  nglImageInfo Info;
  Info.mBufferFormat = eImageFormatRaw;
  Info.mPixelFormat = eImagePixelRGBA;
  Info.mWidth = ToZero(mWidth);
  Info.mHeight = ToZero(mHeight);
  Info.mBitDepth = 32; // Only 32bits implemented atm
  Info.mBytesPerPixel = 4; // 32 bits = 4 bytes per pixel (yes! :)
  Info.mBytesPerLine = Info.mBytesPerPixel * Info.mWidth;
  Info.mpBuffer = (char*)mpImage;
  nglImage Image(Info,false);

  Image.Save(rPath,nglImage::CreateCodec(rImageType));
}

/****************************************************************************
*
* Drawing function
*
****************************************************************************/
void nuiMemoryDrawContext::SetSize(int w,int h)
{
  if (w * h != mWidth * mHeight)
  {
    mpImage = (Color*)realloc(mpImage, w * h * sizeof(nuiMemoryDrawContext::Color));
    mpLineBuffer = (Color*)realloc(mpLineBuffer, w * sizeof(nuiMemoryDrawContext::Color));
    mWidth = (nuiSize)w;
    mHeight = (nuiSize)h;

    mTransformedClippingRect.mLeft   = 0;
    mTransformedClippingRect.mRight  = (nuiSize) w;
    mTransformedClippingRect.mTop    = 0;
    mTransformedClippingRect.mBottom = (nuiSize) h;
  }

  mpTriangleMin = (int*) realloc(mpTriangleMin, sizeof(int) * h);
  mpTriangleMax = (int*) realloc(mpTriangleMax, sizeof(int) * h);
  mpColorMin = (Color*) realloc(mpColorMin, sizeof(Color) * h);
  mpColorMax = (Color*) realloc(mpColorMax, sizeof(Color) * h);

  // Create AGG structures:
  if (mpAGGPrivate)
  {
    delete mpAGGPrivate;
  }

  mpAGGPrivate = new nuiAGGPrivate(this);
}


// Fast and simple drawings:
void nuiMemoryDrawContext::DrawRect(const nuiRect& rRect, nuiShapeMode Mode)
{
  nuiRect Rect;
  nglVectorf P1(rRect.mLeft, rRect.mTop, 0);
  nglVectorf P2(rRect.mRight, rRect.mBottom, 0);

  P1 = mMatrixStack.top() * nglVectorf(rRect.mLeft, rRect.mTop, 0);
  P2 = mMatrixStack.top() * nglVectorf(rRect.mRight, rRect.mBottom, 0);

  Rect.Set(
    P1[0],
    P1[1],
    P2[0]-P1[0],
    P2[1]-P1[1]
    );

    if (mTransformedClippingRect.mEnabled)
      Rect.Intersect(Rect,mTransformedClippingRect);
    else
      Rect.Intersect(Rect,nuiRect(0, 0, ToZero(mWidth), ToZero(mHeight)));

    uint height = ToZero(Rect.GetHeight());
    uint width  = ToZero(Rect.GetWidth());

    uint w = ToZero(mWidth);
    Color* pColor = mpImage + ToZero(Rect.mTop) * w + ToZero(Rect.mLeft);
    Color color;
    color = mCurrentState.mFillColor;

    if (Mode == eStrokeAndFillShape || Mode == eFillShape)
    {
      if (mCurrentState.mBlending && (color.mA != 1))
      {
        while (height--)
        {
          RasterLineA(pColor, width, color);
          pColor += w;
        }
      }
      else
      {
        while (height--)
        {
          RasterLine(pColor, width, color);
          pColor += w;
        }
      }
    }

    if (Mode == eStrokeAndFillShape || Mode == eStrokeShape)
    {
      DrawLine(rRect.mLeft,  rRect.mTop,    rRect.mRight, rRect.mTop);
      DrawLine(rRect.mRight, rRect.mTop,    rRect.mRight, rRect.mBottom);
      DrawLine(rRect.mLeft,  rRect.mBottom, rRect.mRight, rRect.mBottom);
      DrawLine(rRect.mLeft,  rRect.mTop,    rRect.mLeft,  rRect.mBottom);
    }
}


// Cohen-Sutherland line-clipping algorithm 
/* 
* define the coding of end points
*/
enum 
{
  BOTTOM = 0x1, 
  TOP    = 0x2, 
  RIGHT  = 0x4, 
  LEFT   = 0x8
};

/*
* compute the code of a point relative to a rectangle
*/
uint ComputeCode (nuiSize x, nuiSize y, const nuiRect& r)
{
  uint c = 0;
  if (y > r.mBottom)
    c |= BOTTOM;
  else if (y < r.mTop)
    c |= TOP;
  if (x > r.mRight)
    c |= RIGHT;
  else if (x < r.mLeft)
    c |= LEFT;
  return c;
}


void nuiMemoryDrawContext::DrawLine(const nglVector2f& rOrigin, const nglVector2f& rDestination)
{
  if (mCurrentState.mAntialiasing || mCurrentState.mLineWidth == 1.0f)
  {
    nglVectorf P1 = mMatrixStack.top() * nglVectorf(rOrigin[0],rOrigin[1],0);
    nglVectorf P2 = mMatrixStack.top() * nglVectorf(rDestination[0],rDestination[1],0);
    nglVector2f Origin, Destination;
    Origin[0] = P1[0];
    Origin[1] = P1[1];
    Destination[0] = P2[0];
    Destination[1] = P2[1];

    //Clipping:
    nuiRect clip(0.0f,0.0f,mWidth,mHeight);
    if (mTransformedClippingRect.mEnabled)
      clip = mTransformedClippingRect;

    if (ToZero(clip.mRight) > 0)
      clip.mRight -= 1;
    if (ToZero(clip.mBottom) > 0)
      clip.mBottom -= 1;

    nuiSize x0 = Origin[0];
    nuiSize y0 = Origin[1];
    nuiSize x1 = Destination[0];
    nuiSize y1 = Destination[1];

    uint C0, C1, C;
    nuiSize x, y;

    C0 = ComputeCode (x0, y0, clip);
    C1 = ComputeCode (x1, y1, clip);

    for (;;) 
    {
      /* trivial accept: both ends in rectangle */
      if ((C0 | C1) == 0) 
      {
        Color color;
        color = mCurrentState.mStrokeColor;
        bool blended = mCurrentState.mBlending && (color.mA < 255);
        BresenhamLine(ToZero(x0), ToZero(y0), ToZero(x1), ToZero(y1),  blended ? &nuiMemoryDrawContext::BlendPixel : &nuiMemoryDrawContext::SetPixel, color);
        return;
      }

      /* trivial reject: both ends on the external side of the rectangle */
      if ((C0 & C1) != 0)
        return;

      /* normal case: clip end outside rectangle */
      C = C0 ? C0 : C1;
      if (C & TOP) 
      {
        x = x0 + (x1 - x0) * (clip.mTop - y0) / (y1 - y0);
        y = clip.mTop;
      } 
      else if (C & BOTTOM) 
      {
        x = x0 + (x1 - x0) * (clip.mBottom - y0) / (y1 - y0);
        y = clip.mBottom;
      } 
      else if (C & RIGHT) 
      {
        x = clip.mRight;
        y = y0 + (y1 - y0) * (clip.mRight - x0) / (x1 - x0);
      } 
      else 
      {
        x = clip.mLeft;
        y = y0 + (y1 - y0) * (clip.mLeft - x0) / (x1 - x0);
      }

      /* set new end point and iterate */
      if (C == C0) 
      {
        x0 = x; 
        y0 = y;
        C0 = ComputeCode (x0, y0, clip);
      } 
      else 
      {
        x1 = x; 
        y1 = y;
        C1 = ComputeCode (x1, y1, clip);
      }
    }
  }
  else
  {
    float width = mCurrentState.mLineWidth;
    float x1, y1, x2, y2;
    x1 = rOrigin[0];
    y1 = rOrigin[1];
    x2 = rDestination[0];
    y2 = rDestination[1];
    float dx = x2 - x1;
    float dy = y2 - y1;
    float d = 3.0f * (float)sqrt(dx*dx + dy*dy);

    if (d == 0.0f)
      d = 1.0f;

    dx = width * (y2 - y1) / d;
    dy = width * (x2 - x1) / d;

    nuiShape shape;
    nuiContour* contour = new nuiContour();
    nuiPath points;

    nuiPoint p1(x1 - dx,  y1 + dy, 0);
    nuiPoint p2(x2 - dx,  y2 + dy, 0);
    nuiPoint p3(x2 + dx,  y2 - dy, 0);
    nuiPoint p4(x1 + dx,  y1 - dy, 0);

    nuiColor c = mCurrentState.mFillColor;

    points.AddVertex(p1);
    points.AddVertex(p2);
    points.AddVertex(p3);
    points.AddVertex(p4);

    contour->AddLines(points);
    shape.AddContour(contour);

    DrawShape(&shape, eFillShape);

    mCurrentState.mFillColor = c;
  }
}

void nuiMemoryDrawContext::DrawLine(float x1, float y1, float x2, float y2)
{
  DrawLine(nglVector2f(x1,y1),nglVector2f(x2,y2));
}

/*
void nuiMemoryDrawContext::DrawCachedShape(std::list<nuiShape::CacheElement*>* pElements, nuiShapeMode Mode)
{
  //  nuiShape* pShape = NULL; // (unused)
  bool blend = mCurrentState.mBlending;
  uint func1,func2;
  bool tex2D;
  func1 = mCurrentState.mBlendSourceFactor;
  func2 = mCurrentState.mBlendDestinationFactor;
  tex2D = mCurrentState.mTexture2D;

  if (Mode == eStrokeShape)
  {
    if (mCurrentState.mAntialiasing && mPermitAntialising)
    {
      glEnable(GL_LINE_SMOOTH);
      EnableBlending(true);
      SetBlendFunc(nuiBlendTransp);
    }
    else
    {
      glDisable(GL_LINE_SMOOTH);
    }

    //    glLineWidth(mCurrentState.mLineWidth);
  }

  std::list<nuiShape::CacheElement*>::iterator it;
  std::list<nuiShape::CacheElement*>::iterator end = pElements->end();

  it = pElements->begin();

  nuiShape::CacheElement::Vertex* pItem1,*pItem2,*pItem3,*pItem4;
  pItem1 = pItem2 = pItem3 = pItem4 = NULL;

  for (; it != end; ++it)
  {
    nuiShape::CacheElement* pElement = *it;
    uint i;

    if (pElement->mOperation == GL_LINES)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        pItem1 = &pElement->mpVertices[i];
        i++;
        pItem2 = &pElement->mpVertices[i];
        mCurrentState.mStrokeColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawLine(pItem1->mCoord[0],pItem1->mCoord[1],pItem2->mCoord[0],pItem2->mCoord[1]);
      }
    }
    else if (pElement->mOperation == GL_LINE_LOOP)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        if (!pItem1)
        {
          pItem1 = &pElement->mpVertices[i];
          i++;
        }
        if (i>= pElement->mCount)
          pItem2 = &pElement->mpVertices[0];
        else
          pItem2 = &pElement->mpVertices[i];
        mCurrentState.mStrokeColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawLine(pItem1->mCoord[0],pItem1->mCoord[1],pItem2->mCoord[0],pItem2->mCoord[1]);
        pItem1 = pItem2;
      }
    }
    else if (pElement->mOperation == GL_LINE_STRIP)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        if (!pItem1)
        {
          pItem1 = &pElement->mpVertices[i];
          i++;
        }
        if (i>= pElement->mCount)
          pItem2 = &pElement->mpVertices[0];
        else
          pItem2 = &pElement->mpVertices[i];
        mCurrentState.mStrokeColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawLine(pItem1->mCoord[0],pItem1->mCoord[1],pItem2->mCoord[0],pItem2->mCoord[1]);
      }
    }
    else if (pElement->mOperation == GL_TRIANGLES)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        pItem1 = &pElement->mpVertices[i++];
        pItem2 = &pElement->mpVertices[i++];
        pItem3 = &pElement->mpVertices[i];
        mCurrentState.mFillColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawTriangle(pItem1,pItem2,pItem3);
      }
    }
    else if (pElement->mOperation == GL_TRIANGLE_FAN)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        if (!pItem1)
          pItem1 = &pElement->mpVertices[i++];

        if (!pItem2)
          pItem2 = &pElement->mpVertices[i++];

        pItem3 = &pElement->mpVertices[i];
        mCurrentState.mFillColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawTriangle(pItem1,pItem2,pItem3);
        pItem2 = pItem3;
      }
    }
    else if (pElement->mOperation == GL_TRIANGLE_STRIP)
    {
      for (i = 0; i<pElement->mCount; i++)
      {
        if (!pItem1)
          pItem1 = &pElement->mpVertices[i++];

        if (!pItem2)
          pItem2 = &pElement->mpVertices[i++];

        pItem3 = &pElement->mpVertices[i];
        mCurrentState.mFillColor.Set(pItem1->mColor[0],pItem1->mColor[1],pItem1->mColor[2],pItem1->mColor[3]);
        DrawTriangle(pItem1,pItem2,pItem3);
        pItem1 = pItem2;
        pItem2 = pItem3;
      }
    }

    pItem1 = pItem2 = pItem3 = NULL;
  }

  //EnableLineSmooth(false);
  SetBlendFunc(func1, func2);
  EnableTexturing(tex2D);
  EnableBlending(blend);
}
*/

// Rasterizer engine:

// Without texturing:
void nuiMemoryDrawContext::RasterLine(Color* pScreen, uint size, const Color& rColor)
{
  if (rColor.mB == rColor.mG && rColor.mG == rColor.mR)
  {
    memset(pScreen, rColor.mR, size * 4);
    return;
  }

  while (size--)
    *pScreen++ = rColor; 
}


void nuiMemoryDrawContext::RasterLineA(Color* pScreen, uint size, const Color& rColor)
{
#if 1

  if (rColor.mA < 129 && rColor.mA > 126) // optimize things a bit, ok, I cheat...
  {
    while (size--)
    {
      pScreen->FastBlendWith(rColor);
      pScreen++;
    }
    return;
  }

  while (size--)
  {
    pScreen->BlendWith(rColor);
    pScreen++;
  }
#else
  Color c;
  c.mR = 255;
  c.mG = 0;
  c.mB = 0;

  RasterLine(pScreen,size, c);
#endif
}

void nuiMemoryDrawContext::RasterLine(Color* pScreen, uint size, const Color& rSColor, const Color& rDColor)
{
  short incrR,incrG,incrB;
  short R,G,B;
  incrR = (rDColor.mR - rSColor.mR) << 7;
  incrG = (rDColor.mG - rSColor.mG) << 7;
  incrB = (rDColor.mB - rSColor.mB) << 7;
  int s = (int)size;
  incrR /= s;
  incrG /= s;
  incrB /= s;
  R = rSColor.mR<<7;
  G = rSColor.mG<<7;
  B = rSColor.mB<<7;

  while (size--)
  {
    pScreen->mR = R>>7;
    pScreen->mG = G>>7;
    pScreen->mB = B>>7;
    R += incrR;
    G += incrG;
    B += incrB;
    pScreen++;
  }
}

void nuiMemoryDrawContext::RasterLineA(Color* pScreen, uint size, const Color& rSColor, const Color& rDColor)
{
  short incrR,incrG,incrB,incrA;
  short R,G,B,A;
  Color Temp;
  incrR = (rDColor.mR - rSColor.mR) << 7;
  incrG = (rDColor.mG - rSColor.mG) << 7;
  incrB = (rDColor.mB - rSColor.mB) << 7;
  incrA = (rDColor.mA - rSColor.mA) << 7;
  int s = (int)size;
  incrR /= s;
  incrG /= s;
  incrB /= s;
  incrA /= s;
  R = rSColor.mR<<7;
  G = rSColor.mG<<7;
  B = rSColor.mB<<7;
  A = rSColor.mA<<7;

  while (size--)
  {
    Temp.mR = R>>7;
    Temp.mG = G>>7;
    Temp.mB = B>>7;
    Temp.mA = A>>7;
    pScreen->BlendWith(Temp);
    R += incrR;
    G += incrG;
    B += incrB;
    A += incrA;
    pScreen++;
  }
}


// Draw line:
void nuiMemoryDrawContext::SetPixel(Color* pScreen, const Color& rColor)
{
  *pScreen = rColor;
}

void nuiMemoryDrawContext::BlendPixel(Color* pScreen, const Color& rColor)
{
  pScreen->BlendWith(rColor);
}

void nuiMemoryDrawContext::BresenhamLine(int x0, int y0, int x1, int y1, DrawPixelFunc _DrawPixel, const Color& rColor)
{
  if (mCurrentState.mAntialiasing) // Anti-aliased lines are drawn with AGG
  {
    float width = mCurrentState.mLineWidth;
    float dx = (float)(x1 - x0);
    float dy = (float)(y1 - y0);
    float d = (float)sqrt(dx*dx + dy*dy);

    dx = width * (y1 - y0) / d;
    dy = width * (x1 - x0) / d;

    mpAGGPrivate->mpRasterizer->move_to_d(x0 - dx,  y0 + dy);
    mpAGGPrivate->mpRasterizer->line_to_d(x1 - dx,  y1 + dy);
    mpAGGPrivate->mpRasterizer->line_to_d(x1 + dx,  y1 - dy);
    mpAGGPrivate->mpRasterizer->line_to_d(x0 + dx,  y0 - dy);

    mpAGGPrivate->mpRasterizer->render(*(mpAGGPrivate->mpRenderer), 
      agg::rgba8(rColor.mR, rColor.mG, rColor.mB, mCurrentState.mBlending ? rColor.mA:255 ));
  }
  else
  {
    // Starting point of line:
    int x = x0, y = y0;

    // Direction of line:
    int dx = x1 - x0, dy = y1 - y0;

    int w = ToZero(mWidth);
    Color* pImage = mpImage + x + y * w;

    // Increment of decrement depending on direction of line
    int sx, sy;
    if ( dx > 0 )
    {
      sx = 1;
    }
    else if ( dx < 0 )
    {
      sx = -1;
      dx = -dx;
    }
    else
    {
      sx = 0;
    }

    if ( dy > 0 )
    {
      sy = 1;
    }
    else if ( dy < 0 )
    {
      sy = -1;
      w = -w;
      dy = -dy;
    }
    else
    {
      sy = 0;
    }

    // Let's care about horizontal & vertical lines:
    if (!dx)
    {
      dy++;
      while(dy--)
      {
        (this->*_DrawPixel)(pImage, rColor);
        pImage += w;
      }
      return;
    }

    if (!dy)
    {
      int inc = 1;
      if ( x0 > x1)
        inc = -1;
      dx++;
      while(dx--)
      {
        (this->*_DrawPixel)(pImage, rColor);
        pImage += inc;
      }
      return;
    }



    int ax = 2 * dx, ay = 2 * dy;

    if ( dy <= dx )
    {
      // Single step in x direction
      for (int decy = ay - dx; /**/ ; x += sx, decy += ay, pImage += sx)
      {
        (this->*_DrawPixel)(pImage, rColor);

        // Take brensenham step
        if ( x == x1 )
          break;
        if ( decy >= 0 )
        {
          decy -= ax;
          //        y += sy;
          pImage += w;
        }
      }
    }
    else
    {
      // Single step in y direction
      for (int decx = ax - dy; /**/ ; y += sy, decx += ax, pImage += w)
      {
        (this->*_DrawPixel)(pImage, rColor);

        // Take brensenham step
        if ( y == y1 )
          break;
        if ( decx >= 0 )
        {
          decx -= ay;
          //        x += sx;
          pImage += sx;
        }
      }
    }
  }
}

// Triangle Rasterizer:

void nuiMemoryDrawContext::DrawTriangle(TempVert* pP1, TempVert* pP2, TempVert* pP3)
{
  mTriRasterStart = ToZero(mHeight);
  mTriRasterEnd = 0;

  int i;
  int w = ToZero(mWidth);
  int h = ToZero(mHeight);
  int* pMins = mpTriangleMin;
  for (i = 0; i < h; i++)
    *pMins++ = w;
  memset( mpTriangleMax, 0, sizeof(int)*h);


  // Fill the edge array:
  CalcTriangleBorder(pP1,pP2);
  CalcTriangleBorder(pP1,pP3);
  CalcTriangleBorder(pP2,pP3);

  if (mTriRasterEnd < mTriRasterStart)
    return;

  Color color1;
  //  Color color2;
  color1 = mCurrentState.mFillColor;

  if (mCurrentState.mBlending && (color1.mA != 1))
  {
    for (i = mTriRasterStart; i < mTriRasterEnd; i++)
    {
      int mx = mpTriangleMax[i];
      int mn = mpTriangleMin[i];

      int size =  mx - mn;
      if (size > 0)
      {
        Color* pScreen = mpImage + mn + (uint)(i * w);
        RasterLineA(pScreen, size, color1);
      }
    }
  }
  else
  {
    for (i = mTriRasterStart; i < mTriRasterEnd; i++)
    {
      int mx = mpTriangleMax[i];
      int mn = mpTriangleMin[i];

      int size =  mx - mn;
      if (size > 0)
      {
        Color* pScreen = mpImage + mn + (uint)(i * w);
        RasterLine(pScreen, size, color1);
      }
    }
  }
}

void nuiMemoryDrawContext::CalcTriangleBorder(TempVert* pOrigin, TempVert* pDestination)
{
  nglVectorf P1 = mMatrixStack.top() * nglVectorf(pOrigin->mVertex[0],pOrigin->mVertex[1],0);
  nglVectorf P2 = mMatrixStack.top() * nglVectorf(pDestination->mVertex[0],pDestination->mVertex[1],0);

  TempVert p1,p2;
  p1 = *pOrigin;
  p2 = *pDestination;
  p1.mVertex[0] = P1[0];
  p1.mVertex[1] = P1[1];
  p2.mVertex[0] = P2[0];
  p2.mVertex[1] = P2[1];

  //Clipping:
  nuiRect clip(0.0f,0.0f,mWidth,mHeight);
  if (mTransformedClippingRect.mEnabled)
    clip = mTransformedClippingRect;

  if (ToZero(clip.mRight) > 0)
    clip.mRight -= 1;
  if (ToZero(clip.mBottom) > 0)
    clip.mBottom -= 1;


  uint C0, C1, C;
  nuiSize x, y;

  C0 = ComputeCode (p1.mVertex[0], p1.mVertex[1], clip);
  C1 = ComputeCode (p2.mVertex[0], p2.mVertex[1], clip);

  for (;;) 
  {
    /* trivial accept: both ends in rectangle */
    if ((C0 | C1) == 0) 
    {
      Color color;
      color = mCurrentState.mStrokeColor;
      BresenhamTriangleLine(&p1,&p2);
      return;
    }

    /* trivial reject: both ends on the external side of the rectangle */
    if ((C0 & C1) != 0)
    {
      TempVert tp1,tp2;
      tp1 = p1;
      tp2 = p2;
      // Don't forget to fill the right precalc array or the triangles will have holes:
      if (p1.mVertex[0] <= clip.mLeft && p2.mVertex[0] <= clip.mLeft)
      {
        tp1.mVertex[0] = tp2.mVertex[0] = clip.mLeft;
        tp1.mVertex[1] = MIN(p1.mVertex[1],p2.mVertex[1]);
        tp2.mVertex[1] = MAX(p1.mVertex[1],p2.mVertex[1]);
        tp1.mVertex[1] = MIN(MAX(p1.mVertex[1],clip.mTop), clip.mBottom);
        tp2.mVertex[1] = MAX(MIN(p2.mVertex[1],clip.mBottom), clip.mTop);

        BresenhamTriangleLine(&tp1,&tp2);
      }
      else if (p1.mVertex[0] >= clip.mRight && p2.mVertex[0] >= clip.mRight)
      {
        tp1.mVertex[0] = tp2.mVertex[0] = clip.mRight;
        tp1.mVertex[1] = MIN(p1.mVertex[1],p2.mVertex[1]);
        tp2.mVertex[1] = MAX(p1.mVertex[1],p2.mVertex[1]);
        tp1.mVertex[1] = MIN(MAX(p1.mVertex[1],clip.mTop), clip.mBottom);
        tp2.mVertex[1] = MAX(MIN(p2.mVertex[1],clip.mBottom), clip.mTop);

        BresenhamTriangleLine(&tp1,&tp2);
      }
      return;
    }

    /* normal case: clip end outside rectangle */
    C = C0 ? C0 : C1;
    if (C & TOP) 
    {
      x = p1.mVertex[0] + (p2.mVertex[0] - p1.mVertex[0]) * (clip.mTop - p1.mVertex[1]) / (p2.mVertex[1] - p1.mVertex[1]);
      y = clip.mTop;
    } 
    else if (C & BOTTOM) 
    {
      x = p1.mVertex[0] + (p2.mVertex[0] - p1.mVertex[0]) * (clip.mBottom - p1.mVertex[1]) / (p2.mVertex[1] - p1.mVertex[1]);
      y = clip.mBottom;
    } 
    else if (C & RIGHT) 
    {
      x = clip.mRight;
      y = p1.mVertex[1] + (p2.mVertex[1] - p1.mVertex[1]) * (clip.mRight - p1.mVertex[0]) / (p2.mVertex[0] - p1.mVertex[0]);

      TempVert tp1,tp2;
      tp1 = p1;
      tp2 = p2;
      tp1.mVertex[0] = tp2.mVertex[0] = x;
      tp1.mVertex[1] = tp2.mVertex[1] = y;
      if (C == C0)
        tp1.mVertex[1] = p1.mVertex[1];
      else
        tp2.mVertex[1] = p2.mVertex[1];

      if (tp1.mVertex[1] > clip.mBottom)
        tp1.mVertex[1] = clip.mBottom;
      if (tp1.mVertex[1] < clip.mTop)
        tp1.mVertex[1] = clip.mTop;
      if (tp2.mVertex[1] > clip.mBottom)
        tp2.mVertex[1] = clip.mBottom;
      if (tp2.mVertex[1] < clip.mTop)
        tp2.mVertex[1] = clip.mTop;

      BresenhamTriangleLine(&tp1,&tp2);
    } 
    else 
    {
      x = clip.mLeft;
      y = p1.mVertex[1] + (p2.mVertex[1] - p1.mVertex[1]) * (clip.mLeft - p1.mVertex[0]) / (p2.mVertex[0] - p1.mVertex[0]);

      TempVert tp1,tp2;
      tp1 = p1;
      tp2 = p2;
      tp1.mVertex[0] = tp2.mVertex[0] = x;
      tp1.mVertex[1] = tp2.mVertex[1] = y;
      if (C == C0)
        tp1.mVertex[1] = p1.mVertex[1];
      else
        tp2.mVertex[1] = p2.mVertex[1];

      if (tp1.mVertex[1] > clip.mBottom)
        tp1.mVertex[1] = clip.mBottom;
      if (tp1.mVertex[1] < clip.mTop)
        tp1.mVertex[1] = clip.mTop;
      if (tp2.mVertex[1] > clip.mBottom)
        tp2.mVertex[1] = clip.mBottom;
      if (tp2.mVertex[1] < clip.mTop)
        tp2.mVertex[1] = clip.mTop;

      BresenhamTriangleLine(&tp1,&tp2);
    }

    /* set new end point and iterate */
    if (C == C0) 
    {
      p1.mVertex[0] = x; 
      p1.mVertex[1] = y;
      C0 = ComputeCode (p1.mVertex[0], p1.mVertex[1], clip);
    } 
    else 
    {
      p2.mVertex[0] = x; 
      p2.mVertex[1] = y;
      C1 = ComputeCode (p2.mVertex[0], p2.mVertex[1], clip);
    }
  }
}

void nuiMemoryDrawContext::BresenhamTriangleLine(TempVert* pOrigin, TempVert* pDestination)
{
  int x0, y0, x1, y1;
  x0 = ToZero(pOrigin->mVertex[0]);
  y0 = ToZero(pOrigin->mVertex[1]);
  x1 = ToZero(pDestination->mVertex[0]);
  y1 = ToZero(pDestination->mVertex[1]);

  mTriRasterStart = MIN(mTriRasterStart,y0);
  mTriRasterStart = MIN(mTriRasterStart,y1);
  mTriRasterEnd = MAX(mTriRasterEnd,y0);
  mTriRasterEnd = MAX(mTriRasterEnd,y1);

  // Starting point of line:
  int x = x0, y = y0;

  // Direction of line:
  int dx = x1 - x0, dy = y1 - y0;

  // Increment of decrement depending on direction of line
  int sx, sy;
  if ( dx > 0 )
  {
    sx = 1;
  }
  else if ( dx < 0 )
  {
    sx = -1;
    dx = -dx;
  }
  else
  {
    sx = 0;
  }

  if ( dy > 0 )
  {
    sy = 1;
  }
  else if ( dy < 0 )
  {
    sy = -1;
    dy = -dy;
  }
  else
  {
    sy = 0;
  }

  // Let's care about horizontal & vertical lines:
  if (!dx)
  {
    while(dy--)
    {
      y += sy;
      mpTriangleMin[y] = MIN( mpTriangleMin[y], x );
      mpTriangleMax[y] = MAX( mpTriangleMax[y], x );
    }
    return;
  }

  if (!dy)
  {
    mpTriangleMin[y] = MIN( mpTriangleMin[y], x0 );
    mpTriangleMin[y] = MIN( mpTriangleMin[y], x1 );
    mpTriangleMax[y] = MAX( mpTriangleMax[y], x0 );
    mpTriangleMax[y] = MAX( mpTriangleMax[y], x1 );
    return;
  }

  int ax = 2 * dx, ay = 2 * dy;

  if ( dy <= dx )
  {
    // Single step in x direction
    for (int decy = ay - dx; /**/ ; x += sx, decy += ay)
    {
      mpTriangleMin[y] = MIN( mpTriangleMin[y], x );
      mpTriangleMax[y] = MAX( mpTriangleMax[y], x );
      // Take brensenham step
      if ( x == x1 )
        break;
      if ( decy >= 0 )
      {
        decy -= ax;
        y += sy;
      }
    }
  }
  else
  {
    // Single step in y direction
    for (int decx = ax - dy; /**/ ; y += sy, decx += ax)
    {
      mpTriangleMin[y] = MIN( mpTriangleMin[y], x );
      mpTriangleMax[y] = MAX( mpTriangleMax[y], x );

      // Take brensenham step
      if ( y == y1 )
        break;
      if ( decx >= 0 )
      {
        decx -= ay;
        x += sx;
      }
    }
  }
}

void nuiMemoryDrawContext::BlurRect(const nuiRect& rRect, uint Strength)
{
  /*
  nuiRect Rect = rRect;
  if (mTransformedClippingRect.mEnabled)
  Rect.Intersect(mTransformedClippingRect,rRect);
  nuiRect size = Rect.Size();

  nuiTexture* pScratchPad = GetScratchPad(ToZero(size.GetWidth()), ToZero(size.GetHeight()));

  if (!pScratchPad)
  return;

  SetTextureCurrent(pScratchPad,0);

  glPushMatrix();
  glLoadIdentity();

  EnableBlending(true);
  EnableTexturing(true);
  SetBlendFunc(nuiBlendTransp);

  do
  {
  glCopyTexSubImage2D(
  GL_TEXTURE_2D, 0, 
  0, 0, 
  ToZero(rRect.mLeft), ToZero(mHeight) - 1 - ToZero(rRect.mTop) - ToZero(rRect.GetHeight()), 
  ToZero(rRect.GetWidth()), ToZero(rRect.GetHeight())
  );

  SetFillColor(nuiColor(1,1,1,.1f));
  nuiRect rect = Rect;

  rect.Move(-1,-1);
  DrawImage(rect,size);
  rect.Move(1,0);
  DrawImage(rect,size);
  rect.Move(1,0);
  DrawImage(rect,size);

  rect.Move(-2,1);
  DrawImage(rect,size);
  rect.Move(1,0);
  DrawImage(rect,size);
  rect.Move(1,0);
  DrawImage(rect,size);

  rect.Move(-2,1);
  DrawImage(rect,size);
  rect.Move(0,1);
  DrawImage(rect,size);
  rect.Move(0,1);
  DrawImage(rect,size);
  } while ((long)(Strength--) > 0);

  EnableBlending(false);
  EnableTexturing(false);

  glPopMatrix();
  */
}


void nuiMemoryDrawContext::Display(nglWindow* pWindow)
{
  if (!pWindow)
    return;
  const nglWindow::OSInfo* pInfo = pWindow->GetOSInfo();
  if (!pInfo)
    return;

#ifdef _WIN32_
  HDC hdc = GetDC(pInfo->WindowHandle);

  BITMAPINFO* pbminfo;
  pbminfo = (BITMAPINFO*)malloc(sizeof(BITMAPV5HEADER));

  // BITMAPV5HEADER
  if (pbminfo)
  {
    BITMAPV5HEADER* pheader = (BITMAPV5HEADER*)pbminfo;
    pbminfo->bmiHeader.biClrImportant =0;
    pbminfo->bmiHeader.biClrUsed = 0;
    pbminfo->bmiHeader.biHeight = -(LONG)mHeight;
    pbminfo->bmiHeader.biWidth = (LONG)mWidth;
    pbminfo->bmiHeader.biPlanes = 1;
    pbminfo->bmiHeader.biSize = sizeof(BITMAPV5HEADER);
    pbminfo->bmiHeader.biSizeImage = (LONG)(4*mWidth*mHeight);
    //    bminfo->bmiHeader.biSizeImage = 0;
    pbminfo->bmiHeader.biXPelsPerMeter = 0;
    pbminfo->bmiHeader.biYPelsPerMeter = 0;

    pbminfo->bmiHeader.biCompression = BI_RGB;
    pbminfo->bmiHeader.biBitCount = 32;
  }
  else 
    return;

  RECT r;
  GetClientRect(pInfo->WindowHandle, &r);

  r.right = (LONG)mWidth;
  r.bottom = (LONG)mHeight;

  SetDIBitsToDevice(
    hdc,              // handle to DC
    0,                // x-coord of destination upper-left corner
    0,                // y-coord of destination upper-left corner
    r.right,          // width of destination rectangle
    r.bottom,         // height of destination rectangle
    0,                // x-coord of source upper-left corner
    0,                // y-coord of source upper-left corner

    0,                // first scan line
    (LONG)mHeight,    // number of scan lines

    mpImage,          // bitmap bits
    pbminfo,          // bitmap data
    DIB_RGB_COLORS    // usage options
    );

  free(pbminfo);
  ReleaseDC(pInfo->WindowHandle, hdc);

#elif (defined _CARBON_)

  /*
  static WindowRef win = NULL;

  if (!win)
  {
  Rect wRect;
  wRect.left = 0;
  wRect.top = 50;
  wRect.right = mWidth;
  wRect.bottom = mHeight + 50;
  win = NewCWindow(nil,
  &wRect, 
  (unsigned char*)"\008testtest", 
  true, 
  0, 
  (WindowRef) -1, 
  true, 
  0);

  }

  PixMap pixmap;
  pixmap.baseAddr = (char*)mpImage;
  pixmap.rowBytes = ((int32)mWidth * 4) | 0x8000; 
  pixmap.bounds.left = pixmap.bounds.top = 0;
  pixmap.bounds.right = mWidth-1;
  pixmap.bounds.bottom = mHeight-1;
  pixmap.pmVersion = 4;
  pixmap.packType = 0;
  pixmap.packSize = 0; 
  pixmap.hRes = 0x0480000; 
  pixmap.vRes = 0x0480000;
  pixmap.pixelType = RGBDirect;
  pixmap.pixelSize = 32;
  pixmap.cmpCount = 3;
  pixmap.cmpSize = 8;
  pixmap.pixelFormat = 0; 
  pixmap.pmTable = NULL;
  pixmap.pmExt = NULL; 

  Rect SrcRect,DstRect;

  GetPortBounds(GetWindowPort(pInfo->WindowHandle), &DstRect);

  SetPort ( GetWindowPort(win) );

  BitMap* pDst = (BitMap*)(GetPortBitMapForCopyBits(GetWindowPort(win)));
  CopyBits((BitMap*)&pixmap,pDst,&pixmap.bounds,&DstRect,srcCopy,NULL);

  glDrawPixels(mWidth, mHeight, GL_BGRA, GL_UNSIGNED_BYTE, mpImage);
  pWindow->EndSession();
  */
#endif
}


void nuiMemoryDrawContext::Clear()
{
  if (mClippingRect.mEnabled)
  {
    uint height = ToZero(mTransformedClippingRect.GetHeight());
    uint width  = ToZero(mTransformedClippingRect.GetWidth());

    uint w = ToZero(mWidth);

    Color* pColor = mpImage + ToZero(mTransformedClippingRect.mTop) * w + ToZero(mTransformedClippingRect.mLeft);
    Color color;
    color = mCurrentState.mFillColor;

    while (height--)
    {
      RasterLine(pColor, width, color);
      pColor += w;
    }
  }
  else
  {
    Color color;
    color = mCurrentState.mClearColor;
    uint count = (ToZero(mWidth) * ToZero(mHeight));
    Color* pScreen = mpImage;

    while (count--)
      *pScreen++ = color; 
  }
}

///////// RASTER COPY HELPERS:
#define FIXED_ACCURACY 8
#define INTERPOLATE(source, bias)  (((int)source[o] * ii - (int)source[o + bias] * i) >> (2 + 2 * FIXED_ACCURACY))

void nuiMemoryDrawContext::StretchRaster8Lum(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o;
  int incr = (size2 << FIXED_ACCURACY) / (size1);
  uint8 v;
  // FIXME
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);

    v = pSrc[o];
    *pDst++ = v;
    *pDst++ = v;
    *pDst++ = v;
    *(uint8*)pDst++ = 255;

    offset+=incr;
  }
}

void nuiMemoryDrawContext::StretchRaster8Alpha(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o;
  int incr = (size2 << FIXED_ACCURACY) / (size1);
  uint8 v;
  // FIXME
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);

    v = pSrc[o];
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = v;

    offset+=incr;
  }
}

void nuiMemoryDrawContext::InterpolateRaster8Alpha(char* pDst, char* pSrc, uint size1, uint size2)
{
  uint pos = 0, i , ii; 
  uint incr = (size2 << FIXED_ACCURACY) / (size1);
  uint o;
  if (size2 < size1)
    size2 = 0;

  uint mask, one;

  one = 1 - FIXED_ACCURACY;
  mask = one - 1;

  while (size1--)
  {
    o = pos >> (FIXED_ACCURACY);

    i = o & mask;
    ii = one - i;

    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = ((ii * (uint)((uint8*)pSrc)[o]) + (i * (uint)((uint8*)pSrc)[o+1])) >> FIXED_ACCURACY;

    pos+=incr;
  }
}

void nuiMemoryDrawContext::StretchRaster16(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o;
  int incr = (size2 << FIXED_ACCURACY) / (size1);

  char r,g,b;
  // FIXME
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY-1);

    uint16 pixel;
    pixel = (pSrc[o++]) << 8;
    pixel += pSrc[o++];
    b = (pixel >> 8) & 0xF8;
    g = (pixel >> 5) & 0xFC;
    r = (pixel<< 3);

    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    *pDst++ = 0;

    offset+=incr;
  }
}

void nuiMemoryDrawContext::StretchRaster24(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o;
  int incr = (size2 << FIXED_ACCURACY) / (size1);

  char r,g,b;
  while (size1--)
  {
    o = 3*(offset >> FIXED_ACCURACY);
    b = pSrc[o++];
    g = pSrc[o++];
    r = pSrc[o++];
    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    pDst++;

    offset += incr;
  }
}

void nuiMemoryDrawContext::StretchRaster32(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o;
  int incr = (size2 << FIXED_ACCURACY) / (size1);

#ifdef NUI_USE_BGRA  
  char r,g,b,a;
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);
    o *= 4;
    b = pSrc[o++];
    g = pSrc[o++];
    r = pSrc[o++];
    a = pSrc[o++];
    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    *pDst++ = a;

    offset += incr;
  }
#else
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);
    o *= 4;
    *pDst++ = pSrc[o++];
    *pDst++ = pSrc[o++];
    *pDst++ = pSrc[o++];
    *pDst++ = pSrc[o++];

    offset += incr;
  }
#endif // NUI_USE_RGBA
}

void nuiMemoryDrawContext::InterpolateRaster32(char* pDst, char* pSrc, uint size1, uint size2)
{
  int offset = 0,o, i, ii;
  int incr = (size2 << FIXED_ACCURACY) / (size1);

#ifdef NUI_USE_BGRA 
  char r,g,b,a;
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);
    o *= 4;

    i = offset & (FIXED_ACCURACY-1);
    ii = (FIXED_ACCURACY-1) - i;

    b = INTERPOLATE(pSrc, 4); o++;
    g = INTERPOLATE(pSrc, 4); o++;
    r = INTERPOLATE(pSrc, 4); o++;
    a = INTERPOLATE(pSrc, 4); o++;
    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    *pDst++ = a;

    offset += incr;
  }
#else
  while (size1--)
  {
    o = offset >> (FIXED_ACCURACY);
    o *= 4;

    i = offset & (FIXED_ACCURACY-1);
    ii = (FIXED_ACCURACY-1) - i;

    *pDst++ = INTERPOLATE(pSrc, 4); o++;
    *pDst++ = INTERPOLATE(pSrc, 4); o++;
    *pDst++ = INTERPOLATE(pSrc, 4); o++;
    *pDst++ = INTERPOLATE(pSrc, 4); o++;


    offset += incr;
  }
#endif // NUI_USE_RGBA
}

void nuiMemoryDrawContext::CopyRaster8Lum(char* pDst, char* pSrc, uint size1, uint size2)
{
  // FIXME
  if (size1 != size2)
  {
    StretchRaster8Lum(pDst, pSrc, size1, size2);
    return;
  }

  uint8 v;
  while (size1--)
  {
    v = *pSrc++;
    *pDst++ = v;
    *pDst++ = v;
    *pDst++ = v;
    *(uint8*)pDst++ = 255;
  }
}

void nuiMemoryDrawContext::CopyRaster8Alpha(char* pDst, char* pSrc, uint size1, uint size2)
{
  // FIXME
  if (size1 != size2)
  {
    StretchRaster8Alpha(pDst, pSrc, size1, size2);
    //    InterpolateRaster8Alpha(pDst, pSrc, size1, size2);
    return;
  }

  uint8 v;
  while (size1--)
  {
    v = *pSrc++;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = 255;
    *(uint8*)pDst++ = v;
  }
}


void nuiMemoryDrawContext::CopyRaster16(char* pDst, char* pSrc, uint size1, uint size2)
{
  // FIXME
  if (size1 != size2)
  {
    StretchRaster16(pDst, pSrc, size1, size2);
    return;
  }

  char r,g,b;
  while (size1--)
  {
    uint16 pixel;
    pixel = (*pSrc++) << 8;
    pixel += *pSrc++;
    b = (pixel >> 8) & 0xF8;
    g = (pixel >> 5) & 0xFC;
    r = (pixel<< 3);

    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    *pDst++ = 0;
  }
}

void nuiMemoryDrawContext::CopyRaster24(char* pDst, char* pSrc, uint size1, uint size2)
{
  if (size1 != size2)
  {
    StretchRaster24(pDst, pSrc, size1, size2);
    return;
  }

  char r,g,b;
  while (size1--)
  {
    b = *pSrc++;
    g = *pSrc++;
    r = *pSrc++;
    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    pDst++;
  }
}

void nuiMemoryDrawContext::CopyRaster32(char* pDst, char* pSrc, uint size1, uint size2)
{
  if (size1 != size2)
  {
    StretchRaster32(pDst, pSrc, size1, size2);
    return;
  }

#ifdef NUI_USE_BGRA
  while (size1--)
  {
    char r,g,b,a;
    b = *pSrc++;
    g = *pSrc++;
    r = *pSrc++;
    a = *pSrc++;
    *pDst++ = r;
    *pDst++ = g;
    *pDst++ = b;
    *pDst++ = a;

  }
#else
  memcpy(pDst, pSrc, size1*4);
#endif // NUI_USE_RGBA

}

void nuiMemoryDrawContext::RasterBlend( nuiMemoryDrawContext::Color* pDst, nuiMemoryDrawContext::Color* pSrc, uint count)
{
  while (count)
  {
    while (count && pSrc->mA != 255)
    {
      pDst->BlendWith(*pSrc);
      pDst++;
      pSrc++;
      count--;
    }

    while (count && pSrc->mA == 255)
    {
      *pDst++ = *pSrc++;
      count--;
    }

  }
}

void nuiMemoryDrawContext::RasterBlend( nuiMemoryDrawContext::Color* pDst, const nuiMemoryDrawContext::Color& rSrc, uint count)
{
  while (count--)
  {
    pDst->BlendWith(rSrc);
    pDst++;
  }
}

void nuiMemoryDrawContext::RasterModulate( nuiMemoryDrawContext::Color* pDst, const nuiMemoryDrawContext::Color& rSrc, uint count)
{
  while (count--)
  {
    pDst->ModulateWith(rSrc);
    pDst++;
  }
}


void nuiMemoryDrawContext::DrawImage(const nuiRect& rDest, const nuiRect& rSource)
{
  uint x,y,w,h;
  uint ix,iy,iw,ih;
  nuiRect Dest, Source, Temp;
  nglVectorf p;

  // Apply the current transformations:
  p = mMatrixStack.top() * nglVectorf(rDest.mLeft,rDest.mTop,0);
  Temp.mLeft = p[0];
  Temp.mTop = p[1];

  p = mMatrixStack.top() * nglVectorf(rDest.mRight,rDest.mBottom,0);
  Temp.mRight = p[0];
  Temp.mBottom = p[1];

  Dest.Intersect(Temp, mTransformedClippingRect);
  nuiTexture* pTexture = mCurrentState.mpTexture;
  if (!pTexture) 
    return;
  nglImage* pImage = pTexture->GetImage();
  if (!pImage)
    return;

  nuiSize imHeight = (nuiSize)pImage->GetHeight();
  Source = nuiRect(rSource.mLeft, 
    imHeight - rSource.mBottom, 
    rSource.GetWidth(), rSource.GetHeight());

  Source.Intersect( Source, nuiRect(0,0,pImage->GetWidth(),(int32)imHeight) );

  //  NGL_OUT("[%s] x: %f / y: %f\n",pTexture->GetProperty(_T("Source")).GetChars(),stretchx,stretchy); 

  x = ToZero(Dest.mLeft);
  y = ToZero(Dest.mTop);
  w = ToZero(Dest.GetWidth());
  h = ToZero(Dest.GetHeight());

  if (Dest.mLeft > Temp.mLeft)
  {
    float d = (Dest.mLeft - Temp.mLeft) / Temp.GetWidth();
    Source.mLeft += d * rSource.GetWidth();
  }

  if (Dest.mRight < Temp.mRight)
  {
    float d = (Temp.mRight - Dest.mRight) / Temp.GetWidth();
    Source.mRight -= d * rSource.GetWidth();
  }

  if (Dest.mTop > Temp.mTop)
  {
    float d = (Dest.mTop - Temp.mTop) / Temp.GetHeight();
    Source.mBottom -= d * rSource.GetHeight();
  }

  if (Dest.mBottom < Temp.mBottom)
  {
    float d = (Temp.mBottom - Dest.mBottom) / Temp.GetHeight();
    Source.mTop += d * rSource.GetHeight();
  }

  if (Source.mBottom < Source.mTop)
    Source.mBottom = Source.mTop;

  if (Source.mRight < Source.mLeft)
    Source.mRight = Source.mLeft;

  ix = ToNearest(Source.mLeft);
  iy = pImage->GetHeight() - (ToNearest(Source.mTop) + ToNearest(Source.GetHeight()));
  iw = ToNearest(Source.GetWidth());
  ih = ToNearest(Source.GetHeight());

  if (w && h && iw && ih)
  {
    CopyRasterFunc pFunc;
    uint bytes = 0;

    switch (pImage->GetBitDepth())
    {
    case 8:
      switch (pImage->GetPixelFormat())
      {
      case eImagePixelLum:
        pFunc = &nuiMemoryDrawContext::CopyRaster8Lum;
        break;
      case eImagePixelAlpha:
        pFunc = &nuiMemoryDrawContext::CopyRaster8Alpha;
        break;
      default:
        NGL_ASSERT(0); // Not implemented.
        break;
      }
      bytes = 1;
      break;
    case 16:
      pFunc = &nuiMemoryDrawContext::CopyRaster16;
      bytes = 2;
      break;
    case 24:
      pFunc = &nuiMemoryDrawContext::CopyRaster24;
      bytes = 3;
      break;
    case 32:
      pFunc = &nuiMemoryDrawContext::CopyRaster32;
      bytes = 4;
      break;
    default:
      return;
    }

    uint offset = x + y * (ToZero(mWidth));
    uint ioffset = (ix + iy * (pImage->GetWidth()) * bytes);

    char* pBuffer = (char*)pImage->GetBuffer();

    int foffset,o;
    int fincr =  (ih << FIXED_ACCURACY) / (h);
    int riw = pImage->GetWidth();
    int incr = ToZero(mWidth);
    foffset = iy << FIXED_ACCURACY; 

    GLuint EnvMode = pTexture->GetEnvMode();

    Color FillColor;
    FillColor = mCurrentState.mFillColor;

    if (mCurrentState.mBlending)
    {
      while (h--)
      {
        o = foffset >> FIXED_ACCURACY;
        ioffset = ((ix + o * riw) * bytes);
        (this->*pFunc)((char*)(mpLineBuffer), pBuffer + ioffset, w,iw);

        if (EnvMode == GL_BLEND)
          RasterBlend(mpLineBuffer, FillColor, w);
        else if (EnvMode == GL_MODULATE)
          RasterModulate(mpLineBuffer, FillColor, w);

        RasterBlend( mpImage + offset, mpLineBuffer, w);

        offset+= incr;

        foffset += fincr;
        if (foffset < 0)
          foffset = 0;
      }
    }
    else
    {
      while (h--)
      {
        o = foffset >> FIXED_ACCURACY;
        ioffset = ((ix + o * riw) * bytes);
        (this->*pFunc)((char*)(mpImage + offset), pBuffer + ioffset, w,iw);
        offset+= incr;

        foffset += fincr;
      }
    }
  }
}

void nuiMemoryDrawContext::DrawImageQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, const nuiRect& rSource)
{
  // #FIXME Not Implmented
}

void nuiMemoryDrawContext::DrawShade (const nuiRect& rSourceRect, const nuiRect& rShadeRect)
{
}

/****************************************************************************
*
* MMX and SSE Optimization (MSVC asm inlines only)
*
****************************************************************************/

#ifdef NUI_USE_MMX

class nuiMemoryDrawContext_MMX : public nuiMemoryDrawContext
{
public:
  nuiMemoryDrawContext_MMX(const nuiRect& rRect);
  virtual ~nuiMemoryDrawContext_MMX();

  virtual void RasterLine(Color* pScreen, uint size, const Color& rColor);
  virtual void RasterLineA(Color* pScreen, uint count, const Color& rColor);
  virtual void Clear();
  virtual void StretchRaster32(char* pDst, char* pSrc, uint size1, uint size2);
  virtual void RasterBlend( nuiMemoryDrawContext::Color* pDst, nuiMemoryDrawContext::Color* pSrc, uint count);

};

nuiMemoryDrawContext_MMX::nuiMemoryDrawContext_MMX(const nuiRect& rRect)
: nuiMemoryDrawContext(rRect)
{
}

nuiMemoryDrawContext_MMX::~nuiMemoryDrawContext_MMX()
{
}

void nuiMemoryDrawContext_MMX::RasterLine(Color* pScreen, uint count, const Color& rColor)
{
  uint col = rColor.GetInt();
  __asm 
  {
    mov ecx,count
      shr ecx,2
      test ecx,ecx
      jz done
      movq mm0, col
      punpckldq mm0, mm0
      movq mm1, mm0
      mov edi, pScreen

loopwrite:

    movq 0[edi], mm0
      movq 8[edi], mm1

      add edi, 16
      dec ecx
      jnz loopwrite

      emms
      mov pScreen,edi
done:
    and count,3 
  }

  while (count--)
    *pScreen++ = rColor; 
}



void nuiMemoryDrawContext_MMX::RasterLineA(Color* pScreen, uint count, const Color& rColor)
{
  int color = rColor.GetInt();

  // Every optim goes by pair so lets begin by getting the pixel count even:
  if (count & 1)
  {
    pScreen->BlendWith(rColor);
    pScreen++;
    count--;
  }


  if (rColor.mA < 129 && rColor.mA > 126) // optimize things a bit, ok, I cheat... 
  {
    __asm
    {
      //
      // Initialize the counter and skip 
      // if the latter is equal to zero.
      //
      mov     ecx,count
        shr     ecx,1
        test    ecx,ecx  
        jz      skip

        //
        // Load the frame buffer pointers into the registers.
        //
        mov     edi, pScreen

        pxor    mm4, mm4        // Clear mm4 so we can unpack easily.

        mov     eax, color // Load the source color

        movd      mm1, eax     // Load the source color in mmx.
        punpcklbw  mm1, mm4     // Unpack the first source pixel.

havealpha:
      //
      // Alpha blend two target and two source pixels.
      //
      movq      mm5, [edi]      // Load the dest pixel.
      movq      mm0, mm5
        punpcklbw  mm0, mm4        // Unpack the first dest copy.

        paddusw   mm0, mm1        // d + s

        psrlw     mm0, 1          // (d + s)/2

        packuswb  mm0, mm4        // repack the pixel
        movq      mm6, mm0

        // Second pixel:
        psrlq     mm5, 32

        punpcklbw  mm5, mm4        // Unpack the dest copy.

        paddusw   mm5, mm1        // d + s
        psrlw     mm5, 1          // (d + s) / 2

        packuswb  mm5, mm4        // repack the pixel
        psllq     mm5, 32

        por       mm6,mm5

        movq      [edi], mm6      // Write back the new dest value.

        //
        // Advance to the next pixel.
        //
        add     edi, 8

        //
        // Loop again or break.
        //
        dec     ecx
        jnz     havealpha

        emms

skip:
    }

    return;
  }

  //    pScreen->BlendWith(rColor);
  //    pScreen++;

  __asm
  {
    //
    // Initialize the counter and skip 
    // if the latter is equal to zero.
    //
    mov     ecx,count
      shr     ecx,1
      test    ecx,ecx  
      jz      blend_skip

      //
      // Load the frame buffer pointers into the registers.
      //
      mov     edi, pScreen

      pxor      mm4, mm4        // Clear mm4 so we can unpack easily.

      mov     eax, color // Load the source color
      mov     ebx, eax   // save it in ebx
      shr     eax, 24    // get the alpha value in al

      //    movd    mm5, eax        // mm5 = 0000 0000 -- 00aa 00aa
      //    pinsrw  mm5, eax,1
      //    pinsrw  mm5, eax,2
      pinsrw  mm5, eax,3      // mm5 = alpha alpha alpha alpha
      pinsrw  mm5, eax,2      // mm5 = alpha alpha alpha alpha
      pinsrw  mm5, eax,1      // mm5 = alpha alpha alpha alpha
      pinsrw  mm5, eax,0      // mm5 = alpha alpha alpha alpha

      pcmpeqw   mm3, mm3      // mm3 = 0xFFFF 0xFFFF 0xFFFF 0xFFFF
      psrlw     mm3, 15       // mm3 =   1   1   1   1 
      psllw     mm3, 8        // mm3 = 256 256 256 256 
      psubusw   mm3, mm5      // mm3 = 256 - alpha 

      movd      mm1, ebx      // Load the source color in mmx.
      punpcklbw  mm1, mm4      // Unpack the first source pixel.
      pmullw    mm1, mm5      // s * alpha

blend_havealpha:
    //
    // Alpha blend two target and two source pixels.
    //
    movq      mm5, [edi]    // Load the dest pixel.
    movq      mm0, mm5
      punpcklbw  mm0, mm4      // Unpack the first dest copy.

      pmullw    mm0, mm3      // d * (256-alpha)
      paddusw   mm0, mm1      // d * (256-alpha) + s * alpha

      psrlw     mm0, 8        // (d * (256-alpha) + s * alpha) /256

      packuswb  mm0, mm4      // repack the pixel
      movq      mm6, mm0

      // Second pixel:
      psrlq     mm5, 32
      movq      mm0, mm5

      punpcklbw  mm0, mm4      // Unpack the dest copy.

      pmullw    mm0, mm3      // d * (256-alpha)
      paddusw   mm0, mm1      // d * (256-alpha) + s * alpha

      psrlw     mm0, 8        // (d * (256-alpha) + s * alpha) /256

      packuswb  mm0, mm4      // repack the pixel
      psllq     mm0, 32

      por       mm6,mm0

      movq      [edi], mm6    // Write back the new dest value.

      //
      // Advance to the next pixel.
      //
      add     edi, 8

      //
      // Loop again or break.
      //
      dec     ecx
      jnz     blend_havealpha

      emms

blend_skip:
  }
}

void nuiMemoryDrawContext_MMX::Clear()
{
  if (mClippingRect.mEnabled)
  {
    uint height = ToZero(mTransformedClippingRect.GetHeight());
    uint width  = ToZero(mTransformedClippingRect.GetWidth());

    uint w = ToZero(mWidth);

    Color* pColor = mpImage + ToZero(mTransformedClippingRect.mTop) * w + ToZero(mTransformedClippingRect.mLeft);
    Color color;
    color = mCurrentState.mFillColor;

    while (height--)
    {
      RasterLine(pColor, width, color);
      pColor += w;
    }
  }
  else
  {
    Color color;
    color = mCurrentState.mClearColor;
    uint count = (ToZero(mWidth) * ToZero(mHeight));
    Color* pScreen = mpImage;
    uint col = color.GetInt();
    __asm 
    {
      movq mm0, col
        punpckldq mm0, mm0
        mov edi, pScreen

loopwrite:

      movq 0[edi], mm0
        movq 8[edi], mm0

        add edi, 16
        sub count, 4
        jg loopwrite

        emms
        mov pScreen,edi
    }

    while (count--)
      *pScreen++ = color; 
  }
}

void nuiMemoryDrawContext_MMX::StretchRaster32(char* pDst, char* pSrc, uint size1, uint size2)
{
  __asm
  {
    mov ecx, size1  // Number of destination pixels

      mov esi, pSrc  // Load the source pointer
      mov edi, pDst  // Load the destination pointer

      mov eax, size2             // eax = size2
      xor edx, edx                 // prepare division
      shl eax, FIXED_ACCURACY    // eax = size2 <<  FIXED_ACCURACY
      div ecx                    // eax = (size2 << FIXED_ACCURACY) / (size1)

      xor edx, edx // edx = offset
      sub edi,4 // For scheduling optims

do_stretch:
    mov ebx, edx               // ebx = o = offset
      add edi,4
      shr ebx, FIXED_ACCURACY-2  // ebx = (offset * 4) >> FIXED_ACCURACY
      add edx,eax                // offset += incr
      and ebx, 0FFffFFfch                // ebx = (offset >> FIXED_ACCURACY) * 4

      mov  ebx,[esi+ebx]          // Read the texel
      xchg  bh,bl                 // Transform bgra in rgba (snirf :'( )
        ror  ebx,16
        xchg bh,bl
        ror  ebx,8

        mov [edi],ebx


        dec ecx
        jnz do_stretch

        emms
  }

}

void nuiMemoryDrawContext_MMX::RasterBlend( nuiMemoryDrawContext::Color* pDst, nuiMemoryDrawContext::Color* pSrc, uint count)
{
  __asm
  {
    //
    // Initialize the counter and skip 
    // if the latter is equal to zero.
    //
    mov     ecx, count
      test    ecx, ecx  
      jz      skip

      //
      // Load the frame buffer pointers into the registers.
      //
      mov     edi, pDst    
      mov     esi, pSrc

      pxor      mm4, mm4        // Clear mm4 so we can unpack easily.

do_blend:
    mov     eax, [esi] // Load the next source pixel
    mov     ebx, eax   // save it in ebx
      shr     eax, 24    // get the alpha value in al

      test    al, al    
      je      nextpixel // if alpha == O we don't have to blend this pixel.

      cmp     al, 0ffh  
      jne     havealpha // if alpha != 255 we have to blend

      mov     [edi], ebx     // alpha == 255, we can safely overwrite the current target pixel and get to the next pixel.
      //
      // Advance to the next pixel.
      //
      add     esi, 4
      add     edi, 4

      //
      // Loop again or break.
      //
      dec     ecx
      jnz     do_blend
      jmp     stopline

havealpha:
    //
    // Alpha blend two target and two source pixels.
    //
    movd    mm5, eax   // mm5 = 0000 0000 -- 00aa 00aa
      pinsrw  mm5, eax,1
      pinsrw  mm5, eax,2
      pinsrw  mm5, eax,3

      movd      mm6, [edi]      // Load the dest pixel.
      movq      mm0, mm6      // Load the dest pixel.
        movd      mm7, ebx      // Load the source pixel.
        movq      mm1, mm7      // Load the dest pixel.

        punpcklbw  mm0, mm4        // Unpack the first dest copy.
        punpcklbw  mm1, mm4        // Unpack the first source pixel.

        pcmpeqw   mm3, mm3        // mm3 = 0xFFFF 0xFFFF 0xFFFF 0xFFFF
        psrlw     mm3, 15         // mm3 =   1   1   1   1 
        psllw     mm3, 8          // mm3 = 256 256 256 256 
        psubusw   mm3, mm5         // mm3 = 256 - alpha 

        pmullw    mm0, mm3         // d * (256-alpha)
        pmullw    mm1, mm5         // s * alpha
        paddusw   mm0, mm1         // d * (256-alpha) + s * alpha

        psrlw     mm0, 8           // (d * (256-alpha) + s * alpha) /256

        packuswb  mm0, mm4         // repack the pixel

        movd      [edi], mm0      // Write back the new dest value.

nextpixel:
      //
      // Advance to the next pixel.
      //
      add     edi, 4
        add     esi, 4

        //
        // Loop again or break.
        //
        dec     ecx
        jnz     do_blend

stopline:
      //
      // Write back the frame buffer pointers and clean up.
      //
      mov     pDst, edi  
        mov     pSrc, esi
        emms

skip:
  }

}

bool CheckAvailableArch(bool NeedMMX, bool NeedSSE)
{
  bool found_MMX = false;
  bool found_SSE = false;
  __asm
  {
    pushfd
      pop eax // get EFLAGS into eax
      mov ebx,eax // keep a copy
      xor eax,0x200000 
      // toggle CPUID bit

      push eax
      popfd // set new EFLAGS
      pushfd
      pop eax // EFLAGS back into eax

      xor eax,ebx 
      // have we changed the ID bit?

      je DONE
      // No, no CPUID instruction

      // we could toggle the 
      // ID bit so CPUID is present
      mov eax,1

      cpuid // get processor features
      test edx,1<<23 // check the MMX bit
      jz DONE_MMX
      mov found_MMX,1
DONE_MMX:
    test edx,1<<25 // check the SSE bit
      jz DONE
      mov found_SSE,1 
DONE:
  }

  if (found_MMX)
    NGL_OUT("x86 MMX Extensions found\n");

  if (found_SSE)
    NGL_OUT("x86 SSE Extensions found\n");

  return  (NeedMMX?found_MMX:true) && (NeedSSE?found_SSE:true);
}

#ifdef NUI_USE_SSE
class nuiMemoryDrawContext_SSE : public nuiMemoryDrawContext_MMX
{
public:
  nuiMemoryDrawContext_SSE(const nuiRect& rRect);
  virtual ~nuiMemoryDrawContext_SSE();

  virtual void RasterLine(Color* pScreen, uint count, const Color& rColor);
  virtual void Clear();
};

nuiMemoryDrawContext_SSE::nuiMemoryDrawContext_SSE(const nuiRect& rRect)
: nuiMemoryDrawContext_MMX(rRect)
{
}

nuiMemoryDrawContext_SSE::~nuiMemoryDrawContext_SSE()
{
}

void nuiMemoryDrawContext_SSE::RasterLine(Color* pScreen, uint count, const Color& rColor)
{
  uint col = rColor.GetInt();
  __asm 
  {
    mov ecx,count
      shr ecx,2
      test ecx,ecx
      jz done
      movq mm0, col
      punpckldq mm0, mm0
      mov edi, pScreen

loopwrite:

    movntq 0[edi], mm0
      movntq 8[edi], mm0

      add edi, 16
      dec ecx
      jnz loopwrite

      emms
      mov pScreen,edi
done:
    and count,3 
  }

  while (count--)
    *pScreen++ = rColor; 
}

void nuiMemoryDrawContext_SSE::Clear()
{
  if (mClippingRect.mEnabled)
  {
    uint height = ToZero(mTransformedClippingRect.GetHeight());
    uint width  = ToZero(mTransformedClippingRect.GetWidth());

    uint w = ToZero(mWidth);

    Color* pColor = mpImage + ToZero(mTransformedClippingRect.mTop) * w + ToZero(mTransformedClippingRect.mLeft);
    Color color;
    color = mCurrentState.mFillColor;

    while (height--)
    {
      RasterLine(pColor, width, color);
      pColor += w;
    }
  }
  else
  {
    Color color;
    color = mCurrentState.mClearColor;
    uint count = (ToZero(mWidth) * ToZero(mHeight));
    Color* pScreen = mpImage;
    uint col = color.GetInt();
    __asm 
    {
      movq mm0, col
        mov ecx,count
        shr ecx,2
        punpckldq mm0, mm0
        mov edi, pScreen

loopwrite:

      movntq 0[edi], mm0
        movntq 8[edi], mm0

        add edi, 16
        dec ecx
        jnz loopwrite

        and count,3
        emms
        mov pScreen,edi
    }

    while (count--)
      *pScreen++ = color; 
  }
}

#endif // NUI_USE_SSE

#endif // NUI_USE_MMX


/****************************************************************************
*
* Altivec Optimization
*
****************************************************************************/

#ifdef NUI_USE_ALTIVEC

class nuiMemoryDrawContext_Altivec : public nuiMemoryDrawContext
{
public:
  nuiMemoryDrawContext_Altivec(const nuiRect& rRect);
  virtual ~nuiMemoryDrawContext_Altivec();

  virtual void RasterLine(Color* pScreen, uint size, const Color& rColor);
  virtual void RasterLineA(Color* pScreen, uint count, const Color& rColor);
  virtual void Clear();
  virtual void RasterBlend( nuiMemoryDrawContext::Color* pDst, nuiMemoryDrawContext::Color* pSrc, uint count);

};

nuiMemoryDrawContext_Altivec::nuiMemoryDrawContext_Altivec(const nuiRect& rRect)
: nuiMemoryDrawContext(rRect)
{
}

nuiMemoryDrawContext_Altivec::~nuiMemoryDrawContext_Altivec()
{
}

void nuiMemoryDrawContext_Altivec::RasterLine(Color* pScreen, uint count, const Color& rColor)
{
  Color color;
  color = rColor;
  int cnt = count;
  vector signed int c;
  ((int*)&c)[0] = ((int*)&c)[1] = ((int*)&c)[2] = ((int*)&c)[3] = (color.GetInt());
  vector signed int* pDst; 

  while (cnt && ((unsigned long)pScreen & 0x0f))
  {
    *pScreen++ = color;
    cnt--;
  }

  pDst = (vector signed int*) pScreen;
  while (cnt > 15)
  {
    pDst[0] = c;
    pDst[1] = c;
    pDst[2] = c;
    pDst[3] = c;

    pDst += 4;
    cnt -= 16;
  }

  while (cnt > 7)
  {
    pDst[0] = c;
    pDst[1] = c;

    pDst += 2;
    cnt -= 8;
  }

  while (cnt > 3)
  {
    pDst[0] = c;

    pDst++;
    cnt -= 4;
  }

  pScreen = (Color*)pDst;
  while (cnt--)
    *pScreen++ = color;
}

inline void DoTransparent( vector unsigned char& dest, vector unsigned char& alpha2 , vector unsigned short& temp3, vector unsigned short& temp4, vector unsigned char mergePermute )
{
  vector unsigned short temp1, temp2;

  //do multiplications
  temp1 = vec_mule( dest, alpha2 );
  temp2 = vec_mulo( dest, alpha2 );

  //temp2 holds red and blue dest channels
  //temp1 holds alpha and green dest channels
  //temp4 holds red and blue src channels
  //temp3 holds alpha and green src channels

  //Do the addition
  temp1 = vec_add( temp1, temp3 );
  temp2 = vec_add( temp2, temp4 );

  //temp2 now holds the red and blue channel results
  //temp1 now holds the alpha and green channel results

  //merge the channels back together
  dest = (vector unsigned char) vec_perm( temp1, temp2, mergePermute );
}


void nuiMemoryDrawContext_Altivec::RasterLineA(Color* pScreen, uint count, const Color& rColor)
{
  Color color;
  color = rColor;
  int cnt = count;

  vector signed int c;
  ((int*)&c)[0] = ((int*)&c)[1] = ((int*)&c)[2] = ((int*)&c)[3] = (color.GetInt());
  vector unsigned char alpha, alpha2, alphaPermute, mergePermute;

  //Prepare some permute constants for later
  alphaPermute = mergePermute = vec_lvsl( 0, (int*) NULL );
  mergePermute = vec_andc( mergePermute, vec_splat_u8(1) );
  alphaPermute = vec_andc( alphaPermute, vec_splat_u8(3) );
  alphaPermute = vec_add( alphaPermute, vec_splat_u8(3) );
  mergePermute = (vector unsigned char) vec_sub( (vector signed short) mergePermute, vec_splat_s16(-16) );

  //Extract the src alpha channel out and splat across all four channels
  alpha = vec_perm( (vector unsigned char) c, (vector unsigned char)c, alphaPermute );

  //alpha2 = 255-alpha
  alpha2 = vec_sub( (vector unsigned char) vec_splat_s8(-1), alpha );

  vector unsigned short temp3, temp4;
  temp3 = vec_mule( (vector unsigned char) c, alpha );
  temp4 = vec_mulo( (vector unsigned char) c, alpha );

  vector unsigned char* pDst; 

  while (cnt && ((unsigned long)pScreen & 0x0f))
  {
    (*pScreen).BlendWith(color);
    pScreen++;
    cnt--;
  }

  pDst = (vector unsigned char*) pScreen;
  while (cnt > 3)
  {
    DoTransparent(*pDst++, alpha2, temp3, temp4, mergePermute);

    cnt -= 4;
  }

  pScreen = (Color*)pDst;
  while (cnt--)
  {
    (*pScreen).BlendWith(color);
    pScreen++;
  }
}

void nuiMemoryDrawContext_Altivec::Clear()
{
  if (mClippingRect.mEnabled)
  {
    uint height = ToZero(mTransformedClippingRect.GetHeight());
    uint width  = ToZero(mTransformedClippingRect.GetWidth());

    uint w = ToZero(mWidth);

    Color* pColor = mpImage + ToZero(mTransformedClippingRect.mTop) * w + ToZero(mTransformedClippingRect.mLeft);
    Color color;
    color = mCurrentState.mFillColor;

    while (height--)
    {
      RasterLine(pColor, width, color);
      pColor += w;
    }
  }
  else
  {
    Color color;
    color = mCurrentState.mClearColor;
    uint count = (ToZero(mWidth) * ToZero(mHeight));

    RasterLine(mpImage, count, color);  
  }
}

inline void DoTransparent2( vector unsigned char& dest, vector unsigned char& source, vector unsigned char& mergePermute  , vector unsigned char& alphaPermute  )
{
  vector unsigned short temp1, temp2, temp3, temp4;
  vector unsigned char alpha, alpha2;

  //Extract the src alpha channel out and splat across all four channels
  alpha = vec_perm( (vector unsigned char) source, (vector unsigned char)source, alphaPermute );

  //alpha2 = 255-alpha
  alpha2 = vec_sub( (vector unsigned char) vec_splat_s8(-1), alpha );


  //do multiplications
  temp1 = vec_mule( dest, alpha2 );
  temp2 = vec_mulo( dest, alpha2 );
  temp3 = vec_mule( source, alpha );
  temp4 = vec_mulo( source, alpha );

  //temp2 holds red and blue dest channels
  //temp1 holds alpha and green dest channels
  //temp4 holds red and blue src channels
  //temp3 holds alpha and green src channels

  //Do the addition
  temp1 = vec_add( temp1, temp3 );
  temp2 = vec_add( temp2, temp4 );

  //temp2 now holds the red and blue channel results
  //temp1 now holds the alpha and green channel results

  //merge the channels back together
  dest = (vector unsigned char) vec_perm( temp1, temp2, mergePermute );
}



void nuiMemoryDrawContext_Altivec::RasterBlend( nuiMemoryDrawContext::Color* pDst, nuiMemoryDrawContext::Color* pSrc, uint count)
{
  vector unsigned char alphaPermute, mergePermute;
  int cnt = count; 

  //Prepare some permute constants for later
  alphaPermute = mergePermute = vec_lvsl( 0, (int*) NULL );
  mergePermute = vec_andc( mergePermute, vec_splat_u8(1) );
  alphaPermute = vec_andc( alphaPermute, vec_splat_u8(3) );
  alphaPermute = vec_add( alphaPermute, vec_splat_u8(3) );
  mergePermute = (vector unsigned char) vec_sub( (vector signed short) mergePermute, vec_splat_s16(-16) );

  // Start aligned:
  while (cnt && ((unsigned long)pDst & 0x0f))
  {
    pDst->BlendWith(*pSrc);
    pDst++;
    pSrc++;
    cnt--;
  }

  // Altivec the feasible stuff:
  vector unsigned char* pSource      = (vector unsigned char*) pSrc;
  vector unsigned char* pDestination = (vector unsigned char*) pDst;
  vector unsigned char alignPermute = vec_lvsl(0, (int*)pSrc);

  while (cnt > 3)
  {
    vector unsigned char col = vec_perm(pSource[0], pSource[1], alignPermute);
    pSource++;
    DoTransparent2(*pDestination++, col, mergePermute, alphaPermute);

    cnt -= 4;
  }

  pSrc = (Color*)pSource;
  pDst = (Color*)pDestination;

  // Finish unaligned:
  while (cnt)
  {
    while (cnt && pSrc->mA != 255)
    {
      pDst->BlendWith(*pSrc);
      pDst++;
      pSrc++;
      cnt--;
    }

    while (cnt && pSrc->mA == 255)
    {
      *pDst++ = *pSrc++;
      cnt--;
    }

  }
}

bool CheckAvailableAltivec()
{
  long processorAttributes;
  bool result = false;

  OSErr err = Gestalt(gestaltPowerPCProcessorFeatures, &processorAttributes);
  if (err == noErr)
    result = (1 << gestaltPowerPCHasVectorInstructions) & processorAttributes;
  return result;
}



#endif

/****************************************************************************
*
* DrawContext Factory
*
****************************************************************************/

nuiDrawContext *nuiDrawContext::CreateDrawContext(const nuiRect& rRect, nuiRenderer Renderer, nglContext* pContext)
{
  switch (Renderer)
  {
  case eSoftware:
#ifdef NUI_USE_MMX
#ifdef NUI_USE_SSE
    if (CheckAvailableArch(true,true))
    {
      return new nuiMemoryDrawContext_SSE(rRect);
    }
    else 
#else // NUI_USE_SSE 
    if (CheckAvailableArch(true,false))
    {
      return new nuiMemoryDrawContext_MMX(rRect);
    }
#endif // NUI_USE_SSE
#endif // NUI_USE_MMX
#ifdef NUI_USE_ALTIVEC
    if (CheckAvailableAltivec())
    {
      return new nuiMemoryDrawContext_Altivec(rRect);
    }
#endif // NUI_USE_ALTIVEC
    return new nuiMemoryDrawContext(rRect);
    break;
#ifndef __NUI_NOGL__
  case eOpenGL:
  default:
    return new nuiGLPainter(pContext, rRect);
    break;
#else
  default:
    return NULL;
#endif //   #ifndef __NUI_NOGL__
  }
}

#endif

nuiDrawContext *nuiDrawContext::CreateDrawContext(const nuiRect& rRect, nuiRenderer Renderer, nglContext* pContext)
{
  nuiDrawContext* pC = new nuiDrawContext(rRect);
  pC->SetPainter(new nuiGLPainter(pContext, rRect));
  return pC;
}

