/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"

#include "Gui/Gui.h"
#include "Engine/Engine.h"

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


//virtual
void MainWindow::OnCreation()
{
  // build engine
  Engine* pEngine();

  // build gui
  Gui* pGui = new Gui();
  AddChild(pGui);
}


// virtual
void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



