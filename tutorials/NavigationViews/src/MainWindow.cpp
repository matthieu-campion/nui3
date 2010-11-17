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
  
  MyViewController* pView = new MyViewController(mpNav);
  pView->Acquire();  
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



//************************************************************************************
//
// MyViewController
//
//************************************************************************************

// static 
uint32 MyViewController::mCount = 0;
nuiNavigationController* MyViewController::mpNav = NULL;

MyViewController::MyViewController(nuiNavigationController* pNav)
: nuiViewController(), mEventSink(this)
{
  if (!mpNav)
    mpNav = pNav;
      
  nglString name;
  name.Format(_T("MyViewController%d"), mCount % 3);
  SetObjectClass(name);
  
  name.Format(_T("%d"), mCount);
  nuiLabel* pLabel = new nuiLabel(name);
  pLabel->SetObjectName(_T("ViewControllerName"));
  AddChild(pLabel);
  
  nuiButton* pButton = new nuiButton(_T("pop"));
  pButton->SetObjectName(_T("PopButton"));
  pButton->SetEnabled(mCount != 0);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick);
  
  pButton = new nuiButton(_T("push"));
  pButton->SetObjectName(_T("PushButton"));
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPushClick);
}

MyViewController::~MyViewController()
{

}


void MyViewController::OnPushClick(const nuiEvent& rEvent)
{
  TransitionType transition = (mCount & 1)? eTransitionElastic : eTransitionSlide;
  mCount++;
  
  MyViewController* pView = new MyViewController();
  
  // if you plan to keep your object alive when it's poped from the stack,
  // you can acquire a reference right here. You'll be responsible for releasing it, then, after the pop operation is processed.
  // pView->Acquire();
  // 

  mpNav->PushViewController(pView, true, transition);
}


void MyViewController::OnPopClick(const nuiEvent& rEvent)
{
  TransitionType transition = (mCount & 1)? eTransitionElastic : eTransitionSlide;
  mCount--;
  mpNav->PopViewControllerAnimated(true, transition); 
}






