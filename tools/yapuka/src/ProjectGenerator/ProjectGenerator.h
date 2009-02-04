/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiToggleButton.h"

class ProjectGenerator : public nuiSimpleContainer
{
public:
  
  ProjectGenerator();
  virtual ~ProjectGenerator();
  
private:
  
  nuiWidget* BuildBlocSourceDirectory();
  nuiWidget* BuildBlocProjectDirectory();
  nuiWidget* BuildBlocOptions();
  nuiWidget* BuildBlocButtons();
  
  
  
  bool OnBrowseSource(const nuiEvent& rEvent);
  bool OnBrowseTarget(const nuiEvent& rEvent);

  bool OnSourceTextChanged(const nuiEvent& rEvent);
  bool OnTimerTick(const nuiEvent& rEvent);
  bool OnTargetTextChanged(const nuiEvent& rEvent);
  
  bool OnSourceSelected(const nuiEvent& rEvent);
  bool OnTargetSelected(const nuiEvent& rEvent);
  bool IsTargetValid(const nglString& rTarget);
  
  bool OnGenerateButton(const nuiEvent& rEvent);
  bool OnQuitButton(const nuiEvent& rEvent);
  
  bool OnIconUpdate(const nuiEvent& rEvent);

  bool Make();
  bool MsgError(const nglString& error);
  bool CopyDirectory(const nglPath& targetPath, const nglPath& srcpath);  
  bool GenerateFile(const nglPath& src, const nglPath& dst);
  

  nglString mNuiSourcePath;
  nglPath mNuiRelativeSource;
  nuiLabel* mpNuiRelativeSource;
  nglString mProjectTargetPath;
  nglString mProjectName;
  nglString miPhoneProjectName;
  nuiEditLine* mpNuiSource;
  nuiEditLine* mpProjectTarget;
  nuiLabel* mpProjectFilename;
  
  
  nuiImage* mpIconSourceDirectory;
  nuiImage* mpIconProjectDirectory;
  
  
  nuiToggleButton* mpCheckXcode;
  nuiToggleButton* mpCheckVisualStudio2005;
  nuiToggleButton* mpCheckVisualStudio2008;
  nuiToggleButton* mpCheckiPhone;
  
  
  bool mNuiCheckProjectFile;
  nuiLabel* mpNuiCheckProjectFile;
  bool mNuiCheckTools;
  nuiLabel* mpNuiCheckTools;
  nuiTimer* mpTimer;

  nuiEventSink<ProjectGenerator> mEventSink;
};