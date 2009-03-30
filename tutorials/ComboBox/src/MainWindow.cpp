/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiComboBox.h"
#include "nuiBackgroundPane.h"
#include "nuiCSS.h"
/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
#ifdef _WIN32_
  LoadCSS(_T("rsrc:/css/MXEditor-Win32.css"));
#else
  LoadCSS(_T("rsrc:/css/main.css"));
#endif  
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
    pNode = new nuiTreeNode(new nuiLabel(str,_T("Tutorial::Label")), false);
    pTree->AddChild(pNode);
  }
  
  // combo choices sub-tree
  for (uint i = 0; i < 10; i++)
  {
    nglString str;
    str.Format(_T("Choice %c"), (char)(_T('A') + i));
    nuiTreeNode* pNode2 = new nuiTreeNode(new nuiLabel(str,_T("Tutorial::Label")), false);
    pNode->AddChild(pNode2);
  }
  
  
  // create combo box
  nuiComboBox* pCombo = new nuiComboBox(pTree, true);
  pCombo->SetUserSize(150,25);
  mEventSink.Connect(pCombo->ValueChanged, &MainWindow::OnComboChanged, (void*)pCombo);
  
  pCombo->SetPosition(nuiCenter);
  AddChild(pCombo);
  
  // create label for output display
  nuiBackgroundPane* pPane = new nuiBackgroundPane(eInnerBackground);
  pPane->SetBorder(20,30);
  pPane->SetPosition(nuiTop);
  pPane->SetUserWidth(280);
  pPane->SetUserHeight(40);
  AddChild(pPane);
  mpLabel = new nuiLabel(_T(" "));
  mpLabel->SetObjectName(_T("Tutorial::Label"));
  pPane->AddChild(mpLabel);

}

bool MainWindow::OnComboChanged(const nuiEvent& rEvent)
{
  nuiComboBox* pCombo = (nuiComboBox*)rEvent.mpUser;
  
  nuiWidget* pWidget = pCombo->GetSelectedWidget();
  nuiLabel* pLabel = (nuiLabel*)pWidget;
  uint32 index = pCombo->GetValue();
  
  nglString msg;
  msg.Format(_T("the user choice is:\n '%ls' (index %d)"), pLabel->GetText().GetChars(), index);
  mpLabel->SetText(msg);
  
  return true;
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}


