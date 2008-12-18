/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiInit.h"
#include "MainWindow.h"
#include "nuiApplication.h"

#include "nuiHBox.h"
#include "nuiVBox.h"
#include "nuiList.h"
#include "nuiLabel.h"
#include "nuiPane.h"

#include "ElementDesc.h"
#include "ElementEditor.h"

#include "nuiLabelRenamer.h"
#include "nuiDialog.h"
#include "Yapuka.h"
#include "ProjectGenerator.h"
#include "nuiCSS.h"


using namespace std;


/*
 * MainWindow
 */

MainWindow::MainWindow(const nglContextInfo& rContextInfo, const nglWindowInfo& rInfo )
  : nuiMainWindow(rContextInfo, rInfo, NULL, nglPath((_T("../data")))),
	mWinSink(this)
{
  //nuiTopLevel::EnablePartialRedraw(false);
  SetDebugMode(true);

	InitAttributes();	
}



void MainWindow::InitAttributes()
{
  mpAttributeMouseCoord = new nuiAttribute<nuiPoint>
  (nglString(_T("Mouse Coordinates")), nuiUnitPixels,
   nuiAttribute<nuiPoint>::GetterDelegate(this, &MainWindow::GetMouseCoord), 
   nuiAttribute<nuiPoint>::SetterDelegate(this, &MainWindow::SetMouseCoord));
  mAttributeMouseCoord = nuiAttribBase(this, mpAttributeMouseCoord);
}



MainWindow::~MainWindow()
{
  delete mpAttributeMouseCoord;
}


bool MainWindow::LoadCSS(const nglPath& rPath)
{
  nglIStream* pF = rPath.OpenRead();
  if (!pF)
  {
    NGL_OUT(_T("Unable to open CSS source file '%ls'\n"), rPath.GetChars());
    return false;
  }
  
  nuiCSS* pCSS = new nuiCSS();
  bool res = pCSS->Load(*pF, rPath);
  
  if (res)
  {
    SetCSS(pCSS);
    return true;
  }
  
  NGL_OUT(_T("%ls\n"), pCSS->GetErrorString().GetChars());
  
  delete pCSS;
  return false;
}

void MainWindow::OnCreation()
{
  LoadCSS(_T("rsrc:/css/yapuka.css"));
  
  // Main box [menus + buttons, main area]
  nuiVBox* pVBox = new nuiVBox(2);
  pVBox->SetExpand(nuiExpandShrinkAndGrow);
  AddChild(pVBox);
  
  // Buttons box:
  nuiHBox* pButtonBox = new nuiHBox();
  pButtonBox->SetExpand(nuiExpandShrinkAndGrow);
  pButtonBox->SetDecoration(GetDecorationManager()->Get(DECO_TOOLBAR), eDecorationBorder);
  pVBox->SetCell(0, pButtonBox);
  pVBox->SetCellExpand(0, nuiExpandFixed);

  // Main Buttons
  nuiButton* pLoad = new nuiButton(_T("Load project"));
  pButtonBox->AddCell(pLoad,nuiCenter);
  
  nuiButton* pSave = new nuiButton(_T("Save project"));
  pButtonBox->AddCell(pSave,nuiCenter);
  
  nuiButton* pFrameEditor = new nuiButton(_T("New Frame"));
  pButtonBox->AddCell(pFrameEditor,nuiCenter);

  nuiButton* pWidgetEditor = new nuiButton(_T("New Widget"));
  pButtonBox->AddCell(pWidgetEditor,nuiCenter);

  nuiButton* pProjectCreator = new nuiButton(_T("Create nui Project"));
  pButtonBox->AddCell(pProjectCreator,nuiCenter);

  
  
  // Splitter:
  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  pVBox->SetCell(1, pSplitter);
  pVBox->SetCellExpand(1, nuiExpandShrinkAndGrow);

  nuiPane* pLeftPane = new nuiPane(nuiColor(210, 215, 255));

  nuiScrollView* pListSV = new nuiScrollView();
  pLeftPane->AddChild(pListSV);
  pSplitter->AddChild(pLeftPane);
  
	
	
  nuiTreeNode* pMainTree = new nuiTreeNode(_T("Elements"), true, false);
	mpFrameTree = new nuiTreeNode(_T("Frames"), true, false);
	mpWidgetTree = new nuiTreeNode(_T("Widgets"), true, false);
	pMainTree->AddChild(mpFrameTree);
	pMainTree->AddChild(mpWidgetTree);
	
	mpElementTree = new nuiTreeView(pMainTree);
  pListSV->AddChild(mpElementTree);
  
  mpEditorContainer = new nuiSimpleContainer();
  pSplitter->AddChild(mpEditorContainer);
  
  mWinSink.Connect(pLoad->Activated, &MainWindow::OnLoad);
  mWinSink.Connect(pSave->Activated, &MainWindow::OnSave);
  mWinSink.Connect(pFrameEditor->Activated, &MainWindow::OnNewFrame);
  mWinSink.Connect(pWidgetEditor->Activated, &MainWindow::OnNewWidget);
  mWinSink.Connect(pProjectCreator->Activated, &MainWindow::OnCreateProject);

  mWinSink.Connect(mpElementTree->SelectionChanged, &MainWindow::OnElementSelected);
  mWinSink.Connect(mpElementTree->Activated, &MainWindow::OnElementActivated);
  
  UpdateList();
  
}


void MainWindow::OnClose()
{
  App->Quit();
}


bool MainWindow::OnSave(const nuiEvent& rEvent)
{
  nglPath path(ePathUserDocuments);
  nuiPane* pEditPane = new nuiPane(nuiColor(255, 255, 255, 192), nuiColor(0, 0, 0, 192));
  nuiEditLine* pEntry = new nuiEditLine();
  pEditPane->AddChild(pEntry);
  nuiPane* pPane = new nuiPane();
  pPane->SetCurve(8);
  nuiFileSelector* pFS = new nuiFileSelector(path, _T("*"), pEntry, nuiFileSelector::eColumn);
  
  nuiVBox* pBox = new nuiVBox();
  pPane->AddChild(pFS);
  pBox->AddCell(pPane);
  pBox->AddCell(pEditPane);

  pPane->SetUserSize(GetWidth() * .8, GetHeight() * .8);
  pEditPane->SetUserSize(GetWidth() * .8, 20);
  pEditPane->SetCurve(2);
  pEditPane->SetBorder(5, 5);
  
  //pBox->SetExpand(nuiExpandShrinkAndGrow);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pBox->SetCellExpand(1, nuiExpandFixed);

  nuiDialog* pDialog = new nuiDialog(this);
  pDialog->InitDialog(_T("Save yapuka project"), NULL, nuiDialog::eDialogButtonOk | nuiDialog::eDialogButtonCancel);
  pDialog->SetContents(pBox, nuiCenter);
  pDialog->SetDefaultPos();
  pDialog->SetToken(new nuiToken<nuiFileSelector*>(pFS));
  
  mWinSink.Connect(pDialog->DialogDone, &MainWindow::OnSaved, pDialog);

  return false;
}

bool MainWindow::OnSaved(const nuiEvent& rEvent)
{
  nuiDialog* pDialog = (nuiDialog*)rEvent.mpUser;
  nuiFileSelector* pFS = ((nuiToken<nuiFileSelector*>*)pDialog->GetToken())->Token;
  
  if (pDialog->GetResult() == nuiDialog::eDialogAccepted)
  {
    nuiXML xml(_T("yapuka"));
    
    ElementDescList::iterator it = mElements.begin();
    ElementDescList::iterator end = mElements.end();
    while (it != end)
    {
      ElementDesc* pElement = *it;
      pElement->CommitChanges();
      
      const nuiXMLNode* pXML = pElement->GetXML();
      if (pXML)
      {
        nuiXMLNode* pNode = pXML->Clone(&xml);
        printf("Saving %ls\n", pElement->GetName().GetChars());
        pNode->SetAttribute(_T("Name"), pElement->GetName());
      }
      ++it;
    }
    
    nglString Dump(xml.Dump());
    
    NGL_OUT(_T("\n%ls\n"), Dump.GetChars());
 
    nglPath path(pFS->GetPath());
    if (path.GetExtension().IsEmpty())
    {
      path = (path.GetPathName() + nglString(_T(".ypk")));
    }
    
    nglFile file(path, eFileWrite);
    
    file.Write(Dump, eUTF8);
  }
  
  return false;
}




bool MainWindow::OnLoad(const nuiEvent& rEvent)
{
  nglPath path(ePathUserDocuments);
  nuiPane* pEditPane = new nuiPane(nuiColor(255, 255, 255, 192), nuiColor(0, 0, 0, 192));
  nuiEditLine* pEntry = new nuiEditLine();
  pEditPane->AddChild(pEntry);
  nuiPane* pPane = new nuiPane();
  pPane->SetCurve(8);
  nuiFileSelector* pFS = new nuiFileSelector(path, _T("*"), pEntry, nuiFileSelector::eColumn);
  
  nuiVBox* pBox = new nuiVBox();
  pPane->AddChild(pFS);
  pBox->AddCell(pPane);
  pBox->AddCell(pEditPane);
  
  pPane->SetUserSize(GetWidth() * .8, GetHeight() * .8);
  pEditPane->SetUserSize(GetWidth() * .8, 20);
  pEditPane->SetCurve(2);
  pEditPane->SetBorder(5, 5);
  
  //pBox->SetExpand(nuiExpandShrinkAndGrow);
  pBox->SetCellExpand(0, nuiExpandShrinkAndGrow);
  pBox->SetCellExpand(1, nuiExpandFixed);
  
  nuiDialog* pDialog = new nuiDialog(this);
  pDialog->InitDialog(_T("Load yapuka project"), NULL, nuiDialog::eDialogButtonOk | nuiDialog::eDialogButtonCancel);
  pDialog->SetContents(pBox, nuiCenter);
  pDialog->SetDefaultPos();
  pDialog->SetToken(new nuiToken<nuiFileSelector*>(pFS));
  
  mWinSink.Connect(pDialog->DialogDone, &MainWindow::OnLoaded, pDialog);
  
  return false;
}

bool MainWindow::OnLoaded(const nuiEvent& rEvent)
{
  nuiDialog* pDialog = (nuiDialog*)rEvent.mpUser;
  nuiFileSelector* pFS = ((nuiToken<nuiFileSelector*>*)pDialog->GetToken())->Token;
  
  if (pDialog->GetResult() == nuiDialog::eDialogAccepted)
  {
    nglPath path(pFS->GetPath());
    nglIFile file(path);
 
    nuiXML XML("yapuka");
    if (!XML.Load(file))
      return false;

    for (uint32 i = 0; i < XML.GetChildrenCount(); i++)
    {
      nuiXMLNode* pNode = XML.GetChild(i);
      
      ElementDesc* pDesc = new ElementDesc();
      const nglString& rName = pNode->GetName();
      printf("Load Element: %ls\n", rName.GetChars());
      if (rName == _T("nuiFrame"))
      {
        pDesc->SetType(eElementFrame);
      }
      else
      {
        pDesc->SetType(eElementWidget);
      }
      
      pDesc->SetName(pNode->GetAttribute(_T("Name")));
      pDesc->SetXML(pNode->Clone());
      mElements.push_back(pDesc);
    }
    
    UpdateList();
    if (!mElements.empty())
			mpElementTree->Select(mpLastElement, true);
  }
  
  return false;
}



bool MainWindow::OnNewFrame(const nuiEvent& rEvent)
{
  ElementDesc* pDesc = new ElementDesc();
  pDesc->SetType(eElementFrame);
  pDesc->SetName(_T("unnamed frame"));
  mElements.push_back(pDesc);
  UpdateList();
  if (!mElements.empty())
    mpElementTree->Select(mpLastElement, true);

  return false;
}

bool MainWindow::OnNewWidget(const nuiEvent& rEvent)
{
  ElementDesc* pDesc = new ElementDesc();
  pDesc->SetType(eElementWidget);
  pDesc->SetName(_T("unnamed widget"));
  mElements.push_back(pDesc);
  UpdateList();
  if (!mElements.empty())
    mpElementTree->Select(mpLastElement, true);

  return false;
}

void MainWindow::UpdateList()
{
  mpFrameTree->Clear();
  mpWidgetTree->Clear();

  if (mElements.empty())
  {
    nuiTreeNode* pNode = new nuiTreeNode(_T("Empty"));
    mpFrameTree->AddChild(pNode);
		pNode = new nuiTreeNode(_T("Empty"));
    mpWidgetTree->AddChild(pNode);
  }
  else
  {
    uint nbFrames=0;
    uint nbWidgets=0;
    ElementDescList::iterator it = mElements.begin();
    ElementDescList::iterator end = mElements.end();
    while (it != end)
    {
      ElementDesc* pElement = *it;
      nuiTreeNode* pNode = new nuiTreeNode(pElement->GetName());
			mpLastElement = pNode;
      pNode->SetToken(new nuiToken<ElementDesc*>(pElement));
			switch (pElement->GetType())
			{
			case eElementFrame:
				mpFrameTree->AddChild(pNode);
        nbFrames++;
				break;
			case eElementWidget:
				mpWidgetTree->AddChild(pNode);
        nbWidgets++;
				break;
			}


      ++it;
    }
    
    if (!nbFrames)
     mpFrameTree->AddChild(new nuiTreeNode(_T("Empty")));

    if (!nbWidgets)
     mpWidgetTree->AddChild(new nuiTreeNode(_T("Empty")));
      
  }
  
  if (mElements.empty())
  {
    ClearEditor();
  }
}

void MainWindow::ClearEditor()
{
  nuiLabel* pLabel = new nuiLabel(_T("No editor"), nuiFont::GetFont(36));
  pLabel->SetPosition(nuiCenter);
  pLabel->SetTextColor(nuiColor(_T("lightgray")));
  mpEditorContainer->AddChild(pLabel);
}

bool MainWindow::OnElementSelected(const nuiEvent& rEvent)
{
  nuiTreeNode* pNode = mpElementTree->GetSelectedNode();
  if (pNode)
  {
    mpEditorContainer->Clear();
    nuiToken<ElementDesc*>* pToken = (nuiToken<ElementDesc*>*) pNode->GetToken();
    
		if (!pToken)
			return false;
		
		ElementDesc* pDesc = pToken->Token;
    NGL_ASSERT(pDesc);
    ElementEditor* pEditor = new ElementEditor(pDesc);
    mpEditorContainer->AddChild(pEditor);
  }
  else
  {
    ClearEditor();
  }
  
  return false;
}

bool MainWindow::OnElementActivated(const nuiEvent& rEvent)
{
  nuiTreeNode* pNode = mpElementTree->GetSelectedNode();
  if (pNode)
	{
    nuiToken<ElementDesc*>* pToken = (nuiToken<ElementDesc*>*) pNode->GetToken();
		
		if (!pToken)
			return false;

		// take care about that... be sure it's a nuiLabel
		nuiLabel* pLabel = (nuiLabel*)pNode->GetElement();
		
    nuiLabelRenamer* pRenamer = new nuiLabelRenamer(pLabel);
    pRenamer->SetToken(new nuiToken<ElementDesc*>(pToken->Token));
    mWinSink.Connect(pRenamer->Renamed, &MainWindow::OnElementRenamed, pRenamer);
  }
  
  return false;
}

bool MainWindow::OnElementRenamed(const nuiEvent& rEvent)
{
  nuiLabelRenamer* pItem = (nuiLabelRenamer*)rEvent.mpUser;
  NGL_ASSERT(pItem);
  
  nuiToken<ElementDesc*>* pToken = (nuiToken<ElementDesc*>*)pItem->GetToken();
  
  ElementDesc* pDesc = pToken->Token;
  pDesc->SetName(pItem->GetText());
  
  OnElementSelected(rEvent);
  return true;
}


bool MainWindow::OnCreateProject(const nuiEvent& rEvent)
{
  new ProjectCreator(this);
	
	return true;
}




// properties **************************************************************


nuiPoint MainWindow::GetMouseCoord()
{
	return mMouseCoord;
}

void MainWindow::SetMouseCoord(nuiPoint point)
{
	mMouseCoord = point;
}





