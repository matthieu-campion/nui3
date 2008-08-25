/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiApp_h__
#define __nuiApp_h__

#include "ngl.h"
#include "nuiApplication.h"
#include "nuiWindow.h"

class nuiWin;

class nuiApp : public nuiApplication
{
public:
  nuiApp();
  ~nuiApp();

   void OnInit();
  void OnExit (int Code);
protected:
   nuiWin *win;
};

#endif
