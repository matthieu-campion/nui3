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
#include "nuiSpriteView.h"



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







//**************************************************************************************
//**************************************************************************************




//*************************************************
// MyViewController
//
MyViewController::MyViewController()
: nuiViewController(), mTimer(0.4f), mEventSink(this)
{
  SetObjectClass(_T("MyViewController"));
  SetTitle(_T("Swipe Detector"));
  
  // a simple background icon
  nuiImage* pIcon = new nuiImage();
  pIcon->SetObjectName(_T("SwipeIcon"));
  AddChild(pIcon);

  
  // sprite view to manage the sprites
  nuiSpriteView* pSpriteView = new nuiSpriteView();
  AddChild(pSpriteView);
  
  nuiSpriteDef* pDef = new nuiSpriteDef(nglPath(_T("rsrc:/decorations/arrow")));

  nuiSprite* pSprite = NULL;
  float angle = 0;
  
  // a set of sprite, rotated around the pivot
  for (uint32 i = 0; i < 8; i++)
  {
    pSprite = new nuiSprite(pDef);
    mpArrows.push_back(pSprite);
    pSprite->SetAnimation(_T("off"));
    pSprite->SetPosition(160, 310);
    pSprite->SetPivot(nglVectorf(0.f, -80.f, 0.f));
    pSprite->SetAngle(angle);
    pSpriteView->AddSprite(pSprite);
    
    angle += 360 / 8;
  }
  
  mEventSink.Connect(mTimer.Tick, &MyViewController::OnResetArrows);  
  
}


//*************************************************
// ~MyViewController
//
MyViewController::~MyViewController()
{

}



//*************************************************
// virtual SwipeBegan : the swipe gesture is initiated
//
void MyViewController::SwipeBegan(nuiPosition swipeDirection)
{
  mTimer.Stop();

  uint32 index = 0;
  
  switch (swipeDirection)
  {
    case nuiTop:          index = 0; break;
    case nuiTopRight:     index = 1; break;
    case nuiRight:        index = 2; break;
    case nuiBottomRight:  index = 3; break;
    case nuiBottom:       index = 4; break;
    case nuiBottomLeft:   index = 5; break;
    case nuiLeft:         index = 6; break;
    case nuiTopLeft:      index = 7; break;
  }
  
  mpArrows[index]->SetAnimation(_T("initiated"));
  
  mTimer.Start(false);
}




//*************************************************
// virtual SwipeEnd : the swipe gesture is completed
//
void MyViewController::SwipeEnd(nuiPosition swipeDirection)
{
  mTimer.Stop();

  uint32 index = 0;
  
  switch (swipeDirection)
  {
    case nuiTop:          index = 0; break;
    case nuiTopRight:     index = 1; break;
    case nuiRight:        index = 2; break;
    case nuiBottomRight:  index = 3; break;
    case nuiBottom:       index = 4; break;
    case nuiBottomLeft:   index = 5; break;
    case nuiLeft:         index = 6; break;
    case nuiTopLeft:      index = 7; break;
  }
  
  mpArrows[index]->SetAnimation(_T("activated"));
  mTimer.Start(false);
}



//*************************************************
// OnResetArrows
//
void MyViewController::OnResetArrows(const nuiEvent& rEvent)
{
  mTimer.Stop();
  
  for (uint32 index = 0; index < 8; index++)
    mpArrows[index]->SetAnimation(_T("off"));
}
