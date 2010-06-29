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
  : mEventSink(this),
    nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
    
{
  //              mClearBackground = false;
  SetDebugMode(true);
}

MainWindow::~MainWindow()
{
}


void MainWindow::OnCreation()
{
  nuiWidget* pBg = new nuiImage(_T("rsrc:/decorations/image.png"));
  pBg->SetRect(nuiRect(-(float)GetWidth(), -(float)GetHeight(), (float)GetWidth() * 3, (float)GetHeight() * 3));
  pBg->SetPosition(nuiCenter);
  pBg->SetAlpha(.1);
  //  pBg->SetLayoutAnimationEasing(nuiEasingCubicRev);
  pBg->SetLayoutAnimationEasing(nuiEasingElasticOut<700>);
  pBg->SetLayoutAnimationDuration(3);
               
  AddChild(pBg);
  
  nuiVBox* pBox = new nuiVBox();
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  
  nuiButton* pButton = new nuiButton();
  nuiLabel* pLabel = new nuiLabel(nglString(_T("Press Me!")), nuiFont::GetFont(35));
  pLabel->SetPosition(nuiCenter);
  pButton->AddChild(pLabel);
  pButton->SetAlpha(.5);
  pBox->AddCell(pButton, nuiCenter);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pButton->SetLayoutAnimationDuration(1);
  //pButton->SetLayoutAnimationEasing(nuiEasingSinus);
  pButton->SetLayoutAnimationEasing(nuiEasingElasticOut<500>);
  //pButton->SetLayoutAnimationEasing(nuiEasingBounceOut);
  //pButton->SetLayoutAnimationEasing(nuiEasingBounceIn);
  //pButton->SetLayoutAnimationEasing(nuiEasingBackIn);

  mEventSink.Connect(pButton->Activated, &MainWindow::ChangeLayout, pButton);

  AddChild(pBox);
 
  EnableAutoRotation(false);
  SetRotation(90);
  
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
