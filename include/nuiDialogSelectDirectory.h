/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDialog.h"
#include "nuiFileSelector.h"


#pragma once

class nuiDialogSelectDirectory : public nuiDialog
{
public:
  nuiDialogSelectDirectory(nuiMainWindow* pParent, const nglString& rTitle, const nglPath& rPath, const nglPath& rRootPath, nuiSize Left=0.f, nuiSize Top=0.f, nuiSize Width=0.f, nuiSize Height=0.f);
  virtual ~nuiDialogSelectDirectory();
  
  const nglPath& GetSelectedDirectory();
  const nglPath& GetRootPath();
  
  nuiSimpleEventSource<0> DirectorySelected;
  
private:
  
  bool OnSelectorOK(const nuiEvent& rEvent);
  bool OnDialogDone(const nuiEvent& rEvent);
  
  
  
  bool OnCreateNewFolder(const nuiEvent& rEvent);
  bool OnCreateNewFolderDone(const nuiEvent& rEvent);
  
  
  
  nuiMainWindow* mpParent;
  nuiSimpleContainer* mpContainer;
  nuiFileSelector* mpSelector;
  
  nuiDialog* mpCreateDialog;
  nuiEditLine* mpCreateEditLine;
  
  nglPath mPath;
  nglPath mRootPath;
  
  nuiEventSink<nuiDialogSelectDirectory> mEventSink;
};