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
  CheckValid();
  return nuiWidget::Serialize(pParentNode, Recursive);
}

void nuiContainer::SerializeChildren(nuiXMLNode* pParentNode, bool Recursive) const
{
  CheckValid();
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
  CheckValid();
  //NGL_OUT(_T("Deleting nuiContainer '%ls' (class='%ls')\n"), GetObjectName().GetChars(), GetObjectClass().GetChars());
}

// We need to do something special about SetObjectXXX in order to avoid pure virtual method called from the constructor.
bool nuiContainer::SetObjectClass(const nglString& rName)
{
  return nuiObject::SetObjectClass(rName);
}

// We need to do something special about SetObjectXXX in order to avoid pure virtual method called from the constructor.
void nuiContainer::SetObjectName(const nglString& rName)
{
  nuiObject::SetObjectName(rName);
}


bool nuiContainer::Trash()
{
  return nuiWidget::Trash();
}

void nuiContainer::CallOnTrash()
{
  CheckValid();
  ChildrenCallOnTrash();
  
  OnTrash();
}


void nuiContainer::ChildrenCallOnTrash()
{
  CheckValid();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      pItem->CallOnTrash();
  }
  delete pIt;  
}

nuiContainerPtr nuiContainer::GetRoot() const
{
  CheckValid();
  void* Self = (void*)this;
  if (mpParent)
    return mpParent->GetRoot();
  else
    return (nuiContainerPtr)Self;
}

nuiWidgetPtr nuiContainer::GetChild(int index)
{
  CheckValid();
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
  CheckValid();
  X -= mRect.mLeft;
  Y -= mRect.mTop;

  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideFromSelf(X,Y))
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

void nuiContainer::GetChildren(nuiSize X, nuiSize Y, nuiWidgetList& rChildren, bool DeepSearch)
{
  CheckValid();
  X -= mRect.mLeft;
  Y -= mRect.mTop;
  
  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideFromParent(X,Y))
    {
      if (DeepSearch)
      {
        nuiContainerPtr pContainer = dynamic_cast<nuiContainerPtr>(pItem);
        if (pContainer)
          pContainer->GetChildren(X, Y, rChildren, DeepSearch);
      }
      rChildren.push_back(pItem);
    }
  }
  delete pIt;
}



nuiWidgetPtr nuiContainer::GetChildIf(nuiSize X, nuiSize Y, TestWidgetFunctor* pFunctor)
{
  CheckValid();
  X -= mRect.mLeft;
  Y -= mRect.mTop;

  IteratorPtr pIt;
  for (pIt = GetLastChild(); pIt && pIt->IsValid(); GetPreviousChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem && pItem->IsInsideFromParent(X,Y))
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
  CheckValid();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetObjectName() == rName)
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
  name.Tokenize(tokens, _T('/'));

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
  CheckValid();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->GetObjectName() == rName)
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
  CheckValid();
  nuiWidget::CallConnectTopLevel(pTopLevel);
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    pIt->GetWidget()->CallConnectTopLevel(pTopLevel);
  }
  delete pIt;
}

void nuiContainer::CallDisconnectTopLevel(nuiTopLevel* pTopLevel)
{
  CheckValid();
  nuiWidget::CallDisconnectTopLevel(pTopLevel);
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    pIt->GetWidget()->CallDisconnectTopLevel(pTopLevel);
  }
  delete pIt;
}

void nuiContainer::InvalidateChildren(bool Recurse)
{
  CheckValid();
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
  CheckValid();
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
  CheckValid();
  return DrawChildren(pContext);
}

bool nuiContainer::DrawChildren(nuiDrawContext* pContext)
{
  CheckValid();
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
  CheckValid();
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

  if (IsDrawingInCache(true))
  {
    nuiMetaPainter* pMetaPainter = dynamic_cast<nuiMetaPainter*>(pPainter);
    if (pMetaPainter)
      pMetaPainter->DrawChild(pContext, pChild);
  }

  pContext->PopMatrix();
}

////// Private event management:
bool nuiContainer::DispatchMouseClick(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  nglMouseInfo info(rInfo);
  GlobalToLocal(info.X, info.Y);

  // Get a chance to preempt the mouse event before the children get it:
  if (PreMouseClicked(info))
    return true;
  
  if (IsInsideFromRoot(rInfo.X, rInfo.Y) || hasgrab)
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
            if (pItem->DispatchMouseClick(rInfo))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    nglMouseInfo info(rInfo);
    GlobalToLocal(info.X, info.Y);
    if (PreClicked(info))
      return true;
    bool ret = MouseClicked(info);
    ret |= Clicked(info);
    return ret;
  }
  return false;
}

bool nuiContainer::DispatchMouseUnclick(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return false;

  bool hasgrab = HasGrab();
  if (IsDisabled() && !hasgrab)
    return false;

  nglMouseInfo info(rInfo);
  GlobalToLocal(info.X, info.Y);
  // Get a chance to preempt the mouse event before the children get it:
  if (PreMouseUnclicked(info))
    return true;
  
  if (IsInsideFromRoot(rInfo.X, rInfo.Y) || hasgrab)
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
            if ((pItem)->DispatchMouseUnclick(rInfo))
            {
              delete pIt;
              return true;
            }
          }
        }
      }
      delete pIt;
    }

    bool res = PreUnclicked(info);
    if (!res)
    {
      res = MouseUnclicked(info);
      res |= Unclicked(info);
    }

    return res;
  }
  return false;
}

nuiWidgetPtr nuiContainer::DispatchMouseMove(const nglMouseInfo& rInfo)
{
  CheckValid();
  if (!mMouseEventEnabled || mTrashed)
    return false;

  nuiWidgetPtr pHandled=NULL;
  bool inside=false,res=false;
  bool hasgrab = HasGrab();

  if (IsDisabled() && !hasgrab)
    return NULL;

  nglMouseInfo info(rInfo);
  GlobalToLocal(info.X, info.Y);

  // Get a chance to preempt the mouse event before the children get it:
  if (PreMouseMoved(info))
    return this;
  
  if (IsInsideFromRoot(rInfo.X, rInfo.Y) || hasgrab)
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
            pHandled = pItem->DispatchMouseMove(rInfo);
        if (pHandled)
        {
          // stop as soon as someone caught the event
          delete pIt;
          return pHandled;
        }
      }
      delete pIt;
    }

    if (PreMouseMoved(info))
      return this;
    res = MouseMoved(info);
    res |= MovedMouse(info);
  }
  else
  {
    if (GetHover())
    {
      if (PreMouseMoved(info))
        return this;
      res = MouseMoved(info);
      res |= MovedMouse(info);
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
  CheckValid();
  nuiWidget::SetAlpha(Alpha);
  SilentInvalidateChildren(true);
  DebugRefreshInfo();
}

void nuiContainer::SetEnabled(bool set)
{
  CheckValid();
  nuiWidget::SetEnabled(set);
  SilentInvalidateChildren(true);
  Invalidate();
  DebugRefreshInfo();
}

void nuiContainer::SetSelected(bool set)
{
  CheckValid();
  nuiWidget::SetSelected(set);
  SilentInvalidateChildren(true);
  Invalidate();
  DebugRefreshInfo();
}

void nuiContainer::SetVisible(bool Visible)
{
  CheckValid();
  nuiWidget::SetVisible(Visible);
  if (mVisible)
    BroadcastVisible();
}

void nuiContainer::BroadcastVisible()
{
  CheckValid();
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
  CheckValid();
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
  CheckValid();
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

void nuiContainer::InternalSetLayout(const nuiRect& rect, bool PositionChanged, bool SizeChanged)
{
  CheckValid();
  if (mNeedSelfLayout || SizeChanged)
  {
    SetRect(rect);
    Invalidate();
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
  CheckValid();
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
  CheckValid();
  SetHotRect(rChildHotRect);
}

void nuiContainer::InternalResetCSSPass()
{
  CheckValid();
  nuiWidget::InternalResetCSSPass();
  
  IteratorPtr pIt = GetFirstChild();
  for (; pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->ResetCSSPass();
  }
  delete pIt;
}

nuiContainer::IteratorPtr nuiContainer::GetChildIterator(nuiWidgetPtr pChild)
{
  CheckValid();
  IteratorPtr pIt = GetFirstChild();
  while (pIt->IsValid() && pIt->GetWidget() != pChild)
    GetNextChild(pIt);
  return pIt;
}

nuiContainer::ConstIteratorPtr nuiContainer::GetChildIterator(nuiWidgetPtr pChild) const
{
  CheckValid();
  ConstIteratorPtr pIt = GetFirstChild();
  while (pIt->IsValid() && pIt->GetWidget() != pChild)
    GetNextChild(pIt);
  return pIt;
}

nuiWidgetPtr nuiContainer::GetNextFocussableChild(nuiWidgetPtr pChild) const
{
  CheckValid();
  ConstIteratorPtr pIt = pChild ? GetChildIterator(pChild) : GetFirstChild();
  if (!pIt->IsValid())
    return NULL;
  
  if (pChild)
    GetNextChild(pIt);
  
  while (pIt->IsValid() && !pIt->GetWidget()->GetWantKeyboardFocus() && pIt->GetWidget())
    GetNextChild(pIt);
  
  if (pIt->IsValid())
  {
    nuiWidgetPtr pW = pIt->GetWidget();
    delete pIt;
    return pW;
  }
  
  delete pIt;
  return NULL;
}

nuiWidgetPtr nuiContainer::GetPreviousFocussableChild(nuiWidgetPtr pChild) const
{
  CheckValid();
  ConstIteratorPtr pIt = pChild ? GetChildIterator(pChild) : GetLastChild();
  if (!pIt->IsValid())
    return NULL;
  
  if (pChild)
    GetPreviousChild(pIt);
  
  while (pIt->IsValid() && !pIt->GetWidget()->GetWantKeyboardFocus() && pIt->GetWidget())
    GetPreviousChild(pIt);
  
  if (pIt->IsValid())
  {
    nuiWidgetPtr pW = pIt->GetWidget();
    delete pIt;
    return pW;
  }
  
  delete pIt;
  return NULL;
}


nuiWidgetPtr nuiContainer::GetNextSibling(nuiWidgetPtr pChild) const
{
  CheckValid();
  ConstIteratorPtr pIt = pChild ? GetChildIterator(pChild) : GetFirstChild();
  if (!pIt->IsValid())
    return NULL;
  
  if (pChild)
    GetNextChild(pIt);
  
  nuiWidgetPtr pW = NULL;
  if (pIt->IsValid())
    pW = pIt->GetWidget();
  
  delete pIt;
  return pW;
}

nuiWidgetPtr nuiContainer::GetPreviousSibling(nuiWidgetPtr pChild) const
{
  CheckValid();
  ConstIteratorPtr pIt = pChild ? GetChildIterator(pChild) : GetFirstChild();
  if (!pIt->IsValid())
    return NULL;
  
  if (pChild)
    GetPreviousChild(pIt);
  
  nuiWidgetPtr pW = NULL;
  if (pIt->IsValid())
    pW = pIt->GetWidget();
  
  delete pIt;
  return pW;
}

void nuiContainer::SetChildrenLayoutAnimationDuration(float duration)
{
  CheckValid();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetLayoutAnimationDuration(duration);
  }
  delete pIt;
}

void nuiContainer::SetChildrenLayoutAnimationEasing(const nuiEasingMethod& rMethod)
{
  CheckValid();
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    pItem->SetLayoutAnimationEasing(rMethod);
  }
  delete pIt;
}

bool nuiContainer::PreMouseClicked(const nglMouseInfo& rInfo)
{
  CheckValid();
  return false;
}

bool nuiContainer::PreMouseUnclicked(const nglMouseInfo& rInfo)
{
  CheckValid();
  return false;
}

bool nuiContainer::PreMouseMoved(const nglMouseInfo& rInfo)
{
  CheckValid();
  return false;
}

void nuiContainer::GetHoverList(nuiSize X, nuiSize Y, std::set<nuiWidget*>& rHoverSet, std::list<nuiWidget*>& rHoverList) const
{
  CheckValid();
  nuiContainer::ConstIteratorPtr pIt = NULL;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem->IsInsideFromRoot(X, Y))
    {
      rHoverList.push_back(pItem);
      rHoverSet.insert(pItem);
      nuiContainer* pChild = dynamic_cast<nuiContainer*>(pItem);
      if (pChild)
        pChild->GetHoverList(X, Y, rHoverSet, rHoverList);
    }
  }
  delete pIt;
}

