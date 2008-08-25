/*
  NUI3 demonstration sample - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#ifndef __MainWindow_h__
#define __MainWindow_h__

#include "nuiMainWindow.h"

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();

  void OnClose();

  bool LoadCSS(const nglPath& rPath);

protected:

  void Test01();
  void Test02();

  nuiEventSink<MainWindow> mSink;
};

#endif//__MainWindow_h__
