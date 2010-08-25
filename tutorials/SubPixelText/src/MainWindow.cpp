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
#include "nuiFontManager.h"

class Scroller : public nuiWidget
{
public:

  Scroller()
  : mEventSink(this)
  {
    nuiTimer* pTimer = nuiAnimation::AcquireTimer();
    mEventSink.Connect(pTimer->Tick, &Scroller::OnTick);
    mMin = 20;
    mX = mMin;
    mY = mMin;
    mMax = 200;
    mStep = .01;
    
    nglString str("Testing sub pixel font rendering by interpolation");
    nuiFontRequest request;
    request.SetName(_T("Arial"), 1);
    request.SetBold(false, 1);
    mpFont = nuiFontManager::GetManager().GetFont(request);
    mpLayout = new nuiFontLayout(*mpFont);
    mpLayout->Layout(str);
  }
  
  virtual ~Scroller()
  {
    nuiAnimation::ReleaseTimer();
    delete mpLayout;
    mpFont->Release();
  }
  
  nuiRect CalcIdealSize()
  {
    return nuiRect(400, 200);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    pContext->DrawText(mX, 50  + mY, *mpLayout, true);
    pContext->DrawText(mX, 100 + mY, *mpLayout, false);
    return true;
  }
  
  bool OnTick(const nuiEvent& rEvent)
  {
    mX += mStep;
    mY += mStep;
    if (mX > mMax || mX < mMin)
      mStep = -mStep;
    
    Invalidate();
    return false;
  }
  
protected:
  nuiFont* mpFont;
  nuiFontLayout* mpLayout;
  float mX;
  float mY;
  float mMin;
  float mMax;
  float mStep;
  nuiEventSink<Scroller> mEventSink;
};


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
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnCreation()
{
  // a vertical box for page layout
  nuiVBox* pLayoutBox = new nuiVBox(0);
  pLayoutBox->SetObjectName(_T("MainBox"));
  pLayoutBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pLayoutBox);
  
  // image in the first box's cell
  nuiImage* pImg = new nuiImage();
  pImg->SetObjectName(_T("MyImage"));
  pImg->SetPosition(nuiCenter);
  pLayoutBox->AddCell(pImg);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
  
  // label with decoration in the third cell
  pLayoutBox->AddCell(new Scroller);
  pLayoutBox->SetCellExpand(pLayoutBox->GetNbCells()-1, nuiExpandShrinkAndGrow);
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
