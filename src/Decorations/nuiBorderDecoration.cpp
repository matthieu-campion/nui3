/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

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
   nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeSize),
   nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeSize)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeColor), 
   nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeColor)));

  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeLeftColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeLeftColor), 
                nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeLeftColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeRightColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeRightColor), 
                nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeRightColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeTopColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeTopColor), 
                nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeTopColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("StrokeBottomColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiBorderDecoration::GetStrokeBottomColor), 
                nuiMakeDelegate(this, &nuiBorderDecoration::SetStrokeBottomColor)));
  
  
  AddAttribute(new nuiAttribute<nglString>
  (nglString(_T("Border")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiBorderDecoration::GetBorderType), 
   nuiMakeDelegate(this, &nuiBorderDecoration::SetBorderType)));

  AddAttribute(new nuiAttribute<nglString>
  (nglString(_T("BorderMode")), nuiUnitNone,
   nuiMakeDelegate(this, &nuiBorderDecoration::GetBorderMode), 
   nuiMakeDelegate(this, &nuiBorderDecoration::SetBorderMode)));
}


// virtual
void nuiBorderDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->PushState();
  pContext->ResetState();

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
  
  
  pContext->PopState();
}



nuiRect nuiBorderDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return mClientRect.Size();
}



nuiSize nuiBorderDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
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

void nuiBorderDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
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
  Changed();
}



const nuiColor& nuiBorderDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}


void nuiBorderDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
  Changed();
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
  Changed();
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
  Changed();
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
  Changed();
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
  Changed();
}



void nuiBorderDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
  Changed();
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
  Changed();
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
  Changed();
}
