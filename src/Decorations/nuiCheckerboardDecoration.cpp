/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiCheckerboardDecoration.h"

nuiCheckerboardDecoration::nuiCheckerboardDecoration(const nglString& rName)
: nuiDecoration(rName)
{
  if (SetObjectClass(_T("nuiCheckerboardDecoration")))
    InitAttributes();

  mStrokeSize = 1;
  mStrokeColor = nuiColor(0,0,0);
  mClientRect = nuiRect(0,0,0,0);
}

nuiCheckerboardDecoration::~nuiCheckerboardDecoration()
{
}


void nuiCheckerboardDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiCheckerboardDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiCheckerboardDecoration::SetSourceClientRect)));

  AddAttribute(new nuiAttribute<uint32>
  (nglString(_T("StrokeSize")), nuiUnitPixels,
   nuiMakeDelegate(this, &nuiCheckerboardDecoration::GetStrokeSize),
   nuiMakeDelegate(this, &nuiCheckerboardDecoration::SetStrokeSize)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
  (nglString(_T("StrokeColor")), nuiUnitColor,
   nuiMakeDelegate(this, &nuiCheckerboardDecoration::GetStrokeColor), 
   nuiMakeDelegate(this, &nuiCheckerboardDecoration::SetStrokeColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("BackgroundColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::GetBackgroundColor), 
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::SetBackgroundColor)));

  AddAttribute(new nuiAttribute<uint32>
               (nglString(_T("TilesPerLine")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::GetTilesPerLine), 
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::SetTilesPerLine)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("TileColor")), nuiUnitColor,
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::GetTileColor), 
                nuiMakeDelegate(this, &nuiCheckerboardDecoration::SetTileColor)));
}



bool nuiCheckerboardDecoration::Load(const nuiXMLNode* pNode)
{
  //#FIXME
  return true;
}

nuiXMLNode* nuiCheckerboardDecoration::Serialize(nuiXMLNode* pNode)
{
//#FIXME
  return pNode;
}

// virtual
void nuiCheckerboardDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);
  
  // background
  pContext->SetFillColor(mBackgroundColor);
  pContext->DrawRect(rDestRect, eFillShape);
  
  
  // checkerboard
  nuiSize clientWidth = rDestRect.GetWidth() - mStrokeSize;
  nuiSize tileWidth = clientWidth / (float)mTilesPerLine;

  nuiRect tileRect = nuiRect(mStrokeSize, mStrokeSize, tileWidth, tileWidth);
  
  pContext->SetFillColor(mTileColor);
  
  
  // mTilesPerLine <=> nb of columns AND nb of rows
  for (uint32 row = 0; row < mTilesPerLine; row++)
  {
    
    uint32 firstCol = (row & 1)? 0 : 1;
    
    for (uint32 col = firstCol; col < mTilesPerLine; col+=2)
    {
      tileRect.MoveTo((col * tileWidth)+mStrokeSize, tileRect.Top());
      pContext->DrawRect(tileRect, eFillShape);
    }

    // go to next line
    tileRect.Move(0, tileWidth);   
  }

  
  // border
  if (mStrokeSize != 0)
  {
    pContext->SetLineWidth(mStrokeSize);  
    pContext->SetStrokeColor(mStrokeColor);
    pContext->DrawRect(rDestRect, eStrokeShape);
  }
  
  
}



nuiRect nuiCheckerboardDecoration::GetIdealClientRect() const
{
  return mClientRect.Size();
}



nuiSize nuiCheckerboardDecoration::GetBorder(nuiPosition position) const
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


const nuiRect& nuiCheckerboardDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiCheckerboardDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}



const nuiColor& nuiCheckerboardDecoration::GetStrokeColor() const
{
  return mStrokeColor;
}


void nuiCheckerboardDecoration::SetStrokeColor(const nuiColor& color)
{
  mStrokeColor = color;
}



const nuiColor& nuiCheckerboardDecoration::GetTileColor() const
{
  return mTileColor;
}


void nuiCheckerboardDecoration::SetTileColor(const nuiColor& color)
{
  mTileColor = color;
}





const nuiColor& nuiCheckerboardDecoration::GetBackgroundColor() const
{
  return mBackgroundColor;
}


void nuiCheckerboardDecoration::SetBackgroundColor(const nuiColor& color)
{
  mBackgroundColor = color;
}





void nuiCheckerboardDecoration::SetStrokeSize(uint32 size)
{
  mStrokeSize = size;
}


uint32 nuiCheckerboardDecoration::GetStrokeSize() const
{
  return mStrokeSize;
}


void nuiCheckerboardDecoration::SetTilesPerLine(uint32 nb)
{
  mTilesPerLine = nb;
}


uint32 nuiCheckerboardDecoration::GetTilesPerLine() const
{
  return mTilesPerLine;
}
