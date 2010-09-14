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


/*
 * MainWindow
 */

class Springs : public nuiWidget
{
public:
  Springs()
  {
    mP1.Set(0, 20, 0);
    mP2.Set(100, 20, 0);
    mForce.Set(10, 0, 0);
    StartAutoDraw();
  }
  
  ~Springs()
  {
    
  }
  
  nuiRect CalcIdealSize()
  {
    return nuiRect(320, 240);
  }
  
  bool Draw(nuiDrawContext* pContext)
  {
    float K = 1.0f;
    float D = 50.0f;
    SpringComputeSingleForce(mForce, mP1, mP2, K, D);

    mP2 += mForce;
    
    pContext->DrawLine(mP1[0], mP1[1], mP2[0], mP2[1]);
    
    return true;
  }
  
  static const float SPRING_TOLERANCE = 0.0000000005f;
  
  void SpringComputeSingleForce(nuiVector3& force, const nuiVector3& pa, const nuiVector3& pb, float k, float delta_distance)
  {
    nuiVector3 force_dir;
    float intensity;
    float distance;
    float delta;
    
    //get the distance
    nuiVector3 d = pb - pa;
    
    distance = d.Length();
    
    if (distance < SPRING_TOLERANCE)
    {
      force[0] =
      force[1] =
      force[2] = 0.0f;
      
      return;
    }
    
    force_dir = d;
    
    //normalize
    force_dir *= (float)(1.0 / distance);
    
    //force
    delta = distance - delta_distance;
    intensity = k * delta;
    
    //store
    force = force_dir;
    force *= intensity;
  }
  
protected:
  nuiVector3 mP1;
  nuiVector3 mP2;
  nuiVector3 mForce;
};



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
  AddChild(new Springs);
}



bool MainWindow::OnButtonClick(const nuiEvent& rEvent)
{
  nglString message;
  double currentTime = nglTime();
  message.Format(_T("click time: %.2f"), currentTime);
  mMyLabel->SetText(message);
  
  return true; // means the event is caught and not broadcasted
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
