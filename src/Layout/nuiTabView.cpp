/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#include "nui.h"
#include "nuiTabView.h"
#include "nuiLabel.h"
#include "nuiShape.h"
#include "nuiContour.h"
#include "nuiGradient.h"
#include "nuiDrawContext.h"

/////////////////// nuiSimpleIcon //////////////////////////
#define ICON_DEFAULT_RADIUS  8.f
#define DEFAULT_FONT_SIZE   10.f

nuiSimpleIcon::nuiSimpleIcon(const nglString& rName, nuiPosition pos)
: nuiSimpleContainer(), mName(rName), mPosition(pos), mpGradient(NULL), mpSelectedGradient(NULL), mFontSize(DEFAULT_FONT_SIZE), mpFont(NULL), mpShape(NULL), mRadius(ICON_DEFAULT_RADIUS)
{
  SetObjectClass(_T("nuiSimpleIcon"));
  mOrientation = ( mPosition == nuiLeft || mPosition == nuiRight ? nuiVertical : nuiHorizontal );
  ResetGradient();
  ResetSelectedGradient();
  mpLabel = new nuiLabel(mName);
  mpLabel->SetPosition(nuiCenter);
  mpLabel->SetVMargin(6.f);
  mpLabel->SetOrientation(mOrientation);
  SetFontSize(mFontSize);
}

nuiSimpleIcon::~nuiSimpleIcon()
{
  if (mpGradient)
    delete mpGradient;
  if (mpSelectedGradient)
    delete mpSelectedGradient;
}

void nuiSimpleIcon::SetPosition(nuiPosition pos)
{
  if (mPosition == pos)
    return;
  mPosition = pos;
  mOrientation = ( mPosition == nuiLeft || mPosition == nuiRight ? nuiVertical : nuiHorizontal);
  mpLabel->SetOrientation(mOrientation);
  InvalidateLayout();
}

void nuiSimpleIcon::SetFontSize(const nuiSize& rFontSize)
{
  mFontSize = rFontSize;
  if (mpFont)
    mpFont->Release();
  mpLabel->SetFont(nuiFont::GetFont(mFontSize), true);
  InvalidateLayout();
}

void nuiSimpleIcon::SetColor(nuiWidgetElement elem, const nuiColor& rColor)
{
  nuiWidget::SetColor(elem, rColor);
  switch (elem)
  {
  case eNormalTabBg:
    ResetGradient();
    break;
  case eSelectedTabBg:
    ResetSelectedGradient();
    break;
  case eNormalTextFg:
    mpLabel->SetColor(elem, rColor);
  case eSelectedTextFg:
    mpLabel->SetColor(elem, rColor);
  default:
    break;
  }
  Invalidate();
}

void nuiSimpleIcon::ResetGradient()
{
  if (mpGradient)
    delete mpGradient;
  
  nuiColor tmp = GetColor(eNormalTabBg);

  float alpha = tmp.Alpha();

  mpGradient = new nuiGradient();
  tmp.Alpha() = alpha * .5f;
  tmp.Crop();
  mpGradient->AddStop(tmp, 0); 
  
  tmp.Alpha() = alpha * .8f;
  tmp.Crop();
  mpGradient->AddStop(tmp, .5f); 

  tmp.Alpha() = alpha * 1.f;
  tmp.Crop();
  mpGradient->AddStop(tmp, 1);
}

void nuiSimpleIcon::ResetSelectedGradient()
{
  if (mpSelectedGradient)
    delete mpSelectedGradient;
  
  nuiColor tmp = GetColor(eSelectedTabBg);

  float alpha = tmp.Alpha();

  mpSelectedGradient = new nuiGradient();
  tmp.Alpha() = alpha * .5f;
  tmp.Crop();
  mpSelectedGradient->AddStop(tmp, 0.f); 
  
  tmp.Alpha() = alpha * .8f;
  tmp.Crop();
  mpSelectedGradient->AddStop(tmp, .5f); 

  tmp.Alpha() = alpha * 1.f;
  tmp.Crop();
  mpSelectedGradient->AddStop(tmp, 1.f);
}

nuiRect nuiSimpleIcon::CalcIdealSize()
{
  return mpLabel->GetIdealRect();
}

bool nuiSimpleIcon::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect); 
  const nuiRect& rect(rRect.Size());
  
  if (mpShape)
    delete mpShape;
  mpShape = new nuiShape();
  nuiContour* pContour = new nuiContour();
  mpShape->AddContour(pContour);
  
  float radius = mRadius;
  
  if (mPosition == nuiRight)
  {
    if (rect.GetHeight() < 2*radius)
      radius = rRect.GetHeight() / 2.f;
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top()));
    pContour->LineTo(nuiPoint(rect.Right() - radius, rect.Top()));
    pContour->ArcTo (nuiPoint(rect.Right(), rect.Top() + radius), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Right(), rect.Bottom() - radius));
    pContour->ArcTo (nuiPoint(rect.Right() - radius, rect.Bottom()), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Left(), rect.Bottom()));
  }
  else if (mPosition == nuiLeft)
  {                                    
    if (rect.GetHeight() < 2*radius)
      radius = rRect.GetHeight() / 2.f;
    pContour->LineTo(nuiPoint(rect.Right(), rect.Bottom()));
    pContour->LineTo(nuiPoint(rect.Left() + radius, rect.Bottom()));
    pContour->ArcTo (nuiPoint(rect.Left(), rect.Bottom() - radius), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top() + radius));
    pContour->ArcTo (nuiPoint(rect.Left() + radius, rect.Top()), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Right(), rect.Top()));
  }
  else if (mPosition == nuiTop)
  {                                    
    if (rect.GetWidth() < 2*radius)
      radius = rRect.GetWidth() / 2.f;
    pContour->LineTo(nuiPoint(rect.Right(), rect.Bottom()));
    pContour->LineTo(nuiPoint(rect.Left(), rect.Bottom()));
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top() + radius));
    pContour->ArcTo (nuiPoint(rect.Left() + radius, rect.Top()), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Right() - radius, rect.Top()));
    pContour->ArcTo (nuiPoint(rect.Right(), rect.Top() + radius), radius, radius, 0, false, true);
  }
  else// if (mPosition == nuiBottom)
  {                                    
    if (rect.GetWidth() < 2*radius)
      radius = rRect.GetWidth() / 2.f;
    pContour->LineTo(nuiPoint(rect.Left(), rect.Top()));
    pContour->LineTo(nuiPoint(rect.Right(), rect.Top()));
    pContour->LineTo(nuiPoint(rect.Right(), rect.Bottom() - radius));
    pContour->ArcTo (nuiPoint(rect.Right() - radius, rect.Bottom()), radius, radius, 0, false, true);
    pContour->LineTo(nuiPoint(rect.Left() + radius, rect.Bottom()));
    pContour->ArcTo (nuiPoint(rect.Left(), rect.Bottom() - radius), radius, radius, 0, false, true); 
  }
  return true;
}

bool nuiSimpleIcon::Draw(nuiDrawContext* pContext)
{
  const nuiRect& rRect(GetRect().Size());
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->ResetClipShape();
  pContext->AddClipShape(*mpShape);
  
  bool sel = IsSelected();
  if (mPosition == nuiTop)
    pContext->DrawGradient((sel ? *mpSelectedGradient : *mpGradient), rRect, 0.f, rRect.GetHeight(), 0.f, 0.f);
  else if (mPosition == nuiLeft)
    pContext->DrawGradient((sel ? *mpSelectedGradient : *mpGradient), rRect, rRect.GetWidth(), 0.f, 0.f, 0.f);
  else if (mPosition == nuiRight)
    pContext->DrawGradient((sel ? *mpSelectedGradient : *mpGradient), rRect, 0.f, 0.f, rRect.GetWidth(), 0.f);
  else // nuiBottom
    pContext->DrawGradient((sel ? *mpSelectedGradient : *mpGradient), rRect, 0.f, 0.f, 0.f, rRect.GetHeight());
  DrawChild(pContext, mpLabel);
  pContext->ResetClipShape();
  return true;
}

void nuiSimpleIcon::SetRadius( float radius )
{
  mRadius = radius; InvalidateLayout();
}

nuiOrientation nuiSimpleIcon::GetOrientation()
{
  return mOrientation;
}

nuiPosition nuiSimpleIcon::GetPosition()
{
  return mPosition;
}
////////////////// TabView ///////////////////////////

nuiTabView::nuiTabView(nuiPosition pos)
: nuiSimpleContainer(), mTabViewEvents(this),
  mCurrentTabIndex(0), mPosition(pos)
{
  SetObjectClass(_T("nuiTabView"));
  mChildrenRectUnion = true;
  mFoldable = false;
  mFolded = false;
  mSliding = false;
  mFoldRatio = 0.0f;
}

nuiTabView::~nuiTabView()
{
}

nuiRect nuiTabView::CalcIdealSize()
{
  nuiRect IdealRect(0.f, 0.f, 0.f, 0.f);
  
  nuiSize w = 0.f, h = 0.f;
  nuiSize W = 0.f, H = 0.f;

  for (std::vector<nuiWidget*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
  {
    W += (*it)->GetIdealRect().GetWidth();
    H += (*it)->GetIdealRect().GetHeight();
    w = MAX(w, (*it)->GetIdealRect().GetWidth());
    h = MAX(h, (*it)->GetIdealRect().GetHeight());
  }
  IdealRect.SetSize(w, h);
  mIdealIconsRect = IdealRect;
  
  w = h = 0.f;
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++)
  {
    w = MAX(w, (*it)->GetIdealRect().GetWidth());
    h = MAX(h, (*it)->GetIdealRect().GetHeight());
  }
  IdealRect.SetSize(w, h);
  mIdealTabsRect = IdealRect;
  if (mPosition == nuiTop || mPosition == nuiBottom)
  { 
    mIdealIconsRect.SetSize(W, mIdealIconsRect.GetHeight());
    IdealRect.SetSize(w, h + mIdealIconsRect.GetHeight());
  }
  else // nuiLeft || nuiRight
  {
    mIdealIconsRect.SetSize(mIdealIconsRect.GetWidth(), MIN(H,h));
    IdealRect.SetSize(w + mIdealIconsRect.GetWidth(), h);
  }
   
  //mIdealRect = IdealRect;
  return IdealRect;
}

bool nuiTabView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiRect TabRect;

  if (mFoldable)
  {
    // Adjust XOff and YOff
    if (mSliding)
    {
      if (!mFolded)
      {
        mFoldRatio += 0.1f;
        if (mFoldRatio >= 0.0f)
        {
          mSliding = false;
          mFoldRatio = 0.0f;
          StopAutoDraw();
        }
      }
      else
      {
        mFoldRatio -= 0.1f;
        if (mFoldRatio <= -1.0f)
        {
          mSliding = false;
          mFoldRatio = -1.0f;
          StopAutoDraw();
        }
      }
    }
    else
    {
      if (mFolded)
        mFoldRatio = -1.0f;
    }
  }

  nuiSize Y = 0;
  nuiSize X = 0;
  nuiSize R = 1.f;
  nuiSize Tmp;
  uint i = 0;

  if (mPosition == nuiRight || mPosition == nuiLeft)
  {
    if (mIdealIconsRect.GetHeight() > rRect.GetHeight())
      R = rRect.GetHeight() / mIdealIconsRect.GetHeight();        
  }
  else // nuiTop || nuiBottom
  {
    if (mIdealIconsRect.GetWidth() > rRect.GetWidth())
      R = rRect.GetWidth() / mIdealIconsRect.GetWidth();
  }

  if (mChildrenRectUnion)
  {
    if (mPosition == nuiRight)
    {
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      X = TabRect.GetWidth();
      float XOff = TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else if (mPosition == nuiTop)
    {
      TabRect = nuiRect(0.f, mIdealIconsRect.GetHeight(), rRect.GetWidth(), rRect.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = -TabRect.GetHeight() * mFoldRatio;
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
    else if (mPosition == nuiLeft)
    {
      TabRect = nuiRect(mIdealIconsRect.GetWidth(), 0.f, rRect.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      float XOff = -TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else// if (mPosition == nuiBottom)
    {
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth(), rRect.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = TabRect.GetHeight() * mFoldRatio;
      Y = TabRect.Bottom();
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
  }
  else
  {
    nuiRect r(mTabs[mCurrentTabIndex]->GetIdealRect());
    if (mPosition == nuiRight)
    {
      r.Right() = (MIN(rRect.GetWidth() - mIdealIconsRect.GetWidth(), r.GetWidth()));
      TabRect = nuiRect(0.f, 0.f, r.GetWidth(), rRect.GetHeight());
      float XOff = TabRect.GetWidth() * mFoldRatio;
      X = TabRect.GetWidth();
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else if (mPosition == nuiTop)
    {
      r.Bottom() = (MIN(rRect.GetHeight() - mIdealIconsRect.GetHeight(), r.GetHeight()));
      TabRect = nuiRect(0.f, mIdealIconsRect.GetHeight(), rRect.GetWidth(), r.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = -TabRect.GetHeight() * mFoldRatio;
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
    else if (mPosition == nuiLeft)
    {
      r.Right() = (MIN(rRect.GetWidth() - mIdealIconsRect.GetWidth(), r.GetWidth()));
      TabRect = nuiRect(mIdealIconsRect.GetWidth(), 0.f, r.GetWidth() - mIdealIconsRect.GetWidth(), rRect.GetHeight());
      float XOff = -TabRect.GetWidth() * mFoldRatio;
      TabRect.Move(XOff, 0.0f);
      X += XOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetHeight());
        mIcons[i]->SetLayout(nuiRect(X, Y, mIdealIconsRect.GetWidth(), Tmp));
        Y += Tmp;
      }
    }
    else// if (mPosition == nuiBottom)
    {
      r.Bottom() = (MIN(rRect.GetHeight() - mIdealIconsRect.GetHeight(), r.GetHeight()));
      TabRect = nuiRect(0.f, 0.f, rRect.GetWidth(), r.GetHeight() - mIdealIconsRect.GetHeight());
      float YOff = TabRect.GetHeight() * mFoldRatio;
      Y = TabRect.Bottom();
      TabRect.Move(0.0f, YOff);
      Y += YOff;
      for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
      {
        (*it)->SetLayout(TabRect);
        Tmp = (nuiSize)ToNearest(R * mIcons[i]->GetIdealRect().GetWidth());
        mIcons[i]->SetLayout(nuiRect(X, Y, Tmp, mIdealIconsRect.GetHeight()));
        X += Tmp;
      }
    }
  }

  return true;
}

bool nuiTabView::Draw(nuiDrawContext* pContext)
{
  pContext->Clip(mRect.Size());
  pContext->SetStrokeColor(nuiColor(1.f,1.f,0.f));
  for (std::vector<nuiWidget*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
  {
    DrawChild(pContext, (*it));
  }
  if (mFoldRatio > -1.0f)
    DrawChild(pContext, mTabs[mCurrentTabIndex]);
  return true;
}

bool nuiTabView::MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  for (std::vector<nuiWidget*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
    if ((*it)->GetRect().IsInside(X,Y))
      return true;
  return false;
}

bool nuiTabView::MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  for (std::vector<nuiWidget*>::iterator it = mIcons.begin(); it != mIcons.end(); it++)
    if ((*it)->GetRect().IsInside(X,Y))
      return true;
  return false;
}
 

void nuiTabView::AddTab(nuiWidget* pTab, nuiWidget* pIcon)
{
  InsertTab(pTab, pIcon, mIcons.size());
}

void nuiTabView::InsertTab(nuiWidget* pTab, nuiWidget* pIcon, uint pos)
{
  AddChild(pTab);
  AddChild(pIcon);
  if (!mTabs.empty())
    pTab->SetEnabled(false);
  pIcon->SetSelected(false);
  mTabViewEvents.Connect(pIcon->Clicked, &nuiTabView::OnIconClicked, pIcon);
  
  if (pos >= mIcons.size())
  {
    mTabs.push_back(pTab);
    mIcons.push_back(pIcon);
  }
  else
  {
    uint t = 0;
    std::vector<nuiWidget*>::iterator i = mIcons.begin();
    for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++, t++)
    {
      if (t == pos)
      {
        mIcons.insert(i, pIcon);
        mTabs.insert(it, pTab);
      }
    }
  }
  InvalidateLayout();
}

void nuiTabView::RemoveTab(nuiWidget* pTab, bool trashit)
{
  std::vector<nuiWidget*>::iterator i = mIcons.begin();
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++)
  {
    if (*it == pTab)
    {
      DelChild((*it), trashit);
      mTabViewEvents.Disconnect((*i)->Clicked);
      DelChild((*i), trashit);

      mTabs.erase(it);     
      mIcons.erase(i);
      return;
    }
  }
}

void nuiTabView::RemoveTab(const uint& tab_index, bool trashit)
{
  NGL_ASSERT(tab_index < mIcons.size());

  uint t = 0;
  std::vector<nuiWidget*>::iterator i = mIcons.begin();
  for (std::vector<nuiWidget*>::iterator it = mTabs.begin(); it != mTabs.end(); it++, i++, t++)
  {
    if (t == tab_index)
    {
      DelChild(*it, trashit);
      mTabViewEvents.Disconnect((*i)->Clicked);
      DelChild(*i, trashit);

      mTabs.erase(it);     
      mIcons.erase(i);
      return;
    }
  }
}

bool nuiTabView::OnIconClicked(const nuiEvent& rEvent)
{
  nuiWidget* pIcon = (nuiWidget*)(rEvent.mpUser);
  for (uint i = 0; i < mIcons.size(); i++)
  {
    if (mIcons[i] == pIcon)
    {
      if (mFoldable)
      {
        if (mFolded || (i == mCurrentTabIndex))
        {
          // Slide in or out
          mFolded = !mFolded;
          mSliding = true;
          AnimateLayout(true);
          StartAutoDraw(30);
        }
      }

      pIcon->SetSelected(true);
      SelectTab(i);
      TabSelect(TabEvent(mCurrentTabIndex));
    }
    else
      ((nuiWidget*)(mIcons[i]))->SetSelected(false);
  }
  return true;
}

void nuiTabView::SelectTab(const uint& rIndex)
{
  NGL_ASSERT(rIndex < mIcons.size());
  if (mCurrentTabIndex == rIndex)
    return;

  ((nuiWidget*)(mIcons[mCurrentTabIndex]))->SetSelected(false);
  mTabs[mCurrentTabIndex]->SetEnabled(false);
  mCurrentTabIndex = rIndex;
  mTabs[mCurrentTabIndex]->SetEnabled(true);

  ((nuiWidget*)(mIcons[mCurrentTabIndex]))->SetSelected(true);
  if (mChildrenRectUnion)
    Invalidate();
  else
    InvalidateLayout();
}

void nuiTabView::SetPosition(nuiPosition position)
{
  if (mPosition == position)
    return;
  mPosition = position;
 InvalidateLayout();
}

void nuiTabView::EnableChildrenRectUnion(bool set)
{
  if (mChildrenRectUnion != set)
  {
    mChildrenRectUnion = set;
    InvalidateLayout();
  }
}

bool nuiTabView::GetChildrenRectUnion() const
{
  return mChildrenRectUnion;
}

nuiOrientation nuiTabView::GetOrientation()
{
  return mPosition == nuiLeft || mPosition == nuiRight ? nuiVertical : nuiHorizontal;
}

nuiPosition nuiTabView::GetPosition()
{
  return mPosition;
}

uint nuiTabView::GetTabCount()
{
  return (uint)mIcons.size();
}

void nuiTabView::SetFoldable( bool Foldable )
{
  mFoldable = Foldable;
}

bool nuiTabView::GetFoldable() const
{
  return mFoldable;
}

void nuiTabView::SetFolded( bool set, bool Animate )
{
  mFolded = set;
  if (Animate)
  {
    mSliding = true;
    AnimateLayout(true);
    StartAutoDraw(30.0f);
  }
  else
  {
    mSliding = false;
    StopAutoDraw();
    if (mRect.GetSurface() > 0.0f)
      SetRect(mRect);
    else
      InvalidateLayout();
  }
}

bool nuiTabView::GetFolded() const
{
  return mFolded;
}

TabEvent::TabEvent(const uint& index) : nuiEvent(), mTabIndex(index)
{

}
