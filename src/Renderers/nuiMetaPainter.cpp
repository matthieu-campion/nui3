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
  mNbClearColor = 0;
  mNbOperations = 0;
  mLastSize = -1;
}

nuiMetaPainter::~nuiMetaPainter()
{
  Reset(NULL);
}

void nuiMetaPainter::StoreOpCode(OpCode code)
{
  StoreInt((int32)code);
  mNbOperations++;
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

nuiMetaPainter::OpCode nuiMetaPainter::FetchOpCode() const
{
  return (OpCode)FetchInt();
}

int32 nuiMetaPainter::FetchInt() const
{
  int32 tmp;
  FetchBuffer(&tmp, sizeof(int32), 1);
  return tmp;
}

void nuiMetaPainter::FetchFloat(double& rDouble) const
{
  FetchBuffer(&rDouble, sizeof(double), 1);
}

void nuiMetaPainter::FetchFloat(float& rFloat) const
{
  FetchBuffer(&rFloat, sizeof(float), 1);
}

void* nuiMetaPainter::FetchPointer() const
{
  void* pTmp;
  FetchBuffer(&pTmp, sizeof(void*), 1);
  return pTmp;
}

void nuiMetaPainter::FetchBuffer(void* pBuffer, uint ElementSize, uint ElementCount) const
{
  uint size = ElementSize * ElementCount;
  memcpy(pBuffer, &mOperations[mOperationPos], size);
  mOperationPos += size;
}

// Rendering operations:
void nuiMetaPainter::StartRendering()
{
  StoreOpCode(eStartRendering);

  nuiPainter::StartRendering();
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

void nuiMetaPainter::DrawArray(nuiRenderArray* pRenderArray)
{
  if (mDummyMode)
    return;
  StoreOpCode(eDrawArray);
  StorePointer(pRenderArray);

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














void nuiMetaPainter::LoadProjectionMatrix(const nuiRect& rViewport, const nuiMatrix& rMatrix)
{
  StoreOpCode(eLoadProjectionMatrix);
  StoreFloat(rViewport.Left());
  StoreFloat(rViewport.Top());
  StoreFloat(rViewport.GetWidth());
  StoreFloat(rViewport.GetHeight());
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);  
  
  nuiPainter::LoadProjectionMatrix(rViewport, rMatrix);
}

void nuiMetaPainter::MultProjectionMatrix(const nuiMatrix& rMatrix)
{
  StoreOpCode(eMultProjectionMatrix);
  StoreBuffer(rMatrix.Array, sizeof(nuiSize), 16);
  
  nuiPainter::MultProjectionMatrix(rMatrix);
}

void nuiMetaPainter::PushProjectionMatrix()
{
  StoreOpCode(ePushProjectionMatrix);
  
  nuiPainter::PushProjectionMatrix();
}

void nuiMetaPainter::PopProjectionMatrix()
{
  StoreOpCode(ePopProjectionMatrix);
  
  nuiPainter::PopProjectionMatrix();
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

//bool nuiMetaPainter::GetClipRect(nuiRect& rRect, bool LocalRect);


void nuiMetaPainter::ReDraw(nuiDrawContext* pContext)
{
  PartialReDraw(pContext, 0, mNbOperations);
}

void nuiMetaPainter::Reset(nuiPainter const * pFrom)
{
  mOperationIndices.clear();
  mLastSize = -1;
  mOperationPos = 0;
  mLastStateValid = false;
  mNbDrawChild = 0;
  mNbDrawArray = 0;
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
      ((nuiRenderArray*)FetchPointer())->Release();
      break;
    case eClearColor:
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

    
    case eLoadProjectionMatrix:
      {
        nuiMatrix m;
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
      break;
    case eMultProjectionMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
      break;
    case ePopProjectionMatrix:
      break;
    case ePushProjectionMatrix:
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
    pFrom->GetSize(mWidth, mHeight);
    bool clip = pFrom->GetClipRect(mClip, false);
    
    nuiMatrix m(pFrom->GetMatrix());
    mClip.Move(-m.Elt.M14, -m.Elt.M24);
    mClip.mEnabled = clip;
//    mMatrixStack.top() = pFrom->GetMatrix();
  }
}


int32 nuiMetaPainter::GetNbOperations() const
{
  return mNbOperations;
}

void nuiMetaPainter::PartialReDraw(nuiDrawContext* pContext, int32 first, int32 last) const
{
  nuiPainter* pPainter = pContext->GetPainter();
  mOperationPos = 0;
  uint size = mOperations.size();
  
  const bool DoDrawChild = mNbDrawChild;
  const bool DoDrawArray = mNbDrawArray;
  const bool DoDrawSelf = DoDrawArray || mNbClearColor;
  if (!(DoDrawChild || DoDrawSelf))
    return;
  
  int32 currentop = 0;
  while (mOperationPos < size && currentop < last)
  {
    bool draw = currentop >= first;
    
    OpCode code = FetchOpCode();
    switch (code)
    {
      case eSetSize:
        if (draw)
          pPainter->SetSize(FetchInt(), FetchInt());
        break;
      case eStartRendering:
        if (draw)
        {
          pPainter->StartRendering();
        }
        break;
      case eSetState:
        if (draw)
        {
          nuiRenderState* pState = ((nuiRenderState*)FetchPointer());
          bool ForceApply = FetchInt() ? true : false;
          if (DoDrawSelf)
            pPainter->SetState(*pState, ForceApply);
        }
        break;
      case eDrawArray:
        if (draw)
        {
          nuiRenderArray* pRenderArray = (nuiRenderArray*) FetchPointer();
          pRenderArray->Acquire(); // Pre acquire the render array as the painter will release it
          pPainter->DrawArray(pRenderArray);
        }
        break;
      case eClearColor:
        if (draw)
          pPainter->ClearColor();
        break;
      case eBeginSession:
        if (draw)
          if (DoDrawSelf)
            pPainter->BeginSession();
        break;
      case eEndSession:
        if (draw)
          if (DoDrawSelf)
            pPainter->EndSession();
        break;
      case eDrawChild:
        if (draw)
        {
          nuiWidget* pChild = (nuiWidget*)FetchPointer();
          pChild->DrawWidget(pContext);
        }
        break;
      case eLoadMatrix:
        if (draw)
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          pPainter->LoadMatrix(m);
        }
        break;
      case eMultMatrix:
        if (draw)
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          pPainter->MultMatrix(m);
        }
        break;
      case ePopMatrix:
        if (draw)
          pPainter->PopMatrix();
        break;
      case ePushMatrix:
        if (draw)
          pPainter->PushMatrix();
        break;
        
        
        
        
        
      case eLoadProjectionMatrix:
        if (draw)
        {
          nuiMatrix m;
          float Left, Top, Width, Height;
          FetchFloat(Left);
          FetchFloat(Top);
          FetchFloat(Width);
          FetchFloat(Height);
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          pPainter->LoadProjectionMatrix(nuiRect(Left, Top, Width, Height), m);
        }
        break;
      case eMultProjectionMatrix:
        if (draw)
        {
          nuiMatrix m;
          FetchBuffer(m.Array, sizeof(nuiSize), 16);
          pPainter->MultProjectionMatrix(m);
        }
        break;
      case ePopProjectionMatrix:
        if (draw)
          pPainter->PopProjectionMatrix();
        break;
      case ePushProjectionMatrix:
        if (draw)
          pPainter->PushProjectionMatrix();
        break;
        
        
        
        
        
        
      case ePushClipping:
        if (draw)
          pPainter->PushClipping();
        break;
      case ePopClipping:
        if (draw)
          pPainter->PopClipping();
        break;
      case eClip:
        if (draw)
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
        if (draw)
          pPainter->EnableClipping(FetchInt()?true:false);
        break;
      case eResetClipRect:
        if (draw)
          pPainter->ResetClipRect();
        break;
      case eBreak:
        if (draw)
          NGL_ASSERT(false);
        break;
      default:
        if (draw)
          NGL_ASSERT(false);
        break;
    }
    
    currentop++;
  }
}

nglString nuiMetaPainter::GetOperationDescription(int32 OperationIndex) const
{
  nglString str;
  int32 mOperationPos = GetOffsetFromOperationIndex(OperationIndex);
  
  OpCode code = FetchOpCode();
  switch (code)
  {
    case eSetSize:
      FetchInt();
      FetchInt();
      str = _T("SetSize");
      break;
    case eStartRendering:
      {
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
      }
      str = _T("StartRendering");
      break;
    case eSetState:
      FetchPointer();
      FetchInt();
      str = _T("SetState");
      break;
    case eDrawArray:
      {
        nuiRenderArray* pArray = (nuiRenderArray*)FetchPointer();
        str.CFormat(_T("DrawArray 0x%x"), pArray);
      }
      break;
    case eClearColor:
      str = _T("ClearColor");
      break;
    case eBeginSession:
      str = _T("BeginSession");
      break;
    case eEndSession:
      str = _T("EndSession");
      break;
    case eDrawChild:
      {
        nuiWidget* pS = (nuiWidget*)FetchPointer();
        str.CFormat(_T("DrawChild 0x%x / '%ls'"), pS, pS->GetObjectName().GetChars());
      }
      break;
    case eLoadMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = _T("LoadMatrix") + v;
      }
      break;
    case eMultMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = _T("MultMatrix") + v;
      }
      break;
    case ePopMatrix:
      str = _T("PopMatrix");
      break;
    case ePushMatrix:
      str = _T("PushMatrix");
      break;
      
      
    case eLoadProjectionMatrix:
      {
        nuiMatrix m;
        nuiSize a, b, c, d;
        FetchFloat(a);
        FetchFloat(b);
        FetchFloat(c);
        FetchFloat(d);
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str.CFormat(_T("LoadProjectionMatrix(%f, %f, %f, %f) / %ls"), a, b, d, c, v.GetChars());
      }
      break;
    case eMultProjectionMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
        nglString v;
        m.GetValue(v);
        str = _T("MultProjectionMatrix") + v;
      }
      break;
    case ePopProjectionMatrix:
      str = _T("PopProjectionMatrix");
      break;
    case ePushProjectionMatrix:
      str = _T("PushProjectionMatrix");
      break;
      
      
      
      
    case ePushClipping:
      str = _T("PushClipping");
      break;
    case ePopClipping:
      str = _T("PopClipping");
      break;
    case eClip:
      {
        nuiSize a, b, c, d;
        FetchFloat(a);
        FetchFloat(b);
        FetchFloat(c);
        FetchFloat(d);
        str.CFormat(_T("Clip(%f, %f, %f, %f)"), a, b, d, c);
      }
      break;
    case eResetClipRect:
      str = _T("ResetClipRect");
      break;
    case eEnableClipping:
      str.CFormat(_T("EnableClipping(%d)"), FetchInt());
      break;
    case eBreak:
      str = _T("Break");
      break;
    default:
      str = _T("Unknown operation");
      break;
  }
  
  return nglString().Add(OperationIndex).Add(_T(": ")).Add(str);
}

void nuiMetaPainter::SetName(const nglString& rName)
{
  mName = rName;
}

const nglString& nuiMetaPainter::GetName() const
{
  return mName;
}

int32 nuiMetaPainter::GetOffsetFromOperationIndex(int32 index) const
{
  UpdateIndices();
  return mOperationIndices[index];
}


void nuiMetaPainter::UpdateIndices() const
{
  if (!mOperationIndices.empty()) // || mLastSize == mOperations.size())
    return;
  
  mOperationIndices.clear();
  mOperationPos = 0;
  uint size = mOperations.size();
  mLastSize = size;
  while (mOperationPos < size)
  {
    mOperationIndices.push_back(mOperationPos);
    OpCode code = FetchOpCode();
    switch (code)
    {
      case eSetSize:
        FetchInt();
        FetchInt();
        break;
      case eStartRendering:
        break;
      case eSetState:
        FetchPointer();
        FetchInt();
        break;
      case eDrawArray:
        FetchPointer();
        break;
      case eClearColor:
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
        
        
      case eLoadProjectionMatrix:
      {
        nuiMatrix m;
        nuiSize tmp;
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchFloat(tmp);
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case eMultProjectionMatrix:
      {
        nuiMatrix m;
        FetchBuffer(m.Array, sizeof(nuiSize), 16);
      }
        break;
      case ePopProjectionMatrix:
        break;
      case ePushProjectionMatrix:
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
}

