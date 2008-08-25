/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiWidgetBox.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"

using namespace std;

nuiWidgetBox::nuiWidgetBox(nuiOrientation orientation)
  : nuiSimpleContainer()
{
  SetObjectClass(_T("nuiWidgetBox"));
  mOrientation = orientation;
  mBorderSize = mDefaultBorderSize;
  mProportional = true;
  mFill = true;
  InitProperties();
}

bool nuiWidgetBox::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiWidgetBox"));

  mBorderSize = nuiGetVal( pNode, _T("BorderSize"), mDefaultBorderSize);
  mOrientation = nuiGetOrientation( pNode, nuiVertical);
  mProportional = nuiGetBool( pNode, nglString(_T("Proportional")), true);
  mFill = nuiGetBool( pNode, nglString(_T("Fill")), true); 
  InitProperties();
  
  return true;
}


nuiWidgetBox::~nuiWidgetBox()
{
}

bool nuiWidgetBox::Draw(nuiDrawContext* pContext)
{
  return DrawChildren(pContext);
}

void nuiWidgetBox::InitProperties()
{
  mCanRespectConstraint = true;
#ifdef NGL_USE_COMPLEX_PROPERTIES
  mProperties[_T("BorderSize")].Bind(&mBorderSize,true,false);
  mProperties[_T("Orientation")].Bind(&mOrientation,true,false);
  mProperties[_T("Proportional")].Bind(&mProportional,true,false);
  mProperties[_T("Fill")].Bind(&mFill,true,false);
#endif
}


nuiRect nuiWidgetBox::CalcIdealSize()
{
  nuiSize Height=0,Width=0;
  nuiRect Rect;
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();

  nuiWidget::LayoutConstraint constraint(mConstraint);
  constraint.mMaxWidth -= mBorderLeft + mBorderRight;
  constraint.mMaxHeight -= mBorderTop + mBorderBottom;

  //NGL_OUT(_T("nuiWidgetBox  new constraint %d x %d\n"),(int)constraint.mMaxWidth,(int)constraint.mMaxHeight);

  mPositions.clear();
  if (mOrientation == nuiVertical)
  {
    for (it=mpChildren.begin(); it!=end; ++it)
    {
      nuiWidgetPtr pItem = (*it);
      pItem->SetLayoutConstraint(constraint);

      Rect = pItem->GetIdealRect();
      Rect.RoundToBiggest();
      Width = MAX(Width, Rect.GetWidth()+2.0f*mBorderSize);
      mPositions.push_back(Rect.Size());

      //NGL_OUT(_T("    WB rect %ls\n"), Rect.GetValue().GetChars());

      Height += Rect.GetHeight() + mBorderSize * 2.0f;
    }
  }
  else
  {
    for (it=mpChildren.begin(); it!=end; ++it)
    {
      nuiWidgetPtr pItem = (*it);
/*
      pItem->SetLayoutConstraint(constraint);
*/
      Rect = pItem->GetIdealRect();
      Rect.RoundToBiggest();
      Height = MAX(Height, Rect.GetHeight()+2.0f*mBorderSize);
      mPositions.push_back(Rect.Size());

      Width += Rect.GetWidth() + mBorderSize * 2.0f;
    }
  }

  mIdealRect.Set(0.0f ,0.0f, Width, Height);
  mPositions.push_front(mIdealRect);
  //NGL_OUT(_T("nuiWidgetBox Ideal Size: %ls\n"),(int)mIdealRect.GetValue().GetChars());
  return mIdealRect;
}

bool nuiWidgetBox::SetRect(const nuiRect& rRect)
{
  nuiWidgetList::iterator it;
  nuiWidgetList::iterator end = mpChildren.end();
  nuiSize Height = rRect.GetHeight();
  nuiSize Width = rRect.GetWidth();
  nuiSize Xratio,Yratio;
  nuiRect Rect;
  //NGL_OUT(_T("nuiWidgetBox::SetRect: %d x %d\n"),(int)Width,(int)Height);

  CalcIdealSize();

  nuiWidget::SetRect(rRect);

  list<nuiRect>::iterator rit = mPositions.begin();
  Rect=(*rit);
  ++rit;

  Xratio = (Width)/(Rect.GetWidth());
  Yratio = (Height)/(Rect.GetHeight());

  if (mOrientation == nuiVertical)
  {
    uint fixedsize = 0;

    if (!mProportional && mpChildren.size())
    {
      fixedsize = (uint)(Height/mpChildren.size());
    }

    Height = mBorderSize;
    for (it = mpChildren.begin(); it != end; ++it,++rit)
    {
      nuiWidgetPtr pItem = (*it);
      Rect = (*rit);
      nuiSize h;
      if (mFill)
      {
        h = (nuiSize)(int)(mProportional ? (Rect.GetHeight() * Yratio) : fixedsize);
      }
      else
      {
        h = (nuiSize)(int)Rect.GetHeight();
        if (h + Height > rRect.GetHeight())
          h = rRect.GetHeight() - Height;
      }

      Rect.Set(mBorderSize,Height,Width - 2.0f * mBorderSize ,h);
      Rect.RoundToAbove();
      Height += h + mBorderSize * 2;
      pItem->SetLayout(Rect);
    }
  }
  else
  {
    uint fixedsize = 0;

    if (!mProportional && mpChildren.size())
    {
      fixedsize = (uint)(Width/mpChildren.size());
    }

    Width = mBorderSize;
    for (it=mpChildren.begin(); it!=end; ++it,++rit)
    {
      nuiWidgetPtr pItem = (*it);
      Rect=(*rit);
      nuiSize w;
      if (mFill)
      {
        w =(nuiSize)(int)(mProportional ? (Rect.GetWidth() * Xratio) : fixedsize);
      }
      else
      {
        w = (nuiSize)(int) Rect.GetWidth();
      }

      Rect.Set(Width, mBorderSize, w, Height - 2.0f * mBorderSize);
      Rect.RoundToAbove();
      Width += w + mBorderSize * 2.0f;
      pItem->SetLayout(Rect);
    }
  }

  return true;
}

void nuiWidgetBox::SetOrientation(nuiOrientation orientation)
{
  mOrientation = orientation;
  InvalidateLayout();
}

nuiOrientation nuiWidgetBox::SetOrientation()
{
  return mOrientation;
}

nuiSize nuiWidgetBox::GetBorderSize()
{
  return mBorderSize;
}

void nuiWidgetBox::SetBorderSize(nuiSize BorderSize)
{
  if (mBorderSize == BorderSize)
    return;
  mBorderSize = BorderSize;
  InvalidateLayout();
}

nuiSize nuiWidgetBox::mDefaultBorderSize = 4;

nuiSize nuiWidgetBox::GetDefaultBorderSize()
{
  return mDefaultBorderSize;
}

void nuiWidgetBox::SetDefaultBorderSize(nuiSize BorderSize)
{
  mDefaultBorderSize = BorderSize;
}

void nuiWidgetBox::SetProportional(bool Proportional)
{
  if (mProportional == Proportional)
    return;
  mProportional = Proportional;
  Invalidate();
}

bool nuiWidgetBox::GetProportional()
{
  return mProportional;
}

void nuiWidgetBox::SetFill(bool Fill)
{
  if (mFill == Fill)
    return;
  mFill = Fill;
  Invalidate();
}

bool nuiWidgetBox::GetFill()
{
  return mFill;
}
