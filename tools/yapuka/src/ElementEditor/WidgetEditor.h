/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiSimpleContainer.h"
#include "ElementEditor/ElementDesc.h"
#include "Tools/ElementInspector.h"
#include "nuiColumnTreeView.h"
#include "nuiDialog.h"
#include "Tools/ToolPaneAttributes.h"




class WidgetInfo
{
public:

	WidgetInfo(nuiWidget* pWidget, bool isContainer);
	~WidgetInfo();

	nuiWidget* GetWidget();
	nuiSimpleContainer* GetContainer();
	bool IsContainer();

private:

	nuiWidget* mpWidget;
	nuiSimpleContainer* mpContainer;
	bool mIsContainer;

};





typedef nuiFastDelegate1<nuiWidget*> WidgetDelegate;


class WidgetEditor : public nuiSimpleContainer
{
public:
 
  WidgetEditor(ElementDesc* pDesc, ElementInspector* pInspector);
  void BuildTreeView(const nuiXMLNode* pXmlNode, nuiTreeNode* pParent);
  virtual ~WidgetEditor();
	
	void CommitChanges();

	
protected : 

		bool OnSelectionChanged(const nuiEvent& rEvent);
		bool OnActivated(const nuiEvent& rEvent);
		bool OnDeleteActivated(const nuiEvent& rEvent);
		bool DialogCantDo();
	
private : 

	void FillWidgetDelegates();
	void RunWidgetDelegate(const nglString& classname, nuiWidget* pWidget);

	void CommitWidgetChanges(nuiTreeNode* pTreeNode, nuiXMLNode* pXmlParent);
  bool OnCommitChanges(const nuiEvent& rEvent);



	nuiDialog* mpDialog;

	nuiEventSink<WidgetEditor> mEventSink;
	
	nuiList* mpContainerList;
	nuiList* mpWidgetList;

  ElementDesc* mpDesc;
	ElementInspector* mpInspector;
	ToolPaneAttributes* mpCurrentToolpane;
	
	nuiTreeNode* mpTreeRoot;
	nuiColumnTreeView* mpTreeMain;
	
	nuiSimpleContainer* mpTreeView;
	nuiSimpleContainer* mpWidgetView;
	

	static std::map<nglString, WidgetDelegate> mWidgetDelegates;
};
