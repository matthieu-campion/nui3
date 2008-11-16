/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiFrame.h"

nuiFrame::nuiFrame(const nglString& rName)
: nuiDecoration(rName),
  mpTexture(NULL),
  mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
  mColor = nuiColor(255,255,255);
  mDebug = false;
  mInterpolated = true;
}

nuiFrame::nuiFrame(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(pTexture),
  mColor(rColor),
  mClientRect(rClientRect),
  mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
  mDebug = false;
  mInterpolated = true;
}

nuiFrame::nuiFrame(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(NULL),
  mColor(rColor),
  mClientRect(rClientRect),
  mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
	
  mpTexture = nuiTexture::GetTexture(rTexturePath);
  if (mpTexture)
    SetProperty(_T("Texture"), rTexturePath.GetPathName());
  mDebug = false;
  mInterpolated = true;
}




void nuiFrame::InitAttributes()
{
  nuiAttribute<bool>* AttributeBorder = new nuiAttribute<bool>
   (nglString(_T("EnableBorder")), nuiUnitBoolean,
    nuiAttribute<bool>::GetterDelegate(this, &nuiFrame::IsBorderEnabled),
    nuiAttribute<bool>::SetterDelegate(this, &nuiFrame::EnableBorder));
  
  nuiAttribute<const nuiRect&>* AttributeRect = new nuiAttribute<const nuiRect&>
   (nglString(_T("ClientRect")), nuiUnitNone,
    nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiFrame::GetSourceClientRect),
    nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiFrame::SetSourceClientRect));
  
  nuiAttribute<const nuiColor&>* AttributeColor = new nuiAttribute<const nuiColor&>
   (nglString(_T("Color")), nuiUnitNone,
    nuiAttribute<const nuiColor&>::GetterDelegate(this, &nuiFrame::GetColor), 
    nuiAttribute<const nuiColor&>::SetterDelegate(this, &nuiFrame::SetColor));
  
  nuiAttribute<const nglPath&>* AttributeTexture = new nuiAttribute<const nglPath&>
   (nglString(_T("Texture")), nuiUnitNone,
    nuiFastDelegate::MakeDelegate(this, &nuiFrame::GetTexturePath), 
    nuiFastDelegate::MakeDelegate(this, &nuiFrame::SetTexturePath));

  nuiAttribute<bool>* AttributeInterpolation = new nuiAttribute<bool>
   (nglString(_T("Interpolation")), nuiUnitBoolean,
    nuiAttribute<bool>::GetterDelegate(this, &nuiFrame::IsInterpolated),
    nuiAttribute<bool>::SetterDelegate(this, &nuiFrame::SetInterpolated));
  

	AddAttribute(_T("EnableBorder"), AttributeBorder);
	AddAttribute(_T("ClientRect"), AttributeRect);
	AddAttribute(_T("Color"), AttributeColor);
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

bool nuiFrame::Load(const nuiXMLNode* pNode)
{
  mColor.SetValue(nuiGetString(pNode, _T("Color"), _T("white")));
  mClientRect.SetValue(nuiGetString(pNode, _T("ClientRect"), _T("{0,0,0,0}")));
  mpTexture = nuiTexture::GetTexture(nglPath(nuiGetString(pNode, _T("Texture"), nglString::Empty)));
  return true;
}

nuiXMLNode* nuiFrame::Serialize(nuiXMLNode* pNode)
{
  pNode->SetName(_T("nuiFrame"));
  pNode->SetAttribute(_T("Color"), mColor.GetValue());
  pNode->SetAttribute(_T("ClientRect"), mClientRect.GetValue());

  pNode->SetAttribute(_T("Texture"), GetTexturePath());
  return pNode;
}

const nglPath& nuiFrame::GetTexturePath() const
{
  if (HasProperty(_T("Texture")))
    return GetProperty(_T("Texture"));

  return mpTexture->GetSource();
}

void nuiFrame::SetTexturePath(const nglPath& rPath)
{
  SetProperty(_T("Texture"), rPath.GetPathName());
  nuiTexture* pOld = mpTexture;
  mpTexture = nuiTexture::GetTexture(rPath);
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
}


// virtual
void nuiFrame::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  nuiRenderArray array(GL_TRIANGLES);
  array.EnableArray(nuiRenderArray::eVertex, true);
  array.EnableArray(nuiRenderArray::eTexCoord, true);
  array.EnableArray(nuiRenderArray::eColor, false);
  
  
  nuiSize w = mpTexture->GetImage()->GetWidth(), h = mpTexture->GetImage()->GetHeight();
  
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

  mpTexture->ImageToTextureCoord(X0, Y0);
  mpTexture->ImageToTextureCoord(X1, Y1);
  mpTexture->ImageToTextureCoord(X2, Y2);
  mpTexture->ImageToTextureCoord(X3, Y3);
  
  //////// TOP PART
  // TopLeft rect:
  array.SetVertex(x0, y0);
  array.SetTexCoords(X0, Y0);
  array.PushVertex();
  
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();

  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  ///
  array.SetVertex(x0, y0);
  array.SetTexCoords(X0, Y0);
  array.PushVertex();

  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();

  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();
  
  // TopMiddle rect:
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();
  
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  ///
  array.SetVertex(x1, y0);
  array.SetTexCoords(X1, Y0);
  array.PushVertex();

  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();

  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  // TopRight rect:
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();
  
  array.SetVertex(x3, y0);
  array.SetTexCoords(X3, Y0);
  array.PushVertex();
  
  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();

  ///
  array.SetVertex(x2, y0);
  array.SetTexCoords(X2, Y0);
  array.PushVertex();

  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();

  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();

  ///// MIDDLE PART
  // LeftSide rect:
  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();

  ///
  array.SetVertex(x0, y1);
  array.SetTexCoords(X0, Y1);
  array.PushVertex();

  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();

  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();
  
  // Middle (client) rect:
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();

  ///
  array.SetVertex(x1, y1);
  array.SetTexCoords(X1, Y1);
  array.PushVertex();

  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();

  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  // RightSide rect:
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();
  
  array.SetVertex(x3, y1);
  array.SetTexCoords(X3, Y1);
  array.PushVertex();
  
  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();

  ///
  array.SetVertex(x2, y1);
  array.SetTexCoords(X2, Y1);
  array.PushVertex();

  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();

  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  

  ///// BOTTOM PART
  // BottomLeft rect:
  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();

  ///
  array.SetVertex(x0, y2);
  array.SetTexCoords(X0, Y2);
  array.PushVertex();

  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();

  array.SetVertex(x0, y3);
  array.SetTexCoords(X0, Y3);
  array.PushVertex();
  
  // BottomMiddle rect:
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();

  ///
  array.SetVertex(x1, y2);
  array.SetTexCoords(X1, Y2);
  array.PushVertex();

  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();

  array.SetVertex(x1, y3);
  array.SetTexCoords(X1, Y3);
  array.PushVertex();
  
  // BottomRight rect:
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();
  
  array.SetVertex(x3, y2);
  array.SetTexCoords(X3, Y2);
  array.PushVertex();
  
  array.SetVertex(x3, y3);
  array.SetTexCoords(X3, Y3);
  array.PushVertex();

  ///
  array.SetVertex(x2, y2);
  array.SetTexCoords(X2, Y2);
  array.PushVertex();

  array.SetVertex(x3, y3);
  array.SetTexCoords(X3, Y3);
  array.PushVertex();

  array.SetVertex(x2, y3);
  array.SetTexCoords(X2, Y3);
  array.PushVertex();

  nuiColor color = mColor;
  if (mUseWidgetAlpha && pWidget)
  {
    float widgetAlpha = pWidget->GetAlpha();
    color.Alpha() *= widgetAlpha;
    
//    wprintf(_T("nuiFrame::Draw (0x%x) alpha %.2f => color (%.2f, %.2f, %.2f, %.2f)\n"), this, widgetAlpha,
//    color.Alpha(), color.Red(), color.Green(), color.Blue());
  }
  
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture);
  pContext->SetFillColor(color);
  pContext->DrawArray(array);
}





const nuiRect& nuiFrame::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiFrame::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
}

void nuiFrame::EnableBorder(bool set)
{
  mBorderEnabled = set;
}

bool nuiFrame::IsBorderEnabled() const
{
  return mBorderEnabled;
}

nuiSize nuiFrame::GetBorder(nuiPosition position) const
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
}

nuiRect nuiFrame::GetIdealClientRect() const
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
}


