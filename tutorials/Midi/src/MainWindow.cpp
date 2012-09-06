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
  for (int32 i = 0; i < mMiniIn.size(); i++)
  {
    mMiniIn[i]->Close();
    mMiniIn[i]->Release();
  }
  mMiniIn.clear();
}

void MidiRead(nuiMidiInPort* pPort, const uint8* pData, uint32 size, double time)
{
  if (size == 1 && pData[0] == 0xfe) // filter out active sensing
    return;
  if (size == 1 && pData[0] == 0xf8) // filter out timing ticks
    return;

  
  //Parse data:
  const uint8* p = pData;
  while (size)
  {
    switch (*p & 0xf0)
    {
      case 0x80:
        NGL_OUT("[%x] Note Off %d %d\n", *p & 0xf, p[1], p[2]);
        p+= 3;
        size -= 3;
        break;
      case 0x90:
        if (p[2] > 0)
          NGL_OUT("[%x] Note On %d %d\n", *p & 0xf, p[1], p[2]);
        else
          NGL_OUT("[%x] Note Off %d %d\n", *p & 0xf, p[1], p[2]);
        p+= 3;
        size -= 3;
        break;
      default:
        NGL_OUT("Received %d bytes from port %p (time = %f)\n", size, pPort, time - (double)nglTime());
        if (0)
        {
          static int32 i = 0;
          NGL_OUT("%d, %f\n", i++, (time - (double)nglTime()));
        }
        pPort+= size;
        size = 0;
        break;
    }
  }
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

    if (pPort->Open(MidiRead))
      mMiniIn.push_back(pPort);
    else
    {
      NGL_OUT("\tUnable to open this port!\n");
      pPort->Release();
    }
    
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
