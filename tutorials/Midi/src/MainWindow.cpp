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

#include "nuiMidi.h"



/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
  SetDebugMode(true);
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  nuiMidiManager& midi(nuiMidiManager::Get());
  uint32 incount = midi.GetInPortCount();
  NGL_OUT("In port count: %d\n", incount);
  for (uint32 i = 0; i < incount; i++)
  {
    nuiMidiInPort* pPort = midi.GetInPort(i);
    nglString name = pPort->GetName();
    nglString manuf = pPort->GetManufacturer();
    nglString device = pPort->GetDeviceName();
    pPort->Release();
    NGL_OUT("%d: %s / %s / %s\n", i, name.GetChars(), manuf.GetChars(), device.GetChars());
  }

  uint32 outcount = midi.GetOutPortCount();
  NGL_OUT("Out port count: %d\n", outcount);
  for (uint32 i = 0; i < outcount; i++)
  {
    nuiMidiOutPort* pPort = midi.GetOutPort(i);
    nglString name = pPort->GetName();
    nglString manuf = pPort->GetManufacturer();
    nglString device = pPort->GetDeviceName();
    pPort->Release();
    NGL_OUT("%d: %s / %s / %s\n", i, name.GetChars(), manuf.GetChars(), device.GetChars());
  }
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
