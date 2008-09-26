/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiComboBox.h"
#include "nuiGrid.h"

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
  // build combo choices tree
  nuiTreeNodePtr pNode = NULL;
  nuiTreeNodePtr pTree = new nuiTreeNode(_T("Root"), true);
  for (uint i = 0; i < 10; i++)
  {
    nglString str;
    str.Format(_T("Choice #%d"), i);
    pNode = new nuiTreeNode(str, true);
    pTree->AddChild(pNode);
  }

  // combo choices sub-tree
  for (uint i = 0; i < 10; i++)
  {
    nglString str;
    str.Format(_T("Choice %c"), (char)(_T('A') + i));
    nuiTreeNode* pNode2 = new nuiTreeNode(str, true);
    pNode->AddChild(pNode2);
  }
  nuiTreeNode* pNode2 = new nuiTreeNode(_T("my last long choice"), true);
  pNode->AddChild(pNode2);


  // create combo box
  nuiComboBox* pCombo = new nuiComboBox(pTree, true);
  mEventSink.Connect(pCombo->ValueChanged, &MainWindow::OnComboChanged, (void*)pCombo);
  
  // create label for output display
  mpLabel = new nuiLabel(_T(" "));
  
  // grid for gui layout
  nuiGrid* pGrid = new nuiGrid(3,4);
  pGrid->SetCell(1,1, pCombo);
  pGrid->SetCell(1,2, mpLabel);

  AddChild(pGrid);
}


bool MainWindow::OnComboChanged(const nuiEvent& rEvent)
{
  nuiComboBox* pCombo = (nuiComboBox*)rEvent.mpUser;
  
  nuiWidget* pWidget = pCombo->GetSelectedWidget();
  nuiLabel* pLabel = (nuiLabel*)pWidget;
  uint32 index = pCombo->GetValue();
  
  nglString msg;
  msg.Format(_T("the user choice is : '%ls' (index %d)"), pLabel->GetText().GetChars(), index);
  mpLabel->SetText(msg);
  
  return true;
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



