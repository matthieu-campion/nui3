/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "ElementEditor/WidgetEditor.h"
#include "Main/Yapuka.h"
#include "Main/MainWindow.h"
#include "ElementEditor/WidgetDelegates.h"
#include "nuiHBox.h"


WidgetInfo::WidgetInfo(nuiWidget* pWidget, bool isContainer)
{
	mIsContainer = isContainer;
	
	if (mIsContainer)
	{
		mpContainer = (nuiSimpleContainer*) dynamic_cast <nuiSimpleContainer*> (pWidget);
		mpWidget = NULL;
	}
	else
	{	
		mpWidget = pWidget;
		mpContainer = NULL;
	}
}
	
WidgetInfo::~WidgetInfo()
{

}

nuiWidget* WidgetInfo::GetWidget()
{
	return mpWidget;
}

nuiSimpleContainer* WidgetInfo::GetContainer()
{
	return mpContainer;
}

bool WidgetInfo::IsContainer()
{
	return mIsContainer;
}












WidgetEditor::WidgetEditor(ElementDesc* pDesc, ElementInspector* pInspector)
	: mEventSink(this)
{
	FillWidgetDelegates();

  mpDesc = pDesc;
	mpInspector = pInspector;
	mpCurrentToolpane = NULL;
	
	mEventSink.Connect(mpDesc->CommitChanges, &WidgetEditor::OnCommitChanges);


	std::list<nuiWidgetDesc> classNames;
	nuiBuilder::Get().GetClassList(classNames);
	std::list<nuiWidgetDesc>::iterator it;

	//
	// container list in toolpane inspector
	//
	nuiScrollView* pContainerScroll = new nuiScrollView(false, true);
	mpContainerList = new nuiList();
	pContainerScroll->AddChild(mpContainerList);
	mEventSink.Connect(mpContainerList->Activated, &WidgetEditor::OnActivated, (void*)mpContainerList);
	
	ToolPane* pContainerPane = new ToolPane(_T("Container List"), pContainerScroll);
	mpInspector->AddToolpane(pContainerPane, 250/* max height */);


	//
	// widgets list in toolpane inspector
	//
	nuiScrollView* pWidgetScroll = new nuiScrollView(false, true);
	mpWidgetList = new nuiList();
	pWidgetScroll->AddChild(mpWidgetList);
	mEventSink.Connect(mpWidgetList->Activated, &WidgetEditor::OnActivated, (void*)mpWidgetList);

	ToolPane* pWidgetPane = new ToolPane(_T("Widget List"), pWidgetScroll);
	mpInspector->AddToolpane(pWidgetPane, 250/* max height */);


	//
	// build toolpanes contents
	//
	for (it = classNames.begin(); it != classNames.end(); ++it)
	{
    const nuiWidgetDesc& desc = *it;
		nuiLabel* pLabel = new nuiLabel(desc.GetClassName());
    if (!desc.GetClassGroup().Compare(_T("Container")))
      mpContainerList->AddChild(pLabel);
    else
      mpWidgetList->AddChild(pLabel);
    
	}


	//
	// splitter : the tree view and the widget view
	//
  nuiSplitter* pSplitter = new nuiSplitter(nuiHorizontal, eModePixel);
  pSplitter->SetMasterChild(false);
	AddChild(pSplitter);
	
	mpTreeView = new nuiSimpleContainer();
	mpWidgetView = new nuiSimpleContainer();

	
	
	pSplitter->AddChild(mpTreeView);
	pSplitter->AddChild(mpWidgetView);
	
	
	mpTreeRoot = new nuiTreeNode(_T("unamed widget"));
	WidgetInfo* info  = new WidgetInfo(mpWidgetView, true);
	mpTreeRoot->SetToken(new nuiToken<WidgetInfo*>(info));
	
	mpTreeMain = new nuiColumnTreeView(mpTreeRoot);
	mpTreeMain->Select(mpTreeRoot, true);
	mEventSink.Connect(mpTreeMain->SelectionChanged, &WidgetEditor::OnSelectionChanged);
	mpTreeView->AddChild(mpTreeMain);
	


	// load the xml widget definition, if any
	const nuiXMLNode* xml = pDesc->GetXML();
	if (xml)
	{
    //#FIXME!! Serialization API is deprecated!
		//nuiXMLNode* child = xml->GetChild(0);
		nuiWidget* pWidget = NULL; //nuiCreateWidget(child);
		if (!pWidget)
			return;
			
		mpWidgetView->AddChild(pWidget);
    BuildTreeView(xml, mpTreeRoot);
    mpTreeMain->Select((nuiTreeNode*)mpTreeRoot->GetChild(0), true);
	}
	

	
}


//#FIXME
void WidgetEditor::BuildTreeView(const nuiXMLNode* pXmlNode, nuiTreeNode* pParent)
{

//
//  IteratorPtr pIt;
//  for (pIt = pCont->GetFirstChild(); pIt && pIt->IsValid(); pCont->GetNextChild(pIt))
//  {
//    nuiWidgetPtr pWidget = pIt->GetWidget();
//    if (!pWidget)
//      continue;
//      
//    nuiTreeNode* pNode = new nuiTreeNode(pWidget->GetProperty(_T("Class")));
//    
//    nuiSimpleContainer* pContainer = (nuiSimpleContainer*) dynamic_cast <nuiSimpleContainer*>(pWidget);
//    if (pContainer)
//    {
//      // store the relation (treenode->container)
//      WidgetInfo* info = new WidgetInfo(pWidget, true);
//      pNode->SetToken(new nuiToken<WidgetInfo*>(info));
//    }
//    
//    // no it's a simple widget
//    else
//    {
//      // store the relation (treenode->widget)
//      WidgetInfo* info = new WidgetInfo(pWidget, false);
//      pNode->SetToken(new nuiToken<WidgetInfo*>(info));
//    }
//    
//    
//    // add the new node to the tree	
//    pParent->AddChild(pNode);
//    
//    if (pContainer)
//      BuildTreeView(pContainer, pNode);
//
//  }
//  delete pIt;
//
  
}


WidgetEditor::~WidgetEditor()
{
	CommitChanges();
}




//
// a tree node has been selected from the tree : display the attribute editor
//
void WidgetEditor::OnSelectionChanged(const nuiEvent& rEvent)
{
  nuiWidget* pTargetedWidget=NULL;

	// get the selected tree node
	nuiTreeNode* pSelectedNode = mpTreeMain->GetSelectedNode();
	NGL_ASSERT(pSelectedNode);


	// get the associated container (or widget...)
	WidgetInfo* pSelectionInfo;
	bool res = nuiGetTokenValue<WidgetInfo*>(pSelectedNode->GetToken(), pSelectionInfo);
	NGL_ASSERT(res == true);
	
	// the associated widget may be a container or a simple widget
	nuiSimpleContainer* pSelectedContainer = pSelectionInfo->GetContainer();
	if (pSelectedContainer == NULL)
		pTargetedWidget = pSelectionInfo->GetWidget();
	else 
		pTargetedWidget = (nuiWidget*)pSelectedContainer;
    
    
    
    
		
	// create a toolpane with the name attribute of the selected widget as a title
	nuiAttrib<const nglString&> ClassAttribute = pTargetedWidget->GetAttribute(_T("Class"));
	NGL_ASSERT(ClassAttribute.IsValid());
	nuiAttrib<const nglString&> NameAttribute = pTargetedWidget->GetAttribute(_T("Name"));
	NGL_ASSERT(NameAttribute.IsValid());
  

//	nuiLabelAttribute* pLabel = new nuiLabelAttribute(pNameAttribute);


	// a button to delete the widget
	nuiButton* pBtn = new nuiButton(_T("delete widget"));
	mEventSink.Connect(pBtn->Activated, &WidgetEditor::OnDeleteActivated);

  std::list<nuiAttribBase> attributes;
  pTargetedWidget->GetSortedAttributes(attributes);
	ToolPaneAttributes* pToolpane = new ToolPaneAttributes(ClassAttribute.Get(), attributes, pBtn);
	
	// put the toolpane in the inspector
	if (mpCurrentToolpane)
		mpInspector->ReplaceToolpane(mpCurrentToolpane, pToolpane);
	else
		mpInspector->AddToolpane(pToolpane, 250);
		
	mpCurrentToolpane = pToolpane;
}





//
// the user requested a widget deletion
//
void WidgetEditor::OnDeleteActivated(const nuiEvent& rEvent)
{
	// get the selected tree node
	nuiTreeNode* pSelectedNode = mpTreeMain->GetSelectedNode();
	NGL_ASSERT(pSelectedNode);
	
	// get the associated container
	WidgetInfo* pSelectionInfo;
	bool res = nuiGetTokenValue<WidgetInfo*>(pSelectedNode->GetToken(), pSelectionInfo);
	NGL_ASSERT(res == true);
	
	// the associated widget may not be a container, it may be a simple widget
	nuiWidget* pSelectedWidget = (nuiWidget*)pSelectionInfo->GetContainer();
	if (pSelectedWidget == NULL)
	{
		pSelectedWidget = pSelectionInfo->GetWidget();
		NGL_ASSERT(pSelectedWidget);
	}
	
	
	// retrieve the tree node parent
	nuiTreeBase* pParent = pSelectedNode->GetParent();
	nuiTreeNode* pParentNode = (nuiTreeNode*) dynamic_cast <nuiTreeNode*> (pParent);
	NGL_ASSERT(pParentNode);
	
	// delete widget and childs
	pSelectedWidget->Trash();
	
	// delete tree node
	pParentNode->DelChild(pSelectedNode);

	
	// the parent becomes the new selected object
	mpTreeMain->Select(pParentNode, true);
	mpContainerList->DeselectAll();
	mpWidgetList->DeselectAll();
	
	rEvent.Cancel();
}




//
// a widget class has been activated in the list
//
void WidgetEditor::OnActivated(const nuiEvent& rEvent)
{
  nuiList* pList = (nuiList*)rEvent.mpUser;
  NGL_ASSERT(pList);
  
	// get the selected tree node
	nuiTreeNode* pSelectedNode = mpTreeMain->GetSelectedNode();
	NGL_ASSERT(pSelectedNode);

	// get the associated container
	WidgetInfo* pSelectionInfo;
	bool res = nuiGetTokenValue<WidgetInfo*>(pSelectedNode->GetToken(), pSelectionInfo);
	NGL_ASSERT(res == true);
	
	// the associated widget is not a container, it's a simple widget : stop the process
	nuiSimpleContainer* pSelectedContainer = pSelectionInfo->GetContainer();
	if (pSelectedContainer == NULL)
	{
    if (DialogCantDo())
      rEvent.Cancel();
    return;
  }


	// yes, it's a container. go on...
			

	// get the selected widget class
	nuiLabel* pLabel = (nuiLabel*)pList->GetSelected();
	nglString classname = pLabel->GetText();
	

  // create the new widget	
	nuiWidget* pWidget = nuiBuilder::Get().CreateWidget(classname);
	RunWidgetDelegate(classname, pWidget);

	// create the new tree node
	nuiTreeNode* pNewNode = new nuiTreeNode(classname);
	
	
	// is the new widget a Container or not?
	bool isContainer = false;
	nuiSimpleContainer* pContainer = (nuiSimpleContainer*) dynamic_cast <nuiSimpleContainer*> (pWidget);
	if (pContainer)
	{
		isContainer = true;
		
		// store the relation (treenode->container)
		WidgetInfo* info = new WidgetInfo(pWidget, true);
		pNewNode->SetToken(new nuiToken<WidgetInfo*>(info));
	}
	
	// no it's a simple widget
	else
	{
		isContainer = false;
		// store the relation (treenode->widget)
		WidgetInfo* info = new WidgetInfo(pWidget, false);
		pNewNode->SetToken(new nuiToken<WidgetInfo*>(info));
	}
	
	
	// add the new node to the tree	
	pSelectedNode->AddChild(pNewNode); 
	
	//add the new widget to the parent widget
	pSelectedContainer->AddChild(pWidget);

	
	// the new widget becomes the new selection by default
	mpTreeMain->SelectionChanged.Disable();
	mpTreeMain->SelectAll(false);
	mpTreeMain->SelectionChanged.Enable();
	mpTreeMain->Select(pNewNode, true);
}




bool WidgetEditor::DialogCantDo()
{
	MainWindow* pWin = GetMainWindow();
	
  mpDialog = new nuiDialog(pWin);
  nuiPane* pPane = new nuiPane(nuiColor(1.0f,1.0f,1.0f, 0.8f), nuiColor(0.0f, 0.0f, 0.0f, 0.8f));
  pPane->SetCurve(16);
  pPane->SetUserSize(pWin->GetWidth()*.35f, pWin->GetHeight() * .1f);

	pPane->AddChild(new nuiLabel(_T("The selected parent is not a container!\nYou can't add the selected widget!")));
	
  mpDialog->InitDialog(_T("Alors on me dit que non..."), NULL, nuiDialog::eDialogButtonOk);
  mpDialog->SetContents(pPane);
  mpDialog->SetDefaultPos();
	
	return true;
}



void WidgetEditor::RunWidgetDelegate(const nglString& classname, nuiWidget* pWidget)
{
	std::map<nglString, WidgetDelegate>::iterator it = mWidgetDelegates.find(classname);
	if (it == mWidgetDelegates.end())
		return;
		
	WidgetDelegate delegate = it->second;
	delegate(pWidget);
}

void WidgetEditor::FillWidgetDelegates()
{
	if (!mWidgetDelegates.empty())
		return;
	
	mWidgetDelegates[_T("nuiLabel")] = WidgetDelegateLabel;
	mWidgetDelegates[_T("nuiTitledPane")] = WidgetDelegateTitledPane;
	mWidgetDelegates[_T("nuiFolderPane")] = WidgetDelegateFolderPane;
}


void WidgetEditor::CommitWidgetChanges(nuiTreeNode* pTreeNode, nuiXMLNode* pXmlParent)
{

	// enum children nodes
	std::vector<nuiTreePtr> Children;
	pTreeNode->GetChildren(Children);
	for (uint i=0; i < (uint)Children.size(); i++)
	{
    nuiTreePtr pTreeItem = Children[i];

		nuiTreeNode* pChildNode = (nuiTreeNode*) dynamic_cast <nuiTreeNode*>(pTreeItem);
		NGL_ASSERT(pChildNode);

		//**************************

		nuiWidget* pWidget;

		// get the associated container (or widget...)
		WidgetInfo* pSelectionInfo;
		bool res = nuiGetTokenValue<WidgetInfo*>(pChildNode->GetToken(), pSelectionInfo);
		NGL_ASSERT(res == true);
		
		// the associated widget may be a a container or a simple widget
		nuiSimpleContainer* pSelectedContainer = pSelectionInfo->GetContainer();
		if (pSelectedContainer == NULL)
			pWidget = pSelectionInfo->GetWidget();
		else 
			pWidget = (nuiWidget*)pSelectedContainer;
			
			
		//**************************

		// xml serialize
    //#FIXME!!! Serialization API deprecated
		//nuiXMLNode* pXmlNode = pWidget->Serialize(pXmlParent);

		if (!pSelectionInfo->IsContainer())
			return;
		
		//CommitWidgetChanges(pChildNode, pXmlNode); // recurence
	
	}

}





void WidgetEditor::CommitChanges()
{
  if (mpWidgetView->GetChildrenCount() > 0)
  {
    nuiXMLNode* pNode = new nuiXMLNode(_T("widget"));
      
    CommitWidgetChanges(mpTreeRoot, pNode);
    
    pNode->SetAttribute(_T("Name"), mpDesc->GetName());
    mpDesc->SetXML(pNode);
  }
  else 
    mpDesc->SetXML(NULL);
}



void WidgetEditor::OnCommitChanges(const nuiEvent& rEvent)
{
  CommitChanges();
}



std::map<nglString, WidgetDelegate> WidgetEditor::mWidgetDelegates;
