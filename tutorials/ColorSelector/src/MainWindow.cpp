/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiBackgroundPane.h"
#include "nuiColorSelector.h"


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
  nuiBackgroundPane* pPane = new nuiBackgroundPane();  
  pPane->SetPosition(nuiCenter);
  AddChild(pPane);
  
  
  std::vector<nuiColor> colors;
  colors.push_back(nuiColor(203,2,3));
  colors.push_back(nuiColor(234,87,10));
  colors.push_back(nuiColor(242,155,3));
  colors.push_back(nuiColor(252,247,74));
  colors.push_back(nuiColor(121,188,94));
  colors.push_back(nuiColor(53,167,134));
  colors.push_back(nuiColor(2,108,87));
  colors.push_back(nuiColor(12,57,187));
  colors.push_back(nuiColor(5,84,234));
  colors.push_back(nuiColor(8,155,232));
  colors.push_back(nuiColor(1,190,252));
  colors.push_back(nuiColor(135,212,212));
  colors.push_back(nuiColor(193,212,164));
  colors.push_back(nuiColor(237,219,131));
  colors.push_back(nuiColor(150,83,34));
  
  nuiColorSelector* pSelector = new nuiColorSelector(colors[0], colors, eModeRGB);
  pPane->AddChild(pSelector);
  pSelector->SetUserSize(220, 200);
}


void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}



