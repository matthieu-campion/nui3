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
#include "nuiImageAnimation.h"
#include "nuiTask.h"



/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  LoadCSS(_T("rsrc:/css/style.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  mpNav = new nuiNavigationController();
  AddChild(mpNav);
  
  MyViewController* pView = new MyViewController();
  mpNav->PushViewController(pView, false);
  
  
}







void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  delete pF;
  
  if (res)
  {
    nuiMainWindow::SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}











MyViewController::MyViewController()
{
  SetObjectClass(_T("MyViewController"));
  SetTitle(_T("Swipe Detector"));
  
  mpArrowsContainer = new nuiSimpleContainer();
  mpArrowsContainer->SetObjectName(_T("ArrowsContainer"));
  AddChild(mpArrowsContainer);
  
  mpArrows = new nuiImageAnimation(17, _T("rsrc:/decorations/directions.png"));
  mpArrows->SetObjectName(_T("Arrows"));
  mpArrowsContainer->AddChild(mpArrows);
}


MyViewController::~MyViewController()
{

}

// virtual 
void MyViewController::HandleSwipe(nuiPosition swipeDirection)
{
  uint32 index = 0;
  
  switch (swipeDirection)
  {
    case nuiTop: index = 1; break;
    case nuiTopRight: index = 2; break;
    case nuiRight: index = 3; break;
    case nuiBottomRight: index = 4; break;
    case nuiBottom: index = 5; break;
    case nuiBottomLeft: index = 6; break;
    case nuiLeft: index = 7; break;
    case nuiTopLeft: index = 8; break;
  }
  
  mpArrows->SetFrameIndex(index);
  mpArrows->Invalidate();
  
  SetDelay(nuiMakeTask(this, &MyViewController::ResetArrows), 0.4f);
}


void MyViewController::SetDelay(nuiTask* pTask, float seconds)
{
  nuiAnimation::RunOnAnimationTick(pTask, seconds / nuiAnimation::GetTimer()->GetPeriod());      
}


void MyViewController::ResetArrows()
{
  
  mpArrows->SetFrameIndex(0);
  mpArrows->Invalidate();
}
