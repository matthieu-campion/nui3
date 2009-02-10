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
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

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
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(pInput, pCodec);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (const nglPath& rPath, nglImageCodec* pCodec)
  : nuiWidget()
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rPath, pCodec);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"),rPath.GetPathName());
  mIgnoreState = false;
}

nuiImage::nuiImage (nglImageInfo& rInfo, bool Clone)
  : nuiWidget()
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rInfo, Clone);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (const nglImage& rImage)
  : nuiWidget()
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(rImage);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}

nuiImage::nuiImage (nglImage* pImage, bool OwnImage)
  : nuiWidget()
{
  if (SetObjectClass(_T("nuiImage")))
    InitAttributes();

  mpTexture = nuiTexture::GetTexture(pImage,OwnImage);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  mIgnoreState = false;
}


void nuiImage::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nglPath&>
   (nglString(_T("Texture")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiImage::GetTexturePath), 
    nuiMakeDelegate(this, &nuiImage::SetTexturePath)));
}


const nglPath& nuiImage::GetTexturePath()
{
  return mTexturePath;
}

void nuiImage::SetTexturePath(const nglPath& rTexturePath)
{
  mTexturePath = rTexturePath;
  
  mpTexture = nuiTexture::GetTexture(mTexturePath, NULL);
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"),mTexturePath.GetPathName());
  mIgnoreState = false;  
  Invalidate();
}

void nuiImage::SetTexture(nuiTexture* pTex)
{  
  mpTexture = pTex;
  mUseAlpha = true;
  mBlendFunc = nuiBlendTransp;
  SetProperty(_T("Source"), _T("Memory Buffer"));
  mIgnoreState = false;  
  Invalidate();
}


bool nuiImage::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  SetObjectClass(_T("nuiImage"));

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
  if (mpTexture)
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
  w = (nuiSize)pImage->GetWidth();
  h = (nuiSize)pImage->GetHeight();

  float alpha = 1.0f;

  if (mUseAlpha)
  {
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(mBlendFunc);
    alpha = GetAlpha();
  }

  nuiRect rect = mRect.Size();

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

  //pContext->EnableClipping(false);
  pContext->SetFillColor(color);
  pContext->DrawImage(rect, nuiRect(0.0f, 0.0f, w, h));

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
