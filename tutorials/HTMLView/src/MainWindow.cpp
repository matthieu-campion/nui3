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
#include "nuiHBox.h"
#include "nuiEditLine.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  
  LoadCSS(_T("rsrc:/css/main.css"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  //nglString url(_T("http://redmine.libnui.net/projects/show/libnui"));
  //nglString url(_T("http://www.kvraudio.com"));
  nglString url(_T("file://rsrc:/Index.html"));
  //nglString url(_T("http://www.google.fr"));
  //nglString url(_T("http://www.w3.org/Consortium/siteindex"));
  //nglString url(_T("http://viewvc.libnui.net/cgi-bin/viewvc.cgi/nui/trunk/nui3/"));
  //nglString url(_T("http://www.google.fr/search?hl=fr&q=libnui&btnG=Recherche+Google&meta=&aq=f&oq="));
  //nglString url(_T("file:///Users/meeloo/Desktop/Tutorial/Index.html"));
  
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  nuiHBox* pHLayoutBox = new nuiHBox(0);
  pHLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  pLayoutBox->AddCell(pHLayoutBox);
  //  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // image in the first box's cell
  nuiImage* pImg = new nuiImage();
  pImg->SetObjectName(_T("MyImage"));
  pImg->SetPosition(nuiCenter);
  pHLayoutBox->AddCell(pImg);
  pHLayoutBox->SetCellExpand(pHLayoutBox->GetNbCells()-1, nuiExpandFixed);
  
  // text entry in the second cell
  mpInput = new nuiEditLine(url);
  mpInput->SetPosition(nuiFillHorizontal);
  mpInput->SetObjectName(_T("RSSURL"));
  mEventSink.Connect(mpInput->Activated, &MainWindow::OnButtonClick);
  pHLayoutBox->AddCell(mpInput);
  pHLayoutBox->SetCellExpand(pHLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // button in the third cell : we use the default decoration for this button, but you could use the css to assign your own decoration
  nuiButton* pButton = new nuiButton();
  pButton->SetPosition(nuiCenter);
  pHLayoutBox->AddCell(pButton);
  pHLayoutBox->SetCellExpand(pHLayoutBox->GetNbCells()-1, nuiExpandFixed);
  
  // click event on button
  mEventSink.Connect(pButton->Activated, &MainWindow::OnButtonClick);
  mEventSink.Connect(mpInput->Activated, &MainWindow::OnButtonClick);
  
  // label in the button
  nuiLabel* pButtonLabel = new nuiLabel(_T("Go"));
  pButtonLabel->SetPosition(nuiCenter);
  pButtonLabel->SetBorder(8,8);
  pButton->AddChild(pButtonLabel);
  
  //nuiScrollView* pScroll = new nuiScrollView(false, true);
  nuiScrollView* pScroll = new nuiScrollView();
  pLayoutBox->AddCell(pScroll);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells() - 1, nuiExpandShrinkAndGrow);
  mpHTMLView = new nuiHTMLView(GetWidth());
  pScroll->AddChild(mpHTMLView);
  
  mSlotSink.Connect(mpHTMLView->URLChanged, nuiMakeDelegate(this, &MainWindow::OnURLChanged));
  mEventSink.Connect(pImg->Clicked, &MainWindow::OnGoHome);
  if (1)
  {
    mpHTMLView->SetURL(url);
  }
  else
  {
    mpHTMLView->SetText(_T("<p>Petit</p><p><i>texte</i> de <b>test</b>.</p><p>&eacute;couter le vent</p><p>Je viens de sauter une ligne</p>"));
  }
}

void MainWindow::OnURLChanged(const nglString& rString)
{
  mpInput->SetText(rString);
}


void MainWindow::OnButtonClick(const nuiEvent& rEvent)
{  
  mpHTMLView->SetURL(mpInput->GetText());
  if (!mpHTMLView->GetURL().IsNull())
    mpInput->SetText(mpHTMLView->GetURL());
  
  // the event is caught and not broadcast
  rEvent.Cancel();
}

void MainWindow::OnGoHome(const nuiEvent& rEvent)
{  
  mpInput->SetText(_T("file://rsrc:/Index.html"));
  OnButtonClick(rEvent);
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
