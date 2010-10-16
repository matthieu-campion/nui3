/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiLabel.h"
#include "nuiMatrixNode.h"
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
  // a vertical box for page layout
  nuiFixed* pFixed = new nuiFixed();
  AddChild(pFixed);
  nuiLabel* pLabel = new nuiLabel(_T("Label"));
  pLabel->SetUserPos(320, 200);
  pLabel->SetAutoClip(false);
  nuiMatrixNode* pTranslation = new nuiMatrixNode_Translation(-20,-10,0);
  nuiMatrixNode* pRotation = new nuiMatrixNode_Rotation(0,0,0,1);
  nuiMatrixNode* pScale = new nuiMatrixNode_Scale(1.0,1.0,1.0);

  pLabel->AddMatrixNode(pScale);
  pLabel->AddMatrixNode(pRotation);
  pLabel->AddMatrixNode(pTranslation);
  
  nuiAttributeAnim<float>* pAnimRot = new nuiAttributeAnim<float>();
  pAnimRot->SetStartValue(0);
  pAnimRot->SetEndValue(360);
  pAnimRot->SetTargetObject(pRotation);
  pAnimRot->SetTargetAttribute(_T("Angle"));
  pAnimRot->SetDeleteOnStop(true);
  pAnimRot->SetDuration(3);
  //pAnimRot->SetEasing(nuiEasingElasticOut<1400>);
  pAnimRot->SetEasing(nuiEasingQuintic);
  pAnimRot->Play(1000000, eAnimLoopForward);

  nuiAttributeAnim<float>* pAnimScale = new nuiAttributeAnim<float>();
  pAnimScale->SetStartValue(0.5);
  pAnimScale->SetEndValue(1);
  pAnimScale->SetTargetObject(pScale);
  pAnimScale->SetTargetAttribute(_T("Scale"));
  pAnimScale->SetDeleteOnStop(true);
  pAnimScale->SetDuration(3);
  pAnimScale->SetEasing(nuiEasingElasticOut<1400>);
  pAnimScale->Play(1000000, eAnimLoopForward);
  
  pFixed->AddChild(pLabel);
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
