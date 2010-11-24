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
  
  mpSpriteView = NULL;
  
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
  SetColor(eActiveWindowBg, nuiColor("black"));
  mpSpriteView = new nuiSpriteView();
  mpSpriteView->EnableRenderCache(false);
  AddChild(mpSpriteView);
  
  // Define the sprite:
  mpSpriteDef = new nuiSpriteDef(nglPath("rsrc:/Gizmo"));
  
  // Number of sprites to acheive about 60FPS:
  // iPod 2G   : 50
  // iPhone 3GS:
  // iPhone 4  : 120
  for (int32 i = 0; i < 120; i++)
  {
    nuiSprite* pSprite = new nuiSprite(_T("Gizmo"));
    int32 x, y;
    x = rand() % GetWidth();
    y = rand() % GetHeight();
    float angle = rand() % 360;
    float speed = (float)(rand() % 20) / 10.f;
    pSprite->SetPosition(x, y);
    pSprite->SetAngle(angle);
    pSprite->SetSpeed(speed);
    mpSpriteView->AddSprite(pSprite);
  }

}



void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}

bool MainWindow::MouseMoved(const nglMouseInfo& rInfo)
{
  std::vector<nuiSprite*> sprites;
  mpSpriteView->GetSpritesAtPoint(rInfo.X, rInfo.Y, sprites);
  
  for (uint32 i = 0; i < mpSpriteView->GetSpriteCount(); i++)
  {
    nuiSprite* pSprite = mpSpriteView->GetSprites()[i];
    pSprite->SetScale(1.0f);
  }
  
  for (uint32 i = 0; i < sprites.size(); i++)
  {
    nuiSprite* pSprite = sprites[i];
    pSprite->SetScale(2.0f);
  }
  
  return true;
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
