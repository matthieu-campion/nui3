/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"




nuiTreeHandleDecoration::nuiTreeHandleDecoration(const nglString& rName, nuiColor handleColor, uint32 handleSize, const nuiBorder& rHandleBorder)
  : nuiDecoration(rName), mBorder(rHandleBorder)
{
  if (SetObjectClass(_T("nuiTreeHandleDecoration")))
    InitAttributes();
  
  mHandleColor = handleColor;
  mHandleSize = handleSize;
    
  // on the front layer by default
  mLayer = eLayerFront;

}



nuiTreeHandleDecoration::~nuiTreeHandleDecoration()
{
}



void nuiTreeHandleDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiBorder&>
   (nglString(_T("HandleBorder")), nuiUnitNone,
    nuiAttribute<const nuiBorder&>::GetterDelegate(this, &nuiTreeHandleDecoration::GetHandleBorder),
    nuiAttribute<const nuiBorder&>::SetterDelegate(this, &nuiTreeHandleDecoration::SetHandleBorder)));

  AddAttribute(new nuiAttribute<uint32>
  (nglString(_T("HandleSize")), nuiUnitPixels,
   nuiMakeDelegate(this, &nuiTreeHandleDecoration::GetHandleSize),
   nuiMakeDelegate(this, &nuiTreeHandleDecoration::SetHandleSize)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("HandleColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiTreeHandleDecoration::GetHandleColor), 
   nuiMakeDelegate(this, &nuiTreeHandleDecoration::SetHandleColor)));
}


uint32 nuiTreeHandleDecoration::GetHandleSize()
{
  return mHandleSize;
}

void nuiTreeHandleDecoration::SetHandleSize(uint32 size)
{
  mHandleSize = size;
  Changed();
}

const nuiColor& nuiTreeHandleDecoration::GetHandleColor()
{
  return mHandleColor;
}

void nuiTreeHandleDecoration::SetHandleColor(const nuiColor& handleColor)
{
  mHandleColor = handleColor;
  Changed();
}




void nuiTreeHandleDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rRect)
{
  pContext->PushState();
  pContext->ResetState();
  
  bool IsOpened = pWidget->IsSelected();

  nuiShape Shape;

  nuiSize x,y;
  x = rRect.mLeft + mBorder.Left();
  y = rRect.mTop + mBorder.Top() + (rRect.GetHeight() - mHandleSize)/2.0f;

  nuiContour* pContour = new nuiContour();
  if (IsOpened)
  {
    pContour->LineTo(nuiPoint(x, y, 0));
    pContour->LineTo(nuiPoint(x + mHandleSize, y, 0));
    pContour->LineTo(nuiPoint(x + mHandleSize/2, y + mHandleSize, 0));
    pContour->LineTo(nuiPoint(x, y, 0));
  }
  else
  {
    pContour->LineTo(nuiPoint(x, y, 0));
    pContour->LineTo(nuiPoint(x + mHandleSize, y + mHandleSize/2, 0));
    pContour->LineTo(nuiPoint(x, y + mHandleSize, 0));
    pContour->LineTo(nuiPoint(x, y, 0));
  }

  Shape.AddContour(pContour);

  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);

  pContext->SetFillColor(mHandleColor);
  pContext->DrawShape(&Shape, eFillShape);

  pContext->PopState();
}



nuiRect nuiTreeHandleDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return nuiRect(0.f,0.f,0.f,0.f);
}



nuiSize nuiTreeHandleDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  if (!mBorderEnabled)
    return 0;
  
  switch (position)
  {
  case nuiLeft:
    return mBorder.GetWidth() + mHandleSize/2.0f;
  case nuiRight:
    return 0;
  case nuiTop:
    return mBorder.Top();
  case nuiBottom:
    return mBorder.Bottom();
  case nuiFillHorizontal:
    return mBorder.GetWidth() + mHandleSize/2.0f;
  case nuiFillVertical:
    return mBorder.Top()+mBorder.Bottom();
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

void nuiTreeHandleDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
  {
    rLeft = rRight = rTop = rBottom = rHorizontal = rVertical = 0;
    return;
  }
  
  rLeft = mBorder.GetWidth() + mHandleSize/2.0f;
  rRight = 0;
  rTop = mBorder.Top();
  rBottom = mBorder.Bottom();
  rHorizontal = rLeft;
  rVertical = rTop + rBottom;
}




const nuiBorder& nuiTreeHandleDecoration::GetHandleBorder() const
{
  return mBorder;
}

void nuiTreeHandleDecoration::SetHandleBorder(const nuiBorder& rHandleBorder)
{
  mBorder = rHandleBorder;
  Changed();
}
