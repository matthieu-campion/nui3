/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"

#include "nuiVertex.h"

inline uint32 nuiLERPColor(uint32 a, uint32 b, uint32 x0, uint32 x1)
{
  uint32 colA0 = ((a & 0xff00ff00) >> 8) * x0;
  uint32 colA1 = (a & 0x00ff00ff) * x0;
  uint32 colB0 = ((b & 0xff00ff00) >> 8) * x1;
  uint32 colB1 = (b & 0x00ff00ff) * x1;
  
  return ((colA0 + colB0) & 0xff00ff00) | (((colA1 + colB1) & 0xff00ff00) >> 8);
}

template <int bits_per_pixel> class Screen
{
public:
  Screen()
  {
    mWidth = 0;
    mHeight = 0;
    mStride = 0;
    mpBuffer = NULL;
    mManage = false;
  }
  
  Screen(uint32 width, uint32 height, bool Manage, void* pBuffer = NULL, uint32 stride = 0)
  {
    mWidth = width;
    mHeight = height;
    mStride = stride;
    if (!mStride)
      mStride = mWidth * ((bits_per_pixel + 1) / 8);
    
    mpBuffer = (uint8*)pBuffer;
    mManage = Manage;
    
    if (!mpBuffer)
      Allocate();
  }
  
  ~Screen()
  {
    if (mManage)
      delete mpBuffer;
  }
  
  uint8* GetBuffer()
  {
    return mpBuffer;
  }

  const uint8* GetBuffer() const
  {
    return mpBuffer;
  }
  
  uint8* GetLine(uint32 line)
  {
    return mpBuffer + line * mStride;
  }
  
  const uint8* GetLine(uint32 line) const
  {
    return mpBuffer + line * mStride;
  }
  
  uint8* GetPixel(uint32 x, uint32 y)
  {
    return GetLine(y) + x * ((bits_per_pixel + 1) / 8);
  }
  
  const uint8* GetPixel(uint32 x, uint32 y) const
  {
    return GetLine(y) + x * ((bits_per_pixel + 1) / 8);
  }
  
  uint32 GetWidth() const
  {
    return mWidth;
  }
  
  uint32 GetHeight() const
  {
    return mHeight;
  }
  
  uint32 GetStride() const
  {
    return mStride;
  }
  
private:
  uint32 mWidth;
  uint32 mHeight;
  uint32 mStride;
  uint8* mpBuffer;
  bool mManage;
  
  void Release()
  {
    if (mManage)
      delete mpBuffer;
    mpBuffer = NULL;
  }
  
  void Allocate()
  {
    Release();
    mpBuffer = new uint8[((bits_per_pixel + 1) / 8) * mStride];
    mManage = true;
  }  
};

class vertex
{
public:
  vertex(float _x, float _y, float _r = 0, float _g = 0, float _b = 0, float _a = 1, float _u = 0, float _v = 0)
  {
    x = _x;
    y = _y;

    r = _r;
    g = _g;
    b = _b;
    a = _a;

    u = _u;
    v = _v;
}
  
  float x, y;
  float r, g, b, a;
  float u, v;
};

class edge
{
public:
  edge(const vertex& rV0, const vertex& rV1, int32 TextureWidth, int32 TextureHeight)
  : v0(rV0.y < rV1.y ? rV0 : rV1),
    v1(rV0.y < rV1.y ? rV1 : rV0),
    upward(rV0.y > rV1.y)
  {
    // coords:
    const uint32 ydiff = (FastToBelow(v1.y) - FastToAbove(v0.y));

    x = v0.x;
    // color
    r = v0.r;
    g = v0.g;
    b = v0.b;
    a = v0.a;
    // texture
    u = v0.u * TextureWidth;
    v = v0.v * TextureHeight;

    if (ydiff)
    {
      const float invdiff = 1.0 / ydiff;
      xincr = (v1.x - v0.x) * invdiff; // Compute the slope
      
      rincr = (v1.r - v0.r) * invdiff; // Compute the slope
      gincr = (v1.g - v0.g) * invdiff; // Compute the slope
      bincr = (v1.b - v0.b) * invdiff; // Compute the slope
      aincr = (v1.a - v0.a) * invdiff; // Compute the slope
      
      uincr = ((v1.u * TextureWidth)  - u) * invdiff; // Compute the slope
      vincr = ((v1.v * TextureHeight) - v) * invdiff; // Compute the slope
    }
    else
    {
      xincr = 0; // Compute the slope
      
      rincr = 0; // Compute the slope
      gincr = 0; // Compute the slope
      bincr = 0; // Compute the slope
      aincr = 0; // Compute the slope
      
      uincr = 0; // Compute the slope
      vincr = 0; // Compute the slope
    }
  }

  void step_coords()
  {
    x += xincr;
  }
  
  void step_color()
  {
    r += rincr;
    g += gincr;
    b += bincr;
    a += aincr;
  }

  void step_texture()
  {
    u += uincr;
    v += vincr;
  }

  bool clip(int32 top, int32 bottom)
  {
    float y = v0.y;
    float yy = v1.y;
    // Bail out if the edge is completely clipped
    if (y >= bottom || yy <= top)
      return true;
    
    float bdiff = yy - bottom;
    float tdiff = top - y;
    if (bdiff > 0)
    {
      // reduce width from the right
      v1.x -= bdiff * xincr;
      v1.y -= bdiff;
    }
    
    if (tdiff > 0)
    {
      // reduce width from the left
      float xdiff = tdiff * xincr;
      x += xdiff;
      v0.x += xdiff;
      v0.y += tdiff;

      // adjust the gradients:
      r += tdiff * rincr;
      g += tdiff * gincr;
      b += tdiff * bincr;
      a += tdiff * aincr;
      u += tdiff * uincr;
      v += tdiff * vincr;
    }
    
    return false;
  }
  
  vertex v0;
  vertex v1;

  float x;
  float xincr;
  
  int32 count;

  float r, g, b, a;
  float u, v;
  float rincr, gincr, bincr, aincr;
  float uincr, vincr;
  
  bool upward;
};

bool compare_edge_y(const edge* e0, const edge* e1)
{
  if (!e0)
    return false;
  if (!e1)
    return true;
  return e0->v0.y < e1->v0.y;
}

bool compare_edge_x(const edge* e0, const edge* e1)
{
  if (!e0)
    return false;
  if (!e1)
    return true;
  return e0->x < e1->x;
}



class span
{
public:
  span(const edge& e0, const edge& e1)
  {
    x = FastToAbove(e0.x);
    width = e1.x - x;

    float leftoffset = x - e0.x;
    
    // color
    r = e0.r;
    g = e0.g;
    b = e0.b;
    a = e0.a;

    u = e0.u;
    v = e0.v;
    
    if (width)
    {
      float invdiff = 1.0 / width;
      rincr = (e1.r - e0.r) * invdiff; // Compute the slope
      gincr = (e1.g - e0.g) * invdiff; // Compute the slope
      bincr = (e1.b - e0.b) * invdiff; // Compute the slope
      aincr = (e1.a - e0.a) * invdiff; // Compute the slope
      uincr = (e1.u - e0.u) * invdiff; // Compute the slope
      vincr = (e1.v - e0.v) * invdiff; // Compute the slope
      stable = ((rincr * rincr) + (gincr * gincr) + (bincr * bincr) + (aincr * aincr) + (uincr * uincr) + (vincr * vincr)) == 0;
    }
    else
    {
      rincr = 0; // Compute the slope
      gincr = 0; // Compute the slope
      bincr = 0; // Compute the slope
      aincr = 0; // Compute the slope
      uincr = 0; // Compute the slope
      vincr = 0; // Compute the slope
      stable = true;
    }
    
  }

  uint32 GetColor() const
  {
    return NUI_RGBA_F(r, g, b, a);
  }

  bool clip(int32 left, int32 right)
  {
    const float xx = x + width;
    // Bail out if the span is completely clipped
    if (x >= right || xx <= left)
      return true;
    
    const float rdiff = xx - right;
    const float ldiff = left - x;
    if (rdiff > 0)
    {
      // reduce width from the right
      width -= rdiff;
    }
    
    if (ldiff > 0)
    {
      // reduce width from the left
      x += ldiff;
      width -= ldiff;
      r += ldiff * rincr;
      g += ldiff * gincr;
      b += ldiff * bincr;
      a += ldiff * aincr;
      u += ldiff * uincr;
      v += ldiff * vincr;
    }
    
    return false;
  }

  bool IsStable()
  {
    return stable;
  }
  
  float x;
  uint32 width;
  
  float r, g, b, a;
  float u, v;
  float rincr, gincr, bincr, aincr;
  float uincr, vincr;
  bool stable;
};

////////////////////////////////////////////////////////////////// RASTERIZER

class Rasterizer
{
public:
  typedef nuiFastDelegate2<span&, int32> HLiner;
  typedef nuiFastDelegate2<int32 /* U */, int32 /* V */, uint32 /* result */> Texelizer;

  Rasterizer(Screen<32>* pScreen, bool OwnScreen)
  {
    mOwnScreen = OwnScreen;
    mpScreen = pScreen;
    mpTexture = NULL;
    mClipLeft = 0;
    mClipRight = pScreen->GetWidth();
    mClipTop = 0;
    mClipBottom = pScreen->GetHeight();
    
    mEdges.reserve(3);
    mActiveEdges.reserve(3);
    
    mFilterTextures = false;
  }
  
  ~Rasterizer()
  {
    if (mOwnScreen)
      delete mpScreen;
  }
  
  void SetClipRect(int32 left, int32 top, int32 right, int32 bottom)
  {
    mClipLeft = left;
    mClipRight = right;
    mClipTop = top;
    mClipBottom = bottom;
  }
  
  void SetTexture(nuiTexture* pTexture)
  {
    mpTexture = pTexture;
    if (!pTexture)
      return;
    
    switch (pTexture->GetImage()->GetBitDepth())
    {
      case 8:
        switch (pTexture->GetImage()->GetPixelFormat())
        {
          case eImagePixelLum:
            mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel8Lum);
            break;
          case eImagePixelAlpha:
            mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel8Alpha);
            break;
          default:
            NGL_ASSERT(0);
            break;
        }
        break;
      case 15:
        mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel15);
        break;
      case 16:
        switch (pTexture->GetImage()->GetPixelFormat())
        {
          case eImagePixelLum:
            mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel16RGB);
            break;
          case eImagePixelAlpha:
            mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel16RGBA);
            break;
          case eImagePixelLumA:
            mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel16LumA);
            break;
          default:
            NGL_ASSERT(0);
            break;
        }
        break;
      case 24:
        mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel24);
        break;
      case 32:
        mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexel32);
        break;
      default:
        NGL_ASSERT(0);
        break;
    }

    if (mFilterTextures)
    {
      mFilterTexelizer = mTexelizer;
      mTexelizer = nuiMakeDelegate(this, &Rasterizer::GetTexelSmooth);
    }
    
    mTextureWidth = mpTexture->GetWidth();
    mTextureHeight = mpTexture->GetHeight();
    mpTextureBuffer = mpTexture->GetImage()->GetBuffer();    
  }
  
  nuiTexture* GetTexture() const
  {
    return mpTexture;
  }
  
  void SetTextureFilter(bool set)
  {
    if (mFilterTextures == set)
      return;
    
    mFilterTextures = set;
    SetTexture(mpTexture);
  }
  
  bool GetTextureFilter() const
  {
    return mFilterTextures;
  }

  void DrawHLineTranspStable(span& rSpan, int32 y)
  {
    if (rSpan.clip(mClipLeft, mClipRight))
      return;
    
    int32 count = rSpan.width;
    uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
    
    const int32 A0 = ToZero(rSpan.r * 0xffff);
    const int32 A1 = ToZero(rSpan.g * 0xffff);
    const int32 A2 = ToZero(rSpan.b * 0xffff);
    const int32 A3 = ToZero(rSpan.a * 0xffff);
    
    const uint32 alpha = A3 >> 8;
    const uint32 malpha = 0x100 - alpha;
    const uint32 colA0 = (((A3 & 0xff00) << 8) | ((A1 & 0xff00) >> 8)) * alpha;
    const uint32 colA1 = (((A2 & 0xff00) << 8) | ((A0 & 0xff00) >> 8)) * alpha;
    
    while (count--)
    {
      uint32 colB = *pSpan;
      
      //uint32 colA = ((A3 & 0xff00) << 16) + ((A2 & 0xff00) << 8) + ((A1 & 0xff00)) + ((A0 & 0xff00) >> 8);
      uint32 colB0 = (colB & 0xff00ff00) >> 8;
      uint32 colB1 = colB & 0x00ff00ff;
      
      if (alpha)
      {
        colB0 *= malpha;
        colB1 *= malpha;
        
        const uint32 col = ((colA0 + colB0) & 0xff00ff00) | (((colA1 + colB1) & 0xff00ff00) >> 8);
        
        *pSpan = col;
      }
      
      pSpan++;
    }
  }
  
  void DrawHLineTransp(span& rSpan, int32 y)
  {
    if (rSpan.clip(mClipLeft, mClipRight))
      return;
    
    int32 count = rSpan.width;
    uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
    
    //int32 col = rSpan.GetColor();
    int32 A0 = ToZero(rSpan.r * 0xffff);
    int32 A1 = ToZero(rSpan.g * 0xffff);
    int32 A2 = ToZero(rSpan.b * 0xffff);
    int32 A3 = ToZero(rSpan.a * 0xffff);
    const int32 I0 = ToZero(rSpan.rincr * 0x10000);
    const int32 I1 = ToZero(rSpan.gincr * 0x10000);
    const int32 I2 = ToZero(rSpan.bincr * 0x10000);
    const int32 I3 = ToZero(rSpan.aincr * 0x10000);
    
    while (count--)
    {
      const uint32 colB = *pSpan;
      
      //uint32 colA = ((A3 & 0xff00) << 16) + ((A2 & 0xff00) << 8) + ((A1 & 0xff00)) + ((A0 & 0xff00) >> 8);
      uint32 colA0 = ((A3 & 0xff00) << 8) | ((A1 & 0xff00) >> 8);
      uint32 colA1 = ((A2 & 0xff00) << 8) | ((A0 & 0xff00) >> 8);
      uint32 colB0 = (colB & 0xff00ff00) >> 8;
      uint32 colB1 = colB & 0x00ff00ff;
      
      const uint32 alpha = A3 >> 8;
      if (alpha)
      {
        const uint32 malpha = 0x100 - alpha;
        
        colA0 *= alpha;
        colA1 *= alpha;
        colB0 *= malpha;
        colB1 *= malpha;
        
        const uint32 col = ((colA0 + colB0) & 0xff00ff00) | (((colA1 + colB1) & 0xff00ff00) >> 8);
        
        *pSpan = col;
      }
      
      pSpan++;
      
      A0 += I0;
      A1 += I1;
      A2 += I2;
      A3 += I3;
    }
  }
  
  void DrawHLineCopyStable(span& rSpan, int32 y)
  {
    if (rSpan.clip(mClipLeft, mClipRight))
      return;
    
    int32 count = rSpan.width;
    uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
    
    const uint32 col = rSpan.GetColor();
    
    while (count--)
      *pSpan++ = col;
  }
  
  void DrawHLineCopy(span& rSpan, int32 y)
  {
    if (rSpan.IsStable())
    {
      DrawHLineCopyStable(rSpan, y);
      return;
    }
    if (rSpan.clip(mClipLeft, mClipRight))
      return;
    
    int32 count = rSpan.width;
    uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
    
    //int32 col = rSpan.GetColor();
    int32 A0 = ToZero(rSpan.r * 0x10000);
    int32 A1 = ToZero(rSpan.g * 0x10000);
    int32 A2 = ToZero(rSpan.b * 0x10000);
    int32 A3 = ToZero(rSpan.a * 0x10000);
    const int32 I0 = ToZero(rSpan.rincr * 0x10000);
    const int32 I1 = ToZero(rSpan.gincr * 0x10000);
    const int32 I2 = ToZero(rSpan.bincr * 0x10000);
    const int32 I3 = ToZero(rSpan.aincr * 0x10000);
    
    while (count--)
    {
      const uint32 colA = ((A3 & 0xff00) << 16) + ((A2 & 0xff00) << 8) + ((A1 & 0xff00)) + ((A0 & 0xff00) >> 8);
      *pSpan++ = colA;
      
      A0 += I0;
      A1 += I1;
      A2 += I2;
      A3 += I3;
    }
  }
  
  void DrawHLineTextureCopy(span& rSpan, int32 y)
  {
//    if (rSpan.IsStable())
//    {
//      DrawHLineCopyStable(rSpan, y);
//      return;
//    }
    if (rSpan.clip(mClipLeft, mClipRight))
      return;
    
    int32 count = rSpan.width;
    uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
    
    //int32 col = rSpan.GetColor();
    int32 U = ToNearest(rSpan.u * (float)0x10000);
    int32 V = ToNearest(rSpan.v * (float)0x10000);
    const int32 IU = ToNearest(rSpan.uincr * (float)0x10000);
    const int32 IV = ToNearest(rSpan.vincr * (float)0x10000);
    
    while (count--)
    {
      *pSpan++ = mTexelizer(U, V);
      
      U += IU;
      V += IV;
    }
  }
  
  typedef nuiFastDelegate2<const edge*, const edge*, bool> Comparator;
  
  void bubblesort(std::vector<edge*>& rVec, const Comparator& rComp)
  {
    uint32 n = rVec.size();
    bool swapped;
    do
    {
      swapped = false;
      n--;
      for (uint32 i = 0; i < n; i++)
      {
        if (rComp(rVec[i + 1], rVec[i]))
        {
          edge* pEdge = rVec[i];
          rVec[i] = rVec[i + 1];
          rVec[i + 1] = pEdge;
          swapped = true;
        }
      }
    } while (swapped);
  }
  
  void DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2, const HLiner& rHLiner)
  {
    mEdges.clear();
    mActiveEdges.clear();
    edge Edge0(v0, v1, mTextureWidth, mTextureHeight);
    edge Edge1(v1, v2, mTextureWidth, mTextureHeight);
    edge Edge2(v2, v0, mTextureWidth, mTextureHeight);
    
    if (v0.y != v1.y)
    {
      if (!Edge0.clip(mClipTop, mClipBottom))
        mEdges.push_back(&Edge0);
    }
    if (v1.y != v2.y)
    {
      if (!Edge1.clip(mClipTop, mClipBottom))
        mEdges.push_back(&Edge1);
    }
    if (v2.y != v0.y)
    {
      if (!Edge2.clip(mClipTop, mClipBottom))
        mEdges.push_back(&Edge2);
    }
    
    if (mEdges.empty())
      return; // float on y?
    
    if (v0.x == v1.x && v0.x == v2.x)
      return; // flat on x?
    
    {
      // Is the triangle flat but not parallel to an axis
      if (0.0f == (v0.x * (v1.y - v2.y) + v1.x * (v0.y - v2.y) + v2.x * (v0.y - v1.y)))
        return;
    }
    
    bubblesort(mEdges, Comparator(&compare_edge_y));
    
    uint32 y = FastToBelow(mEdges[0]->v0.y);
    uint32 current_edge = 0;
    for (uint32 e = 0; e < mEdges.size(); e++)
    {
      // Compute the number of scan lines before this edge is to become active
      mEdges[e]->count = FastToAbove(mEdges[e]->v0.y) - y;
      NGL_ASSERT(mEdges[e]->count >= 0);
    }
    
    uint32 done;
    uint32 count = 0;
    
    do
    {
      done = 0;
      // Check the edges list for activation:
      for (uint32 e = current_edge; e < mEdges.size(); e++)
      {
        if (mEdges[e]->count == 0)
        {
          // Make this edge an active one
          mEdges[e]->count = FastToAbove(mEdges[e]->v1.y) - FastToBelow(mEdges[e]->v0.y);
          mActiveEdges.push_back(mEdges[e]);
          current_edge++;
          count++;
          
          bubblesort(mActiveEdges, Comparator(&compare_edge_x));
        }
        else
        {
          // count down to activation
          mEdges[e]->count--;
        }
      }
      
      bool pen_down = false;
      bool last_upward = !mActiveEdges[0]->upward;
      edge* pLastEdge = NULL;
      for (uint32 i = 0; i < count; i++)
      {
        edge* pEdge = mActiveEdges[i];
        if (pEdge)
        {
          bool last_pen_down = pen_down;
          
          pen_down ^= last_upward != pEdge->upward;
          
          if (!pen_down && pLastEdge)
          {
            // Draw this span
            span Span(*pLastEdge, *pEdge);
            rHLiner(Span, y);
            
            pLastEdge = NULL;
          }
          
          if (pen_down && !last_pen_down)
            pLastEdge = pEdge;
          
          last_upward = pEdge->upward;
        }
      }
      
      // Go to the next scanline for each active edge:
      for (uint32 i = 0; i < count; i++)
      {
        edge* pEdge = mActiveEdges[i];
        if (pEdge)
        {
          pEdge->x += pEdge->xincr;
          pEdge->step_color();
          pEdge->step_texture();
          
          // Count down to desactivation
          pEdge->count--;
          
          // Check for desactivation:
          if (!pEdge->count)
            mActiveEdges[i] = NULL;
          else
            done++;
        }
      }
      
      y++;
    }
    while (done);
  }
  
private:
  Screen<32>* mpScreen;
  nuiTexture* mpTexture;
  Texelizer mTexelizer;
  Texelizer mFilterTexelizer;
  bool mOwnScreen;
  
  int32 mClipLeft;
  int32 mClipRight;
  int32 mClipTop;
  int32 mClipBottom;
  std::vector<edge*> mEdges;
  std::vector<edge*> mActiveEdges;

  int32 mTextureWidth;
  int32 mTextureHeight;
  void* mpTextureBuffer;
  
  bool mFilterTextures;
  
  uint32 GetTexel8Lum(int32 u, int32 v) const
  {
    u = u >> 16;
    v = v >> 16;
    
    if (u < 0)
      u = 0;
    else if (u > mTextureWidth)
      u = mTextureWidth - 1;
    if (v < 0)
      v = 0;
    else if (v > mTextureHeight)
      v = mTextureHeight - 1;
    
    const uint8* pBuffer = (uint8*)mpTextureBuffer;
    const uint32 offset = (u + v * mTextureWidth);
    const uint8 c = pBuffer[offset];
    return 0xff000000 + (c << 16) | (c << 8) | (c);
  }
  
  uint32 GetTexel8Alpha(int32 u, int32 v) const
  {
    u = u >> 16;
    v = v >> 16;
    
    if (u < 0)
      u = 0;
    else if (u > mTextureWidth)
      u = mTextureWidth - 1;
    if (v < 0)
      v = 0;
    else if (v > mTextureHeight)
      v = mTextureHeight - 1;
    
    const uint8* pBuffer = (uint8*)mpTextureBuffer;
    const uint32 offset = (u + v * mTextureWidth);
    const uint8 a = pBuffer[offset];
    return (a << 24);
  }
  
  uint32 GetTexel15(int32 u, int32 v) const
  {
    return -1;
  }
  
  uint32 GetTexel16RGB(int32 u, int32 v) const
  {
    return -1;
  }
  
  uint32 GetTexel16RGBA(int32 u, int32 v) const
  {
    return -1;
  }
  
  uint32 GetTexel16LumA(int32 u, int32 v) const
  {
    u = u >> 16;
    v = v >> 16;
    
    if (u < 0)
      u = 0;
    else if (u > mTextureWidth)
      u = mTextureWidth - 1;
    if (v < 0)
      v = 0;
    else if (v > mTextureHeight)
      v = mTextureHeight - 1;
    
    const uint8* pBuffer = (uint8*)mpTextureBuffer;
    const uint32 offset = (u + v * mTextureWidth) * 2;
    const uint8 c = pBuffer[offset];
    const uint8 a = pBuffer[offset + 1];
    return (a << 24) + (c << 16) | (c << 8) | (c);
  }
  
  uint32 GetTexel24(int32 u, int32 v) const 
  {
    u = u >> 16;
    v = v >> 16;
    
    if (u < 0)
      u = 0;
    else if (u > mTextureWidth)
      u = mTextureWidth - 1;
    if (v < 0)
      v = 0;
    else if (v > mTextureHeight)
      v = mTextureHeight - 1;
    
    const uint8* pBuffer = (uint8*)mpTextureBuffer;
    const uint32 offset = (u + v * mTextureWidth) * 3;
    return 0xff000000 + (pBuffer[offset + 2] << 16) | (pBuffer[offset+1] << 8) | (pBuffer[offset]);
  }
  
  
  uint32 GetTexel32(int32 u, int32 v)
  {
    u = u >> 16;
    v = v >> 16;
    
    const uint32* pBuffer = (uint32*)mpTextureBuffer;
    if (u < 0)
      u = 0;
    else if (u > mTextureWidth)
      u = mTextureWidth - 1;
    if (v < 0)
      v = 0;
    else if (v > mTextureHeight)
      v = mTextureHeight - 1;
    
    return pBuffer[u + v * mTextureWidth];
  }

  uint32 GetTexelSmooth(int32 u, int32 v) const
  {
    const int32 w = mTextureWidth;
    const int32 h = mTextureHeight;
    int32 _u = u * w;
    int32 _v = v * h;
    
    int32 x = (_u & 0xffff) >> 8;
    int32 y = (_v & 0xffff) >> 8;
    
    if (x)
      x += 1;
    if (y)
      y += 1;
    
    const int32 xx = 0x100 - x;
    const int32 yy = 0x100 - y;
    
    int32 uu = u + 0x10000;
    int32 vv = v + 0x10000;
    
    const uint32 col00 = mFilterTexelizer(u,  v);
    const uint32 col10 = mFilterTexelizer(uu, v);
    const uint32 col01 = mFilterTexelizer(u,  vv);
    const uint32 col11 = mFilterTexelizer(uu, vv);
    
    const uint32 res = nuiLERPColor( nuiLERPColor(col00, col10, xx, x), nuiLERPColor(col01, col11, xx, x), yy, y) ;
    
    return res;
  }
  
};



/*
 * MainWindow
 */

void DrawHLineNULL(span& rSpan, int32 y)
{
  // Do nothing!
}

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
//  nuiTexture* pTexture = nuiTexture::GetTexture(nglPath(_T("/Users/meeloo/work/nui3/examples/data/jpeg.jpg")));
  nuiTexture* pTexture = nuiTexture::GetTexture(nglPath(_T("/Users/meeloo/work/nui3/examples/data/rasters.png")));
  SetDebugMode(true);
  nglImageInfo info(640, 480, 32);
  mpImage = new nglImage(info, eTransfert);
  uint32* pBuffer = (uint32*)mpImage->GetBuffer();
  for (uint32 i = 0; i < info.mHeight * info.mWidth; i++)
    pBuffer[i] = 0xff7f7f7f;
  
  Screen<32> screen(info.mWidth, info.mHeight, false, pBuffer);
  Rasterizer rasterizer(&screen, false);
  rasterizer.SetClipRect(0, 0, 640, 480);
  rasterizer.SetTexture(pTexture);
  rasterizer.SetTextureFilter(false);
  
  {
//    vertex v0(10, 10,   1, 0, 0,   0, 0, 0);
//    vertex v1(10, 100,  0, 1, 0,  .5, 0, 1);
//    vertex v2(303, 100, 0, 0, 1,  .5, 1, 1);
//    vertex v3(303, 10,  0, 1, 0, 1.0, 1, 0);
    
#define XMULTIPLIER 4
#define YMULTIPLIER 4
    vertex v0(0, 0,   1, 0, 0,   0, 0, 0);
    vertex v1(0, pTexture->GetHeight() * YMULTIPLIER,  0, 1, 0,  .5, 0, 1);
    vertex v2(pTexture->GetWidth() * XMULTIPLIER, pTexture->GetHeight() * YMULTIPLIER, 0, 0, 1,  .5, 1, 1);
    vertex v3(pTexture->GetWidth() * XMULTIPLIER, 0,  0, 1, 0, 1.0, 1, 0);
    //    rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
//    rasterizer.DrawTriangle(v0, v2, v3, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
    rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTextureCopy));
    rasterizer.DrawTriangle(v0, v2, v3, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTextureCopy));
//    rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTextureCopy));
//    rasterizer.DrawTriangle(v0, v2, v3, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTextureCopy));
  }
  
#if 0
  rasterizer.SetClipRect(330, 40, 400, 90);
  {
    vertex v0(310, 10,  1, 0, 0, 0);
    vertex v1(310, 100,  0, 1, 0, .5);
    vertex v2(603, 100, 0, 0, 1, .5);
    vertex v3(603, 10,  0, 1, 0, 1.0);
    rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
    rasterizer.DrawTriangle(v0, v2, v3, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
  }

  rasterizer.SetClipRect(0, 200, 640, 480);
  //rasterizer.SetClipRect(0, 200, 20, 220);
  {
    //srandom(time(NULL));
    double now = nglTime();
    for (uint32 i = 0; i < 1000; i++)
    {
#define R ((random() % 200 + 55) / 255.0f)
#if 1 // Large triangles
      vertex v0(random() % 1200 - 200, random() % 800 - 200, R, R, R, R, 0, 0);
      vertex v1(random() % 1200 - 200, random() % 800 - 200, R, R, R, R, 1, 0);
      vertex v2(random() % 1200 - 200, random() % 800 - 200, R, R, R, R, 0, 1);
#else // small triangles
      float x = random() % 1200 - 200;
      float y = random() % 1200 - 200;
      vertex v0(x, y, R, R, R, R, 0, 0);
      vertex v1(x + random() % 100 - 50, y + random() % 100 - 50, R, R, R, R, 1, 0);
      vertex v2(x + random() % 100 - 50, y + random() % 100 - 50, R, R, R, R, 0, 1);
#endif
        
//      printf("triangle %d (%f, %f / %f, %f / %f, %f)\n", i, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineCopyStable));
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineCopy));
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTranspStable));
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTextureCopy));
//      rasterizer.DrawTriangle(v0, v1, v2, DrawHLineNULL);
    }
    now = nglTime() - now;
    printf("triangle rendering time: %fs\n", now);
  }
#endif
  
  mpTexture = nuiTexture::GetTexture(mpImage, true);
  nuiImage* pImageUI = new nuiImage(mpTexture);
  pImageUI->SetPosition(nuiCenter);
  AddChild(pImageUI);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



