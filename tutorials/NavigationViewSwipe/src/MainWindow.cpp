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
#include "nuiGestureRecognizer.h"
#include "nuiComboBox.h"


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
  
  // swipe gesture recognizer
  mpSwipeRecognizer = new nuiSwipeGestureRecognizer(nuiGestureDirectionRight);
  mSink.Connect(mpSwipeRecognizer->SignalSwipe, nuiMakeDelegate(this, &MyViewController::OnSignalSwipe));
  mSink.Connect(mpSwipeRecognizer->SignalStateChanged, nuiMakeDelegate(this, &MyViewController::OnSignalRecognizerStateChanged));
  AddChild(mpSwipeRecognizer);

  // a combo box to setup the gesture recognizer
  nuiTreeNode* pTree = new nuiTreeNode(_T("Swipe"), true);
  nuiTreeNode* pNode = new nuiTreeNode(_T("right"), true);
  pTree->AddChild(pNode);
  pTree->AddChild(new nuiTreeNode(_T("right/left")));
  pTree->AddChild(new nuiTreeNode(_T("top/bottom")));
  pTree->AddChild(new nuiTreeNode(_T("diagonals")));
  pTree->AddChild(new nuiTreeNode(_T("all")));
  nuiComboBox* pCombo = new nuiComboBox(pTree);
  pCombo->SetSelected(pNode);
  mEventSink.Connect(pCombo->ValueChanged, &MyViewController::OnComboChanged, (void*)pCombo);
  AddChild(pCombo);
  
  
  // monitor reset
  mEventSink.Connect(mTimer.Tick, &MyViewController::OnResetArrows);  
  
}


//*************************************************
// ~MyViewController
//
MyViewController::~MyViewController()
{

}


void MyViewController::OnComboChanged(const nuiEvent& rEvent)
{
  nuiComboBox* pCombo = (nuiComboBox*)rEvent.mpUser;
  uint32 value = pCombo->GetValue();
  
  nuiGestureDirection direction;
  
  switch (value)
  {
    case 0: direction = nuiGestureDirectionRight; break;
    case 1: direction = nuiGestureDirectionHorizontal; break;
    case 2: direction = nuiGestureDirectionVertical; break;
    case 3: direction = nuiGestureDirectionDiagonals; break;
    case 4: direction = nuiGestureDirectionHorizontal | nuiGestureDirectionVertical | nuiGestureDirectionDiagonals; break;
  }
  
  mpSwipeRecognizer->SetDirections(direction);
}




void MyViewController::OnSignalSwipe(nuiGestureDirection direction)
{
  mTimer.Stop();
  
  uint32 index = 0;
  switch (direction)
  {
    case nuiGestureDirectionUp : index = 0; break;
    case nuiGestureDirectionUpRight : index = 1; break;
    case nuiGestureDirectionRight : index = 2; break;
    case nuiGestureDirectionDownRight : index = 3; break;
    case nuiGestureDirectionDown : index = 4; break;
    case nuiGestureDirectionDownLeft: index = 5; break;
    case nuiGestureDirectionLeft: index = 6; break;
    case nuiGestureDirectionUpLeft: index = 7; break;
  }
  
  mpArrows[index]->SetAnimation(_T("activated"));
  mTimer.Start(false);
}


void MyViewController::OnSignalRecognizerStateChanged(nuiGestureRecognizerState state)
{
  
  nglString str;
  switch (state) 
  {
    case eGestureRecognizerStatePossible: str = _T("StatePossible"); break;
    case eGestureRecognizerStateBegan: str = _T("StateBegan"); break;
    case eGestureRecognizerStateEnded: str = _T("StateEnd"); break;
    case eGestureRecognizerStateFailed: str = _T("StateFailed"); break;
    default: str = _T("unknown");
  }
  
  NGL_OUT(_T("OnSwipeStateChanged %ls\n"), str.GetChars());
  
  if (state != eGestureRecognizerStateBegan)
    return;

  mTimer.Stop();

  uint32 index = 0;
  switch (mpSwipeRecognizer->GetRecognizedDirection())
  {
    case nuiGestureDirectionUp : index = 0; break;
    case nuiGestureDirectionUpRight : index = 1; break;
    case nuiGestureDirectionRight : index = 2; break;
    case nuiGestureDirectionDownRight : index = 3; break;
    case nuiGestureDirectionDown : index = 4; break;
    case nuiGestureDirectionDownLeft: index = 5; break;
    case nuiGestureDirectionLeft: index = 6; break;
    case nuiGestureDirectionUpLeft: index = 7; break;
  }
  
  mpArrows[index]->SetAnimation(_T("initiated"));
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
