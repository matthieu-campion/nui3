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

#define GRID_NB_ROWS 4
#define GRID_NB_COLUMNS 4

/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo, bool ShowFPS, const nglContext* pShared )
: nuiMainWindow(rContextInfo, rInfo, pShared, nglPath(ePathCurrent)), mEventSink(this)
{
#ifdef _DEBUG_
  SetDebugMode(true);
#endif
  
#ifdef NUI_IPHONE
  LoadCSS(_T("rsrc:/css/style-iPhone.css"));
#else
  LoadCSS(_T("rsrc:/css/style.css"));
#endif

  mpLoopVoice = NULL;
}

MainWindow::~MainWindow()
{
  if (mpLoopVoice)
    mpLoopVoice->Release();
  
  for (uint32 i = 0; i < mSounds.size(); i++)
    mSounds[i]->Release();
}

void MainWindow::OnCreation()
{  
  mpLoopVoice = GetAudioEngine()->PlaySound(nglPath(_T("rsrc:/audio/drums-loop.wav")), nuiSound::eStream);
  mpLoopVoice->Acquire();
  mpLoopVoice->SetLoop(true);
  
  
  for (uint32 i = 0; i < GRID_NB_ROWS * GRID_NB_COLUMNS; i++)
  {
    nglString str;
    str.Format(_T("rsrc:/audio/sound%d.wav"), i);
    nglPath path(str);
    
    nuiSound* pSound = nuiSoundManager::Instance.GetSound(path, nuiSound::eMemory);
    pSound->Acquire();
    mSounds.push_back(pSound);
  }
  
  
  std::vector<std::pair<nglKeyCode, nglString> > keys;
  keys.push_back(std::make_pair(NK_1, _T("1")));
  keys.push_back(std::make_pair(NK_2, _T("2")));
  keys.push_back(std::make_pair(NK_3, _T("3")));
  keys.push_back(std::make_pair(NK_4, _T("4")));
  
  keys.push_back(std::make_pair(NK_Q, _T("q")));
  keys.push_back(std::make_pair(NK_W, _T("w")));
  keys.push_back(std::make_pair(NK_E, _T("e")));
  keys.push_back(std::make_pair(NK_R, _T("r")));
  
  keys.push_back(std::make_pair(NK_A, _T("a")));
  keys.push_back(std::make_pair(NK_S, _T("s")));
  keys.push_back(std::make_pair(NK_D, _T("d")));
  keys.push_back(std::make_pair(NK_F, _T("f")));
  
  keys.push_back(std::make_pair(NK_Z, _T("z")));
  keys.push_back(std::make_pair(NK_X, _T("x")));
  keys.push_back(std::make_pair(NK_C, _T("c")));
  keys.push_back(std::make_pair(NK_V, _T("v")));
  
  uint32 nbRows = GRID_NB_ROWS;
  uint32 nbCol = GRID_NB_COLUMNS;
  nuiGrid* pGrid = new nuiGrid(nbCol, nbRows);
  pGrid->SetObjectName(_T("ButtonsGrid"));
  AddChild(pGrid);
  
  NGL_ASSERT(keys.size() == nbRows * nbCol);
  
  for (uint32 r = 0; r < nbRows; r++)
  {
    for (uint32 c = 0; c < nbCol; c++)
    {
      uint32 index = r * nbCol + c;
      nglString text = keys[index].second;
      nuiButton* pButton = new nuiButton(text);
      mSoundButtons.push_back(pButton);
      pButton->SetObjectName(_T("GridButton"));
      pGrid->SetCell(c, r, pButton);
      mEventSink.Connect(pButton->Activated, &MainWindow::OnButtonActivated, (void*)index);
      
      nglString hotkeyName;
      nglString hotkeyDesc;
      hotkeyName.Format(_T("SoundButton%d"), index);
      hotkeyDesc.Format(_T("PlaySound%d"), index);
      RegisterHotKeyKey(hotkeyName, keys[index].first, nuiNoKey, false, false, hotkeyDesc);
      mEventSink.Connect(GetHotKeyEvent(hotkeyName), &MainWindow::OnSoundHotKey, (void*)index);
    }
  }
}

void MainWindow::OnButtonActivated(const nuiEvent& rEvent)
{
  uint32 index = (uint32)rEvent.mpUser;  
  GetAudioEngine()->PlaySound(mSounds[index]);
}

void MainWindow::OnSoundHotKey(const nuiEvent& rEvent)
{
  uint32 i = (uint32)rEvent.mpUser;
  mSoundButtons[i]->Activate();
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
