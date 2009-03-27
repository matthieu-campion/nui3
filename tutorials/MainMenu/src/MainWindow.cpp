/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiVBox.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(_T("../data"))), mEventSink(this)
{
  nuiMainMenu* pMenu = GetApp()->GetMainMenu();
  if (pMenu)
    SetMainMenu(pMenu);
  mpCurrentTestMenu = GetApp()->GetTestMenu();
  mUniqueID = 1;
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiVBox* pBox = new nuiVBox(0);
  AddChild(pBox);
  
  nuiButton* pBtn = new nuiButton(_T("1 - add a item in Test menu"));
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnAddItem);
  pBox->AddCell(pBtn, nuiLeft);
  
  pBtn = new nuiButton(_T("2 - add a submenu in Test menu"));
  mEventSink.Connect(pBtn->Activated, &MainWindow::OnAddSubMenu);
  pBox->AddCell(pBtn, nuiLeft);
  
  
  mText = _T("play with the menu bar, and check the results below:\n");
  mpText = new nuiLabel(mText, nuiFont::GetFont(14));
  mpText->SetPosition(nuiTop);
  pBox->AddCell(mpText);
}

void MainWindow::OnClose()
{
  App->Quit();
}

void MainWindow::MyCommand(const nglString& rMsg)
{
  nglString msg = rMsg;
  msg.Append(_T("\n"));
  mText.Append(msg);
  mpText->SetText(mText);  
}


bool MainWindow::OnAddItem(const nuiEvent& rEvent)
{
  uint32 ID = mUniqueID;
  mUniqueID++;
  
  nglString name;
  name.Format(_T("New%d"), ID);
  nuiMainMenuItem* pNew = new nuiMainMenuItem(name);
  mpCurrentTestMenu->AddChild(pNew);
  mEventSink.Connect(pNew->Activated, &MainWindow::OnMenuCommand, (void*)ID);
  
  mpLastItem = pNew;
  
  return true;
}

bool MainWindow::OnAddSubMenu(const nuiEvent& rEvent)
{
  mpCurrentTestMenu = mpLastItem;
  return OnAddItem(rEvent);
}

bool MainWindow::OnMenuCommand(const nuiEvent& rEvent)
{
  uint32 ID = (uint32)rEvent.mpUser;
  nglString msg;
  msg.Format(_T("event item New%d"), ID);
  MyCommand(msg);
  return true;
}



