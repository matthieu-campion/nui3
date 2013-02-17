/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiGradientDecoration::nuiGradientDecoration(const nglString& rName)
: nuiDecoration(rName),
  mColor1(128, 128, 128),
  mColor2(0, 0, 0),
  mClientRect(0, 0, 0, 0),
  mStrokeSize(1),
  mStrokeColor(64, 64, 64),
  mShapeMode(eFillShape),
  mOrientation(nuiVertical)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, nuiOrientation orientation, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiRect& rClientRect, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, nuiOrientation orientation, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mColor3(rColor3),
  mColor4(rColor4),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient4Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode, nuiOrientation orientation, const nuiRect& rClientRect)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient2Colors;
  mUserOffsets = false;
  InitOffsets();
}


nuiGradientDecoration::nuiGradientDecoration(const nglString& rName, const nuiColor& rColor1, const nuiColor& rColor2, const nuiColor& rColor3, const nuiColor& rColor4, uint strokeSize, const nuiColor& rStrokeColor, nuiShapeMode shapeMode, nuiOrientation orientation, const nuiRect& rClientRect)
: nuiDecoration(rName),
  mColor1(rColor1),
  mColor2(rColor2),
  mColor3(rColor3),
  mColor4(rColor4),
  mStrokeSize(strokeSize),
  mStrokeColor(rStrokeColor),
  mShapeMode(shapeMode),
  mOrientation(orientation)
{
  if (SetObjectClass(_T("nuiGradientDecoration")))
    InitAttributes();

  mClientRect = rClientRect;
  mGradientType = nuiGradient4Colors;
  mUserOffsets = false;
  InitOffsets();
}




nuiGradientDecoration::~nuiGradientDecoration()
{
}

void nuiGradientDecoration::InitAttributes()
{
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiGradientDecoration::GetSourceClientRect),
    nuiMakeDelegate(this, &nuiGradientDecoration::SetSourceClientRect));
  AddAttribute(_T("ClientRect"), AttributeRect);

  nuiAttribute<nuiOrientation>* AttributeOrientation = new nuiAttribute<nuiOrientation>
  (nglString(_T("Orientation")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiGradientDecoration::GetOrientation),
    nuiMakeDelegate(this, &nuiGradientDecoration::SetOrientation));
  AddAttribute(_T("Orientation"), AttributeOrientation);

  nuiAttribute<uint32>* StrokeSize= new nuiAttribute<uint32>
  (nglString(_T("StrokeSize")), nuiUnitPixels,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetStrokeSize),
   nuiMakeDelegate(this, &nuiGradientDecoration::SetStrokeSize));
	AddAttribute(_T("StrokeSize"), StrokeSize);
  
  nuiAttribute<const nuiColor&>* Color1 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color1")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetColor1), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetColor1));
	AddAttribute(_T("Color1"), Color1);
  
  nuiAttribute<const nuiColor&>* Color2 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color2")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetColor2), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetColor2));
	AddAttribute(_T("Color2"), Color2);
  
  nuiAttribute<const nuiColor&>* Color3 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color3")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetColor3), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetColor3));
	AddAttribute(_T("Color3"), Color3);
  
  nuiAttribute<const nuiColor&>* Color4 = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color4")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetColor4), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetColor4));
	AddAttribute(_T("Color4"), Color4);
  
  nuiAttribute<const nuiColor&>* StrokeColor = new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetStrokeColor), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetStrokeColor));
	AddAttribute(_T("StrokeColor"), StrokeColor);
  
  
  AddAttribute(new nuiAttribute<nuiShapeMode>
  (nglString(_T("ShapeMode")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetShapeMode), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetShapeMode)));
  
  nuiAttribute<double>* Offset1 = new nuiAttribute<double>
  (nglString(_T("Offset1")), nuiUnitPercent,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetOffset1), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetOffset1));
	AddAttribute(_T("Offset1"), Offset1);

  nuiAttribute<double>* Offset2 = new nuiAttribute<double>
  (nglString(_T("Offset2")), nuiUnitPercent,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetOffset2), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetOffset2));
	AddAttribute(_T("Offset2"), Offset2);

  nuiAttribute<double>* Offset3 = new nuiAttribute<double>
  (nglString(_T("Offset3")), nuiUnitPercent,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetOffset3), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetOffset3));
	AddAttribute(_T("Offset3"), Offset3);

  nuiAttribute<double>* Offset4 = new nuiAttribute<double>
  (nglString(_T("Offset4")), nuiUnitPercent,
   nuiMakeDelegate(this, &nuiGradientDecoration::GetOffset4), 
   nuiMakeDelegate(this, &nuiGradientDecoration::SetOffset4));
	AddAttribute(_T("Offset4"), Offset4);
}

void nuiGradientDecoration::InitOffsets()
{
  if (mGradientType == nuiGradient4Colors)
  {
    mOffset1 = 0;
    mOffset2 = 0.48f;
    mOffset3 = 0.52f;
    mOffset4 = 1;
  }
  else
  {
    mOffset1 = 0;
    mOffset2 = 1;
  }
}

//virtual
void nuiGradientDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  nuiColor c1 = mColor1;
  nuiColor c2 = mColor2;
  nuiColor c3 = mColor3;
  nuiColor c4 = mColor4;
  nuiColor s = mStrokeColor;
  pContext->PushState();
  pContext->ResetState();
  
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);

  
  if (mUseWidgetAlpha && pWidget)
  {
    float widgetAlpha = pWidget->GetMixedAlpha();
    c1.Multiply(widgetAlpha);
    c2.Multiply(widgetAlpha);
    s.Multiply(widgetAlpha);
  }
  
  pContext->SetFillColor(nuiColor(1, 1, 1, 1));
  pContext->SetStrokeColor(s);

  nuiGradient gradient;
  if (!mUserOffsets)
    InitOffsets();
  gradient.AddStop(c1, mOffset1);
  gradient.AddStop(c2, mOffset2);
  if (mGradientType == nuiGradient4Colors)
  {
    if (mUseWidgetAlpha && pWidget)
    {
      float widgetAlpha = pWidget->GetMixedAlpha();
      c3.Multiply(widgetAlpha);
      c4.Multiply(widgetAlpha);
    }

    gradient.AddStop(c3, mOffset3);
    gradient.AddStop(c4, mOffset4);
  }

  if (mOrientation == nuiVertical)
    pContext->DrawGradient(gradient, rDestRect, 0, rDestRect.Top(), 0, rDestRect.Bottom());
  else
  {
    pContext->DrawGradient(gradient, rDestRect, 0, rDestRect.Top(), rDestRect.Right(), rDestRect.Top());
  }
  
  if (((mShapeMode == eStrokeShape) || (mShapeMode == eStrokeAndFillShape)) && mStrokeSize)
  {
    pContext->SetFillColor(s);
    
    nuiRect rect(rDestRect.Left(), rDestRect.Top(), rDestRect.GetWidth(), mStrokeSize);
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Left(), rDestRect.Bottom()-mStrokeSize, rDestRect.GetWidth(), mStrokeSize);
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Left(), rDestRect.Top(), mStrokeSize, rDestRect.GetHeight());
    pContext->DrawRect(rect, eFillShape);
    
    rect = nuiRect(rDestRect.Right()-mStrokeSize, rDestRect.Top(), mStrokeSize, rDestRect.GetHeight());
    pContext->DrawRect(rect, eFillShape);
    
  }

  pContext->PopState();
}


void nuiGradientDecoration::SetOrientation(nuiOrientation orientation)
{
  mOrientation = orientation;
  Changed();
}

nuiOrientation nuiGradientDecoration::GetOrientation() const
{
  return mOrientation;
}



nuiRect nuiGradientDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return nuiRect(0,0,0,0);
}



nuiSize nuiGradientDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
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

void nuiGradientDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
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



const nuiRect& nuiGradientDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiGradientDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  Changed();
}


void nuiGradientDecoration::GetColors(nuiColor& color1, nuiColor& color2) const
{
  color1 = mColor1;
  color2 = mColor2;
}

void nuiGradientDecoration::GetColors(nuiColor& color1, nuiColor& color2, nuiColor& color3, nuiColor& color4) const
{
  color1 = mColor1;
  color2 = mColor2;
  color3 = mColor3;
  color4 = mColor4;
}


void nuiGradientDecoration::SetColors(const nuiColor& color1, const nuiColor& color2)
{
  mColor1 = color1;
  mColor2 = color2;
  Changed();
}

void nuiGradientDecoration::SetColors(const nuiColor& color1, const nuiColor& color2, const nuiColor& color3, const nuiColor& color4)
{
  mColor1 = color1;
  mColor2 = color2;
  mColor3 = color3;
  mColor4 = color4;
  Changed();
}

const nuiColor& nuiGradientDecoration::GetColor1() const
{
  return mColor1;
}

void nuiGradientDecoration::SetColor1(const nuiColor& color)
{
  mColor1 = color;
  Changed();
}

const nuiColor& nuiGradientDecoration::GetColor2() const
{
  return mColor2;
}

void nuiGradientDecoration::SetColor2(const nuiColor& color)
{
  mColor2 = color;
  Changed();
}

const nuiColor& nuiGradientDecoration::GetColor3() const
{
  return mColor3;
}

void nuiGradientDecoration::SetColor3(const nuiColor& color)
{
  mColor3 = color;
  mGradientType = nuiGradient4Colors;
  Changed();
}

const nuiColor& nuiGradientDecoration::GetColor4() const
{
  return mColor4;
}

void nuiGradientDecoration::SetColor4(const nuiColor& color)
{
  mColor4 = color;
  mGradientType = nuiGradient4Colors;
  Changed();
}

const nuiColor& nuiGradientDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}

double nuiGradientDecoration::GetOffset1() const
{
  return mOffset1;
}

double nuiGradientDecoration::GetOffset2() const
{
  return mOffset2;
}


double nuiGradientDecoration::GetOffset3() const
{
  return mOffset3;
}


double nuiGradientDecoration::GetOffset4() const
{
  return mOffset4;
}


void nuiGradientDecoration::SetOffset1(double offset)
{
  mOffset1 = offset;
  mUserOffsets = true;
  Changed();
}


void nuiGradientDecoration::SetOffset2(double offset)
{
  mOffset2 = offset;
  mUserOffsets = true;
  Changed();
}


void nuiGradientDecoration::SetOffset3(double offset)
{
  mOffset3 = offset;
  mUserOffsets = true;
  Changed();
}


void nuiGradientDecoration::SetOffset4(double offset)
{
  mOffset4 = offset;
  mUserOffsets = true;
  Changed();
}





void nuiGradientDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
  Changed();
}

uint32 nuiGradientDecoration::GetStrokeSize() const
{
  return mStrokeSize;
}

void nuiGradientDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
  Changed();
}

void nuiGradientDecoration::SetShapeMode(nuiShapeMode mode)
{
  mShapeMode = mode;
  Changed();
}


nuiShapeMode nuiGradientDecoration::GetShapeMode() const
{
  return mShapeMode;
}



