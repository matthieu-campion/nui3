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


#include "nuiAttributeAnimation.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  // label with decoration in the third cell
  mMyLabel = new nuiLabel(_T("my label is spinning!"));
  mMyLabel->SetFont(nuiFont::GetFont(30));
  mMyLabel->SetPosition(nuiCenter);
  AddChild(mMyLabel);

  mMyLabel->EnableSurface(true);
  
  
  nuiRect r = mMyLabel->CalcIdealSize();
  float s = MAX(r.GetWidth(), r.GetHeight());
  float x = s / 2;
  float y = s / 2;
  mMyLabel->SetUserRect(nuiRect(s, s));
  nuiRotateMatrixAttributeAnimation* pAnim = new nuiRotateMatrixAttributeAnimation();
  pAnim->SetTargetObject(mMyLabel);
  pAnim->SetTargetAttribute(_T("SurfaceMatrix"));
  pAnim->SetStartValue(0, x, y);
  pAnim->SetEndValue(180, x, y);
  pAnim->SetDuration(2);
  pAnim->SetEasing(nuiEasingElasticIn<10>);
  AddAnimation(_T("RotateLabel"), pAnim);
  pAnim->Play(1000, eAnimLoopPingPong);
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
