/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiHBox.h"
#include "nuiVBox.h"




/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{

  SetDebugMode(true);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // mainbox for layout
  nuiHBox* pMainBox = new nuiHBox(0);
  pMainBox->SetPosition(nuiCenter);
  AddChild(pMainBox);
  
  // bloc 1 : knob with background
  nuiVBox* pBloc = new nuiVBox(0);
  pMainBox->AddCell(pBloc);
  pMainBox->SetCellExpand(pMainBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  nuiKnob* pKnob = new nuiKnob(nuiRange(0, 0, 30, 1, 10, 10), true/*with background*/);
  pKnob->SetPosition(nuiCenter);
  pBloc->AddCell(pKnob);
  
  nuiLabel* pLabel = new nuiLabel(_T("with background"), nuiFont::GetFont(9));
  pBloc->AddCell(pLabel);

  
  
  // bloc 2 : knob without background
  pBloc = new nuiVBox(0);
  pMainBox->AddCell(pBloc);
  pMainBox->SetCellExpand(pMainBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  pKnob = new nuiKnob(nuiRange(0, 0, 30, 1, 10, 10), false/*no background*/);
  pKnob->SetPosition(nuiCenter);
  pBloc->AddCell(pKnob);
  pBloc->SetCellExpand(pBloc->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  pLabel = new nuiLabel(_T("without background"), nuiFont::GetFont(9));
  pBloc->AddCell(pLabel);

  
  // bloc 3 : knob with a label to display the value
  pBloc = new nuiVBox(0);
  pMainBox->AddCell(pBloc);
  pMainBox->SetCellExpand(pMainBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  mpKnob = new nuiKnob(nuiRange(0, 0, 30, 1, 10, 10), false/*no background*/);
  mpKnob->SetPosition(nuiCenter);
  pBloc->AddCell(mpKnob);
  pBloc->SetCellExpand(pBloc->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  mpLabel = new nuiLabel(_T("0"), nuiFont::GetFont(10));
  pBloc->AddCell(mpLabel, nuiCenter);  
  
  mEventSink.Connect(mpKnob->InteractiveValueChanged, &MainWindow::OnValueChanged);
  
  pLabel = new nuiLabel(_T("with value output"), nuiFont::GetFont(9));
  pBloc->AddCell(pLabel);
  
  
  
  pMainBox->SetAllCellsSpacing(20);
  
}


bool MainWindow::OnValueChanged(const nuiEvent& rEvent)
{
  nglString value;
  value.Format(_T("%.0f"), mpKnob->GetRange().GetValue());
  mpLabel->SetText(value);
  return true;
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



