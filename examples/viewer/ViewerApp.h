/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __ViewerApp_h__
#define __ViewerApp_h__

#include "nui.h"
#include "nuiApplication.h"
#include "ViewerWin.h"


class ViewerApp : public nuiApplication
{
public:
	ViewerApp();

 	void OnInit();
  void OnExit (int Code);

protected:
 	ViewerWin *mpWindow;
};

#endif // __ViewerApp_h__
