/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#ifndef __Application_h__
#define __Application_h__

#include "nui.h"
#include "nuiApplication.h"

class MainWindow;

class Application : public nuiApplication
{
public:
  Application();
  ~Application();

   void OnInit();
  void OnExit (int Code);
protected:
   MainWindow *mpMainWindow;
};

#endif
