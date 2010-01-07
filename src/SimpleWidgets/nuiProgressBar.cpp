/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiProgressBar.h"
#include "nuiShape.h"
#include "nuiDrawContext.h"

#define AMP 0.15f
#define INC 2.5f

nuiProgressBar::nuiProgressBar(float Progress)
{
  if (SetObjectClass(_T("nuiProgressBar")))
    InitAttributes();
  
  nuiColor::GetColor(_T("nuiDefaultClrProgressFg"), mFGColor);
  nuiColor::GetColor(_T("nuiDefaultClrProgressBg"), mBGColor);
  
  mProgress = Progress;
  mAlphaIncr = INC;
  mAlphaAnim = 0;
  mGlowStrength = AMP;
  mEndless = false;
  mEndlessAnim = 0.f;
  mEndlessIncr = INC;
  if (mGlowStrength != 0 || mAlphaIncr != 0 || mEndless)
    StartAutoDraw();
}

nuiProgressBar::nuiProgressBar()
{
  if (SetObjectClass(_T("nuiProgressBar")))
    InitAttributes();
  
  nuiColor::GetColor(_T("nuiDefaultClrProgressFg"), mFGColor);
  nuiColor::GetColor(_T("nuiDefaultClrProgressBg"), mBGColor);

  mProgress = 0.f;
  mAlphaIncr = INC;
  mAlphaAnim = 0;
  mGlowStrength = AMP;
  mEndless = true;
  mEndlessAnim = 0.f;
  mEndlessIncr = INC;
  if (mGlowStrength != 0 || mAlphaIncr != 0 || mEndless)
    StartAutoDraw();
}

bool nuiProgressBar::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  SetProgress(0);
  mAlphaIncr = INC;
  mAlphaAnim = 0;
  mGlowStrength = 0;
  if (mGlowStrength != 0 || mAlphaIncr != 0)
    StartAutoDraw();
  
  return true;
}

nuiProgressBar::~nuiProgressBar()
{
}



void nuiProgressBar::InitAttributes()
{
  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("ForegroundColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiProgressBar::GetFGColor), 
                nuiMakeDelegate(this, &nuiProgressBar::SetFGColor)));

  AddAttribute(new nuiAttribute<const nuiColor&>
               (nglString(_T("BackgroundColor")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiProgressBar::GetBGColor), 
                nuiMakeDelegate(this, &nuiProgressBar::SetBGColor)));

  AddAttribute(new nuiAttribute<float>
               (nglString(_T("Glow")), nuiUnitNone,
                nuiMakeDelegate(this, &nuiProgressBar::GetGlow), 
                nuiMakeDelegate(this, &nuiProgressBar::SetGlow)));
  
  
}


void nuiProgressBar::SetFGColor(const nuiColor& Color)
{
  mFGColor = Color;
  Invalidate();
}

const nuiColor& nuiProgressBar::GetFGColor() const
{
  return mFGColor;
}

void nuiProgressBar::SetBGColor(const nuiColor& Color)
{
  mBGColor = Color;
  Invalidate();
}

const nuiColor& nuiProgressBar::GetBGColor() const
{
  return mBGColor;
}


void nuiProgressBar::SetGlow(float value)
{
  mGlowStrength = value;
  if (mGlowStrength > 0)
    StartAutoDraw();
  else 
    StopAutoDraw();
}

float nuiProgressBar::GetGlow() const
{
  return mGlowStrength;
}



nuiRect nuiProgressBar::CalcIdealSize()
{
  return nuiRect(0,0,150,16);
}

bool nuiProgressBar::Draw(nuiDrawContext* pContext)
{
  float alpha = 0;
  
  nglTime now;
  double lap = (double)now - (double)mLastRender;
  mLastRender = now;
  
  if (mGlowStrength != 0)
  {
    mAlphaAnim += mAlphaIncr * (float)lap;
    
    //  while (mAlphaAnim >= 1)
    //    mAlphaAnim -= 1;
    
    alpha = mGlowStrength * (float)sin(M_PI * mAlphaAnim) - mGlowStrength * .5f;
  }
  
  nuiRect size = GetRect().Size();
  
  pContext->EnableBlending(true);
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->SetFillColor(mBGColor);
  pContext->DrawRect(size, eFillShape);
  
  nuiColor col = mFGColor;
  
  col.Alpha() += alpha;
  col.Crop();
  
  if (mEndless)
  {
    mEndlessAnim += mEndlessIncr * (float)lap * .25f;
    
    float ratio = fmod(mEndlessAnim, 1.f);
    
    nuiGradient grad;
    grad.AddStop(col, ratio);
    
    nuiColor noAlpha = col;
    noAlpha.Alpha() = 0.f;
    if (ratio > .5f)
    {
      grad.AddStop(noAlpha, ratio - .5f);
      
      nuiColor midAlpha = col;
      midAlpha.Alpha() *= (2.f*ratio - 1.f);
      grad.AddStop(midAlpha, 0.0);
      grad.AddStop(midAlpha, 1.0);
    }
    else
    {
      grad.AddStop(noAlpha, ratio + .5f);
      
      nuiColor midAlpha = col;
      midAlpha.Alpha() *= (-2.f*ratio + 1.f);
      grad.AddStop(midAlpha, 0.0);
      grad.AddStop(midAlpha, 1.0);
    }
    
    pContext->SetBlendFunc(nuiBlendTranspAdd);
    pContext->DrawGradient(grad, size, size.mLeft, size.mBottom, size.mRight, size.mBottom);
    
    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->DrawGradient(grad, size, size.mLeft, size.mBottom, size.mRight, size.mBottom);
  }
  else
  {
    nuiRect r(0.0f, 0.0f, 0.01f * mProgress * size.GetWidth(), size.GetHeight());
    
    pContext->SetBlendFunc(nuiBlendTranspAdd);
    pContext->SetFillColor(col);
    pContext->DrawRect(r, eFillShape);
    
    pContext->SetBlendFunc(nuiBlendTransp);
    pContext->SetFillColor(col);
    pContext->DrawRect(r, eFillShape);
  }
  
  pContext->SetBlendFunc(nuiBlendTransp);
  nuiReflection ref(.5f, .3f);
  ref.Draw(pContext, size, NULL);
  
  pContext->SetBlendFunc(nuiBlendTransp);
  pContext->EnableAntialiasing(true);
  pContext->SetStrokeColor(nuiColor(0,0,0));
  pContext->DrawRect(GetRect().Size(), eStrokeShape);
  
  return true;
}


void nuiProgressBar::SetProgress(float Progress)
{
  if (mProgress != Progress)
  {
    mProgress = Progress;
    Invalidate();
  }
}

float nuiProgressBar::GetProgress() const
{
  return mProgress;
}

void nuiProgressBar::SetGlowStrength(float Strength)
{
  mGlowStrength = Strength;
  if (mGlowStrength != 0)
    StartAutoDraw();
  else
  {
    StopAutoDraw();
    Invalidate();
  }
}

float nuiProgressBar::GetGlowStrength() const
{
  return mGlowStrength;
}

bool nuiProgressBar::IsEndless() const
{
  return mEndless;
}

void nuiProgressBar::SetEndless(bool endless)
{
  if (mEndless != endless)
  {
    mEndless = endless;
    if (endless)
    {
      mEndlessAnim = 0.f;
      StartAutoDraw();
    }
    else if (mGlowStrength == 0)
    {
      StopAutoDraw();
    }
    Invalidate();
  }
}
