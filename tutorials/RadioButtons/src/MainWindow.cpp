/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiRadioButtonGroup.h"

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
  nuiSimpleContainer* pContainer = new nuiSimpleContainer();
  pContainer->SetUserSize(400,300);
  pContainer->SetPosition(nuiCenter);
  AddChild(pContainer);

  mpLabel = new nuiLabel();
  mpLabel->SetPosition(nuiTopLeft);
  mpLabel->SetBorder(20,20);
  AddChild(mpLabel);
      
  nuiWidgetBox* pRadioBoxBox = new nuiWidgetBox(nuiVertical);
  pContainer->AddChild(pRadioBoxBox);

  
  // Radio groups:
  {
    nuiWidgetBox* pRadioBox = new nuiWidgetBox(nuiHorizontal);
    pRadioBoxBox->AddChild(pRadioBox);
    for (int index = 0; index < 4; index++)
    {
      nglString tmp;
      tmp.Format(_T("Radio %d"), index);
      nuiRadioButton* pRadioBut = new nuiRadioButton(tmp);
      pRadioBox->AddChild(pRadioBut);
      pRadioBut->SetGroup(_T("radios"));
      
      mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnSelected, (void*)index);
    }
  }
  
  // buttons without internal label:
  nuiRadioButtonGroup* pGroup= new nuiRadioButtonGroup();
  {
    nuiWidgetBox* pRadioBox = new nuiWidgetBox(nuiHorizontal);
    pRadioBoxBox->AddChild(pRadioBox);
    for (int index = 4; index < 8; index++)
    {
      nuiRadioButton* pRadioBut = new nuiRadioButton();
      pRadioBut->SetPosition(nuiCenter);
      pRadioBox->AddChild(pRadioBut);
      pGroup->AddRadioButton(pRadioBut);

      mEventSink.Connect(pRadioBut->Activated, &MainWindow::OnSelected, (void*)index);    
    }
  }
  
  // disabled buttons without internal label:
  pGroup= new nuiRadioButtonGroup();
  {
    nuiWidgetBox* pRadioBox = new nuiWidgetBox(nuiHorizontal);
    pRadioBoxBox->AddChild(pRadioBox);
    
    nuiRadioButton* pRadioBut = new nuiRadioButton();
    pRadioBut->SetPosition(nuiCenter);
    pRadioBox->AddChild(pRadioBut);
    pGroup->AddRadioButton(pRadioBut);
    pRadioBut->SetEnabled(false);
    
    pRadioBut = new nuiRadioButton();
    pRadioBut->SetPosition(nuiCenter);
    pRadioBox->AddChild(pRadioBut);
    pGroup->AddRadioButton(pRadioBut);
    pRadioBut->SetPressed(true);
    pRadioBut->SetEnabled(false);
  }

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



