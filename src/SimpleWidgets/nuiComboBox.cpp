/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#define DEFAULT_HANDLE_WIDTH    10.f
#define DEFAULT_HANDLE_HEIGHT   10.f
#define DEFAULT_HANDLE_OFFSET  12.f

nuiComboBox::nuiComboBox()
: nuiSimpleContainer(), 
  mpChoicesTree(NULL), 
  mpSelected(NULL), 
  mpOldSelected(NULL), 
  mHandleWidth(DEFAULT_HANDLE_WIDTH), 
  mHandleHeight(DEFAULT_HANDLE_HEIGHT),
  mHandleOffset(DEFAULT_HANDLE_OFFSET),
  mComboBoxEvents(this)
{
  SetObjectClass(_T("nuiComboBox"));
  SetWantKeyboardFocus(true);
}


nuiComboBox::nuiComboBox(nuiTreeNode* pChoicesTree, bool ownTree)
: nuiSimpleContainer(), 
  mpChoicesTree(pChoicesTree), 
  mpSelected(NULL), 
  mpOldSelected(NULL), 
  mHandleWidth(DEFAULT_HANDLE_WIDTH), 
  mHandleHeight(DEFAULT_HANDLE_HEIGHT),
  mHandleOffset(DEFAULT_HANDLE_OFFSET),
  mComboBoxEvents(this)
{
  SetObjectClass(_T("nuiComboBox"));

  if (mpChoicesTree)
    mpChoicesTree->Acquire();
  ReparentTree(mpChoicesTree);
  SetWantKeyboardFocus(true);
}

nuiComboBox::~nuiComboBox()
{
  UnparentTree(mpChoicesTree);

  if (mpChoicesTree)
    mpChoicesTree->Release();
}

void CalcIdealTreeSize(nuiTreeNode* pTree, float& w, float& h)
{
  if (pTree)
  {
    uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pChildNode = (nuiTreeNode*)pTree->GetChild(i);
      CalcIdealTreeSize(pChildNode, w, h);
      nuiWidget* pWidget = pChildNode->GetElement();
      NGL_ASSERT(pWidget);
      nuiRect IdealRect( pWidget->GetIdealRect() );
      w = MAX(w, IdealRect.GetWidth());
      h = MAX(h, IdealRect.GetHeight());
    }
  }
}

#define VERTICAL_MARGIN 4
#define HORIZONTAL_MARGIN 4

nuiRect nuiComboBox::CalcIdealSize()
{
  float w=0.f, h=0.f;

  if (mpChoicesTree)
  {
    CalcIdealTreeSize(mpChoicesTree, w, h);
    
  }
  nuiRect rect(0.f,0.f, w + mHandleWidth + mHandleOffset + HORIZONTAL_MARGIN, MAX(h,mHandleHeight) + VERTICAL_MARGIN);
  return rect;  
}

void SetTreeRect(nuiTreeNode* pTree, const nuiRect& rRect)
{
  if (pTree)
  {
     uint32 count = pTree->GetChildrenCount();
    for (uint32 i = 0; i < count; i++)
    {
      nuiTreeNode* pChildNode = (nuiTreeNode*)pTree->GetChild(i);
      SetTreeRect(pChildNode, rRect);
      nuiWidget* pWidget;
      pWidget = pChildNode->GetElement();
      if (pWidget)
        pWidget->SetLayout(rRect);
    }
  }
}


bool nuiComboBox::SetRect(const nuiRect& rRect)
{
  nuiRect rect(rRect);

  nuiWidget::SetRect(rect);
  SetTreeRect(mpChoicesTree, nuiRect(0.f, 0.f, rect.GetWidth() - (mHandleWidth + mHandleOffset), rect.GetHeight()));
  return true;
}

bool nuiComboBox::Draw(nuiDrawContext* pContext)
{
  nuiWidget* pWidget = GetSelectedWidget();
  if (pWidget)
  {
    NGL_ASSERT(pWidget->GetParent() == this);
    DrawChild(pContext, pWidget);
  }
  
  return true;
}

void PrepareMenuTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  nuiWidgetPtr pWidget = pTree->GetElement();
  NGL_ASSERT(pWidget);
  pWidget->SetVisible(true);

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    PrepareMenuTree(pNode);
  }
}


bool nuiComboBox::MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (mpChoicesTree)
  {
    if (Button & nglMouseInfo::ButtonLeft)
    {
      mpOldSelected = mpSelected;
      mpSelected = NULL;
      nuiRect rect(0.f,0.f,0.f,0.f);
      PrepareMenuTree(mpChoicesTree);
      nuiPopupMenu* pMenu = new nuiPopupMenu(this, mpChoicesTree, nuiRect(0.f,0.f,GetRect().GetWidth() - (mHandleWidth + mHandleOffset),0.f), false);  
      
      pMenu->ShowFirstNode(false);
      mComboBoxEvents.Connect(pMenu->MenuDone, &nuiComboBox::OnSelect, pMenu);
      return true;
    }
  }

  return false;
}

bool nuiComboBox::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_SPACE || rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
  {
    mpOldSelected = mpSelected;
    mpSelected = NULL;
    nuiRect rect(0.f,0.f,0.f,0.f);
    PrepareMenuTree(mpChoicesTree);
    nuiPopupMenu* pMenu = new nuiPopupMenu(this, mpChoicesTree, nuiRect(0.f,0.f,GetRect().GetWidth() - (mHandleWidth + mHandleOffset),0.f), false);  
    
    pMenu->ShowFirstNode(false);
    mComboBoxEvents.Connect(pMenu->MenuDone, &nuiComboBox::OnSelect, pMenu);
    return true;
  }
  
  return false;
}

bool nuiComboBox::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_SPACE || rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER)
    return true;
  return false;
}

void nuiComboBox::OnSelect(const nuiEvent& rEvent)
{
  nuiWidget::SetSelected(false);

  nuiPopupMenu* pMenu = (nuiPopupMenu*)rEvent.mpUser;
  nuiTreeNode* pNode = pMenu->GetSelectedNode();

  SetSelected(pNode ? pNode : mpOldSelected);
//  if (mpOldSelected)
//    mpOldSelected->Release();
  mpOldSelected = NULL;
  if (pNode)
  {
    ValueChanged();
  }
  
  pMenu->SetTrashRemoval(true);
  InvalidateLayout();
      
  rEvent.Cancel();
  
  Focus();
}

void nuiComboBox::SetSelected(uint32 childIndex)
{
  NGL_ASSERT(mpChoicesTree);
  NGL_ASSERT(mpChoicesTree->GetChildrenCount() > childIndex);

  SetSelected((nuiTreeNode*)(mpChoicesTree->GetChild(childIndex)));
  InvalidateLayout();
}

void nuiComboBox::SetValue(uint32 childIndex)
{
  NGL_ASSERT(mpChoicesTree);
  NGL_ASSERT(mpChoicesTree->GetChildrenCount() > childIndex);

  SetSelected((nuiTreeNode*)(mpChoicesTree->GetChild(childIndex)));
  InvalidateLayout();
}

uint32 nuiComboBox::GetValue()
{
  uint32 res = 0;
  NGL_ASSERT(mpChoicesTree);
  if (mpSelected)
  {
    for (uint32 count = mpChoicesTree->GetChildrenCount(); res < count; res++)
    {
      if (mpSelected == ((nuiTreeNode*)(mpChoicesTree->GetChild(res))))
        return res;
    }
  }
  return res;
}

nuiWidget* nuiComboBox::GetSelectedWidget() 
{ 
  return mpSelected ? mpSelected->GetElement() : NULL; 
}

nuiTreeNode* nuiComboBox::GetSelected() 
{ 
  return mpSelected; 
}

const nuiTreeNode* nuiComboBox::GetChoices() const
{
  return mpChoicesTree;
}

void nuiComboBox::SetSelected(nuiTreeNode* pSelection) 
{
  const nuiTreeNode* pCTNode = pSelection;
  SetSelected(pCTNode);
}

void nuiComboBox::SetSelected(const nuiTreeNode* pSelection) 
{ 
//  if (pSelection)
//    pSelection->Acquire();
//  if (mpSelected)
//    mpSelected->Release();
  mpSelected = const_cast<nuiTreeNode*>(pSelection);
  ReparentTree(mpChoicesTree);
  
  SelectionChanged();
  InvalidateLayout();
}

void nuiComboBox::UnparentTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  pTree->Select(false);
  pTree->Open(false);
  nuiWidgetPtr pWidget = pTree->GetElement();
  NGL_ASSERT(pWidget);
  if (pWidget->GetParent() == this)
    DelChild(pWidget);
  
  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    UnparentTree(pNode);
  }
}

void nuiComboBox::ReparentTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  nuiWidgetPtr pWidget = pTree->GetElement();
  NGL_ASSERT(pWidget);

  pWidget->SetLayout(nuiRect(0.f,0.f, GetRect().GetWidth()-mHandleWidth, GetRect().GetHeight()));

  if (pWidget->GetParent() != this)
  {
    AddChild(pWidget);
    pWidget->InvalidateLayout();
  }

  if (GetSelectedWidget() == pWidget)
  {
    if (!pWidget->IsVisible())
      pWidget->SetVisible(true);
  }
  else 
  {
    if (pWidget->IsVisible())
      pWidget->SetVisible(false);
  }

  uint32 count = pTree->GetChildrenCount();
  for (uint32 i = 0; i < count; i++)
  {
    nuiTreeNode* pNode = dynamic_cast<nuiTreeNode*>(pTree->GetChild(i));
    NGL_ASSERT(pNode);
    ReparentTree(pNode);
  }
}

void nuiComboBox::SetTree(nuiTreeNode* pTree)
{
  if (mpChoicesTree == pTree)
    return;
  
//  if (mpSelected)
//  {
//    mpSelected->Release();
//    nuiWidgetPtr pWidget = mpSelected->GetElement();
//    NGL_ASSERT(pWidget);
//    DelChild(pWidget);
//  }

//  if (mpOldSelected)
//    mpOldSelected->Release();

  if (mpChoicesTree)
    mpChoicesTree->Release();

  mpSelected = NULL;
  mpOldSelected = NULL;
  
  mpChoicesTree = pTree;
  if (mpChoicesTree)
    mpChoicesTree->Acquire();

  ReparentTree(mpChoicesTree);
  InvalidateLayout();
}

