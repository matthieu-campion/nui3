/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiWidgetInspector.h"
#include "nuiColumnTreeView.h"
#include "nuiTreeView.h"
#include "nuiSplitter.h"
#include "nuiGrid.h"
#include "nuiVBox.h"
#include "nuiPane.h"
#include "nuiFolderPane.h"
#include "nuiPositioner.h"
#include "nuiScrollView.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiColorDecoration.h"
#include "nuiAttributeEditor.h"
#include "nuiIntrospector.h"
#include <typeinfo>

class nuiMetaPainterInspector : public nuiSimpleContainer
{
public:
  nuiMetaPainterInspector(nuiMetaPainter* pPainter)
  {
    
  }
  
private:
  nuiMetaPainter* mpPainter;
};

///////// nuiWidgetInspectorNode:

nuiWidgetInspectorNode::nuiWidgetInspectorNode(nuiWidget* pTarget)
: nuiTreeNode(pTarget ? (pTarget->GetObjectClass() + nglString(_T(" - ")) + pTarget->GetObjectName()) : nglString(_T("Empty")), false, false),
  mInspectorNodeSink(this),
  mpTarget(pTarget),
  mpTree(NULL)
{
  UpdateInfos(NULL);
  if (pTarget)
	mInspectorNodeSink.Connect(mpTarget->DebugRefreshInfo, &nuiWidgetInspectorNode::UpdateInfos);
}

bool nuiWidgetInspectorNode::UpdateInfos(const nuiEvent& rEvent)
{
  if (!mpTarget)
	return false;

  float r = 0;
  float g = 0;
  float b = 0;
  nuiLabel* pLabel = (nuiLabel*)GetElement();

  nglString str = mpTarget->GetObjectClass() + nglString(" - ") + mpTarget->GetObjectName();
  if (mpTarget->HasGrab())
    str.Insert(_T("[Grab] "), 0);
  if (mpTarget->HasFocus())
    str.Insert(_T("[Focus] "), 0);

  pLabel->SetText(str);

  if (mpTarget->GetNeedRender())
    r = .5f;
  if (mpTarget->GetNeedLayout())
    g = .5f;
  if (mpTarget->GetNeedSelfRedraw())
    b = .5f;

  pLabel->SetColor(eNormalTextFg, nuiColor(r, g, b));

  return false;
}

nuiWidgetInspectorNode::~nuiWidgetInspectorNode()
{
}

void nuiWidgetInspectorNode::Open(bool Opened)
{
  if (Opened && !mOpened)
  {
    Clear();

    nuiContainer* pContainer = dynamic_cast<nuiContainer*>(mpTarget);
    if (pContainer)
    {
      nuiContainer::IteratorPtr pIt = pContainer->GetFirstChild();

      while (pIt->IsValid())
      {
        nuiWidget* pTarget = pIt->GetWidget();
        nuiWidgetInspectorNode* pINode = new nuiWidgetInspectorNode(pTarget);
				
        AddChild(pINode);

        DelChildOnEvent(pTarget->Destroyed, pINode);
        pContainer->GetNextChild(pIt);
      }
      delete pIt;
    }
  }

  nuiTreeNode::Open(Opened);
}

bool nuiWidgetInspectorNode::IsEmpty() const
{
  nuiContainer* pContainer = dynamic_cast<nuiContainer*>(mpTarget);
  if (pContainer && pContainer->GetChildrenCount())
    return false;

  return true;
}

nuiWidget* nuiWidgetInspectorNode::GetTarget()
{
  return mpTarget;
}

nuiWidgetInspector::nuiWidgetInspector(nuiWidget* pTarget)
: nuiComposite(),
  mInspectorSink(this)
{
  SetObjectClass(_T("nuiWidgetInspector"));
  mpTarget = pTarget;
  
  // decoration
  nuiDecoration* pDeco = nuiDecoration::Get(INTROSPECTOR_DECO_CLIENT_BKG);
  if (pDeco)
  {
    SetDecoration(pDeco, eDecorationBorder);
  }

  nuiSplitter* pSplitter = new nuiSplitter(nuiVertical);
  AddChild(pSplitter);

  mpTree = new nuiWidgetInspectorNode(pTarget);
  //nuiColumnTreeView* pTree = new nuiColumnTreeView(new nuiScrollView(pSplitter, true, false), mpTree);
  nuiScrollView* pScrollView = new nuiScrollView(true, true);
  pSplitter->AddChild(pScrollView);
  nuiTreeView* pTree = new nuiTreeView(mpTree);
  pScrollView->AddChild(pTree);
  pTree->SetMultiSelectable(false);
  mpTree->Open(true);

  mpWidgetInfo = new nuiWidgetInfo(NULL);
  pSplitter->AddChild(mpWidgetInfo);

  mInspectorSink.Connect(pTree->SelectionChanged, &nuiWidgetInspector::OnSelectionChanged, pTree);
}

nuiWidgetInspector::~nuiWidgetInspector()
{
}


bool nuiWidgetInspector::OnSelectionChanged(const nuiEvent& rEvent)
{
  //nuiColumnTreeView* pTreeView = (nuiColumnTreeView*)rEvent.mpUser;
  nuiTreeView* pTreeView = (nuiTreeView*)rEvent.mpUser;
  nuiWidget* pTarget = NULL;

  if (pTreeView)
  {
    nuiWidgetInspectorNode* pNode = (nuiWidgetInspectorNode*)pTreeView->GetSelectedNode();
    if (pNode)
    {
      pTarget = pNode->GetTarget();
    }
  }

  mpWidgetInfo->SetTarget(pTarget);

  return false;
}

/////////
class nuiWidgetProxy : public nuiWidget
{
public:
  nuiWidgetProxy(nuiWidget* pTarget)
    : nuiWidget()
  {
    SetObjectClass(_T("nuiWidgetProxy"));
    mpTarget = pTarget;
  }

  virtual ~nuiWidgetProxy()
  {
  }

  virtual nuiRect CalcIdealSize()
  {
    if (mpTarget)
      return mpTarget->GetRect().Size();
    else
      return nuiRect(0,0, 32, 32);
  }
  
  virtual bool Draw(nuiDrawContext* pContext)
  {
/*
    const float f = .8f;
    pContext->SetFillColor(nuiColor(f, f, f));
    pContext->DrawRect(GetRect().Size(), eFillShape);
*/

    if (mpTarget)
      mpTarget->DrawWidget(pContext);
    else
      pContext->DrawText(0, 0, _T("No widget"));
    return true;
  }

  void SetTarget(nuiWidget* pWidget)
  {
    mpTarget = pWidget;
    InvalidateLayout();
  }

private:
  nuiWidget* mpTarget;
};

nuiWidgetInfo::nuiWidgetInfo(nuiWidget* pTarget)
: nuiComposite(),
  mWISink(this)
{
  mpTarget = NULL;
  mNeedUpdate = false;
  mpTimer = new nuiTimer(1.0/15.0);
  mWISink.Connect(mpTimer->Tick, &nuiWidgetInfo::OnTimerTick);
  mpTimer->Start(false);

  mpDummy = NULL;
  mpInfos = NULL;
  
  mpName = NULL;
  mpClass = NULL;
  mpCType = NULL;
#ifndef __GNUC__
  mpCRawType = NULL;
#endif
  mpIdealRect = NULL;
  mpUserRect = NULL;
  mpHasUserSize = NULL;
  mpHasUserPos = NULL;
  mpRect = NULL;
  mpNeedLayout = NULL;
  mpNeedRender = NULL;
  mpNeedSelfRedraw = NULL;
  mpNeedIdealRectCalc = NULL;
  mpGlobalRect = NULL;
  mpMatrix = NULL;
  mpPropertyGrid = NULL;
  mpAttributeGrid = NULL;
  mpProxy = NULL;
  mpMetaPainterPane = NULL;
  
  BuildInfo();

  SetTarget(pTarget);
}

nuiWidgetInfo::~nuiWidgetInfo()
{
  delete mpTimer;
}

void nuiWidgetInfo::SetTarget(nuiWidget* pWidget)
{
  nuiWidget* pTarget = mpTarget;
  
  if (mpTarget)
  {
    nuiTopLevel* pTop = mpTarget->GetTopLevel();
    if (pTop)
      pTop->SetWatchedWidget(NULL);
  }

  mpTarget = pWidget;

  if (mpTarget && mpTarget != pTarget)
  {
    mWISink.Connect(mpTarget->DebugRefreshInfo, &nuiWidgetInfo::OnDebugUpdate);
    mWISink.Connect(mpTarget->Destroyed, &nuiWidgetInfo::OnTrashed);  
    if (pTarget)
    {
      mWISink.Disconnect(pTarget->DebugRefreshInfo);
      mWISink.Disconnect(pTarget->Destroyed);
    }

    nuiTopLevel* pTop = mpTarget->GetTopLevel();
    if (pTop)
      pTop->SetWatchedWidget(mpTarget);
  }

  RebuildInfo();
}

bool nuiWidgetInfo::OnTimerTick(const nuiEvent& rEvent)
{
  if (mNeedUpdate)
  {
    RebuildInfo();
    mNeedUpdate = false;
  }

  return false;
}

bool nuiWidgetInfo::OnDebugUpdate(const nuiEvent& rEvent)
{
  mNeedUpdate = true;
  return false;
}

bool nuiWidgetInfo::OnTrashed(const nuiEvent& rEvent)
{
  SetTarget(NULL);
  return false;
}

void nuiWidgetInfo::RebuildInfo()
{
  if (mpTarget)
  {
    nglString text = mpTarget->GetObjectName();
    mpName->SetText(text);

    //text = mpTarget->GetObjectClass();
    std::vector<nglString> classes;
    mpTarget->GetObjectInheritance(classes);
    text.Wipe();
    for (uint i = 0; i < classes.size(); i++)
    {
      if (i)
      {
        text.Add(_T(" <-- "));
      }
      text.Add(classes[i]);
    }
    mpClass->SetText(text);
    printf("Inheritance: %ls\n", text.GetChars());

    nglString str(typeid(mpTarget).name());
    mpCType->SetText(str);

#ifndef __GNUC__
    str = (typeid(mpTarget).raw_name());
    mpCRawType->SetText(str);
#endif
    
    text = mpTarget->GetIdealRect().GetValue();
    mpIdealRect->SetText(text);

    text = mpTarget->GetUserRect().GetValue();
    mpUserRect->SetText(text);

    text = mpTarget->HasUserSize()?_T("true"):_T("false");
    mpHasUserSize->SetText(text);

    text = mpTarget->HasUserPos()?_T("true"):_T("false");
    mpHasUserPos->SetText(text);

    text = mpTarget->GetRect().GetValue();
    mpRect->SetText(text);

    text = mpTarget->GetNeedLayout()?_T("true"):_T("false");
    mpNeedLayout->SetText(text);

    text = mpTarget->GetNeedRender()?_T("true"):_T("false");
    mpNeedRender->SetText(text);

    text = mpTarget->GetNeedSelfRedraw()?_T("true"):_T("false");
    mpNeedSelfRedraw->SetText(text);

    text = mpTarget->GetNeedIdealRect()?_T("true"):_T("false");
    mpNeedIdealRectCalc->SetText(text);

    nuiRect rect(mpTarget->GetRect().Size());
    mpTarget->LocalToGlobal(rect);
    text = rect.GetValue().GetChars();
    mpGlobalRect->SetText(text);

    nuiMatrix mat(mpTarget->GetMatrix());
    for (int j = 0; j < 4; j++)
    {
      for (int k = 0; k < 4; k++)
      {
        text.SetCFloat(mat(j,k));
        mpMatrix->SetCell(j, k, new nuiLabel(text, nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL"))));
      }
    }

    // build properties list
    std::list<nglString> pnames;
    mpTarget->GetProperties(pnames);
    uint i = 0;
    std::list<nglString>::iterator it = pnames.begin();
    std::list<nglString>::iterator end = pnames.end();

    uint rows = mpPropertyGrid->GetNbRows();
    if (rows)
      mpPropertyGrid->RemoveRows(0, rows);

    rows = pnames.size();
    mpPropertyGrid->AddRows(0, rows);

    while (it != end)
    {
      nglString pname(*it);
      mpPropertyGrid->SetCell(0, i, new nuiLabel(pname + nglString(":")));
      nuiLabel* pLabel = new nuiLabel(mpTarget->GetProperty(pname));
      pLabel->SetWrapping(true);
      mpPropertyGrid->SetCell(1, i, pLabel);

      ++it;
      i++;
    }

    // build attributes list
    std::map<nglString, nuiAttribBase> attributes;
    mpTarget->GetAttributes(attributes);
    i = 0;
    std::map<nglString, nuiAttribBase>::const_iterator it_a = attributes.begin();
    std::map<nglString, nuiAttribBase>::const_iterator end_a = attributes.end();

    rows = mpAttributeGrid->GetNbRows();
    if (rows)
      mpAttributeGrid->RemoveRows(0, rows);

    rows = attributes.size();
    if (rows)
    {
      mpAttributeGrid->AddRows(0, rows);
    }

    while (it_a != end_a)
    {
      nglString pname(it_a->first);
      //printf("\tattr: %ls\n", pname.GetChars());
      nuiAttribBase Base = it_a->second;
      nuiAttributeEditor* pEditor = Base.GetEditor();
      
      mpAttributeGrid->SetCell(0, i, new nuiLabel(pname + nglString(":")));

      if (!pEditor)
      {
        nglString value;
        switch (Base.GetDimension())
        {
          case 0:
            Base.ToString(value);
            break;
          case 1:
            {
              nglString str;
              uint32 count = Base.GetIndexRange(0);
              for (uint32 i = 0; i < count; i++)
              {
                Base.ToString(i, str);
                value.Add(i).Add(_T(":")).Add(str).Add("\t");
              }
              value.Trim(_T('\t'));
            }
            break;
          case 2:
            {
              nglString str;
              uint32 counti = Base.GetIndexRange(0);
              uint32 countj = Base.GetIndexRange(1);
              for (uint32 i = 0; i < MIN(10, counti); i++)
              {
                for (uint32 j = 0; j < MIN(10, countj); j++)
                {
                  Base.ToString(i, j, str);
                  value.Add(i).Add(",").Add(j).Add(_T(":")).Add(str).Add("\t");
                }
              }
              value.Trim(_T('\t'));
            }
            break;
        }
        nuiLabel* pLabel = new nuiLabel(value);
        mpAttributeGrid->SetCell(1, i, pLabel, nuiLeft);
      }
      else
        mpAttributeGrid->SetCell(1, i, pEditor, nuiLeft);

      ++it_a;
      i++;
    }




    mpProxy->SetTarget(mpTarget);

    if (mpTarget->IsRenderCacheEnabled() && mpMetaPainterPane)
    {
      mpMetaPainterPane->Clear(true);
      nuiMetaPainterInspector* pMetaPainterInspector = new nuiMetaPainterInspector(mpTarget->mpRenderCache);
      mpMetaPainterPane->AddChild(pMetaPainterInspector);
    }

    mpDummy->SetVisible(false);
    mpInfos->SetVisible(true);
  }
  else
  {
    mpDummy->SetVisible(true);
    mpInfos->SetVisible(false);
  }
  
  mNeedUpdate = false;
}

void nuiWidgetInfo::BuildInfo()
{
  mpDummy = new nuiSimpleContainer();
  nuiVBox* pMainBox = new nuiVBox(3);
  mpInfos = pMainBox;
  //if (mpTarget)
  {
    mpDummy->SetVisible(false);
    mpInfos->SetVisible(true);
    
    nuiScrollView* pMainScroll = new nuiScrollView(false, true);
    pMainScroll->AddChild(pMainBox);
    AddChild(pMainScroll);
    uint32 count = 14;
#ifndef __GNUC__
    count++;
#endif
    
    uint32 index = 0;
    
    nuiGrid* pBox = new nuiGrid(2, count);
    pBox->SetColumnExpand(1, nuiExpandShrinkAndGrow);
    pBox->DisplayGridBorder(true, 1);
    pBox->SetColor(eBorder, nuiColor(200,200,200));

    //pBox->SetExpand(nuiExpandShrinkAndGrow);

    nuiLabel* pLabel = new nuiLabel(_T("Name"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpName = new nuiLabel();
    mpName->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpName);
    
    pLabel = new nuiLabel(_T("Class"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpClass = new nuiLabel();
    mpClass->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpClass);
    
    pLabel = new nuiLabel(_T("C++ Type"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpCType = new nuiLabel();
    mpCType->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpCType);

#ifndef __GNUC__
    pLabel = new nuiLabel(_T("C++ Raw Type"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpCRawType = new nuiLabel();
	pBox->SetCell(1, index++, mpCRawType);
    mpName->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
#endif
    
    pLabel = new nuiLabel(_T("Ideal Rect"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpIdealRect = new nuiLabel();
    mpIdealRect->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpIdealRect);

    pLabel = new nuiLabel(_T("User Rect"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpUserRect = new nuiLabel();
    mpUserRect->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpUserRect);

    pLabel = new nuiLabel(_T("Has User Size"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpHasUserSize = new nuiLabel();
    mpHasUserSize->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpHasUserSize);

    pLabel = new nuiLabel(_T("Has User Pos"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpHasUserPos = new nuiLabel();
    mpHasUserPos->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpHasUserPos);

    
    pLabel = new nuiLabel(_T("Rect"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpRect = new nuiLabel();
    mpRect->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpRect);
    
    pLabel = new nuiLabel(_T("Need Layout"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpNeedLayout = new nuiLabel();
    mpNeedLayout->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpNeedLayout);
    
    pLabel = new nuiLabel(_T("Need Render"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpNeedRender = new nuiLabel();
    mpNeedRender->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpNeedRender);
    
    pLabel = new nuiLabel(_T("Need Redraw"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpNeedSelfRedraw = new nuiLabel();
    mpNeedSelfRedraw->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpNeedSelfRedraw);
    
    pLabel = new nuiLabel(_T("Need Ideal Rect"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpNeedIdealRectCalc = new nuiLabel();
    mpNeedIdealRectCalc->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpNeedIdealRectCalc);
    
    pLabel = new nuiLabel(_T("Global Rect"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpGlobalRect = new nuiLabel();
    mpGlobalRect->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(1, index++, mpGlobalRect);
    
    pLabel = new nuiLabel(_T("Matrix"));
    pLabel->SetFont(nuiFont::GetFont(_T("INTROSPECTOR_FONT_NORMAL")));
    pBox->SetCell(0, index, pLabel);
    mpMatrix = new nuiGrid(4, 4);
    pBox->SetCell(1, index++, mpMatrix);

    // decoration
    nuiDecoration* pTitlePaneDeco = nuiDecoration::Get(INTROSPECTOR_DECO_FOLDERPANE_TITLE);

    
    nuiFolderPane* pSPane = new nuiFolderPane();
    pSPane->AddChild(pBox);
    
    pLabel = new nuiLabel(_T("Standard"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
    pLabel->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pLabel->SetColor(eSelectedTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pSPane->SetTitleWithHandle(pLabel);
    if (pTitlePaneDeco)
      pSPane->GetTitle()->SetDecoration(pTitlePaneDeco, eDecorationBorder);
    pMainBox->AddCell(pSPane);
    

    /// Property list
    mpPropertyGrid = new nuiGrid(2, 0);
    mpPropertyGrid->DisplayGridBorder(true, 1.0f);
    mpPropertyGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
        
    nuiFolderPane* pVPane = new nuiFolderPane();
    nuiLabel* pLabel2 = new nuiLabel(_T("Properties"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
    pLabel2->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pLabel2->SetColor(eSelectedTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pVPane->SetTitleWithHandle(pLabel2);
    if (pTitlePaneDeco)
      pVPane->GetTitle()->SetDecoration(pTitlePaneDeco, eDecorationBorder);

    
    pVPane->AddChild(mpPropertyGrid);
    pMainBox->AddCell(pVPane);


    /// Attribute list
    mpAttributeGrid = new nuiGrid(2, 0);
    mpAttributeGrid->DisplayGridBorder(true, 1.0f);
    mpAttributeGrid->SetColumnExpand(1, nuiExpandShrinkAndGrow);
        
    nuiFolderPane* pAPane = new nuiFolderPane();
    nuiLabel* pLabel3 = new nuiLabel(_T("Attributes"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
    pLabel3->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pLabel3->SetColor(eSelectedTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pAPane->SetTitleWithHandle(pLabel3);
    if (pTitlePaneDeco)
      pAPane->GetTitle()->SetDecoration(pTitlePaneDeco, eDecorationBorder);
    
    pAPane->AddChild(mpAttributeGrid);
    pMainBox->AddCell(pAPane);



    // Widget rendering
    nuiFolderPane* pProxyPane = new nuiFolderPane(_T("Rendering"), true);
    nuiLabel* pLabel4 = new nuiLabel(_T("Rendering"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
    pLabel4->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pLabel4->SetColor(eSelectedTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
    pProxyPane->SetTitleWithHandle(pLabel4);
    if (pTitlePaneDeco)
      pProxyPane->GetTitle()->SetDecoration(pTitlePaneDeco, eDecorationBorder);

    
    nuiDecoration* pDeco = nuiDecoration::Get(_T("InspectorRenderingDeco"));
    if (!pDeco)
      pDeco = new nuiColorDecoration(_T("InspectorRenderingDeco"), nuiRect(0,0,0,0),  nuiColor(255, 255, 255, 255));
    pProxyPane->SetDecoration(pDeco);
    
    mpProxy = new nuiWidgetProxy(mpTarget);
    nuiScrollView* pProxyScroll = new nuiScrollView(true, false);
    pProxyPane->AddChild(pProxyScroll);
    pProxyScroll->AddChild(mpProxy);
    pProxyScroll->SetForceNoSmartScroll(true);
    pMainBox->AddCell(pProxyPane);

    // nuiMetaPainterInspector container
//     mpMetaPainterPane = new nuiFolderPane(_T("Render cache inspector"), false, nuiColor(255, 255, 255, 255));
//     pMainBox->AddCell(mpMetaPainterPane);

    pMainBox->SetExpand(nuiExpandShrinkAndGrow);
  }

  {    
    nuiLabel* pLabel = new nuiLabel(_T("No widget Selected"));
    pLabel->SetPosition(nuiCenter);
    mpDummy->AddChild(pLabel);
  }

  mpDummy->SetVisible(true);
  mpInfos->SetVisible(false);
  
  mNeedUpdate = false;
}



