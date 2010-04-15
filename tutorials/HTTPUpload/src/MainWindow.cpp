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
#include "nuiHTTP.h"
#include "nglOMemory.h"
#include "nuiHTMLView.h"
#include "nuiMimeMultiPart.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  LoadCSS(_T("rsrc:/css/main.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiHTTPRequest request(_T("http://127.0.0.1:8888/"), _T("POST"));
  
  nuiMimeMultiPart mime;
  mime.AddVariable(_T("MyParam"), _T("MyValue"));
  mime.AddFile(_T("rsrc:/css/main.css"), _T("MyFile"));
  mime.Dump(&request);
  
  nuiHTTPResponse* pRes = request.SendRequest();

  nuiHTMLView* pView = new nuiHTMLView();
  AddChild(pView);

  if (pRes)
  {
    NGL_OUT(_T("Result:\n%ls\n"), pRes->GetBodyStr().GetChars());
    pView->SetText(pRes->GetBodyStr());
  }
  else
  {
    NGL_OUT(_T("Unable to send HTTP Request"));
    pView->SetText(_T("Unable to send HTTP Request"));
  }

  delete pRes;
  

}



bool MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
  
  return true; // means the event is caught and not broadcasted
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
