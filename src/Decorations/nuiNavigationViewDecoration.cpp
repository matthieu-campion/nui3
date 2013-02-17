/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiNavigationViewDecoration::nuiNavigationViewDecoration(const nglString& rName)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiNavigationViewDecoration")))
    InitAttributes();

  mClientRect = nuiRect(0,0,0,0);
  nglString mDeviceStyle = DEVICESTYLE_DEFAULT;
  //LBDEBUG TODO PLATEFORM
  mEvenWidth = 10; 
  mOddWidth = 4; 
  //LBDEBUG TODO PLATEFORM
  mColumnColor = nuiColor(203,210,216);
  mBackgroundColor = nuiColor(197,204,212);
  
}

nuiNavigationViewDecoration::~nuiNavigationViewDecoration()
{
}


void nuiNavigationViewDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiNavigationViewDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiNavigationViewDecoration::SetSourceClientRect)));

  AddAttribute(new nuiAttribute<const nglString&>
               (nglString(_T("DeviceStyle")), nuiUnitNone,
                nuiAttribute<const nglString&>::GetterDelegate(this, &nuiNavigationViewDecoration::GetDeviceStyle),
                nuiAttribute<const nglString&>::SetterDelegate(this, &nuiNavigationViewDecoration::SetDeviceStyle)));

}


// virtual
void nuiNavigationViewDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->PushState();
  pContext->ResetState();
  
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);

  //LBDEBUG TODO PLATEFORM

  // background
  pContext->SetFillColor(mBackgroundColor);
  pContext->DrawRect(rDestRect, eFillShape);
  
  // columns
  nuiRect rect;
  pContext->SetFillColor(mColumnColor);
  float size = mEvenWidth;
  while (size < rDestRect.GetWidth())
  {
    rect.Set(size, rDestRect.Top(), mOddWidth, rDestRect.GetHeight());
    pContext->DrawRect(rect, eFillShape);
    size += mOddWidth;
    size += mEvenWidth;
  }

  pContext->PopState();
}



nuiRect nuiNavigationViewDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return mClientRect.Size();
}



nuiSize nuiNavigationViewDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  if (!mBorderEnabled)
    return 0;
  
  switch (position)
  {
  case nuiLeft:
  case nuiRight:
    return mClientRect.Left();
    break;
  case nuiTop:
  case nuiBottom:
    return mClientRect.Top();
    break;
  case nuiFillHorizontal:
    return mClientRect.Left()*2;
    break;
  case nuiFillVertical:
    return mClientRect.Top()*2;
    break;
  case nuiNoPosition: break;
  case nuiTopLeft: break;
  case nuiTopRight: break;
  case nuiBottomLeft: break;
  case nuiBottomRight: break;
  case nuiCenter: break;
  case nuiTile: break;
  case nuiFill: break;
  case nuiFillLeft: break;
  case nuiFillRight: break;
  case nuiFillTop: break;
  case nuiFillBottom: break;
  }
  //we should'nt arrive here
  return NULL; 
}

void nuiNavigationViewDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
  {
    rLeft = rRight = rTop = rBottom = rHorizontal = rVertical = 0;
    return;
  }
  
  rLeft = mClientRect.Left();
  rRight = rLeft;
  rTop = mClientRect.Top();
  rBottom = rTop;
  rHorizontal = rLeft * 2;
  rVertical = rTop * 2;
}

void nuiNavigationViewDecoration::SetDeviceStyle(const nglString& style)
{
  mDeviceStyle = style;
}

const nglString& nuiNavigationViewDecoration::GetDeviceStyle() const
{
  return mDeviceStyle;
}



//*************************************************************************
//
// attributes setters/getters
//


const nuiRect& nuiNavigationViewDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiNavigationViewDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  Changed();
}


