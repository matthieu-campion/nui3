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
  mBorderEnabled(true)
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
  mBorderEnabled(true)
{
  if (SetObjectClass(_T("nuiFrame")))
    InitAttributes();
	
  SetTexturePath(rTexturePath.GetPathName());
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
  

	AddAttribute(_T("EnableBorder"), AttributeBorder);
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
  return mTexturePath;
}

void nuiFrame::SetTexturePath(const nglPath& rPath)
{
  mTexturePath = rPath;
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
  Changed();
}


// virtual
void nuiFrame::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  nuiRenderArray* pArray = new nuiRenderArray(GL_TRIANGLES);
  pArray->EnableArray(nuiRenderArray::eVertex, true);
  pArray->EnableArray(nuiRenderArray::eTexCoord, true);
  pArray->EnableArray(nuiRenderArray::eColor, false);
  
  
  nuiSize w = mpTexture->GetImage()->GetWidth();
  nuiSize h = mpTexture->GetImage()->GetHeight();
  
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
  pArray->SetVertex(x0, y0);
  pArray->SetTexCoords(X0, Y0);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y0);
  pArray->SetTexCoords(X1, Y0);
  pArray->PushVertex();

  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();
  
  ///
  pArray->SetVertex(x0, y0);
  pArray->SetTexCoords(X0, Y0);
  pArray->PushVertex();

  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x0, y1);
  pArray->SetTexCoords(X0, Y1);
  pArray->PushVertex();
  
  // TopMiddle rect:
  pArray->SetVertex(x1, y0);
  pArray->SetTexCoords(X1, Y0);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y0);
  pArray->SetTexCoords(X2, Y0);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();
  
  ///
  pArray->SetVertex(x1, y0);
  pArray->SetTexCoords(X1, Y0);
  pArray->PushVertex();

  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();
  
  // TopRight rect:
  pArray->SetVertex(x2, y0);
  pArray->SetTexCoords(X2, Y0);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y0);
  pArray->SetTexCoords(X3, Y0);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y1);
  pArray->SetTexCoords(X3, Y1);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x2, y0);
  pArray->SetTexCoords(X2, Y0);
  pArray->PushVertex();

  pArray->SetVertex(x3, y1);
  pArray->SetTexCoords(X3, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();

  ///// MIDDLE PART
  // LeftSide rect:
  pArray->SetVertex(x0, y1);
  pArray->SetTexCoords(X0, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x0, y1);
  pArray->SetTexCoords(X0, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x0, y2);
  pArray->SetTexCoords(X0, Y2);
  pArray->PushVertex();
  
  // Middle (client) rect:
  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x1, y1);
  pArray->SetTexCoords(X1, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();
  
  // RightSide rect:
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y1);
  pArray->SetTexCoords(X3, Y1);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y2);
  pArray->SetTexCoords(X3, Y2);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x2, y1);
  pArray->SetTexCoords(X2, Y1);
  pArray->PushVertex();

  pArray->SetVertex(x3, y2);
  pArray->SetTexCoords(X3, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();
  
  

  ///// BOTTOM PART
  // BottomLeft rect:
  pArray->SetVertex(x0, y2);
  pArray->SetTexCoords(X0, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x1, y3);
  pArray->SetTexCoords(X1, Y3);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x0, y2);
  pArray->SetTexCoords(X0, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x1, y3);
  pArray->SetTexCoords(X1, Y3);
  pArray->PushVertex();

  pArray->SetVertex(x0, y3);
  pArray->SetTexCoords(X0, Y3);
  pArray->PushVertex();
  
  // BottomMiddle rect:
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x2, y3);
  pArray->SetTexCoords(X2, Y3);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x1, y2);
  pArray->SetTexCoords(X1, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x2, y3);
  pArray->SetTexCoords(X2, Y3);
  pArray->PushVertex();

  pArray->SetVertex(x1, y3);
  pArray->SetTexCoords(X1, Y3);
  pArray->PushVertex();
  
  // BottomRight rect:
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y2);
  pArray->SetTexCoords(X3, Y2);
  pArray->PushVertex();
  
  pArray->SetVertex(x3, y3);
  pArray->SetTexCoords(X3, Y3);
  pArray->PushVertex();

  ///
  pArray->SetVertex(x2, y2);
  pArray->SetTexCoords(X2, Y2);
  pArray->PushVertex();

  pArray->SetVertex(x3, y3);
  pArray->SetTexCoords(X3, Y3);
  pArray->PushVertex();

  pArray->SetVertex(x2, y3);
  pArray->SetTexCoords(X2, Y3);
  pArray->PushVertex();

  nuiColor color = mColor;
  
  if (pWidget)
  {
    if (mUseWidgetFrameColor)
    {
      nuiAttrib<const nuiColor&> colorAttr(pWidget->GetAttribute(_T("FrameColor")));
      if (colorAttr.IsValid()) {
        color = colorAttr.Get();
      }
    }
    if (mUseWidgetAlpha)
    {
      float widgetAlpha = pWidget->GetAlpha();
      color.Alpha() *= widgetAlpha;
    }
  }
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture);
  pContext->SetFillColor(color);
  pContext->DrawArray(pArray);
}


const nuiRect& nuiFrame::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiFrame::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  Changed();
}

void nuiFrame::EnableBorder(bool set)
{
  mBorderEnabled = set;
  Changed();
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
  Changed();
}


