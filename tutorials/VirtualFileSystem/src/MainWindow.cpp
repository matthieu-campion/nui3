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

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
  // create and mount virtual disk from a zip file (can be compressed or not...)
  nglZipFS* pFS = new nglZipFS(_T("rsrc:/myVirtualDisk.zip"));
  bool res = pFS->Open();
  if (!res)
    NGL_OUT(_T("Error trying to open the file system\n"));
  nglVolume::Mount(pFS);

  // load css from the virtual disk
#ifdef NUI_IPHONE
  LoadCSS(_T("myVirtualDisk:/myFolder/css/style-iPhone.css"));
#else
  LoadCSS(_T("myVirtualDisk:/myFolder/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // GUI : a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetObjectName(_T("MainBox"));
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // THE IMAGE IS LOAD FROM THE VIRTUAL DISK
  nuiImage* pImg = new nuiImage(_T("myVirtualDisk:/myFolder/images/Logo.png"));
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
  
  
  
  
  
  // something else : let's enumerate available audio devices
  // (this way, the template has everything it needs to compile and link audio source code)
  // see output debug console to read the results
  nuiAudioDeviceManager& deviceMan = nuiAudioDeviceManager::Get();
  uint32 devs = nuiAudioDeviceManager::Get().GetDeviceCount();
  for (uint32 i = 0; i < devs; i++)
  {
    nglString name = nuiAudioDeviceManager::Get().GetDeviceName(i);
    name.Add(_T(" (")).Add(nuiAudioDeviceManager::Get().GetDeviceAPIName(i)).Add(_T(")"));
    #ifdef _WIN32_
    nglString tmp;
    tmp.Format(_T("AudioDevice : %ls\n"), name.GetChars());
    OutputDebugString(tmp.GetChars());
    #else
    NGL_OUT(_T("AudioDevice : %ls\n"), name.GetChars());
    #endif
  }  
}







void MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
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
