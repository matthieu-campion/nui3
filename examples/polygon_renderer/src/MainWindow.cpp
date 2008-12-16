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
  edge(const vertex& rV0, const vertex& rV1)
  : v0(rV0.y < rV1.y ? rV0 : rV1),
    v1(rV0.y < rV1.y ? rV1 : rV0),
    upward(rV0.y > rV1.y)
  {
    // coords:
    const uint32 ydiff = (FastToBelow(v1.y) - FastToAbove(v0.y));
    const float invdiff = 1.0 / ydiff;

    x = v0.x;
    // color
    r = v0.r;
    g = v0.g;
    b = v0.b;
    a = v0.a;
    // texture
    u = v0.u;
    v = v0.v;

    xincr = (v1.x - v0.x) * invdiff; // Compute the slope

    rincr = (v1.r - v0.r) * invdiff; // Compute the slope
    gincr = (v1.g - v0.g) * invdiff; // Compute the slope
    bincr = (v1.b - v0.b) * invdiff; // Compute the slope
    aincr = (v1.a - v0.a) * invdiff; // Compute the slope

    uincr = (v1.u - v0.u) * invdiff; // Compute the slope
    vincr = (v1.v - v0.v) * invdiff; // Compute the slope
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
    v += uincr;
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

class span
{
public:
  span(const edge& e0, const edge& e1)
  {
    x = FastToAbove(e0.x);
    width = FastToAbove(e1.x) - x;

    const float diff = e1.x - e0.x;
    
    // color
    r = e0.r;
    g = e0.g;
    b = e0.b;
    a = e0.a;

    u = e0.u;
    v = e0.v;
    
    if (diff)
    {
      float invdiff = 1.0 / diff;
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

class Rasterizer
{
public:
  Rasterizer(Screen<32>* pScreen, bool OwnScreen);
  ~Rasterizer();

  typedef nuiFastDelegate2<span&, int32> HLiner;
    
  void DrawHLineCopyStable(span& rSpan, int32 y);
  void DrawHLineCopy(span& rSpan, int32 y);
  void DrawHLineTranspStable(span& rSpan, int32 y);
  void DrawHLineTransp(span& rSpan, int32 y);
  
  void DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2, const HLiner& rHLiner);
  
  void SetClipRect(int32 left, int32 top, int32 right, int32 bottom);

private:
  Screen<32>* mpScreen;
  bool mOwnScreen;
  
  int32 mClipLeft;
  int32 mClipRight;
  int32 mClipTop;
  int32 mClipBottom;
  std::vector<edge*> mEdges;
  std::vector<edge*> mActiveEdges;
};

Rasterizer::Rasterizer(Screen<32>* pScreen, bool OwnScreen)
{
  mOwnScreen = OwnScreen;
  mpScreen = pScreen;
  mClipLeft = 0;
  mClipRight = pScreen->GetWidth();
  mClipTop = 0;
  mClipBottom = pScreen->GetHeight();

  mEdges.reserve(3);
  mActiveEdges.reserve(3);
}

Rasterizer::~Rasterizer()
{
  if (mOwnScreen)
    delete mpScreen;
}

void Rasterizer::SetClipRect(int32 left, int32 top, int32 right, int32 bottom)
{
  mClipLeft = left;
  mClipRight = right;
  mClipTop = top;
  mClipBottom = bottom;
}

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

void Rasterizer::DrawHLineTranspStable(span& rSpan, int32 y)
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

void Rasterizer::DrawHLineTransp(span& rSpan, int32 y)
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

void Rasterizer::DrawHLineCopyStable(span& rSpan, int32 y)
{
  if (rSpan.clip(mClipLeft, mClipRight))
    return;
  
  int32 count = rSpan.width;
  uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
  
  const uint32 col = rSpan.GetColor();

  while (count--)
    *pSpan++ = col;
}

void Rasterizer::DrawHLineCopy(span& rSpan, int32 y)
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

void Rasterizer::DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2, const HLiner& rHLiner)
{
  mEdges.clear();
  mActiveEdges.clear();
  edge Edge0(v0, v1);
  edge Edge1(v1, v2);
  edge Edge2(v2, v0);
  
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
          const float x0 = pLastEdge->x;
          const float x1 = pEdge->x;
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
  SetDebugMode(true);
  nglImageInfo info(640, 480, 32);
  mpImage = new nglImage(info, eTransfert);
  uint32* pBuffer = (uint32*)mpImage->GetBuffer();
  for (uint32 i = 0; i < info.mHeight * info.mWidth; i++)
    pBuffer[i] = 0xff7f7f7f;
  
  Screen<32> screen(info.mWidth, info.mHeight, false, pBuffer);
  Rasterizer rasterizer(&screen, false);
  rasterizer.SetClipRect(0, 0, 640, 480);
  
  {
    vertex v0(10, 10,  1, 0, 0, 0);
    vertex v1(10, 100,  0, 1, 0, .5);
    vertex v2(303, 100, 0, 0, 1, .5);
    vertex v3(303, 10,  0, 1, 0, 1.0);
    rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
    rasterizer.DrawTriangle(v0, v2, v3, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
  }
  
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
    for (uint32 i = 0; i < 1000000; i++)
    {
#define R ((random() % 200 + 55) / 255.0f)
#if 0 // Large triangles
      vertex v0(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
      vertex v1(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
      vertex v2(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
#else // small triangles
      float x = random() % 1200 - 200;
      float y = random() % 1200 - 200;
      vertex v0(x, y, R, R, R, R);
      vertex v1(x + random() % 100 - 50, y + random() % 100 - 50, R, R, R, R);
      vertex v2(x + random() % 100 - 50, y + random() % 100 - 50, R, R, R, R);
#endif
        
//      printf("trangle %d (%f, %f / %f, %f / %f, %f)\n", i, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineCopyStable));
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineCopy));
//      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTranspStable));
      rasterizer.DrawTriangle(v0, v1, v2, nuiMakeDelegate(&rasterizer, &Rasterizer::DrawHLineTransp));
//      rasterizer.DrawTriangle(v0, v1, v2, DrawHLineNULL);
    }
    now = nglTime() - now;
    printf("triangle rendering time: %fs\n", now);
  }
  
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



