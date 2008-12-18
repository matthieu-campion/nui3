/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __MainWindow_h__
#define __MainWindow_h__

#include "ngl.h"
#include "nuiEvent.h"
#include "nuiMainWindow.h"
#include "nuiFileSelector.h"


class ElementDesc;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo);
	void InitAttributes();

  virtual ~MainWindow();


  void OnCreation();
  bool LoadCSS(const nglPath& rPath);

  void OnClose();


	// attributes ***********************************
	nuiAttrib<nuiPoint> mAttributeMouseCoord;


protected:

	nuiPoint GetMouseCoord();
	void SetMouseCoord(nuiPoint point);

  nuiEventSink<MainWindow> mWinSink;
  
	nuiAttribute<nuiPoint>* mpAttributeMouseCoord;
	nuiPoint mMouseCoord;
};

#endif
