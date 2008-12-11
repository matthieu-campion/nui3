/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiImageDecoration.h"

nuiImageDecoration::nuiImageDecoration(const nglString& rName)
: nuiDecoration(rName),
  mpTexture(NULL),
  mPosition(nuiCenter),
  mBorderEnabled(true),
  mColor(255,255,255,255)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
}

nuiImageDecoration::nuiImageDecoration(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect, nuiPosition position, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(pTexture),
  mPosition(position),
  mClientRect(rClientRect),
  mBorderEnabled(true),
  mColor(rColor)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
}

nuiImageDecoration::nuiImageDecoration(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect, nuiPosition position, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(NULL),
  mPosition(position),
  mClientRect(rClientRect),
  mBorderEnabled(true),
  mColor(rColor)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
	
  mpTexture = nuiTexture::GetTexture(rTexturePath);
  if (mpTexture)
    SetProperty(_T("Texture"), rTexturePath.GetPathName());
}




void nuiImageDecoration::InitAttributes()
{

  nuiAttribute<bool>* AttributeBorder = new nuiAttribute<bool>
   (nglString(_T("EnableBorder")), nuiUnitBoolean,
    nuiAttribute<bool>::GetterDelegate(this, &nuiImageDecoration::IsBorderEnabled),
    nuiAttribute<bool>::SetterDelegate(this, &nuiImageDecoration::EnableBorder));
  
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiImageDecoration::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiImageDecoration::SetSourceClientRect));
  
  nuiAttribute<const nglPath&>* AttributeTexture = new nuiAttribute<const nglPath&>
   (nglString(_T("Texture")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiImageDecoration::GetTexturePath), 
    nuiMakeDelegate(this, &nuiImageDecoration::SetTexturePath));

  nuiAttribute<nuiPosition>* AttributePosition = new nuiAttribute<nuiPosition>
   (nglString(_T("Position")), nuiUnitPosition,
    nuiMakeDelegate(this, &nuiImageDecoration::GetPosition), 
    nuiMakeDelegate(this, &nuiImageDecoration::SetPosition));
  
  nuiAttribute<const nuiColor&>* AttributeColor = new nuiAttribute<const nuiColor&>
  (nglString(_T("Color")), nuiUnitNone,
   nuiAttribute<const nuiColor&>::GetterDelegate(this, &nuiImageDecoration::GetColor), 
   nuiAttribute<const nuiColor&>::SetterDelegate(this, &nuiImageDecoration::SetColor));
  
	AddAttribute(_T("EnableBorder"), AttributeBorder);
	AddAttribute(_T("ClientRect"), AttributeRect);
	AddAttribute(_T("Texture"), AttributeTexture);
	AddAttribute(_T("Position"), AttributePosition);
}



nuiImageDecoration::~nuiImageDecoration()
{
  mpTexture->Release();
}

bool nuiImageDecoration::Load(const nuiXMLNode* pNode)
{
  mClientRect.SetValue(nuiGetString(pNode, _T("ClientRect"), _T("{0,0,0,0}")));
  mpTexture = nuiTexture::GetTexture(nglPath(nuiGetString(pNode, _T("Texture"), nglString::Empty)));
  return true;
}

nuiXMLNode* nuiImageDecoration::Serialize(nuiXMLNode* pNode)
{
  pNode->SetName(_T("nuiImageDecoration"));
  pNode->SetAttribute(_T("ClientRect"), mClientRect.GetValue());

  pNode->SetAttribute(_T("Texture"), GetTexturePath());
  return pNode;
}


nuiPosition nuiImageDecoration::GetPosition()
{
  return mPosition;
}


void nuiImageDecoration::SetPosition(nuiPosition pos)
{
  mPosition = pos;
}


const nglPath& nuiImageDecoration::GetTexturePath() const
{
  if (HasProperty(_T("Texture")))
    return GetProperty(_T("Texture"));

  return mpTexture->GetSource();
}

void nuiImageDecoration::SetTexturePath(const nglPath& rPath)
{
  SetProperty(_T("Texture"), rPath.GetPathName());
  nuiTexture* pOld = mpTexture;
  mpTexture = nuiTexture::GetTexture(rPath);
  if (GetSourceClientRect() == nuiRect(0,0,0,0))
    SetSourceClientRect(nuiRect(0, 0, mpTexture->GetWidth(), mpTexture->GetHeight()));
  if (pOld)
    pOld->Release();
}

const nuiColor& nuiImageDecoration::GetColor() const
{
  return mColor;
}

void nuiImageDecoration::SetColor(const nuiColor& rColor)
{
  mColor = rColor;
}

void nuiImageDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  if (!mpTexture || !mpTexture->GetImage() || !mpTexture->GetImage()->GetPixelSize())
    return;

  nuiRect rect = mClientRect;
  rect.SetPosition(mPosition, rDestRect);
  rect.RoundToBelow();
    
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture);
  pContext->SetFillColor(mColor);
  pContext->DrawImage(rect, mClientRect);
}





const nuiRect& nuiImageDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiImageDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}

void nuiImageDecoration::EnableBorder(bool set)
{
  mBorderEnabled = set;
}

bool nuiImageDecoration::IsBorderEnabled() const
{
  return mBorderEnabled;
}

nuiSize nuiImageDecoration::GetBorder(nuiPosition position) const
{
  if (!mBorderEnabled)
    return 0;
  
  nuiSize w = 1.0, h = 1.0;
  mpTexture->TextureToImageCoord(w, h);
  switch (position)
  {
  case nuiLeft:
    return mClientRect.Left();
    break;
  case nuiRight:
    return w - mClientRect.Right();
    break;
  case nuiTop:
    return mClientRect.Top();
    break;
  case nuiBottom:
    return h - mClientRect.Bottom();
    break;
  case nuiFillHorizontal:
    return w - mClientRect.GetWidth();
    break;
  case nuiFillVertical:
    return h - mClientRect.GetHeight();
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


nuiTexture* nuiImageDecoration::GetTexture() const
{
  return mpTexture;
}



nuiRect nuiImageDecoration::GetIdealClientRect() const
{
  return mClientRect.Size();
}




