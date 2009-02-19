/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiSoftwarePainter_h__
#define __nuiSoftwarePainter_h__

#include "nuiDrawContext.h"
#include "nglImage.h"

class nuiRasterizer;

class NUI_API nuiSoftwarePainter : public nuiPainter, public nuiCacheManager
{
public:
  nuiSoftwarePainter(const nuiRect& rRect, nglContext* pContext = NULL);
  virtual ~nuiSoftwarePainter();

  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(const nuiRenderArray& rArray);
  virtual void ClearColor();
  virtual void ClearStencil(uint8 value);
  virtual void BeginSession();
  virtual void EndSession();
  virtual uint32 GetRectangleTextureSupport() const { return 1; }

  void Display(nglWindow* pWindow, const nuiRect& rRect);

  nuiRasterizer* GetRasterizer() const
  {
    return mpRasterizer;
  }

protected:

  virtual void ReleaseCacheObject(void* pHandle);

  class nuiRasterizer* mpRasterizer;

  void DrawLines(const nuiRenderArray& rArray);
  void DrawLineStrip(const nuiRenderArray& rArray);
  void DrawLineLoop(const nuiRenderArray& rArray);
  void DrawTriangles(const nuiRenderArray& rArray);
  void DrawTrianglesFan(const nuiRenderArray& rArray);
  void DrawTrianglesStrip(const nuiRenderArray& rArray);
  void DrawQuads(const nuiRenderArray& rArray);
  void DrawQuadStrip(const nuiRenderArray& rArray);

  void DrawLine(const nuiRenderArray& rArray, int p1, int p2);
  void DrawTriangle(const nuiRenderArray& rArray, int p1, int p2, int p3);
  void DrawRectangle(const nuiRenderArray& rArray, int p1, int p2, int p3, int p4);
};

#endif //__nuiSoftwarePainter_h__

