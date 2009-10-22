/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiHTMLView.h"

class nuiEditLine;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();

protected:
  
  bool OnButtonClick(const nuiEvent& rEvent);
  void OnURLChanged(const nglString& rString);
private:
  
  bool LoadCSS(const nglPath& rPath);

  nuiHTMLView* mpHTMLView;
  nuiEditLine* mpInput;
  
  nuiEventSink<MainWindow> mEventSink;

  nuiSlotsSink mSlotSink;
};

