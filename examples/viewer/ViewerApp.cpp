/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "ViewerApp.h"


/* Create application instance
*/
NGL_APP_CREATE(ViewerApp);


ViewerApp::ViewerApp()
{
	mpWindow = NULL;
}

void ViewerApp::OnExit (int Code)
{
  if (mpWindow)
    delete mpWindow;
}

void ViewerApp::OnInit()
{
  nuiRenderer renderer = eOpenGL;

  ParseDefaultArgs();

  int i = 0;
  while (i < GetArgCount())
  {
    nglString arg = GetArg(i);

    if (arg[0] != _T('-'))
      break;

    else if (arg == _T("--help") || arg == _T("-h"))
    {
      NGL_OUT(_T("viewer [options] <filename>\n\nOptions :\n\n"));
      NGL_OUT(_T("  -r, --render    render target, 'software' or 'opengl' (default)\n"));
      Quit();
      return;
    }
    else if (arg == _T("--renderer") || arg == _T("-r"))
    {
      arg = GetArg(i+1);
      if (arg == _T("opengl")) renderer = eOpenGL; else
      if (arg == _T("direct3d"))
      {
        renderer = eDirect3D;
      } else
      if (arg == _T("software")) renderer = eSoftware; else
      {
        NGL_OUT(_T("Unknown rendering target\n"));
        exit(0);
      }
      i++;
    }
    i++;
  }
  
  nuiMainWindow::SetRenderer(renderer);

  nuiContextInfo ctx_info(nuiContextInfo::StandardContext3D);
  nglWindowInfo win_info;
  nglWindow::ParseArgs(ctx_info, win_info);
  nglPath path = i < GetArgCount() ? nglPath(GetArg(i)) : nglPath(ePathCurrent);
  if (!path.Exists())
    path = nglPath(ePathCurrent);

  mpWindow = new ViewerWin(ctx_info, win_info, path);
  if (mpWindow->GetError())
  {
    NGL_OUT(_T("Error: cannot create rendering window (%s)\n"), mpWindow->GetErrorStr());
    Quit(1);
    return;
  }
  
  mpWindow->SetState(nglWindow::eShow);
  

}
