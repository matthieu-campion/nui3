/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

nuiImageDecoration::nuiImageDecoration(const nglString& rName)
: nuiDecoration(rName),
  mpTexture(NULL),
  mPosition(nuiCenter),
  mColor(255,255,255,255),
  mRepeatX(false),
  mRepeatY(false)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
}

nuiImageDecoration::nuiImageDecoration(const nglString& rName, nuiTexture* pTexture, const nuiRect& rClientRect, nuiPosition position, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(pTexture),
  mPosition(position),
  mClientRect(rClientRect),
  mColor(rColor),
  mRepeatX(false),
  mRepeatY(false)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
}

nuiImageDecoration::nuiImageDecoration(const nglString& rName, const nglPath& rTexturePath, const nuiRect& rClientRect, nuiPosition position, const nuiColor& rColor)
: nuiDecoration(rName),
  mpTexture(NULL),
  mPosition(position),
  mClientRect(rClientRect),
  mColor(rColor),
  mRepeatX(false),
  mRepeatY(false)
{
  if (SetObjectClass(_T("nuiImageDecoration")))
    InitAttributes();
	
  mpTexture = nuiTexture::GetTexture(rTexturePath);
}




void nuiImageDecoration::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiRect&>
               (nglString(_T("ClientRect")), nuiUnitNone,
                nuiAttribute<const nuiRect&>::GetterDelegate(this, &nuiImageDecoration::GetSourceClientRect),
                nuiAttribute<const nuiRect&>::SetterDelegate(this, &nuiImageDecoration::SetSourceClientRect)));
  
  AddAttribute(new nuiAttribute<nglPath>
               (nglString(_T("Texture")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiImageDecoration::GetTexturePath), 
                nuiMakeDelegate(this, &nuiImageDecoration::SetTexturePath)));
  
  AddAttribute(new nuiAttribute<nuiPosition>
               (nglString(_T("Position")), nuiUnitPosition,
                nuiMakeDelegate(this, &nuiImageDecoration::GetPosition), 
                nuiMakeDelegate(this, &nuiImageDecoration::SetPosition)));
  
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("Color")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiImageDecoration::GetColor), 
                nuiMakeDelegate(this, &nuiImageDecoration::SetColor)));
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RepeatX")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiImageDecoration::GetRepeatX), 
                nuiMakeDelegate(this, &nuiImageDecoration::SetRepeatX)));  
  
  AddAttribute(new nuiAttribute<bool>
               (nglString(_T("RepeatY")), nuiUnitBoolean,
                nuiMakeDelegate(this, &nuiImageDecoration::GetRepeatY), 
                nuiMakeDelegate(this, &nuiImageDecoration::SetRepeatY)));  
}



nuiImageDecoration::~nuiImageDecoration()
{
  mpTexture->Release();
}

bool nuiImageDecoration::GetRepeatX() const
{
  return mRepeatX;
}


void nuiImageDecoration::SetRepeatX(bool set)
{
  mRepeatX = set;
  if (mpTexture && set)
    mpTexture->SetWrapS(GL_REPEAT);
  else if (mpTexture && !set)
  {
  #ifndef _OPENGL_ES_
      mpTexture->SetWrapS(GL_CLAMP);
  #else
      mpTexture->SetWrapS(GL_CLAMP_TO_EDGE);
  #endif
  }
}


bool nuiImageDecoration::GetRepeatY() const
{
  return mRepeatY;  
}


void nuiImageDecoration::SetRepeatY(bool set)
{
  mRepeatY = set;  
  if (mpTexture && set)
    mpTexture->SetWrapT(GL_REPEAT);
  else if (mpTexture && !set)
  {
  #ifndef _OPENGL_ES_
      mpTexture->SetWrapT(GL_CLAMP);
  #else
      mpTexture->SetWrapT(GL_CLAMP_TO_EDGE);
  #endif
  }
}




nuiPosition nuiImageDecoration::GetPosition()
{
  return mPosition;
}


void nuiImageDecoration::SetPosition(nuiPosition pos)
{
  mPosition = pos;
  Changed();
}


nglPath nuiImageDecoration::GetTexturePath() const
{
  return mpTexture->GetSource();
}

void nuiImageDecoration::SetTexturePath(nglPath path)
{
  nuiTexture* pOld = mpTexture;
  mpTexture = nuiTexture::GetTexture(path);
  if (!mpTexture || !mpTexture->IsValid())
  {
    NGL_OUT(_T("nuiImageDecoration::SetTexturePath warning : could not load graphic resource '%s'\n"), path.GetChars());
    return;
  }
  
  if (GetSourceClientRect() == nuiRect(0,0,0,0))
    SetSourceClientRect(nuiRect(0, 0, mpTexture->GetWidth(), mpTexture->GetHeight()));
  if (pOld)
    pOld->Release();
  
  SetRepeatX(mRepeatX);
  SetRepeatY(mRepeatY);
  
  Changed();
}

const nuiColor& nuiImageDecoration::GetColor() const
{
  return mColor;
}

void nuiImageDecoration::SetColor(const nuiColor& rColor)
{
  mColor = rColor;
  Changed();
}

void nuiImageDecoration::Draw(nuiDrawContext* pContext, nuiWidget* pWidget, const nuiRect& rDestRect)
{
  if (!mpTexture)
    return;
  
  pContext->PushState();
  pContext->ResetState();
  
  nuiRect rect = mClientRect;
  rect.SetPosition(mPosition, rDestRect);
  rect.RoundToBelow();
  
  pContext->EnableTexturing(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetTexture(mpTexture);
  nuiColor col(mColor);
  if (mUseWidgetAlpha && pWidget)
    col.Multiply(pWidget->GetMixedAlpha());
  
  pContext->SetFillColor(col);
  
  nuiRect srcrect(mClientRect);
  if (mRepeatX)
  {
    srcrect.SetWidth(rDestRect.GetWidth());
    srcrect.MoveTo(0, srcrect.Top());
  }

  if (mRepeatY)
  {
    srcrect.SetHeight(rDestRect.GetHeight());
    srcrect.MoveTo(srcrect.Left(), 0);
  }

  if (mRepeatX || mRepeatY)
    rect = rDestRect;
  
  
  pContext->DrawImage(rect, srcrect);
  //pContext->DrawRect(rDestRect, eStrokeShape);
  pContext->PopState();
}





const nuiRect& nuiImageDecoration::GetSourceClientRect() const
{
  return mClientRect;
}

void nuiImageDecoration::SetSourceClientRect(const nuiRect& rRect)
{
  mClientRect = rRect;
  Changed();
}


nuiSize nuiImageDecoration::GetBorder(nuiPosition position, const nuiWidget* pWidget) const
{
  if (!mBorderEnabled)
    return 0;
  
  float w = mpTexture->GetWidth();
  float h = mpTexture->GetHeight();
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

void nuiImageDecoration::GetBorders(const nuiWidget* pWidget, float& rLeft, float& rRight, float& rTop, float& rBottom, float& rHorizontal, float& rVertical) const
{
  if (!mpTexture)
  {
    NGL_OUT(_T("nuiImageDecoration::mpTexture should not be NULL!"));
    NGL_ASSERT(mpTexture);
    return;
  }
    
  
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



nuiTexture* nuiImageDecoration::GetTexture() const
{
  return mpTexture;
}



nuiRect nuiImageDecoration::GetIdealClientRect(const nuiWidget* pWidget) const
{
  return mClientRect.Size();
}




