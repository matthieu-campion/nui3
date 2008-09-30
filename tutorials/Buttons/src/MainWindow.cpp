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

void MainWindow::OnCreation()
{
  nuiBackgroundPane* pPane = new nuiBackgroundPane();
  AddChild(pPane);
  
  // create a vertical box for the layout
  nuiVBox* pMainBox = new nuiVBox(0);
  pMainBox->SetUserSize(400,300);
  pMainBox->SetPosition(nuiCenter);
  pPane->AddChild(pMainBox);
  
  // create a label for information display 
  mpLabel = new nuiLabel();
  mpLabel->SetPosition(nuiTopLeft);
  mpLabel->SetBorder(20,20);
  AddChild(mpLabel);
  
  
  //***********************************************************
  // first set : radio button with text inside
  //
  {
    // create a hbox as a container and add it to the mainbox
    nuiHBox* pBox = new nuiHBox(0);
    pBox->SetExpand(nuiExpandShrinkAndGrow);               // make the hbox fill the entire provided height, and make shrink/grow with the parent (<=> window)
    pMainBox->AddCell(pBox);
    
    for (int index = 0; index < 4; index++)                // will create 4 radiobuttons,
    {
      nglString tmp;
      tmp.Format(_T("Radio %d"), index);
      nuiRadioButton* pRadioBut = new nuiRadioButton(tmp);// with text inside
      pBox->AddCell(pRadioBut);
      pRadioBut->SetGroup(_T("radios"));                  // set the radio group for group behavior
      
      // will send an event in the ::OnSelected receiver when the radiobutton is 'activated'
      mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnSelected, (void*)index);  // index is given as a user parameter to recognise the button
    }
    
    pBox->SetAllCellsExpand(nuiExpandShrinkAndGrow);      // make the hbox's cells fill the entire width
  }
  
  
  //***********************************************************
  // second set : classic radio button, using a radiobutton group
  //
  nuiRadioButtonGroup* pGroup= new nuiRadioButtonGroup();
  {
    nuiHBox* pBox = new nuiHBox(0);
    pMainBox->AddCell(pBox);
    for (int index = 4; index < 8; index++)
    {
      nuiRadioButton* pRadioBut = new nuiRadioButton();
      pRadioBut->SetPosition(nuiCenter);
      pBox->AddCell(pRadioBut);
      pGroup->AddRadioButton(pRadioBut);
      
      mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnSelected, (void*)index);    
    }
    pBox->SetAllCellsExpand(nuiExpandShrinkAndGrow);      // make the hbox's cells fill the entire width
  }
  
  //***********************************************************
  // third set : classic disabled radio button
  //
  pGroup= new nuiRadioButtonGroup();
  {
    nuiHBox* pBox = new nuiHBox(0);
    pMainBox->AddCell(pBox);
    
    nuiRadioButton* pRadioBut = new nuiRadioButton();
    pRadioBut->SetPosition(nuiCenter);
    pBox->AddCell(pRadioBut);
    pGroup->AddRadioButton(pRadioBut);
    pRadioBut->SetEnabled(false);
    
    pRadioBut = new nuiRadioButton();
    pRadioBut->SetPosition(nuiCenter);
    pBox->AddCell(pRadioBut);
    pGroup->AddRadioButton(pRadioBut);
    pRadioBut->SetPressed(true);
    pRadioBut->SetEnabled(false);
    
    // don't need to connect the buttons' events, since all we wanna do here is to show how disabled buttons are looking
    
    pBox->SetAllCellsExpand(nuiExpandShrinkAndGrow);      // make the hbox's cells fill the entire width
  }
  
  
  
  // make the mainbox's layout fill the entire user size (c.f. line #33)
  pMainBox->SetAllCellsExpand(nuiExpandShrinkAndGrow);
  
}


bool MainWindow::OnSelected(const nuiEvent& rEvent)
{
  int32 index = (int32)rEvent.mpUser;
  
  nglString msg;
  msg.Format(_T("the user has selected radio button #%d"), index);
  mpLabel->SetText(msg);
  
  return true;
}



void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



