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

  void OnLoad(const nuiEvent& rEvent);
  void OnLoaded(const nuiEvent& rEvent);
  
  void OnSave(const nuiEvent& rEvent);
  void OnSaved(const nuiEvent& rEvent);
  
  void OnNewFrame(const nuiEvent& rEvent);
	void OnNewWidget(const nuiEvent& rEvent);
  
  void OnElementSelected(const nuiEvent& rEvent);
  void OnElementActivated(const nuiEvent& rEvent);
  void OnElementRenamed(const nuiEvent& rEvent);

  
  
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


