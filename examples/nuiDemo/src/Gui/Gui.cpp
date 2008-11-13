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
  nuiSimpleContainer* pOscilloBkg = new nuiSimpleContainer();
  pOscilloBkg->SetObjectName(_T("guiOscilloBackground"));
  pBox->AddCell(pOscilloBkg, nuiCenter);
  
  guiOscillo* pOscillo = new guiOscillo(GetEngine()->GetAudioTrack()->GetSamplesBuffer());
  pOscilloBkg->AddChild(pOscillo);
  pOscillo->SetBorder(15,15);
  

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
  pBox->AddCell(pText, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // load text contents from binary's resources
  nglString textContents;
  nglIStream* pTextInput = nglPath(_T("rsrc:/text.txt")).OpenRead();
  NGL_ASSERT(pTextInput);
  pTextInput->ReadText(textContents);
  pText->SetText(textContents);
  
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
  nuiSlider* pQSlider = new nuiSlider(nuiHorizontal, nuiRange(0, 0, 100));
  pQSlider->SetObjectName(_T("SliderQ"));
  pBox->AddCell(pQSlider, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect slider event to receiver
  // InteractiveValueChanged event : real-time slider value change
  // ValueChanged event: once the value has been set and the mouse has been released from the slider
  mEventSink.Connect(pQSlider->InteractiveValueChanged, &Gui::OnQSliderChanged, (void*)pQSlider);
  
  // frequency knob
  nuiKnob* pFreqKnob = new nuiKnob(nuiRange(.4f, 0.0f, 1.0f));
  pFreqKnob->SetObjectName(_T("KnobFrequency"));
  pBox->AddCell(pFreqKnob, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect knob event to receiver
  mEventSink.Connect(pFreqKnob->InteractiveValueChanged, &Gui::OnFreqKnobChanged, (void*)pFreqKnob);
  
  // Q knob
  nuiKnob* pQKnob = new nuiKnob(nuiRange(0.0f, 0.0f, 1.0f));
  pQKnob->SetObjectName(_T("KnobQ"));
  pBox->AddCell(pQKnob, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect knob event to receiver
  mEventSink.Connect(pQKnob->InteractiveValueChanged, &Gui::OnQKnobChanged, (void*)pQKnob);

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


bool Gui::OnQSliderChanged(const nuiEvent& rEvent)
{
  nuiSlider* pQSlider = (nuiSlider*)rEvent.mpUser;
  NGL_ASSERT(pQSlider);
  
  
  GetEngine()->GetFilter().SetQ(pQSlider->GetRange().GetValue());
  GetEngine()->GetFilter().ComputeCoefficients();
  
  return true;
}


bool Gui::OnFreqKnobChanged(const nuiEvent& rEvent)
{
  nuiKnob* pFreqKnob = (nuiKnob*)rEvent.mpUser;
  NGL_ASSERT(pFreqKnob);
  
  
  float val = pFreqKnob->GetRange().GetValue();
  val = val * val * val * val;
  val *= 19980;
  val += 20;
  GetEngine()->GetFilter().SetFreq(val);
  GetEngine()->GetFilter().ComputeCoefficients();
  return true;
}


bool Gui::OnQKnobChanged(const nuiEvent& rEvent)
{
  nuiKnob* pQKnob = (nuiKnob*)rEvent.mpUser;
  NGL_ASSERT(pQKnob);
  
  
  GetEngine()->GetFilter().SetQ(pQKnob->GetRange().GetValue());
  GetEngine()->GetFilter().ComputeCoefficients();
  return true;
}


