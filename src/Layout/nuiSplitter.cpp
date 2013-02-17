/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/
#include "nui.h"

using namespace std;






//***************************************************************************************************
//
// nuiSplitterHandle
//
//***************************************************************************************************




nuiSplitterHandle::nuiSplitterHandle(nuiSplitter* pParent)
{
  SetObjectClass(_T("nuiSplitterHandle"));
  
  mpParent = pParent;
  mClicked = false;  
}

nuiSplitterHandle::~nuiSplitterHandle()
{
}



nuiSplitter* nuiSplitterHandle::GetSplitter()
{
  return mpParent;
}

// virtual 
bool nuiSplitterHandle::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    if (Button & nglMouseInfo::ButtonDoubleClick)
    {
      nuiSize minDiff = mpParent->mHandlePos - mpParent->mHandlePosMin;
      nuiSize maxDiff = mpParent->mHandlePosMax - mpParent->mHandlePos;
      
      if( minDiff < maxDiff)
        mpParent->GotoHandlePos(mpParent->mHandlePosMax);
      else
        mpParent->GotoHandlePos(mpParent->mHandlePosMin);
      
      return true;
    }

    mClickPos = (mpParent->mOrientation == nuiVertical)? X : Y;
    mClicked = true;
    Grab();
    
    Invalidate();
    return true;
  }
  return false;
}

// virtual 
bool nuiSplitterHandle::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button == nglMouseInfo::ButtonLeft)
  {
    if (mClicked)
    {
      mClicked = false;
      Ungrab();

      SetMouseCursor(eCursorArrow);

      Invalidate();
      return true;
    }
  }
  return false;

}


nuiWidgetPtr nuiSplitterHandle::DispatchMouseMove(const nglMouseInfo& rInfo)
{
  if (!mMouseEventEnabled || mTrashed)
    return NULL;

//  nuiWidgetPtr pHandled=NULL;
  bool res=false;
  bool hasgrab = HasGrab();
  bool inside=false;

  if (IsDisabled())
    return NULL;

  if (IsInsideFromParent(rInfo.X, rInfo.Y))
    inside = true;
  
  if (inside)
  {
    nuiMouseCursor cursorModel = (mpParent->mOrientation == nuiVertical) ? eCursorResizeWE : eCursorResizeNS;
    if (GetMouseCursor() != cursorModel)
      SetMouseCursor(cursorModel);
      
    if (!hasgrab)
      return this;
  }

  if (inside || hasgrab)
  {
    inside = true;

    nuiSize X = rInfo.X;
    nuiSize Y = rInfo.Y;
    mpParent->GlobalToLocal(X, Y);
    nglMouseInfo info(rInfo);
    info.X = X;
    info.Y = Y;
    if (PreMouseMoved(info))
      return this;
    nuiWidget* pThis = this;
    res = pThis->MouseMoved(info);
    res |= MovedMouse(info);
  }

  return (res && inside) ? this : NULL;
}




// virtual 
bool nuiSplitterHandle::MouseMoved(nuiSize X, nuiSize Y)
{
  nuiRect Rect = mpParent->mRect.Size();

  if (mpParent->mFixed)
    return false;

    if (!mClicked) 
      return true;


  if (mClicked)
  {
    if (mpParent->mOrientation == nuiVertical)
    {
      if (mpParent->mMode == eModePercentage)
        mpParent->SetHandlePos(100.0f * ((X - mClickPos) / mpParent->mRect.GetWidth()));
      else mpParent->SetHandlePos(X);
    }
    else
    {
      if (mpParent->mMode == eModePercentage)
        mpParent->SetHandlePos(100.0f * ((Y - mClickPos) / mpParent->mRect.GetHeight()));
      else mpParent->SetHandlePos(Y);
    }
    
    mpParent->UpdateLayout();
    return true;
  }

  return false;

}





//***************************************************************************************************************************





nuiSplitter::nuiSplitter ( nuiOrientation orientation, nuiSplitterMode mode)
: nuiSimpleContainer()
//  mSplitterSink(this)
{
  if (SetObjectClass(_T("nuiSplitter")))
    InitAttributes();

  mHandlePos = 50; // 50%
  mHandlePosMin = 10;
  mHandlePosMax = 90;
  mFirstWidgetMinPixels = 0;
  mSeconWidgetMinPixels = 0;
  mOrientation = orientation;
  mMode = mode;
  if (mode == eModePixel)
    mHandlePosMax = -mHandlePosMax;
    
  mpHandle = NULL;  
    
  mFixed = false;
  mHandleHover = false;
  mMasterChild = true;
  mStartHandlePos = 50;
  mEndHandlePos = 50;
  mGotoStep = 0;

  InitProperties();
  
  mpHandle = new nuiSplitterHandle(this);
  AddChild(mpHandle); 
}

void nuiSplitter::InitAttributes()
{
  AddAttribute(new nuiAttribute<nuiSize>
               (nglString(_T("HandlePos")), nuiUnitSize,
                nuiMakeDelegate(this, &nuiSplitter::GetHandlePos),
                nuiMakeDelegate(this, &nuiSplitter::SetHandlePos)));
}



nuiSplitter::~nuiSplitter()
{
}


// virtual 
void nuiSplitter::SetDecoration(nuiDecoration* pDecoration, nuiDecorationMode Mode)
{
  mpHandle->SetDecoration(pDecoration, Mode);
}


void nuiSplitter::InitProperties()
{
}




nuiRect nuiSplitter::CalcIdealSize()
{



  NGL_ASSERT(mpChildren.size() <= 3);

  nuiSize Height=0,Width=0;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();
  

  mPositions.clear();
  if (mOrientation == nuiHorizontal)
  {
    for (it=mpChildren.begin(); it!=end; ++it)
    {
      nuiRect Rect;
      nuiWidgetPtr pItem = (*it);
      Rect = pItem->GetIdealRect();

      Width = MAX(Width, Rect.GetWidth()+2);
      mPositions.push_back(Rect);

      Height+=Rect.GetHeight();
    }
  }
  else
  {
    for (it=mpChildren.begin(); it!=end; ++it)
    {
      nuiRect Rect;
      nuiWidgetPtr pItem = (*it);
      Rect = pItem->GetIdealRect();

      Height = MAX(Height, Rect.GetHeight()+2);
      mPositions.push_back(Rect);

      Width += Rect.GetWidth();
    }
  }

  mIdealRect.SetSize(Width, Height);
  mPositions.push_front(mIdealRect);
  return mIdealRect;
}



bool nuiSplitter::SetRect(const nuiRect& rRect)
{

  NGL_ASSERT(mpChildren.size() <= 3);

  if (mPositions.empty())
    GetIdealRect();

  nuiWidget::SetRect(rRect);

  if (mOrientation == nuiHorizontal)
    SetRectHorizontal(rRect);
  else
    SetRectVertical(rRect);

  if (!mFixed)
    mMasterChild = false;

  return true;
}



//*******************************************
//
// ORIENTATION HORIZONTAL
//
bool nuiSplitter::SetRectHorizontal(const nuiRect& rRect)
{
  nuiSize Height = rRect.GetHeight();
  nuiSize Width = rRect.GetWidth();
  nuiRect Rect;
  
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();

  list<nuiRect>::iterator rit = mPositions.begin();
  Rect = (*rit);

  ++rit;


  // First Child:
  nuiWidgetPtr pItem;
  Height = 0;
  // We only care about the first two widgets!
  int i;
  it = mpChildren.begin();
  i = 0;
  nuiSize h;
  
  // don't process the splitter handle before the first child (we need the size of the first child)
  if (it != end)
  {
    pItem = (*it);
    if (pItem == mpHandle)
      ++it;
  }


  if (it != end)
  {
    pItem = (*it);

    //*****************************************************
    //
    // MASTERCHILD TRUE
    if (mMasterChild)
    {
      h = pItem->GetIdealRect().GetHeight();

      mHandlePos = h;
      if (mMode == eModePercentage) 
      {
        mHandlePos = (mHandlePos / (rRect.GetHeight() * (mHandlePosMax / 100.0f))) * 100.0f; 
        h = mHandlePos;
      }
      
      Rect.Set(0.f, Height, Width, h);
      Rect.RoundToBelow();
    }

    // MASTERCHILD FALSE
    else
    {
      h = mHandlePos;

      if (mMode == eModePercentage) 
        h = mRect.GetHeight() * (h / 100.0f);

      h = MAX(h, mFirstWidgetMinPixels);

      if (h + mSeconWidgetMinPixels > mRect.GetHeight())
      {
        h = mRect.GetHeight() - mSeconWidgetMinPixels;

        mHandlePos = h;
        if (mMode == eModePercentage)
          mHandlePos = ((h / mRect.GetHeight()) * 100.f); 
      }

      Rect.Set(0.f, Height, Width, h);
      Rect.RoundToBelow();
    }

    Height += h;
    pItem->SetLayout(Rect);
  }


  //******************************************
  //
  // handle
  nuiSize handleSize =0;
  if (mpHandle)
  {
    const nuiRect& handleRect = mpHandle->GetIdealRect();
    handleSize = handleRect.GetHeight();

    Rect.Set(0.0f, Height, rRect.GetWidth(), handleSize);
    Rect.RoundToBelow();
    mpHandle->SetLayout(Rect);
    Height += handleSize;
  }

  if (it != end)
  {
    ++it;
    i++;
    if (it != end)
    {
      pItem = (*it);
      if (pItem == mpHandle)
      {
        ++it;
        i++;
      }
    }
  }




  //*****************************************************
  //
  // Second child:
  if (it != end)
  {
    pItem = (*it);

    if (mMasterChild)
    {
      h = mRect.GetHeight() - h ;
      NGL_ASSERT(h >= mSeconWidgetMinPixels);

      Rect.Set(0.f, Height + handleSize, Width, h - handleSize);
      Rect.RoundToBelow();
    }
    else
    {
      if (mMode == eModePercentage)
      {
        h = mRect.GetHeight() - ((mHandlePos/100.0f) * mRect.GetHeight()) - handleSize;
      }
      else
      {
        h = mRect.GetHeight() - mHandlePos - handleSize;
      }


      h = MAX(h, mSeconWidgetMinPixels);

      if (h + mFirstWidgetMinPixels > mRect.GetHeight())
      {
        h = mRect.GetHeight() - mFirstWidgetMinPixels;

        mHandlePos = mRect.GetHeight() - h;

        if (mMode == eModePercentage)
          mHandlePos = (mHandlePos * 100.0f) / mRect.GetHeight();
      }

      Rect.Set(0.f, Height, Width, h);
      Rect.RoundToBelow();
    }

    pItem->SetLayout(Rect);
  }

  return true;
}






//*******************************************
//
// ORIENTATION VERTICAL
//  
bool nuiSplitter::SetRectVertical(const nuiRect& rRect)
{
  nuiSize Height = rRect.GetHeight();
//  nuiSize Width = rRect.GetWidth();
  nuiSize left;
  nuiRect Rect;
  
  nuiSize handleSize = 0;
  if (mpHandle)
  {
    const nuiRect& handleRect = mpHandle->GetIdealRect();
    handleSize = handleRect.GetWidth();
  }
  
  nuiWidgetPtr pItem = NULL;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();

  list<nuiRect>::iterator rit=mPositions.begin();
  Rect=(*rit);

  ++rit;

  left = 0;
  nuiSize w=0;
  int i;
  it=mpChildren.begin();
  i=0;
  
   // don't process the splitter handle before the first child (we need the size of the first child)
  if (it != end)
  {
    pItem = (*it);
    if (pItem == mpHandle)
    {
      ++it;
      i++;
    }
  }



  //******************************************
  //
  // First Child:

  if (it != end)
  {
    pItem = (*it);

    if (mMasterChild)
    {
      w = pItem->GetIdealRect().GetWidth();

      mHandlePos = (mMode == eModePercentage) ? ((w / rRect.GetWidth()) * 100.0f) : w;

      Rect.Set(left, 0.f, w, Height);
      Rect.RoundToBelow();
    }
    else
    {
      w = (mMode == eModePercentage) ? MAX((mRect.GetWidth() * (mHandlePos/100.0f)), mFirstWidgetMinPixels) : MAX(mHandlePos, mFirstWidgetMinPixels);
      
      if (w + mSeconWidgetMinPixels > mRect.GetWidth())
      {

        w = mRect.GetWidth() - mSeconWidgetMinPixels;
        mHandlePos = (mMode == eModePercentage) ? (w / mRect.GetWidth() * 100.0f) : w;
      }


      Rect.Set(left, 0.f, w, Height);
      Rect.RoundToBelow();
    }

    pItem->SetLayout(Rect);

    left += w;
  }
  // no child
  else
  {
    if (mMasterChild)
    {
      left = (mMode == eModePercentage) ? ((mHandlePos / rRect.GetWidth()) * 100.0f) : mHandlePos;
    }
    else
    {
      left = (mMode == eModePercentage) ? MAX((mRect.GetWidth() * (mHandlePos/100.0f)), mFirstWidgetMinPixels) : MAX(mHandlePos, mFirstWidgetMinPixels);
    }
  }





  //******************************************
  //
  // handle
  if (mpHandle)
  {
    Rect.Set(left, 0.0f, handleSize, rRect.GetHeight());
    Rect.RoundToBelow();
    mpHandle->SetLayout(Rect);
    left += handleSize;
  }

  if (it != end)
  {
    ++it;
    i++;
    if (it != end)
    {
      pItem = (*it);
      if (pItem == mpHandle)
      {
        ++it;
        i++;
      }
    }
  }


  //******************************************
  //
  // second child
  if (it !=end)
  {
    w = mRect.GetWidth() - left;
    pItem = (*it);
    if (mMasterChild)
    {

      if (w < 0)
        w = 0;

      Rect.Set(left, 0.f, w, Height);
      Rect.RoundToBelow();
    }
    else
    {
      if (w + mFirstWidgetMinPixels > mRect.GetWidth())
      {
        w = mRect.GetWidth() - mFirstWidgetMinPixels;
        mHandlePos = (mMode == eModePercentage) ? (100.0f - (w / mRect.GetWidth()) * 100.0f) : (mRect.GetWidth() - w);
      }
      
      Rect.Set(left, 0.f, w, Height);
      Rect.RoundToBelow();
    }


    pItem->SetLayout(Rect);
  }
    


  return true;
}






bool nuiSplitter::GetHandleHover() const
{
  return mHandleHover;
}


nuiSplitterHandle* nuiSplitter::GetHandle() const
{
  return mpHandle;
}


nuiSize nuiSplitter::GetHandlePos()
{
  return mHandlePos;
}

void nuiSplitter::SetHandlePos(nuiSize HandlePos)
{
  if (mMode == eModePercentage)
  {
    if (HandlePos > mHandlePosMax)
      HandlePos = mHandlePosMax;
    if (HandlePos < mHandlePosMin)
      HandlePos = mHandlePosMin;
    
    if (mHandlePos == HandlePos)
      return;
    mHandlePos = HandlePos;
    HandleMoved();
    UpdateLayout();
  }
  else /// In pixel mode the max pos can be relative to the end of the widget
  {
    nuiSize PosMax = mHandlePosMax;
    
    if (mHandlePosMax < 0)
    {
      if (mOrientation == nuiHorizontal)
        PosMax = MAX(0, mRect.GetHeight() + mHandlePosMax);
      else
        PosMax = MAX(0, mRect.GetWidth() + mHandlePosMax);
      
    }

    if (HandlePos > PosMax)
      HandlePos = PosMax;

    if (HandlePos < mHandlePosMin)
      HandlePos = mHandlePosMin;
    
    if (mHandlePos == HandlePos)
      return;
    mHandlePos = HandlePos;
    HandleMoved();
    UpdateLayout();
  }
}

nuiSize nuiSplitter::GetHandlePosMin()
{
  if (mFirstWidgetMinPixels)
  {
    nuiSize handleSize = 0;
    if (mpHandle)
    {
      const nuiRect& handleRect = mpHandle->GetIdealRect();
      handleSize = (mOrientation == nuiVertical)? handleRect.GetWidth() : handleRect.GetHeight();
    }
  
    nuiSize size = mOrientation == nuiVertical ? GetRect().GetWidth() : GetRect().GetHeight();
    nuiSize handlePosMin = (mMode == eModePercentage) ?
      (((mFirstWidgetMinPixels + handleSize/2.0f) / size) * 100.0f) : 
    (mFirstWidgetMinPixels + handleSize/2.0f);
    return MAX(handlePosMin, mHandlePosMin);
  }
  return mHandlePosMin;
}

void nuiSplitter::SetHandlePosMin(nuiSize HandlePosMin)
{
  if (mMode == eModePercentage)
    if (HandlePosMin>100.0f)
      HandlePosMin=100.0f;

  if (HandlePosMin<0.0f)
    HandlePosMin=0.0f;

  mHandlePosMin = HandlePosMin;
  
  if (mHandlePos < HandlePosMin)
  {
      SetHandlePos(HandlePosMin);
  }
}

nuiSize nuiSplitter::GetHandlePosMax()
{
  if (mSeconWidgetMinPixels)
  {
    nuiSize handleSize = 0;
    if (mpHandle)
    {
      const nuiRect& handleRect = mpHandle->GetIdealRect();
      handleSize = (mOrientation == nuiVertical)? handleRect.GetWidth() : handleRect.GetHeight();
    }

    nuiSize size = mOrientation == nuiVertical ? GetRect().GetWidth() : GetRect().GetHeight();
    nuiSize handlePosMax = (mMode == eModePercentage) ?
      (((size - mSeconWidgetMinPixels - handleSize/2.0f) / size) * 100.0f) : 
    (size - mSeconWidgetMinPixels - handleSize/2.0f);

    return MIN(handlePosMax, mHandlePosMax);
  }
  return mHandlePosMax;
}

void nuiSplitter::SetHandlePosMax(nuiSize HandlePosMax)
{
  if (mMode == eModePercentage)
    if (HandlePosMax>100.0f)
      HandlePosMax=100.0f;

  if (HandlePosMax<0.0f)
    HandlePosMax=0.0f;
  
  mHandlePosMax = HandlePosMax;
  
  if (mHandlePos > HandlePosMax)
  {
      SetHandlePos(HandlePosMax);
  }
}

nuiSize nuiSplitter::GetFirstWidgetMinPixels()
{
  return mFirstWidgetMinPixels;
}

void nuiSplitter::SetFirstWidgetMinPixels(nuiSize MinPixels)
{
  mFirstWidgetMinPixels = MinPixels;
  UpdateLayout();
}

nuiSize nuiSplitter::GetSeconWidgetMinPixels()
{
  return mSeconWidgetMinPixels;
}

void nuiSplitter::SetSeconWidgetMinPixels(nuiSize MinPixels)
{
  mSeconWidgetMinPixels = MinPixels;
  UpdateLayout();
}


void nuiSplitter::GotoHandlePos(nuiSize HandlePos, nuiSize handleStep, double timerStep)
{
  mGotoStep = handleStep;
  if (mMode == eModePixel) mGotoStep *= 12;

  mStartHandlePos = GetHandlePos();

  if(HandlePos < GetHandlePosMin())
  {
    mEndHandlePos = GetHandlePosMin();
  }
  else if(HandlePos > GetHandlePosMax())
  {
    mEndHandlePos = GetHandlePosMax();
  }
  else
  {
    mEndHandlePos = HandlePos;
  }
  
  nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
  pAnim->SetTargetObject(this);
  pAnim->SetTargetAttribute(_T("HandlePos"));
  pAnim->SetDeleteOnStop(true); 
  pAnim->SetEasing(nuiEasingSinusStartFast);
  pAnim->SetStartValue(mStartHandlePos);
  pAnim->SetEndValue(mEndHandlePos);
  
  pAnim->Play();
  


//  mSplitterSink.Connect(nuiAnimation::AcquireTimer()->Tick, &nuiSplitter::StepHandlePos);
}

nuiSize nuiSplitter::GetHandleSize()
{
  nuiSize handleSize = 0;
  if (mpHandle)
  {
    const nuiRect& handleRect = mpHandle->GetIdealRect();
    handleSize = (mOrientation == nuiVertical)? handleRect.GetWidth() : handleRect.GetHeight();
  }

  return handleSize;
}



void nuiSplitter::SetOrientation(nuiOrientation orientation)
{
  mOrientation = orientation;
  InvalidateLayout();
}

nuiOrientation nuiSplitter::GetOrientation()
{
  return mOrientation;
}

void nuiSplitter::SetMode (nuiSplitterMode mode)
{
  mMode = mode;
}

nuiSplitterMode nuiSplitter::GetMode ()
{
  return mMode;
}



void nuiSplitter::SetFixed(bool flag)
{
  if (mFixed == flag)
    return;
  mFixed = flag;
  Invalidate();
}

bool nuiSplitter::GetFixed()
{
  return mFixed;
}


//void nuiSplitter::StepHandlePos(const nuiEvent& rEvent)
//{
//  if(mStartHandlePos > mEndHandlePos)
//  {
//    if(GetHandlePos() > mEndHandlePos)
//    {
//      nuiSize newPos = ((GetHandlePos() - mGotoStep) > mEndHandlePos) ? (GetHandlePos() - mGotoStep) : mEndHandlePos;
//      SetHandlePos(newPos);
//    }
//    else
//    {
////      mSplitterSink.Disconnect(&nuiSplitter::StepHandlePos);
//      GoToHandlePosAnimDone();
//    }
//  }
//  else
//  {
//    if(GetHandlePos() < mEndHandlePos)
//    {
//      nuiSize newPos = ((GetHandlePos() + mGotoStep) < mEndHandlePos) ? (GetHandlePos() + mGotoStep) : mEndHandlePos;
//      SetHandlePos(newPos);
//    }
//    else
//    {
////      mSplitterSink.Disconnect(&nuiSplitter::StepHandlePos);
//      GoToHandlePosAnimDone();
//    }
//  } 
//}

void nuiSplitter::SetMasterChild(bool MasterChild)
{
  mMasterChild = MasterChild;
  if (mFixed)
    Invalidate();
}

bool nuiSplitter::GetMasterChild()
{
  return mMasterChild;
}
