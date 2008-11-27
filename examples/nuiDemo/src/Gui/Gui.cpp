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
: nuiVBox(0), mEventSink(this), mTimerSink(this), mTimer(0.001)
{
  SetObjectClass(_T("Gui"));
  SetExpand(nuiExpandShrinkAndGrow);
  
  
  // row 1  : title image *****************************************
  //
  nuiImage* pTitleImg = new nuiImage();
  pTitleImg->SetObjectName(_T("ImageTitle"));
  AddCell(pTitleImg, nuiTopLeft);
    

  // row 2: oscillo ***********************************************
  //
  
  nuiSimpleContainer* pOscilloBkg = new nuiSimpleContainer();
  pOscilloBkg->SetObjectName(_T("guiOscilloBackground"));
  AddCell(pOscilloBkg, nuiCenter);
  
  guiOscillo* pOscillo = new guiOscillo(GetEngine()->GetAudioTrack()->GetSamplesBuffer());
  pOscilloBkg->AddChild(pOscillo);
  
  
  // row 3: gui switch knob and on/off button  *********************
  //
  nuiHBox* pBox = new nuiHBox(2);
  AddCell(pBox);
  
  // gui switch button (to switch from the audio controls to the explanation text display)
  nuiToggleButton* pSwitch = new nuiToggleButton();
  pSwitch->SetObjectName(_T("ButtonSwitch"));
  pBox->SetCell(0, pSwitch, nuiBottom);
  
  pSwitch->SetPressed(true); // by default
  
  // connect switch event to receiver
  mEventSink.Connect(pSwitch->ButtonPressed, &Gui::OnSwitchChanged, (void*)pSwitch);  
  mEventSink.Connect(pSwitch->ButtonDePressed, &Gui::OnSwitchChanged, (void*)pSwitch);  
  
  // on/off button
  nuiToggleButton* pStartBtn = new nuiToggleButton();
  pStartBtn->SetObjectName(_T("ButtonStart"));
  pBox->SetCell(1, pStartBtn, nuiTop);
  
  // connect the button event to the gui controller.
  // <=> in the nui-way-of-thinking : connect the button event source you're interessted in to the event receiver of your choice
  mEventSink.Connect(pStartBtn->ButtonPressed, &Gui::OnStartButtonPressed);
  mEventSink.Connect(pStartBtn->ButtonDePressed, &Gui::OnStartButtonDePressed);
  
  // row 4: sound controls and text *******************************
  //
  nuiSimpleContainer* pPaneBkg = new nuiSimpleContainer();
  pPaneBkg->SetObjectName(_T("PaneBkg"));
  AddCell(pPaneBkg, nuiBottom);
  SetCellExpand(GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // a box for row layout
  nuiHBox* pPaneBox = new nuiHBox(0);
  pPaneBox->SetExpand(nuiExpandShrinkAndGrow);
  pPaneBkg->AddChild(pPaneBox);
  
  // area for the sound controls, in the first cell
  mpControls = BuildControls();
  pPaneBox->AddCell(mpControls);
  pPaneBox->SetCellExpand(pPaneBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  

  // a text to display, in the second cell
  mpText = new nuiLabel(_T("TEXT"));
  mpText->SetObjectName(_T("Text"));
  pPaneBox->AddCell(mpText, nuiCenter);
  pPaneBox->SetCellExpand(pPaneBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // load text contents from binary's resources
  nglString textContents;
  nglIStream* pTextInput = nglPath(_T("rsrc:/text.txt")).OpenRead();
  NGL_ASSERT(pTextInput);
  pTextInput->ReadText(textContents);
  mpText->SetText(textContents);
  
  // hide the second cell by default
  mControlsPaneWidth = 246;
  mTextPaneWidth = 0;
  mpControls->SetUserSize(mControlsPaneWidth,160);
  mpText->SetUserSize(mTextPaneWidth,160);
  
  
  
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
  
  // volume slider
  nuiSlider* pVolumeSlider = new nuiSlider(nuiHorizontal, nuiRange(1.f, 0.f, 1.f, 0.1f, 0.f, 0.f));
  pVolumeSlider->SetObjectName(_T("SliderVolume"));
  pBox->AddCell(pVolumeSlider, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect slider event to receiver
  // InteractiveValueChanged event : real-time slider value change
  // ValueChanged event: once the value has been set and the mouse has been released from the slider
  mEventSink.Connect(pVolumeSlider->InteractiveValueChanged, &Gui::OnVolumeSliderChanged, (void*)pVolumeSlider);
  
  // frequency knob
  nuiImageSequence* pImgSequence = new nuiImageSequence(31, "rsrc:/decorations/knob.png", nuiVertical);
  
  nuiKnob* pFreqKnob = new nuiKnob(nuiRange(.4f, 0.0f, 1.0f), pImgSequence, false);
  pFreqKnob->SetObjectName(_T("KnobFrequency"));
  pBox->AddCell(pFreqKnob, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect knob event to receiver
  mEventSink.Connect(pFreqKnob->InteractiveValueChanged, &Gui::OnFreqKnobChanged, (void*)pFreqKnob);
  
  // Q knob
  nuiKnob* pQKnob = new nuiKnob(nuiRange(0.0f, 0.0f, 1.0f), pImgSequence, false);
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

bool Gui::OnSwitchChanged(const nuiEvent& rEvent)
{
  nuiToggleButton* pBtn = (nuiToggleButton*)rEvent.mpUser;
  NGL_ASSERT(pBtn);

  mTimer.Stop();
  mTimerSink.DisconnectAll();

  if (!pBtn->IsPressed())
    mTimerSink.Connect(mTimer.Tick, &Gui::OnShowText);
  else
    mTimerSink.Connect(mTimer.Tick, &Gui::OnShowControls);
  
  mTimer.Start();
  return true;
}


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


bool Gui::OnVolumeSliderChanged(const nuiEvent& rEvent)
{
  nuiSlider* pSlider = (nuiSlider*)rEvent.mpUser;
  NGL_ASSERT(pSlider);
  
  GetEngine()->SetVolume(pSlider->GetRange().GetValue());
  
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


bool Gui::OnShowText(const nuiEvent& rEvent)
{
  if (mControlsPaneWidth <= 0)
  {
    mTimer.Stop();
    return true;
  }
  
  mControlsPaneWidth--;
  mTextPaneWidth++;
  mpControls->SetUserSize(mControlsPaneWidth,160);
  mpText->SetUserSize(mTextPaneWidth,160);
  return true;  
}


bool Gui::OnShowControls(const nuiEvent& rEvent)
{
  if (mTextPaneWidth <= 0)
  {
    mTimer.Stop();
    return true;
  }
  
  mControlsPaneWidth++;
  mTextPaneWidth--;
  mpControls->SetUserSize(mControlsPaneWidth,160);
  mpText->SetUserSize(mTextPaneWidth,160);
  return true;  
}


