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
#include "Application.h"


//
// Gui inherits from nuiVBox
//

Gui::Gui()
: nuiVBox(0), mEventSink(this)
{
  SetObjectClass(_T("Gui"));
  SetExpand(nuiExpandShrinkAndGrow);
  
  
  // row 1  : title image *****************************************
  //
  nuiImage* pTitleImg = new nuiImage();
  pTitleImg->SetObjectName(_T("ImageTitle"));
  AddCell(pTitleImg, nuiTopLeft);
    

  // row 2: start button and oscillo ******************************
  //
  
  // a box for row layout
  nuiHBox* pBox = new nuiHBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddCell(pBox);
  
  // start/stop button
  nuiToggleButton* pStartBtn = new nuiToggleButton();
  pStartBtn->SetObjectName(_T("ButtonStart"));
  pBox->AddCell(pStartBtn, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect the button event to the gui controller.
  // <=> in the nui-way-of-thinking : connect the button event source you're interessted in to the event receiver of your choice
  mEventSink.Connect(pStartBtn->ButtonPressed, &Gui::OnStartButtonPressed);
  mEventSink.Connect(pStartBtn->ButtonDePressed, &Gui::OnStartButtonDePressed);
  
  // oscillo
  guiOscillo* pOscillo = new guiOscillo(GetEngine()->GetAudioTrack()->GetSamplesBuffer());
  pBox->AddCell(pOscillo, nuiCenter);
  

  // row 3: text and sound controls *******************************
  //
  
  // a box for row layout
  pBox = new nuiHBox(0);
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddCell(pBox);
  SetCellExpand(GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // a text to display
  nuiLabel* pText = new nuiLabel(_T("TEXT"));
  pText->SetObjectName(_T("Text"));
  pBox->AddCell(pText);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // area for the sound controls
  nuiWidget* pControls = BuildControls();
  pBox->AddCell(pControls);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
}


Gui::~Gui()
{
  // don't have to delete or clean anything. NUI does it automatically
  // => everything that is connected to a nui object is automatically trashed when the object is trashed or deleted
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




//***********************************************************************************
//
// Gui Events Receivers
//

bool Gui::OnStartButtonPressed(const nuiEvent& rEvent)
{
  GetEngine()->GetAudioTrack()->Start();
  return true;
  // Returning true from an event receiver means the event is caught : no one else can receive the event.
  //
  // When an event source is connected to several receivers (<=> several objects need to know when a particular event is sent),
  // - return false from each receiver, if you want the event to be broadcasted to all the receivers
  // - return true, for instance, if the receiver currently processing the event has reached a condition to make the event broadcast stop.
  //
  // in our demo example, the event source is connect to this single receiver, so we can return true or false, it doesn't matter.
}

bool Gui::OnStartButtonDePressed(const nuiEvent& rEvent)
{
  GetEngine()->GetAudioTrack()->Stop();
  return true;
}


