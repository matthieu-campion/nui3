/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiInit.h"
#include "Application.h"
#include "MainWindow.h"

#include "nglConsole.h"

#define APPLICATION_TITLE _T("tutorial MainMenu")


NGL_APP_CREATE(Application);

Application::Application()
: mEventSink(this)
{
  mpMainWindow = NULL;
}

Application::~Application()
{
}

void Application::OnExit (int Code)
{
  if (mpMainWindow)
  {
    mpMainWindow->Release();
    mpMainWindow = NULL;
  }

  nuiUninit();
}

void Application::OnInit()
{
  nuiInit(NULL);

  uint Width = 0, Height = 0;
  bool HasSize = false;
  bool IsFullScreen = false;
  bool DebugObject = false;
  bool DebugInfo = false;
  bool ShowFPS = false;
//  nuiRenderer Renderer = eDirect3D;
  nuiRenderer Renderer = eOpenGL;
//  nuiRenderer Renderer = eSoftware;

  // Accept NGL default options
  ParseDefaultArgs();

  GetLog().UseConsole(true);
  GetLog().SetLevel(_T("font"), 100);

  // Manual
  if ( (GetArgCount() == 1) &&
       ((!GetArg(0).Compare(_T("-h"))) || (!GetArg(0).Compare(_T("--help")))) )
  {
    NGL_OUT(_T("no params\n"));
    Quit (0);
    return;
  }
  
  // Parse args
  int i = 0;
  while (i < GetArgCount())
  {
    nglString arg = GetArg(i);
    if ((!arg.Compare(_T("--size")) || !arg.Compare(_T("-s"))) && ((i+1) < GetArgCount()))
    {
      int w, h;

      std::string str(GetArg(i+1).GetStdString());
      sscanf(str.c_str(), "%dx%d", &w, &h);
      if (w > 0) Width  = w;
      if (h > 0) Height = h;
      HasSize = true;
      i++;
    }
    else if (!arg.Compare(_T("--showfps")) || !arg.Compare(_T("-fps"))) ShowFPS = true;
    else if (!arg.Compare(_T("--fullscreen")) || !arg.Compare(_T("-f"))) IsFullScreen = true;
    else if (!arg.Compare(_T("--debugobject")) || !arg.Compare(_T("-d"))) DebugObject = true;
    else if (!arg.Compare(_T("--debuginfo")) || !arg.Compare(_T("-i"))) DebugInfo = true;
    else if (!arg.Compare(_T("--renderer")) || !arg.Compare(_T("-r"))) 
    {
      arg = GetArg(i+1);
      if (!arg.Compare(_T("opengl"))) Renderer = eOpenGL;
      else if (!arg.Compare(_T("direct3d"))) Renderer = eDirect3D;
      else if (!arg.Compare(_T("software"))) Renderer = eSoftware;
      i++;
    }
    i++;
  }
  
  nuiMainWindow::SetRenderer(Renderer);

  if (!HasSize)
  {
    if (IsFullScreen)
    {
      nglVideoMode current_mode;

      Width = current_mode.GetWidth();
      Height = current_mode.GetHeight();
    }
    else
    {
      Width = 800;
      Height = 600;
    }
  }

  // build main menu bar
  InitMainMenu();


  /* Create the nglWindow (and thus a GL context, don't even try to
   *   instantiate the gui (or nglFont) before the nuiWin !)
   */
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  nglWindowInfo Info;

  Info.Flags = IsFullScreen ? nglWindow::FullScreen : 0;
  Info.Width = Width;
  Info.Height = Height;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = APPLICATION_TITLE;
  Info.XPos = 0;
  Info.YPos = 0;
        
  mpMainWindow = new MainWindow(ContextInfo,Info, ShowFPS);
  if ((!mpMainWindow) || (mpMainWindow->GetError()))
  {
    if (mpMainWindow) 
      NGL_OUT(_T("Error: cannot create window (%s)\n"), mpMainWindow->GetErrorStr());
    Quit (1);
    return;
  }
  mpMainWindow->Acquire();
  mpMainWindow->DBG_SetMouseOverInfo(DebugInfo);  mpMainWindow->DBG_SetMouseOverObject(DebugObject);
  mpMainWindow->SetState(nglWindow::eShow);

}



// build menu bar
void Application::InitMainMenu()
{
  // root item for the menu bar. it's not displayed
  nuiMainMenuItem* pRoot = new nuiMainMenuItem(_T("MainMenuRoot")); 
  
  // create the main menu object, you'll have to register it from the main window
  mpMainMenu = new nuiMainMenu(pRoot);
  
  
  nuiMainMenuItem* pMenuFile = new nuiMainMenuItem(_T("File"));
  pRoot->AddChild(pMenuFile);
  nuiMainMenuItem* pMenuEdit = new nuiMainMenuItem(_T("Edit"));
  pRoot->AddChild(pMenuEdit);
  mpMenuTest = new nuiMainMenuItem(_T("Test"));
  pRoot->AddChild(mpMenuTest);
  nuiMainMenuItem* pMenuHelp = new nuiMainMenuItem(_T("Help"));
  pRoot->AddChild(pMenuHelp);

  //***********************************************************
  // menu File
  //
  nuiMainMenuItem* pFileOpen = new nuiMainMenuItem(_T("Open"));
  pMenuFile->AddChild(pFileOpen);
  mEventSink.Connect(pFileOpen->Activated, &Application::OnMenuFileOpen);
  
  nuiMainMenuItem* pFileClose = new nuiMainMenuItem(_T("Close"));
  pMenuFile->AddChild(pFileClose);
  mEventSink.Connect(pFileClose->Activated, &Application::OnMenuFileClose);
  
  nuiMainMenuItem* pSeparator = new nuiMainMenuItem(_T(""), eItemSeparator);
  pMenuFile->AddChild(pSeparator);

  nuiMainMenuItem* pFileDI = new nuiMainMenuItem(_T("Disabled Item"));
  pMenuFile->AddChild(pFileDI);
  
  // we want to disable this item.
  pFileDI->SetEnabled(false);
  mEventSink.Connect(pFileDI->Activated, &Application::OnMenuFileDI); // you can still enable the item whenever you want. 'Til you do so, the receiver can't get any event

  
  //***********************************************************
  // menu Edit
  //
  // example of checkabled item
  nuiMainMenuItem* pEditOption = new nuiMainMenuItem(_T("Option"), eItemCheck);
  pMenuEdit->AddChild(pEditOption);
  mEventSink.Connect(pEditOption->Checked, &Application::OnMenuEditOptionChecked, (void*)true);  
  mEventSink.Connect(pEditOption->Unchecked, &Application::OnMenuEditOptionChecked, (void*)false);  

  // example of a submenu
  nuiMainMenuItem* pEditSub = new nuiMainMenuItem(_T("Preferences"));
  pMenuEdit->AddChild(pEditSub);
  
  nuiMainMenuItem* pPref1 = new nuiMainMenuItem(_T("Option1"), eItemCheck);
  pEditSub->AddChild(pPref1);
  mEventSink.Connect(pPref1->Checked, &Application::OnMenuEditPrefOption1Checked, (void*)true);
  mEventSink.Connect(pPref1->Unchecked, &Application::OnMenuEditPrefOption1Checked, (void*)false);
}



//**********************************************************************************
//
// menu commands' receivers
//

void Application::OnMenuFileOpen(const nuiEvent& rEvent)
{
  if (mpMainWindow)
    mpMainWindow->MyCommand(_T("File / Open"));
  rEvent.Cancel();
}

void Application::OnMenuFileClose(const nuiEvent& rEvent)
{
  if (mpMainWindow)
    mpMainWindow->MyCommand(_T("File / Close"));
  rEvent.Cancel();
}

void Application::OnMenuFileDI(const nuiEvent& rEvent)
{
  if (mpMainWindow)
    mpMainWindow->MyCommand(_T("this should not happen in our example, since the item has been disabled :)"));
  rEvent.Cancel();
}

void Application::OnMenuEditCopy(const nuiEvent& rEvent)
{
  if (mpMainWindow)
    mpMainWindow->MyCommand(_T("Edit / Copy"));
  rEvent.Cancel();
}

void Application::OnMenuEditPaste(const nuiEvent& rEvent)
{
  if (mpMainWindow)
    mpMainWindow->MyCommand(_T("Edit / Paste"));
  rEvent.Cancel();
}

void Application::OnMenuEditOptionChecked(const nuiEvent& rEvent)
{
  if (!mpMainWindow)
    return;
  
  bool set = (bool)rEvent.mpUser;
  if (set)
    mpMainWindow->MyCommand(_T("Edit / Option CHECKED"));
  else
    mpMainWindow->MyCommand(_T("Edit / Option UNCHECKED"));
  rEvent.Cancel();
}

void Application::OnMenuEditPrefOption1Checked(const nuiEvent& rEvent)
{
  if (!mpMainWindow)
    return;
  
  bool set = (bool)rEvent.mpUser;
  if (set)
    mpMainWindow->MyCommand(_T("Edit / Preferences / Option1 CHECKED"));
  else
    mpMainWindow->MyCommand(_T("Edit / Preferences / Option1 UNCHECKED"));
      
  rEvent.Cancel();
}



nuiMainMenu* Application::GetMainMenu()
{
  return mpMainMenu;
}

nuiMainMenuItem* Application::GetTestMenu()
{
  return mpMenuTest;
}


Application* GetApp()
{
  return ((Application*)App);
}