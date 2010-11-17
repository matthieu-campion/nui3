#include "nui.h"
#include "nuiInit.h"
#include "nuiMainWindow.h"
#include "nuiButton.h"

class Dummy
{
public:
  Dummy()
  : mEventSink(this)
  {
    nuiInit(NULL,NULL);
    
    nuiContextInfo ContextInfo(nuiContextInfo::StandardContext3D);
    nglVideoMode current_mode;
    nglWindowInfo Info;
    
    Info.Flags = false ? nglWindow::FullScreen : 0;
    Info.Width =  current_mode.GetWidth() * 1.f;
    if (Info.Width < 0)
      Info.Width = 640;
    Info.Height = current_mode.GetHeight() * .9;
    if (Info.Height < 0)
      Info.Height = 480;
    Info.Pos = nglWindowInfo::ePosUser;
    Info.Title = _T("test");
    Info.XPos = 0;
    Info.YPos = 0;
    
    
    mpWin = new nuiMainWindow(ContextInfo,Info, false);
    if ((!mpWin) || (mpWin->GetError()))
    {
      if (mpWin) 
        NGL_OUT(_T("Error: cannot create window (%s)\n"), mpWin->GetErrorStr());
      //    Quit (1);
      return;
    }
    mpWin->DBG_SetMouseOverInfo(false);
    mpWin->DBG_SetMouseOverObject(false);
    mpWin->SetState(nglWindow::eShow);
    
    nuiButton* pButton = new nuiButton(_T("pouet"));
    mpWin->AddChild(pButton);
    mEventSink.Connect(pButton->Activated, &Dummy::OnBleh);
  }

  ~Dummy()
  {
    delete mpWin;
  }
  
protected:
  nuiEventSink<Dummy> mEventSink;
  nuiMainWindow* mpWin;
  void OnBleh(const nuiEvent& rEvent)
  {
    delete this;
  }
};

UIWindow* GetAppWindow()
{
  return (UIWindow *)([[UIApplication sharedApplication].windows objectAtIndex:0]);
}

static Dummy* gpDummy = NULL;
extern "C" void StartGame()
{
  UIWindow* pWin = GetAppWindow();
  pWin.hidden = TRUE;
  gpDummy = new Dummy;
}


extern "C" void StopGame()
{
  delete gpDummy;
  gpDummy = NULL;
}


