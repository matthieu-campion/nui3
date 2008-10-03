/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"

#include "nuiKnobSequence.h"


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
  
  nuiKnobSequence* pKnob = new nuiKnobSequence(nuiRange(0, 0, 30, 1, 10, 10));
  pKnob->SetPosition(nuiCenter);
  AddChild(pKnob);
  
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



