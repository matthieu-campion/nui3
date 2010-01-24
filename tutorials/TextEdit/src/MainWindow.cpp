/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiVBox.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  //mClearBackground = false;
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiScrollView* pScrollView = new nuiScrollView;
  nuiEditText* pText = new nuiEditText(_T("Type something here"));
  pText->SetTextColor(nuiColor(128,128,128));
  AddChild(pScrollView);
  pScrollView->AddChild(pText);
  
  pScrollView->SetBorder(10, 10, 32, 32);
}
