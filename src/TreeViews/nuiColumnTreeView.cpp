/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#include "nui.h"

#define NUI_COLUMNTREEVIEW_INTERLINE (3.0f)
#define NUI_COLUMNTREEVIEW_HANDLE_SIZE (12.0f)
#define NUI_COLUMNTREEVIEW_BAR_SIZE (12.0f)
#define NUI_COLUMNTREEVIEW_MINCOLSIZE (120.0f)

#define INITIAL_TIMER_PERIOD 0.2

/////////////////////////////
// nuiColumnTreeView
nuiColumnTreeView::nuiColumnTreeView(nuiTreeNodePtr pTree)
: nuiTreeView(pTree),
  mColumnTreeViewSink(this),
  mKey(nglString::Empty)
{
  SetObjectClass(_T("nuiColumnTreeView"));
  mMinColumnSize = NUI_COLUMNTREEVIEW_MINCOLSIZE;
  SetWantKeyboardFocus(true);

  mInterline = NUI_COLUMNTREEVIEW_INTERLINE;
  mHandleSize = NUI_COLUMNTREEVIEW_HANDLE_SIZE;
  mBarSize = NUI_COLUMNTREEVIEW_BAR_SIZE;

  mpPreview = NULL;
  mActivateOnUnclick = false;
  mNeedCalcVerticalHotRect = true;

  CreateScrollBars();
  InitTimer();
}

nuiColumnTreeView::~nuiColumnTreeView()
{
}

void nuiColumnTreeView::InitTimer()
{
  mpButtonTimer = new nuiTimer(INITIAL_TIMER_PERIOD);
  mColumnTreeViewSink.Connect(mpButtonTimer->Tick, &nuiColumnTreeView::OnButtonTimerTick);
}

bool nuiColumnTreeView::Draw(nuiDrawContext* pContext)
{
  pContext->ResetState();

  nuiTreeNode* pTree = mpTree;
  while (pTree)
    pTree = DrawColumn(pContext, pTree);

  for (uint i = 0; i < mpScrollBars.size(); i++)
    DrawChild(pContext, mpScrollBars[i]);

  if (mpPreview && mpPreview->IsVisible())
    DrawChild(pContext, mpPreview);

  return true;
}

nuiTreeNode* nuiColumnTreeView::DrawColumn(nuiDrawContext* pContext, nuiTreeNode* pTree)
{
  nuiTreeNode* pRes = NULL;
  if (!pTree)
    return NULL;

  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = checked_cast<nuiTreeNodePtr>(pTree->GetChild(i));
    {
      nuiWidgetPtr pWidget = pNode->GetElement();
      if (!pRes && (pNode->IsOpened() || pNode->IsSelected()))
      {
        nuiRect r = pWidget->GetRect();
        r.SetSize(r.GetWidth() + mHandleSize, r.GetHeight());
        pTheme->DrawSelectionBackground(pContext, r, pWidget);
      }

      if (!pNode->IsEmpty())
      {

        nuiRect r = pWidget->GetRect();
        nuiRect HR(r.Left() + r.GetWidth(), r.Top(), mHandleSize, r.GetHeight());
        
        SetFillColor(pContext, eTreeViewHandle);
        SetStrokeColor(pContext, eTreeViewHandle);
        pContext->PushClipping();
        r.SetSize(r.GetWidth() + mHandleSize, r.GetHeight());
        pContext->Clip(r);
        pTheme->DrawTreeHandle(pContext, HR, false, mHandleSize-4, GetColor(eTreeViewHandle));
        pContext->PopClipping();
      }
      DrawChild(pContext, pWidget);

      if (!pRes && (pNode->IsOpened() || pNode->IsSelected()))
      {
        pRes = pNode;
        
        nuiRect r = pWidget->GetRect();
        r.SetSize(r.GetWidth() + mHandleSize, r.GetHeight());
        pTheme->DrawSelectionForeground(pContext, r, pWidget);
      }
      
    }
  }
  pTheme->Release();
  return pRes;
}


nuiRect nuiColumnTreeView::CalcIdealSize()
{
  mColumnSizes.clear();
  mIdealRect = nuiRect();

  nuiTreeNode* pTree = mpTree;

  while (pTree)
  {
    nuiRect Rect;
    pTree = CalcColumnSize(Rect, pTree);
    mColumnSizes.push_back(Rect);
    mIdealRect.Right() += Rect.GetWidth();
    mIdealRect.Bottom() = MAX(Rect.GetHeight(), mIdealRect.Bottom());
  }

  if (mpPreview && mpPreview->IsVisible())
  {
    nuiRect r(mpPreview->GetIdealRect().Size());
    mIdealRect.Right() += r.GetWidth();
  }
  
  return mIdealRect;
}

nuiTreeNode* nuiColumnTreeView::CalcColumnSize(nuiRect& rRect, nuiTreeNode* pTree)
{
  nuiTreeNode* pRes = NULL;
  nuiSize w = mMinColumnSize;
  nuiSize h = 0;
  if (!pTree)
    return NULL;

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = checked_cast<nuiTreeNodePtr>(pTree->GetChild(i));
    {
      if (!pRes && pNode->IsOpened())
        pRes = pNode;
      nuiWidgetPtr pWidget = pNode->GetElement();

      nuiRect R = pWidget->GetIdealRect().Size();
      w = MAX(w, R.GetWidth());
      h += R.GetHeight();
      h += mInterline;
    }
  }

  rRect.Set(0.0f,0.0f, w + mHandleSize + mBarSize, h);

  return pRes;
}

bool nuiColumnTreeView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  CreateScrollBars();
  
  nuiTreeNode* pTree = mpTree;

  uint Depth = 0;

  nuiSize x = 0;
  while (pTree)
  {
    nuiRange& range(mpScrollBars[Depth]->GetRange());
    nuiSize y = 0;
    if (pTree->GetChildrenCount())
      y = -range.GetValue();
    nuiRect Rect(x,y, mColumnSizes[Depth].GetWidth(), mColumnSizes[Depth].GetHeight()+ mInterline);
    nuiSize lineSize;
    
    if (pTree->GetChildrenCount())
      lineSize = mColumnSizes[Depth].GetHeight()/(float)pTree->GetChildrenCount();
    else
      lineSize = 0.f;

    pTree = SetColumnRect(Rect, pTree);
    x+= Rect.GetWidth();
    
    Rect.Move(Rect.GetWidth() - mBarSize, -y);
    Rect.SetSize(mBarSize, rRect.GetHeight());
    range.SetRange(0, mColumnSizes[Depth].GetHeight());
    range.SetPageSize(rRect.GetHeight());
    range.SetIncrement(lineSize);
    range.SetPageIncrement(MAX(0, rRect.GetHeight() - lineSize)); // 1 line of overlap between two page skips
    mpScrollBars[Depth]->SetLayout(Rect);
    Depth++;
  }

  if (mpPreview)
  {
    // There is a preview widget
    if (mpPreview->IsVisible(false))
    {
      nuiRect p(mpPreview->GetIdealRect().Size());
      nuiRect Rect(x,0.0f, p.GetWidth(), rRect.GetHeight());
      mpPreview->SetLayout(Rect);
    }
  }

  if (mNeedCalcVerticalHotRect)
  {
     mNeedCalcVerticalHotRect = false;
     CalcVerticalHotRect();
  }
  return true;
}

nuiTreeNode* nuiColumnTreeView::SetColumnRect(const nuiRect& rRect, nuiTreeNode* pTree)
{
  nuiSize x,y,w,h;
  x = rRect.Left();
  y = rRect.Top();
  w = rRect.GetWidth() - (mBarSize + mHandleSize);
  h = rRect.GetHeight();
  nuiTreeNode* pRes = NULL;

  if (!pTree)
    return NULL;

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = checked_cast<nuiTreeNodePtr>(pTree->GetChild(i));
    {
      if (!pRes && pNode->IsOpened())
        pRes = pNode;
      nuiWidgetPtr pWidget = pNode->GetElement();

      nuiRect R = pWidget->GetIdealRect().Size();
      R.Move(x,y);
      R.RoundToNearest();
      R.SetSize(w,R.GetHeight());
      pWidget->SetLayout(R);
      y += R.GetHeight();
      y += mInterline;
    }
  }

  return pRes;
}

////// Interaction:

nuiTreeNodePtr nuiColumnTreeView::GetParentNode(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode)
{
  for (uint32 i = 0; i < pParent->GetChildrenCount(); i++)
  {
    nuiTreeNodePtr pChild = checked_cast<nuiTreeNodePtr>(pParent->GetChild(i));
    if (pChild == pNode)
      return pParent;
    pChild = GetParentNode(pChild, pNode);
    if (pChild)
      return pChild;
  }
  return NULL;
}

nuiTreeNodePtr nuiColumnTreeView::GetNodePath(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode, std::list<nuiTreeNodePtr>& rPath)
{
  rPath.clear();
  for (uint32 i = 0; i < pParent->GetChildrenCount(); i++)
  {
    nuiTreeNodePtr pChild = checked_cast<nuiTreeNodePtr>(pParent->GetChild(i));
    if (pChild == pNode)
    {
      rPath.push_front(pChild);
      return pParent;
    }
    pChild = GetParentNode(pChild, pNode);
    if (pChild)
    {
      rPath.push_front(pChild);
      return pChild;
    }
  }
  return NULL;
}

nuiLabel* SearchLabel(nuiWidgetPtr pParent)
{
  nuiLabel* pLabel = dynamic_cast<nuiLabel*>(pParent);
  if (pLabel)
    return pLabel;

  nuiContainer* pContainer = dynamic_cast<nuiContainer*>(pParent);
  if (pContainer)
  {
    nuiContainer::IteratorPtr pIt;
    for (pIt = pContainer->GetFirstChild(); pIt && pIt->IsValid(); pContainer->GetNextChild(pIt))
    {
      pLabel = SearchLabel(pIt->GetWidget());
      if (pLabel)
        return pLabel;
    }
    delete pIt;
  }

  return NULL;
}

// Event callbacks:
bool nuiColumnTreeView::KeyDown (const nglKeyEvent& rEvent)
{
  bool handled = false;
  bool SelectAnyNode = false;
  nuiTreeNode* pNode = NULL;

  if (rEvent.mKey == NK_RIGHT)
  {
    handled = true;
    if (mpSelectedNode)
    {
      if (!mpSelectedNode->IsEmpty() && mpSelectedNode->GetChildrenCount())
      {
        pNode = checked_cast<nuiTreeNode*>(mpSelectedNode->GetChild(0));
      }
    }
    else 
      SelectAnyNode = true;
  }
  else if (rEvent.mKey==NK_LEFT)
  {
    handled = true;
    if (mpSelectedNode)
    {
      pNode = GetParentNode(mpTree, mpSelectedNode);
      if (pNode == mpTree)
        pNode = NULL;
    }
    else 
      SelectAnyNode = true;
  }
  else if (rEvent.mKey==NK_UP)
  {
    handled = true;
    if (mpSelectedNode)
    {
      nuiTreeNodePtr pParent = GetParentNode(mpTree, mpSelectedNode);
      if (pParent)
      {
        uint size = pParent->GetChildrenCount();
        nuiTreeNode* pNext = NULL;
        nuiTreeNode* pCurrent = NULL;
        nuiTreeNode* pLast = NULL;
        for (uint i = 0; i<size && !pNext; i++)
        {
          pCurrent = checked_cast<nuiTreeNode*>(pParent->GetChild(i));
          if (pCurrent == mpSelectedNode)
            pNext = pLast;
          pLast = pCurrent;
        }
        pNode = pNext;
      }
    }
    else 
      SelectAnyNode = true;
  }
  else if (rEvent.mKey==NK_DOWN)
  {
    handled = true;
    if (mpSelectedNode)
    {
      nuiTreeNodePtr pParent = GetParentNode(mpTree, mpSelectedNode);
      if (pParent)
      {
        uint size = pParent->GetChildrenCount();
        nuiTreeNode* pNext = NULL;
        nuiTreeNode* pCurrent = NULL;
        nuiTreeNode* pLast = NULL;
        if (size)
        {
          for (uint i = size-1; i>=0 && !pNext; i--)
          {
            pCurrent = checked_cast<nuiTreeNode*>(pParent->GetChild(i));
            if (pCurrent == mpSelectedNode)
            {
              pNext = pLast;
              break;
            }
            pLast = pCurrent;
          }
        }
        pNode = pNext;
      }
    }
    else 
      SelectAnyNode = true;
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    handled = true;
    if (mpSelectedNode)
    {
      Activated();
      return true;
    }
  }
  else if (rEvent.mChar)
  {
    int index = -1;
    nuiTreeNodePtr pParent = mpTree;
    if (mpSelectedNode)
    {
      pParent = checked_cast<nuiTreeNodePtr>(mpSelectedNode->GetParent());
      if (pParent)
      {
        uint count = pParent->GetChildrenCount();
        for (uint i = 0; i < count && index == -1; i++)
        {
          if (mpSelectedNode == pParent->GetChild(i))
          {
            index = i;
          }
        }
      }
    }

    if (pParent)
    {
      index++;

      nglChar Char = rEvent.mChar;
      Char = toupper(Char);
      uint count = pParent->GetChildrenCount();
      for (uint i = 0; i < count && !pNode; i++)
      {
        nuiTreeNodePtr pN = checked_cast<nuiTreeNodePtr>(pParent->GetChild((i + index) % count));
        if (mKey.IsEmpty())
        {
          nuiWidget* pItem = pN->GetElement();
          nuiLabel* pLabel = SearchLabel(pItem);
          if (pLabel)
          {
            const nglString& rKey = pLabel->GetText();
            if (toupper(rKey[0]) == Char)
            {
              pNode = pN;
            }
          }
        }
        else
        {
          const nglString& rKey = pN->GetProperty(mKey);
          if (rKey[0] == Char)
          {
            pNode = pN;
          }
        }
      }
    }
  }

  if (!pNode && SelectAnyNode)
  {
    if (mpTree->GetChildrenCount())
      pNode = checked_cast<nuiTreeNodePtr>(mpTree->GetChild(0));
  }

  if (pNode && (pNode != mpSelectedNode))
  {
    handled = true;

    nuiTreeNodePtr pParent = GetParentNode(mpTree, pNode);
    
    NGL_ASSERT(pParent != NULL);
    pParent->OpenAllChildren(false);
    pParent->SelectAllChildren(false);
    //pParent->Open(true);
    //pParent->Select(true);

    if (!pNode->IsEmpty())
    {
      pNode->Open(true);
      if((GetDepth()-1) < mpScrollBars.size())
        mpScrollBars[GetDepth()-1]->GetRange().SetValue(0.f);
    }
    
    mpSelectedNode = pNode;
    pNode->Select(true);
    
    Invalidate();
    
    SelectionChanged();
    
    CalcHotRect();
  }
  CreateScrollBars();
  
  if (mpSelectedNode)
  {
    nuiRect r;
    nuiSize y = 0, ys = 0;
    nuiTreeNodePtr pTree = GetParentNode(mpTree, mpSelectedNode);
    nuiTreeNodePtr pN = NULL;
    if (pTree)
    {
      for (uint i = 0; i < pTree->GetChildrenCount() && pN != mpSelectedNode; i++)
      {
        pN = (nuiTreeNodePtr)pTree->GetChild(i);
        y += ys;
        ys = pN->GetElement()->GetRect().GetHeight() + mInterline;
      }

      uint32 depth = GetDepth()-1;
      if (mpSelectedNode->IsOpened() && depth)
        depth--;
      mpScrollBars[depth]->GetRange().MakeInRange(y, ys);
    }
  }

  return handled;
}

bool nuiColumnTreeView::KeyUp (const nglKeyEvent& rEvent)
{
  if (rEvent.mKey==NK_RIGHT || rEvent.mKey==NK_LEFT || rEvent.mKey==NK_UP || rEvent.mKey==NK_DOWN
    || rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
    return true;
  return false;
}


bool nuiColumnTreeView::DispatchMouseClick(const nglMouseInfo& rInfo)
{
  return nuiSimpleContainer::DispatchMouseClick(rInfo);
}

bool nuiColumnTreeView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  bool shiftisdown = IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT);
  if ((Button & nglMouseInfo::ButtonLeft) || (Button & nglMouseInfo::ButtonRight))
  {
    if ((Button & nglMouseInfo::ButtonLeft) && (Button & nglMouseInfo::ButtonDoubleClick))
    {
      if (mpSelectedNode)
      {
        nuiRect r = mpSelectedNode->GetElement()->GetRect();
        r.SetSize(r.GetWidth() + mHandleSize, r.GetHeight() + mInterline);
        if (r.IsInside(X,Y))
        {
          mActivateOnUnclick = true;
          Grab();
          return true;
        }
      }
    }

    nuiTreeNodePtr pNode = FindNode(X,Y);
    if (pNode && (pNode != mpSelectedNode))
    {
      nuiTreeNodePtr pParent = GetParentNode(mpTree, pNode);

      NGL_ASSERT(pParent != NULL);
      pParent->OpenAllChildren(false);
      pParent->SelectAllChildren(false);

      if (!pNode->IsEmpty())
      {
        pNode->Open(true);
        if((GetDepth()-1) < mpScrollBars.size())
          mpScrollBars[GetDepth()-1]->GetRange().SetValue(0.f);
      }

      mpSelectedNode = pNode;
      pNode->Select(true);

      Invalidate();

      Clicked(X,Y, Button); ///< This event is called whenever an item is clicked.
      SelectionChanged();

      CalcHotRect();
      return true;
    }

    return false;
  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    if (shiftisdown)
      return false;
    uint32 depth = 0;
    while (depth < mpScrollBars.size() && (mpScrollBars[depth]->GetRect().Left() < X))
      depth++;
    if (depth < mpScrollBars.size())
      mpScrollBars[depth]->GetRange().Increment();
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelUp)
  {
    if (shiftisdown)
      return false;
    uint32 depth = 0;
    while (depth < mpScrollBars.size() && (mpScrollBars[depth]->GetRect().Left() < X))
      depth++;
    if (depth < mpScrollBars.size())
      mpScrollBars[depth]->GetRange().Decrement();
    return true;
  }

  return false;
}

bool nuiColumnTreeView::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mActivateOnUnclick)
  {
    mActivateOnUnclick = false;
    Ungrab();
    Activated();
  }

  if (Button & nglMouseInfo::ButtonRight)
  {
    // Show up the context menu...
    ContextMenuRequested(X, Y, Button);
  }

  return true;
}

bool nuiColumnTreeView::MouseMoved(nuiSize X, nuiSize Y)
{
  return false;
}


nuiTreeNodePtr nuiColumnTreeView::FindNode(nuiSize X, nuiSize Y)
{
  nuiTreeNode* pTree = NULL;
  nuiTreeNode* pNextTree = mpTree;

  while (pTree != pNextTree)
  {
    pTree = pNextTree;
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pNode = checked_cast<nuiTreeNodePtr>(pTree->GetChild(i));
      {
        nuiRect r = pNode->GetElement()->GetRect();
        r.SetSize(r.GetWidth() + mHandleSize, r.GetHeight() + mInterline);
        if (r.IsInside(X,Y))
          return pNode;
        
        if (pNode->IsOpened())
          pNextTree = pNode;
      }
    }
  }
  return NULL;
}

uint32 nuiColumnTreeView::GetDepth()
{
  uint32 Depth = 0;
  nuiTreeNode* pTree = NULL;
  nuiTreeNode* pNextTree = mpTree;
  while (pTree != pNextTree)
  {
    Depth++;
    pTree = pNextTree;
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreePtr pPtr = pTree->GetChild(i);
      nuiTreeNode* pNode = checked_cast<nuiTreeNodePtr>(pPtr);
      if (pNode->IsOpened())
        pNextTree = pNode;
    }
  }
  return Depth;
}

void nuiColumnTreeView::CalcHotRect()
{
  GetIdealRect(); // Make sure the ideal rects are ok.

  nuiTreeNodePtr pNode = mpSelectedNode;
  if (!pNode) // if there is no current selection, then take the root node
    pNode = mpTree;
  if (pNode)
  {
    nuiRect hotrect = pNode->GetElement()->GetRect();
    if (mpPreview && mpPreview->IsVisible())
    {
      hotrect.SetSize(hotrect.Right() + mBarSize + mHandleSize + mpPreview->GetIdealRect().GetWidth(), hotrect.GetHeight());
    }
    else if (pNode->IsOpened() && !pNode->IsEmpty())
    {
      nuiRect tmp;
      CalcColumnSize(tmp, pNode);
      hotrect.SetSize(hotrect.GetWidth() + mBarSize + mHandleSize + tmp.GetWidth(), hotrect.GetHeight());
    }

    CreateScrollBars();
    SetHotRect(hotrect);
  }
}

void nuiColumnTreeView::CalcVerticalHotRect()
{
  nuiTreeNodePtr pNode = mpSelectedNode;
  if (!pNode) // if there is not current selection, then take the root node
    pNode = mpTree;
  if (pNode)
  {
    nuiRect r;
    nuiSize y = 0, ys = 0;
    nuiTreeNodePtr pTree = GetParentNode(mpTree, mpSelectedNode);
    nuiTreeNodePtr pN = NULL;
    if (pTree)
    {
      for (uint i = 0; i < pTree->GetChildrenCount() && pN != mpSelectedNode; i++)
      {
        pN = (nuiTreeNodePtr)pTree->GetChild(i);
        y += ys;
        ys = pN->GetElement()->GetRect().GetHeight() + mInterline;
      }

      uint32 depth = GetDepth()-1;
      if (mpSelectedNode->IsOpened() && depth)
        depth--;

      nuiRange &range = mpScrollBars[depth]->GetRange();
      if (range.GetPageSize() <= 0)
      {
        mNeedCalcVerticalHotRect = true;
        return;
      }
      if (y < range.GetPageSize())
      {
        
        return; // already in range
      }

      range.MakeInRange(y, ys);
    }
  }
}


void nuiColumnTreeView::InvalidateColumn(const nuiEvent& rEvent)
{
//  int column = (int)rEvent.mpUser;
  //SetRect(GetBorderedRect());
/*

  nuiTreeNode* pTree = mpTree;

  uint Depth = 0;

  nuiSize x = 0;
  while (pTree)
  {
    if (Depth == column)

    nuiRange& range(mpScrollBars[Depth]->GetRange());
    nuiSize y = mpScrollBars[Depth]?(-range.GetValue()):0;
    nuiRect Rect(x,y, mColumnSizes[Depth].GetWidth(), mColumnSizes[Depth].GetHeight()+ mInterline);
    nuiSize lineSize = mColumnSizes[Depth].GetHeight()/(float)pTree->GetChildrenCount();

    pTree = GetNextColumn(Rect, pTree);
    Depth++;
  }
*/

  UpdateLayout();
}

void nuiColumnTreeView::CreateScrollBars()
{
  uint32 Depth = GetDepth();
  
  //NGL_OUT(_T("CreateScrollBars for %d columns (%d)\n"), Depth, mpScrollBars.size());
  
  if (mpScrollBars.size() == Depth)
  {
    //NGL_OUT(_T("Done (no change)\n"));
    return;
  }

  if (mpScrollBars.size() < Depth)
  {
    //NGL_OUT(_T("Need %d more\n"), Depth - mpScrollBars.size());
    // Allocate new scrollbars
    while (mpScrollBars.size() < Depth)
    {
      nuiCustomScrollBar* pScrollBar = new nuiCustomScrollBar(nuiVertical);
      AddChild(pScrollBar);
      
      nuiPane* pPane = new nuiPane(GetColor(eScrollBarBg), GetColor(eScrollBarFg), eFillShape);
      pPane->SetUserSize(mBarSize, mBarSize + 2.f);
      nuiPath path;
      path.AddVertex(nuiPoint(0.f, 0.f));
      path.AddVertex(nuiPoint(3.5f, 6.f));
      path.AddVertex(nuiPoint(7.f, 0.f));
      path.AddVertex(nuiPoint(0.f, 0.f));
      nuiShape* pShape = new nuiShape();
      pShape->AddLines(path);

      nuiShapeView* pShapeView = new nuiShapeView(pShape);
      pShapeView->SetPosition(nuiCenter);
      pPane->AddChild(pShapeView);
      pShapeView->SetUserSize(8.f,8.f);
      pShapeView->SetShapeMode(eStrokeAndFillShape);
      pShapeView->SetColor(eShapeStroke, GetColor(eScrollBarFg));
      pShapeView->SetColor(eShapeFill, GetColor(eScrollBarFg));
      pScrollBar->AddWidget(pPane, nuiBottom);
      mColumnTreeViewSink.Connect(pPane->Clicked, &nuiColumnTreeView::ArrowDownClicked, (void*)mpScrollBars.size());
      mColumnTreeViewSink.Connect(pPane->Unclicked, &nuiColumnTreeView::StopButtonTimer, (void*)mpScrollBars.size());
      
      pPane = new nuiPane(GetColor(eScrollBarBg), GetColor(eScrollBarFg), eFillShape);
      pPane->SetUserSize(mBarSize, mBarSize + 2.f);
      path = nuiPath();
      path.AddVertex(nuiPoint(0.f, 6.f));
      path.AddVertex(nuiPoint(7.f, 6.f));
      path.AddVertex(nuiPoint(3.5f, 0.f));
      path.AddVertex(nuiPoint(0.f, 6.f));
      pShape = new nuiShape();
      pShape->AddLines(path);

      pShapeView = new nuiShapeView(pShape);
      pShapeView->SetPosition(nuiCenter);
      pPane->AddChild(pShapeView);
      pShapeView->SetUserSize(8.f,8.f);
      pShapeView->SetShapeMode(eStrokeAndFillShape);
      pShapeView->SetColor(eShapeStroke, GetColor(eScrollBarFg));
      pShapeView->SetColor(eShapeFill, GetColor(eScrollBarFg));
      pScrollBar->AddWidget(pPane, nuiBottom);
      pPane->SetInterceptMouse(true);
      mColumnTreeViewSink.Connect(pPane->Clicked, &nuiColumnTreeView::ArrowUpClicked, (void*)mpScrollBars.size());
      mColumnTreeViewSink.Connect(pPane->Unclicked, &nuiColumnTreeView::StopButtonTimer, (void*)mpScrollBars.size());
      
      mColumnTreeViewSink.Connect(pScrollBar->ValueChanged, &nuiColumnTreeView::InvalidateColumn, (void*)mpScrollBars.size());
      mpScrollBars.push_back(pScrollBar);
    }

    return;
  }

  if (mpScrollBars.size() > Depth)
  {
    //NGL_OUT(_T("Need %d less\n"), mpScrollBars.size() - Depth);
    // Delete redundant Scrollbars:
    for (uint i = Depth; i < mpScrollBars.size(); i++)
    {
      mpScrollBars[i]->Trash();
      mpScrollBars[i] = NULL;
    }

    mpScrollBars.resize(Depth);
  }
}

void nuiColumnTreeView::OnButtonTimerTick(const nuiEvent& rEvent)
{
  nuiScrollBar* pScrollBar = mpScrollBars[mButtonDepth];
  switch (mTimedButtonEvent)
  {
    case eUpPressed:
      pScrollBar->GetRange().Decrement();
      break;
    case eDownPressed:
      pScrollBar->GetRange().Increment();
      break;
    default:
      break;
  }
  rEvent.Cancel();
}

void nuiColumnTreeView::StopButtonTimer(const nuiEvent& rEvent)
{
  mpButtonTimer->Stop();
  mButtonDepth = (unsigned long int)rEvent.mpUser;
  mpButtonTimer->SetPeriod(INITIAL_TIMER_PERIOD);
  rEvent.Cancel();
}

void nuiColumnTreeView::ArrowUpClicked(const nuiEvent& rEvent)
{
  mTimedButtonEvent = eUpPressed;
  mButtonDepth = (unsigned long int)rEvent.mpUser;
  mpButtonTimer->Start();
  rEvent.Cancel();
}

void nuiColumnTreeView::ArrowDownClicked(const nuiEvent& rEvent)
{
  mTimedButtonEvent = eDownPressed;
  mButtonDepth = (unsigned long int)rEvent.mpUser;
  mpButtonTimer->Start();
  rEvent.Cancel();
}

void nuiColumnTreeView::OnTreeChildAdded(const nuiEvent& rEvent)
{
  nuiTreeView::OnTreeChildAdded(rEvent);
  bool res = rEvent.IsCanceled();
  CreateScrollBars();
  if (res)
    rEvent.Cancel();
}

void nuiColumnTreeView::OnTreeChildDeleted(const nuiEvent& rEvent)
{
  nuiTreeView::OnTreeChildDeleted(rEvent);
  bool res = rEvent.IsCanceled();
  CreateScrollBars();
  if (res)
    rEvent.Cancel();
}

void nuiColumnTreeView::OnTreeChanged(const nuiEvent& rEvent)
{
  nuiTreeView::OnTreeChanged(rEvent);
  CreateScrollBars();
}

void nuiColumnTreeView::SetPreview(nuiWidgetPtr pPreview)
{
  if (mpPreview == pPreview)
    return;

  if (mpPreview)
    mpPreview->Trash();
  mpPreview = pPreview;
  if (mpPreview)
    AddChild(mpPreview);
}

nuiWidgetPtr nuiColumnTreeView::GetPreview() const
{
  return mpPreview;
}

// Standard preview widget:
nuiColumnTreeViewPreview::nuiColumnTreeViewPreview(nuiColumnTreeView* pParent)
: nuiSimpleContainer(),
  mPreviewSink(this)
{
  mPreviewSink.Connect(pParent->SelectionChanged, &nuiColumnTreeViewPreview::OnUpdatePreview);
  pParent->SetPreview(this);
}

nuiColumnTreeViewPreview::~nuiColumnTreeViewPreview()
{
}

void nuiColumnTreeViewPreview::OnUpdatePreview(const nuiEvent& rEvent)
{
  nuiColumnTreeView* pTree = (nuiColumnTreeView*)GetParent();
  nuiTreeNode* pNode = pTree?pTree->GetSelectedNode():NULL;

  SetVisible(UpdatePreview(pTree, pNode));
}

bool nuiColumnTreeViewPreview::UpdatePreview(nuiColumnTreeView* pTree, nuiTreeNodePtr pSelectedNode)
{
  return false;
}


void nuiColumnTreeView::SetTree(nuiTreeNodePtr pTree)
{
  nuiTreeView::SetTree(pTree);
  CreateScrollBars();
  for (uint i = 0; i < mpScrollBars.size(); i++)
    mpScrollBars[i]->GetRange().SetValue(0);
}

void nuiColumnTreeView::SetKey(const nglString& rKey)
{
  mKey = rKey;
}

const nglString& nuiColumnTreeView::GetKey() const
{
  return mKey;
}
