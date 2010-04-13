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
#include "nuiBindings.h"

#include "nuiSpiderMonkey.h"

/*
 * MainWindow
 */

void TestVariant();
void TestBinding();


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

void MainWindow::JSTest()
{
  nuiSpiderMonkey* pM = new nuiSpiderMonkey();
  pM->SetGlobal(_T("window"), (nuiSimpleContainer*)this);
  
  const char* script =
  "function OnClicked(x, y, b) { this.Text = 'OnClicked called!(' + x + ',' + y + ',' + b + ')'; this.TextColor = 'green'; }"
  "function OnUnclicked(x, y, b) { this.Text = 'OnUnclicked called!(' + x + ',' + y + ',' + b + ')'; this.TextColor = 'blue'; }"
  "function OnMoved(x, y) { this.Text = 'OnMoved called!(' + x + ',' + y + ')'; this.TextColor = 'red'; }"
  "NGL_OUT('nuiRight:' + nuiRight);\n"
  "var label = new nuiLabel('FromJS');\n"
  "NGL_OUT('label class:'+label.GetObjectClass());\n"
  "window.AddChild(label);\n"
  "NGL_OUT(window.GetObjectClass());\n"
  "var test = label.test;\n"
  "label.test = 'testval';\n"
  "var res = label.Text;\n"
  "NGL_OUT(res);\n"
  "label.TextColor = 'red';\n"
  "label.Position = nuiRight;\n"
  "label.Clicked = OnClicked;"
  "label.Unclicked = OnUnclicked;"
  "label.MovedMouse = OnMoved;"
  ;
  
  nglString program(script);
  pM->ExecuteExpression(program);
}

void MainWindow::OnCreation()
{
  TestVariant();
  nuiInitBindings();
  TestBinding();

  Acquire();
  JSTest();
  
  

#if 0
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // image in the first box's cell
  nuiImage* pImg = new nuiImage();
  pImg->SetObjectName(_T("MyImage"));
  pImg->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pImg);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // button in the second cell : we use the default decoration for this button, but you could use the css to assign your own decoration
  nuiButton* pButton = new nuiButton();
  pButton->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pButton);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // click event on button
  mEventSink.Connect(pButton->Activated, &MainWindow::OnButtonClick);
  
  // label with border in the button (put the label string in the button's constructor if you don't need borders)
  nuiLabel* pButtonLabel = new nuiLabel(_T("click!"));
  pButtonLabel->SetPosition(nuiCenter);
  pButtonLabel->SetBorder(8,8);
  pButton->AddChild(pButtonLabel);

  // label with decoration in the third cell
  mMyLabel = new nuiLabel(_T("my label"));
  mMyLabel->SetObjectName(_T("MyLabel"));
  mMyLabel->SetPosition(nuiCenter);
  pLayoutBox->AddCell(mMyLabel);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
#endif
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
