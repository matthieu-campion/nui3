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
  
  mRunning = false;
      
  nglString name;
  name.Format(_T("MyViewController%d"), mCount % 3);
  SetObjectClass(name);
  
  name.Format(_T("%d"), mCount);
  nuiLabel* pLabel = new nuiLabel(name);
  pLabel->SetObjectName(_T("ViewControllerName"));
  AddChild(pLabel);
  
  // pop buttons
  nuiButton* pButton = new nuiButton(_T("pop [slide]"));
  pButton->SetObjectName(_T("PopButton0"));
  pButton->SetEnabled(mCount != 0);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick, (void*)0);
  
  pButton = new nuiButton(_T("pop [elastic]"));
  pButton->SetObjectName(_T("PopButton1"));
  pButton->SetEnabled(mCount != 0);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick, (void*)1);

  pButton = new nuiButton(_T("pop [transp.]"));
  pButton->SetObjectName(_T("PopButton2"));
  pButton->SetEnabled(mCount != 0);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick, (void*)2);
  
  name.Format(_T("pop to [%d]"), mCount-5);
  pButton = new nuiButton(name);
  pButton->SetObjectName(_T("PopButton3"));
  pButton->SetEnabled(mCount >= 5);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick, (void*)3);

  pButton = new nuiButton(_T("pop to root"));
  pButton->SetObjectName(_T("PopButton4"));
  pButton->SetEnabled(mCount != 0);
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPopClick, (void*)4);
  
  

  // push buttons
  pButton = new nuiButton(_T("push [slide]"));
  pButton->SetObjectName(_T("PushButton0"));
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPushClick, (void*)0);

  pButton = new nuiButton(_T("push [elastic]"));
  pButton->SetObjectName(_T("PushButton1"));
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPushClick, (void*)1);

  pButton = new nuiButton(_T("push [transp.]"));
  pButton->SetObjectName(_T("PushButton2"));
  AddChild(pButton);
  mEventSink.Connect(pButton->Activated, &MyViewController::OnPushClick, (void*)2);
}

MyViewController::~MyViewController()
{

}

// virtual 
void MyViewController::ViewWillAppear()
{
  
}

//virtual 
void MyViewController::ViewWillDisappear()
{
  mRunning = false;
}

// virtual 
void MyViewController::ViewDidAppear()
{
  mRunning = true;
}

//virtual 
void MyViewController::ViewDidDisappear()
{
  
}



void MyViewController::OnPushClick(const nuiEvent& rEvent)
{
  if (!mRunning)
    return;
  
  uint32 code = (uint32)rEvent.mpUser;
  TransitionType transition = (code == 0)? eTransitionSlide : (code == 1)? eTransitionElastic : eTransitionTransparency;
  mCount++;
  
  MyViewController* pView = new MyViewController();
  
  mpNav->PushViewController(pView, true, transition);
}


void MyViewController::OnPopClick(const nuiEvent& rEvent)
{
  if (!mRunning)
    return;
  
  uint32 code = (uint32)rEvent.mpUser;  
  
  if (code == 3)
  {
    mCount-=5;
    mpNav->PopToViewController(mpNav->GetViewControllers()[mCount]);
  }
  else if (code == 4)
  {
    mCount = 0;
    mpNav->PopToRootViewControllerAnimated();
  }
  else
  {
    mCount--;
    TransitionType transition = (code == 0)? eTransitionSlide : (code == 1)? eTransitionElastic : eTransitionTransparency;
    mpNav->PopViewControllerAnimated(true, transition); 
  }
}






