/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "DropDestination_Container.h"
#include "DragSource_FileTree.h"
#include "nuiCSS.h"
#include "nuiVBox.h"


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
  // a box for page layout
  nuiHBox* pLayoutBox = new nuiHBox(0);
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // drag source file tree in left column
  DragFileTree* pTree = new DragFileTree();
  pLayoutBox->AddCell(pTree);
  pLayoutBox->SetCellPixels(pLayoutBox->GetNbCells()-1, 250);
  
  //
  // code writing note:
  //
  // again, instead of writing:
  // myBox = new nuiHBox(0);
  // myBox->AddCell(myWidget);
  // myBox->SetCellExpand(myBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  //
  // you can simply goes for:
  // myBox = new nuiHBox(2);
  // myBox->SetCell(0, myWidget);
  // myBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  //
  // but I like the first version because it's completely dynamic. 
  // You don't have to modify anything in the code you have already wrote if you choose to add/remove some cells 
  // or change the positions of the widgets in the box.
  // It's a simple choice of code covenience...
  //

  //
  // another code writing note:
  //
  // in this application, instead of using a box to create the UI layout,
  // you could use a nuiSplitter. This way, the user can strech the column size manually.
  //
  
  
  // drop destination container in the right column
  DropContainer* pCont = new DropContainer();
  pLayoutBox->AddCell(pCont);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
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
