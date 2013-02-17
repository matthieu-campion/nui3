/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#define TEXTURE_SIZE 1024

nuiHugeImage::nuiHugeImage(const nglPath& rImagePath)
{
  if (SetObjectClass(_T("nuiHugeImage")))
  {
    InitAttributes();
  }

  
  mClicked = false;
  mZoom = 1.0f;
  mMinZoom = 0.2f;
  mMaxZoom = 1.0f;
  mX = 0.0f;
  mY = 0.0f;

  mpZoom = new nuiAttributeAnimation();
  mpZoom->SetTargetObject(this);
  mpZoom->SetTargetAttribute(_T("Zoom"));
  mpZoom->SetCaptureStartOnPlay(true);
  mpZoom->SetEndValue(mMaxZoom);
  mpZoom->SetEasing(nuiEasingSinus);
  mpZoom->SetDuration(1.0f);
  AddAnimation(_T("Zoom"), mpZoom);
  
  mpPanX = new nuiAttributeAnimation();
  mpPanX->SetTargetObject(this);
  mpPanX->SetTargetAttribute(_T("CenterX"));
  mpPanX->SetCaptureStartOnPlay(true);
  mpPanX->SetEndValue(0);
  mpPanX->SetEasing(nuiEasingSinus);
  mpPanX->SetDuration(1.0f);
  AddAnimation(_T("PanX"), mpPanX);

  mpPanY = new nuiAttributeAnimation();
  mpPanY->SetTargetObject(this);
  mpPanY->SetTargetAttribute(_T("CenterY"));
  mpPanY->SetCaptureStartOnPlay(true);
  mpPanY->SetEndValue(0);
  mpPanY->SetEasing(nuiEasingSinus);
  mpPanY->SetDuration(1.0f);
  AddAnimation(_T("PanY"), mpPanY);
  
  InitImage();
  Load(rImagePath);
  //StartAnimation(_T("Zoom"));
}

nuiHugeImage::~nuiHugeImage()
{
  
}

void nuiHugeImage::InitAttributes()
{
  AddAttribute(new nuiAttribute<float>(nglString(_T("Zoom")), nuiUnitCustom,
                                       nuiMakeDelegate(this, &nuiHugeImage::GetZoom),
                                       nuiMakeDelegate(this, &nuiHugeImage::SetZoom)));
  
  AddAttribute(new nuiAttribute<float>(nglString(_T("MinZoom")), nuiUnitCustom,
                                       nuiMakeDelegate(this, &nuiHugeImage::GetMinZoom),
                                       nuiMakeDelegate(this, &nuiHugeImage::SetMaxZoom)));
  
  AddAttribute(new nuiAttribute<float>(nglString(_T("MaxZoom")), nuiUnitCustom,
                                       nuiMakeDelegate(this, &nuiHugeImage::GetMaxZoom),
                                       nuiMakeDelegate(this, &nuiHugeImage::SetMaxZoom)));
  
  AddAttribute(new nuiAttribute<float>(nglString(_T("CenterX")), nuiUnitCustom,
                                       nuiMakeDelegate(this, &nuiHugeImage::GetCenterX),
                                       nuiMakeDelegate(this, &nuiHugeImage::SetCenterX)));

  AddAttribute(new nuiAttribute<float>(nglString(_T("CenterY")), nuiUnitCustom,
                                       nuiMakeDelegate(this, &nuiHugeImage::GetCenterY),
                                       nuiMakeDelegate(this, &nuiHugeImage::SetCenterY)));
}


bool nuiHugeImage::Load(const nglPath& rImagePath)
{
  ClearImage();
  
  // Load the big image:
  nglIStream* pStream = rImagePath.OpenRead();
  nglImage* pImage = new nglImage(pStream);
  delete pStream;
  if (!pImage)
    return false;
  
  int32 w = pImage->GetWidth();
  int32 h = pImage->GetHeight();

  mTextures.resize((w / TEXTURE_SIZE) + 1);

  mImageSize.Set(w, h);
  
  int32 i = 0;
  for (int32 x = 0; x < w; x += TEXTURE_SIZE, i++)
  {
    int32 j = 0;
    mTextures[i].resize((h / TEXTURE_SIZE) + 1);
    for (int32 y = 0; y < h; y += TEXTURE_SIZE, j++)
    {
      int32 sx = MIN(TEXTURE_SIZE, w - x);
      int32 sy = MIN(TEXTURE_SIZE, h - y);
      nglImage* pCrop = pImage->Crop(x, y, sx, sy);
      //NGL_OUT(_T("Crop[%d][%d] 0x%x\n"), i, j, pCrop);
      mTextures[i][j] = nuiTexture::GetTexture(pCrop, true);
//       mTextures[i][j]->EnableAutoMipMap(true);
//       mTextures[i][j]->SetMagFilter(GL_LINEAR);
//       mTextures[i][j]->SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
    }
  }
  
  delete pImage;
  
  mZoom = 1.0f;
  mX = w / 2;
  mY = h / 2;
  
  InvalidateLayout();
  return true;
}



nuiRect nuiHugeImage::CalcIdealSize()
{
  return mImageSize;
}

bool nuiHugeImage::Draw(nuiDrawContext* pContext)
{
  pContext->Translate(mRect.GetWidth()/2, mRect.GetHeight()/2, 0.0f);
  pContext->Scale(mZoom, mZoom, 1.0f);
  pContext->Translate(-mX, -mY, 0.0f);
  pContext->EnableTexturing(true);
  pContext->SetFillColor(nuiColor(255, 255, 255));
  
  for (int32 i = 0; i < mTextures.size(); i++)
  {
    for (int32 j = 0; j < mTextures[i].size(); j++)
    {
      nuiTexture* pTexture = mTextures[i][j];
      if (pTexture)
      {
        int32 x = i * TEXTURE_SIZE;
        int32 y = j * TEXTURE_SIZE;
        int32 w = pTexture->GetWidth();
        int32 h = pTexture->GetHeight();
        nuiRect src(w, h);
        nuiRect dest(x, y, w, h);
        //NGL_OUT(_T("%d x %d - %s\n"), i, j, dest.GetValue().GetChars());
        pContext->SetTexture(pTexture);
        pContext->DrawImage(dest, src);
      }
    }
  }

  return true;
}

#define WHEEL_ZOOM 1.07f

bool nuiHugeImage::MouseClicked(const nglMouseInfo& rInfo)
{
  if ((rInfo.Buttons & nglMouseInfo::ButtonLeft)
   && (rInfo.Buttons & nglMouseInfo::ButtonDoubleClick))
  {
    float zoom = 0;
    int32 x = mX;
    int32 y = mY;
    if (mZoom == mMinZoom)
    {
      zoom = mMaxZoom;
    }
    else
    {
      zoom = mMinZoom;
      x += (rInfo.X - (mRect.GetWidth() / 2)) / mZoom;
      y += (rInfo.Y - (mRect.GetHeight() / 2)) / mZoom;
    }
    
    StopAnimation(_T("Zoom"));
    mpZoom->SetEndValue(zoom);
    StartAnimation(_T("Zoom"));
    
    if (mX != x)
    {
      StopAnimation(_T("PanX"));
      mpPanX->SetEndValue(x);
      StartAnimation(_T("PanX"));
    }

    if (mY != y)
    {
      StopAnimation(_T("PanY"));
      mpPanY->SetEndValue(y);
      StartAnimation(_T("PanY"));
    }    
    Invalidate();
  }
  else if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    Grab();
    mLastX = rInfo.X;
    mLastY = rInfo.Y;
  }
  else if (rInfo.Buttons & nglMouseInfo::ButtonWheelUp)
  {
    SetZoom(nuiClamp(mZoom * WHEEL_ZOOM, mMinZoom, mMaxZoom));
  }
  else if (rInfo.Buttons & nglMouseInfo::ButtonWheelDown)
  {
    SetZoom(nuiClamp(mZoom / WHEEL_ZOOM, mMinZoom, mMaxZoom));
  }
  return false;
}

bool nuiHugeImage::MouseUnclicked(const nglMouseInfo& rInfo)
{
  if (rInfo.Buttons & nglMouseInfo::ButtonLeft)
  {
    mClicked = false;
    Ungrab();
  }
  return false;
}

bool nuiHugeImage::MouseMoved(const nglMouseInfo& rInfo)
{
  if (mClicked)
  {
    mX += (mLastX - rInfo.X) / mZoom;
    mY += (mLastY - rInfo.Y) / mZoom;
    mLastX = rInfo.X;
    mLastY = rInfo.Y;
    Invalidate();
  }
  return false;
}

void nuiHugeImage::InitImage()
{
  for (int32 i = 0; i < mTextures.size(); i++)
  {
    for (int32 j = 0; j < mTextures[i].size(); j++)
    {
      mTextures[i][j] = NULL;
    }
  }
}

void nuiHugeImage::ClearImage()
{
  for (int32 i = 0; i < mTextures.size(); i++)
  {
    for (int32 j = 0; j < mTextures[i].size(); j++)
    {
      nuiTexture* pTexture = mTextures[i][j];
      if (pTexture)
      {
        pTexture->Release();
        mTextures[i][j] = NULL;
      }
    }
  }
}

void nuiHugeImage::ZoomTo(float zoom)
{
  mpZoom->SetEndValue(zoom);
  StopAnimation(_T("Zoom"));
  StartAnimation(_T("Zoom"));
  Invalidate();
}

void nuiHugeImage::SetZoom(float zoom)
{
  mZoom = nuiClamp(zoom, mMinZoom, mMaxZoom);
  Invalidate();
}

float nuiHugeImage::GetZoom() const
{
  return mZoom;
}

void nuiHugeImage::SetCenterX(float set)
{
  mX = set;
  Invalidate();
}

float nuiHugeImage::GetCenterX() const
{
  return mX;
}

void nuiHugeImage::SetCenterY(float set)
{
  mY = set;
  Invalidate();
}

float nuiHugeImage::GetCenterY() const
{
  return mX;
}

float nuiHugeImage::GetMinZoom() const
{
  return mMinZoom;
}

float nuiHugeImage::GetMaxZoom() const
{
  return mMaxZoom;
}

void nuiHugeImage::SetMinZoom(float set)
{
  mMinZoom = set;
  mZoom = nuiClamp(mZoom, mMinZoom, mMaxZoom);
  Invalidate();
}

void nuiHugeImage::SetMaxZoom(float set)
{
  mMaxZoom = set;
  mZoom = nuiClamp(mZoom, mMinZoom, mMaxZoom);
  Invalidate();
}

void nuiHugeImage::Pan(nuiPosition dir)
{
  float displacement = 10;
  switch (dir)
  {
    case nuiTopLeft:
      mX -= displacement;
      mY -= displacement;
      break;
    case nuiTop:
      mY -= displacement;
      break;
    case nuiTopRight:
      mX += displacement;
      mY -= displacement;
      break;
    case nuiRight:
      mX += displacement;
      break;
    case nuiBottomRight:
      mX += displacement;
      mY += displacement;
      break;
    case nuiBottom:
      mY += displacement;
      break;
    case nuiBottomLeft:
      mX -= displacement;
      mY += displacement;
      break;
    case nuiLeft:
      mX -= displacement;
      break;
    default:
      mX = mImageSize.GetWidth() / 2;
      mY = mImageSize.GetHeight() / 2;
      break;
  }
  
  Invalidate();
}

