#include "nui.h"

#include "nuiInit.h"
#include "nuiApp.h"
#include "nuiWin.h"

NGL_APP_CREATE(nuiApp);

nuiApp::nuiApp()
{
}
nuiApp::~nuiApp()
{
}

void nuiApp::OnWillExit()
{
}

void nuiApp::OnExit (int Code)
{
  nuiUninit();
}

void nuiApp::OnInit()
{
  nuiInit(NULL);

  nuiRenderer Renderer = eOpenGL;

// Accept NGL default options
  ParseDefaultArgs();

  uint Width = 0, Height = 0;
  uint Angle = 0;
//Angle = 90;
//Angle = 180;
  Angle = 0;

  bool HasSize = false;
  bool IsFullScreen = false;
  bool DebugObject = false;
  bool DebugInfo = false;
  bool ShowFPS = false;
  bool PartialRedraw = false;
//  bool PartialRedraw = true;

  nuiMainWindow::SetRenderer(Renderer);

  nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
  ContextInfo.FrameBitsR = 5;
  ContextInfo.FrameBitsG = 6;
  ContextInfo.FrameBitsB = 5;
  ContextInfo.FrameBitsA = 0;
  ContextInfo.DepthBits = 0;

///< If false partial redraw cannot be achieved
  ContextInfo.CopyOnSwap = PartialRedraw;
 

  nglWindowInfo Info;

  Info.Flags = IsFullScreen ? nglWindow::FullScreen : 0;
  Info.Width = Width;
  Info.Height = Height;
  Info.Rotate = Angle;
  Info.Pos = nglWindowInfo::ePosCenter;
  Info.Title = _T("nuiApp");
  Info.XPos = 0;
  Info.YPos = 0;

  nuiWin* pWin = new nuiWin(ContextInfo, Info);

  if ((!pWin) || (pWin->GetError()))
  {
    if (pWin) {
      NGL_OUT(_T("[nuiApp] Error: cannot create window (%s)\n"), pWin->GetErrorStr());
    }
    Quit (1);
    return;
  }

  pWin->SetState(nglWindow::eShow);
  pWin->EnablePartialRedraw(PartialRedraw);
//  mpWindow->SetFrameRateLimit(20.f);
}
