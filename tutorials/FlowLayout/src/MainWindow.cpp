/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiFlowView.h"


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
  nuiScrollView* pScroll = new nuiScrollView(false, true);
  AddChild(pScroll);
  nuiFlowView* pFlow = new nuiFlowView(640);
  pScroll->AddChild(pFlow);

  pFlow->AddChild(new nuiImage(_T("rsrc:/decorations/image.png")));
  
  nglIStream* pStream = nglPath(_T("rsrc:/sample.txt")).OpenRead();
  nglTextFormat format = eTextNone;
  nglString text;
  int64 res = pStream->ReadText(text, &format);
  delete pStream;
  
  std::vector<nglString> tokens;
  text.Tokenize(tokens);
  
  for (int32 i = 0; i < tokens.size(); i++)
  {
    nuiLabel* pLabel = new nuiLabel(tokens[i]);
    pLabel->SetPosition(nuiBottom);
    pFlow->AddChild(pLabel);
  }
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
