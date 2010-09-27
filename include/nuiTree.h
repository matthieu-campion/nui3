/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTree_h__
#define __nuiTree_h__

#include "nuiObject.h"
#include "nuiTreeEvent.h"

typedef class nuiTreeBase* nuiTreePtr;


class NUI_API nuiTreeBase : public nuiObject
{
public:
  class NUI_API SortFunction
  {
  public:
    SortFunction();
    virtual ~SortFunction();

    virtual bool Compare(const nuiTreeBase* pElem1, const nuiTreeBase* pElem2) = 0;
  };

  nuiTreeBase();
  virtual ~nuiTreeBase();

  bool AddChild(nuiTreePtr pChild);
  bool InsertChild(uint32 Index, nuiTreePtr pChild);
  bool SetChild(uint32 Index, nuiTreePtr pChild);
  bool DelChild(uint32 Index);
  bool DelChild(nuiTreePtr pChild);
  void DelChildOnEvent(nuiEventSource& rEvent, nuiTreePtr pChild);
  bool Clear(bool erase = true);

  uint32 GetChildrenCount() const;
  nuiTreePtr GetChild(uint32 Index);
  const nuiTreeBase* GetChild(uint32 Index) const;
  bool GetChildren(std::vector<nuiTreePtr>& rChildren);
  bool GetChildren(std::list<nuiTreePtr>& rChildren);

  const nuiTreeBase* FindNodeFromProperty(const nglString& rPropertyName, const nglString& rPropertyValue) const;
  nuiTreeBase* FindNodeFromProperty(const nglString& rPropertyName, const nglString& rPropertyValue);

  int32 ComputeIndexInParent() const; ///< compute dynamically the position index of the item in its parent. take care, the cost of this method depends on the number of children in the parent. returns (-1) if error
  uint32 GetTreeDepth() const;
  uint32 GetDepth() const;
  virtual bool IsEmpty() const;

  nuiSimpleEventSource<nuiChanged> Changed;
  nuiChildAddedEventSource ChildAdded;
  nuiChildDeletedEventSource ChildDeleted;
  nuiChildDeletedEventSource Deleted; ///< This is deleted

  void Sort(SortFunction& rCompare, bool DeepSort = false);
  void SetAutoSortFunction(SortFunction* pAutoSortFunction);

  nuiTreeBase* GetParent() const;

protected:
  void OnChildChanged(const nuiEvent& rEvent);
  void OnChildAdded(const nuiEvent& rEvent);
  void OnChildRemoved(const nuiEvent& rEvent);
  void OnChildDeleted(const nuiEvent& rEvent);
  void OnDelChildRequest(const nuiEvent& rEvent);
  nuiEventSink<nuiTreeBase> mTreeNodeEventSink;
  
  std::vector<nuiTreePtr> mpChildren;
  SortFunction* mpAutoSort;
  void AutoSort();
 
  class PrivateSortFunction : std::binary_function<const nuiTreeBase*, const nuiTreeBase*, bool>
  {
  public:
    PrivateSortFunction(SortFunction* pSortFunction);
    PrivateSortFunction(const PrivateSortFunction& rRef);
    virtual ~PrivateSortFunction();

    bool operator()(const nuiTreeBase* pElem1, const nuiTreeBase* pElem2);
  private:
    SortFunction* mpSortFunction;
  };

private:
  void SetParent(nuiTreeBase* pParent);
  nuiTreeBase* mpParent;
};

template <class T> class NUI_API nuiTree : public nuiTreeBase
{
public:
  nuiTree(T* pElement, bool OwnElement = true)
  {
    mpElement = pElement;
    mOwnElement = OwnElement;
  }

  virtual ~nuiTree()
  {
    if (mOwnElement)
    {
      delete mpElement;
    }
  }

  T* GetElement()
  {
    return mpElement;
  }

  void SetElement(T* pNewElement, bool DeletePrevious = true, bool OwnNewElement = true) ///< Beware what you are doing if you decide to use this method!
  {
    if (mOwnElement && DeletePrevious)
      delete mpElement;
    mpElement = pNewElement;
    mOwnElement = OwnNewElement;
  }
  
  const T* GetElement() const
  {
    return mpElement;
  }

  bool IsElementOwned() const
  {
    return mOwnElement;
  }

  void SetOwnElement(bool OwnElement)
  {
    mOwnElement = OwnElement;
  }

protected:
  bool mOwnElement;
  T* mpElement;
};


template <class T> class NUI_API nuiValueTree : public nuiTreeBase
{
public:
  nuiValueTree(const T& rElement)
  {
    mElement = rElement;
  }

  virtual ~nuiValueTree()
  {
  }

  T& GetElement()
  {
    return mElement;
  }

  const T& GetElement() const
  {
    return mElement;
  }

protected:
  T mElement;
};


#endif

