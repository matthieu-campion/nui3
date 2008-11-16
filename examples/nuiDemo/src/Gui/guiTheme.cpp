/*
 *  guiTheme.cpp
 *  MXEditor
 *
 *  Created by Loic Berthelot on 2008/11/15
 *  Copyright 2007 MXP4. All rights reserved.
 *
 */


#include "guiTheme.h"
#include "nuiFrame.h"


guiTheme::guiTheme()
: nuiTheme(nglPath(ePathCurrent))
{
  nuiTheme::SetTheme(this);
}


guiTheme::~guiTheme()
{
  
}

// virtual from nuiTheme
void guiTheme::DrawSliderBackground(nuiDrawContext* pContext, nuiSlider* pSlider)
{
  // we don't care about vertical slider in our application
  if (pSlider->GetOrientation() == nuiVertical)
    return;

  nuiRect rect = pSlider->GetRect().Size();
  nuiFrame* pFrame = NULL;
  
  nuiSize min = pSlider->GetHandlePosMin();
  nuiSize max = pSlider->GetHandlePosMax();
  
  pFrame = (nuiFrame*)nuiDecoration::Get(_T("dSliderBkg")); // dSliderBkg is declared in Gui.css stylesheet
  nuiSize y = (int)((rect.GetHeight() - pFrame->GetSourceClientRect().GetHeight()) / 2);
  nuiSize h = pFrame->GetSourceClientRect().GetHeight();
  rect.Set(min, y, max - min, h);

  pFrame->Draw(pContext, NULL, rect);  
}



// virtual from nuiTheme
void guiTheme::DrawSliderForeground(nuiDrawContext* pContext, nuiSlider* pSlider)
{
  // we don't care about vertical slider in our application
  if (pSlider->GetOrientation() == nuiVertical)
    return;

  nuiRect rect = pSlider->GetRect().Size();
  float start;
  const nuiRange& Range = pSlider->GetRange();
  
  start  = Range.ConvertToUnit(Range.GetValue());
  nuiFrame* pFrame = NULL;
  

  pFrame = (nuiFrame*)nuiDecoration::Get(_T("dSliderHandle")); // dSliderHandle is declared in Gui.css stylesheet
  NGL_ASSERT(pFrame);
  const nuiRect& srcRect = pFrame->GetSourceClientRect();
  
  rect.mLeft = (start * (rect.GetWidth() - srcRect.GetWidth()));
  rect.mRight = rect.mLeft + srcRect.GetWidth();
  
  rect.mTop = (int)(rect.GetHeight() - srcRect.GetHeight())/2;
  rect.mBottom = rect.mTop + srcRect.GetHeight();
  
  
  pFrame->Draw(pContext, NULL, rect);   
  
}

