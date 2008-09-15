/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiBorderDecoration.h"

nuiBorderDecoration::nuiBorderDecoration(const nglString& rName)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiBorderDecoration")))
    InitAttributes();

  mStrokeSize = 0;
  mStrokeColor = nuiColor(0,0,0);
  mBorderType = eBorderAll;
  mBorderMode = eBorderNormal;
  mBlendFunc = nuiBlendTransp;
  mClientRect = nuiRect(0,0,0,0);
  
  mUseStrokeGlobalColor = true;
  mUseStrokeLeftColor  = false;
  mUseStrokeRightColor  = false;
  mUseStrokeTopColor  = false;
  mUseStrokeBottomColor  = false;
}

nuiBorderDecoration::~nuiBorderDecoration()
{
}


void nuiBorderDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiBorderDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiBorderDecoration::SetSourceClientRect)));

  AddAttribute(new nuiAttribute<uint32>
  (nglString(_T("StrokeSize")), nuiUnitPixels,
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeSize),
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeSize)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeColor), 
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeColor)));

  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeLeftColor")), nuiUnitColor,
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeLeftColor), 
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeLeftColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeRightColor")), nuiUnitColor,
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeRightColor), 
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeRightColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeTopColor")), nuiUnitColor,
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeTopColor), 
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeTopColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeBottomColor")), nuiUnitColor,
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetStrokeBottomColor), 
                nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetStrokeBottomColor)));
  
  
  AddAttribute(new nuiAttribute<nglString>
  (nglString(_T("Border")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetBorderType), 
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetBorderType)));

  AddAttribute(new nuiAttribute<nglString>
  (nglString(_T("BorderMode")), nuiUnitNone,
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::GetBorderMode), 
   nuiFastDelegate::MakeDelegate(this, &nuiBorderDecoration::SetBorderMode)));
}



bool nuiBorderDecoration::Load(const nuiXMLNode* pNode)
{
  //#FIXME
  return true;
}

nuiXMLNode* nuiBorderDecoration::Serialize(nuiXMLNode* pNode)
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
void nuiBorderDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(mBlendFunc);
  pContext->EnableTexturing(false);

  pContext->SetLineWidth(mStrokeSize);
  
  if ((mBorderType == eBorderAll) && mUseStrokeGlobalColor)
  {
    pContext->SetStrokeColor(mStrokeColor);
    pContext->DrawRect(rDestRect, eStrokeShape);
    return;
  }


  // left
  if ((mBorderType == eBorderAll) || (mBorderType == eBorderLeft) || (mBorderType == eBorderVertical))
  {
    if (mUseStrokeLeftColor)
      pContext->SetStrokeColor(mStrokeLeftColor);
    else
      pContext->SetStrokeColor(mStrokeColor);
      
    pContext->DrawLine(rDestRect.Left(), rDestRect.Top(), rDestRect.Left(), rDestRect.Bottom());
  }

  // right
  if ((mBorderType == eBorderAll) || (mBorderType == eBorderRight) || (mBorderType == eBorderVertical))
  {
    if (mUseStrokeRightColor)
      pContext->SetStrokeColor(mStrokeRightColor);
    else
      pContext->SetStrokeColor(mStrokeColor);
    
    pContext->DrawLine(rDestRect.Right()-1, rDestRect.Top(), rDestRect.Right()-1, rDestRect.Bottom());
  }
  
  //top
  if ((mBorderType == eBorderAll) || (mBorderType == eBorderTop) || (mBorderType == eBorderHorizontal))
  {
    if (mUseStrokeTopColor)
      pContext->SetStrokeColor(mStrokeTopColor);
    else
      pContext->SetStrokeColor(mStrokeColor);
    
    pContext->DrawLine(rDestRect.Left(), rDestRect.Top(), rDestRect.Right(), rDestRect.Top());
  }

  // bottom
  if ((mBorderType == eBorderAll) || (mBorderType == eBorderBottom) || (mBorderType == eBorderHorizontal))
  {
    if (mUseStrokeBottomColor)
      pContext->SetStrokeColor(mStrokeBottomColor);
    else
      pContext->SetStrokeColor(mStrokeColor);
    
    pContext->DrawLine(rDestRect.Left(), rDestRect.Bottom()-1, rDestRect.Right(), rDestRect.Bottom()-1);
  }
  
  
}



nuiRect nuiBorderDecoration::GetIdealClientRect() const
{
  return mClientRect.Size();
}



nuiSize nuiBorderDecoration::GetBorder(nuiPosition position) const
{
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



//*************************************************************************
//
// attributes setters/getters
//


const nuiRect& nuiBorderDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiBorderDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}



const nuiColor& nuiBorderDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}


void nuiBorderDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
}




const nuiColor& nuiBorderDecoration::GetStrokeLeftColor() const
{
  return mStrokeLeftColor;
}


void nuiBorderDecoration::SetStrokeLeftColor(const nuiColor& color)
{
  mStrokeLeftColor = color;
  mUseStrokeLeftColor = true;
  mUseStrokeGlobalColor = false;
}




const nuiColor& nuiBorderDecoration::GetStrokeRightColor() const
{
  return mStrokeRightColor;
}


void nuiBorderDecoration::SetStrokeRightColor(const nuiColor& color)
{
  mStrokeRightColor = color;
  mUseStrokeRightColor = true;
  mUseStrokeGlobalColor = false;
}




const nuiColor& nuiBorderDecoration::GetStrokeTopColor() const
{
  return mStrokeTopColor;
}


void nuiBorderDecoration::SetStrokeTopColor(const nuiColor& color)
{
  mStrokeTopColor = color;
  mUseStrokeTopColor = true;
  mUseStrokeGlobalColor = false;
}




const nuiColor& nuiBorderDecoration::GetStrokeBottomColor() const
{
  return mStrokeBottomColor;
}


void nuiBorderDecoration::SetStrokeBottomColor(const nuiColor& color)
{
  mStrokeBottomColor = color;
  mUseStrokeBottomColor = true;
  mUseStrokeGlobalColor = false;
}



void nuiBorderDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
}


uint32 nuiBorderDecoration::GetStrokeSize() const
{
  return mStrokeSize;
}



nglString nuiBorderDecoration::GetBorderType()
{
  switch(mBorderType)
  {
  case eBorderAll:
    return _T("All");
  case eBorderLeft:
    return _T("Left");
  case eBorderRight:
    return _T("Right");
  case eBorderTop:
    return _T("Top");
  case eBorderBottom:
    return _T("Bottom");
  case eBorderHorizontal:
    return _T("Horizontal");
  case eBorderVertical:
    return _T("Vertical");
  case eBorderNone:
    return _T("None");
  default: 
    return _T("error");
  }
}

void nuiBorderDecoration::SetBorderType(nglString type)
{
  if (!type.Compare(_T("All"), false))
    mBorderType = eBorderAll;
  else if (!type.Compare(_T("Left"), false))
    mBorderType = eBorderLeft;
  else if (!type.Compare(_T("Right"), false))
    mBorderType = eBorderRight;
  else if (!type.Compare(_T("Top"), false))
    mBorderType = eBorderTop;
  else if (!type.Compare(_T("Bottom"), false))
    mBorderType = eBorderBottom;
  else if (!type.Compare(_T("Horizontal"), false))
    mBorderType = eBorderHorizontal;
  else if (!type.Compare(_T("Vertical"), false))
    mBorderType = eBorderVertical;
  else if (!type.Compare(_T("None"), false))
    mBorderType = eBorderNone;
  else
    mBorderType = eBorderNone;
}


nglString nuiBorderDecoration::GetBorderMode() const
{
  switch (mBorderMode)
  {
  case eBorderNormal:
    return _T("Normal");
  case eBorder3DIn:
    return _T("3D");
  case eBorder3DOut:
    return _T("3DOut");
  default:
    return _T("error");
  }
}

void nuiBorderDecoration::SetBorderMode(nglString mode)
{
  if (!mode.Compare(_T("Normal"), false))
    mBorderMode = eBorderNormal;
  else if (!mode.Compare(_T("3D"), false))
    mBorderMode = eBorder3DIn;
  else if (!mode.Compare(_T("3DIn"), false))
    mBorderMode = eBorder3DIn;
  else if (!mode.Compare(_T("3DOut"), false))
    mBorderMode = eBorder3DOut;
  else
    mBorderMode = eBorderNormal;
}
