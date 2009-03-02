/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCoverFlow.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  nuiCoverFlow* pFlow = new nuiCoverFlow();
  AddChild(pFlow);
  nglPath p(_T("rsrc:/decorations"));
  std::list<nglPath> children;
  p.GetChildren(&children);
  for (std::list<nglPath>::const_iterator it = children.begin(); children.end() != it; ++it)
  {
    nuiTexture* pTexture = nuiTexture::GetTexture(*it);
    if (pTexture)
      pFlow->AddImage(pTexture);
  }
  //  pFlow->SetBackground(nuiColor(255, 255, 255, 0));
  pFlow->SelectImage(4);
  pFlow->SetBackground(nuiColor(0, 0, 0, 255));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
}



void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;  
  
  App->Quit();
}


