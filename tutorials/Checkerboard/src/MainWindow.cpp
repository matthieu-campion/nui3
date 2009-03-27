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
#include "nuiCheckerboardDecoration.h"

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

  // add an empty widget to its parent (the MainWindow)
  nuiWidget* pWidget = new nuiWidget();
  AddChild(pWidget);

  // create a checkerboard decoration and assign it to the widget
  nuiCheckerboardDecoration* pDeco = new nuiCheckerboardDecoration(_T("checker1"));
  pDeco->SetBackgroundColor(nuiColor(255,255,255));
  pDeco->SetTileColor(nuiColor(180,180,180));
  pDeco->SetTilesPerLine(8);
  pDeco->SetStrokeColor(nuiColor(0,0,0));
  pDeco->SetStrokeSize(1);
  
  pWidget->SetDecoration(pDeco);
  
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



