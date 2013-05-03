/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiMetaPainter_h__
#define __nuiMetaPainter_h__

#include "nuiRenderState.h"
#include "nuiRenderArray.h"
#include "nuiRect.h"
#include "nuiPainter.h"

class nuiDrawContext;

typedef std::vector<uint8> nuiRenderCache;

// 
class NUI_API nuiMetaPainter : public nuiPainter
{
public:
  nuiMetaPainter(nglContext* pContext = NULL);
  virtual ~nuiMetaPainter();

  virtual void SetSize(uint32 sizex, uint32 sizey);
  virtual void StartRendering();
  virtual void SetState(const nuiRenderState& rState, bool ForceApply = false);
  virtual void DrawArray(nuiRenderArray* pArray);
  virtual void Clear(bool color, bool depth, bool stencil);
  virtual void BeginSession();
  virtual void EndSession();
  virtual void LoadMatrix(const nuiMatrix& rMatrix);
  virtual void MultMatrix(const nuiMatrix& rMatrix);
  virtual void PushMatrix();
  virtual void PopMatrix();
  virtual void LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix);
  virtual void MultProjectionMatrix(const nuiMatrix& rMatrix);
  virtual void PushProjectionMatrix();
  virtual void PopProjectionMatrix();
  
  virtual void PushClipping();
  virtual void PopClipping();
  virtual void Clip(const nuiRect& rRect);
  virtual void ResetClipRect();
  virtual void EnableClipping(bool set);
//  virtual bool GetClipRect(nuiRect& rRect, bool LocalRect);
  
  void DrawChild(nuiDrawContext* pContext, nuiWidget* pChild);

  /** @name Render operation storage management */
  //@{
  void UseRenderBuffer(nuiRenderCache* pCache)
  {
    if (pCache)
      mpCache = pCache;
    else
      mpCache = &mOperations;
  }
  //@}

  void ReDraw(nuiDrawContext* pContext);
  void Reset(nuiPainter const * pFrom);

  // Debugging and profiling
  virtual void AddBreakPoint();
  int32 GetNbOperations() const;
  void PartialReDraw(nuiDrawContext* pContext, int32 first, int32 last) const;
  nglString GetOperationDescription(int32 OperationIndex) const;
  nglString GetNextDescription() const;
  void SetName(const nglString& rName);
  const nglString& GetName() const;
  
  void SetDrawChildrenImmediat(bool set);
  bool GetDrawChildrenImmediat() const;

  virtual void DestroySurface(nuiSurface* pSurface);
  virtual void ResizeSurface(nuiSurface* pSurface, int32 width, int32 height);

#ifdef _DEBUG_
  void DBGSetReferenceObject(const nuiObject* pRef);
#endif
protected:
  // Rendering Operations OpCodes:
  enum OpCode
  {
    eSetSize,
    eStartRendering,
    eSetState,
    eDrawArray,
    eClear,
    eBeginSession,
    eEndSession,
    eLoadMatrix,
    eMultMatrix,
    ePushMatrix,
    ePopMatrix,
    eLoadProjectionMatrix,
    eMultProjectionMatrix,
    ePushProjectionMatrix,
    ePopProjectionMatrix,
    
    ePushClipping,
    ePopClipping,
    eClip,
    eResetClipRect,
    eEnableClipping,

    eDrawChild,
    
    eBreak
  };


  nuiRenderCache mOperations;

  void StoreOpCode(OpCode code);
  void StoreInt(int32 Val);
  void StorePointer(void* pVal);
  void StoreBuffer(const void* pBuffer, uint ElementSize, uint ElementCount);
  void StoreFloat(float Val);
  void StoreFloat(double Val);

  OpCode FetchOpCode() const;
  int32 FetchInt() const;
  void* FetchPointer() const;
  void FetchBuffer(void* pBuffer, uint ElementSize, uint ElementCount) const;
  void FetchFloat(double& rDouble) const;
  void FetchFloat(float& rFloat) const;

  int32 GetOffsetFromOperationIndex(int32 index) const;
  
  nuiRenderCache* mpCache;
  nuiRenderState mLastState;
  bool mLastStateValid;

  int32 mNbDrawChild;
  int32 mNbDrawArray;
  int32 mNbClearColor;
  mutable int32 mOperationPos;
  int32 mNbOperations;
    
  nglString mName;
  mutable std::vector<int32> mOperationIndices;
  mutable int32 mLastSize;
  void UpdateIndices() const;
  bool mDrawChildrenImmediat;
  
#ifdef _DEBUG_
  const nuiObject* mpDebugObjectRef;
  nglString mDebugObjectName;
  nglString mDebugObjectClass;
#endif
  
  std::vector<nuiRenderState> mRenderStates;
  std::vector<nuiRenderArray*> mRenderArrays;
};

#endif // __nuiMetaPainter_h__
