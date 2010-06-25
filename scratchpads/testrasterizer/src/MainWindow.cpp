/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDecoration(NULL);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  bool test1 = true;
  bool test2 = true;

  if (test1)
  {
    nuiImage* pImage = new nuiImage(_T("rsrc:/rasters2.png"));
    pImage->SetPosition(nuiCenter);
    pImage->SetAlpha(0.5);
    pImage->SetEnabled(false);
    AddChild(pImage);
  }

  if (test2)
  {
    nuiImage* pImage = new nuiImage(_T("rsrc:/testalacon.png"));
    pImage->SetPosition(nuiCenter);
    pImage->SetAlpha(0.5);
    pImage->SetEnabled(false);
    AddChild(pImage);
  }
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



