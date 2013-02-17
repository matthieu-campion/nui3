/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiPane::nuiPane(const nuiColor& rFillColor, const nuiColor& rStrokeColor, nuiShapeMode ShapeMode, nuiBlendFunc BlendFunc)
 : nuiSimpleContainer()
{
  if (SetObjectClass(_T("nuiPane")))
  {
    AddAttribute(new nuiAttribute<const nuiColor&>
                 (nglString(_T("FillColor")), nuiUnitColor,
                  nuiMakeDelegate(this, &nuiPane::GetFillColor),
                  nuiMakeDelegate(this, &nuiPane::SetFillColor)));                
    
    AddAttribute(new nuiAttribute<const nuiColor&>
                 (nglString(_T("StrokeColor")), nuiUnitColor,
                  nuiMakeDelegate(this, &nuiPane::GetStrokeColor),
                  nuiMakeDelegate(this, &nuiPane::SetStrokeColor)));                
  }
  mInterceptMouse = false;
  
  mFillColor = rFillColor;
  mStrokeColor = rStrokeColor;
  
  mShapeMode = ShapeMode;
  mBlendFunc = BlendFunc;
  mLineWidth = 1;
  mCanRespectConstraint = true;

  NUI_ADD_EVENT(ClickedMouse);
  NUI_ADD_EVENT(UnclickedMouse);
  NUI_ADD_EVENT(MovedMouse);

  mpShape = NULL;
  SetCurve(0);
}


nuiPane::~nuiPane()
{
  delete mpShape;
}

bool nuiPane::Draw(nuiDrawContext* pContext)
{
  pContext->PushState();

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(mBlendFunc);

  pContext->SetFillColor(mFillColor);
  pContext->SetStrokeColor(mStrokeColor);
  pContext->EnableAntialiasing(true);
  pContext->SetLineWidth(mLineWidth);
  if (mCurve != 0)
  {
    pContext->DrawShape(mpShape, mShapeMode);
  }
  else
  {
    pContext->DrawRect(mRect.Size(), mShapeMode);
  }
  pContext->PopState();

  DrawChildren(pContext);
  return true;
}

bool nuiPane::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  ClickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiPane::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  UnclickedMouse(X,Y,Button);
  return mInterceptMouse;
}

bool nuiPane::MouseMoved  (nuiSize X, nuiSize Y)
{
  MovedMouse(X,Y);
  return mInterceptMouse;
}

void nuiPane::SetInterceptMouse(bool intercept)
{
  mInterceptMouse = intercept;
}

bool nuiPane::GetInterceptMouse()
{
  return mInterceptMouse;
}

bool nuiPane::SetRect(const nuiRect& rRect)
{
  bool reload = !(mRect == rRect);

  nuiRect r(rRect.Size());
  r.Grow((nuiSize)ToNearest(-mCurve * .5f), (nuiSize)ToNearest(-mCurve * .5f));
  r.RoundToNearest();

  nuiWidget::LayoutConstraint constraint(mConstraint);
  constraint.mMaxWidth -= (nuiSize)ToNearest(mCurve * .5f);
  constraint.mMaxHeight -= (nuiSize)ToNearest(mCurve * .5f);

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetLayoutConstraint(constraint);
    pItem->SetLayout(r);
  }
  delete pIt;

  nuiWidget::SetRect(rRect);

  if (reload)
    SetCurve(mCurve);

  return true;
}

void nuiPane::SetCurve(float curve)
{
  mCurve = curve;
  delete mpShape;

  if (curve != 0) 
  {
    mpShape = new nuiShape();
    mpShape->AddRoundRect(mRect.Size(), curve);
  }
}

void nuiPane::SetLineWidth(float Width)
{
  mLineWidth = Width;
  Invalidate();
}


void nuiPane::SetFillColor(const nuiColor& rFillColor)
{
  mFillColor = rFillColor;
  Invalidate();
}

void nuiPane::SetStrokeColor(const nuiColor& rStrokeColor)
{
  mStrokeColor = rStrokeColor;
  Invalidate();
}


const nuiColor& nuiPane::GetFillColor() const
{
  return mFillColor;
}

const nuiColor& nuiPane::GetStrokeColor() const
{
  return mStrokeColor;
}

void nuiPane::SetShapeMode(nuiShapeMode shapeMode)
{
  mShapeMode = shapeMode;
  Invalidate();
}

nuiRect nuiPane::CalcIdealSize()
{
  nuiRect temp;

  nuiWidget::LayoutConstraint constraint(mConstraint);
  constraint.mMaxWidth -= (nuiSize)ToNearest(mCurve * .5f);
  constraint.mMaxHeight -= (nuiSize)ToNearest(mCurve * .5f);

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetLayoutConstraint(constraint);
    nuiRect t = pItem->GetIdealRect();
    temp.Union(t,temp); // Dummy call. Only the side effect is important: the object recalculates its layout.
    //NGL_OUT(_T("    PaneItem rect %s\n"), t.GetValue().GetChars());
  }
  delete pIt;

  temp.Grow((nuiSize)ToNearest(mCurve * .5f), (nuiSize)ToNearest(mCurve * .5f));
  temp.RoundToBiggest();
  

  DebugRefreshInfo();

  temp.MoveTo(0,0);

  //NGL_OUT(_T("    Pane rect %s\n"), mIdealRect.GetValue().GetChars());
  return temp;
}
