/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"

class nuiMetaPainterInspector : public nuiSimpleContainer
  {
  public:
    nuiMetaPainterInspector()
    {
      mpBox = new nuiVBox();
      AddChild(mpBox);
    }

    void SetTarget(const nuiWidget* pTarget)
    {
      mpBox->Clear();
      if (!pTarget)
        return;

      const nuiMetaPainter* pPainter = pTarget->GetRenderCache();
      if (!pPainter)
        return;
      
      int32 count = pPainter->GetNbOperations();
      for (int32 i = 0; i < count; i++)
      {
        nglString str;
        str = pPainter->GetOperationDescription(i);
        mpBox->AddCell(new nuiLabel(str));
      }
    }
    
  private:
    nuiMetaPainter* mpPainter;
    nuiVBox* mpBox;
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

void nuiWidgetInspectorNode::UpdateInfos(const nuiEvent& rEvent)
{
  if (!mpTarget)
    return;
  
  float r = 0;
  float g = 0;
  float b = 0;
  nuiLabel* pLabel = (nuiLabel*)GetElement();
  
  nglString str = mpTarget->GetObjectClass() + nglString(" - ") + mpTarget->GetObjectName();
  if (mpTarget->HasGrab())
    str.Insert(_T("[Grab] "), 0);
  if (mpTarget->HasFocus())
    str.Insert(_T("[Focus] "), 0);

  const nuiMetaPainter* pCache = mpTarget->GetRenderCache();
  if (pCache)
  {
    uint32 rops = pCache->GetRenderOperations();
    uint32 verts = pCache->GetVertices();
    uint32 batches = pCache->GetBatches();
    nglString s;
    s.CFormat(_T(" (%d Vertices / %d arrays)\n"), verts, rops);
    str.Add(s);
  }
  
  pLabel->SetText(str);
  
  if (mpTarget->GetNeedRender())
    r = .5f;
  if (mpTarget->GetNeedLayout())
    g = .5f;
  if (mpTarget->GetNeedSelfRedraw())
    b = .5f;
  
  pLabel->SetColor(eNormalTextFg, nuiColor(r, g, b));
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
: nuiSimpleContainer(),
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


void nuiWidgetInspector::OnSelectionChanged(const nuiEvent& rEvent)
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
      {
        nuiFont* pFont = nuiFont::GetFont(24.0f);
        pContext->SetFont(pFont, true);
        pContext->DrawText(0, 0, _T("No widget"));
      }
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
: nuiSimpleContainer(),
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
  mpAttributeGrid = NULL;
  mpProxy = NULL;
  mpPainterInspector = NULL;
  
  BuildInfo();
  
  SetTarget(pTarget);
}

nuiWidgetInfo::~nuiWidgetInfo()
{
  SetTarget(NULL);
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
      mWISink.DisconnectSource(pTarget->DebugRefreshInfo);
      mWISink.DisconnectSource(pTarget->Destroyed);
    }
    
    nuiTopLevel* pTop = mpTarget->GetTopLevel();
    if (pTop)
      pTop->SetWatchedWidget(mpTarget);
  }
  
  RebuildInfo(true);
}

void nuiWidgetInfo::OnTimerTick(const nuiEvent& rEvent)
{
  if (mNeedUpdate)
  {
    RebuildInfo(false);
    mNeedUpdate = false;
  }
}

void nuiWidgetInfo::OnDebugUpdate(const nuiEvent& rEvent)
{
  mNeedUpdate = true;
}

void nuiWidgetInfo::OnTrashed(const nuiEvent& rEvent)
{
  SetTarget(NULL);
}

void nuiWidgetInfo::RebuildInfo(bool Reconstruct)
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
    //printf("Inheritance: %s\n", text.GetChars());
    
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

    if (Reconstruct)
    {
      // build attributes list
      std::map<nglString, nuiAttribBase> attributes;
      mpTarget->GetAttributes(attributes);
      int32 i = 0;
      int32 rows = 0;
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
        //printf("\tattr: %s\n", pname.GetChars());
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
        {
          mpAttributeGrid->SetCell(1, i, pEditor, nuiFill);
        }
        
        ++it_a;
        i++;
      }
      
    }
    
    
    
    mpProxy->SetTarget(mpTarget);
    mpPainterInspector->SetTarget(mpTarget);
    
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

    {
      // Widget rendering list
      nuiFolderPane* pCachePane = new nuiFolderPane(_T("Render cache"), true);
      nuiLabel* pLabel = new nuiLabel(_T("Render Cache"), nuiFont::GetFont(_T("INTROSPECTOR_FONT_BOLD")));
      pLabel->SetColor(eNormalTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
      pLabel->SetColor(eSelectedTextFg, INTROSPECTOR_COLOR_FOLDERPANE_TITLE);
      pCachePane->SetTitleWithHandle(pLabel);
      pCachePane->Close();
      if (pTitlePaneDeco)
        pCachePane->GetTitle()->SetDecoration(pTitlePaneDeco, eDecorationBorder);
      
      
      nuiDecoration* pDeco = nuiDecoration::Get(_T("InspectorRenderingDeco"));
      if (!pDeco)
        pDeco = new nuiColorDecoration(_T("InspectorRenderingDeco"), nuiRect(0,0,0,0),  nuiColor(255, 255, 255, 255));
      pProxyPane->SetDecoration(pDeco);
      
      mpPainterInspector = new nuiMetaPainterInspector();
      nuiScrollView* pScroll = new nuiScrollView(true, false);
      pCachePane->AddChild(pScroll);
      pScroll->AddChild(mpPainterInspector);
      pScroll->SetForceNoSmartScroll(true);
      pMainBox->AddCell(pCachePane);
      
    }
    

    
    
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



