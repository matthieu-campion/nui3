/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPainter_h__
#define __nuiPainter_h__

/* nuiDrawContext design

  Rendering context divided in two parts:
  - Front End = nui high level rendering API. Backward compatible with the former nui API.
  - Back End = nui low level rendering API.

  Back End:
  Accepts two kind of requests:
  - Change the rendering state.
  - Draw something.

  Rendering state changes are accumulated until a Drawing operation is requested. The state is then applied before we carry the rendering operation.

*/

//#include "nui.h"
#include "nuiRect.h"

#include "nuiRenderArray.h"
#include "nuiRenderState.h"

class nuiSurface;

class NUI_API nuiClipper : public nuiRect
{
public:
  nuiClipper(const nuiRect& rRect, bool enable = true)
    : nuiRect(rRect)
  {
    mEnabled = enable;
  }

  nuiClipper(bool enable = false)
  {
    mEnabled = enable;
  }

  nuiClipper(const nuiClipper& rClipper)
    : nuiRect(rClipper)
  {
    mEnabled = rClipper.mEnabled;
  }

  nuiClipper& operator= (const nuiClipper& rClipper)
  {
    mLeft  = rClipper.mLeft;
    mRight = rClipper.mRight;
    mTop = rClipper.mTop;
    mBottom = rClipper.mBottom;
    mEnabled = rClipper.mEnabled;
    return *this;
  }

  bool mEnabled;
};

class NUI_API nuiPainter
{
public:
  nuiPainter(nglContext* pContext = NULL);
  virtual ~nuiPainter();

  virtual void SetSize(uint32 sizex, uint32 sizey) = 0;
  virtual void StartRendering();
  virtual void BeginSession() = 0;
  virtual void EndSession() = 0;

  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false) = 0;
  virtual void DrawArray(nuiRenderArray* pArray) = 0;
  virtual void Clear(bool color, bool depth, bool stencil) = 0;

  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual const nuiMatrix& GetMatrix() const;

  virtual void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix);
  virtual void MultProjectionMatrix(const nuiMatrix& rMatrix);
  virtual void PushProjectionMatrix();
  virtual void PopProjectionMatrix();
  virtual const nuiMatrix& GetProjectionMatrix() const;
  virtual const nuiRect& GetProjectionViewport() const;
  
  virtual void PushClipping();
  virtual void PopClipping();
  virtual void Clip(const nuiRect& rRect);
  virtual void ResetClipRect();
  virtual void EnableClipping(bool set);
  virtual bool GetClipRect(nuiRect& rRect, bool LocalRect) const;

  virtual uint32 GetClipStackSize() const;

  virtual uint32 GetRectangleTextureSupport() const;

  void GetSize(uint32& rX, uint32& rY) const;
  const nuiRenderState& GetState() const;

  void SetDummyMode(bool set);
  bool GetDummyMode() const;

  // Stats:
  void ResetStats();
  uint32 GetRenderOperations() const;
  uint32 GetVertices() const;
  uint32 GetBatches() const;

  // Display rotation
  void SetAngle(int32 Angle);
  int32 GetAngle() const;
  

  // Debug:
  virtual void AddBreakPoint();
  
  void DEBUG_EnableDrawArray(bool set) const;
  bool DEBUG_GetEnableDrawArray() const;
  
  virtual void SetSurface(nuiSurface* pSurface);
  virtual nuiSurface* GetSurface() const;

  virtual void DestroySurface(nuiSurface* pSurface) = 0;
  virtual void ResizeSurface(nuiSurface* pSurface, int32 width, int32 height) = 0;

protected:
  nuiSurface* mpSurface;

  nuiRenderState mState;
  std::stack<nuiClipper> mpClippingStack;
  uint32 mWidth;
  uint32 mHeight;
  uint32 mAngle;

  nuiClipper mClip;
  std::stack<nuiMatrix> mMatrixStack;
  std::stack<nuiMatrix> mProjectionMatrixStack;
  std::stack<nuiRect> mProjectionViewportStack;

  uint32 mRenderOperations;
  uint32 mVertices;
  uint32 mBatches;

  bool mDummyMode;

  mutable bool mEnableDrawArray;
  static uint32 mNeedTextureBackingStore;
  void AddNeedTextureBackingStore();
  void DelNeedTextureBackingStore();

  int32 GetCurrentWidth() const;
  int32 GetCurrentHeight() const;
};

#endif
