/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiCSS.h"
#include "nuiHBox.h"
#include "nuiTabView.h"
#include "nuiHyperLink.h"

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
  mpMainBox = new nuiVBox(2);
  mpMainBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(mpMainBox);

  nuiHBox* pBox = new nuiHBox(0);
  pBox->SetBorder(20,20,20,10);
  mpMainBox->SetCell(0, pBox);

  nuiRadioButton* pTopButton = new nuiRadioButton(_T("top"));
  pTopButton->SetGroup(_T("radios"));
  pTopButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pTopButton);
  mEventSink.Connect(pTopButton->Activated, &MainWindow::Reset, (void*)nuiTop);

  nuiRadioButton* pButton = new nuiRadioButton(_T("left"));
  pButton->SetGroup(_T("radios"));
  pButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pButton);
  mEventSink.Connect(pButton->Activated, &MainWindow::Reset, (void*)nuiLeft);

  pButton = new nuiRadioButton(_T("right"));
  pButton->SetGroup(_T("radios"));
  pButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pButton);
  mEventSink.Connect(pButton->Activated, &MainWindow::Reset, (void*)nuiRight);

  pButton = new nuiRadioButton(_T("bottom"));
  pButton->SetGroup(_T("radios"));
  pButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pButton);
  mEventSink.Connect(pButton->Activated, &MainWindow::Reset, (void*)nuiBottom);

  // default
  pTopButton->SetPressed(true);
  pTopButton->Activated();
}



bool MainWindow::Reset(const nuiEvent& rEvent)
{
  uint64 code = (uint64)rEvent.mpUser;
  nuiPosition pos = (nuiPosition)code;
  nuiWidget* pWidget = BuildTabView(pos);
  mpMainBox->SetCell(1, pWidget);
  mpMainBox->SetCellExpand(1, nuiExpandShrinkAndGrow);

  return true;
}


nuiWidget* MainWindow::BuildTabView(nuiPosition pos)
{
  nuiTabView* pTabView = new nuiTabView(pos);
  pTabView->SetChangeOnDrag(true);
  pTabView->SetBorder(20,20,10,20);
  AddChild(pTabView);

  // tab 1
  nuiLabel* pLabel = new nuiLabel(nuiTR("Tab\nOne\nThe\nfirst\nand\nforemost"));
  pLabel->SetObjectName(_T("Tab1Contents"));
  pTabView->AddTab(nuiTR("Tab1"), pLabel);

  // tab 2
  nuiImage* pTitle = new nuiImage();
  pTitle->SetObjectName(_T("Tab2Title"));

  nuiImage* pPage = new nuiImage();
  pPage->SetObjectName(_T("Tab2Contents"));
  pPage->SetPosition(nuiCenter);

  pTabView->AddTab(pTitle, pPage);


  // tab 3
  nuiHyperLink* pLink = new nuiHyperLink(_T("http://libnui.net"), nuiTR("Go to the nui website!"));
  pLink->SetObjectName(_T("HyperLink"));
  pTabView->AddTab(nuiTR("Tab3"), pLink);


  // tab 4
  nuiVBox* pBox = new nuiVBox(0);
  nuiButton* pButton = new nuiButton(nuiTR("Add Tab"));
  pButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pButton);
  mEventSink.Connect(pButton->ButtonPressed, &MainWindow::OnAddTab, pTabView);

  pButton = new nuiButton(nuiTR("Remove Tab"));
  pButton->SetObjectName(_T("TabButton"));
  pBox->AddCell(pButton);
  mEventSink.Connect(pButton->ButtonPressed, &MainWindow::OnRemoveTab, pTabView);

  pTabView->AddTab(nuiTR("Tab4"), pBox);

  pTabView->SelectTab(0);

  return pTabView;
}


bool MainWindow::OnAddTab(const nuiEvent& rEvent)
{
  nuiTabView* pTabView = (nuiTabView*)rEvent.mpUser;
  nglString s;

  s.Format(_T("Tab%d"), pTabView->GetTabCount()+1);

  pTabView->AddTab(s, new nuiLabel(s));
  return false;
}

bool MainWindow::OnRemoveTab(const nuiEvent& rEvent)
{
  nuiTabView* pTabView = (nuiTabView*)rEvent.mpUser;

  if (pTabView->GetTabCount() > 4)
  {
    pTabView->RemoveTab(pTabView->GetTabCount()-1);
    return true;
  }

  return false;
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
