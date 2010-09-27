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
  
  
  
  void OnBrowseSource(const nuiEvent& rEvent);
  void OnBrowseTarget(const nuiEvent& rEvent);

  void OnSourceTextChanged(const nuiEvent& rEvent);
  void OnTimerTick(const nuiEvent& rEvent);
  void OnTargetTextChanged(const nuiEvent& rEvent);
  
  void OnSourceSelected(const nuiEvent& rEvent);
  void OnTargetSelected(const nuiEvent& rEvent);

  void OnGenerateButton(const nuiEvent& rEvent);
  void OnQuitButton(const nuiEvent& rEvent);
  
  void OnIconUpdate(const nuiEvent& rEvent);

  bool Make();
  bool MsgError(const nglString& error);
  bool CopyDirectory(const nglPath& targetPath, const nglPath& srcpath);  
  bool GenerateFile(const nglPath& src, const nglPath& dst);
  

  nglString mNuiSourcePath;
  nglPath mNuiRelativeSource;
  nglPath mNuiTemplatePath;
  nuiLabel* mpNuiRelativeSource;
  nglString mProjectTargetPath;
  nglString mProjectName;
  nuiEditLine* mpNuiSource;
  nuiEditLine* mpProjectTarget;
  nuiLabel* mpProjectFilename;
  
  
  nuiImage* mpIconSourceDirectory;
  nuiImage* mpIconProjectDirectory;
  
  
  nuiToggleButton* mpCheckXcode;
  nuiToggleButton* mpCheckVisualStudio2008;
  
  
  bool mNuiCheckProjectFile;
  nuiLabel* mpNuiCheckProjectFile;
  bool mNuiCheckTools;
  nuiLabel* mpNuiCheckTools;
  bool mNuiCheckTemplate;
  nuiLabel* mpNuiCheckTemplate;
  nuiTimer* mpTimer;

  nuiEventSink<ProjectGenerator> mEventSink;
};