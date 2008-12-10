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
    uint32 ydiff = (FastToBelow(v1.y) - FastToAbove(v0.y));
    x = v0.x;
    xincr = (v1.x - v0.x) / ydiff; // Compute the slope

    // color
    r = v0.r;
    g = v0.g;
    b = v0.b;
    a = v0.a;
    rincr = (v1.r - v0.r) / ydiff; // Compute the slope
    gincr = (v1.g - v0.g) / ydiff; // Compute the slope
    bincr = (v1.b - v0.b) / ydiff; // Compute the slope
    aincr = (v1.a - v0.a) / ydiff; // Compute the slope

    // texture
    u = v0.u;
    v = v0.v;
    uincr = (v1.u - v0.u) / ydiff; // Compute the slope
    vincr = (v1.v - v0.v) / ydiff; // Compute the slope
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

    float diff = e1.x - e0.x;
    
    // color
    r = e0.r;
    g = e0.g;
    b = e0.b;
    a = e0.a;
    rincr = (e1.r - e0.r) / diff; // Compute the slope
    gincr = (e1.g - e0.g) / diff; // Compute the slope
    bincr = (e1.b - e0.b) / diff; // Compute the slope
    aincr = (e1.a - e0.a) / diff; // Compute the slope
    
    // texture
    u = e0.u;
    v = e0.v;
    uincr = (e1.u - e0.u) / diff; // Compute the slope
    vincr = (e1.v - e0.v) / diff; // Compute the slope
  }

  uint32 GetColor() const
  {
    return NUI_RGBA_F(r, g, b, a);
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
  
  bool clip(int32 left, int32 right)
  {
    float xx = x + width;
    // Bail out if the span is completely clipped
    if (x >= right || xx <= left)
      return true;
    
    float rdiff = xx - right;
    float ldiff = left - x;
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
  
  float x;
  uint32 width;
  
  float r, g, b, a;
  float u, v;
  float rincr, gincr, bincr, aincr;
  float uincr, vincr;
};

class Rasterizer
{
public:
  Rasterizer(Screen<32>* pScreen, bool OwnScreen);
  ~Rasterizer();
  
  void DrawHLine(uint32 x0, uint32 x1, uint32 y, uint32 col);
  void DrawHLine(span& rSpan, int32 y);
  void BlendHLine(span& rSpan, int32 y);
  
  void DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2);
  void DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2, uint32 color);
  void DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint32 color);
  
  void SetClipRect(int32 left, int32 top, int32 right, int32 bottom);
  
private:
  Screen<32>* mpScreen;
  bool mOwnScreen;
  
  int32 mClipLeft;
  int32 mClipRight;
  int32 mClipTop;
  int32 mClipBottom;
};

Rasterizer::Rasterizer(Screen<32>* pScreen, bool OwnScreen)
{
  mOwnScreen = OwnScreen;
  mpScreen = pScreen;
  mClipLeft = 0;
  mClipRight = pScreen->GetWidth();
  mClipTop = 0;
  mClipBottom = pScreen->GetHeight();
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
  return e0->v0.y < e1->v0.y;
}

bool compare_edge_x(const edge* e0, const edge* e1)
{
  return e0->x < e1->x;
}

void Rasterizer::DrawHLine(uint32 x0, uint32 x1, uint32 y, uint32 col)
{
  uint32* pSpan = (uint32*)mpScreen->GetPixel(x0, y);
  for (uint32 x = x0; x < x1; x++)
    *pSpan++ += col;
}

void Rasterizer::DrawHLine(span& rSpan, int32 y)
{
  if (rSpan.clip(mClipLeft, mClipRight))
    return;

  int32 count = rSpan.width;
  uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
  while (count--)
  {
    *pSpan++ = rSpan.GetColor();
    rSpan.step_color();
    rSpan.step_texture();
  }
}

void Rasterizer::BlendHLine(span& rSpan, int32 y)
{
  if (rSpan.clip(mClipLeft, mClipRight))
    return;
  
  int32 count = rSpan.width;
  uint32* pSpan = (uint32*)mpScreen->GetPixel(rSpan.x, y);
  while (count--)
  {
    uint32 colA = rSpan.GetColor();
    uint32 colB = *pSpan;
    
    uint32 colA0 = (colA & 0xff00ff00) >> 8;
    uint32 colB0 = (colB & 0xff00ff00) >> 8;
    uint32 colA1 = colA & 0x00ff00ff;
    uint32 colB1 = colB & 0x00ff00ff;

    uint32 alpha = (colA0 >> 16);
    if (alpha)
    {
      uint32 malpha = 0x100 - alpha;
      
      colA0 *= alpha;
      colA1 *= alpha;
      colB0 *= malpha;
      colB1 *= malpha;
      
      colA = colA0 + colB0;
      colB = colA1 + colB1;
      
      uint32 col = (colA & 0xff00ff00) | ((colB & 0xff00ff00) >> 8);
      
      *pSpan = col;
    }

    pSpan++;
    
    rSpan.step_color();
    rSpan.step_texture();
  }
}

void Rasterizer::DrawTriangle(float x0, float y0, float x1, float y1, float x2, float y2, uint32 color)
{
  DrawTriangle(vertex(x0, y0), vertex(x1, y1), vertex(x2, y2), color);
}

void Rasterizer::DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2, uint32 color)
{
  std::vector<edge*> edges;
  std::list<edge*> active_edges;
  if (v0.y != v1.y)
    edges.push_back(new edge(v0, v1));
  if (v1.y != v2.y)
    edges.push_back(new edge(v1, v2));
  if (v2.y != v0.y)
    edges.push_back(new edge(v2, v0));
  
  if (edges.empty())
    return;

  uint32 y = FastToBelow(edges[0]->v0.y);

  std::sort(edges.begin(), edges.end(), &compare_edge_y);
  uint32 current_edge = 0;
  for (uint32 e = 0; e < edges.size(); e++)
  {
    // Compute the number of scan lines before this edge is to become active
    edges[e]->count = FastToAbove(edges[e]->v0.y) - y;
  }
  
  
  do
  {
    // Check the edges list for activation:
    for (uint32 e = current_edge; e < edges.size(); e++)
    {
      if (edges[e]->count == 0)
      {
        // Make this edge an active one
        edges[e]->count = FastToAbove(edges[e]->v1.y) - FastToBelow(edges[e]->v0.y);
        active_edges.push_back(edges[e]);
        current_edge++;
      }
      else
      {
        // count down to activation
        edges[e]->count--;
      }
    }
    
    active_edges.sort(&compare_edge_x);
    
    std::list<edge*>::iterator it  = active_edges.begin();
    std::list<edge*>::iterator end = active_edges.end();
    
    bool pen_down = false;
    bool last_upward = !(*it)->upward;
    edge* pLastEdge = NULL;
    while (it != end)
    {
      edge* pEdge = *it;
      
      bool last_pen_down = pen_down;

      pen_down ^= last_upward != pEdge->upward;
      
      if (!pen_down && pLastEdge)
      {
        // Draw this span
        const float x0 = pLastEdge->x;
        const float x1 = pEdge->x;
        DrawHLine(FastToAbove(x0), FastToAbove(x1), y, color);
        
        pLastEdge = NULL;
      }
      
      if (pen_down && !last_pen_down)
        pLastEdge = pEdge;
      
      last_upward = pEdge->upward;
      
      ++it;
    }

    // Go to the next scanline for each active edge:
    it  = active_edges.begin();
    while (it != end)
    {
      edge* pEdge = *it;
      pEdge->x += pEdge->xincr;
      
      // Count down to desactivation
      pEdge->count--;
      
      // Check for desactivation:
      if (pEdge->count)
        ++it;
      else
        active_edges.erase(it++);
    }
    
    y++;
  }
  while (!active_edges.empty());
}

void Rasterizer::DrawTriangle(const vertex& v0, const vertex& v1, const vertex& v2)
{
  std::vector<edge*> edges;
  std::list<edge*> active_edges;
  if (v0.y != v1.y)
  {
    edge* pEdge = new edge(v0, v1);
    if (pEdge->clip(mClipTop, mClipBottom))
      delete pEdge;
    else
      edges.push_back(pEdge);
  }
  if (v1.y != v2.y)
  {
    edge* pEdge = new edge(v1, v2);
    if (pEdge->clip(mClipTop, mClipBottom))
      delete pEdge;
    else
      edges.push_back(pEdge);
  }
  if (v2.y != v0.y)
  {
    edge* pEdge = new edge(v2, v0);
    if (pEdge->clip(mClipTop, mClipBottom))
      delete pEdge;
    else
      edges.push_back(pEdge);
  }
  
  if (edges.empty())
    return; // float on y?
  
  if (v0.x == v1.x && v0.x == v2.x)
    return; // flat on x?
  
  {
    // Is the triangle flat but not parallel to an axis
    float ax = v0.x;
    float bx = v1.x;
    float cx = v2.x;
    float ay = v0.y;
    float by = v1.y;
    float cy = v2.y;
    if (0 == (ax * (by - cy) + bx * (ay - cy) + cx * (ay - by)))
      return;
  }
    
  std::sort(edges.begin(), edges.end(), &compare_edge_y);
  uint32 y = FastToBelow(edges[0]->v0.y);
  uint32 current_edge = 0;
  for (uint32 e = 0; e < edges.size(); e++)
  {
    // Compute the number of scan lines before this edge is to become active
    edges[e]->count = FastToAbove(edges[e]->v0.y) - y;
    NGL_ASSERT(edges[e]->count >= 0);
  }
  
  
  do
  {
    // Check the edges list for activation:
    for (uint32 e = current_edge; e < edges.size(); e++)
    {
      if (edges[e]->count == 0)
      {
        // Make this edge an active one
        edges[e]->count = FastToAbove(edges[e]->v1.y) - FastToBelow(edges[e]->v0.y);
        active_edges.push_back(edges[e]);
        current_edge++;
      }
      else
      {
        // count down to activation
        edges[e]->count--;
      }
    }
    
    active_edges.sort(&compare_edge_x);
    
    std::list<edge*>::iterator it  = active_edges.begin();
    std::list<edge*>::iterator end = active_edges.end();
    
    bool pen_down = false;
    bool last_upward = !(*it)->upward;
    edge* pLastEdge = NULL;
    while (it != end)
    {
      edge* pEdge = *it;
      
      bool last_pen_down = pen_down;
      
      pen_down ^= last_upward != pEdge->upward;
      
      if (!pen_down && pLastEdge)
      {
        // Draw this span
        const float x0 = pLastEdge->x;
        const float x1 = pEdge->x;
        span Span(*pLastEdge, *pEdge);
        BlendHLine(Span, y);
        
        pLastEdge = NULL;
      }
      
      if (pen_down && !last_pen_down)
        pLastEdge = pEdge;
      
      last_upward = pEdge->upward;
      
      ++it;
    }
    
    // Go to the next scanline for each active edge:
    it  = active_edges.begin();
    while (it != end)
    {
      edge* pEdge = *it;
      pEdge->x += pEdge->xincr;
      pEdge->step_color();
      pEdge->step_texture();
      
      // Count down to desactivation
      pEdge->count--;
      
      // Check for desactivation:
      if (pEdge->count)
        ++it;
      else
        active_edges.erase(it++);
    }
    
    y++;
  }
  while (!active_edges.empty());
}


/*
 * MainWindow
 */

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
  
//  rasterizer.DrawTriangle(10, 10, 20, 15, 5, 20,       0x7f7f0000);
//  rasterizer.DrawTriangle(100, 10, 200, 10, 100, 100,  0x7f007f00);
//  rasterizer.DrawTriangle(200, 10, 200, 100, 100, 100, 0x7f00007f);

//  vertex v0(100, 10,  1, 0, 0);
//  vertex v1(10, 100,  0, 1, 0);
//  vertex v2(150, 150, 0, 0, 1);
//  vertex v3(200, 10,  0, 1, 0);
//  rasterizer.DrawTriangle(v0, v1, v2);
//  rasterizer.DrawTriangle(v0, v2, v3);
  {
    vertex v0(10, 10,  1, 0, 0, 0);
    vertex v1(10, 100,  0, 1, 0, .5);
    vertex v2(303, 100, 0, 0, 1, .5);
    vertex v3(303, 10,  0, 1, 0, 1.0);
    rasterizer.DrawTriangle(v0, v1, v2);
    rasterizer.DrawTriangle(v0, v2, v3);
  }
  
  rasterizer.SetClipRect(330, 40, 400, 90);
  {
    vertex v0(310, 10,  1, 0, 0, 0);
    vertex v1(310, 100,  0, 1, 0, .5);
    vertex v2(603, 100, 0, 0, 1, .5);
    vertex v3(603, 10,  0, 1, 0, 1.0);
    rasterizer.DrawTriangle(v0, v1, v2);
    rasterizer.DrawTriangle(v0, v2, v3);
  }

  rasterizer.SetClipRect(0, 200, 640, 480);
  {
    srandom(time(NULL));
    double now = nglTime();
    for (uint32 i = 0; i < 10000; i++)
    {
#define R ((random() % 255) / 255.0f)
      vertex v0(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
      vertex v1(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
      vertex v2(random() % 1200 - 200, random() % 800 - 200, R, R, R, R);
//      printf("trangle %d (%f, %f / %f, %f / %f, %f)\n", i, v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
      rasterizer.DrawTriangle(v0, v1, v2);
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



