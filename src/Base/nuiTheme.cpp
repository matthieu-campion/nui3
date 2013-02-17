/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

#define DEFAULTFONTSIZE 11.0f


nuiTheme::nuiTheme()
{
  SetObjectClass(_T("nuiTheme"));
  LoadDefaults();
  mpWindowTitleFont = nuiFont::GetFont(11);
}

void nuiTheme::LoadDefaults()
{
  SetElementColor(eActiveWindowShade,           nuiColor());
  SetElementColor(eInactiveWindowShade,         nuiColor());

  SetElementColor(eActiveWindowBg,              nuiColor(1.f,1.f,1.f));
  SetElementColor(eInactiveWindowBg,            nuiColor(.9f,.9f,.9f));

  SetElementColor(eActiveWindowFg,              nuiColor(.8f,.8f,1.f));
  SetElementColor(eInactiveWindowFg,            nuiColor(.7f,.7f,.8f));

  SetElementColor(eActiveWindowTitle,           nuiColor(1.f,1.f,1.f));
  SetElementColor(eInactiveWindowTitle,         nuiColor(.8f,.8f,.8f));

  SetElementColor(eSplitterBarFg,               nuiColor(.5f,.5f,.6f));
  SetElementColor(eSplitterBarHover,            nuiColor(.7f,.7f,.8f));
  SetElementColor(eSplitterBarBg,               nuiColor(.5f,.5f,.6f));

  SetElementColor(eScrollBarFg,                 nuiColor(1.f,1.f,1.f, .8f));
  SetElementColor(eScrollBarFgHover,            nuiColor(1.f,1.f,1.f));
  SetElementColor(eScrollBarBg,                 nuiColor(.8f,.8f,1.0f));
  SetElementColor(eScrollBarBgHover,            nuiColor(.5f,.5f,0.6f));

  SetElementColor(eSliderBarFg,                 nuiColor(.5f,.5f,.9f));
  SetElementColor(eSliderBarBg,                 nuiColor(.8f,.8f,1.0f));
  SetElementColor(eSliderBarBgHover,            nuiColor(.5f,.5f,0.6f));

  SetElementColor(eNormalButtonBg,              nuiColor(255, 255, 255, 255));
  SetElementColor(eSelectedButtonBg,            nuiColor(255, 255, 255, 255));
  SetElementColor(eDisabledButtonBg,            nuiColor(255, 255, 255, 255));
  SetElementColor(eHoverNormalButtonBg,         nuiColor(255, 255, 255, 255));
  SetElementColor(eHoverSelectedButtonBg,       nuiColor(255, 255, 255, 255));
  SetElementColor(eHoverDisabledButtonBg,       nuiColor(255, 255, 255, 255));

  SetElementColor(eKnobBg,                      nuiColor(.5f,.5f,0.6f));
  SetElementColor(eKnobMarker,                  nuiColor(.8f,.8f,1.0f));

  SetElementColor(eNormalTabBg,                 nuiColor());
  SetElementColor(eSelectedTabBg,               nuiColor());
  SetElementColor(eDisabledTabBg,               nuiColor());
  SetElementColor(eHoverNormalTabBg,            nuiColor());
  SetElementColor(eHoverSelectedTabBg,          nuiColor());
  SetElementColor(eHoverDisabledTabBg,          nuiColor());

  SetElementColor(eSelectedTableColumnBg,       nuiColor(0.86328125f,0.86328125f,0.86328125f,0.5f));
  SetElementColor(eTableColumnBg,               nuiColor(0.9296875f,0.9296875f,0.9296875f,0.7f));
  SetElementColor(eSelectedTableLineBg,         nuiColor(0.2f,0.2f,0.9f,0.3f));

  SetElementColor(eTreeViewHandle,              nuiColor(0.4f, 0.4f, 0.4f, 0.5f));
  SetElementColor(eTreeViewSelection,           nuiColor(0.4f, 0.5f, 1.f, 0.5f));

  SetElementColor(eSelectionMarkee,             nuiColor(0.8f, 0.8f, 1.f, 0.9f));

  SetElementColor(eNormalTextFg,                nuiColor(0.f,0.f,0.f));
  SetElementColor(eSelectedTextFg,              nuiColor(1.f,1.f,1.f));
  SetElementColor(eDisabledTextFg,              nuiColor(.5f,.5f,.5f));
  SetElementColor(eNormalTextBg,                nuiColor(1.0f, 1.0f, 1.0f, 0.0f));
  SetElementColor(eSelectedTextBg,              nuiColor(.8f,.8f,.8f,.5f));
  SetElementColor(eDisabledTextBg,              nuiColor(.3f,.3f,.3f,0.f));

  SetElementColor(eMenuBg,                      nuiColor(1.0f, 1.0f, 1.0f, .9f));
  SetElementColor(eMenuFg,                      nuiColor(0.f, 0.f, 0.f, 0.f));
  SetElementColor(eMenuTitleBg,                 nuiColor(.5f, .5f, .5f));

  SetElementColor(eShapeFill,                   nuiColor(.5f, .5f, .5f, .5f));
  SetElementColor(eShapeStroke,                 nuiColor(0.0f, 0.0f, 0.0f, 0.5f));
  SetElementColor(eBorder,                      nuiColor(.5f, .5f, .5f, 1.f));
  SetElementColor(eDisabledImage,               nuiColor(.5f, .5f, .5f, 1.f));
  SetElementColor(eSelectedImage,               nuiColor(.9f, .9f, .9f, 1.f));

  SetElementColor(eToolTipBg,                   nuiColor(1.f, 1.f, 1.f, .8f));
  SetElementColor(eToolTipFg,                   nuiColor());
  SetElementColor(eToolTipBorder,               nuiColor(0.f, 0.f, 0.f, .3f));

  SetElementColor(eMatrixViewHeaderBorder,      nuiColor(120,120,120));
  SetElementColor(eMatrixViewHeaderBackground,  nuiColor(180,180,180));
  SetElementColor(eMatrixViewBorder,            nuiColor(120,120,120));
  SetElementColor(eMatrixViewBackground,        nuiColor(220,220,220));

  /// 
  /// Button colors:
  ///

  // Clicked & Hovered
  mButtonBorder[1][1].Set(.6f,.6f,.6f);
  mButtonFill[1][1].Set(.5f,.5f,.53f, 0.6f);

  // Clicked & Normal
  mButtonBorder[1][0].Set(.6f,.6f,.6f);
  mButtonFill[1][0].Set(.3f,.3f,.3f, 0.5f);

  // Normal & Hovered
  mButtonBorder[0][1].Set(.6f,.6f,.6f);
  mButtonFill[0][1].Set(.7f,.7f,.7f, 1.f);

  // Normal & Normal
  mButtonBorder[0][0].Set(.6f,.6f,.6f);
  mButtonFill[0][0].Set(0.7f,0.7f,0.7f, 0.9f);

  /// 
  /// tab colors:
  ///

  // Clicked & Hovered
  for (int i = 0; i<3; i++)
  {
    int j,k;
    j = i&1;
    k = i>>1;
    mTabBorder[j][k] = mButtonBorder[j][k];
    mTabFill[j][k] = mButtonFill[j][k];
  }

  Acquire();
}

nuiTheme::~nuiTheme()
{
  mpWindowTitleFont->Release();
  
  if (mpTheme == this)
    mpTheme = NULL;
}


// Window decorations:
#define INACTIVE_SHADE_SIZE 8
#define ACTIVE_SHADE_SIZE 15
#define SHADE_ALPHA .2f

void nuiTheme::DrawWindowShade(nuiDrawContext* pContext, const nuiRect& rRect, const nuiColor& rColor, bool Active)
{
  int ShadeSize = Active? ACTIVE_SHADE_SIZE : INACTIVE_SHADE_SIZE;
  nuiRect ShadeRect = rRect;
  ShadeRect.mLeft -= ShadeSize;
  ShadeRect.mRight += ShadeSize;
  ShadeRect.mTop += ShadeSize;
  ShadeRect.mBottom += ShadeSize;
  
  pContext->DrawShade(rRect,ShadeRect, rColor);
}

#define CAPTION_SIZE 24
#define RESIZE_SIZE 3
#define BORDER_SIZE 2

void nuiTheme::AdjustWindowRect(nuiRect& rRect, nuiWindowFlags Flags, bool RectIsClient, bool IncludeShadow)
{
  if (RectIsClient)
  {
    if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
      rRect.mTop-= CAPTION_SIZE;
    if (!(Flags & nuiWindow::NoResize))
    {
      rRect.mTop    -= RESIZE_SIZE;
      rRect.mBottom += RESIZE_SIZE;
      rRect.mLeft   -= RESIZE_SIZE;
      rRect.mRight  += RESIZE_SIZE;
    }
    if (IncludeShadow)
    {
      rRect.Grow(ACTIVE_SHADE_SIZE, 0);
      rRect.Bottom() += ACTIVE_SHADE_SIZE;
    }
  }
  else
  {
    if (IncludeShadow)
    {
      rRect.Grow(-ACTIVE_SHADE_SIZE, 0);
      rRect.Bottom() -= ACTIVE_SHADE_SIZE;
    }
    if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
      rRect.mTop+= CAPTION_SIZE;
    if (!(Flags & nuiWindow::NoResize))
    {
      rRect.mTop    += RESIZE_SIZE;
      rRect.mBottom -= RESIZE_SIZE;
      rRect.mLeft   += RESIZE_SIZE;
      rRect.mRight  -= RESIZE_SIZE;
    }
  }
}

bool nuiTheme::IsInsideWindow(nuiRect Rect,nuiSize X,nuiSize Y,nuiWindowFlags Flags)
{
  AdjustWindowRect(Rect,Flags);
  return Rect.IsInside(X,Y);
}

nuiPosition nuiTheme::GetWindowPart(nuiRect rect,nuiSize X,nuiSize Y,nuiWindowFlags Flags, bool RectIsClient)
{
  nuiPosition ClickPos = nuiCenter;
  if (!RectIsClient)
    AdjustWindowRect(rect, Flags, RectIsClient);
  
  if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
    rect.mTop -= CAPTION_SIZE;
  if (!(Flags & nuiWindow::NoResize))
  {
    if (!rect.IsInside(X,Y))
    {
      if (X <= rect.mLeft)
      {
        if (Y <= rect.mTop)
          ClickPos = nuiTopLeft;
        else if (Y >= rect.mBottom)
          ClickPos = nuiBottomLeft;
        else
          ClickPos = nuiLeft;
      }
      else if (X >= rect.mRight)
      {
        if (Y <= rect.mTop)
          ClickPos = nuiTopRight;
        else if (Y >= rect.mBottom)
          ClickPos = nuiBottomRight;
        else
          ClickPos = nuiRight;
      }
      else if (Y <= rect.mTop)
        ClickPos = nuiTop;
      else if (Y >= rect.mBottom)
        ClickPos = nuiBottom;
    }
  }
  return ClickPos;
}

void nuiTheme::DrawWindowBackground(nuiDrawContext* pContext, nuiWindow* pWindow, bool IsActive)
{
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  
  pContext->SetFillColor(pWindow->GetColor( IsActive ? eActiveWindowBg : eInactiveWindowBg));
  pContext->DrawRect(pWindow->GetRect().Size(), eFillShape);
}


void nuiTheme::DrawWindow(nuiDrawContext* pContext, nuiWindow* pWindow)
{
  pContext->EnableAntialiasing(false);
  // Box:
  nuiWindowFlags Flags = pWindow->GetFlags();
  nglString Title = pWindow->GetTitle();
  nuiRect Rect = pWindow->GetRect().Size();
  nuiRect rRect = Rect;
  bool blending = pContext->GetState().mBlending;
  
  float alpha = pWindow->GetAlpha();
  pContext->EnableBlending(alpha != 1);
  
  if (!(Flags & (nuiWindow::Raw & ~nuiWindow::NoCaption)))
  {
    if (!pWindow->GetDecoration())
      DrawWindowBackground(pContext, pWindow, true);
    
    // Draw window decorations:
    if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
    {
      // Draw the caption bar:
      nuiRect r = Rect;
      r.mBottom = r.mTop + CAPTION_SIZE;
      
      nuiGradient gradient;
      nuiColor color;
      nuiColor::GetColor(_T("nuiDefaultClrInactiveCaptionBkg1"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 0.f);
      nuiColor::GetColor(_T("nuiDefaultClrInactiveCaptionBkg2"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 1.f);
      
      pContext->DrawGradient(gradient, r, 0, r.Top(), 0, r.Bottom());
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorder"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawRect(r, eStrokeShape);
      
      nuiColor::GetColor(_T("nuiDefaultClrInactiveCaptionBorderLight"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left()+1, r.Top()+1, r.Right()-2, r.Top()+1);
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorderDark"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left(), r.Bottom(), r.Right(), r.Bottom());
      
      // window title
      pContext->SetFont(mpWindowTitleFont);
      nuiFontInfo Info;
      mpWindowTitleFont->GetInfo(Info);
      nuiColor::GetColor(_T("nuiDefaultClrInactiveCaptionTextLight"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 +1, r.mTop + Info.Ascender +5 +1, Title.GetChars());
      nuiColor::GetColor(_T("nuiDefaultClrCaptionText"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 , r.mTop + Info.Ascender +5 , Title.GetChars());
    }
    
  }
  pContext->EnableBlending(blending);
}

void nuiTheme::DrawActiveWindow(nuiDrawContext* pContext, nuiWindow* pWindow)
{
  pContext->EnableAntialiasing(false);
  // Box:
  nuiWindowFlags Flags = pWindow->GetFlags();
  nglString Title = pWindow->GetTitle();
  nuiRect Rect = pWindow->GetRect().Size();
  nuiRect rRect = Rect;
  bool blending = pContext->GetState().mBlending;
  
  float alpha = pWindow->GetAlpha();
  pContext->EnableBlending(alpha != 1);
  
  if (!(Flags & (nuiWindow::Raw & ~nuiWindow::NoCaption)))
  {
    if (!pWindow->GetDecoration())
      DrawWindowBackground(pContext, pWindow, true);
    
    // Draw window decorations:
    if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
    {
      // Draw the caption bar:
      nuiRect r = Rect;
      r.mBottom = r.mTop + CAPTION_SIZE;
      
      nuiGradient gradient;
      nuiColor color;
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBkg1"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 0.f);
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBkg2"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 1.f);
      
      pContext->DrawGradient(gradient, r, 0, r.Top(), 0, r.Bottom());
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorder"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawRect(r, eStrokeShape);
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorderLight"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left()+1, r.Top()+1, r.Right()-2, r.Top()+1);
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorderDark"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left(), r.Bottom(), r.Right(), r.Bottom());
      
      // window title
      pContext->SetFont(mpWindowTitleFont);
      nuiFontInfo Info;
      mpWindowTitleFont->GetInfo(Info);
      nuiColor::GetColor(_T("nuiDefaultClrCaptionTextLight"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 +1, r.mTop + Info.Ascender +5 +1, Title.GetChars());
      nuiColor::GetColor(_T("nuiDefaultClrCaptionText"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 , r.mTop + Info.Ascender +5 , Title.GetChars());
    }
    
  }
  pContext->EnableBlending(blending);
}

void nuiTheme::DrawMovingWindow(nuiDrawContext* pContext, nuiWindow* pWindow)
{
  pContext->EnableAntialiasing(false);
  // Box:
  nuiWindowFlags Flags = pWindow->GetFlags();
  nglString Title = pWindow->GetTitle();
  nuiRect Rect = pWindow->GetRect().Size();
  nuiRect rRect = Rect;
  float alpha = pWindow->GetMixedAlpha();
  
  nuiRect rect = rRect.Size();
  pWindow->LocalToGlobal(rect);
  //  pContext->BlurRect(rect,1);
  
  if (!(Flags & (nuiWindow::Raw & ~nuiWindow::NoCaption)))
  {
    if (!pWindow->GetDecoration())
      DrawWindowBackground(pContext, pWindow, true);
    
    // Draw window decorations:
    if (!(Flags & (nuiWindow::NoCaption & ~nuiWindow::NoClose)))
    {
      // Draw the caption bar:
      nuiRect r = Rect;
      r.mBottom = r.mTop + CAPTION_SIZE;
      
      nuiGradient gradient;
      nuiColor color;
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBkg1"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 0.f);
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBkg2"), color);
      color.Multiply(alpha);
      gradient.AddStop(color, 1.f);
      
      pContext->DrawGradient(gradient, r, 0, r.Top(), 0, r.Bottom());
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorder"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawRect(r, eStrokeShape);
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorderLight"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left()+1, r.Top()+1, r.Right()-2, r.Top()+1);
      
      nuiColor::GetColor(_T("nuiDefaultClrCaptionBorderDark"), color);
      color.Multiply(alpha);
      pContext->SetStrokeColor(color);
      pContext->DrawLine(r.Left(), r.Bottom(), r.Right(), r.Bottom());
      
      // window title
      nuiFont *pFont = nuiFont::GetFont(11);
      pContext->SetFont(pFont);
      nuiFontInfo Info;
      pFont->GetInfo(Info);
      nuiColor::GetColor(_T("nuiDefaultClrCaptionTextLight"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 +1, r.mTop + Info.Ascender +5 +1, Title.GetChars());
      nuiColor::GetColor(_T("nuiDefaultClrCaptionText"), color);
      color.Multiply(alpha);
      pContext->SetTextColor(color);
      pContext->DrawText(r.mLeft + RESIZE_SIZE +5 , r.mTop + Info.Ascender +5 , Title.GetChars());
      pFont->Release();
    }
    
  }
}

void nuiTheme::DrawCheckBox(nuiDrawContext* pContext, nuiToggleButton* pButton)
{
  float alpha = pButton->GetMixedAlpha();
  nuiColor border;
  int c = pButton->IsPressed()?1:0;
  int h = pButton->GetHover()?1:0;
  
  nuiSize size = 16;
  border = mButtonBorder[c][h];
  border.Multiply(alpha);
  border.Multiply(pButton->GetColor(eNormalButtonBg), true);
  
  pContext->SetStrokeColor(border);
  pContext->SetFillColor(border);
  
  nuiRect Rect = pButton->GetRect().Size();
  float s = MIN(Rect.GetWidth(), Rect.GetHeight());
  if (s > 30)
    s = 30;
  Rect = nuiRect(s, s);
  
  nuiSize topPos = Rect.Top() + ((Rect.GetHeight() - size) / 2.f);
  Rect.Set(Rect.Left(), topPos, size, size);
  Rect.RoundToNearest();
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  if (c)
  {
    pContext->DrawRect(Rect, eStrokeShape);
    Rect.Grow(-2,-2);
    pContext->DrawRect(Rect, eFillShape);
  }
  else
  {
    pContext->DrawRect(Rect, eStrokeShape);
  }
  
}




void nuiTheme::DrawMenuWindow(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget)
{
  nuiFrame* pFrame = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationPopupMenu"));
  NGL_ASSERT(pFrame);
  
  nuiRect rect;
  rect.Set(rRect.Left() - pFrame->GetBorder(nuiLeft, pWidget), rRect.Top() - pFrame->GetBorder(nuiTop, pWidget), 
           rRect.GetWidth() + pFrame->GetBorder(nuiLeft, pWidget) + pFrame->GetBorder(nuiRight, pWidget),
           rRect.GetHeight() + pFrame->GetBorder(nuiTop, pWidget) + pFrame->GetBorder(nuiBottom, pWidget));
  
  pFrame->Draw(pContext, pWidget, rect);
  pFrame->Release();
}

void nuiTheme::DrawMenuItem(nuiDrawContext* pContext, const nuiRect& rRect, bool Hover)
{
  bool blending = pContext->GetState().mBlending;
  nuiBlendFunc blendfunc = pContext->GetState().mBlendFunc;
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  
  if (Hover) // "Inverse video" if the mouse if above the menu item...
  {
    pContext->SetFillColor(nuiColor(0.0f,0.0f,0.5f,0.6f));
    pContext->DrawRect(rRect,eFillShape);
  }
  else // Do nothing if the mouse is not on the menu item
  {
    pContext->SetFillColor(nuiColor(.9f,.9f,1.f,.6f));
    pContext->DrawRect(rRect,eFillShape);
  }
  
  pContext->EnableBlending(blending);
  pContext->SetBlendFunc(blendfunc);
}

nuiDialog*  nuiTheme::CreateDefaultDialog(nuiContainer* pParent)
{
  return new nuiDialog(pParent);
}

nuiLabel *nuiTheme::CreateTreeNodeLabel(const nglString &text)
{
  return new nuiLabel(text);
}

void nuiTheme::DrawScrollBarBackground(nuiDrawContext* pContext, nuiScrollBar* pScroll)
{
  const nuiRect& rRect = pScroll->GetRangeRect();
  
  nuiDecoration* pDeco = NULL;
  if (pScroll->GetOrientation() == nuiVertical)
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarVerticalBkg"));
  else
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarHorizontalBkg"));
  NGL_ASSERT(pDeco);
  
  nuiRect rectDest(0.0f, 0.0f, rRect.GetWidth(), rRect.GetHeight());
  pDeco->Draw(pContext, pScroll, rectDest);
  pDeco->Release();
}


void nuiTheme::DrawScrollBarForeground(nuiDrawContext* pContext, nuiScrollBar* pScroll)
{
  nuiDecoration* pDeco = NULL;
  if (pScroll->GetOrientation() == nuiVertical)
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarVerticalHdl"));
  else
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationScrollBarHorizontalHdl"));
  NGL_ASSERT(pDeco);
  
  nuiRect rRect = pScroll->GetThumbRect();
  rRect.Grow(-1.f, -1.f);
  
  pDeco->Draw(pContext, pScroll, rRect);
  pDeco->Release();
}

void nuiTheme::DrawSliderBackground(nuiDrawContext* pContext, nuiSlider* pSlider)
{
  nuiRect rect = pSlider->GetRect().Size();
  nuiDecoration* pDeco = NULL;
  
  nuiSize min = pSlider->GetHandlePosMin();
  nuiSize max = pSlider->GetHandlePosMax();
  
  if (pSlider->GetOrientation() == nuiVertical)
  {
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSliderVerticalBkg"));
    if (!pDeco)
      return;
    nuiSize x = (int)((rect.GetWidth() - pDeco->GetIdealClientRect(pSlider).GetWidth()) / 2);
    nuiSize w = pDeco->GetIdealClientRect(pSlider).GetWidth();
    rect.Set(x, min, w, max-min);
  }
  else
  {
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSliderHorizontalBkg"));
    if (!pDeco)
      return;
    nuiSize y = (int)((rect.GetHeight() - pDeco->GetIdealClientRect(pSlider).GetHeight()) / 2);
    nuiSize h = pDeco->GetIdealClientRect(pSlider).GetHeight();
    rect.Set(min, y, max - min, h);
  }  
  
  if (pDeco)
  {
    pDeco->Draw(pContext, pSlider, rect);
    pDeco->Release();
  }
}

void nuiTheme::DrawSliderForeground(nuiDrawContext* pContext, nuiSlider* pSlider)
{
  nuiRect rect = pSlider->GetRect().Size();
  float start;
  const nuiRange& Range = pSlider->GetRange();
  
  start  = Range.ConvertToUnit(Range.GetValue());
  nuiDecoration* pDeco = NULL;
  
  if (pSlider->GetOrientation() == nuiVertical)
  {
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSliderVerticalHdl"));

    if (pDeco)
    {
      const nuiRect& srcRect = pDeco->GetIdealClientRect(pSlider);
      
      nuiSize h = rect.GetHeight() - srcRect.GetHeight();
      rect.mTop = h - (start * h);
      rect.mBottom = rect.mTop + srcRect.GetHeight();
      
      rect.mLeft = (int)(rect.GetWidth() - srcRect.GetWidth())/2;
      rect.mRight = rect.mLeft + srcRect.GetWidth();
    }
  }
  else
  {
    pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSliderHorizontalHdl"));
    if (pDeco)
    {
      const nuiRect& srcRect = pDeco->GetIdealClientRect(pSlider);
      
      rect.mLeft = (start * (rect.GetWidth() - srcRect.GetWidth()));
      rect.mRight = rect.mLeft + srcRect.GetWidth();
      
      rect.mTop = (int)(rect.GetHeight() - srcRect.GetHeight())/2;
      rect.mBottom = rect.mTop + srcRect.GetHeight();
    }
  }
  
  if (pDeco)
  {
    pDeco->Draw(pContext, pSlider, rect);
    pDeco->Release();
  }
  else
  {
    if (pSlider->GetOrientation() == nuiVertical)
    {
      nuiRect srcRect = pSlider->GetRect().Size();
      srcRect.SetHeight(srcRect.GetWidth() / 2);
      
      nuiSize h = rect.GetHeight() - srcRect.GetHeight();
      rect.mTop = h - (start * h);
      rect.mBottom = rect.mTop + srcRect.GetHeight();
      
      rect.mLeft = (int)(rect.GetWidth() - srcRect.GetWidth())/2;
      rect.mRight = rect.mLeft + srcRect.GetWidth();
    }
    else
    {
      nuiRect srcRect = pSlider->GetRect().Size();
      srcRect.SetWidth(srcRect.GetHeight() / 2);
      
      rect.mLeft = (start * (rect.GetWidth() - srcRect.GetWidth()));
      rect.mRight = rect.mLeft + srcRect.GetWidth();
      
      rect.mTop = (int)(rect.GetHeight() - srcRect.GetHeight())/2;
      rect.mBottom = rect.mTop + srcRect.GetHeight();
    }
    
    pContext->DrawRect(rect, eFillShape);
  }
}

const nuiColor& nuiTheme::GetElementColor(nuiWidgetElement Element) const
{
  return mElementColors[Element];
}

void nuiTheme::SetElementColor(nuiWidgetElement Element,const nuiColor& rColor)
{
  mElementColors[Element] = rColor;
}

void nuiTheme::DrawTreeHandle(nuiDrawContext* pContext, const nuiRect& rRect, bool IsOpened, nuiSize TREE_HANDLE_SIZE, const nuiColor& rColor)
{
  nuiFrame* pDeco = NULL;
  if (IsOpened)
    pDeco = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationArrowOpen"));
  else
    pDeco = (nuiFrame*)nuiDecoration::Get(_T("nuiDefaultDecorationArrowClose"));
  if (pDeco)
  {
    const nuiRect& rectSrc = pDeco->GetIdealClientRect(NULL);
    
    nuiSize x,y;
    x = rRect.Left() + (int)((rRect.GetWidth() - rectSrc.GetWidth()) / 2.f);
    y = rRect.Top() + (int)((rRect.GetHeight() - rectSrc.GetHeight()) / 2.f);
    nuiRect rectDest(x, y, rectSrc.GetWidth() , rectSrc.GetHeight());
    
    pDeco->SetColor(rColor);
    pDeco->Draw(pContext, NULL, rectDest);
    
    pDeco->Release();
  }
  else
  {
    pContext->SetFillColor(rColor);
    nuiRenderArray* array = new nuiRenderArray(GL_TRIANGLES);
    float l = MIN(rRect.GetWidth(), rRect.GetHeight());
    nuiRect r(l, l);
    r.SetPosition(nuiCenter, rRect);
    const float x = r.Left();
    const float y = r.Top();
    const float w = r.GetWidth();
    const float h = r.GetHeight();
    
    if (IsOpened)
    {
      array->SetVertex(x,       y);     array->PushVertex();
      array->SetVertex(x + w,   y);     array->PushVertex();
      array->SetVertex(x + w/2, y + h); array->PushVertex();
    }
    else
    {
      array->SetVertex(x,     y);       array->PushVertex();
      array->SetVertex(x + w, y + h/2); array->PushVertex();
      array->SetVertex(x,     y + h);   array->PushVertex();
    }
    
    pContext->DrawArray(array);
  }
     
}


void nuiTheme::DrawSelectionBackground(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget)
{
  nuiDecoration* pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSelectionBackground"));
  if (!pDeco)
    return;
  
  pDeco->Draw(pContext, pWidget, rRect);
  pDeco->Release();
}


void nuiTheme::DrawSelectionForeground(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidget* pWidget)
{
  nuiDecoration* pDeco = nuiDecoration::Get(_T("nuiDefaultDecorationSelectionForeground"));
  if (!pDeco)
    return;
  
  pDeco->Draw(pContext, pWidget, rRect);  
  pDeco->Release();
}


void nuiTheme::DrawMarkee(nuiDrawContext* pContext, const nuiRect& rRect, nuiWidgetElement Color)
{
  pContext->EnableAntialiasing(false);
  pContext->EnableBlending(true);
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableTexturing(false);
  nuiColor col = GetElementColor(Color);
  pContext->SetFillColor(col);
  col.Multiply(.3f);
  pContext->SetStrokeColor(col);
  pContext->DrawRect(rRect, eStrokeAndFillShape);
}


/****************************************************************************
 *
 * Generic Theme Support
 *
 ****************************************************************************/
nuiTheme* nuiTheme::GetTheme()
{
  if (!mpTheme)
  {
    mpTheme = new nuiTheme();
  }
  mpTheme->Acquire();
  return mpTheme;
}

void nuiTheme::SetTheme(nuiTheme* pTheme)
{
  if ( mpTheme )
    mpTheme->Release();
  mpTheme = pTheme;
  if (mpTheme)
    mpTheme->Acquire();
}


nuiTheme* nuiTheme::mpTheme = NULL;
