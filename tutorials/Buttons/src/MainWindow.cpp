/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiRadioButtonGroup.h"
#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiBackgroundPane.h"

#include "nuiColorDecoration.h"
#include "nuiGradientDecoration.h"
#include "nuiFrame.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{

}

MainWindow::~MainWindow()
{
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



void MainWindow::OnCreation()
{
  // create a vertical box for the layout
  nuiVBox* pMainBox = new nuiVBox(0);
  pMainBox->SetExpand(nuiExpandShrinkAndGrow);
  pMainBox->SetPosition(nuiFillVertical);
  pMainBox->SetUserWidth(500);
  AddChild(pMainBox);
  
  // create a label width background for information display 
  nuiBackgroundPane* pPane = new nuiBackgroundPane(eInnerBackground);
  pPane->SetBorder(20,20);
  pMainBox->AddCell(pPane, nuiFillHorizontal);
  mpLabel = new nuiLabel();
  pPane->AddChild(mpLabel);
  pPane->SetUserHeight(20);
  
  pMainBox->AddCell(Tutorial_Buttons());
  pMainBox->AddCell(Tutorial_ToggleButtons());
  pMainBox->AddCell(Tutorial_RadioButtons1());
  pMainBox->AddCell(Tutorial_RadioButtons2());

  // make the mainbox's layout fill the entire user size (c.f. line #33)
  pMainBox->SetAllCellsExpand(nuiExpandShrinkAndGrow);
  
}








//******************************************************************************************
//
// nuiButton
//
//

#define TAG_BUTTON1 1
#define TAG_BUTTON2 2
#define TAG_BUTTON3 3
#define TAG_BUTTON4 4

nuiWidget* MainWindow::Tutorial_Buttons()
{
  nuiHBox* pBox = new nuiHBox(0);
  
  nuiLabel* pLabel = new nuiLabel(_T("nuiButton:"), nuiFont::GetFont(16));
  pBox->AddCell(pLabel, nuiLeft);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  
  // a simple button
  nuiButton* pBtn = new nuiButton(_T("button"));
  pBox->AddCell(pBtn, nuiCenter);
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnButtonPressed, (void*)TAG_BUTTON1);

  // a simple button filling the box's cell
  pBtn = new nuiButton(_T("button"));
  pBox->AddCell(pBtn, nuiFill);
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnButtonPressed, (void*)TAG_BUTTON2);
  
  
  // a button with an image
  nglImage pImg(_T("rsrc:/decorations/button1.png"));
  pBtn = new nuiButton(pImg);
  pBox->AddCell(pBtn);
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnButtonPressed, (void*)TAG_BUTTON3);

  
  // a roll-over button using decorations
  nuiColorDecoration* pDecoUp = new nuiColorDecoration(_T("DecoUpHover"), nuiColor(255,0,0,128), 1, nuiColor(0,0,0));
  nuiGradientDecoration* pDecoUpHover = new nuiGradientDecoration(_T("DecoUp"), nuiColor(192,192,192), nuiColor(128,128,128), 1, nuiColor(0,0,0), eStrokeAndFillShape);
  nuiFrame* pFrame = new nuiFrame(_T("DecoDown"), _T("rsrc:/button1.png"), nuiRect(0,0,57,54));
  
  // create a nuiStateDecoration using the three previous decorations for the rollover's three states : up, hover and done
  nuiStateDecoration* pStateDeco = new nuiStateDecoration(_T("Deco"), _T("DecoUp"), _T("DecoDown"), _T("DecoUpHover"));

  pBtn = new nuiButton(pStateDeco);
  pBtn->SetUserSize(40,40);
  pBox->AddCell(pBtn, nuiCenter);
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnButtonPressed, (void*)TAG_BUTTON4);
  
  
  return pBox;
}




//******************************************************************************************
//
// nuiToggleButton
//
//

#define TAG_TOGGLEBUTTON1 1
#define TAG_TOGGLEBUTTON2 2
#define TAG_TOGGLEBUTTON3 3
#define TAG_TOGGLEBUTTON4 4

nuiWidget* MainWindow::Tutorial_ToggleButtons()
{
  nuiHBox* pBox = new nuiHBox(0);
  
  nuiLabel* pLabel = new nuiLabel(_T("nuiToggleButton:"), nuiFont::GetFont(16));
  pBox->AddCell(pLabel, nuiLeft);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  
  // a simple togglebutton
  nuiToggleButton* pBtn = new nuiToggleButton(_T("toggleButton"));
  pBox->AddCell(pBtn);
  mEventSink.Connect(pBtn->ButtonPressed, &MainWindow::OnTogglePressed, (void*)TAG_BUTTON1);
  mEventSink.Connect(pBtn->ButtonDePressed, &MainWindow::OnTogglePressed, (void*)TAG_BUTTON2);

  // a togglebutton, with a "checkbox" look : leave the button without any child
  pBtn = new nuiToggleButton();
  pBox->AddCell(pBtn, nuiCenter);
  mEventSink.Connect(pBtn->ButtonPressed, &MainWindow::OnTogglePressed, (void*)TAG_BUTTON3);
  mEventSink.Connect(pBtn->ButtonDePressed, &MainWindow::OnTogglePressed, (void*)TAG_BUTTON4);
  
  return pBox;
}




//******************************************************************************************
//
// nuiRadioButton
//
//

// first set : radio button with text inside
nuiWidget* MainWindow::Tutorial_RadioButtons1()
{
  nuiHBox* pBox = new nuiHBox(0);
  
  nuiLabel* pLabel = new nuiLabel(_T("nuiRadioButton:"), nuiFont::GetFont(16));
  pBox->AddCell(pLabel, nuiLeft);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);

    
  for (int index = 0; index < 3; index++)                // will create 3 radiobuttons,
  {
    nglString tmp;
    tmp.Format(_T("Radio %d"), index);
    nuiRadioButton* pRadioBut = new nuiRadioButton(tmp);// with text inside
    pBox->AddCell(pRadioBut);
    pRadioBut->SetGroup(_T("radios"));                  // set the radio group for group behavior
    
    // will send an event in the ::OnRadioPressed receiver when the radiobutton is 'activated'
    mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnRadioPressed, (void*)index);  // index is given as a user parameter to recognise the button
  }
    
  
  return pBox;
}
  

// second set : classic radio button, using a radiobutton group
nuiWidget* MainWindow::Tutorial_RadioButtons2()
{
  nuiHBox* pBox = new nuiHBox(0);

  nuiLabel* pLabel = new nuiLabel(_T("nuiRadioButton:"), nuiFont::GetFont(16));
  pBox->AddCell(pLabel, nuiLeft);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
    
  for (int index = 0; index < 3; index++)
  {
    nuiRadioButton* pRadioBut = new nuiRadioButton(); // leave it without any child : it'll get a class "radio" look
    pRadioBut->SetPosition(nuiCenter);
    pBox->AddCell(pRadioBut);
    pRadioBut->SetGroup(_T("radios2"));
    
    mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnRadioPressed, (void*)index);    
  }

  return pBox;

}











//******************************************************************************************
//
// events receivers
//
//

bool MainWindow::OnButtonPressed(const nuiEvent& rEvent)
{
  int32 tag = (int32)rEvent.mpUser;
  
  nglString msg;
  
  switch (tag)
  {
    case TAG_BUTTON1:
      msg = _T("a simple button, with a 'nuiCenter' position");
      break;
    case TAG_BUTTON2:
      msg = _T("the same simple button but with a 'nuiFill' position");
      break;
    case TAG_BUTTON3:
      msg = _T("a simple button with an image inside");
      break;
    case TAG_BUTTON4:
      msg = _T("a rollover button using three decorations");
      break;
  }

  mpLabel->SetText(msg);
  
  return true;
}


bool MainWindow::OnTogglePressed(const nuiEvent& rEvent)
{
  int32 tag = (int32)rEvent.mpUser;
  
  nglString msg;
  
  switch (tag)
  {
    case TAG_TOGGLEBUTTON1:
      msg = _T("a simple togglebutton, pressed");
      break;
    case TAG_TOGGLEBUTTON2:
      msg = _T("a simple togglebutton, released");
      break;
    case TAG_TOGGLEBUTTON3:
      msg = _T("a checkbox, pressed");
      break;
    case TAG_TOGGLEBUTTON4:
      msg = _T("a checkbox, released");
      break;
  }
  
  mpLabel->SetText(msg);
  
  return true;
}



bool MainWindow::OnRadioPressed(const nuiEvent& rEvent)
{
  int32 index = (int32)rEvent.mpUser;
  
  nglString msg;
  msg.Format(_T("radio button #%d"), index);
  mpLabel->SetText(msg);
  
  return true;
}




