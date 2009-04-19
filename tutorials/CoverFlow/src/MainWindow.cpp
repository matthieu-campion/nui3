/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiVBox.h"
#include "nuiCoverFlow.h"
#include "nuiAttributeAnimation.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  EnableAutoRotation(false);
#ifdef _UIKIT_
  SetRotation(270);
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
  pFlow->SelectImageNow(4);

  { // Bounce
    nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
    pAnim->SetTargetObject(pFlow);
    pAnim->SetTargetAttribute(_T("SelectionYOffset"));
    pAnim->SetStartValue(0);
    pAnim->SetEndValue(0.2);
    pAnim->SetDuration(.5);
    pAnim->SetEasing(&nuiEasingSquareRev);
    pFlow->AddAnimation(_T("Bounce"), pAnim);
    pAnim->Play(-1, eAnimLoopPingPong);
  }

  { // Appear
    nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
    pAnim->SetTargetObject(pFlow);
    pAnim->SetTargetAttribute(_T("SideDepth"));
    pAnim->SetStartValue(5);
    pAnim->SetEndValue(.7);
    pAnim->SetDuration(1.5);
    pAnim->SetEasing(&nuiEasingCubicRev);
    pFlow->AddAnimation(_T("Appear"), pAnim);
    pAnim->Play();
  }
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


