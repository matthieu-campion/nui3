/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiTreeView.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiContainer.h"

#define NUI_TREEVIEW_DEPTH_INSET (12.0f)
#define NUI_TREEVIEW_INTERLINE (2.0f)
#define NUI_TREEVIEW_HANDLE_SIZE (8.0f)

// nuiTreeNode
nuiTreeNode::nuiTreeNode(nuiWidgetPtr pElement, bool Opened, bool Selected, bool DrawTreeHandle, bool alwaysDisplayTreeHandle)
  : nuiTree<nuiWidget>(pElement, true)
{
  SetObjectClass(_T("nuiTreeNode"));
  mOpened = Opened;
  mSelected = Selected;
  mDrawTreeHandle = DrawTreeHandle;
  mAlwaysDisplayTreeHandle = alwaysDisplayTreeHandle;
  mTempSelected = false;
  if (mSelected && mpElement)
    mpElement->SetSelected(true);

  mPosition = 0;
  mSize = 0;
  mSelectionEnabled = true;
}

nuiTreeNode::nuiTreeNode(const nglString& rLabelName, bool Opened, bool Selected, bool DrawTreeHandle, bool alwaysDisplayTreeHandle)
  : nuiTree<nuiWidget>(NULL, true)
{
  SetObjectClass(_T("nuiTreeNode"));

  nuiTheme *pTheme = nuiTheme::GetTheme();
  mpElement = pTheme->CreateTreeNodeLabel(rLabelName);
  pTheme->Release();

  mOpened = Opened;
  mSelected = Selected;
  mDrawTreeHandle = DrawTreeHandle;
  mAlwaysDisplayTreeHandle = alwaysDisplayTreeHandle;
  mTempSelected = false;
  if (mSelected && mpElement)
    mpElement->SetSelected(true);

  mPosition = 0;
  mSize = 0;
  mSelectionEnabled = true;
}

nuiTreeNode::~nuiTreeNode()
{
  //NGL_OUT(_T("nuiTreeNode::~nuiTreeNode() [0x%x]\n"), this);
}

bool nuiTreeNode::IsTreeHandleDrawned() const
{
  return mDrawTreeHandle;
}

bool nuiTreeNode::IsOpened() const
{
  return mOpened;
}

void nuiTreeNode::Open(bool _Opened)
{
  if (mOpened == _Opened)
    return;
  mOpened = _Opened;
  Changed();
  if (mOpened)
    Opened(nuiTreeEvent<nuiTreeNode>(nuiWidgetSelected, dynamic_cast<nuiTreeNode*>(GetParent()), this));
  else
    Closed(nuiTreeEvent<nuiTreeNode>(nuiWidgetSelected, dynamic_cast<nuiTreeNode*>(GetParent()), this));
  Changed();
}

bool nuiTreeNode::IsSelected() const
{
  return mSelected || mTempSelected;
}

void nuiTreeNode::EnableSelection(bool enable)
{
  mSelectionEnabled = enable;
}

bool nuiTreeNode::IsSelectionEnabled()
{
  return mSelectionEnabled;
}

void nuiTreeNode::Select(bool Selected, bool Temporary)
{
  if (!IsSelectionEnabled())
    return;
  if (Temporary)
  {
    if (mTempSelected == Selected)
      return;
    mTempSelected = Selected;
//    Changed();
  }
  else
  {
    if (mSelected == Selected)
      return;
    mSelected = Selected;
    mTempSelected = false; ///< When we print the selection there is no temp selection left.
    SelectionChanged();
//    Changed();
  }
  mpElement->SetSelected(mSelected || mTempSelected);
}

void nuiTreeNode::Select(const nuiRect& rRect, bool Sel, bool TemporarySelection)
{  
  nuiWidgetPtr pWidget = GetElement();
  nuiRect u;
  u.Intersect(pWidget->GetRect(), rRect);
  if ( u.GetSurface() != 0 )
  {
    Select(Sel, TemporarySelection);
  }

  if (IsOpened())
  {
    for (uint32 i = 0; i < GetChildrenCount(); i++)
    {
      nuiTreeNodePtr pNode = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
      if (pNode)
        pNode->Select(rRect, Sel, TemporarySelection);
    }
  }
}

void nuiTreeNode::SelectAllChildren(bool Selected)
{
  for (uint32 i = 0; i < GetChildrenCount(); i++)
  {
    nuiTreeNodePtr pChild = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
    if (pChild)
    {
      pChild->Select(Selected);
      pChild->SelectAllChildren(Selected);
    }
  }
}

void nuiTreeNode::OpenAllChildren(bool Opened)
{
  for (uint32 i = 0; i < GetChildrenCount(); i++)
  {
    nuiTreeNodePtr pChild = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
    if (pChild)
    {
      pChild->Open(Opened);
      pChild->OpenAllChildren(Opened);
    }
  }
}

void nuiTreeNode::SelectAllVisibleChildren(bool Selected)
{
  if (IsOpened())
  {
    for (uint32 i = 0; i < GetChildrenCount(); i++)
    {
      nuiTreeNodePtr pChild = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
      if (pChild)
      {
        pChild->Select(Selected);
        pChild->SelectAllChildren(Selected);
      }
    }
  }
}


void nuiTreeNode::SetPosition(nuiSize Position)
{
  mPosition = Position;
}

void nuiTreeNode::SetSize(nuiSize Size)
{
  mSize = Size;
}

nuiSize nuiTreeNode::GetPosition() const
{
  return mPosition;
}

nuiSize nuiTreeNode::GetSize() const
{
  return mSize;
}

nuiTreeNodePtr nuiTreeNode::FindNode(nuiSize X, nuiSize Y)
{
  nuiSize y = GetPosition();
  nuiSize h = GetSize();
  if ((y <= Y) && ((y + h) >= Y))
  {
    // The node we're looking for is in this sub tree...
    if (IsOpened())
    {
      for (uint32 i = 0; i < GetChildrenCount(); i++)
      {
        nuiTreeNodePtr pNode = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
        if (pNode)
        {
          nuiTreeNodePtr pResult = pNode->FindNode(X,Y);
          if (pResult) // Do we have a valid result?
            return pResult;
        }
      }
    }

    return this; // No sub node was found so it's the parent node...
  }
  return NULL;
}

void nuiTreeNode::GetSelected(std::list<nuiTreeNodePtr>& rSelected, bool IncludeInvisibleItem)
{
  if (IsSelected())
    rSelected.push_back(this);

  if (IsOpened() || IncludeInvisibleItem)
  {
    for (uint32 i = 0; i < GetChildrenCount(); i++)
    {
      nuiTreeNodePtr pNode = dynamic_cast<nuiTreeNodePtr>(GetChild(i));
      if (pNode)
        pNode->GetSelected(rSelected);
    }
  }
}

nuiWidgetPtr nuiTreeNode::GetSubElement(uint32 index)
{
  return NULL;
}


/////////////////////////////
// nuiTreeView
nuiTreeView::nuiTreeView(nuiTreeNodePtr pTree, bool displayRoot)
: nuiSimpleContainer(),
  mTreeViewSink(this),
  mDisplayRoot(displayRoot),
  mpSelectedNode(NULL)
{
  SetObjectClass(_T("nuiTreeView"));
  mMultiSelectable = false;
  mInMultiSelection = false;
  mDeSelectable = true;
  SetWantKeyboardFocus(true);
  
  mClickX = mClickY = mNewX = mNewY= 0;
  mClicked = mDrawMarkee = false;
  
  mpTree = pTree;

  ReparentTree(mpTree);

  AddEvent(_T("TreeViewClicked"), Clicked);
  NUI_ADD_EVENT(Activated);
  NUI_ADD_EVENT(SelectionChanged);
}

bool nuiTreeView::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  mpSelectedNode = NULL;
  mMultiSelectable = true;
  mInMultiSelection = false;
  mDeSelectable = true;

  mClickX = mClickY = mNewX = mNewY= 0;
  mClicked = mDrawMarkee = false;
  
  mpTree = NULL;

  AddEvent(_T("TreeViewClicked"), Clicked);
  NUI_ADD_EVENT(Activated);
  NUI_ADD_EVENT(SelectionChanged);
  return true;
}

nuiTreeView::~nuiTreeView()
{
  if (mpTree)
    delete mpTree;
}


nuiXMLNode* nuiTreeView::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiWidget::Serialize(pParentNode, Recursive);
  if (mpTree)
    mpTree->Serialize(pNode);
  return pNode;
}


bool nuiTreeView::Draw(nuiDrawContext* pContext)
{
  pContext->ResetState();

  DrawTree(pContext, 0, mpTree);

  if (mDrawMarkee)
  {
    nuiRect Markee(mClickX, mClickY, mNewX, mNewY, false);
    nuiTheme* pTheme = GetTheme();
    NGL_ASSERT(pTheme);
    pTheme->DrawMarkee(pContext, Markee, eSelectionMarkee);
    pTheme->Release();
  }

  return true;
}


bool nuiTreeView::DrawTree(nuiDrawContext* pContext, uint32 Depth, nuiTreeNode* pTree)
{
  if (!pTree)
    return false;

  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);

  nuiWidgetPtr pWidget = pTree->GetElement();
  if (pWidget && ((Depth != 0) || mDisplayRoot))
  {
    nuiRect rect = pWidget->GetRect();

    if (pTree->IsSelected())
      pTheme->DrawSelectionBackground(pContext, rect);


    if (!pTree->IsEmpty() || pTree->mAlwaysDisplayTreeHandle)
    {
      nuiRect r = rect;
      r.Move(-NUI_TREEVIEW_HANDLE_SIZE, 0);
      r.SetSize(NUI_TREEVIEW_HANDLE_SIZE, r.GetHeight());
      pContext->SetFillColor(GetColor(eTreeViewHandle));
      pContext->SetStrokeColor(GetColor(eTreeViewHandle));
      if (pTree->IsTreeHandleDrawned())
        pTheme->DrawTreeHandle(pContext, r, pTree->IsOpened(), NUI_TREEVIEW_HANDLE_SIZE);
    }
    DrawChild(pContext, pWidget);

    // Draw the sub elements:
    for (uint32 i = 0; i < mSubElements.size(); i++)
    {
      nuiWidgetPtr pWidget = pTree->GetSubElement(i);
      if (pWidget)
        DrawChild(pContext, pWidget);
    }
    
    
    if (pTree->IsSelected())
      pTheme->DrawSelectionForeground(pContext, rect);
    
  }

  if (pTree->IsOpened())
  {
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
      if (pNode)
      {
        DrawTree(pContext, Depth+1, pNode);
      }
    }
  }
  pTheme->Release();
  return true;
}


nuiRect nuiTreeView::CalcIdealSize()
{
  mIdealRect = nuiRect();

  for (uint32 i = 0; i < mSubElements.size(); i++)
    mSubElements[i].mIdealWidth = 0;
  
  CalcTreeSize(mIdealRect, 0, mpTree);
  mTreeIdealWidth = mIdealRect.GetWidth();
  
  for (uint32 i = 0; i < mSubElements.size(); i++)
    mIdealRect.Right() += mSubElements[i].mIdealWidth;  
  
  return mIdealRect;
}

void nuiTreeView::CalcTreeSize(nuiRect& rRect, uint32 Depth, nuiTreeNode* pTree)
{
  if (!pTree)
    return;
  nuiWidgetPtr pWidget = pTree->GetElement();
  
  uint32 depthInset = ((Depth != 0) || mDisplayRoot)? Depth-1 : Depth;
  
  if (pWidget && ((Depth != 0) || mDisplayRoot))
  {
    nuiRect WidgetRect = pWidget->GetIdealRect().Size();
    
    WidgetRect.Set(0.0f, 0.0f, WidgetRect.GetWidth() + GetDepthInset(depthInset), (nuiSize)ToAbove(WidgetRect.GetHeight() + NUI_TREEVIEW_INTERLINE));

    WidgetRect.RoundToAbove();
    rRect.SetWidth(MAX(rRect.GetWidth(), WidgetRect.GetWidth()));

    nuiSize h = WidgetRect.GetHeight();
    for (uint32 i = 0; i < mSubElements.size(); i++)
    {
      nuiWidgetPtr pWidget = pTree->GetSubElement(i);
      if (pWidget)
      {
        nuiRect r = pWidget->GetIdealRect();
        mSubElements[i].mIdealWidth = MAX(mSubElements[i].mIdealWidth, r.GetWidth());
        h = MAX(r.GetHeight(), WidgetRect.GetHeight());
      }
    }
    rRect.SetHeight(rRect.GetHeight() + h);

  }

  if (pTree->IsOpened())
  {
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
      if (pNode)
      {
        CalcTreeSize(rRect, Depth + 1, pNode);
      }
    }
  }
}

bool nuiTreeView::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  if (!mSubElements.empty())
  {
    mSubElements[0].mPosition = mTreeIdealWidth;
    for (uint32 i = 1; i < mSubElements.size(); i++)
    {
      mSubElements[i].mWidth = mSubElements[i].mIdealWidth;
      mSubElements[i].mPosition = mSubElements[i - 1].mPosition + mSubElements[i].mWidth;
    }      
  }
    
  
  nuiSize Y = 0;
  SetTreeRect(Y, 0, mpTree);

  return true;
}

void nuiTreeView::SetTreeRect(nuiSize& Y, uint32 Depth, nuiTreeNode* pTree)
{
  if (!pTree)
    return;

  pTree->SetPosition(Y);
  
  uint32 depthInset = ((Depth != 0) || mDisplayRoot)? Depth-1 : Depth;
  

  nuiWidgetPtr pWidget = pTree->GetElement();
  if (pWidget && ((Depth != 0) || mDisplayRoot))
  {
    nuiRect WidgetRect = pWidget->GetIdealRect().Size();
    WidgetRect.Move((nuiSize)ToNearest(GetDepthInset(depthInset)), (nuiSize)ToNearest(Y + NUI_TREEVIEW_INTERLINE));
    if (WidgetRect.Right() > GetRect().GetWidth())
    {
      nuiSize width = GetRect().GetWidth() - WidgetRect.Left();
      if (width < 0)
        width = 0;
      WidgetRect.SetWidth(width);
    }
    
    WidgetRect.RoundToAbove();
    pWidget->SetLayout(WidgetRect);
  
    for (uint32 i = 0; i < mSubElements.size(); i++)
    {
      nuiWidgetPtr pWidget = pTree->GetSubElement(i);
      if (pWidget)
      {
        pWidget->GetIdealRect();
        nuiRect r(mSubElements[i].mPosition, WidgetRect.Top(), mSubElements[i].mWidth, WidgetRect.GetHeight());
        pWidget->SetLayout(r);
      }      
    }
                        
    Y = WidgetRect.Bottom();

  }


  if (pTree->IsOpened())
  {
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
      if (pNode)
      {
        SetTreeRect(Y, Depth + 1, pNode);
      }
    }
  }

  pTree->SetSize(Y - pTree->GetPosition());
}


bool nuiTreeView::OnTreeChanged(const nuiEvent& rEvent)
{
  InvalidateLayout();
  return false;
}

bool nuiTreeView::OnTreeChildAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTreeEvent->mpChild);

  if (pNode)
  {
    //NGL_OUT(_T("Adding TreeNode 0x%x\n"), pNode);
    nuiWidgetPtr pWidget = pNode->GetElement();
    if (pWidget)
    {
      //NGL_OUT(_T("Adding TreeNode widget 0x%x\n"), pWidget);
      AddChild(pWidget);
      pNode->SetOwnElement(false);
    }
    
    for (uint32 i = 0; i < mSubElements.size(); i++)
    {
      nuiWidgetPtr pWidget = pNode->GetSubElement(i);
      if (pWidget)
        AddChild(pWidget);
    }

  }

  return false;
}

bool nuiTreeView::OnTreeChildDeleted(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTreeEvent->mpChild);

  if (pNode)
  {
    nuiWidgetPtr pWidget = pNode->GetElement();
    if (pWidget)
      pWidget->Trash();
    for (uint32 i = 0; i < mSubElements.size(); i++)
    {
      nuiWidgetPtr pWidget = pNode->GetSubElement(i);
      if (pWidget)
        pWidget->Trash();
    }
  }
  if (mpSelectedNode == pNode)
    mpSelectedNode = NULL;

  return false;
}

void nuiTreeView::ReparentTree(nuiTreeNode* pTree)
{
  if (!pTree)
    return;

  nuiWidgetPtr pWidget = pTree->GetElement();
  if (pWidget && pWidget->GetParent() != this)
  {
    AddChild(pWidget);
    pTree->SetOwnElement(false);
  }
  for (uint32 i = 0; i < mSubElements.size(); i++)
  {
    nuiWidgetPtr pWidget = pTree->GetSubElement(i);
    if (pWidget)
      AddChild(pWidget);
  }
  

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    if (pNode)
    {
      ReparentTree(pNode);
    }
  }

  mTreeViewSink.DisconnectAll();
  
  if (mpTree)
  {
    mTreeViewSink.Connect(pTree->Changed, &nuiTreeView::OnTreeChanged);
    mTreeViewSink.Connect(pTree->ChildAdded, &nuiTreeView::OnTreeChildAdded);
    mTreeViewSink.Connect(pTree->ChildDeleted, &nuiTreeView::OnTreeChildDeleted);
  }
}

////// Interaction:

bool nuiTreeView::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft)
  {
    mNewX = mOldX = mClickX = X;
    mNewY = mOldY = mClickY = Y;

    nuiTreeNodePtr pNode = FindNode(X,Y);
    if (pNode)
    {
      if ((!pNode->IsEmpty() && pNode->IsTreeHandleDrawned()) || pNode->mAlwaysDisplayTreeHandle)
      {
        nuiWidgetPtr pWidget = pNode->GetElement();
        if (pWidget)
        {
          nuiRect r = pWidget->GetRect();
          r.Move(-NUI_TREEVIEW_HANDLE_SIZE, 0);
          r.SetSize(NUI_TREEVIEW_HANDLE_SIZE, r.GetHeight());
          if (r.IsInside(X,Y))
          {
            pNode->Open(!pNode->IsOpened());
            if (IsKeyDown(NK_LMOD) || IsKeyDown(NK_RMOD))
            {
              pNode->OpenAllChildren(pNode->IsOpened());
            }

            InvalidateLayout();
            return true;
          }
        }
      }

      if (!IsMultiSelectable() || !IsKeyDown(NK_LMOD) || !IsKeyDown(NK_RMOD))
      {
        if (IsMultiSelectable())
        {
          SelectAll(false);
        }
        else
        {
          
          if (mpSelectedNode && mpSelectedNode != pNode)
          {            
            nuiWidgetPtr pWidget = pNode->GetElement();
            if (!IsDeSelectable() && !pNode && pWidget && !pWidget->GetRect().IsInside(X,Y))
            {
            }
            else
            {
              mpSelectedNode->Select(false);
            }
          }
        }
      }

      nuiWidgetPtr pWidget = pNode->GetElement();
      if (IsDeSelectable() || ((pWidget && pWidget->GetRect().IsInside(X,Y)) && !pNode->IsSelected()))
      {
        pNode->Select(nuiRect(X,Y,1.0f,1.0f), !pNode->IsSelected());
      }
      
      mpSelectedNode = pNode->IsSelected()? pNode : NULL;
      if (Button & nglMouseInfo::ButtonDoubleClick)
      {
        nuiWidgetPtr pWidget = pNode->GetElement();
        if (pWidget && pWidget->GetRect().IsInside(X,Y))
          Selected(); ///< Double click => Selection Action!
        pNode->Activated();
      }
      else
      {
        mClicked = true;
        Grab();
      }

      Invalidate();

      Clicked(X,Y, Button); ///< This event is called whenever an item is clicked.
      SelectionChanged();
      return true;
    }
    else
    {
      if (IsMultiSelectable())
      {
        mClicked = true;
        Grab();
        
        Invalidate();
      }
    }
  }

  return false;
}

bool nuiTreeView::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (Button & nglMouseInfo::ButtonLeft && mClicked)
  {
    mClicked = false;
    mDrawMarkee = false;
    
    if (IsMultiSelectable())
    {
      // Final selection:
      Select(nuiRect(mClickX, mClickY, mNewX, mNewY, false), true);
      SelectionDone();
    }
    
    Ungrab();
    Invalidate();
    return true;
  }
  return false;
}

bool nuiTreeView::MouseMoved(nuiSize X, nuiSize Y)
{
  if (mClicked)
  {
    mNewX = X;
    mNewY = Y;
    if (IsMultiSelectable())
    {
      mDrawMarkee = true;
      Select(nuiRect(mClickX, mClickY, mOldX, mOldY, false), false, true); ///< Unselected old temp selection
      Select(nuiRect(mClickX, mClickY, mNewX, mNewY, false), true, true);  ///< Select Old temp selection

      mOldX = mNewX;
      mOldY = mNewY;
    }
    Invalidate();
    SetHotRect(nuiRect(mNewX, mNewY, 16.0f, 16.0f));
    return true;
  }
  return false;
}


nuiTreeNodePtr nuiTreeView::FindNode(nuiSize X, nuiSize Y)
{
  if (mpTree)
    return mpTree->FindNode(X, Y);
  return NULL;
}

void nuiTreeView::Select(const nuiRect& rRect, bool Sel, bool TempSelection)
{
  if (!mpTree)
    return;

  mpTree->Select(rRect, Sel, TempSelection);
  SelectionChanged();
}

void nuiTreeView::Select(nuiTreeNode* pNode, bool Sel, bool TempSelection)
{
  if (Sel)
  {
    if (!mMultiSelectable)
		{
			SelectionChanged.Disable();
      SelectAll(false);
			SelectionChanged.Enable();

		}
    pNode->Select(Sel, TempSelection);
    mpSelectedNode = pNode;
    SetHotRect(pNode->GetElement()->GetRect());
  }
  else
  {
    if (mDeSelectable)
    {
      pNode->Select(Sel, TempSelection);
      if (mpSelectedNode == pNode)
        mpSelectedNode = NULL;
    }
  }
	
	SelectionChanged();

}

void nuiTreeView::SelectAll(bool Selected)
{
  if (!mpTree)
    return;

  mpTree->Select(Selected, false);
  mpTree->SelectAllChildren(Selected);
  SelectionChanged();
  Invalidate();
}

bool nuiTreeView::IsMultiSelectable()
{
  return mMultiSelectable;
}

bool nuiTreeView::IsDeSelectable()
{
  return mDeSelectable;
}

void nuiTreeView::SetMultiSelectable(bool Set)
{
  mMultiSelectable = Set;
  SelectAll(false);
}

void nuiTreeView::SetDeSelectable(bool Set)
{
  mDeSelectable = Set;
}

void nuiTreeView::SetTree(nuiTreeNodePtr pTree, bool DeleteOldTree)
{
  if (DeleteOldTree)
    delete mpTree;
  mpTree = pTree;
  ReparentTree(pTree);
  mpSelectedNode = NULL;
  InvalidateLayout();
}

nuiTreeNodePtr nuiTreeView::GetTree()
{
  return mpTree;
}

void nuiTreeView::GetSelected(std::list<nuiTreeNodePtr>& rSelected, bool IncludeInvisibleItem)
{
  rSelected.clear();

  if (!mpTree)
    return;

  mpTree->GetSelected(rSelected);
}

const nuiTreeNode* nuiTreeView::GetSelectedNode() const
{
  return mpSelectedNode;
}

nuiTreeNode* nuiTreeView::GetSelectedNode()
{
  return mpSelectedNode;
}

void nuiTreeView::SetDepthInset(uint32 depth, nuiSize inset)
{
  uint32 size = (uint32)mDepthInsets.size();
  if (size <= depth)
  {
    nuiSize lastInset = size ? mDepthInsets.back() : NUI_TREEVIEW_DEPTH_INSET;
    while (size <= depth)
    {
      mDepthInsets.push_back(lastInset);
      size++;
    }
  }
  mDepthInsets[depth] = inset;
  InvalidateLayout();
}

nuiSize nuiTreeView::GetDepthInset(uint32 depth)
{
  uint32 size = (uint32)mDepthInsets.size();
  nuiSize inset = 0.f;
  uint32 i = 0;
  while (i < size && i <= depth)
  {
    inset += mDepthInsets[i];
    i++;
  }
  if (i <= depth)
  {
    nuiSize lastInset = size ? mDepthInsets.back() : NUI_TREEVIEW_DEPTH_INSET;
    inset += (depth-i+1) * lastInset;
  }
  return inset;
}

bool nuiTreeView::DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mMouseEventEnabled)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
    nuiSize x=X, y=Y;
    GlobalToLocal(x,y);
    if (!hasgrab)
    {
      nuiTreeNode* pNode = FindNode(x,y);
      if (pNode && pNode->GetElement() && pNode->GetElement()->DispatchMouseClick(X,Y, Button))
        return true;
    }
    bool ret = MouseClicked(x,y,Button);
    ret |= Clicked(x,y,Button);
    return ret;
  }
  return false;
}

nuiTreeNodePtr nuiTreeNode::GetNextInParent()
{
  nuiTreeNodePtr pParent = (nuiTreeNodePtr)GetParent();
  if (pParent)
  {
    int count = pParent->GetChildrenCount();
    for (int i = 0; i < count; i++)
    {
      if (pParent->GetChild(i) == this && (i+1<count))
      {
        // Return the next sibbling:
        nuiTreeNodePtr pSibbling = (nuiTreeNodePtr)pParent->GetChild(i+1);
        if (pSibbling)
          return pSibbling;
      }
    }
    
    return pParent->GetNextInParent();
  }
  
  return NULL;
}

nuiTreeNodePtr nuiTreeNode::GetPreviousInParent()
{
  nuiTreeNodePtr pParent = (nuiTreeNodePtr)GetParent();
  if (pParent)
  {
    int count = pParent->GetChildrenCount();
    for (int i = 1; i < count; i++)
    {
      if (pParent->GetChild(i) == this)
      {
        // Return the previous sibbling:
        nuiTreeNodePtr pSibbling = (nuiTreeNodePtr)pParent->GetChild(i-1);
        if (pSibbling)
          return pSibbling;
      }
    }
  }
  
  return NULL;
}

nuiTreeNodePtr nuiTreeNode::GetLastOpenChild()
{
  if (!IsOpened() || IsEmpty())
    return this;
  
  int count = GetChildrenCount();

  if (!count)
    return this;

  if (count)
  {
    nuiTreeNodePtr pLastChild = (nuiTreeNodePtr)GetChild(count - 1);
    
    return pLastChild->GetLastOpenChild();
  }
  
  return NULL;
}

nuiTreeNodePtr nuiTreeNode::GetNextOpen()
{
  if (IsOpened() && !IsEmpty() && GetChildrenCount()!=0)
  {
    nuiTreeNodePtr pChild = (nuiTreeNodePtr)GetChild(0);
    if (pChild)
      return pChild;
  }
  
  return GetNextInParent();
}

nuiTreeNodePtr nuiTreeNode::GetPreviousOpen()
{
  nuiTreeNodePtr pPrev = GetPreviousInParent();
  if (pPrev)
  {
    nuiTreeNodePtr pLastOpenChild = pPrev->GetLastOpenChild();
    if (pLastOpenChild)
      return pLastOpenChild;
  }
  
  return (nuiTreeNodePtr)GetParent();
}

bool nuiTreeView::KeyDown(const nglKeyEvent& rEvent)
{
  nuiTreeNodePtr pSelected = mpSelectedNode;
  if (!pSelected)
    pSelected = mpTree;
  if (!pSelected)
    return false;
  
  if (rEvent.mKey == NK_UP)
  {
    nuiTreeNodePtr pPrev = pSelected->GetPreviousOpen();
    if (pPrev)
    {
      if (mDisplayRoot || pPrev != mpTree)
      {
        if (mInMultiSelection)
          SelectAll(false);
        Select(pPrev, true, false);
      }
    }
    return true;
  }
  else if (rEvent.mKey == NK_DOWN)
  {
    nuiTreeNodePtr pNext = pSelected->GetNextOpen();
    if (pNext)
    {
      if (mDisplayRoot || pNext != mpTree)
      {
        if (mInMultiSelection)
          SelectAll(false);
        Select(pNext, true, false);
      }
    }
    return true;
  }
  else if (rEvent.mKey == NK_RIGHT)
  {
    if (!pSelected->IsOpened())
    {
      // Close the selected node
      pSelected->Open(true);
      if (IsKeyDown(NK_LMOD) || IsKeyDown(NK_RMOD))
      {
        pSelected->OpenAllChildren(true);
      }
    }
    else
    {
      if (pSelected->GetChildrenCount())
      {
        nuiTreeNodePtr pChild = (nuiTreeNodePtr)pSelected->GetChild(0);
        if (pChild)
        {
          if (mInMultiSelection)
            SelectAll(false);
          Select(pChild, true, false);
        }
      }
    }
    return true;
  }
  else if (rEvent.mKey == NK_LEFT)
  {
    if (pSelected->IsOpened())
    {
      // Close the selected node
      pSelected->Open(false);
    }
    else
    {
      nuiTreeNodePtr pParent = (nuiTreeNodePtr)pSelected->GetParent();
      if (mDisplayRoot || pParent != mpTree)
      {
        if (pParent)
        {
          if (mInMultiSelection)
            SelectAll(false);
          Select(pParent, true, false);
        }
      }
    }
    return true;
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    if (mpSelectedNode)
      mpSelectedNode->Activated();
    return true;
  }
  return false;
}

bool nuiTreeView::KeyUp(const nglKeyEvent& rEvent)
{
  return false;
}

void nuiTreeView::StartMultiSelection()
{
  mInMultiSelection = true;
}

void nuiTreeView::StopMultiSelection()
{
  mInMultiSelection = false;
}

bool nuiTreeView::IsMultiSelecting() const
{
  return mInMultiSelection;
}

void nuiTreeView::EnableSubElements(uint32 count)
{
  mSubElements.resize(count, SubElement(mDefaultSubElementWidth));
}

nuiSize nuiTreeView::mDefaultSubElementWidth = 32;

nuiTreeView::SubElement::SubElement(nuiSize width)
{
  mWidth = width;
  mIdealWidth = width;
  mPosition = -1;
  mMinWidth = 0;
  mMaxWidth = -1;
}

