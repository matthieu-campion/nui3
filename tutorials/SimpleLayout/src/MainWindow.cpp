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


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent))
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  nuiSplitter* pSplit = new nuiSplitter(nuiVertical);
  
  nuiTreeNode* pTree = new nuiTreeNode(_T("Root"));
  for (uint32 i = 0; i < 10; i++)
    pTree->AddChild(new nuiTreeNode(nglString().Add(_T("Item ")).Add(i)));
  pTree->Open(true); // show the tree children (every node is closed by default)
  nuiTreeView* pTreeView = new nuiTreeView(pTree);
  pSplit->AddChild(pTreeView);
  
  nuiVBox* pBox = new nuiVBox();
  pBox->AddCell(new nuiImage(nglPath(_T("rsrc:/image.png"))));
  pBox->AddCell(new nuiLabel(_T("This is some text displayed as a label")));
  pSplit->AddChild(pBox);
  pBox->SetPosition(nuiCenter);
  pBox->SetCellPosition(0, nuiCenter);
  pBox->SetExpand(nuiExpandShrinkAndGrow); // Allow the box to adapt its size to its parent
  AddChild(pSplit);
}

MainWindow::~MainWindow()
{
}

