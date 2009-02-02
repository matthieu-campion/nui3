/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "Gui/Gui.h"
#include "nuiHBox.h"
#include "nuiSlider.h"
#include "nuiKnob.h"
#include "nuiHyperLink.h"
#include "nuiURL.h"
#include "Gui/guiOscillo.h"
#include "Application.h"


//
// Gui inherits from nuiVBox
//

Gui::Gui()
: nuiVBox(0), mEventSink(this), mTimerSink(this), mTimer(0.020), mShownText(false), mShownControls(false)
{
  SetObjectClass(_T("Gui"));
  SetExpand(nuiExpandShrinkAndGrow);
  
  
  // row 1  : title image *****************************************
  //
  nuiButton* pTitleBtn = new nuiButton();
  pTitleBtn->SetObjectName(_T("ButtonTitle"));
  AddCell(pTitleBtn, nuiTopLeft);
  
  mEventSink.Connect(pTitleBtn->Activated, &Gui::OnTitleButtonActivated);
    

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
  mpPaneBkg = new nuiSimpleContainer();
  mpPaneBkg->SetObjectName(_T("PaneBkg"));
  AddCell(mpPaneBkg, nuiCenter);
  
  
  // area for the sound controls, in the first cell
  mpControls = BuildControls();
  mpControls->SetUserSize(246, 160);
  mpPaneBkg->AddChild(mpControls);
  

  // a text to display, in the second cell
  mpTextPanel = new nuiVBox(2);
  mpTextPanel->SetExpand(nuiExpandShrinkAndGrow);
  mpTextPanel->SetUserSize(246,160);
  mpPaneBkg->AddChild(mpTextPanel);
  
  nuiLabel* pText = new nuiLabel(_T("TEXT"));
  pText->SetObjectName(_T("Text"));
  mpTextPanel->SetCell(0, pText);
  
  // with a url to libnui web page
  nuiHyperLink* pLink = new nuiHyperLink(_T("http://libnui.net"), _T("more info on http://libnui.net"));
  pLink->SetPosition(nuiCenter);
  pLink->SetBorder(0, 10);
  mpTextPanel->SetCell(1, pLink);
  

  // load text contents from binary's resources
  nglString textContents;
  nglIStream* pTextInput = nglPath(_T("rsrc:/text.txt")).OpenRead();
  NGL_ASSERT(pTextInput);
  pTextInput->ReadText(textContents);
  pText->SetText(textContents);
  

  // show audio controls by default
  mpControls->SetVisible(true);
  mpControls->SetEnabled(true);
  mpTextPanel->SetVisible(false);
  mpTextPanel->SetEnabled(false);
  
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
  
  nuiKnob* pFreqKnob = new nuiKnob(nuiRange(.4f, 0.0f, 1.0f, 0.1f, 0.1f, 0.0f), pImgSequence, false);
  pFreqKnob->SetObjectName(_T("KnobFrequency"));
//  pFreqKnob->SetUserSize(100,100);
  pBox->AddCell(pFreqKnob, nuiCenter);
  pBox->SetCellExpand(pBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // connect knob event to receiver
  mEventSink.Connect(pFreqKnob->InteractiveValueChanged, &Gui::OnFreqKnobChanged, (void*)pFreqKnob);
  
  // Q knob
  nuiKnob* pQKnob = new nuiKnob(nuiRange(0.0f, 0.0f, 1.0f, 0.1f, 0.1f, 0.0f), pImgSequence, false);
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

//  mTimer.Stop();
//  mTimerSink.DisconnectAll();
//
//  if (!pBtn->IsPressed())
//    mTimerSink.Connect(mTimer.Tick, &Gui::OnShowText);
//  else
//    mTimerSink.Connect(mTimer.Tick, &Gui::OnShowControls);
//  
//  mTimer.Start();
  
  
  if (!pBtn->IsPressed())
    ShowText(true);
  else
    ShowControls(true);
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



void Gui::ShowControls(bool show)
{
  mTimer.Stop();
  mTimerSink.DisconnectAll();

  mpControls->SetEnabled(false);
  mpTextPanel->SetEnabled(false);

  mpControls->SetVisible(true);
  mpControls->SetAlpha(0.f);
  mAlphaControls = 0.f;
  mAlphaText = 1.f;

  mTimerSink.Connect(mTimer.Tick, &Gui::OnShowControls);
  mTimer.Start();
}


void Gui::ShowText(bool show)
{
  mTimer.Stop();
  mTimerSink.DisconnectAll();
  
  mpControls->SetEnabled(false);
  mpTextPanel->SetEnabled(false);
  
  mpTextPanel->SetVisible(true);
  mpTextPanel->SetAlpha(0.f);
  mAlphaControls = 1.f;
  mAlphaText = 0.f;
  
  mTimerSink.Connect(mTimer.Tick, &Gui::OnShowText);
  mTimer.Start();
}


bool Gui::OnShowText(const nuiEvent& rEvent)
{
  if (mAlphaText >= 1.f)
  {
    mTimer.Stop();
    mpControls->SetVisible(false);
    mpTextPanel->SetEnabled(true);
  }
  
  mAlphaControls -= 0.1f;
  mAlphaText += 0.1f;
  if (mAlphaControls < 0.f)
    mAlphaControls = 0.f;
  if (mAlphaText > 1.f)
    mAlphaText = 1.f;
  mpControls->SetAlpha(mAlphaControls);
  mpTextPanel->SetAlpha(mAlphaText);
  mpPaneBkg->Invalidate();
  
  return true;  
}


bool Gui::OnShowControls(const nuiEvent& rEvent)
{
  if (mAlphaControls >= 1.f)
  {
    mTimer.Stop();
    mpTextPanel->SetVisible(false);
    mpControls->SetEnabled(true);
  }
  
  mAlphaControls += 0.1;
  mAlphaText -= 0.1;
  if (mAlphaText < 0.f)
    mAlphaText = 0.f;
  if (mAlphaControls > 1.f)
    mAlphaControls = 1.f;
  
  mpControls->SetAlpha(mAlphaControls);
  mpTextPanel->SetAlpha(mAlphaText);
  mpPaneBkg->Invalidate();

  return true;  
}



bool Gui::OnTitleButtonActivated(const nuiEvent& rEvent)
{
  NGL_OUT(_T("click\n"));
  
  // Open libnui web page
  nuiURL url(_T("http://libnui.net"));
  url.OpenBrowser();
  return true;
}
