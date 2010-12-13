/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiSimpleContainer.h"

class nuiSimpleContainerIterator : public nuiContainer::Iterator
{
public:
  nuiSimpleContainerIterator(nuiSimpleContainer* pSource, bool DoRefCounting);
  nuiSimpleContainerIterator(const nuiSimpleContainerIterator& rIterator);
  virtual ~nuiSimpleContainerIterator();
  
  virtual nuiWidgetPtr GetWidget() const;

  nuiSimpleContainerIterator& operator = (const nuiSimpleContainerIterator& rIterator);
protected:
  nuiWidgetList::iterator mIterator;
private:
  friend class nuiSimpleContainer;
  bool SetElement(const nuiWidgetList::iterator& rIterator);
  nuiWidgetList::iterator& GetElement();
};

class nuiSimpleContainerConstIterator : public nuiContainer::ConstIterator
{
public:
  nuiSimpleContainerConstIterator(const nuiSimpleContainer* pSource, bool DoRefCounting);
  nuiSimpleContainerConstIterator(const nuiSimpleContainerConstIterator& rIterator);
  virtual ~nuiSimpleContainerConstIterator();
  
  virtual nuiWidgetPtr GetWidget() const;

  nuiSimpleContainerConstIterator& operator = (const nuiSimpleContainerConstIterator& rIterator);
protected:
  nuiWidgetList::const_iterator mIterator;
private:
  friend class nuiSimpleContainer;
  bool SetElement(const nuiWidgetList::const_iterator& rIterator);
  nuiWidgetList::const_iterator& GetElement();
};

typedef nuiSimpleContainerIterator* nuiSimpleContainerIteratorPtr;
typedef nuiSimpleContainerConstIterator* nuiSimpleContainerConstIteratorPtr;

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
  CheckValid();
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
  CheckValid();
  return nuiContainer::Load(pNode);
}


nuiSimpleContainer::~nuiSimpleContainer()
{
  CheckValid();
  // Delete all children:
  IteratorPtr pIt;
  for (pIt = GetFirstChild(false); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
      pItem->Release();
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
  CheckValid();
  if (GetDebug())
  {
    NGL_OUT(_T("[%ls] Add Child 0x%x <--- 0x%x\n"), GetObjectClass().GetChars(), this, pChild);
  }
  pChild->Acquire();
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
    pParent->DelChild(pChild); // Remove from previous parent...
  
  pChild->SetParent(this);
  ChildAdded(this, pChild);
  Invalidate();
  InvalidateLayout();
  
  DebugRefreshInfo();
  return true;
}

bool nuiSimpleContainer::DelChild(nuiWidgetPtr pChild)
{
  CheckValid();
  NGL_ASSERT(pChild->GetParent() == this)

  if (GetDebug())
  {
    NGL_OUT(_T("[%ls] Del Child 0x%x <--- 0x%x (%s)\n"), GetObjectClass().GetChars(), this, pChild);
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
      Invalidate();
      InvalidateLayout();
      DebugRefreshInfo();
      pChild->Release();
      return true;
    }
  }
  DebugRefreshInfo();
  return false;
}

uint nuiSimpleContainer::GetChildrenCount() const
{
  CheckValid();
  return mpChildren.size();
}


bool nuiSimpleContainer::Clear()
{
  CheckValid();
  // start by trashing everybody
  nuiContainer::ChildrenCallOnTrash();

  // then, clear the container
  int childCount = GetChildrenCount();
  for (childCount--; childCount >= 0; childCount--)
  {
    nuiWidget* pWidget = GetChild(childCount);
    if (pWidget)
    {
      DelChild(pWidget);
    }
  }
  mpChildren.clear();
  InvalidateLayout();
  DebugRefreshInfo();
  return true;
}


nuiContainer::Iterator* nuiSimpleContainer::GetFirstChild(bool DoRefCounting)
{
  CheckValid();
  IteratorPtr pIt;
  pIt = new nuiSimpleContainerIterator(this, DoRefCounting);
  bool valid = !mpChildren.empty();
  pIt->SetValid(valid);
  if (valid)
    ((nuiSimpleContainerIterator*)pIt)->SetElement(mpChildren.begin());
  return pIt;
}

nuiContainer::ConstIterator* nuiSimpleContainer::GetFirstChild(bool DoRefCounting) const
{
  CheckValid();
  nuiSimpleContainerConstIteratorPtr pIt;
  pIt = new nuiSimpleContainerConstIterator(this, DoRefCounting);
  bool valid = !mpChildren.empty();
  pIt->SetValid(valid);
  if (valid)
    pIt->SetElement(mpChildren.begin());
  return pIt;
}

nuiContainer::Iterator* nuiSimpleContainer::GetLastChild(bool DoRefCounting)
{
  CheckValid();
  nuiWidgetList::iterator it = mpChildren.end();
  IteratorPtr pIt;
  pIt = new nuiSimpleContainerIterator(this, DoRefCounting);
  if (!mpChildren.empty())
  {
    it--;
    ((nuiSimpleContainerIterator*)pIt)->SetElement(it);
    pIt->SetValid(true);
  }
  else
  {
    pIt->SetValid(false);
  }
  return pIt;
}

nuiContainer::ConstIterator* nuiSimpleContainer::GetLastChild(bool DoRefCounting) const
{
  CheckValid();
  nuiWidgetList::const_iterator it = mpChildren.end();
  nuiSimpleContainerConstIteratorPtr pIt;
  pIt = new nuiSimpleContainerConstIterator(this, DoRefCounting);
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
  CheckValid();
  if (!pIterator)
    return false;
  if (!pIterator->IsValid())
    return false;
  ((nuiSimpleContainerIterator*)pIterator)->GetElement()++;
  if (((nuiSimpleContainerIterator*)pIterator)->GetElement() == mpChildren.end())
  {
    pIterator->SetValid(false);
    return false;
  }
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetNextChild(nuiContainer::ConstIteratorPtr pIterator) const
{
  CheckValid();
  if (!pIterator)
    return false;
  if (!pIterator->IsValid())
    return false;
  ((nuiSimpleContainerConstIterator*)pIterator)->GetElement()++;
  if (((nuiSimpleContainerConstIterator*)pIterator)->GetElement() == mpChildren.end())
  {
    pIterator->SetValid(false);
    return false;
  }
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetPreviousChild(nuiContainer::IteratorPtr pIterator)
{
  CheckValid();
  if (!pIterator)
    return false;  
  if (!pIterator->IsValid())
    return false;
  if (((nuiSimpleContainerIterator*)pIterator)->GetElement() == mpChildren.begin())
  {
    pIterator->SetValid(false);
    return false;
  }
  
  ((nuiSimpleContainerIterator*)pIterator)->GetElement()--;
  
  pIterator->SetValid(true);
  return true;
}

bool nuiSimpleContainer::GetPreviousChild(nuiContainer::ConstIteratorPtr pIterator) const
{
  CheckValid();
  if (!pIterator)
    return false;  
  if (!pIterator->IsValid())
    return false;
  if (((nuiSimpleContainerConstIterator*)pIterator)->GetElement() == mpChildren.begin())
  {
    pIterator->SetValid(false);
    return false;
  }
  
  ((nuiSimpleContainerConstIterator*)pIterator)->GetElement()--;
  
  pIterator->SetValid(true);
  return true;
}

void nuiSimpleContainer::RaiseChild(nuiWidgetPtr pChild)
{
  CheckValid();
  
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
  CheckValid();
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
  CheckValid();
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
  CheckValid();
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

///////////////////////
////// nuiContainer::Iterator

nuiSimpleContainerIterator::nuiSimpleContainerIterator(nuiSimpleContainer* pSource, bool DoRefCounting)
: nuiContainer::Iterator(pSource, DoRefCounting)
{
  mValid = false;
}

nuiSimpleContainerConstIterator::nuiSimpleContainerConstIterator(const nuiSimpleContainer* pSource, bool DoRefCounting)
: nuiContainer::ConstIterator(pSource, DoRefCounting)
{
  mValid = false;
}

nuiSimpleContainerIterator::nuiSimpleContainerIterator(const nuiSimpleContainerIterator& rIterator)
: nuiContainer::Iterator(rIterator)
{
  mIterator = rIterator.mIterator;
}

nuiSimpleContainerConstIterator::nuiSimpleContainerConstIterator(const nuiSimpleContainerConstIterator& rIterator)
: nuiContainer::ConstIterator(rIterator)
{
  mIterator = rIterator.mIterator;
}

nuiSimpleContainerIterator& nuiSimpleContainerIterator::operator = (const nuiSimpleContainerIterator& rIterator)
{
  *((nuiContainer::Iterator*)this) = rIterator;
  mIterator = rIterator.mIterator;
  return *this;
}

nuiSimpleContainerConstIterator& nuiSimpleContainerConstIterator::operator = (const nuiSimpleContainerConstIterator& rIterator)
{
  *((nuiContainer::ConstIterator*)this) = rIterator;
  mIterator = rIterator.mIterator;
  return *this;
}

bool nuiSimpleContainerIterator::SetElement(const nuiWidgetList::iterator& rIterator)
{
  mIterator = rIterator;
  return true;
}

bool nuiSimpleContainerConstIterator::SetElement(const nuiWidgetList::const_iterator& rIterator)
{
  mIterator = rIterator;
  return true;
}

nuiWidgetList::iterator& nuiSimpleContainerIterator::GetElement()
{
  return mIterator; 
}

nuiWidgetList::const_iterator& nuiSimpleContainerConstIterator::GetElement()
{
  return mIterator; 
}

nuiSimpleContainerIterator::~nuiSimpleContainerIterator()
{
}

nuiSimpleContainerConstIterator::~nuiSimpleContainerConstIterator()
{
}

nuiWidgetPtr nuiSimpleContainerIterator::GetWidget() const
{
  return mValid?*mIterator:NULL;
}

nuiWidgetPtr nuiSimpleContainerConstIterator::GetWidget() const
{
  return mValid?*mIterator:NULL;
}

