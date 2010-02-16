/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSimpleContainer.h"

nuiSimpleContainer::nuiSimpleContainer()
: nuiContainer()
{
  SetObjectClass(_T("nuiSimpleContainer"));
}

bool nuiSimpleContainer::Load(const nuiXMLNode* pNode)
{
	bool res=true;
	res &= LoadAttributes(pNode);
	res &= LoadChildren(pNode);
  return res;
}


bool nuiSimpleContainer::LoadChildren(const nuiXMLNode* pNode)
{
	bool res=true;
  uint i, count = pNode->GetChildrenCount();
  for (i = 0; i < count; i++)
  {
    nuiXMLNode* pChild = pNode->GetChild(i);
    nuiWidget* pWidget = nuiBuilder::Get().CreateWidget(pChild->GetName());
    if (!pWidget)
    {
      // If the child is not a creatable element then may be a text property of the object.
      nuiXMLNode* pText = pChild->GetChild(nglString("##text"));
      if (pText)
        SetProperty(pChild->GetName(),pText->GetValue());
    }
    else
    {
      pWidget->Load(pChild);
      AddChild(pWidget);
    }
  }
  
  return res;
}


bool nuiSimpleContainer::LoadAttributes(const nuiXMLNode* pNode)
{
  return nuiContainer::Load(pNode);
}


nuiSimpleContainer::~nuiSimpleContainer()
{
  // Delete all children:
  IteratorPtr pIt;
  for (pIt = GetFirstChild(); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      delete pItem;
  }
  delete pIt;
}

// We need to by pass nuiContainer as is has a special implementation of SetObjectXXX in order to prevent calling pure virtual methods
bool nuiSimpleContainer::SetObjectClass(const nglString& rName)
{
  return nuiWidget::SetObjectClass(rName);
}

// We need to by pass nuiContainer as is has a special implementation of SetObjectXXX in order to prevent calling pure virtual methods
void nuiSimpleContainer::SetObjectName(const nglString& rName)
{
  nuiWidget::SetObjectName(rName);
}


bool nuiSimpleContainer::AddChild(nuiWidgetPtr pChild)
{
  if (GetDebug())
  {
    NGL_OUT(_T("[%ls] Add Child 0x%x <--- 0x%x\n"), GetObjectClass().GetChars(), this, pChild);
  }
  nuiContainer* pParent = pChild->GetParent();
  NGL_ASSERT(pParent != this);

  uint32 capacity = mpChildren.capacity();
  uint32 size = mpChildren.size();
  if (size == capacity)
  {
    if (size < 128)
    {
      mpChildren.reserve(size * 2);
    }
    else
    {
      mpChildren.reserve(size + 128);
    }
  }
  
  mpChildren.push_back(pChild);
  if (pParent)
    pParent->DelChild(pChild, false); // Remove from previous parent...
  
  pChild->SetParent(this);
  ChildAdded(this, pChild);
  Invalidate();
  InvalidateLayout();
  
  DebugRefreshInfo();
  return true;
}

bool nuiSimpleContainer::DelChild(nuiWidgetPtr pChild,bool Delete)
{
  NGL_ASSERT(pChild->GetParent() == this)

  if (GetDebug())
  {
    NGL_OUT(_T("[%ls] Del Child 0x%x <--- 0x%x (%s)\n"), GetObjectClass().GetChars(), this, pChild, Delete?"delete":"no delete");
  }
  
  nuiWidgetList::iterator it  = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  for ( ; it != end; ++it)
  {
    if (*it == pChild)
    {
      mpChildren.erase(it);
      if (!pChild->IsTrashed())
      {
        nuiTopLevel* pRoot = GetTopLevel();
        Trashed();
        Invalidate();
        
        if (pRoot)
          pRoot->AdviseObjectDeath(pChild);
        pChild->SetParent(NULL);
      }
      ChildDeleted(this, pChild);
      if (Delete && pChild)
        delete pChild;
      Invalidate();
      InvalidateLayout();
      DebugRefreshInfo();
      return true;
    }
  }
  DebugRefreshInfo();
  return false;
}

uint nuiSimpleContainer::GetChildrenCount() const
{
  return mpChildren.size();
}


bool nuiSimpleContainer::Clear(bool Delete)
{
  // start by trashing everybody
  nuiContainer::ChildrenCallOnTrash();

  // then, clear the container
  int childCount = GetChildrenCount();
  for (childCount--; childCount >= 0; childCount--)
  {
    nuiWidget* pWidget = GetChild(childCount);
    if (pWidget)
    {
      DelChild(pWidget, Delete);
    }
  }
  mpChildren.clear();
  InvalidateLayout();
  DebugRefreshInfo();
  return true;
}


nuiContainer::Iterator* nuiSimpleContainer::GetFirstChild()
{
  IteratorPtr pIt;
  pIt = new Iterator();
  bool valid = !mpChildren.empty();
  pIt->SetValid(valid);
  if (valid)
    pIt->SetElement(mpChildren.begin());
  return pIt;
}

nuiContainer::ConstIterator* nuiSimpleContainer::GetFirstChild() const
{
  ConstIteratorPtr pIt;
  pIt = new ConstIterator();
  bool valid = !mpChildren.empty();
  pIt->SetValid(valid);
  if (valid)
    pIt->SetElement(mpChildren.begin());
  return pIt;
}

nuiContainer::Iterator* nuiSimpleContainer::GetLastChild()
{
  nuiWidgetList::iterator it = mpChildren.end();
  IteratorPtr pIt;
  pIt = new Iterator();
  if (!mpChildren.empty())
  {
    it--;
    pIt->SetElement(it);
    pIt->SetValid(true);
  }
  else
  {
    pIt->SetValid(false);
  }
  return pIt;
}

nuiContainer::ConstIterator* nuiSimpleContainer::GetLastChild() const
{
  nuiWidgetList::const_iterator it = mpChildren.end();
  ConstIteratorPtr pIt;
  pIt = new ConstIterator();
  if (!mpChildren.empty())
  {
    it--;
    pIt->SetElement(it);
    pIt->SetValid(true);
  }
  else
  {
    pIt->SetValid(false);
  }
  return pIt;
}

bool nuiSimpleContainer::GetNextChild(nuiContainer::IteratorPtr pIterator)
{
  if (!pIterator)
    return false;
  if (!pIterator->IsValid())
    return false;
  pIterator->GetElement()++;
  if (pIterator->GetElement() == mpChildren.end())
  {
    pIterator->SetValid(false);
    return false;
  }
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetNextChild(nuiContainer::ConstIteratorPtr pIterator) const
{
  if (!pIterator)
    return false;
  if (!pIterator->IsValid())
    return false;
  pIterator->GetElement()++;
  if (pIterator->GetElement() == mpChildren.end())
  {
    pIterator->SetValid(false);
    return false;
  }
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetPreviousChild(nuiContainer::IteratorPtr pIterator)
{
  if (!pIterator)
    return false;  
  if (!pIterator->IsValid())
    return false;
  if (pIterator->GetElement() == mpChildren.begin())
  {
    pIterator->SetValid(false);
    return false;
  }
  
  pIterator->GetElement()--;
  
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetPreviousChild(nuiContainer::ConstIteratorPtr pIterator) const
{
  if (!pIterator)
    return false;  
  if (!pIterator->IsValid())
    return false;
  if (pIterator->GetElement() == mpChildren.begin())
  {
    pIterator->SetValid(false);
    return false;
  }
  
  pIterator->GetElement()--;
  
  pIterator->SetValid(true);
  return true;
}

void nuiSimpleContainer::RaiseChild(nuiWidgetPtr pChild)
{
  
  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  for ( ; it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    if (pChild == pItem)
    {
      nuiWidgetList::iterator next = it;
      ++next;
      mpChildren.erase(it);
      mpChildren.insert(next, pItem);
      Invalidate();
      DebugRefreshInfo();
      return;
    }
  }
  DebugRefreshInfo();
}

void nuiSimpleContainer::LowerChild(nuiWidgetPtr pChild)
{
  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  nuiWidgetList::iterator previous = it;
  for ( ; it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    if (pChild == pItem)
    {
      if (previous != mpChildren.begin())
      {
        nuiWidgetPtr pPrevious = *previous;
        mpChildren.erase(previous);
        mpChildren.insert(it, pPrevious);
        Invalidate();
      }
      DebugRefreshInfo();
      return;
    }
    previous = it;
  }
  DebugRefreshInfo();
}

void nuiSimpleContainer::RaiseChildToFront(nuiWidgetPtr pChild)
{
  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  for ( ; it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    if (pChild == pItem)
    {
      mpChildren.erase(it);
      mpChildren.push_back(pItem);
      Invalidate();
      DebugRefreshInfo();
      return;
    }
  }
  DebugRefreshInfo();
}

void nuiSimpleContainer::LowerChildToBack(nuiWidgetPtr pChild)
{
  nuiWidgetList::iterator it = mpChildren.begin();
  nuiWidgetList::iterator end = mpChildren.end();
  for ( ; it != end; ++it)
  {
    nuiWidgetPtr pItem = *it;
    if (pChild == pItem)
    {
      mpChildren.erase(it);
      mpChildren.insert(mpChildren.begin(), pItem);
      Invalidate();
      DebugRefreshInfo();
      return;
    }
  }
  DebugRefreshInfo();
}

