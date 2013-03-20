/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nui.h"
#include "nuiContainer.h"

class nuiSimpleContainer : public nuiContainer
{
public:
  nuiSimpleContainer();
  
  virtual ~nuiSimpleContainer();
  
  virtual bool SetObjectClass(const nglString& rName);
  virtual void SetObjectName(const nglString& rName);
  
  virtual bool AddChild(nuiWidgetPtr pChild);
  virtual bool DelChild(nuiWidgetPtr pChild); ///< Remove this child from the object. If Delete is true then the child will be deleted too. Returns true if success.
  virtual uint GetChildrenCount() const;
  virtual nuiWidgetPtr GetChild(int index);
  virtual nuiWidgetPtr GetChild(nuiSize X, nuiSize Y);
  virtual nuiWidgetPtr GetChild(const nglString& rName, bool ResolveNameAsPath = true); ///< Find a child by its name property. Try to resolve path names like /window/fixed/toto or ../../tata if deepsearch is true
  virtual bool Clear();
  
  virtual nuiContainer::Iterator* GetFirstChild(bool DoRefCounting = false);
  virtual nuiContainer::ConstIterator* GetFirstChild(bool DoRefCounting = false) const;
  virtual nuiContainer::Iterator* GetLastChild(bool DoRefCounting = false);
  virtual nuiContainer::ConstIterator* GetLastChild(bool DoRefCounting = false) const;
  virtual bool GetNextChild(nuiContainer::IteratorPtr pIterator);
  virtual bool GetNextChild(nuiContainer::ConstIteratorPtr pIterator) const;
  virtual bool GetPreviousChild(nuiContainer::IteratorPtr pIterator);
  virtual bool GetPreviousChild(nuiContainer::ConstIteratorPtr pIterator) const;
  
  virtual void RaiseChild(nuiWidgetPtr pChild);
  virtual void LowerChild(nuiWidgetPtr pChild);
  virtual void RaiseChildToFront(nuiWidgetPtr pChild);
  virtual void LowerChildToBack(nuiWidgetPtr pChild);
  
  
protected:
  nuiWidgetList mpChildren;
};
