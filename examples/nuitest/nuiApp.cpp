/*
 NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiInit.h"
#include "nuiApp.h"
#include "nuiWin.h"

#include <string.h>
#include <stdio.h>

#include "nglConsole.h"
#include "nuiWindow.h"
#include "nuiText.h"

#include "nuiGladeLoader.h"

#include "nuiNativeResourceVolume.h"

NGL_APP_CREATE(nuiApp);
/*
_NGL_GLOBALS(nuiApp)
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  if (App)
  {
    return 1;
  }
  else
  {
    _NGL_GLOBALS_INIT(nuiApp)
    NGL_APP_TYPE* pBase = new NGL_APP_TYPE;
    App = pBase;
    int ret = pBase->WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
    pBase->ExeLoop();
    delete gpKernel;
    return ret;
  }
}
*/

nuiApp::nuiApp()
{
  win = NULL;
}

nuiApp::~nuiApp()
{
}

void nuiApp::OnExit (int Code)
{
  if (win)
    delete win;

  nuiUninit();
}

void nuiApp::OnInit()
{
  nuiInit(NULL);
  
  uint Width = 0, Height = 0;
  bool HasSize = false;
  bool IsFullScreen = false;
  bool DebugObject = false;
  bool DebugInfo = false;
  bool ShowFPS = false;
  bool PartialRedraw = true;
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
    else if (!arg.Compare(_T("--partialredraw")) || !arg.Compare(_T("-p"))) PartialRedraw = true;
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


/*
  Width = 512;
  Height = 512;
*/

  /* Create the nglWindow (and thus a GL context, don't even try to
   *   instantiate the gui (or nglFont) before the nuiWin !)
   */
  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  nglWindowInfo Info;

  Info.Flags = IsFullScreen ? nglWindow::FullScreen : 0;
  Info.Width = Width;
  Info.Height = Height;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = _T("nui test");
  Info.XPos = 0;
  Info.YPos = 0;
  
  win = new nuiWin(ContextInfo,Info, ShowFPS);
  if ((!win) || (win->GetError()))
  {
    if (win) 
      NGL_OUT(_T("Error: cannot create window (%s)\n"), win->GetErrorStr());
    Quit (1);
    return;
  }
  win->DBG_SetMouseOverInfo(DebugInfo);
  win->DBG_SetMouseOverObject(DebugObject);
  win->SetState(nglWindow::eShow);
  win->EnablePartialRedraw(PartialRedraw);
/*
  nuiImage* pImage = new nuiImage(win, nglPath(_T("../data/nui.png")));
  if (pImage)
  {
    pImage->SetPosition(nuiFill);
    pImage->SetUseAlpha(true);
  }
  */
  
  NGL_OUT(_T("nuiObject: %d\nnuiWidget: %d\nnuiContainer: %d\nbool: %d\n"), sizeof(nuiObject), sizeof(nuiWidget), sizeof(nuiContainer), sizeof(bool));
}
