/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiDrawContext.h"
#include "nuiComboBox.h"

#define DEFAULT_HANDLE_WIDTH    8.f
#define DEFAULT_HANDLE_HEIGHT   5.f
#define DEFAULT_HANDLE_OFFSET  5.f

nuiComboBox::nuiComboBox()
: nuiSimpleContainer(), 
  mpChoicesTree(NULL), 
  mOwnTree(false), 
  mpSelected(NULL), 
  mpOldSelected(NULL), 
  mHandleWidth(DEFAULT_HANDLE_WIDTH), 
  mHandleHeight(DEFAULT_HANDLE_HEIGHT),
  mHandleOffset(DEFAULT_HANDLE_OFFSET),
  mComboBoxEvents(this)
{
  SetObjectClass(_T("nuiComboBox"));
  mBoxDisplayed = true;
  mHasCustomMenuTextFg = false;
}


nuiComboBox::nuiComboBox(nuiTreeNode* pChoicesTree, bool ownTree)
: nuiSimpleContainer(), 
  mpChoicesTree(pChoicesTree), 
  mOwnTree(ownTree), 
  mpSelected(NULL), 
  mpOldSelected(NULL), 
  mHandleWidth(DEFAULT_HANDLE_WIDTH), 
  mHandleHeight(DEFAULT_HANDLE_HEIGHT),
  mHandleOffset(DEFAULT_HANDLE_OFFSET),
  mComboBoxEvents(this)
{
  SetObjectClass(_T("nuiComboBox"));

  mBoxDisplayed = true;
  mHasCustomMenuTextFg = false;
  
  ReparentTree(mpChoicesTree);
}

bool nuiComboBox::Load(nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  delete mpChoicesTree;
  mpChoicesTree = NULL; 
  mOwnTree = false;
  mpSelected = NULL;
  mpOldSelected = NULL; 
  mHandleWidth = DEFAULT_HANDLE_WIDTH; 
  mHandleHeight = DEFAULT_HANDLE_HEIGHT;
  mHandleOffset = DEFAULT_HANDLE_OFFSET;

  mComboBoxEvents.SetTarget(this);
  mBoxDisplayed = true;
  mHasCustomMenuTextFg = false;

  return true;
}

nuiComboBox::~nuiComboBox()
{
  UnparentTree(mpChoicesTree);

  if (mOwnTree)
    delete mpChoicesTree;
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

nuiRect nuiComboBox::CalcIdealSize()
{
  float w=0.f, h=0.f;

  if (mpChoicesTree)
  {
    CalcIdealTreeSize(mpChoicesTree, w, h);
    
  }
  nuiRect rect(0.f,0.f, w + mHandleWidth+mHandleOffset, MAX(h,mHandleHeight));
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

void nuiComboBox::SetMenuTextColor(const nuiColor &NormalTextColor, const nuiColor &SelectedTextColor, const nuiColor &DisabledTextColor)
{
  mMenuText[0] = NormalTextColor;
  mMenuText[1] = SelectedTextColor;
  mMenuText[2] = DisabledTextColor;
  mHasCustomMenuTextFg = true;
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
  if (mBoxDisplayed)
  {
    pContext->EnableBlending(true);
    pContext->SetBlendFunc(nuiBlendTransp);
    
    const nuiRect& rect(GetRect().Size());
    
    nuiShape shape;
    shape.AddRoundRect(rect, MIN(rect.GetWidth(), rect.GetHeight())/2.f);

    pContext->SetFillColor(GetColor(eComboBoxBg));
    nuiColor black(0.f, 0.f, 0.f, GetAlpha());
    pContext->SetStrokeColor(GetColor(eComboBoxLining));
    pContext->DrawShape(&shape, eStrokeAndFillShape);

    
    nuiRect r(rect.Right() - (mHandleWidth+mHandleOffset), (nuiSize)ToNearest(rect.GetHeight()/2.f - mHandleHeight/2.f), mHandleWidth, mHandleHeight);
    
    pContext->EnableAntialiasing(true);

    nuiShape arrow;
    arrow.LineTo(nuiPoint((nuiSize)ToBelow(r.Left()+r.GetWidth()/2.f) , r.Bottom()));
    arrow.LineTo(nuiPoint(r.Left(), r.Top()));
    arrow.LineTo(nuiPoint(r.Right(), r.Top()));
    arrow.CloseContour();

    pContext->SetFillColor(GetColor(eSelectionMarkee));
    pContext->SetStrokeColor(GetColor(eSelectionMarkee));
    pContext->DrawShape(&arrow, eStrokeAndFillShape);
    
    nuiReflection ref(0.4f, 0.5f);
    ref.Draw(pContext, rect, &shape);
  }

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
      nuiWidget::SetSelected(true);
      
      mpOldSelected = mpSelected;
      mpSelected = NULL;
      nuiRect rect(0.f,0.f,0.f,0.f);
      PrepareMenuTree(mpChoicesTree);
      nuiPopupMenu* pMenu = new nuiPopupMenu(this, mpChoicesTree, nuiRect(0.f,0.f,GetRect().GetWidth() - (mHandleWidth + mHandleOffset),0.f), false);  
      
      nuiColor col;
      std::map<nuiWidgetElement, nuiColor>::iterator it = mWidgetElementColors.begin();
      std::map<nuiWidgetElement, nuiColor>::iterator end = mWidgetElementColors.end();
      while (it != end)
      {
        pMenu->SetColor(it->first, it->second);
        ++it;
      }
      if (mHasCustomMenuTextFg)
      {
        pMenu->SetColor(eNormalTextFg, mMenuText[0]);
        pMenu->SetColor(eSelectedTextFg, mMenuText[1]);
        pMenu->SetColor(eDisabledTextFg, mMenuText[2]);
      }
      
      pMenu->ShowFirstNode(false);
      mComboBoxEvents.Connect(pMenu->MenuDone, &nuiComboBox::OnSelect, pMenu);
      return true;
    }
  }

  return false;
}

bool nuiComboBox::OnSelect(const nuiEvent& rEvent)
{
  nuiWidget::SetSelected(false);

  nuiPopupMenu* pMenu = (nuiPopupMenu*)rEvent.mpUser;
  nuiTreeNode* pNode = pMenu->GetSelectedNode();

  SetSelected(pNode?pNode:mpOldSelected);
  mpOldSelected = NULL;
  if (pNode)
  {
    ValueChanged();
  }
  
  pMenu->SetTrashRemoval(true);
  InvalidateLayout();
      
  return true;
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
  mpSelected = pSelection;
  ReparentTree(mpChoicesTree);

  SelectionChanged();
  InvalidateLayout();
}

void nuiComboBox::SetHandleSize(nuiSize width, nuiSize height)
{ 
  mHandleWidth = width;
  mHandleHeight = height;
  InvalidateLayout();
}

void nuiComboBox::SetHandleOffset(nuiSize offset)
{ 
  mHandleOffset = offset;
  InvalidateLayout();
}

void nuiComboBox::GetHandleSize(nuiSize& rWidth, nuiSize& rHeight)
{
  rWidth = mHandleWidth;
  rHeight = mHandleHeight;
}

nuiSize nuiComboBox::GetHandleWidth() const
{
  return mHandleWidth;
}

nuiSize nuiComboBox::GetHandleHeight() const
{
  return mHandleHeight;
}

nuiSize nuiComboBox::GetHandleOffset() const
{
  return mHandleOffset;
}

bool nuiComboBox::IsBoxDisplayed() const
{
  return mBoxDisplayed;
}

void nuiComboBox::SetBoxDisplayed(bool set)
{
  mBoxDisplayed = set;
}

void nuiComboBox::UnparentTree(nuiTreeNode* pTree)
{
  NGL_ASSERT(pTree);
  pTree->Select(false);
  pTree->Open(false);
  nuiWidgetPtr pWidget = pTree->GetElement();
  NGL_ASSERT(pWidget);
  if (pWidget->GetParent() == this)
    DelChild(pWidget, false);
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
  
  if (mpSelected)
  {
    nuiWidgetPtr pWidget = mpSelected->GetElement();
    NGL_ASSERT(pWidget);
    DelChild(pWidget, false);
  }
  
  if (mOwnTree)
    delete mpChoicesTree;

  mpSelected = NULL;
  mpOldSelected = NULL;
  
  mpChoicesTree = pTree;

  ReparentTree(mpChoicesTree);
  InvalidateLayout();
}

