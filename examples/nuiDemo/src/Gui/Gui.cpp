/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Gui/Gui.h"
#include "nuiHBox.h"
#include "nuiSlider.h"
#include "nuiKnob.h"
#include "Gui/guiOscillo.h"

//
// Gui inherits from nuiVBox
//

Gui::Gui()
: nuiVBox(0)
{
  SetObjectClass(_T("Gui"));
  SetExpand(nuiExpandShrinkAndGrow);
  
  // row 1: title image
  nuiImage* pTitleImg = new nuiImage();
  pTitleImg->SetObjectName(_T("ImageTitle"));
  AddCell(pTitleImg, nuiTopLeft);
    
  // row 2: start button and oscillo
  nuiHBox* pBox = new nuiHBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddCell(pBox);
  SetCellExpand(GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  nuiToggleButton* pStartBtn = new nuiToggleButton();
  pStartBtn->SetObjectName(_T("ButtonStart"));
  pBox->AddCell(pStartBtn, nuiCenter);
  
  guiOscillo* pOscillo = new guiOscillo();
  pBox->AddCell(pOscillo, nuiCenter);
  
  // row 3: text and controls
  pBox = new nuiHBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddCell(pBox);
  SetCellExpand(GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  nuiLabel* pText = new nuiLabel(_T("TEXT"));
  pText->SetObjectName(_T("Text"));
  pBox->AddCell(pText);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  nuiWidget* pControls = BuildControls();
  pBox->AddCell(pControls);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
}


Gui::~Gui()
{

}


nuiWidget* Gui::BuildControls()
{
  nuiVBox* pBox = new nuiVBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  
  
  // pitch slider
  nuiSlider* pPitchSlider = new nuiSlider(nuiHorizontal);
  pPitchSlider->SetObjectName(_T("SliderPitch"));
  pBox->AddCell(pPitchSlider, nuiCenter);
  
  // frequency knob
  nuiKnob* pFreqKnob = new nuiKnob();
  pFreqKnob->SetObjectName(_T("KnobFrequency"));
  pBox->AddCell(pFreqKnob, nuiCenter);
  
  // Q knob
  nuiKnob* pQKnob = new nuiKnob();
  pQKnob->SetObjectName(_T("KnobQ"));
  pBox->AddCell(pQKnob, nuiCenter);
  
  return pBox;
}