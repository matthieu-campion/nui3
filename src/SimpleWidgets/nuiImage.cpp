/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nglImage.h"
#include "nuiImage.h"
#include "nuiXML.h"
#include "nuiDrawContext.h"
#include "nglString.h"

nuiImage::nuiImage (nuiTexture* pTexture, bool AlreadyAcquired)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));
//  mPosition = nuiFill;
//  mFillRule = nuiFill;

  mpTexture = pTexture;
  if (!AlreadyAcquired && pTexture)
    pTexture->Acquire();

  mUseAlpha = true;

  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (nglIStream* pInput, nglImageCodec* pCodec)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;
  mpTexture = nuiTexture::GetTexture(pInput, pCodec);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (const nglPath& rPath, nglImageCodec* pCodec)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;
  mpTexture = nuiTexture::GetTexture(rPath, pCodec);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"),rPath.GetPathName());
  mIgnoreState = false;
}

nuiImage::nuiImage (nglImageInfo& rInfo, bool Clone)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;
  mpTexture = nuiTexture::GetTexture(rInfo, Clone);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (const nglImage& rImage)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;
  mpTexture = nuiTexture::GetTexture(rImage);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (nglImage* pImage, bool OwnImage)
  : nuiWidget()
{
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;
  mpTexture = nuiTexture::GetTexture(pImage,OwnImage);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

bool nuiImage::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  SetObjectClass(_T("nuiImage"));

//  mPosition = nuiFill;
//  mFillRule = nuiFill;

  mpTexture = nuiTexture::GetTexture(pNode);
  mUseAlpha = true;

  mUseAlpha = nuiGetBool( pNode, _T("UseAlpha"), mUseAlpha);
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"), pNode->GetAttribute(_T("Source")));

  mIgnoreState = nuiGetBool(pNode, _T("IgnoreState"), false);

//  mPosition = nuiGetPosition(pNode,nuiFill);
//  mFillRule = nuiGetPosition(pNode,_T("FillRule"),nuiFill);

  // FIXME: interpret other attributes...
  return true;
}


nuiXMLNode* nuiImage::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode,true);
  if (!pNode) 
    return NULL;
//  pNode->SetAttribute(_T("Position"),mPosition);
//  pNode->SetAttribute(_T("FillRule"),mFillRule);
  pNode->SetAttribute(_T("Source"),GetProperty(_T("Source")));
  pNode->SetAttribute(_T("IgnoreState"), mIgnoreState ? _T("true") : _T("false"));
  return pNode;
}


nuiImage::~nuiImage()
{
  NGL_ASSERT(mpTexture);
  mpTexture->Release();
}

void nuiImage::ForceReload()
{
  mpTexture->ForceReload();
  Invalidate();
}

bool nuiImage::Draw(nuiDrawContext* pContext)
{
//  int x=0,y=0;
  if (!mpTexture || !mpTexture->GetImage() || !mpTexture->GetImage()->GetPixelSize())
    return false;

  pContext->PushState();
  pContext->ResetState();
  pContext->SetTexture(mpTexture);
//  mpTexture->Apply(pContext);
  pContext->EnableTexturing(true);
  nglImage* pImage = mpTexture->GetImage();
  nuiSize w,h;
  w=(nuiSize)pImage->GetWidth();
  h=(nuiSize)pImage->GetHeight();
  nuiRect rect(0.0f,0.0f,w,h);

  float alpha = 1.0f;

  if (mUseAlpha)
  {
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(mBlendFunc);
    alpha = GetAlpha();
  }

  rect = mRect.Size();

  nuiColor color = nuiColor(1.0f, 1.0f, 1.0f, alpha);

  if (!mIgnoreState)
  {
    if (IsSelected())
    {
      color = GetColor(eSelectedImage);
      color.Alpha() = alpha;
    }
    if (IsDisabled())
    {
      color = GetColor(eDisabledImage);
      color.Alpha() = alpha;
    }

    pContext->EnableBlending(true);
  }

  pContext->SetFillColor(color);
  pContext->DrawImage(rect,nuiRect(0.0f,0.0f,w,h));

  pContext->EnableBlending(false);
  pContext->EnableTexturing(false);

  pContext->PopState();
  return true;
}

void nuiImage::SetBlendFunc(nuiBlendFunc Func)
{
  mBlendFunc = Func;
  Invalidate();
}


nuiBlendFunc nuiImage::GetBlendFunc()
{
  return mBlendFunc;
}


nuiRect nuiImage::CalcIdealSize()
{
  if (!mpTexture)
    return mIdealRect;
  nglImage* pImage = mpTexture->GetImage();
  if (!pImage)
    return mIdealRect;

  mIdealRect.Set(0.0f,0.0f,(nuiSize) pImage->GetWidth(),(nuiSize) pImage->GetHeight());
  return mIdealRect;
}


void nuiImage::SetUseAlpha(bool Alpha)
{
  mUseAlpha = Alpha;
  Invalidate();
}

bool nuiImage::GetUseAlpha()
{
  return mUseAlpha;
}

nglImage* nuiImage::GetImage()
{
  if (!mpTexture)
    return NULL;
  return mpTexture->GetImage();
}

nuiTexture* nuiImage::GetTexture()
{
  return mpTexture;
}
