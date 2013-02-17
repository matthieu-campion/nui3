/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiTreeBase::nuiTreeBase()
: mTreeNodeEventSink(this)
{
  SetObjectClass(_T("nuiTreeBase"));
  mpParent = NULL;
  mpAutoSort = NULL;
}

nuiTreeBase::~nuiTreeBase()
{
  // Purge the tree:
  Deleted(NULL, this);
  while (mpChildren.size())
  {
    nuiTreePtr pTree = mpChildren.back();
    mpChildren.pop_back();
    pTree->Release();
  }

  delete mpAutoSort;
}

bool nuiTreeBase::AddChild(nuiTreePtr pChild)
{
  pChild->Acquire();
  if (!mpAutoSort)
  {
    mpChildren.push_back(pChild);
  }
  else
  {
    // Sorted insert:
    std::vector<nuiTreePtr>::iterator it = std::upper_bound(mpChildren.begin(), mpChildren.end(), pChild, PrivateSortFunction(mpAutoSort));
    mpChildren.insert(it, pChild);
  }

  mTreeNodeEventSink.Connect(pChild->Changed, &nuiTreeBase::OnChildChanged, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildAdded, &nuiTreeBase::OnChildAdded, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildDeleted, &nuiTreeBase::OnChildRemoved, pChild);
  mTreeNodeEventSink.Connect(pChild->Deleted, &nuiTreeBase::OnChildDeleted, pChild);

  pChild->SetParent(this);

  Changed();
  ChildAdded(this, pChild);
  return false;
}

bool nuiTreeBase::InsertChild(uint32 Index, nuiTreePtr pChild)
{
  if (mpAutoSort)
    return AddChild(pChild);

  if (mpChildren.size() < Index+1)
    mpChildren.resize(Index+1);
  std::vector<nuiTreePtr>::iterator it = mpChildren.begin();
  it+=Index;
  if (it == mpChildren.end())
  {
    mpChildren.resize(mpChildren.size() - 1);
    return false;    
  }
  mpChildren.insert(it, pChild);
  pChild->Acquire();
  mTreeNodeEventSink.Connect(pChild->Changed, &nuiTreeBase::OnChildChanged, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildAdded, &nuiTreeBase::OnChildAdded, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildDeleted, &nuiTreeBase::OnChildRemoved, pChild);
  mTreeNodeEventSink.Connect(pChild->Deleted, &nuiTreeBase::OnChildDeleted, pChild);

  pChild->SetParent(this);

  AutoSort();
  Changed();
  ChildAdded(this, pChild);
  return true;
}

bool nuiTreeBase::SetChild(uint32 Index, nuiTreePtr pChild)
{
  pChild->Acquire();
  if (mpChildren.size() < Index+1)
    mpChildren.resize(Index+1);
  if (mpChildren[Index])
    mpChildren[Index]->Release();
  else if (mpChildren[Index])
  {
    mTreeNodeEventSink.DisconnectSource(mpChildren[Index]->Changed);
    mpChildren[Index]->SetParent(NULL);
  }
  mpChildren[Index] = pChild;  
  mTreeNodeEventSink.Connect(pChild->Changed, &nuiTreeBase::OnChildChanged, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildAdded, &nuiTreeBase::OnChildAdded, pChild);
  mTreeNodeEventSink.Connect(pChild->ChildDeleted, &nuiTreeBase::OnChildRemoved, pChild);
  mTreeNodeEventSink.Connect(pChild->Deleted, &nuiTreeBase::OnChildDeleted, pChild);
  
  pChild->SetParent(this);

  AutoSort();
  Changed();
  ChildAdded(this, pChild);
  return true;
}

bool nuiTreeBase::DelChild(uint32 Index)
{
  NGL_ASSERT(Index < mpChildren.size());
  if (mpChildren[Index])
  {
    ChildDeleted(this, mpChildren[Index]);
    
    mTreeNodeEventSink.DisconnectSource(mpChildren[Index]->Changed);
    mTreeNodeEventSink.DisconnectSource(mpChildren[Index]->ChildAdded);
    mTreeNodeEventSink.DisconnectSource(mpChildren[Index]->ChildDeleted);
    mTreeNodeEventSink.DisconnectSource(mpChildren[Index]->Deleted);

    mpChildren[Index]->SetParent(NULL);
    mpChildren[Index]->Release();
    mpChildren.erase(mpChildren.begin()+Index);

    Changed();
  }
  return true;
}

bool nuiTreeBase::Clear(bool erase)
{
  for (uint32 i = 0; i < mpChildren.size(); i++)
  {
    nuiTreeBase* pChild = mpChildren[i];
    ChildDeleted(this, pChild);
  }
  if (erase)
  {
    std::vector<nuiTreePtr>::iterator it = mpChildren.begin();
    for (; it != mpChildren.end();)
    {
      nuiTreeBase* pChild = (*it);
      it = mpChildren.erase(it);
      pChild->Release();
    }
  }
  else
  {
    for (uint32 i = 0; i < mpChildren.size(); i++)
    {
      mpChildren[i]->SetParent(NULL);
      mTreeNodeEventSink.DisconnectSource(mpChildren[i]->Changed);
      mTreeNodeEventSink.DisconnectSource(mpChildren[i]->ChildAdded);
      mTreeNodeEventSink.DisconnectSource(mpChildren[i]->ChildDeleted);
      mTreeNodeEventSink.DisconnectSource(mpChildren[i]->Deleted);
    }
  }
  mpChildren.clear();
  Changed();
  return true;
}

bool nuiTreeBase::DelChild(nuiTreePtr pChild)
{
  for (uint32 i = 0; i < mpChildren.size(); i++)
  {
    if (mpChildren[i] == pChild && pChild)
    {
      ChildDeleted(this, pChild);
      
      mTreeNodeEventSink.DisconnectSource(pChild->Changed);
      mTreeNodeEventSink.DisconnectSource(pChild->ChildAdded);
      mTreeNodeEventSink.DisconnectSource(pChild->ChildDeleted);
      mTreeNodeEventSink.DisconnectSource(pChild->Deleted);

      pChild->SetParent(NULL);
      mpChildren[i]->Release();
      mpChildren.erase(mpChildren.begin()+i);
      
      Changed();
      return true;
    }
  }

  return false;
}

void nuiTreeBase::DelChildOnEvent(nuiEventSource& rEvent, nuiTreePtr pChild)
{
  mTreeNodeEventSink.Connect(rEvent, &nuiTreeBase::OnDelChildRequest, pChild);
}

void nuiTreeBase::OnDelChildRequest(const nuiEvent& rEvent)
{
  nuiTreePtr pChild = (nuiTreePtr)rEvent.mpUser;
  mTreeNodeEventSink.DisconnectSource(*const_cast<nuiEventSource*>(rEvent.GetSource()));
  DelChild(pChild);
}


uint32 nuiTreeBase::GetChildrenCount() const
{
  return (uint32)mpChildren.size();
}

nuiTreePtr nuiTreeBase::GetChild(uint32 Index)
{
  NGL_ASSERT(mpChildren.size() > Index);
  return mpChildren[Index];
}

const nuiTreeBase* nuiTreeBase::GetChild(uint32 Index) const
{
  NGL_ASSERT(mpChildren.size() > Index);
  return mpChildren[Index];
}

bool nuiTreeBase::GetChildren(std::vector<nuiTreePtr>& rChildren)
{
  rChildren.clear();
  rChildren.assign(mpChildren.begin(), mpChildren.end());
  return true;
}

bool nuiTreeBase::GetChildren(std::list<nuiTreePtr>& rChildren)
{
  rChildren.clear();
  rChildren.assign(mpChildren.begin(), mpChildren.end());
  return false;
}

void nuiTreeBase::OnChildChanged(const nuiEvent& rEvent)
{
  Changed();
}

void nuiTreeBase::OnChildAdded(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  ChildAdded(pTreeEvent->mpParent, pTreeEvent->mpChild);
}

void nuiTreeBase::OnChildRemoved(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  ChildDeleted(pTreeEvent->mpParent, pTreeEvent->mpChild);
}

void nuiTreeBase::OnChildDeleted(const nuiEvent& rEvent)
{
  const nuiTreeEvent<nuiTreeBase>* pTreeEvent = dynamic_cast<const nuiTreeEvent<nuiTreeBase>*>(&rEvent);
  DelChild(pTreeEvent->mpChild);
}

uint32 nuiTreeBase::GetDepth() const
{
  if (!mpParent)
    return 0;
  
  return mpParent->nuiTreeBase::GetDepth() + 1;
}

uint32 nuiTreeBase::GetTreeDepth() const
{
  uint32 Depth = 0;
  uint32 count = (uint32)mpChildren.size();
  for (uint32 i = 0; i < count; i++)
  {
    if (mpChildren[i])
      Depth = MAX(Depth, mpChildren[i]->GetDepth());
  }
  return Depth + 1;
}

int32 nuiTreeBase::ComputeIndexInParent() const
{
  std::list<nuiTreePtr> rChildren;
  GetParent()->GetChildren(rChildren);
  std::list<nuiTreePtr>::iterator it;
  
  int32 index = 0;
  for (it = rChildren.begin(); it != rChildren.end(); ++it, index++)
  {
    nuiTreePtr pChild = *it;
    if (pChild == this)
      return index;
  }
  
  return (-1);
}

bool nuiTreeBase::IsEmpty() const
{
  return mpChildren.empty();
}

const nuiTreeBase* nuiTreeBase::FindNodeFromProperty(const nglString& rPropertyName, const nglString& rPropertyValue) const
{
  if (GetProperty(rPropertyName) == rPropertyValue)
    return this;

  uint32 count = (uint32)mpChildren.size();
  for (uint32 i = 0; i < count; i++)
  {
    if (mpChildren[i])
    {
      const nuiTreeBase* pTree = mpChildren[i]->FindNodeFromProperty(rPropertyName, rPropertyValue);
      if (pTree)
        return pTree;
    }
  }

  return NULL;
}

nuiTreeBase* nuiTreeBase::FindNodeFromProperty(const nglString& rPropertyName, const nglString& rPropertyValue)
{
  if (GetProperty(rPropertyName) == rPropertyValue)
    return this;

  uint32 count = (uint32)mpChildren.size();
  for (uint32 i = 0; i < count; i++)
  {
    if (mpChildren[i])
    {
      nuiTreeBase* pTree = mpChildren[i]->FindNodeFromProperty(rPropertyName, rPropertyValue);
      if (pTree)
        return pTree;
    }
  }

  return NULL;
}

void nuiTreeBase::Sort(SortFunction& rCompare, bool DeepSort)
{
  std::stable_sort(mpChildren.begin(), mpChildren.end(), nuiTreeBase::PrivateSortFunction(&rCompare));
  Changed();

  if (DeepSort)
  {
    // Sort Children too:
    for (uint i = 0; i < mpChildren.size(); i++)
    {
      mpChildren[i]->Sort(rCompare, true);
    }
  }
}

void nuiTreeBase::SetAutoSortFunction(SortFunction* pAutoSortFunction)
{
  mpAutoSort = pAutoSortFunction;
}

void nuiTreeBase::AutoSort()
{
  if (mpAutoSort)
    Sort(*mpAutoSort);
}

//class SortFunction : public std::binary_function<nuiTreePtr, nuiTreePtr, bool>
nuiTreeBase::SortFunction::SortFunction()
{
}

nuiTreeBase::SortFunction::~SortFunction()
{
}

nuiTreeBase::PrivateSortFunction::PrivateSortFunction(SortFunction* pSortFunction)
: mpSortFunction(pSortFunction)
{
}

nuiTreeBase::PrivateSortFunction::PrivateSortFunction(const PrivateSortFunction& rRef)
: mpSortFunction(rRef.mpSortFunction)
{
}

nuiTreeBase::PrivateSortFunction::~PrivateSortFunction()
{
  mpSortFunction = NULL;
}

bool nuiTreeBase::PrivateSortFunction::operator()(const nuiTreeBase* pElem1, const nuiTreeBase* pElem2)
{
  return mpSortFunction->Compare(pElem1, pElem2);
}

void nuiTreeBase::SetParent(nuiTreeBase* pParent)
{
  mpParent = pParent;
}

nuiTreeBase* nuiTreeBase::GetParent() const
{
  return mpParent;
}
