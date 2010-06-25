/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "MainWindow.h"

#include "nuiLabel.h"
#include "nuiVBox.h"
#include "nuiCSS.h"


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
  : nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(_T("rsrc:/"))), mSink(this)
{

  SetDebugMode(true);

  if (rContextInfo.TargetAPI == eTargetAPI_OpenGL)
  {
    NGL_OUT(_T("glExtensions: %s\n"), glGetString(GL_EXTENSIONS));
  }
}

MainWindow::~MainWindow()
{
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
  bool res = pCSS->Load(*pF, rPath/* rPath parameter is needed to handle recursive '#include' directives*/);
  if (res)
  {
    SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}




void MainWindow::OnCreation()
{
  Test01();
}




void MainWindow::Test01()
{
  // Load the CSS:
  LoadCSS(_T("rsrc:/test.css"));

  nuiVBox* pBox = new nuiVBox();
  pBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pBox);
  
  // Add Some buttons
  {
    for (uint i = 0; i < 4; i++)
    {
      nuiButton* pButton = new nuiButton(_T("HELLO WORLD!"));
      nglString str;
      str.Add(_T("btn")).Add(i);
      pButton->SetObjectName(str);
      pBox->AddCell(pButton);
      pBox->SetCellExpand(pBox->GetNbCells() - 1, nuiExpandShrinkAndGrow);
    }
  }

  // Add Some labels
  {
    for (uint i = 0; i < 4; i++)
    {
      nuiLabel* pLabel = new nuiLabel(_T("HELLO WORLD!"));
      nglString str;
      str.Add(_T("hello")).Add(i);
      pLabel->SetObjectName(str);
      pBox->AddCell(pLabel);
      pBox->SetCellExpand(pBox->GetNbCells() - 1, nuiExpandShrinkAndGrow);
    }
  }

  {
    nuiVBox* pB = new nuiVBox();
    nuiLabel* pLabel = new nuiLabel(_T("Main box label"));
    pB->AddCell(pLabel);
    pB->SetObjectName(_T("RecursiveBox"));
    pBox->AddCell(pB);
    
    for (uint i = 0; i < 3; i++)
    {
      nuiVBox* pB2 = new nuiVBox();
      pB->AddCell(pB2);
      
      pB = pB2;

      nglString str;
      str.CFormat(_T("in box label %d"), i);
      nuiLabel* pLabel = new nuiLabel(str);
      pB->AddCell(pLabel);
    }
  }
}



void MainWindow::Test02()
{
  // Load the CSS:
  LoadCSS(_T("rsrc:/test02.css"));


    nglString name;
    name.Format(_T("label"));
    nuiLabel* pLabel = new nuiLabel(name);
    pLabel->SetProperty(_T("Style"), _T("StyleLabelTest")); // "Style" is just the conventional property we use to apply some style, but you can use any property name you want (you can also use the classname (nuiLabel) and/or the object name)

  AddChild(pLabel);  
}




void MainWindow::OnClose()
{
  App->Quit();
}

