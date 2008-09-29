/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "MainWindow.h"
#include "Application.h"
#include "nuiLabel.h"
#include "nuiGrid.h"

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiScrollView* pScrollView = new nuiScrollView();
  AddChild(pScrollView);
  
  mpManager = new nuiWindowManager();
  pScrollView->AddChild(mpManager);

  CreateGridWindow();
  CreateScrollGridWindow();
  CreateResizeGridWindow();
}

void MainWindow::OnClose()
{
  if (GetNGLWindow()->IsInModalState())
    return;
  
  
  App->Quit();
}











void MainWindow::CreateGridWindow()
{
  uint32 rows = 10, cols = 10;
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 200, 200), nuiWindow::DecoratedBackground, _T("Grid..."));
  mpManager->AddChild(pWin);
  nuiGrid* pGrid = new nuiGrid(rows, cols);
  pWin->AddChild(pGrid);
  
  for (uint i = 0; i < rows; i++)
  {
    for (uint j = 0; j < cols; j++)
    {
      nglString text;
      text.Format(_T("[%d,%d]"), i, j);
      pGrid->SetCell(i, j , new nuiLabel(text));
    }
  }
  
}

void MainWindow::CreateScrollGridWindow()
{
  uint32 rows = 15, cols = 15;
  nuiWindow* pWin = new nuiWindow(nuiRect(100,100, 200, 200), nuiWindow::DecoratedBackground, _T("Scrolled Grid..."));
  mpManager->AddChild(pWin);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWin->AddChild(pScrollView);
  
  nuiGrid* pGrid = new nuiGrid(rows, cols);
  pScrollView->AddChild(pGrid);
  
  for (uint i = 0; i < rows; i++)
  {
    for (uint j = 0; j < cols; j++)
    {
      nglString text;
      text.Format(_T("[%d,%d]"), i, j);
      pGrid->SetCell(i, j , new nuiLabel(text));
    }
  }
  
}

void MainWindow::CreateResizeGridWindow()
{
  nuiWindow* pWin = new nuiWindow(nuiRect(200,200, 200, 200), nuiWindow::DecoratedBackground, _T("Resizeable Grid..."));
  mpManager->AddChild(pWin);
  nuiGrid* pGrid = new nuiGrid(10, 10);
  pWin->AddChild(pGrid);
  
  for (uint i = 0; i < 10; i++)
  {
    pGrid->SetRowExpand(i, nuiExpandGrow, 1.1f);
    pGrid->SetRowMaxGrow(i, 3.5f);
    pGrid->SetRowMaxShrink(i, 0.5f);
    pGrid->SetColumnMaxShrink(i, 0.25f);
    pGrid->SetColumnMaxGrow(i, 5.0f);
    for (uint j = 0; j < 10; j++)
    {
      nglString text;
      text.Format(_T("[%d,%d]"), i, j);
      pGrid->SetCell(i, j , new nuiLabel(text));
    }
  }
  
  pGrid->SetColumnExpand(2, nuiExpandGrow, 1.0);
  pGrid->SetColumnExpand(4, nuiExpandShrink, 1.0f);
  pGrid->SetColumnExpand(6, nuiExpandShrinkAndGrow, 1.0f);
}