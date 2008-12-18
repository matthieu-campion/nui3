/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"

#include "ElementDesc.h"

class ElementEditorGui : public nuiSimpleContainer
{
public:
  ElementEditorGui();

  virtual ~ElementEditorGui();
  
  void ClearEditor();  
  
protected:

  bool OnLoad(const nuiEvent& rEvent);
  bool OnLoaded(const nuiEvent& rEvent);
  
  bool OnSave(const nuiEvent& rEvent);
  bool OnSaved(const nuiEvent& rEvent);
  
  bool OnNewFrame(const nuiEvent& rEvent);
	bool OnNewWidget(const nuiEvent& rEvent);
  
  bool OnElementSelected(const nuiEvent& rEvent);
  bool OnElementActivated(const nuiEvent& rEvent);
  bool OnElementRenamed(const nuiEvent& rEvent);

  
  
  void UpdateList();

private:
  
	nuiTreeView* mpElementTree;
	nuiTreeNode* mpLastElement;
	nuiTreeNode* mpFrameTree;
	nuiTreeNode* mpWidgetTree;
	
  typedef std::list<ElementDesc*> ElementDescList;
  ElementDescList mElements;
  
  nuiSimpleContainer* mpEditorContainer;
  
  
  nuiEventSink<ElementEditorGui> mEventSink;

};


