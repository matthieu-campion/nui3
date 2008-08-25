/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/




#include "nui.h"
#include "nuiContainer.h"
#include "nuiDrawContext.h"
#include "nuiMetaPainter.h"
#include "nuiXML.h"
#include "nuiBuilder.h"
#include "nuiTopLevel.h"

//#define NUI_CHECK_LAYOUTS

nuiContainer::nuiContainer()
: nuiWidget()
{
  SetObjectClass(_T("nuiContainer"));
  NUI_ADD_EVENT(ChildAdded);
  NUI_ADD_EVENT(ChildDeleted);
}

bool nuiContainer::Load(const nuiXMLNode* pNode)
{
  nuiWidget::Load(pNode);
  return true;
}

nuiXMLNode* nuiContainer::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{   
  nuiXMLNode* pNode = NULL;

  if (mSerializeMode == eDontSaveNode)
    return NULL;

  pNode = SerializeAttributes(pParentNode, Recursive);

  if (Recursive && pNode)
  {
    SerializeChildren(pNode);
  }

  return pNode;
}

nuiXMLNode* nuiContainer::SerializeAttributes(nuiXMLNode* pParentNode, bool Recursive) const
{
  return nuiWidget::Serialize(pParentNode, Recursive);
}

void nuiContainer::SerializeChildren(nuiXMLNode* pParentNode, bool Recursive) const
{
  ConstIteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      pItem->Serialize(pParentNode,true);
  }
  delete pIt;
}





nuiContainer::~nuiContainer()
{
  //NGL_OUT(_T("Deleting nuiContainer '%ls' (class='%ls')\n"), GetProperty(_T("Name")).GetChars(), GetProperty(_T("Class")).GetChars());
}

bool nuiContainer::Trash()
{
  return nuiWidget::Trash();
}

void nuiContainer::CallOnTrash()
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      pItem->CallOnTrash();
  }
  delete pIt;
  
  OnTrash();
}

nuiContainerPtr nuiContainer::GetRoot() const
{
  void* Self = (void*)this;
  if (mpParent)
    return mpParent->GetRoot();
  else
    return (nuiContainerPtr)Self;
}

nuiWidgetPtr nuiContainer::GetChild(int index)
{
  int i=0;

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    if (i == index)
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      delete pIt;
      return pItem;
    }
    i++;
  }
  delete pIt;

  return NULL;
}

nuiWidgetPtr nuiContainer::GetChild(nuiSize X, nuiSize Y)
{
  X -= mRect.mLeft;
  Y -= mRect.mTop;

  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideLocal(X,Y))
    {
      delete pIt;
      nuiContainerPtr pContainer = dynamic_cast<nuiContainerPtr>(pItem);
      if (pContainer)
        return pContainer->GetChild(X,Y);
      else 
        return pItem;
    }
  }
  delete pIt;

  return this;
}

nuiWidgetPtr nuiContainer::GetChildIf(nuiSize X, nuiSize Y, TestWidgetFunctor* pFunctor)
{
  X -= mRect.mLeft;
  Y -= mRect.mTop;

  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideLocal(X,Y))
    {
      nuiContainerPtr pContainer = dynamic_cast<nuiContainerPtr>(pItem);
      if (pContainer)
      {
        nuiWidget* pWidget = pContainer->GetChildIf(X,Y, pFunctor);
        if (pWidget)
        {
          delete pIt;
          return pWidget;
        }
      }
      else 
      {
        if ((*pFunctor)(pItem))
        {
          delete pIt;
          return pItem;
        }
      }
    }
  }
  delete pIt;

  if ((*pFunctor)(this))
    return this;

  return NULL;
}


nuiWidgetPtr nuiContainer::GetChild(const nglString& rName, bool recurse )
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetProperty(nglString(_T("Name"))) == rName)
    {
      delete pIt;
      return pItem;
    }
  }
  delete pIt;

  if (!recurse) // Are we allowed to search the complete tree?
    return NULL;

  nuiWidgetPtr pNode = this;
  nglString name = rName;

  if (name[0] == '/')
  {
    // Get the root of the tree:
    pNode = GetRoot();

    name.DeleteLeft(1); // Remove the '/'
  }

  // Get all the nodes and remove the slashes:
  std::vector<nglString> tokens;
  name.Tokenize(tokens, '/');

  size_t i;
  size_t count = tokens.size();
  for (i = 0; i < count; i++)
  {
    nglString& rTok = tokens[i];
    //Node* pOld = pNode;
    if (rTok == _T(".."))
      pNode = pNode->GetParent();
    else
      pNode = pNode->GetChild(rTok, false);

    if (!pNode)
    {
      //NUI_OUT("Tried to find %ls on %ls", rTok.GetChars(), pOld->GetParamCString(ParamIds::Name));
      return NULL;
    }
  }

  return pNode;
}

nuiWidgetPtr nuiContainer::SearchForChild(const nglString& rName, bool recurse )
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetProperty(nglString(_T("Name"))) == rName)
    {
      delete pIt;
      return pItem;
    }
  }
  delete pIt;

  if (!recurse) // Are we allowed to search the complete tree?
    return NULL;

  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    nuiContainerPtr pContainer = NULL;
    pContainer = dynamic_cast<nuiContainerPtr>(pItem);
    if (pContainer)
    {
      nuiWidgetPtr pWidget = pContainer->SearchForChild(rName,recurse);
      if (pWidget)
      {
        delete pIt;       
        return pWidget;
      }
    }
  }
  delete pIt;

  return NULL;
}

void nuiContainer::CallConnectTopLevel(nuiTopLevel* pTopLevel)
{
  nuiWidget::CallConnectTopLevel(pTopLevel);
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    pIt->GetWidget()->CallConnectTopLevel(pTopLevel);
  }
  delete pIt;
}


void nuiContainer::InvalidateChildren(bool Recurse)
{
  IteratorPtr pIt;
  if (Recurse)
  {
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiContainerPtr pCont = dynamic_cast<nuiContainerPtr>(pItem);
      if (pCont)
        pCont->InvalidateChildren(Recurse);
      pItem->Invalidate();
    }
  }
  else
  {
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      pItem->Invalidate();
    }
  }
  delete pIt;
}

void nuiContainer::SilentInvalidateChildren(bool Recurse)
{
  IteratorPtr pIt;
  if (Recurse)
  {
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      nuiContainerPtr pCont = dynamic_cast<nuiContainerPtr>(pItem);
      if (pCont)
        pCont->SilentInvalidateChildren(Recurse);
      pItem->SilentInvalidate();
    }
  }
  else
  {
    for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
    {
      nuiWidgetPtr pItem = pIt->GetWidget();
      pItem->SilentInvalidate();
    }
  }
  delete pIt;
}

bool nuiContainer::Draw(nuiDrawContext* pContext)
{
  return DrawChildren(pContext);
}

bool nuiContainer::DrawChildren(nuiDrawContext* pContext)
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      DrawChild(pContext, pItem);
  }
  delete pIt;
  return true;
}

void nuiContainer::DrawChild(nuiDrawContext* pContext, nuiWidget* pChild)
{  
  float x,y;
  pContext->PushMatrix();

  x = (float)pChild->GetRect().mLeft;
  y = (float)pChild->GetRect().mTop;
  pContext->Translate( x, y );

  nuiPainter* pPainter = pContext->GetPainter();
  if (mpSavedPainter)
    pContext->SetPainter(mpSavedPainter);

  pChild->DrawWidget(pContext);

  if (mpSavedPainter)
    pContext->SetPainter(pPainter);

  nuiMetaPainter* pMetaPainter = dynamic_cast<nuiMetaPainter*>(pPainter);
  if (pMetaPainter)
    pMetaPainter->DrawChild(pChild);

  pContext->PopMatrix();
}

////// Private event management:
bool nuiContainer::DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
    if (!hasgrab)
    {
      IteratorPtr pIt;
      for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
        {
          if (IsEnabled() && !HasGrab())
          {
            if (pItem->DispatchMouseClick(X,Y, Button))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    GlobalToLocal(X,Y);
    if (PreClicked(X,Y, Button))
      return true;
    bool ret = MouseClicked(X,Y,Button);
    ret |= Clicked(X,Y,Button);
    return ret;
  }
  return false;
}

bool nuiContainer::DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
    if (!hasgrab)
    {
      IteratorPtr pIt;
      for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
        {
          if (IsEnabled())
          {
            if ((pItem)->DispatchMouseUnclick(X,Y, Button))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    GlobalToLocal(X,Y);
    if (PreUnclicked(X,Y, Button))
      return true;
    bool ret = MouseUnclicked(X,Y,Button);
    ret |= Unclicked(X,Y,Button);
    return ret;
  }
  return false;
}

nuiWidgetPtr nuiContainer::DispatchMouseMove(nuiSize X, nuiSize Y)
{
  if (!mMouseEventEnabled || mTrashed)
    return false;

  nuiWidgetPtr pHandled=NULL;
  bool inside=false,res=false;
  bool hasgrab = HasGrab();

  if (IsDisabled() && !hasgrab)
    return false;

  if (IsInside(X,Y) || hasgrab)
  {
    inside = true;

    // If the object has the grab we should not try to notify its children of mouse events!
    if (!hasgrab) 
    {
      IteratorPtr pIt;
      for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
          if (pItem->IsVisible())
            pHandled = pItem->DispatchMouseMove(X,Y);
        if (pHandled)
        {
          // stop as soon as someone caught the event
          delete pIt;
          return pHandled;
        }
      }
      delete pIt;
    }

    GlobalToLocal(X,Y);
    if (PreMouseMoved(X,Y))
      return this;
    res = MouseMoved(X,Y);
    res |= MovedMouse(X,Y);
  }
  else
  {
    if (GetHover())
    {
      GlobalToLocal(X,Y);
      if (PreMouseMoved(X,Y))
        return this;
      res = MouseMoved(X,Y);
      res |= MovedMouse(X,Y);
    }
  }

  if (!pHandled && res && inside)
  {
    nuiTopLevelPtr pRoot = GetTopLevel();
    if (pRoot)
      return this;
  }

  if (pHandled)
    return pHandled;

  return (res && inside) ? this : NULL;
}

void nuiContainer::SetAlpha(float Alpha)
{
  nuiWidget::SetAlpha(Alpha);
  SilentInvalidateChildren(true);
  DebugRefreshInfo();
}

void nuiContainer::SetEnabled(bool set)
{
  nuiWidget::SetEnabled(set);
  SilentInvalidateChildren(true);
  Invalidate();
  DebugRefreshInfo();
}

void nuiContainer::SetSelected(bool set)
{
  nuiWidget::SetSelected(set);
  SilentInvalidateChildren(true);
  Invalidate();
  DebugRefreshInfo();
}

void nuiContainer::SetVisible(bool Visible)
{
  nuiWidget::SetVisible(Visible);
  if (mVisible)
    BroadcastVisible();
}

void nuiContainer::BroadcastVisible()
{
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetNeedSelfRedraw())
    {
      pItem->InvalidateRect(pItem->GetRect().Size());
    }
    nuiContainer* pContainer = dynamic_cast<nuiContainer*>(pItem);
    if (pContainer)
    {
      pContainer->BroadcastVisible();
    }
  }
  delete pIt;
}

nuiRect nuiContainer::CalcIdealSize()
{
  nuiRect temp;

  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (mCanRespectConstraint)
      pItem->SetLayoutConstraint(mConstraint);
    nuiRect r(pItem->GetIdealRect()); // Dummy call. Only the side effect is important: the object recalculates its layout.
    temp.Union(temp, r.Size()); 
  }
  delete pIt;

  DebugRefreshInfo();
  return temp.Size();
}

bool nuiContainer::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);

  nuiRect rect(rRect.Size());
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (mCanRespectConstraint)
      pItem->SetLayoutConstraint(mConstraint);
    pItem->GetIdealRect();
    pItem->SetLayout(rect);
  }
  delete pIt;

  DebugRefreshInfo();
  return true;
}

bool nuiContainer::SetLayout(const nuiRect& rRect)
{
  bool res = false;
  nuiRect rect(GetIdealRect().Size());
  
  if (mPosition != nuiFill)
  {
    rect.SetPosition(mPosition, rRect);
  }
  else
  {
    if (mFillRule == nuiFill)
    {
      rect.SetPosition(mPosition, rRect);
    }
    else if (mFillRule == nuiTile)
    {
      rect = rRect;
    }
    else
    {
      nuiRect r = rRect;
      float ratio,rratio,rw,rh;
      ratio  = (float)r.GetWidth()    / (float)r.GetHeight();
      rratio = (float)rect.GetWidth() / (float)rect.GetHeight();
      
      if (ratio < rratio) 
      {
        rw = (float)r.GetWidth();
        rh = rw / rratio;
      }
      
      else 
      {
        rh = (float)r.GetHeight();
        rw = rratio * rh;
      }
      
      rect = nuiRect(0.0f, 0.0f, rw, rh);
      rect.SetPosition(mFillRule, rRect);
    }
  }
  
  rect.Bottom() -= mBorderBottom;
  rect.Top() += mBorderTop;
  rect.Left() += mBorderLeft;
  rect.Right() -= mBorderRight;

  rect.RoundToNearest();

  bool SizeChanged = !rect.Size().IsEqual(mRect.Size());
  mNeedSelfLayout = mNeedSelfLayout || mClippingOptims || SizeChanged;

  if (mNeedSelfLayout)
  {
    res = SetRect(rect);
  }
  else
  {
    // Is this case the widget have just been moved inside its parent. No need to re layout it, only change the rect...
    mRect = rect;

    if (mNeedLayout)
    {
      // The children need to be re layed out (at least one of them!).
      nuiContainer::IteratorPtr pIt = GetFirstChild();
      do
      {
        nuiWidgetPtr pItem = pIt->GetWidget();
        if (pItem)
        {
          // The rect of each child doesn't change BUT we still ask for its ideal rect.
          nuiRect rect(pItem->GetBorderedRect());
          nuiRect ideal(pItem->GetIdealRect());

          if (pItem->HasUserPos()) 	 
          { 	 
            rect = ideal; 	 
          } 	 
          else if (pItem->HasUserSize())
          {
            rect.SetSize(ideal.GetWidth(), ideal.GetHeight());
          }
          else
          {
            // Set the widget to the size of the parent
          }

          pItem->SetLayout(rect);
        }
      } while (GetNextChild(pIt));
      delete pIt;

    }
  }

  //#TEST:
#ifdef NUI_CHECK_LAYOUTS
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsVisible())
    {
      NGL_ASSERT(!pItem->GetNeedLayout());
    }
  }
  delete pIt;
  //#TEST end
#endif

  mNeedSelfLayout = false;
  mNeedLayout = false;
  DebugRefreshInfo();
  return res;
}


////// nuiContainer::Iterator

nuiContainer::Iterator::Iterator()
{
  mValid = false;
}

nuiContainer::ConstIterator::ConstIterator()
{
  mValid = false;
}

nuiContainer::Iterator::Iterator(const nuiContainer::Iterator& rIterator)
{
  mIterator = rIterator.mIterator;
  mValid = rIterator.mValid;
}

nuiContainer::ConstIterator::ConstIterator(const nuiContainer::ConstIterator& rIterator)
{
  mIterator = rIterator.mIterator;
  mValid = rIterator.mValid;
}

nuiContainer::Iterator& nuiContainer::Iterator::operator = (const nuiContainer::Iterator& rIterator)
{
  mIterator = rIterator.mIterator;
  mValid = rIterator.mValid;
  return *this;
}

nuiContainer::ConstIterator& nuiContainer::ConstIterator::operator = (const nuiContainer::ConstIterator& rIterator)
{
  mIterator = rIterator.mIterator;
  mValid = rIterator.mValid;
  return *this;
}

bool nuiContainer::Iterator::SetElement(const nuiWidgetList::iterator& rIterator)
{
  mIterator = rIterator;
  return true;
}

bool nuiContainer::ConstIterator::SetElement(const nuiWidgetList::const_iterator& rIterator)
{
  mIterator = rIterator;
  return true;
}

nuiWidgetList::iterator& nuiContainer::Iterator::GetElement()
{
  return mIterator; 
}

nuiWidgetList::const_iterator& nuiContainer::ConstIterator::GetElement()
{
  return mIterator; 
}

nuiContainer::Iterator::~Iterator()
{
}

nuiContainer::ConstIterator::~ConstIterator()
{
}

nuiWidgetPtr nuiContainer::Iterator::GetWidget() const
{
  return mValid?*mIterator:NULL;
}

nuiWidgetPtr nuiContainer::ConstIterator::GetWidget() const
{
  return mValid?*mIterator:NULL;
}

bool nuiContainer::Iterator::IsValid() const
{
  return mValid;
}

bool nuiContainer::ConstIterator::IsValid() const
{
  return mValid;
}

void nuiContainer::Iterator::SetValid(bool Valid)
{
  mValid = Valid;
}

void nuiContainer::ConstIterator::SetValid(bool Valid)
{
  mValid = Valid;
}

nuiWidgetPtr nuiContainer::Find(const nglString& rName)
{ 
  int slash = rName.Find('/'); 

  if (slash >= 0) 
  { 
    nglString path = rName.GetLeft(slash); 
    nglString rest = rName.Extract(slash + 1); 
    nuiWidgetPtr node = SearchForChild(path, false); 

    return node ? node->Find(rest) : NULL; 
  } 
  else 
    return SearchForChild(rName,false); 
} 

void nuiContainer::OnChildHotRectChanged(nuiWidget* pChild, const nuiRect& rChildHotRect)
{
  SetHotRect(rChildHotRect);
}

void nuiContainer::InternalResetCSSPass()
{
  nuiWidget::InternalResetCSSPass();
  
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->ResetCSSPass();
  }
  delete pIt;
}


