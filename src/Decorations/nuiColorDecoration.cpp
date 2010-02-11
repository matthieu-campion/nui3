/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiColorDecoration.h"

nuiColorDecoration::nuiColorDecoration(const nglString& rName)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiColorDecoration")))
    InitAttributes();

  mStrokeSize = 0;
  mFillColor = nuiColor(0,0,0);
  mStrokeColor = nuiColor(0,0,0);
  mShapeMode = eFillShape;
  mBlendFunc = nuiBlendTransp;
  mClientRect = nuiRect(0,0,0,0);
}


nuiColorDecoration::nuiColorDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rFillColor, uint32 strokeSize, const nuiColor& rStrokeColor, nuiShapeMode ShapeMode, nuiBlendFunc BlendFunc)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiColorDecoration")))
    InitAttributes();

  mStrokeSize = strokeSize;
  mFillColor = rFillColor;
  mStrokeColor = rStrokeColor;
  mShapeMode = ShapeMode;
  mBlendFunc = BlendFunc;
  mClientRect = rClientRect;
}


nuiColorDecoration::nuiColorDecoration(const nglString& rName, const nuiColor& rFillColor, uint32 strokeSize, const nuiColor& rStrokeColor, nuiShapeMode ShapeMode, nuiBlendFunc BlendFunc, const nuiRect& rClientRect)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiColorDecoration")))
    InitAttributes();

  mStrokeSize = strokeSize;
  mFillColor = rFillColor;
  mStrokeColor = rStrokeColor;
  mShapeMode = ShapeMode;
  mBlendFunc = BlendFunc;
  mClientRect = rClientRect;
}



nuiColorDecoration::~nuiColorDecoration()
{
}


void nuiColorDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiColorDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiColorDecoration::SetSourceClientRect)));

  AddAttribute(new nuiAttribute<uint32>
  (nglString(_T("StrokeSize")), nuiUnitPixels,
   nuiMakeDelegate(this, &nuiColorDecoration::GetStrokeSize),
   nuiMakeDelegate(this, &nuiColorDecoration::SetStrokeSize)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("FillColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiColorDecoration::GetFillColor), 
   nuiMakeDelegate(this, &nuiColorDecoration::SetFillColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiColorDecoration::GetStrokeColor), 
   nuiMakeDelegate(this, &nuiColorDecoration::SetStrokeColor)));

  AddAttribute(new nuiAttribute<nuiShapeMode>
  (nglString(_T("ShapeMode")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiColorDecoration::GetShapeMode), 
   nuiMakeDelegate(this, &nuiColorDecoration::SetShapeMode)));

  
}


bool nuiColorDecoration::Load(const nuiXMLNode* pNode)
{
  uint32 code = pNode->GetAttribute(_T("ShapeMode")).GetCInt();
  switch(code)
  {
  case 0: mShapeMode = eStrokeShape; break;
  case 1: mShapeMode = eFillShape; break;
  case 2: mShapeMode = eStrokeAndFillShape; break;
  default: mShapeMode = eDefault; break;
  }

  mFillColor.SetValue(nuiGetString(pNode, _T("FillColor"), _T("white")));
  mStrokeSize = nuiGetString(pNode, _T("StrokeSize")).GetCUInt();
  mStrokeColor.SetValue(nuiGetString(pNode, _T("StrokeColor"), _T("black")));
  
  //#FIXME
  return true;
}

nuiXMLNode* nuiColorDecoration::Serialize(nuiXMLNode* pNode)
{
//#FIXME
//  pNode->SetName(_T("nuiGradientDecoration"));
//  pNode->SetAttribute(_T("GradientType"), (int)mGradientType);
//  pNode->SetAttribute(_T("Color1"), mColor1.GetValue());
//  pNode->SetAttribute(_T("Color2"), mColor2.GetValue());
//  pNode->SetAttribute(_T("Color3"), mColor3.GetValue());
//  pNode->SetAttribute(_T("Color4"), mColor4.GetValue());
//  pNode->SetAttribute(_T("Overdraw"), mOverdraw);

  return pNode;
}

// virtual
void nuiColorDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->PushState();
  pContext->ResetState();
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(mBlendFunc);
  //pContext->EnableTexturing(false);
  
  nuiColor fillColor = mFillColor;
  nuiColor strokeColor = mStrokeColor;
  if (pWidget) {
    fillColor.Multiply(pWidget->GetMixedAlpha());
    strokeColor.Multiply(pWidget->GetMixedAlpha());
  }
  pContext->SetFillColor(fillColor);
  pContext->SetStrokeColor(strokeColor);
  pContext->SetLineWidth(mStrokeSize);

  pContext->DrawRect(rDestRect, mShapeMode);

  // waiting for DrawRect to use the StrokeSize parameter, do this hack
  uint32 strokeSize = 1;
  nuiRect rect = rDestRect;
  pContext->SetLineWidth(1);
  while (strokeSize < mStrokeSize)
  {
    rect.Set(rect.Left()+1.f, rect.Top()+1.f, rect.GetWidth()-2.f, rect.GetHeight()-2.f);
    pContext->DrawRect(rect, mShapeMode);
    strokeSize++;
  }
  
  
  pContext->PopState();
}



nuiRect nuiColorDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return mClientRect.Size();
}



nuiSize nuiColorDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
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

void nuiColorDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
    return;
  
  rLeft = mClientRect.Left();
  rRight = rLeft;
  rTop = mClientRect.Top();
  rBottom = rTop;
  rHorizontal = rLeft * 2;
  rVertical = rTop * 2;
}



//*************************************************************************
//
// attributes setters/getters
//


const nuiRect& nuiColorDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiColorDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  Changed();
}

const nuiColor& nuiColorDecoration::GetFillColor() const
{
  return mFillColor;
}

void nuiColorDecoration::SetFillColor(const nuiColor& color)
{
  mFillColor = color;
  Changed();
}


const nuiColor& nuiColorDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}


void nuiColorDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
  Changed();
}


void nuiColorDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
  Changed();
}


uint32 nuiColorDecoration::GetStrokeSize() const
{
  return mStrokeSize;
}


void nuiColorDecoration::SetShapeMode(nuiShapeMode mode)
{
  mShapeMode = mode;
  Changed();
}


nuiShapeMode nuiColorDecoration::GetShapeMode() const
{
  return mShapeMode;
}

