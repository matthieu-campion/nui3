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
  void ClearEditor();


	// attributes ***********************************
	nuiAttrib<nuiPoint> mAttributeMouseCoord;


protected:

	nuiAttribute<nuiPoint>* mpAttributeMouseCoord;

  bool OnLoad(const nuiEvent& rEvent);
  bool OnLoaded(const nuiEvent& rEvent);
  
  bool OnSave(const nuiEvent& rEvent);
  bool OnSaved(const nuiEvent& rEvent);
  
  bool OnNewFrame(const nuiEvent& rEvent);
	bool OnNewWidget(const nuiEvent& rEvent);
  
  bool OnCreateProject(const nuiEvent& rEvent);

  bool OnElementSelected(const nuiEvent& rEvent);
  bool OnElementActivated(const nuiEvent& rEvent);
  bool OnElementRenamed(const nuiEvent& rEvent);

	// attributes ***********************************
	nuiPoint GetMouseCoord();
	void SetMouseCoord(nuiPoint point);


  
  void UpdateList();

	nuiTreeView* mpElementTree;
	nuiTreeNode* mpLastElement;
	nuiTreeNode* mpFrameTree;
	nuiTreeNode* mpWidgetTree;
	
  
  nuiEventSink<MainWindow> mWinSink;
  
  typedef std::list<ElementDesc*> ElementDescList;
  ElementDescList mElements;
  
  nuiSimpleContainer* mpEditorContainer;

	
	// attributes ***********************************
	nuiPoint mMouseCoord;

	

};

#endif
