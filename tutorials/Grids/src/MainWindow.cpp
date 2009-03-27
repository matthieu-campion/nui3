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

  // create a window manager to create several sub-windows
  mpManager = new nuiWindowManager();
  pScrollView->AddChild(mpManager);

  // create a window for each test
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
  
  // create a window and put a grid in it
  nuiWindow* pWin = new nuiWindow(nuiRect(10, 10, 200, 200), nuiWindow::DecoratedBackground, _T("Grid..."));
  mpManager->AddChild(pWin);
  nuiGrid* pGrid = new nuiGrid(rows, cols);
  pWin->AddChild(pGrid);
  
  // simply store text information in the grid
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

  // create a window and put a scrollview in it
  nuiWindow* pWin = new nuiWindow(nuiRect(100,100, 200, 200), nuiWindow::DecoratedBackground, _T("Scrolled Grid..."));
  mpManager->AddChild(pWin);
  nuiScrollView* pScrollView = new nuiScrollView();
  pWin->AddChild(pScrollView);
  
  // add a grid to the scrollview
  nuiGrid* pGrid = new nuiGrid(rows, cols);
  pScrollView->AddChild(pGrid);
  
  // simply store text information in the grid
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
  // create a window and put a grid in it
  nuiWindow* pWin = new nuiWindow(nuiRect(200,200, 200, 200), nuiWindow::DecoratedBackground, _T("Resizeable Grid..."));
  mpManager->AddChild(pWin);
  nuiGrid* pGrid = new nuiGrid(10, 10);
  pWin->AddChild(pGrid);
  
  for (uint i = 0; i < 10; i++)
  {
    pGrid->SetRowExpand(i, nuiExpandGrow, 1.1f);  // the row will grow with the window
    pGrid->SetRowMaxGrow(i, 3.5f);                // put limits to the row's growing
    pGrid->SetRowMaxShrink(i, 0.5f);
    pGrid->SetColumnMaxShrink(i, 0.25f);          // put limits to the column's growing
    pGrid->SetColumnMaxGrow(i, 5.0f);
    
    // store text information in the grid
    for (uint j = 0; j < 10; j++)
    {
      nglString text;
      text.Format(_T("[%d,%d]"), i, j);
      pGrid->SetCell(i, j , new nuiLabel(text));
    }
  }
  
  pGrid->SetColumnExpand(2, nuiExpandGrow, 1.0);          // the column #2 will grow with the window
  pGrid->SetColumnExpand(4, nuiExpandShrink, 1.0f);       // the column #4 will shrink with the window (<=> won't be clipped)
  pGrid->SetColumnExpand(6, nuiExpandShrinkAndGrow, 1.0f);// the column #6 will shrink and grow with the window
}