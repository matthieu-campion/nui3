/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiInit.h"
#include "Application.h"

#include "nglConsole.h"

#define APPLICATION_TITLE _T("SimpleTouch!")


NGL_APP_CREATE(Application);

class TouchTest : public nuiWidget
{
public:
  TouchTest()
  : mSink(this)
  {
    StartAutoDraw();
    mpTexture = nuiTexture::GetTexture(nglPath(_T("rsrc:/particle.png")));
  }
  
  ~TouchTest()
  {
    mpTexture->Release();
  }
  
  nuiRect CalcIdealSize()
  {
    return nuiRect(640, 480);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    pContext->EnableBlending(true);
    pContext->EnableTexturing(true);
    pContext->SetTexture(mpTexture);
    pContext->SetBlendFunc(nuiBlendTransp);
    for (int32 i = 0; i < _NUI_MAX_TOUCHES_; i++)
    {
      nuiShape shp;
      mTouches[i].mValue += mTouches[i].mIncr;
      if (mTouches[i].mValue > 1.0f)
      {
        mTouches[i].mValue = 1.0f;
        mTouches[i].mIncr = 0;
      }
      if (mTouches[i].mValue <= 0.0f)
      {
        mTouches[i].mValue = 0;
        mTouches[i].mIncr = 0;
      }
      
      float w = 50;
      float h = 50;
      pContext->SetFillColor(nuiColor(1.0f, 1.0f, 1.0f, mTouches[i].mValue));
      nuiRect rs((float)mpTexture->GetWidth(), (float)mpTexture->GetHeight());
      nuiRect rd(mTouches[i].mX - w, mTouches[i].mY - h, w * 2, h * 2);
      pContext->DrawImage(rd, rs);
    }
    return true;
  }
  
  bool MouseClicked(const nglMouseInfo& rInfo)
  {
    Grab();
    mTouches[rInfo.TouchId].mIncr = 0.15;
    mTouches[rInfo.TouchId].mX = rInfo.X;
    mTouches[rInfo.TouchId].mY = rInfo.Y;
    
    return true;
  }
  
  bool MouseUnclicked(const nglMouseInfo& rInfo)
  {
    Ungrab();
    mTouches[rInfo.TouchId].mIncr = -0.05;
    mTouches[rInfo.TouchId].mX = rInfo.X;
    mTouches[rInfo.TouchId].mY = rInfo.Y;
    
    return true;
  }
  
  bool MouseMoved(const nglMouseInfo& rInfo)
  {
    mTouches[rInfo.TouchId].mX = rInfo.X;
    mTouches[rInfo.TouchId].mY = rInfo.Y;
    
    return true;
  }
  
private:
  class Touch
  {
  public:
    Touch()
    {
      mX = 0;
      mY = 0;
      
      mValue = 0;
      mIncr = 0;
    }
    
    float mX, mY, mValue, mIncr;
  };
  Touch mTouches[_NUI_MAX_TOUCHES_];
  
  nuiTimer* mpTimer;
  nuiEventSink<TouchTest> mSink;
  nuiTexture* mpTexture;
};



Application::Application()
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

  
  nuiRenderer Renderer = eOpenGL;
//  nuiRenderer Renderer = eSoftware;
//  nuiRenderer Renderer = eDirect3D;

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
#ifdef NUI_IPHONE
      Width = 320;
      Height = 480;
#else
      Width = 800;
      Height = 600;
#endif
    }
  }


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
        
  mpMainWindow = new nuiMainWindow(ContextInfo, Info);
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

  mpMainWindow->AddChild(new TouchTest);
}


nuiMainWindow* Application::GetMainWindow()
{
  return mpMainWindow;
}


Application* GetApp()
{
  return ((Application*)App);
}

nuiMainWindow* GetMainWindow()
{
  return ((Application*)App)->GetMainWindow();
}