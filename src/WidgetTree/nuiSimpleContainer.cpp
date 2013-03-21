/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

class nuiSimpleContainerIterator : public nuiContainer::Iterator
{
public:
  nuiSimpleContainerIterator(nuiSimpleContainer* pSource, bool DoRefCounting);
  nuiSimpleContainerIterator(const nuiSimpleContainerIterator& rIterator);
  virtual ~nuiSimpleContainerIterator();
  
  virtual nuiWidgetPtr GetWidget() const;

  nuiSimpleContainerIterator& operator = (const nuiSimpleContainerIterator& rIterator);
  
  void Increment()
  {
    mIndex++;
  }

  void Decrement()
  {
    mIndex--;
  }
protected:
  int32 mIndex;
private:
  friend class nuiSimpleContainer;
  bool SetIndex(int32 index);
  int32 GetIndex() const;
};

class nuiSimpleContainerConstIterator : public nuiContainer::ConstIterator
{
public:
  nuiSimpleContainerConstIterator(const nuiSimpleContainer* pSource, bool DoRefCounting);
  nuiSimpleContainerConstIterator(const nuiSimpleContainerConstIterator& rIterator);
  virtual ~nuiSimpleContainerConstIterator();
  
  virtual nuiWidgetPtr GetWidget() const;

  nuiSimpleContainerConstIterator& operator = (const nuiSimpleContainerConstIterator& rIterator);
  
  void Increment()
  {
    mIndex++;
  }
  
  void Decrement()
  {
    mIndex--;
  }
protected:
  int32 mIndex;
private:
  friend class nuiSimpleContainer;
  bool SetIndex(int32 index);
  int32 GetIndex() const;
};

typedef nuiSimpleContainerIterator* nuiSimpleContainerIteratorPtr;
typedef nuiSimpleContainerConstIterator* nuiSimpleContainerConstIteratorPtr;

nuiSimpleContainer::nuiSimpleContainer()
: nuiContainer()
{
  SetObjectClass(_T("nuiSimpleContainer"));
}

#if 0
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
#endif

nuiSimpleContainer::~nuiSimpleContainer()
{
  CheckValid();
  // Delete all children:
  IteratorPtr pIt;
  for (pIt = GetFirstChild(false); pIt && pIt->IsValid(); GetNextChild(pIt))
  {
    nuiWidgetPtr pItem = pIt->GetWidget();
    if (pItem)
    {
      if (!pItem->IsTrashed(false))
        pItem->SetParent(NULL);
      if (pItem->Release())
        pItem->SetParent(NULL);
    }
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
    NGL_OUT(_T("[%s] Add Child 0x%x <--- 0x%x\n"), GetObjectClass().GetChars(), this, pChild);
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
    NGL_OUT(_T("[%s] Del Child 0x%x <--- 0x%x (%s)\n"), GetObjectClass().GetChars(), this, pChild);
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

nuiWidgetPtr nuiSimpleContainer::GetChild(int index)
{
  CheckValid();
  NGL_ASSERT(index >= 0);
  NGL_ASSERT(index < mpChildren.size());
  return mpChildren[index];
}

nuiWidgetPtr nuiSimpleContainer::GetChild(nuiSize X, nuiSize Y)
{
  return nuiContainer::GetChild(X, Y);
}

nuiWidgetPtr nuiSimpleContainer::GetChild(const nglString& rName, bool ResolveNameAsPath)
{
  return nuiContainer::GetChild(rName, ResolveNameAsPath);
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
    ((nuiSimpleContainerIterator*)pIt)->SetIndex(0);
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
    pIt->SetIndex(0);
  return pIt;
}

nuiContainer::Iterator* nuiSimpleContainer::GetLastChild(bool DoRefCounting)
{
  CheckValid();
  IteratorPtr pIt;
  pIt = new nuiSimpleContainerIterator(this, DoRefCounting);
  if (!mpChildren.empty())
  {
    ((nuiSimpleContainerIterator*)pIt)->SetIndex(mpChildren.size() - 1);
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
  nuiSimpleContainerConstIteratorPtr pIt;
  pIt = new nuiSimpleContainerConstIterator(this, DoRefCounting);
  if (!mpChildren.empty())
  {
    pIt->SetIndex(mpChildren.size() - 1);
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
  ((nuiSimpleContainerIterator*)pIterator)->Increment();
  if (((nuiSimpleContainerIterator*)pIterator)->GetIndex() >= mpChildren.size())
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
  ((nuiSimpleContainerConstIterator*)pIterator)->Increment();
  if (((nuiSimpleContainerConstIterator*)pIterator)->GetIndex() >= mpChildren.size())
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
  if (((nuiSimpleContainerIterator*)pIterator)->GetIndex() <= 0)
  {
    pIterator->SetValid(false);
    return false;
  }
  
  ((nuiSimpleContainerIterator*)pIterator)->Decrement();
  
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
  if (((nuiSimpleContainerConstIterator*)pIterator)->GetIndex() <= 0)
  {
    pIterator->SetValid(false);
    return false;
  }
  
  ((nuiSimpleContainerConstIterator*)pIterator)->Decrement();
  
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
: nuiContainer::Iterator(pSource, DoRefCounting), mIndex(-1)
{
  mValid = false;
}

nuiSimpleContainerConstIterator::nuiSimpleContainerConstIterator(const nuiSimpleContainer* pSource, bool DoRefCounting)
: nuiContainer::ConstIterator(pSource, DoRefCounting), mIndex(-1)
{
  mValid = false;
}

nuiSimpleContainerIterator::nuiSimpleContainerIterator(const nuiSimpleContainerIterator& rIterator)
: nuiContainer::Iterator(rIterator)
{
  mIndex = rIterator.mIndex;
}

nuiSimpleContainerConstIterator::nuiSimpleContainerConstIterator(const nuiSimpleContainerConstIterator& rIterator)
: nuiContainer::ConstIterator(rIterator)
{
  mIndex = rIterator.mIndex;
}

nuiSimpleContainerIterator& nuiSimpleContainerIterator::operator = (const nuiSimpleContainerIterator& rIterator)
{
  *((nuiContainer::Iterator*)this) = rIterator;
  mIndex = rIterator.mIndex;
  return *this;
}

nuiSimpleContainerConstIterator& nuiSimpleContainerConstIterator::operator = (const nuiSimpleContainerConstIterator& rIterator)
{
  *((nuiContainer::ConstIterator*)this) = rIterator;
  mIndex = rIterator.mIndex;
  return *this;
}

bool nuiSimpleContainerIterator::SetIndex(int32 index)
{
  mIndex = index;
  return true;
}

bool nuiSimpleContainerConstIterator::SetIndex(int32 index)
{
  mIndex = index;
  return true;
}

int32 nuiSimpleContainerIterator::GetIndex() const
{
  return mIndex; 
}

int32 nuiSimpleContainerConstIterator::GetIndex() const
{
  return mIndex; 
}

nuiSimpleContainerIterator::~nuiSimpleContainerIterator()
{
}

nuiSimpleContainerConstIterator::~nuiSimpleContainerConstIterator()
{
}

nuiWidgetPtr nuiSimpleContainerIterator::GetWidget() const
{
  return IsValid() ? (nuiSimpleContainer*)mpSource->GetChild(mIndex) : NULL;
}

nuiWidgetPtr nuiSimpleContainerConstIterator::GetWidget() const
{
  return IsValid() ? const_cast<nuiSimpleContainer*>((nuiSimpleContainer*)mpSource)->GetChild(mIndex) : NULL;
}

