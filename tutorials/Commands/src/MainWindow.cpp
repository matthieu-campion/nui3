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
#include "nuiBackgroundPane.h"
#include "nuiGrid.h"
#include "nuiCommandManager.h"

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
  nuiGrid* pGrid = new nuiGrid(2,2);
  pGrid->SetBorder(40,40);
  pGrid->SetRowExpand(1, nuiExpandShrinkAndGrow);
  pGrid->SetColumnExpand(0, nuiExpandShrinkAndGrow);
  pGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
  AddChild(pGrid);
  
  nuiLabel* pLabel = new nuiLabel(_T("Console"));
  pLabel->SetPosition(nuiCenter);
  pLabel->SetBorder(10,10);
  pGrid->SetCell(0,0, pLabel);
  
  pLabel = new nuiLabel(_T("CommandManager"));
  pLabel->SetPosition(nuiCenter);
  pLabel->SetBorder(10,10);
  pGrid->SetCell(1,0, pLabel);

  
  //****************************************************************
  // console
  //
  nuiBackgroundPane* pPane = new nuiBackgroundPane(eInnerBackground);
  pPane->SetPosition(nuiFill);
  pPane->SetBorder(25,25);
  pGrid->SetCell(0,1, pPane);
  
  nuiScrollView* pScroll = new nuiScrollView(true, true);
  pPane->AddChild(pScroll);
  
  mpConsole = new nuiLabel(_T("Welcome to the 'Commands' tutorial.\nYou can play with the menus.\n"));
  mpConsole->SetPosition(nuiTopLeft);
  pScroll->AddChild(mpConsole);

  
  
  //****************************************************************
  // command manager dump
  //

  pPane = new nuiBackgroundPane(eInnerBackground);
  pPane->SetPosition(nuiFill);
  pPane->SetBorder(25,25);
  pGrid->SetCell(1,1, pPane);

  pScroll = new nuiScrollView(true, true);
  pPane->AddChild(pScroll);
  
  mpDump = new nuiLabel(nuiCommandManager::Dump(), nuiFont::GetFont(8)); // we could use SetObjectName and the css to assign a font, but hell... just for once... :)
  mpDump->SetPosition(nuiTopLeft);
  pScroll->AddChild(mpDump);
  
}


void MainWindow::AddMessage(const nglChar* pFormat, ...)
{
  va_list args;
  nglString buf;
  
  va_start(args, pFormat);
  buf.Formatv(pFormat, args);
  va_end (args);

  mpConsole->SetText(mpConsole->GetText() + nglString(_T("\n\n")) + buf);
  NGL_OUT(_T("%ls\n"), buf.GetChars());
}

void MainWindow::UpdateCommandManagerInfo()
{
  mpDump->SetText(nuiCommandManager::Dump());
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
