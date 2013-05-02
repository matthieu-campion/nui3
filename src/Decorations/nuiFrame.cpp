/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiFrame::nuiFrame(const nglString& rName)
: nuiDecoration(rName),
  mpTexture(NULL),
  mNeedUpdate(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
  mColor = nuiColor(255,255,255);
  mUseWidgetFrameColor = false;
  mDebug = false;
  mInterpolated = true;
}

nuiFrame::nuiFrame(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(pTexture),
  mColor(rColor),
  mUseWidgetFrameColor(false),
  mClientRect(rClientRect),
  mNeedUpdate(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
  mDebug = false;
  mInterpolated = true;
  if (mpTexture)
    mTexturePath = mpTexture->GetSource();
}

nuiFrame::nuiFrame(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(NULL),
  mColor(rColor),
  mUseWidgetFrameColor(false),
  mClientRect(rClientRect),
  mNeedUpdate(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
	
  SetTexturePath(rTexturePath.GetPathName());
  mDebug = false;
  mInterpolated = true;
}




void nuiFrame::InitAttributes()
{
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiFrame::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiFrame::SetSourceClientRect));
  
  nuiAttribute<const nuiColor&>* AttributeColor = new nuiAttribute<const nuiColor&>
   (nglString(_T("Color")), nuiUnitNone,
    nuiAttribute<const nuiColor&>::GetterDelegate(this, &nuiFrame::GetColor), 
    nuiAttribute<const nuiColor&>::SetterDelegate(this, &nuiFrame::SetColor));

  nuiAttribute<bool>* AttributeFrameColor = new nuiAttribute<bool>
  (nglString(_T("UseWidgetFrameColor")), nuiUnitBoolean,
   nuiAttribute<bool>::GetterDelegate(this, &nuiFrame::IsWidgetFrameColorUsed),
   nuiAttribute<bool>::SetterDelegate(this, &nuiFrame::UseWidgetFrameColor));
  
  nuiAttribute<const nglPath&>* AttributeTexture = new nuiAttribute<const nglPath&>
   (nglString(_T("Texture")), nuiUnitNone,
    nuiMakeDelegate(this, &nuiFrame::GetTexturePath), 
    nuiMakeDelegate(this, &nuiFrame::SetTexturePath));

  nuiAttribute<bool>* AttributeInterpolation = new nuiAttribute<bool>
   (nglString(_T("Interpolation")), nuiUnitBoolean,
    nuiAttribute<bool>::GetterDelegate(this, &nuiFrame::IsInterpolated),
    nuiAttribute<bool>::SetterDelegate(this, &nuiFrame::SetInterpolated));
  

	AddAttribute(_T("ClientRect"), AttributeRect);
	AddAttribute(_T("Color"), AttributeColor);
  AddAttribute(_T("UseWidgetFrameColor"), AttributeFrameColor);
	AddAttribute(_T("Texture"), AttributeTexture);
	AddAttribute(_T("Interpolation"), AttributeInterpolation);
}


void nuiFrame::SetDebug(bool set)
{
  mDebug = set;
}


nuiFrame::~nuiFrame()
{
  mpTexture->Release();
}

const nglPath& nuiFrame::GetTexturePath() const
{
  return mTexturePath;
}

void nuiFrame::SetTexturePath(const nglPath& rPath)
{
  mTexturePath = rPath;
  nuiTexture* pOld = mpTexture;
  mpTexture = nuiTexture::GetTexture(rPath);

  if (!mpTexture || !mpTexture->IsValid())
  {
    NGL_OUT(_T("nuiFrame::SetTexturePath warning : could not load graphic resource '%s'\n"), rPath.GetChars());
    return;
  }

  if (GetSourceClientRect() == nuiRect(0,0,0,0))
    SetSourceClientRect(nuiRect(0, 0, mpTexture->GetWidth(), mpTexture->GetHeight()));

  if (pOld)
    pOld->Release();
    
  if (mInterpolated)
  {
    mpTexture->SetMinFilter(GL_LINEAR);
    mpTexture->SetMagFilter(GL_LINEAR);
  }
  else
  {
    mpTexture->SetMinFilter(GL_NEAREST);
    mpTexture->SetMagFilter(GL_NEAREST);
  }
  
  mNeedUpdate = true;
  Changed();
}


// virtual
void nuiFrame::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  pContext->PushState();
  pContext->ResetState();

  //  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLES);
  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLE_STRIP);
  pArray->EnableArray(nuiRenderArray::eVertex, true);
  pArray->EnableArray(nuiRenderArray::eTexCoord, true);
  pArray->EnableArray(nuiRenderArray::eColor, false);
  
  
  nuiSize w = mpTexture->GetWidth();
  nuiSize h = mpTexture->GetHeight();
  
  float X0 = 0;
  float X1 = (float)mClientRect.Left();
  float X2 = (float)mClientRect.Right();
  float X3 = (float)w;

  float Y0 = 0;
  float Y1 = (float)mClientRect.Top();
  float Y2 = (float)mClientRect.Bottom();
  float Y3 = (float)h;

  const float x0 = (float)rDestRect.Left();
  const float x1 = x0 + X1;
  const float x3 = (float)rDestRect.Right();
  const float x2 = x3 - (X3 - X2);
  
  const float y0 = (float)rDestRect.Top();
  const float y1 = y0 + Y1;
  const float y3 = (float)rDestRect.Bottom();
  const float y2 = y3 - (Y3 - Y2);

  UpdateTextureCoordinates();
  
  // Reserve 28 vertices
  pArray->Reserve(28);
  
  //////// TOP PART
  // TopLeft rect:
  pArray->SetVertex(x0, y0);
  pArray->SetTexCoords(mTx[0], mTy[0]);
  pArray->PushVertex();
  
  pArray->SetVertex(x0, y1);
  pArray->SetTexCoords(mTx[1], mTy[1]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y0);
  pArray->SetTexCoords(mTx[2], mTy[2]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(mTx[3], mTy[3]);
  pArray->PushVertex();
  
  // TopMiddle rect:
  pArray->SetVertex(x2, y0);
  pArray->SetTexCoords(mTx[4], mTy[4]);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(mTx[5], mTy[5]);
  pArray->PushVertex();
  
  // TopRight rect:
  pArray->SetVertex(x3, y0);
  pArray->SetTexCoords(mTx[6], mTy[6]);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y1);
  pArray->SetTexCoords(mTx[7], mTy[7]);
  pArray->PushVertex();
  // Double the last vertex to create a flat triangle that will not be displayed
  pArray->PushVertex();
  
  ///// MIDDLE PART
  // LeftSide rect:
  pArray->SetVertex(x0, y1);
  pArray->SetTexCoords(mTx[8], mTy[8]);
  pArray->PushVertex();
  // Double the first vertex to create a flat triangle that will not be displayed
  pArray->PushVertex();
  
  pArray->SetVertex(x0, y2);
  pArray->SetTexCoords(mTx[9], mTy[9]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(mTx[10], mTy[10]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(mTx[11], mTy[11]);
  pArray->PushVertex();
  
  // Middle (client) rect:
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(mTx[12], mTy[12]);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(mTx[13], mTy[13]);
  pArray->PushVertex();
  
  // RightSide rect:
  pArray->SetVertex(x3, y1);
  pArray->SetTexCoords(mTx[14], mTy[14]);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y2);
  pArray->SetTexCoords(mTx[15], mTy[15]);
  pArray->PushVertex();
  // Double the last vertex to create a flat triangle that will not be displayed
  pArray->PushVertex();
  
  
  
  ///// BOTTOM PART
  // BottomLeft rect:
  pArray->SetVertex(x0, y2);
  pArray->SetTexCoords(mTx[16], mTy[16]);
  pArray->PushVertex();
  // Double the first vertex to create a flat triangle that will not be displayed
  pArray->PushVertex();
  
  pArray->SetVertex(x0, y3);
  pArray->SetTexCoords(mTx[17], mTy[17]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(mTx[18], mTy[18]);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y3);
  pArray->SetTexCoords(mTx[19], mTy[19]);
  pArray->PushVertex();
  
  // BottomMiddle rect:
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(mTx[20], mTy[20]);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y3);
  pArray->SetTexCoords(mTx[21], mTy[21]);
  pArray->PushVertex();
  
  // BottomRight rect:
  pArray->SetVertex(x3, y2);
  pArray->SetTexCoords(mTx[22], mTy[22]);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y3);
  pArray->SetTexCoords(mTx[23], mTy[23]);
  pArray->PushVertex();
  
  
  nuiColor color = mColor;
  
  if (pWidget)
  {
    if (mUseWidgetFrameColor)
    {
      nuiAttrib<const nuiColor&> colorAttr(pWidget->GetAttribute(_T("FrameColor")));
      if (colorAttr.IsValid())
      {
        color = colorAttr.Get();
      }
    }
    if (mUseWidgetAlpha)
    {
      float widgetAlpha = pWidget->GetMixedAlpha();
      color.Multiply(widgetAlpha);
    }
  }
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture);
  pContext->SetFillColor(color);
  pContext->DrawArray(pArray);
  
  pContext->PopState();
}


const nuiRect& nuiFrame::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiFrame::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;

  mNeedUpdate = true;
  Changed();
}

void nuiFrame::UpdateTextureCoordinates()
{
  if (!mNeedUpdate)
    return;
  // Precalc texture coordinates:
  const nuiSize w = mpTexture->GetWidth();
  const nuiSize h = mpTexture->GetHeight();
  
  const float X0 = 0;
  const float X1 = (float)mClientRect.Left();
  const float X2 = (float)mClientRect.Right();
  const float X3 = (float)w;
  
  const float Y0 = 0;
  const float Y1 = (float)mClientRect.Top();
  const float Y2 = (float)mClientRect.Bottom();
  const float Y3 = (float)h;
  
  mTx[ 0] = X0; mTy[ 0] = Y0;
  mTx[ 1] = X0; mTy[ 1] = Y1;
  mTx[ 2] = X1; mTy[ 2] = Y0;
  mTx[ 3] = X1; mTy[ 3] = Y1;
  mTx[ 4] = X2; mTy[ 4] = Y0;
  mTx[ 5] = X2; mTy[ 5] = Y1;
  mTx[ 6] = X3; mTy[ 6] = Y0;
  mTx[ 7] = X3; mTy[ 7] = Y1;
  mTx[ 8] = X0; mTy[ 8] = Y1; // dupe!
  mTx[ 9] = X0; mTy[ 9] = Y2;
  mTx[10] = X1; mTy[10] = Y1; // dupe!
  mTx[11] = X1; mTy[11] = Y2;
  mTx[12] = X2; mTy[12] = Y1; // dupe!
  mTx[13] = X2; mTy[13] = Y2;
  mTx[14] = X3; mTy[14] = Y1; // dupe!
  mTx[15] = X3; mTy[15] = Y2;
  mTx[16] = X0; mTy[16] = Y2;
  mTx[17] = X0; mTy[17] = Y3;
  mTx[18] = X1; mTy[18] = Y2;
  mTx[19] = X1; mTy[19] = Y3;
  mTx[20] = X2; mTy[20] = Y2;
  mTx[21] = X2; mTy[21] = Y3;
  mTx[22] = X3; mTy[22] = Y2; // dupe!
  mTx[23] = X3; mTy[23] = Y3;
  
  for (int32 i = 0; i < 24; i++)
  {
    mpTexture->ImageToTextureCoord(mTx[i], mTy[i]);
    NGL_ASSERT(!std::isnan(mTx[i]));
    NGL_ASSERT(!std::isnan(mTy[i]));
  }
  
  mNeedUpdate = false;
}


nuiSize nuiFrame::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  if (!mBorderEnabled)
    return 0;
  
//  nuiSize w = 1.0, h = 1.0;
//  mpTexture->TextureToImageCoord(w, h);
  nuiSize w = mpTexture->GetWidth(), h = mpTexture->GetHeight();
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
  default:
    break;
  }
  //we should'nt arrive here
  return NULL;
}

void nuiFrame::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mBorderEnabled)
  {
    rLeft = rRight = rTop = rBottom = rHorizontal = rVertical = 0;
    return;
  }
  
  nuiSize w = 1.0, h = 1.0;
  mpTexture->TextureToImageCoord(w, h);
  rLeft = mClientRect.Left();
  rRight = w - mClientRect.Right();
  rTop = mClientRect.Top();
  rBottom = h - mClientRect.Bottom();
  rHorizontal = w - mClientRect.GetWidth();
  rVertical = h - mClientRect.GetHeight();
}


nuiTexture* nuiFrame::GetTexture() const
{
  return mpTexture;
}


const nuiColor& nuiFrame::GetColor() const
{
	return mColor;
}

void nuiFrame::SetColor(const nuiColor& color)
{
	mColor = color;
  Changed();
}

bool nuiFrame::IsWidgetFrameColorUsed() const
{
  return mUseWidgetFrameColor;
}

void nuiFrame::UseWidgetFrameColor(bool Use)
{
  mUseWidgetFrameColor = Use;
}


nuiRect nuiFrame::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return mClientRect.Size();
}


bool nuiFrame::IsInterpolated()
{
  return mInterpolated;
}

void nuiFrame::SetInterpolated(bool set)
{
  mInterpolated = set;
  
  if (mpTexture)
  {
    if (mInterpolated)
    {
      mpTexture->SetMinFilter(GL_LINEAR);
      mpTexture->SetMagFilter(GL_LINEAR);
    }
    else
    {
      mpTexture->SetMinFilter(GL_NEAREST);
      mpTexture->SetMagFilter(GL_NEAREST);    
    }
  }
  Changed();
}


