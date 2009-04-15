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
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)),
    mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiWidget* pBg = new nuiImage(_T("rsrc:/decorations/image.png"));
  pBg->SetRect(nuiRect(-(float)GetWidth(), -(float)GetHeight(), (float)GetWidth() * 3, (float)GetHeight() * 3));
  pBg->SetPosition(nuiCenter);
  pBg->SetAlpha(.02);
  pBg->SetLayoutAnimationEasing(nuiEasingCubicRev);
  pBg->SetLayoutAnimationDuration(1);
               
  AddChild(pBg);
  
  {
    nuiVBox* pBox = new nuiVBox();
    pBox->SetExpand(nuiExpandShrinkAndGrow);
    
    nuiButton* pButton = new nuiButton(nglString(_T("Press Me!")));
    pBox->AddCell(pButton, nuiLeft);
    pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
    mEventSink.Connect(pButton->Activated, &MainWindow::ChangeLayout, pButton);
    pButton->SetLayoutAnimationDuration(1);
    pButton->SetLayoutAnimationEasing(nuiEasingSinus);

    AddChild(pBox);
  }
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}

bool MainWindow::ChangeLayout(const nuiEvent& rEvent)
{
  nuiButton* pButton = (nuiButton*)rEvent.mpUser;
  uint32 pos = pButton->GetPosition() + 1;
  pos = pos % (nuiFillBottom + 1);
  if (pos == 0)
    pos = nuiLeft;
  ((nuiLabel*)pButton->GetChild(0))->SetText(nuiGetPosition((nuiPosition)pos));
  pButton->SetPosition((nuiPosition)pos);
  return true;
}
