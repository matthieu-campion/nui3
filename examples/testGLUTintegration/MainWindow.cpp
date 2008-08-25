
#include "nui.h"
#include "nuiVBox.h"
#include "nuiHBox.h"
#include "MainWindow.h"
#include "nuiComboBox.h"
#include "nuiLabel.h"
#include "nuiScrollView.h"
#include "nuiTitledPane.h"
#include "nuiFolderPane.h"
#include "nuiList.h"
#include "nuiTabView.h"
#include "nuiFixed.h"
#include "nuiLabel.h"
#include "nuiPane.h"
#include "nuiScrollView.h"
#include "nuiFixed.h"
#include "nuiAttributeEditor.h"
#include "nuiColorSelector.h"
#include "nuiRectView.h"
#include "nuiWindowManager.h"
#include "nuiTreeView.h"
#include "nuiGrid.h"
#include "nglConsole.h"
#include "nuiPopupTree.h"

/*
 * MainWindow
 */
uint Height=0;
uint Width=0;
MainWindow::MainWindow()
:nglApplication(),mSink(this)
{
	mSink.SetTarget(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnInit()
{
}

void MainWindow::SetSize( uint pH, uint pW)
{
	Height=pH;
	Width=pW;
}
void MainWindow::OnCreation()
{
  nuiImage* pPage = new nuiImage(_T("../data/nui.png"));//logo nui
  pPage->SetUseAlpha(true); 
  
  nuiPane* ColorP = new nuiPane(nuiColor(10, 90, 255,140));//couleur de fond et panel principal
  
  nuiScrollView* pbScrollView = new nuiScrollView();//scroll principal
  
  pbScrollView->AddChild(ColorP);
  
  nuiVBox* pVBox = new nuiVBox(3);//layout principal "vetical"
  ColorP->AddChild(pVBox);
 
  nuiHBox* pHBox = new nuiHBox(3);
  pVBox->SetCell(0,pHBox );
  pVBox->SetCellExpand(0, nuiExpandFixed);
  pHBox->SetExpand(nuiExpandShrinkAndGrow);
 //TODO
  


  /*nuiTreeNodePtr pNode = NULL;
  nuiTreeNodePtr pTree = new nuiTreeNode(_T("Root"), true);
  for (uint i = 0; i < 10; i++)
  {
    nglString str;
    str.Format(_T("Choice #%d"), i);
    pNode = new nuiTreeNode(str, true);
    pTree->AddChild(pNode);
  }

  for (uint i = 0; i < 10; i++)
  {
    nglString str;
    str.Format(_T("Choice %c"), (char)(_T('A') + i));
    nuiTreeNode* pNode2 = new nuiTreeNode(str, true);
    pNode->AddChild(pNode2);
  }
  nuiPopupTree* pCombo = new nuiPopupTree(ColorP,pTree,Ico,true);*/
  
/***************************************************************************************************/
  /**********************boutton de fermetur*************************/
    nuiButton* Close = new nuiButton(nglString("Quit"));
  mSink.Connect(Close->Activated,&MainWindow::OnCloset );
  pHBox->SetCell(0,Close);
  pHBox->SetCellPixels(0,50);
  pHBox->SetCellSpacing(0,10);
  //TODO fermetur pr�sentable    

 /***************************************************************************************************/
  /**********************Liste des widget*************************/
  
  nuiHBox* pHBox1 = new nuiHBox(3);
  pVBox->SetCell(1,pHBox1);
  uint hlll = Height-50;
  pVBox->SetCellPixels(1, hlll);
  
  

  std::list<nuiWidgetDesc> classNames;
  nuiBuilder::Get().GetClassList(classNames);//recuperation de la list disponible
  std::list<nuiWidgetDesc>::iterator it;
  
  
  nuiPane* WidgetListe = new nuiPane(nuiColor(215, 215, 215, 5));
  
  
  nuiVBox* VList = new nuiVBox(2);
  VList->SetCell(0,WidgetListe);
  pHBox1->SetCell(0,VList);
  uint hll = Width/6;
  
  pHBox1->SetCellPixels(0, hll);
  
  nuiScrollView* pScrollWidgetList = new nuiScrollView();//ajout d'un scroll
  mpList = new nuiList();
  mSink.Connect(mpList->Activated, &MainWindow::NewWidget);//connection pour la cr�ation d'un nouveau widget quand le widget est Dropper elle se cr�� dans son conteneur

  
  pScrollWidgetList->AddChild(mpList);
  
  
  nglString name = _T("new window");
  nuiLabel* pLabel = new nuiLabel(name);
  mpList->AddChild(pLabel);

  for (it = classNames.begin(); it != classNames.end(); ++it)
 	{
		nglString name = it->GetClassName();
		nuiLabel* pLabel = new nuiLabel(name);
		mpList->AddChild(pLabel);
	}
  
  WidgetListe->AddChild(pScrollWidgetList);

  /***************************************************************************************************/
  /**********************Work Station*************************/
  //tableaux principaux
  WorkStationPane = new nuiWindowManager();
  nuiPane* WysiwygPane = new nuiPane(nuiColor(255, 155, 255, 192), nuiColor(0, 0, 0, 192));
  nuiPane* LogicalPane = new nuiPane(nuiColor(255, 255, 155, 192), nuiColor(0, 0, 0, 192));
  
  //exemple de presentation de la vue logique "un rectangle"
  nuiRect Ico = nuiRect(0.f, 0.f, 150.f, 150.f);
  nuiRectView* f = new nuiRectView(Ico);
  LogicalPane->AddChild(f);
  
  //onglet
  nuiSimpleContainer* WorkStationTitle = new nuiSimpleContainer();
  nuiSimpleContainer* WysiwygTitle = new nuiSimpleContainer();
  nuiSimpleContainer* LogicalTitle = new nuiSimpleContainer();
  

  nuiLabel* WorkStationLabel = new nuiLabel(_T("Work Station"));
  nuiLabel* WysiwygnuiLabel = new nuiLabel(_T("Wysiwyg"));
  nuiLabel* LogicalnuiLabel = new nuiLabel(_T("Logical view"));
  
  WorkStationTitle->AddChild(WorkStationLabel);
  WysiwygTitle->AddChild(WysiwygnuiLabel);
  LogicalTitle->AddChild(LogicalnuiLabel);

  //d�core des onglet
  
  nuiLabel* WLabel = new nuiLabel(_T("Work Station"));
  nuiLabel* WyLabel = new nuiLabel(_T("Wysiwyg"));
  nuiLabel* LLabel = new nuiLabel(_T("Logical view"));
  
  WLabel->SetFont(nuiFont::GetFont(18));
  WLabel->SetOrientation(nuiVertical);
  
  WyLabel->SetFont(nuiFont::GetFont(18));
  WyLabel->SetOrientation(nuiVertical);
  
  LLabel->SetFont(nuiFont::GetFont(18));
  LLabel->SetOrientation(nuiVertical);
  
  
 
  WorkStationPane->AddChild(WLabel);
  WysiwygPane->AddChild(WyLabel);
  LogicalPane->AddChild(LLabel);



  nuiPosition tabposition = nuiTop;
  nuiTabView* pTabView = new nuiTabView(tabposition);

  nuiScrollView* pScrollView = new nuiScrollView();

  pScrollView->AddChild(pTabView);

  pTabView->AddTab(WorkStationPane, WorkStationTitle);
  pTabView->AddTab(WysiwygPane, WysiwygTitle);
  pTabView->AddTab(LogicalPane, LogicalTitle);
  pTabView->SelectTab(0);

  nuiVBox* VWork = new nuiVBox(2);
  VWork->SetCell(0,pScrollView );
  VWork->SetCellPixels(0, Height-50);
  pHBox1->SetCell(1,VWork );
  uint hllll = (Width/6)*4;
  pHBox1->SetCellPixels(1, hllll);

  /**************************************************************************************************************/
  /****************************cr�ation de la fen�tre qui va �tre construite*****************************/
  ViewWinP = new nuiWindow(nuiRect(0, 0, (Width/6)*4, Height-60), nuiWindow::NoFlag, _T("windows 0"));
  perColor = new nuiPane();
  ViewWinP->AddChild(perColor);
  WorkStationPane->AddChild(ViewWinP);
  
 //TODO 
/**************************************************************************************************************/
  /****************************l'arbre des widget et frame cr��*****************************/
 

  nuiPane* pLeftPane = new nuiPane(nuiColor(255, 255, 255, 192), nuiColor(0, 0, 0, 192));
  
  nuiScrollView* pListSV = new nuiScrollView(true,false);
  
  pLeftPane->AddChild(pListSV);
  
  nuiVBox* VTree = new nuiVBox(3);

  VTree->SetCell(0,pLeftPane);
  VTree->SetCellPixels(0, 200);
  

  pHBox1->SetCell(2,VTree);
  uint hl = Width/6;
  pHBox1->SetCellPixels(2, hl);
  
	
	
  nuiTreeNode* pMainTree = new nuiTreeNode(_T("Elements"), true, false);
  mpFrameTree = new nuiTreeNode(_T("Frames"), true, false);
  mpWidgetTree = new nuiTreeNode(_T("Widgets"), true, false);
	
  pMainTree->AddChild(mpFrameTree);
  pMainTree->AddChild(mpWidgetTree);
	
  mpElementTree = new nuiTreeView(pMainTree);
  mSink.Connect(mpElementTree->Activated, &MainWindow::TreeUpdate);
  pListSV->AddChild(mpElementTree);

  /*********************************************************************************************/
  /*************************color selector*********************************************/
  pSelector = new nuiColorSelector(); 
  VTree->SetCell(1,pSelector);
  const nuiEvent& rEvent=NULL;
  nuiButton* b = new nuiButton();
  
  VTree->SetCell(2,b);
  mSink.Connect(b->Activated, &MainWindow::ColorChanged);
  
  
}
nglString SelectedWin = NULL;
bool MainWindow::TreeUpdate(const nuiEvent& rEvent)
{
   nuiTreeNode* w = mpElementTree->GetSelectedNode();
   nuiLabel* pLabel = (nuiLabel*)w->GetElement();
   SelectedWin=pLabel->GetText();
   return true;
}
uint NBFrame=1;
uint i=0;
uint j=0;

bool MainWindow::NewWidget(const nuiEvent& rEvent)
{
	  h = new nuiGrid(1,1);
	  nuiRect Ie = nuiRect(10.f, 30.f, 20.f, 20.f);
	  nuiRectView* rec = new nuiRectView(Ie);
	  nuiLabel* pLabel = (nuiLabel*)mpList->GetSelected();
	  nglString classname = pLabel->GetText();
	  nuiRect Icfo = nuiRect(0.f, 0.f, 150.f, 150.f);
	  ff = new nuiRectView(Icfo);
	  
	  //TODO ugly, got to do something
//	  mSink.Connect(ff->Activated, &MainWindow::WidgetMove);

	  //nuiDrawContext* context = new nuiDrawContext(Icfo);

	  if(classname==_T("new window"))
	  {
		  nglString str;
		  str.Format(_T("windows %d"), NBFrame);
		  nuiWindow* ViewWin = new nuiWindow(nuiRect(10, 10, 300, 200), nuiWindow::NoFlag, str);
		  NBFrame++;
		  nuiTreeNode* pNewNode = new nuiTreeNode(str);
		  mpFrameTree->AddChild(pNewNode);	
		  WorkStationPane->AddChild(ViewWin);

		  return true;
	  }
	  
	  nuiWidget* pWidget = nuiBuilder::Get().CreateWidget(classname);
	  nuiTreeNode* pNewNode = new nuiTreeNode(classname);
	  
	  
	  h->SetCell(i,j,pWidget);

	  
	 
	  
	  mpWidgetTree->AddChild(pNewNode);	
	  //WorkStationPane->AddChild(rec);
	  ViewWinP->AddChild(h);
	  ViewWinP->AddChild(ff);
	  
	  return true;
}
bool MainWindow::ColorChanged(const nuiEvent& rEvent)
{
  nuiColor vari = pSelector->GetCurrentColor();
  perColor->SetFillColor(vari);
  return true;
}
bool MainWindow::WidgetMove(const nuiEvent& rEvent)
{ 

  nuiRect recc=ff->GetCurrentRect();
  h->SetRowSpacing(0,recc.Top());
  h->SetRowPixels(0,recc.GetHeight());
  h->SetColumnSpacing(0,recc.Left());
  h->SetColumnPixels(0,recc.GetWidth());

  return true;
}
  
bool MainWindow::OnCloset(const nuiEvent& rEvent)
{
 OnClose();
 return true;
}

void MainWindow::OnClose()
{
  App->Quit();

}
