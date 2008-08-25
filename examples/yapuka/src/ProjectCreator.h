/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"
#include "nuiDialog.h"

class ProjectCreator : public nuiDialog
{
public:
  
  ProjectCreator(nuiMainWindow* pWindow);
  virtual ~ProjectCreator();
  
private:
  
  bool OnDialogdone(const nuiEvent& rEvent);
  
  bool OnBrowseSource(const nuiEvent& rEvent);
  bool OnBrowseTarget(const nuiEvent& rEvent);

  bool OnSourceTextChanged(const nuiEvent& rEvent);
  bool OnTimerTick(const nuiEvent& rEvent);
  bool OnTargetTextChanged(const nuiEvent& rEvent);
  
  bool OnSourceSelected(const nuiEvent& rEvent);
  bool OnTargetSelected(const nuiEvent& rEvent);
  

  bool Make();
  bool MsgError(const nglString& error);
  bool GenerateFile(const nglPath& src, const nglPath& dst);
  

  nglString mNuiSourcePath;
  nglPath mNuiRelativeSource;
  nuiLabel* mpNuiRelativeSource;
  nglString mProjectTargetPath;
  nglString mProjectName;
  nuiEditLine* mpNuiSource;
  nuiEditLine* mpProjectTarget;
  nuiLabel* mpProjectFilename;
  
  bool mNuiCheckProjectFile;
  nuiLabel* mpNuiCheckProjectFile;
  bool mNuiCheckTools;
  nuiLabel* mpNuiCheckTools;
  nuiTimer* mpTimer;

  nuiMainWindow* mpWindow;
  
  nuiEventSink<ProjectCreator> mEventSink;
};