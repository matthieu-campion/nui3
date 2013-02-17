/*
NUI3 - C++ cross-platform GUI framework for OpenGL based applications
Copyright (C) 2002-2003 Sebastien Metrot

licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define TIMER_PERIOD                (0.05)
#define DELAY_TIME                  (0.2)
#define NUI_POPUP_MARGIN            (4)
#define NUI_POPUP_TREE_DEPTH_INSET  (12.0f)
#define NUI_POPUP_TREE_INTERLINE    (3.0f)
#define NUI_POPUP_TREE_HANDLE_SIZE  (12.0f)

#define SB_WIDTH          (8.0f)
#define AUTOSCROLL_HEIGHT (16.0f)


nuiPopupMenu::nuiPopupMenu(nuiWidget* pParent, nuiTreeNodePtr pTree, const nuiRect& rRect, bool OwnTree)
: nuiSimpleContainer(),
mPopupTreeSink(this),
mpSelectedNode(NULL),
mpNewSelectedNode(NULL),
mOwnTree(OwnTree),
mSelectionTimer(nglTime(TIMER_PERIOD)),
mScrollTimer(1.0f/15.0f),
mDelayTime(DELAY_TIME),
mpOldFocused(NULL)
{
  mTrashRemoval = false;
  SetObjectClass(_T("nuiPopupMenu"));
  nuiRect r = rRect;

  nuiTopLevel* pTop = NULL;
  if (pParent)
    pTop = pParent->GetTopLevel();
  if (pTop)
    pTop->AddChild(this);

  mpTree = pTree;
  mpTree->Acquire();
  ReparentTree(mpTree);
  mSelectedNodes.push_back(mpTree);
  mXdir = 1;
  mYdir = 1;
  if (pParent)
    pParent->LocalToGlobal(r);
  mInitialPos = r;

  mScrollUp = false;
  mPopupTreeSink.Connect(mScrollTimer.Tick, &nuiPopupMenu::OnScrollTimer);
  mPopupTreeSink.Connect(mSelectionTimer.Tick, &nuiPopupMenu::OnSelectionTimer);

  mScrolledFromKeyboard = false;
  mShowFirstNode = true;
  mWheelMoved = false;
  mFirstClick = false;

  //nuiTopLevel* pTopLevel = GetTopLevel();
  //if (pTopLevel)
  //mpOldFocused = pTopLevel->GetFocus();
  SetWantKeyboardFocus(true);
  Focus();
  //  Grab();
}

nuiPopupMenu::~nuiPopupMenu()
{
  mSelectionTimer.Stop();
  mScrollTimer.Stop();

  if (mpTree)
  {
    mpTree->Release();
  }

  for (std::vector<nuiMenuRect*>::iterator i = mRects.begin(); i != mRects.end(); )
  {
    //DelChild(*i)->mpSBar);
    //(*i)->mpSBar->Trash();
    delete *i;  
    i = mRects.erase(i);
  }
  if (mpOldFocused)
  {
    mpOldFocused->Focus();
  }
  //  Ungrab();
}


bool nuiPopupMenu::Draw(nuiDrawContext* pContext)
{
  nuiRect r = GetRect();

  NGL_ASSERT(!mRects.empty());
  nuiMenuRect* pRect = mRects[0];

  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);

  pTheme->DrawMenuWindow(pContext, pRect->mRect, this);
  if (pRect->mpSBar->IsVisible())
    DrawChild(pContext, pRect->mpSBar);
  //it++;

  if (mShowFirstNode)
  {
    nuiWidgetPtr pWidget = mpTree->GetElement();
    NGL_ASSERT(pWidget);
    nuiRect rect = pWidget->GetRect();
    //Draw the main menu item (tree root item)
    pContext->SetFillColor(GetColor(eMenuTitleBg));
    pContext->DrawRect(rect, eStrokeAndFillShape);

    pContext->EnableBlending(true);
    nuiReflection ref(1.0f, .4f);
    ref.Draw(pContext, rect);
    pContext->EnableBlending(false);

    DrawChild(pContext, pWidget);
  }

  DrawTree(pContext, mpTree, 0);
  pTheme->Release();
  return true;
}

bool nuiPopupMenu::DrawTree(nuiDrawContext* pContext, nuiTreeNode* pTree, uint depth)
{
  nuiTreeNode*  pChildNode;
  nuiTreeNode*  pOpenedNode = NULL;
  nuiWidget*    pChildWidget;    
  nuiRect       WidgetRect;

  NGL_ASSERT(depth < mRects.size());
  nuiMenuRect* pRect= mRects[depth];

  NGL_ASSERT(pTree);
  nuiTheme* pTheme = GetTheme();
  NGL_ASSERT(pTheme);

  if (pTree != mpTree) // then Draw a new rect for this depth
  {
    pTheme->DrawMenuWindow(pContext, pRect->mRect, this);
    if (pRect->mpSBar->IsVisible(false))
      DrawChild(pContext, pRect->mpSBar);
  }

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    pChildNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    pChildWidget = pChildNode->GetElement();
    WidgetRect = pChildWidget->GetRect();

    if (pChildNode->IsSelected() || pChildNode->IsOpened())
    {
      nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
      nuiRect r = WidgetRect;
      r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

      pTheme->DrawSelectionBackground(pContext, r, this);
    }

    if (!pChildNode->IsEmpty())
    {
      nuiRect r = WidgetRect;
      r.Move(WidgetRect.GetWidth(), 0);
      r.SetSize(NUI_POPUP_TREE_HANDLE_SIZE-4.f, r.GetHeight());
      pContext->SetFillColor(GetColor(eTreeViewHandle));
      pTheme->DrawTreeHandle(pContext, r, false, NUI_POPUP_TREE_HANDLE_SIZE-4.f);
    }

    DrawChild(pContext, pChildWidget);

    if (pChildNode->IsSelected() || pChildNode->IsOpened())
    {
      nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
      nuiRect r = WidgetRect;
      r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

      pTheme->DrawSelectionForeground(pContext, r, this);
    }



    if (pChildNode->IsOpened())
    {
      pOpenedNode = pChildNode;
    }
  }

  if (pRect->mpSBar->IsVisible(false))
  {
    SetFillColor(pContext, eMenuBg);
    SetStrokeColor(pContext, eNormalTextFg);
    pContext->EnableBlending(true);
  
    nuiGradient grad;
    nuiColor bg(GetColor(eMenuBg));
    grad.AddStop(bg, 1);
    bg.Multiply(0);
    grad.AddStop(bg, 0);

    if (pRect->mpSBar->GetRange().GetValue() + pRect->mpSBar->GetRange().GetPageSize() < pRect->mpSBar->GetRange().GetMaximum())
    {
      nuiRect r(pRect->mRect.Left() + 2, mRect.Bottom() - AUTOSCROLL_HEIGHT * 2, pRect->mRect.GetWidth() - SB_WIDTH - 2, AUTOSCROLL_HEIGHT * 2);
      pContext->DrawGradient(grad, r, 0, mRect.Bottom() - AUTOSCROLL_HEIGHT * 2, 0, mRect.Bottom());

      float w = pRect->mRect.GetWidth() - SB_WIDTH;
      float x = pRect->mRect.Left();
      float y = mRect.Bottom() - AUTOSCROLL_HEIGHT;
      float s = 8;
      pContext->DrawLine(x + w/2 - s, y + 4, x + w/2 + s, y + 4);
      pContext->DrawLine(x + w/2 + s, y + 4, x + w/2, y + 4 + s);
      pContext->DrawLine(x + w/2, y + 4 + s, x + w/2 - s, y + 4);
    }

    if (pRect->mpSBar->GetRange().GetValue() > pRect->mpSBar->GetRange().GetMinimum())
    {
      nuiRect r(pRect->mRect.Left() + 2, 0.0f, pRect->mRect.GetWidth() - SB_WIDTH - 2, AUTOSCROLL_HEIGHT * 2);
      pContext->DrawGradient(grad, r, 0, AUTOSCROLL_HEIGHT * 2, 0, 0);

      float w = pRect->mRect.GetWidth() - SB_WIDTH;
      float x = pRect->mRect.Left();
      float s = 8;
      pContext->DrawLine(x + w/2, 4, x + w/2 - s, 4 + s);
      pContext->DrawLine(x + w/2 - s, 4 + s, x + w/2 + s, 4 + s);
      pContext->DrawLine(x + w/2 + s, 4 + s, x + w/2, 4);
    }

    pContext->EnableBlending(false);
}

  if (pOpenedNode)
    DrawTree(pContext, pOpenedNode, depth+1);

  pTheme->Release();
  return true;
}

nuiRect nuiPopupMenu::CalcIdealSize()
{
  if (mTrashRemoval)
    return nuiRect();

  uint32 cpt = 0;
  nuiRect rect = mInitialPos; // this rect is for initial position of the menu
  if (mRects.empty())
  {
    mRects.push_back(new nuiMenuRect(this, 0));
    mPopupTreeSink.Connect(mRects[0]->mpSBar->ValueChanged, &nuiPopupMenu::OnScrollBarChange, mRects[0]);
    mRects[0]->mpFromNode = mpTree;
  }

  NGL_ASSERT(mpTree);
  nuiWidgetPtr pWidget = mpTree->GetElement();
  NGL_ASSERT(pWidget);
  nuiRect WidgetRect = pWidget->GetIdealRect();
  if (mShowFirstNode)
    rect.SetSize((nuiSize)ToAbove(WidgetRect.GetWidth() + NUI_POPUP_MARGIN), (nuiSize)ToAbove(WidgetRect.GetHeight())); //size of the first element we wish not to be selectable
  else
    rect.SetSize((nuiSize)0,(nuiSize)0); //size of the first element we wish not to be selectable

  bool HasNonEmpty = false;
  uint32 count = mpTree->GetChildrenCount();
  nuiRect GlobRect = rect;

  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(mpTree->GetChild(i));

    if (pNode)
    {
      pWidget = pNode->GetElement();
      WidgetRect = pWidget->GetIdealRect();
      rect.SetSize(MAX(rect.GetWidth(), WidgetRect.GetWidth()), 
        rect.GetHeight() + WidgetRect.GetHeight());
      if (!pNode->IsEmpty())
        HasNonEmpty = true;
      if (pNode->IsOpened())
      {
        if (mRects.size() <= cpt + 1) // ensure that there is a rect for the next node
        {
          nuiMenuRect* pMenuRect = new nuiMenuRect(this, cpt+1);
          mRects.push_back(pMenuRect);
          mPopupTreeSink.Connect(pMenuRect->mpSBar->ValueChanged, &nuiPopupMenu::OnScrollBarChange, pMenuRect);
        }
        mRects[cpt+1]->mpFromNode = pNode;
        NGL_ASSERT(mRects.size() > cpt+1);

        GlobRect = rect;
        CalcTreeSize(GlobRect, pNode, cpt);
      }
    }           
  }

  if (HasNonEmpty)
  {
    mRects[0]->mHasNonEmpty = true;
    rect.SetSize(rect.GetWidth() + NUI_POPUP_TREE_HANDLE_SIZE * 2, rect.GetHeight());
  }
  else
  {
    mRects[0]->mHasNonEmpty = false;
  }

  if (mInitialPos.GetWidth() > rect.GetWidth())
    rect.SetWidth(mInitialPos.GetWidth());

  mRects[0]->mRect = rect;

  if (cpt+1 < mRects.size())
  { // there is unused rects
    int eraser_countdown = cpt;
    for (std::vector<nuiMenuRect*>::iterator it = mRects.begin(); it != mRects.end(); eraser_countdown--)
    {
      if (eraser_countdown >= 0)
      {
        it++;
      }
      else
      {
        if((*it)->mpSBar)
        {
          (*it)->mpSBar->SetVisible(false);
          (*it)->mpSBar->SetEnabled(false);
          mPopupTreeSink.DisconnectSource((*it)->mpSBar->ValueChanged);
          mSBarsPool.push_back((*it)->mpSBar);
        }
        delete *it;
        it = mRects.erase(it);
      }
    }
  }
  mIdealRect = GlobRect;
  return mIdealRect;
}

/***********************************************
CF: This was an attempt at removing trashed objects automatically
from the tree. However, it results in trashed objects when reparenting
the tree. AddChild -> DelChild -> Trash...
Needs a fix.

bool nuiPopupMenu::DelChild(nuiWidgetPtr pChild,bool Delete)
{
  return DelChild(mpTree, pChild, Delete);
}

bool nuiPopupMenu::DelChild(nuiTreePtr pTree, nuiWidgetPtr pChild, bool Delete)
{
  uint32 count = pTree->GetChildrenCount();
  if (count <= 0)
    return false;

  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    nuiWidget* pWidget = pNode->GetElement();
    NGL_ASSERT(pWidget);
    if (pWidget == pChild)
    {
      bool res = pTree->DelChild(pNode, false);
      if (res)
      {
        res = nuiSimpleContainer::DelChild(pChild, Delete); 
      }
      return res;
    }

    if (pNode->IsOpened())
    {
      return DelChild(pNode, pChild, Delete);
    }
  }
  return false;
}*/

void nuiPopupMenu::CalcTreeSize(nuiRect& rRect, nuiTreeNode* pTree, uint32& cpt)
{
  NGL_ASSERT(pTree); // no chance to happen
  cpt++;
  nuiRect rect(0,0,0,0);
  nuiWidgetPtr pWidget;
  nuiRect WidgetRect;

  uint32 depth = cpt;

  uint32 count = pTree->GetChildrenCount();
  if (count <= 0)
    return;

  bool HasNonEmpty = false;

  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    pWidget = pNode->GetElement();
    NGL_ASSERT(pWidget);
    WidgetRect = pWidget->GetIdealRect();
    rect.SetSize(MAX(rect.GetWidth(), WidgetRect.GetWidth()), rect.GetHeight() + WidgetRect.GetHeight());
    if (!pNode->IsEmpty())
    {
      HasNonEmpty = true;
    }
    if (pNode->IsOpened())
    {
      if (mRects.size() <= depth + 1) // ensure that there is a rect for the next node
      {
        nuiMenuRect* pMenuRect = new nuiMenuRect(this, cpt+1);
        mRects.push_back(pMenuRect);
        mPopupTreeSink.Connect(pMenuRect->mpSBar->ValueChanged, &nuiPopupMenu::OnScrollBarChange, pMenuRect);
      }
      mRects[depth+1]->mpFromNode = pNode;
      CalcTreeSize(rRect, pNode, cpt);
    }
  }
  if (HasNonEmpty)
  {
    mRects[depth]->mHasNonEmpty = true;
    rect.SetSize(rect.GetWidth() + NUI_POPUP_TREE_HANDLE_SIZE * 2, rect.GetHeight());
  }
  else
  {
    mRects[depth]->mHasNonEmpty = false;
  }

  mRects[depth]->mRect = rect;
  NGL_ASSERT(mRects.size() >= depth+1);
  rRect.SetSize(rect.GetWidth()+rRect.GetWidth(), rect.GetHeight()+rRect.GetHeight());
}

void nuiPopupMenu::OnScrollBarChange(const nuiEvent& rEvent)
{
  nuiMenuRect* pMenuRect = (nuiMenuRect*)(rEvent.mpUser);

  if (mScrolledFromKeyboard)
  {
    mScrolledFromKeyboard = false;
    rEvent.Cancel();
    return;
  }
  pMenuRect->mpFromNode->OpenAllChildren(false);
  pMenuRect->mpFromNode->SelectAllChildren(false);

  Invalidate();
  InvalidateLayout();
  rEvent.Cancel();
}

nuiTreeNodePtr nuiPopupMenu::GetParentNode(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode)
{
  for (uint32 i = 0; i < pParent->GetChildrenCount(); i++)
  {
    nuiTreeNodePtr pChild = dynamic_cast<nuiTreeNodePtr>(pParent->GetChild(i));
    if (pChild == pNode)
      return pParent;
    pChild = GetParentNode(pChild, pNode);
    if (pChild)
      return pChild;
  }
  return NULL;
}

nuiSize nuiPopupMenu::AdjustPosToSize(nuiMenuRect* rPosRect)
{
  nuiTopLevel* pRoot = GetTopLevel();
  NGL_ASSERT(pRoot);
  nuiRect MainRect = pRoot->GetRect();

  // adjust horizontally
  if (rPosRect->mRect.Right() > MainRect.GetWidth())
  {
    rPosRect->mRect.Set(rPosRect->mRect.Left() - (rPosRect->mRect.Right() - MainRect.GetWidth()), 
      rPosRect->mRect.Top(), rPosRect->mRect.GetWidth(), rPosRect->mRect.GetHeight());  
  }
  // adjust vertically
  if (rPosRect->mRect.Bottom() > MainRect.GetHeight())
  {
    rPosRect->mRect.Set(rPosRect->mRect.Left(), rPosRect->mRect.Top() - (rPosRect->mRect.Bottom() - MainRect.GetHeight()) - NUI_POPUP_MARGIN,
      rPosRect->mRect.GetWidth(), rPosRect->mRect.GetHeight());
  }
  // check for scrollbar
  if (rPosRect->mRect.GetHeight() > MainRect.GetHeight())
  {
    nuiRect r( ToNearest(rPosRect->mRect.Right() - SB_WIDTH), 0, ToNearest(SB_WIDTH), ToNearest(MainRect.GetHeight()));
    rPosRect->mpSBar->GetRange().EnableEvents(false);
    rPosRect->mpSBar->GetRange().SetRange(0.f, rPosRect->mRect.GetHeight());
    rPosRect->mpSBar->GetRange().SetPageSize(MainRect.GetHeight());
    rPosRect->mpSBar->GetRange().SetIncrement(10.f);
    rPosRect->mpSBar->GetRange().EnableEvents(true);

    rPosRect->mpSBar->SetLayout(r);
    rPosRect->mpSBar->SetVisible(true);
    rPosRect->mpSBar->SetEnabled(true);

    nuiSize decal = rPosRect->mpSBar->GetRange().GetValue();
    rPosRect->mRect.Set(rPosRect->mRect.Left(), 0.f,
      rPosRect->mRect.GetWidth(), rPosRect->mRect.GetHeight());
    return (decal);
  }
  else if (rPosRect->mpSBar)
  {
    rPosRect->mpSBar->SetVisible(false);
    rPosRect->mpSBar->SetEnabled(false);
  }

  return -1.f;
}

bool nuiPopupMenu::SetRect(const nuiRect& rRect)
{
  nuiRect r = rRect;
  nuiWidget::SetRect(r);
  mXdir = 1;
  mYdir = 1;
  nuiSize X, Y;
  X = Y = 0;

  NGL_ASSERT(mpTree);

  NGL_ASSERT(!mRects.empty());
  nuiMenuRect* pRect = mRects[0];

  nuiWidgetPtr pWidget = mpTree->GetElement();
  NGL_ASSERT(pWidget);

  nuiRect WidgetRect = pWidget->GetIdealRect();

  nuiSize decal = AdjustPosToSize(pRect);
  nuiRect PopupRect(pRect->mRect);
  mInitialPos = pRect->mRect;

  WidgetRect.Set( ToNearest(pRect->mRect.Left()), 
    ToNearest(pRect->mRect.Top() - (decal >= 0.f ? decal : 0.f)), 
    ToNearest(pRect->mRect.GetWidth() - (decal >= 0.f ? SB_WIDTH : 0.f)),
    ToNearest(WidgetRect.GetHeight()));
  pWidget->SetLayout(WidgetRect);

  if (mShowFirstNode)
  {
    X = WidgetRect.Left();
    Y = WidgetRect.Bottom();
  }
  else
  {
    X = mInitialPos.Left();
    Y = mInitialPos.Top() - (decal >= 0.f ? decal : 0.f);
  }

  uint32 count = mpTree->GetChildrenCount();
  if (count <= 0)
    return true;

  nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(mpTree->GetChild(i));
    NGL_ASSERT(pNode);
    pWidget = pNode->GetElement();
    NGL_ASSERT(pWidget)
      WidgetRect = pWidget->GetIdealRect();
    WidgetRect.Set(ToNearest(X), 
      ToNearest(Y), 
      ToNearest(PopupRect.GetWidth() - (decal >= 0.f ? SB_WIDTH : 0.f) - TreeHandleSize), 
      ToNearest(WidgetRect.GetHeight()));
    pWidget->SetLayout(WidgetRect);
    if (pNode->IsOpened())
      SetTreeRect(0, X, Y, pNode);
    Y += WidgetRect.GetHeight();
  }
  return true;
}

nuiSize nuiPopupMenu::AdjustRectsPos(nuiSize& rX, nuiSize& rY, uint depth, nuiRect CurRect)
{
  nuiTopLevel* pRoot = GetTopLevel();
  NGL_ASSERT(pRoot);
  NGL_ASSERT(depth+1 < mRects.size());
  nuiRect MainRect = pRoot->GetRect();
  nuiRect& prevRect(mRects[depth]->mRect); // prevRect is the previous rect in the hierarchy (NOT the previous rect of this sub menu)
  depth++;

  nuiMenuRect* pMenuRect = mRects[depth];
  nuiRect& rRect(pMenuRect->mRect);

  nuiSize x, y;
  x = rX + (mXdir * rRect.GetWidth()) - (mXdir < 0 ? prevRect.GetWidth() : 0); // Compute X so that the rect doesn't get out of the main container
  y = rY + (mYdir * rRect.GetHeight()) + (mYdir < 0 ? CurRect.GetHeight() : 0); // Compute Y so that the rect doesn't get out of the main container
  if (x > MainRect.GetWidth() || x < 0) // Should we change the current x direction?
    mXdir *= -1;
  if (y > MainRect.GetHeight() || y < 0)  // Should we change the current y direction?
    mYdir *= -1;

  rX = rX - ((mXdir < 0) ? rRect.GetWidth()  + prevRect.GetWidth() : 0);
  rY = rY - ((mYdir < 0) ? rRect.GetHeight() - CurRect.GetHeight() : 0);

  if (rY < 0)
    rY = 0;
  if ( 
    ( (mYdir > 0) && (MainRect.GetHeight() < (rY + rRect.GetHeight()) ) ) ||
    ( (mYdir < 0) && (0 > (rY - rRect.GetHeight()) ) )
    )
  {
    nuiSize heightDiff = MainRect.GetHeight() - rRect.GetHeight();
    if (heightDiff > 0)
    {
      rY = heightDiff;
    }
    else
    {
      nuiRect r( ToNearest(rX + rRect.GetWidth() - SB_WIDTH), 0, ToNearest(SB_WIDTH), ToNearest(MainRect.GetHeight()));
      pMenuRect->mpSBar->GetRange().SetRange(0.f, rRect.GetHeight());
      pMenuRect->mpSBar->GetRange().SetPageSize(MainRect.GetHeight());
      pMenuRect->mpSBar->GetRange().SetIncrement(10.f);

      pMenuRect->mpSBar->SetLayout(r);
      pMenuRect->mpSBar->SetVisible(true);
      pMenuRect->mpSBar->SetSelected(false);
      pMenuRect->mpSBar->SetEnabled(true);
      rY = -pMenuRect->mpSBar->GetRange().GetValue();
      return (SB_WIDTH);
    }
  }
  if (pMenuRect->mpSBar)
  {
    pMenuRect->mpSBar->SetVisible(false);
    pMenuRect->mpSBar->SetEnabled(false);
  }
  return (0.f);
}

void nuiPopupMenu::SetTreeRect(uint depth, nuiSize X, nuiSize Y, nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  NGL_ASSERT(depth < mRects.size());

  nuiWidgetPtr  pWidget;
  nuiRect       WidgetRect;
  nuiSize       SbWidth = 0.f;

  X += mRects[depth]->mRect.GetWidth();
  SbWidth = AdjustRectsPos(X, Y, depth, (pTree->GetElement())->GetRect());

  depth++;
  nuiMenuRect*  pRect= mRects[depth]; /// get the current menuRect

  pRect->mRect.Set(X, Y, pRect->mRect.GetWidth(), pRect->mRect.GetHeight());
  nuiRect PopupRect(pRect->mRect);

  nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));

    NGL_ASSERT(pNode);
    pWidget = pNode->GetElement();

    NGL_ASSERT(pWidget)
      WidgetRect = pWidget->GetIdealRect();
    WidgetRect.Set(ToNearest(X), ToNearest(Y), ToNearest(PopupRect.GetWidth() - SbWidth - TreeHandleSize), ToNearest(WidgetRect.GetHeight()));
    pWidget->SetLayout(WidgetRect);
    if (pNode->IsOpened())
    {
      SetTreeRect(depth, X, Y, pNode);
    }
    Y += WidgetRect.GetHeight();
  }
}

void nuiPopupMenu::OnTreeChanged(const nuiEvent& rEvent)
{
  Invalidate();
  InvalidateLayout();
}

void nuiPopupMenu::OnTreeChildAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTreeEvent->mpChild);

  if (pNode)
  {
    nuiWidgetPtr pWidget = pNode->GetElement();
    if (pWidget)
    {
      AddChild(pWidget);
      if (mOwnTree)
        pNode->SetOwnElement(false);
    }
  }
}

void nuiPopupMenu::OnTreeChildDeleted(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTreeEvent->mpChild);

  if (pNode)
  {
    nuiWidgetPtr pWidget = pNode->GetElement();
    if (pWidget)
      pWidget->Trash();
  }
  if (mpSelectedNode == pNode)
    mpSelectedNode = NULL;
}

void nuiPopupMenu::ReparentTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  nuiWidgetPtr pWidget = pTree->GetElement();
  NGL_ASSERT(pWidget);
  AddChild(pWidget);
  if (mOwnTree)
    pTree->SetOwnElement(false);
  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    ReparentTree(pNode);
  }
  NGL_ASSERT(mpTree);
  mPopupTreeSink.Connect(pTree->Changed, &nuiPopupMenu::OnTreeChanged);
  mPopupTreeSink.Connect(pTree->ChildAdded, &nuiPopupMenu::OnTreeChildAdded);
  mPopupTreeSink.Connect(pTree->ChildDeleted, &nuiPopupMenu::OnTreeChildDeleted);
}

void nuiPopupMenu::UnparentTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  nuiWidgetPtr pWidget = pTree->GetElement();
  if (pWidget->GetParent() == this)
  {
    pTree->Select(false);
    pTree->Open(false);
    NGL_ASSERT(pWidget);
    DelChild(pWidget);
  }
  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    UnparentTree(pNode);
  }
  NGL_ASSERT(mpTree);
  mPopupTreeSink.DisconnectSource(pTree->Changed);
  mPopupTreeSink.DisconnectSource(pTree->ChildAdded);
  mPopupTreeSink.DisconnectSource(pTree->ChildDeleted);
}

void nuiPopupMenu::FillSelectedNodes()
{
  mSelectedNodes.clear();

  if (!mpSelectedNode)
    return;
  mSelectedNodes.push_back(mpSelectedNode);

  nuiTreeNode* pNode = mpSelectedNode;
  int cpt = 1;
  while (pNode && ((pNode = GetParentNode(mpTree, pNode)) != mpTree))
  {
    mSelectedNodes.push_back(pNode);
    cpt++;
  }
  mSelectedNodes.push_back(mpTree);
}

void nuiPopupMenu::ResetScrolling(uint32 rect)
{
  if (mRects.size() > rect)
  {
    mRects[rect]->mpSBar->GetRange().SetValue(0.f);
  }
}

void nuiPopupMenu::OnSelectionTimer(const nuiEvent& rEvent)
{
  mSelectionTimer.Stop();
  mSelectionTimer.SetPeriod(nglTime(TIMER_PERIOD));
  if (!mpNewSelectedNode || (mpSelectedNode && (mpSelectedNode == mpNewSelectedNode))) 
  {
    rEvent.Cancel();
    return;
  }

  nuiTreeNodePtr pParent = GetParentNode(mpTree, mpNewSelectedNode);
  if (pParent)
  {
    pParent->OpenAllChildren(false);
    pParent->SelectAllChildren(false);
  }
  mpNewSelectedNode->Select(true);

  if (!mpNewSelectedNode->IsEmpty())
  {
    mpNewSelectedNode->Open(true);
    if (!mpNewSelectedNode->GetChildrenCount())
    {
      mpNewSelectedNode->Open(false);
    }
    else
    {
      ResetScrolling(mpNewSelectedNode->GetDepth()-1);
      Invalidate();
      InvalidateLayout();
    }

  }

  if (mpSelectedNode)
  {
    nuiRect WidgetRect = mpSelectedNode->GetElement()->GetRect();
    nuiRect r = WidgetRect;
    nuiMenuRect*  pRect= mRects[mpSelectedNode->GetDepth()-1]; /// get the current menuRect
    nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
    r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

    Invalidate(); //(r);
    mpSelectedNode->GetElement()->Invalidate();
  }
  if (mpNewSelectedNode)
  {
    nuiRect WidgetRect = mpNewSelectedNode->GetElement()->GetRect();
    nuiMenuRect*  pRect= mRects[mpNewSelectedNode->GetDepth()-1]; /// get the current menuRect
    nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
    nuiRect r = WidgetRect;
    r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

    Invalidate(); //(r);
    mpNewSelectedNode->GetElement()->Invalidate();
  }

  mpSelectedNode = mpNewSelectedNode;
  FillSelectedNodes();

  //Invalidate();
  rEvent.Cancel();
}

void nuiPopupMenu::OnScrollTimer(const nuiEvent& rEvent)
{
  IncrementScrollBar(!mScrollUp);
}

bool nuiPopupMenu::MouseMoved(nuiSize X, nuiSize Y)
{
  bool IsInside;
  int cpt = SearchRectIndex(X,Y);
  IsInside = cpt >= 0 ? true : false;
  if (IsInside)
  {
    // Do the auto scroll feature:
    nuiMenuRect* pMenuRect = mRects[cpt];
    NGL_ASSERT(pMenuRect);
    if (pMenuRect->mpSBar->IsVisible())
    {
      nuiScrollBar* pSB = pMenuRect->mpSBar;
      NGL_ASSERT(pSB);
      nuiRange& rRange(pSB->GetRange());

      if (Y > mRect.Bottom() - AUTOSCROLL_HEIGHT)
      {
        // Scroll down
        if (rRange.GetValue() + rRange.GetPageSize() < rRange.GetMaximum())
        {
          mScrollUp = false;
          if (!mScrollTimer.IsRunning())
            mScrollTimer.Start(false, false);
          return true;
        }
      }

      if (Y < AUTOSCROLL_HEIGHT)
      {
        // Scroll up
        if (rRange.GetValue() > rRange.GetMinimum())
        {
          mScrollUp = true;
          if (!mScrollTimer.IsRunning())
            mScrollTimer.Start(false, false);
          return true;
        }
      }
    }

    if (mScrollTimer.IsRunning()) // Disable scroll timer if it was still running...
      mScrollTimer.Stop();

    nuiTreeNodePtr pNode = SearchNode(X,Y,cpt);
    if (pNode)
    {
      if (pNode->GetElement()->IsDisabled())
        return true;
      if (!mSelectionTimer.IsRunning())
      {
        if ((!pNode->IsEmpty() && !pNode->IsOpened()) || 
          (mpSelectedNode && ((mpSelectedNode->GetDepth()-1 != pNode->GetDepth()-1) || mpSelectedNode->IsOpened())))
        {  //may be opened, so start the timer
          if (mpSelectedNode && mpSelectedNode != pNode)
          {
            nuiRect WidgetRect = mpSelectedNode->GetElement()->GetRect();
            nuiRect r = WidgetRect;
            nuiMenuRect*  pRect= mRects[mpSelectedNode->GetDepth()-1]; /// get the current menuRect
            nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
            r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

            mpSelectedNode->Select(false);
            Invalidate(); //(r);
            mpSelectedNode->GetElement()->Invalidate();
          }          

          mpNewSelectedNode = pNode;

          nuiRect WidgetRect = mpNewSelectedNode->GetElement()->GetRect();
          nuiRect r = WidgetRect;
          nuiMenuRect*  pRect= mRects[mpNewSelectedNode->GetDepth()-1]; /// get the current menuRect
          nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
          r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
          mpNewSelectedNode->Select(true);
          Invalidate(); //(r);
          mpNewSelectedNode->GetElement()->Invalidate();

          mSelectionTimer.SetPeriod(nglTime(TIMER_PERIOD));
          mSelectionTimer.Start(false);        

        }
        else
        { //empty node, no need to time this
          if (mpSelectedNode != pNode)
          {
            if (mpSelectedNode)
            {
              mpSelectedNode->Select(false);
              nuiRect WidgetRect = mpSelectedNode->GetElement()->GetRect();
              nuiRect r = WidgetRect;
              nuiMenuRect*  pRect= mRects[mpSelectedNode->GetDepth()-1]; /// get the current menuRect
              nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
              r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
              Invalidate(); //(r);

              mpSelectedNode->GetElement()->Invalidate();
            }

            pNode->Select(true);

            nuiRect WidgetRect = pNode->GetElement()->GetRect();
            nuiRect r = WidgetRect;
            nuiMenuRect*  pRect= mRects[pNode->GetDepth()-1]; /// get the current menuRect
            uint32 dbg = pNode->GetDepth()-1;
            nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
            r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

            Invalidate(); //(r);
            pNode->GetElement()->Invalidate();

            mpSelectedNode = pNode;
          }
        }
      }
      else
      { // Timer is active
        if (mpNewSelectedNode && pNode != mpNewSelectedNode)
        {
          nuiRect WidgetRect = mpNewSelectedNode->GetElement()->GetRect();
          nuiRect r = WidgetRect;
          nuiMenuRect*  pRect= mRects[mpNewSelectedNode->GetDepth()-1]; /// get the current menuRect
          nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
          r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

          mpNewSelectedNode->Select(false);
          Invalidate(); //(r);
          mpNewSelectedNode->GetElement()->Invalidate();
        }
        mpNewSelectedNode = pNode;
        nuiRect WidgetRect = mpNewSelectedNode->GetElement()->GetRect();
        nuiRect r = WidgetRect;
        nuiMenuRect*  pRect= mRects[mpNewSelectedNode->GetDepth()-1]; /// get the current menuRect
        nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
        r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());

        mpNewSelectedNode->Select(true);
        Invalidate(); //(r);        
        mpNewSelectedNode->GetElement()->Invalidate();        
      }
      return true;
    }
  }    
  return false;
}

bool nuiPopupMenu::MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mWheelMoved)
  {
    mWheelMoved = false;
    return true;
  }

  if (nglTime() - mStartTime < mDelayTime)
  {
    return true;
  }

  bool IsInside;
  int cpt = SearchRectIndex(X,Y);
  IsInside = cpt >= 0 ? true : false;

  if (!IsInside)
  { // Out of bounds, had to close menu
    mpTree->OpenAllChildren(false);
    mpTree->SelectAllChildren(false);
    mpSelectedNode = NULL;

    FillSelectedNodes();
    if (!MenuDone())
      Trash();
    else
      RemoveMenu();
    return true;
  }
  else 
  {
    nuiTreeNodePtr pNode = SearchNode(X,Y,cpt);
    if (pNode)
    {
      if (!pNode->IsSelectionEnabled() || pNode->GetElement()->IsDisabled())
        return true;
      if (mpSelectedNode)
      {
        mpSelectedNode->Select(false);
      }

      nuiTreeNodePtr pParent = GetParentNode(mpTree, pNode);
      if (pParent)
      {
        pParent->OpenAllChildren(false);
        pParent->SelectAllChildren(false);
      }
      mpSelectedNode = mpNewSelectedNode = pNode;
      pNode->Select(true);

      if (!pNode->IsEmpty())
      {
        pNode->Open(true);
        if (!pNode->GetChildrenCount())
          pNode->Open(false);
        else
        {
          ResetScrolling(pNode->GetDepth());         
          FillSelectedNodes();
        }
      }
      else
      {
        FillSelectedNodes();
        if (mpSelectedNode)
          mpSelectedNode->Activated();
        if (!MenuDone())
          Trash();
        else
          RemoveMenu();
      }
      Invalidate();
      InvalidateLayout();
    }
    return true;
  }
  return false;
}

void nuiPopupMenu::IncrementScrollBar(bool increment)
{  // dont close anything, OnScrollBarChange will be called
  nuiMenuRect* pMenuRect;
  nuiTreeNode* pCurNode;

  pCurNode = (mpNewSelectedNode ? mpNewSelectedNode : mpSelectedNode);
  if (!pCurNode)
  {
    return;
  }
  uint32 depth = pCurNode->GetDepth()-1;
  if (depth < mRects.size() && mRects[depth]->mpSBar->IsVisible())
  {
    pMenuRect = mRects[depth];
    if (increment)
      pMenuRect->mpSBar->GetRange().Increment();
    else
      pMenuRect->mpSBar->GetRange().Decrement();
  }
}

bool nuiPopupMenu::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{    
  mSelectionTimer.Stop();

  if (Button & nglMouseInfo::ButtonWheelUp)
  {
    IncrementScrollBar(false);
    mWheelMoved = true;
    return true;
  }
  else if (Button & nglMouseInfo::ButtonWheelDown)
  {
    IncrementScrollBar(true);
    mWheelMoved = true;
    return true;
  }  

  bool IsInside;
  int cpt = SearchRectIndex(X,Y);
  IsInside = cpt >= 0 ? true : false;
  if (!IsInside)
  { // Out of bounds, had to close menu
    if (mpSelectedNode)
    {
      mpSelectedNode->Select(false);
      mpSelectedNode = NULL;
    }
    if (!MenuDone())
      Trash();
    else
      RemoveMenu();
    return true;
  }
  else 
  { // Search the clicked node
    nuiTreeNodePtr pNode = SearchNode(X,Y,cpt);
    if (pNode)
    {
      if (pNode->GetElement()->IsDisabled())
        return true;

      if (mpSelectedNode && mpSelectedNode != pNode)
        mpSelectedNode->Select(false);

      nuiTreeNodePtr pParent = GetParentNode(mpTree, pNode);
      if (pParent)
      {
        pParent->OpenAllChildren(false);
        pParent->SelectAllChildren(false);
      }
      mpSelectedNode = mpNewSelectedNode = pNode;
      pNode->Select(true);
      if (!pNode->IsEmpty())
      {
        pNode->Open(true);
        if (!pNode->GetChildrenCount())
          pNode->Open(false);
        else
        {
          ResetScrolling(pNode->GetDepth());         
          FillSelectedNodes();
        }
      }
      Invalidate();
      InvalidateLayout();
      return true;
    }
  }
  return true;
}

nuiTreeNode* nuiPopupMenu::GetNextNode(nuiTreeNode* pParent, nuiTreeNode* pNode, int increment)
{
  NGL_ASSERT(pParent);
  uint32 nchilds = pParent->GetChildrenCount();
  for (uint32 i = 0; i < nchilds; i++)
  {
    if (pParent->GetChild(i) == pNode)
    {
      int child = i;
      child += increment;
      if (child < 0)
        child = nchilds - 1;
      else
        child = child % nchilds;

      nuiTreeNode* pTreeNode = dynamic_cast<nuiTreeNode*>(pParent->GetChild(child));
      if (pTreeNode->GetElement()->IsDisabled())
        pTreeNode = GetNextNode(pParent, pTreeNode, increment);
      return pTreeNode;
    }
  }
  return pParent;
}

void nuiPopupMenu::AdjustScrolling(nuiTreeNode* pNode)
{  // range changes, OnScrollBarChange() will be called
  nuiTopLevel* pRoot = GetTopLevel();
  NGL_ASSERT(pRoot);
  nuiRect MainRect = pRoot->GetRect();
  nuiRect NodeRect = pNode->GetElement()->GetRect();
  if (pNode->GetDepth()-1 >= mRects.size())
    return;
  nuiMenuRect* pMenuRect = mRects[pNode->GetDepth()-1];
  nuiRange& rRange = pMenuRect->mpSBar->GetRange();

  if (NodeRect.Bottom() > MainRect.Bottom())
  {
    mScrolledFromKeyboard = true;
    rRange.SetValue(rRange.GetValue() + (NodeRect.Bottom() - MainRect.Bottom()));
  }
  else if (NodeRect.Top() < 0.f)
  {
    mScrolledFromKeyboard = true;
    rRange.SetValue(rRange.GetValue() + NodeRect.Top());
  }
}

nuiLabel* SearchLabel(nuiWidgetPtr pParent);

bool nuiPopupMenu::KeyDown(const nglKeyEvent& rEvent)
{
  mSelectionTimer.Stop();
  nuiTreeNode* pNode = (mpNewSelectedNode ? mpNewSelectedNode : mpSelectedNode);
  if (pNode)
  {
    nuiRect WidgetRect = pNode->GetElement()->GetRect();
    nuiRect r = WidgetRect;
    nuiMenuRect*  pRect= mRects[pNode->GetDepth()-1]; /// get the current menuRect
    nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
    r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
    Invalidate(); //(r);
  }

  if (rEvent.mKey == NK_UP)
  {
    if (mpSelectedNode && !mpSelectedNode->IsOpened())
      pNode = GetNextNode(GetParentNode(mpTree, mpSelectedNode), mpSelectedNode, -1);
    else
    { ///< find the first non disabled element node from end
      nuiTreeNode* pParent = mpSelectedNode ? mpSelectedNode : mpTree;
      uint32 count = pParent->GetChildrenCount();
      for (int i = count - 1; i >= 0; i--)
      {
        pNode = dynamic_cast<nuiTreeNode*>(pParent->GetChild(i));
        if (pNode->GetElement()->IsDisabled())
        {
          pNode=NULL;
          continue;
        }
        else
          break;
      }
      pNode = pNode ? pNode : mpSelectedNode ? mpSelectedNode : NULL;
    }
    if (!pNode)
      return true;

    nuiRect WidgetRect = pNode->GetElement()->GetRect();
    nuiRect r = WidgetRect;
    nuiMenuRect*  pRect= mRects[pNode->GetDepth()-1]; /// get the current menuRect
    nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
    r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
    Invalidate(); //(r);

    if (mpSelectedNode && !mpSelectedNode->IsOpened())
    {
      mpSelectedNode->Select(false);
      nuiRect WidgetRect = mpSelectedNode->GetElement()->GetRect();
      nuiRect r = WidgetRect;
      nuiMenuRect*  pRect= mRects[mpSelectedNode->GetDepth()-1]; /// get the current menuRect
      nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
      r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
      Invalidate(); //(r); 
    }

    mpSelectedNode = pNode;
    pNode->Select(true);

    AdjustScrolling(pNode);
    FillSelectedNodes();
    return true;
  }
  else if (rEvent.mKey == NK_DOWN)
  {
    if (mpSelectedNode && !mpSelectedNode->IsOpened())
      pNode = GetNextNode(GetParentNode(mpTree, mpSelectedNode), mpSelectedNode, 1);
    else
    { ///< find the first non disabled element node from top
      nuiTreeNode* pParent = mpSelectedNode ? mpSelectedNode : mpTree;
      uint32 count = pParent->GetChildrenCount();
      for (uint32 i = 0; i < count; i++)
      {
        pNode = dynamic_cast<nuiTreeNode*>(pParent->GetChild(i));
        if (pNode->GetElement()->IsDisabled())
        {
          pNode=NULL;
          continue;
        }
        else
          break;
      }
      pNode = pNode ? pNode : mpSelectedNode ? mpSelectedNode : NULL;
    }
    if (!pNode)
      return true;

    nuiRect WidgetRect = pNode->GetElement()->GetRect();
    nuiRect r = WidgetRect;
    nuiMenuRect*  pRect= mRects[pNode->GetDepth()-1]; /// get the current menuRect
    nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
    r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
    Invalidate(); //(r);

    if (mpSelectedNode && !mpSelectedNode->IsOpened())
    {
      mpSelectedNode->Select(false);

      nuiRect WidgetRect = mpSelectedNode->GetElement()->GetRect();
      nuiRect r = WidgetRect;
      nuiMenuRect*  pRect= mRects[mpSelectedNode->GetDepth()-1]; /// get the current menuRect
      nuiSize TreeHandleSize = pRect->mHasNonEmpty ? NUI_POPUP_TREE_HANDLE_SIZE : 0.f;
      r.SetSize(WidgetRect.GetWidth()+TreeHandleSize, WidgetRect.GetHeight());
      Invalidate(); //(r);
    }
    mpSelectedNode = pNode;

    pNode->Select(true);
    AdjustScrolling(pNode);
    FillSelectedNodes();
    return true;  
  }
  else if (rEvent.mKey == NK_LEFT)
  {
    if (mpSelectedNode)
    {
      if (mpSelectedNode->IsOpened())
      {
        mpSelectedNode->Open(false);
        FillSelectedNodes();
        InvalidateLayout();
        Invalidate();
        return true;
      }
      else if ((pNode = GetParentNode(mpTree, mpSelectedNode)) != mpTree)
      {
        mpSelectedNode->Select(false);
        mpSelectedNode = pNode;
        mpSelectedNode->Open(false);
        FillSelectedNodes();
        InvalidateLayout();
        Invalidate();
        return true;
      }
    }
    Invalidate();
    InvalidateLayout();
    return false;
  }
  else if (rEvent.mKey == NK_RIGHT)
  {
    if (mpSelectedNode && !mpSelectedNode->IsEmpty())
    {
      mpSelectedNode->Open(true);
      if (!mpSelectedNode->GetChildrenCount())
      {
        mpSelectedNode->Open(false);
        return false;
      }
      ResetScrolling(mpSelectedNode->GetDepth());
      mpSelectedNode = dynamic_cast<nuiTreeNode*>(mpSelectedNode->GetChild(0));
      mpSelectedNode->Select(true);
      Invalidate();
      InvalidateLayout();
      FillSelectedNodes();
      return true;
    }
    Invalidate();
    InvalidateLayout();
    return false;
  }
  else if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
  {
    if (mpSelectedNode && !mpSelectedNode->IsEmpty()) // maybe open
    {
      mpSelectedNode->Open(true);
      if (!mpSelectedNode->GetChildrenCount())
      {
        mpSelectedNode->Open(false);
        return false;
      }
      else
      {
        ResetScrolling(mpSelectedNode->GetDepth());
        mpSelectedNode = dynamic_cast<nuiTreeNode*>(mpSelectedNode->GetChild(0));
        mpSelectedNode->Select(true);
        Invalidate();
        InvalidateLayout();
        FillSelectedNodes();
        return true;
      }
    }
    // selection done
    FillSelectedNodes();
    if (mpSelectedNode)
      mpSelectedNode->Activated();
    if (!MenuDone())
      Trash();
    else
      RemoveMenu();
    return true;
  }
  else if (rEvent.mKey == NK_ESC)
  {
    mpTree->OpenAllChildren(false);
    mpTree->SelectAllChildren(false);
    mpSelectedNode = NULL;

    FillSelectedNodes();
    if (!MenuDone())
      Trash();
    else
      RemoveMenu();
    return true;
  }
  else if (rEvent.mChar)
  {
    int index = -1;
    nuiTreeNodePtr pParent = mpTree;
    if (mpSelectedNode)
    {
      pParent = (nuiTreeNodePtr)mpSelectedNode->GetParent();
      uint count = pParent->GetChildrenCount();
      for (uint i = 0; i < count && index == -1; i++)
      {
        if (mpSelectedNode == pParent->GetChild(i))
        {
          index = i;
        }
      }
    }

    if (pParent)
    {
      index++;

      nuiTreeNodePtr pNode = NULL;
      uint count = pParent->GetChildrenCount();
      NGL_OUT(_T("Start search\n"));
      for (uint i = 0; i < count && !pNode; i++)
      {
        nuiTreeNodePtr pN = (nuiTreeNodePtr)pParent->GetChild((i + index) % count);
        nuiWidget* pItem = pN->GetElement();
        if (pItem->IsEnabled())
        {
          if (mKey.IsEmpty())
          {
            nuiLabel* pLabel = SearchLabel(pItem);
            if (pLabel)
            {
              const nglString& rKey = pLabel->GetText();
              if (toupper(rKey[0]) == rEvent.mChar)
              {
                pNode = pN;
              }
            }
          }
          else
          {
            const nglString& rKey = pN->GetProperty(mKey);
            if (toupper(rKey[0]) == rEvent.mChar)
            {
              pNode = pN;
            }
          }
        }
      }

      if (pNode)
      {
        if (mpSelectedNode)
        {
          mpSelectedNode->Select(false);
          if (mpSelectedNode->IsOpened())
            mpSelectedNode->Open(false);
        }
        mpSelectedNode = pNode;

        pNode->Select(true);
        AdjustScrolling(pNode);
        FillSelectedNodes();
      }
    }
    return true;
  }
  return false;
}

int nuiPopupMenu::SearchRectIndex(nuiSize X, nuiSize Y)
{
  int cpt = mRects.size() - 1;
  for (std::vector<nuiMenuRect*>::reverse_iterator rit = mRects.rbegin(); rit != mRects.rend(); rit++)
  {
    if ((*rit)->mRect.IsInside(X,Y))
      return cpt;
    cpt--;
  }
  return -1;
}

nuiTreeNodePtr nuiPopupMenu::SearchNode(nuiSize X, nuiSize Y, int cpt)
{
  NGL_ASSERT(mRects[cpt]);
  nuiTreeNode* pChildNode;

  uint32 count = mRects[cpt]->mpFromNode->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    pChildNode = dynamic_cast<nuiTreeNode*>((mRects[cpt]->mpFromNode)->GetChild(i));
    if ((pChildNode->GetElement()->GetRect()).IsInside(X,Y))
      return pChildNode;
  }
  return NULL;
}

nuiTreeNodePtr nuiPopupMenu::GetTree()
{
  return mpTree;
}

const nuiTreeNode* nuiPopupMenu::GetSelectedNode() const
{
  return mpSelectedNode;
}

void nuiPopupMenu::SetTrashRemoval(bool trash)
{
  mTrashRemoval = trash;
}

void nuiPopupMenu::RemoveMenu()
{
  if (mTrashRemoval)
  {
    Trash();
  }
}

bool nuiPopupMenu::IsParentActive() const
{
  if (mpParent)
    return mpParent->IsParentActive();
  else
    return false;
}


int nuiPopupMenu::GetIdInTree(nuiTreeNode* pNode)
{
  if (pNode == mpTree)
  {
    return 0;
  }
  nuiTreeNode* pParent = mRects[pNode->GetDepth()-1]->mpFromNode;

  if (!pParent)
  {
    return -1;
  }
  uint32 count = pParent->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNodePtr pChild = dynamic_cast<nuiTreeNode*>(pParent->GetChild(i));
    if (pChild == pNode)
      return (int)i;
  }

  return -1;
}
std::list<int> nuiPopupMenu::GetTreeIdsCourse()
{
  std::list<int> IdList;
  int id;

  std::list<nuiTreeNode*> NodesCourse = GetTreeNodesCourse();
  for (std::list<nuiTreeNode*>::iterator n = NodesCourse.begin(); n != NodesCourse.end(); n++)
  {
    id = GetIdInTree(*n);
    IdList.push_back(id);
  }
  return IdList;
}

std::list<nuiTreeNode*> nuiPopupMenu::GetTreeNodesCourse()
{
  std::list<nuiTreeNode*> RetList;
  for (std::list<nuiTreeNode*>::iterator i = mSelectedNodes.begin(); i != mSelectedNodes.end() && *i != mpTree; i++)
    RetList.push_front(*i);
  return RetList; 
}

void nuiPopupMenu::ShowFirstNode(bool show)
{
  mShowFirstNode = show;
  Invalidate();
  InvalidateLayout();
}


void nuiPopupMenu::SetKey(const nglString& rKey)
{
  mKey = rKey;
}

const nglString& nuiPopupMenu::GetKey() const
{
  return mKey;
}
