/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiMetaPainter.h"
#include "nuiDrawContext.h"

// nuiMetaPainter:
nuiMetaPainter::nuiMetaPainter(const nuiRect& rRect, nglContext* pContext) 
: nuiPainter(rRect, pContext),
  mVertices(GL_POINTS)
{
  mLastStateValid = false;
  mpCache = &mOperations;
  mNbDrawChild = 0;
  mNbDrawArray = 0;
  mNbClearStencil = 0;
  mNbClearColor = 0;
}

nuiMetaPainter::~nuiMetaPainter()
{
  Reset(NULL);
}

void nuiMetaPainter::StoreOpCode(OpCode code)
{
  StoreInt((int32)code);
}

void nuiMetaPainter::StoreInt(int32 Val)
{
  StoreBuffer(&Val, sizeof(int32), 1);
}

void nuiMetaPainter::StoreFloat(float Val)
{
  StoreBuffer(&Val, sizeof(float), 1);
}

void nuiMetaPainter::StoreFloat(double Val)
{
  StoreBuffer(&Val, sizeof(double), 1);
}

void nuiMetaPainter::StorePointer(void* pVal)
{
  StoreBuffer(&pVal, sizeof(void*), 1);
}

void nuiMetaPainter::StoreBuffer(const void* pBuffer, uint ElementSize, uint ElementCount)
{
  if (mDummyMode)
    return;

  uint size = ElementSize * ElementCount;
  uint pos = mOperations.size();
  mOperations.resize(pos + size);
  memcpy(&mOperations[pos], pBuffer, size);
}

nuiMetaPainter::OpCode nuiMetaPainter::FetchOpCode()
{
  return (OpCode)FetchInt();
}

int32 nuiMetaPainter::FetchInt()
{
  int32 tmp;
  FetchBuffer(&tmp, sizeof(int32), 1);
  return tmp;
}

void nuiMetaPainter::FetchFloat(double& rDouble)
{
  FetchBuffer(&rDouble, sizeof(double), 1);
}

void nuiMetaPainter::FetchFloat(float& rFloat)
{
  FetchBuffer(&rFloat, sizeof(float), 1);
}

void* nuiMetaPainter::FetchPointer()
{
  void* pTmp;
  FetchBuffer(&pTmp, sizeof(void*), 1);
  return pTmp;
}

void nuiMetaPainter::FetchBuffer(void* pBuffer, uint ElementSize, uint ElementCount)
{
  uint size = ElementSize * ElementCount;
  memcpy(pBuffer, &mOperations[mOperationPos], size);
  mOperationPos += size;
}

// Rendering operations:
void nuiMetaPainter::StartRendering(nuiSize ClipOffsetX, nuiSize ClipOffsetY)
{
  StoreOpCode(eStartRendering);
  StoreFloat(ClipOffsetX);
  StoreFloat(ClipOffsetY);

  nuiPainter::StartRendering(ClipOffsetX, ClipOffsetY);
}

void nuiMetaPainter::BeginSession()
{
  StoreOpCode(eBeginSession);
}

void nuiMetaPainter::EndSession()
{
  StoreOpCode(eEndSession);
}

#if NUI_METAPAINTER_STATS
static int StateChanges = 0;
static int IgnoredStateChanges = 0;
#endif

void nuiMetaPainter::SetState(const nuiRenderState& rState, bool ForceApply)
{
  if (mDummyMode)
    return;
  
#if NUI_METAPAINTER_STATS
  StateChanges++;
#endif
  
  if (mLastStateValid && rState == mLastState)
  {
#if NUI_METAPAINTER_STATS
    IgnoredStateChanges++;
    printf("State changes %d (ignored: %d)\n", StateChanges, IgnoredStateChanges);
#endif
    return;
  }
  
  mLastStateValid = true;
  mLastState = rState;
  StoreOpCode(eSetState);
  StorePointer(new nuiRenderState(rState));
  StoreInt(ForceApply?1:0);
}

void nuiMetaPainter::ClearColor()
{
  StoreOpCode(eClearColor);
  mNbClearColor++;
}

void nuiMetaPainter::ClearStencil(uint8 value)
{
  StoreOpCode(eClearStencil);
  StoreInt((int32)value);
  mNbClearStencil++;
}

void nuiMetaPainter::DrawArray(const nuiRenderArray& rRenderArray)
{
  if (mDummyMode)
    return;
  StoreOpCode(eDrawArray);
  StorePointer(new nuiRenderArray(rRenderArray));

  mNbDrawArray++;
  
/*
  uint start = mVertices.size();
  uint count = rRenderArray.GetSize();
  
  mVertices.AddArray(rRenderArray);
  
  StoreInt(start);
  StoreInt(count);
*/
}

void nuiMetaPainter::SetSize(uint32 w, uint32 h)
{
  StoreOpCode(eSetSize);
  StoreInt(w);
  StoreInt(h);

  mWidth = w;
  mHeight = h;
}

void nuiMetaPainter::DrawChild(nuiWidget* pChild)
{
  mLastStateValid = false;
  StoreOpCode(eDrawChild);
  StorePointer(pChild);
  mNbDrawChild++;
}

void nuiMetaPainter::LoadMatrix(const nuiMatrix& rMatrix)
{
  StoreOpCode(eLoadMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);

  nuiPainter::LoadMatrix(rMatrix);
}

void nuiMetaPainter::MultMatrix(const nuiMatrix& rMatrix)
{
  StoreOpCode(eMultMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);

  nuiPainter::MultMatrix(rMatrix);
}

void nuiMetaPainter::PushMatrix()
{
  StoreOpCode(ePushMatrix);

  nuiPainter::PushMatrix();
}

void nuiMetaPainter::PopMatrix()
{
  StoreOpCode(ePopMatrix);

  nuiPainter::PopMatrix();
}

void nuiMetaPainter::PushClipping()
{
  StoreOpCode(ePushClipping);

  nuiPainter::PushClipping();
}

void nuiMetaPainter::PopClipping()
{
  StoreOpCode(ePopClipping);

  nuiPainter::PopClipping();
}

void nuiMetaPainter::Clip(const nuiRect& rRect)
{
  mLastStateValid = false;
  StoreOpCode(eClip);
  StoreFloat(rRect.mLeft);
  StoreFloat(rRect.mTop);
  StoreFloat(rRect.mRight);
  StoreFloat(rRect.mBottom);

  nuiPainter::Clip(rRect);
}

void nuiMetaPainter::ResetClipRect()
{
  mLastStateValid = false;
  StoreOpCode(eResetClipRect);

  nuiPainter::ResetClipRect();
}

void nuiMetaPainter::EnableClipping(bool set)
{
  mLastStateValid = false;
  StoreOpCode(eEnableClipping);
  StoreInt(set?1:0);

  nuiPainter::EnableClipping(set);
}


void nuiMetaPainter::AddBreakPoint()
{
  StoreOpCode(eBreak);
}

//bool nuiMetaPainter::GetClipRect(nuiRect& rRect);


void nuiMetaPainter::ReDraw(nuiDrawContext* pContext)
{
  nuiPainter* pPainter = pContext->GetPainter();
  mOperationPos = 0;
  uint size = mOperations.size();
  
  const bool DoDrawChild = mNbDrawChild;
  const bool DoDrawArray = mNbDrawArray;
  const bool DoDrawSelf = DoDrawArray || mNbClearStencil || mNbClearColor;
  if (!(DoDrawChild || DoDrawSelf))
    return;
  
  while (mOperationPos < size)
  {
    OpCode code = FetchOpCode();
    switch (code)
    {
    case eSetSize:
      pPainter->SetSize(FetchInt(), FetchInt());
      break;
    case eStartRendering:
      {
        nuiSize a, b;
        FetchFloat(a);
        FetchFloat(b);
        pPainter->StartRendering(a, b);
      }
      break;
    case eSetState:
      {
        nuiRenderState* pState = ((nuiRenderState*)FetchPointer());
        bool ForceApply = FetchInt() ? true : false;
        if (DoDrawSelf)
          pPainter->SetState(*pState, ForceApply);
      }
      break;
    case eDrawArray:
      pPainter->DrawArray(*((nuiRenderArray*) FetchPointer()));
      break;
    case eClearColor:
      pPainter->ClearColor();
      break;
    case eClearStencil:
      pPainter->ClearStencil((uint32)FetchInt());
      break;
    case eBeginSession:
      if (DoDrawSelf)
        pPainter->BeginSession();
      break;
    case eEndSession:
      if (DoDrawSelf)
        pPainter->EndSession();
      break;
    case eDrawChild:
      {
        nuiWidget* pChild = (nuiWidget*)FetchPointer();
        pChild->DrawWidget(pContext);
      }
      break;
    case eLoadMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        pPainter->LoadMatrix(m);
      }
      break;
    case eMultMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        pPainter->MultMatrix(m);
      }
      break;
    case ePopMatrix:
      pPainter->PopMatrix();
      break;
    case ePushMatrix:
      pPainter->PushMatrix();
      break;
    case ePushClipping:
      pPainter->PushClipping();
      break;
    case ePopClipping:
      pPainter->PopClipping();
      break;
    case eClip:
      {
        nuiSize x;
        nuiSize y;
        nuiSize xx;
        nuiSize yy;
        FetchFloat(x);
        FetchFloat(y);
        FetchFloat(xx);
        FetchFloat(yy);
        const nuiRect r(x, y, xx, yy, false);
        pPainter->Clip(r);
      }
      break;
    case eEnableClipping:
      pPainter->EnableClipping(FetchInt()?true:false);
      break;
    case eResetClipRect:
      pPainter->ResetClipRect();
      break;
    case eBreak:
      NGL_ASSERT(false);
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
  }
}

void nuiMetaPainter::Reset(nuiPainter const * pFrom)
{
  mOperationPos = 0;
  mLastStateValid = false;
  mNbDrawChild = 0;
  mNbDrawArray = 0;
  mNbClearStencil = 0;
  mNbClearColor = 0;

  uint size = mOperations.size();
  while (mOperationPos < size)
  {
    OpCode code = FetchOpCode();
    switch (code)
    {
    case eSetSize:
      FetchInt();
      FetchInt();
      break;
    case eStartRendering:
      {
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
      }
      break;
    case eSetState:
      delete ((nuiRenderState*)FetchPointer());
      FetchInt();
      break;
    case eDrawArray:
      delete ((nuiRenderArray*)FetchPointer());
      break;
    case eClearColor:
      break;
    case eClearStencil:
      FetchInt();
      break;
    case eBeginSession:
      break;
    case eEndSession:
      break;
    case eDrawChild:
      FetchPointer();
      break;
    case eLoadMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
      break;
    case eMultMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
      break;
    case ePopMatrix:
      break;
    case ePushMatrix:
      break;
    case ePushClipping:
      break;
    case ePopClipping:
      break;
    case eClip:
      {
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
      }
      break;
    case eResetClipRect:
      break;
    case eEnableClipping:
      FetchInt();
      break;
    case eBreak:
      break;
    default:
      NGL_ASSERT(false);
      break;
    }
  }
  mOperations.clear();

  while (!mpClippingStack.empty())
  {
    delete mpClippingStack.top();
    mpClippingStack.pop();
  }

  while (!mMatrixStack.empty())
    mMatrixStack.pop();

  mMatrixStack.push(nuiMatrix());

  if (pFrom)
  {
    mState = pFrom->GetState();
    pFrom->GetClipOffset(mClipOffsetX, mClipOffsetY);
    pFrom->GetSize(mWidth, mHeight);
    bool clip = pFrom->GetClipRect(mClip);
    
    nuiMatrix m(pFrom->GetMatrix());
    mClip.Move(-m.Elt.M14, -m.Elt.M24);
    mClip.mEnabled = clip;
//    mMatrixStack.top() = pFrom->GetMatrix();
  }
}

