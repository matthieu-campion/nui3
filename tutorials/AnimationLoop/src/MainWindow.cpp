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
#include "nuiComboBox.h"
#include "nuiAttributeAnimation.h"

#include "nuiAudioDevice.h"

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
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  mProgress = 0;
  
  AddAttribute(new nuiAttribute<float>
               (nglString(_T("AnimProgress")), nuiUnitNone,
                nuiMakeDelegate(this, &MainWindow::GetProgress), 
                nuiMakeDelegate(this, &MainWindow::SetProgress)));
  
  
  nuiSimpleContainer* pContainer = (nuiSimpleContainer*)nuiBuilder::Get().CreateWidget(_T("MainContainer"));
  AddChild(pContainer);
  
  mpCountSlider = (nuiSlider*)pContainer->GetChild(_T("countSlider"));
  nuiRange& rRange = mpCountSlider->GetRange();
  rRange.SetValue(1);
  rRange.SetRange(1, 10);
  rRange.SetIncrement(1);
  rRange.SetPageIncrement(2);
  mpCountValLabel = (nuiLabel*)pContainer->GetChild(_T("countValueLabel"));
  mEventSink.Connect(mpCountSlider->InteractiveValueChanged, &MainWindow::OnCountSliderChanged);
  OnCountSliderChanged(NULL);
  
  
  mpLoopCombo = (nuiComboBox*)pContainer->GetChild(_T("loopCombo"));
  
  nuiTreeNode* pTree = new nuiTreeNode(_T("loop"));
  nuiTreeNode* pNode = new nuiTreeNode(_T("forward"));
  pTree->AddChild(pNode);
  pNode = new nuiTreeNode(_T("reverse"));
  pTree->AddChild(pNode);
  pNode = new nuiTreeNode(_T("ping pong"));
  pTree->AddChild(pNode);
  
  mpLoopCombo->SetTree(pTree);
  mpLoopCombo->SetSelected((uint32)0);
  
  mpGoButton = (nuiButton*)pContainer->GetChild(_T("goButton"));
  mEventSink.Connect(mpGoButton->Activated, &MainWindow::OnGoButtonClick);
  
  mEventSink.Connect(nuiAnimation::GetTimer()->Tick, &MainWindow::OnTimerTick);
}



void MainWindow::OnCountSliderChanged(const nuiEvent& rEvent)
{
  uint32 count = mpCountSlider->GetRange().GetValue();
  nglString str;
  str.Add(count);
  mpCountValLabel->SetText(str);
}



void MainWindow::OnGoButtonClick(const nuiEvent& rEvent)
{
  uint32 count = mpCountSlider->GetRange().GetValue();
  
  nuiAnimLoop loop = eAnimLoopForward;
  uint32 selected = mpLoopCombo->GetValue();
  
  if (selected == 0)
    loop = eAnimLoopForward;
  else if (selected == 1)
    loop = eAnimLoopReverse;
  else if (selected == 2)
    loop = eAnimLoopPingPong;
  
  
  nuiAttributeAnimation* pAnim = new nuiAttributeAnimation();
  pAnim->SetTargetObject(this);
  pAnim->SetTargetAttribute(_T("AnimProgress"));
  pAnim->SetStartValue(0);
  pAnim->SetEndValue(1);
  pAnim->SetEasing(nuiEasingCubic);
  pAnim->SetDuration(1);
  pAnim->SetDeleteOnStop(true);
  mEventSink.Connect(pAnim->AnimStop, &MainWindow::OnAnimStop);
  
  pAnim->Play(count, loop);
}

void MainWindow::OnAnimStop(const nuiEvent& rEvent)
{
  mProgress = 0;
}

void MainWindow::OnTimerTick(const nuiEvent& rEvent)
{
  nuiSize w = 80;
  nuiSize h = 60;
  w += 3 * w * mProgress;
  h += 3 * h * mProgress;
  mpGoButton->SetUserSize(w, h);
}

float MainWindow::GetProgress()
{
  return mProgress;
}

void MainWindow::SetProgress(float progress)
{
  mProgress = progress;
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
