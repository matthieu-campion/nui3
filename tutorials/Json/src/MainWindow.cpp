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


#include "nuiAudioDevice.h"

#include "ObjectView.h"

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
//  mRoot["attribInt"] = 5;
  mRoot["attribDouble"] = 2.34;
//  mRoot["attribString"] = "mystring";
//  mRoot["attribBool"] = true;
//  mRoot["attribObject"]["attr0"] = 123;
//  mRoot["attribObject"]["attr1"] = 7.89;
//  mRoot["attribObject"]["attr2"] = "a string";
  
  ObjectView* pView = new ObjectView(mRoot);
  AddChild(pView);
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
