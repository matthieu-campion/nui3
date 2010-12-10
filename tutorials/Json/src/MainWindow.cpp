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
#include "nuiDialogSelectFile.h"
#include "nuiMessageBox.h"

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
  nuiButton* pLoadBtn = new nuiButton(_T("load"));
  pLoadBtn->SetObjectName(_T("LoadButton"));
  AddChild(pLoadBtn);
  mEventSink.Connect(pLoadBtn->Activated, &MainWindow::OnLoad);
  
  nuiButton* pSaveBtn = new nuiButton(_T("save"));
  pSaveBtn->SetObjectName(_T("SaveButton"));
  AddChild(pSaveBtn);
  mEventSink.Connect(pSaveBtn->Activated, &MainWindow::OnSave);
  
  
  
  mRoot["attribInt"] = 5;
  mRoot["attribDouble"] = 2.34;
  mRoot["attribString"] = "mystring";
  mRoot["attribBool"] = true;
  
  int idx = 0;
  mRoot["attribArray"][idx] = 10;
  mRoot["attribArray"][1] = 20;
  mRoot["attribArray"][2] = 30;
  mRoot["attribArray"][3] = 40;
  mRoot["attribArray"][4] = "fifty";
  mRoot["attribArray"][5] = false;
  
  
  mRoot["attribObject"]["attr0"] = 123;
  mRoot["attribObject"]["attr1"] = 7.89;
  mRoot["attribObject"]["attr2"] = "a string";
  
  mpRootView = new ObjectView(mRoot);
  mpRootView->SetObjectName(_T("RootView"));
  AddChild(mpRootView);
}

void MainWindow::OnLoad(const nuiEvent& rEvent)
{
  nuiDialogSelectFile* pDialog = new nuiDialogSelectFile((nuiMainWindow*)GetMainWindow(), _T("save to json"), nglPath(nglString::Null), nglPath(nglString::Null), nglString::Null, _T("*"));
  pDialog->DoModal();
  nglPath path = pDialog->GetSelectedFile();
  
  if (!path.Exists())
    return;
  
  if (!path.IsLeaf())
    return;
  
  nglIStream* pStream = path.OpenRead();
  if (!pStream)
    return;
  
  nglString text;
  pStream->ReadText(text);
  
  nuiJson::Value root;
  nuiJson::Reader reader;
  bool parsed = reader.parse(text.GetStdString(), root);
  if (!parsed)
    return;
  
  mpRootView->Trash();
  mRoot = root;
  
  mpRootView = new ObjectView(mRoot);
  mpRootView->SetObjectName(_T("RootView"));
  AddChild(mpRootView);
}

void MainWindow::OnSave(const nuiEvent& rEvent)
{
  nuiDialogSelectFile* pDialog = new nuiDialogSelectFile((nuiMainWindow*)GetMainWindow(), _T("save to json"), nglPath(nglString::Null), nglPath(nglString::Null), nglString::Null, _T("*"));
  pDialog->DoModal();
  nglPath path = pDialog->GetSelectedFile();
  
  if (path.Exists())
  {
    if (!path.IsLeaf())
      return;
    
    nglString msg;
    msg.Add(_T("the file: ")).Add(path.GetPathName()).Add(_T(" already exists. Do you really want to overwrite it?"));
    nuiMessageBox::Button btn = nuiMessageBox::Do((nuiContainer*)GetMainWindow(), _T("file exists"), msg, eMB_OKCancel);
    if (btn == nuiMessageBox::ButtonCancel)
      return;
  }
  
  nglIOStream* pStream = path.OpenWrite(true/*overwrite*/);
  if (!pStream)
    return;
  
  nuiJson::FastWriter writer;
  nglString text = writer.write(mRoot);
  pStream->WriteText(text);
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
