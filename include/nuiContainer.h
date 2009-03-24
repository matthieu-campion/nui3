/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiContainer_h__
#define __nuiContainer_h__

#include "nuiWidget.h"
#include "nuiEvent.h"
#include "nuiTreeEvent.h"

typedef nuiTreeEventSource<nuiChildAdded, nuiWidget> nuiWidgetAddedEventSource;
typedef nuiTreeEventSource<nuiChildDeleted, nuiWidget> nuiWidgetDeletedEventSource;

class NUI_API nuiContainer : public nuiWidget
{
  friend class nuiWidget;
  friend class nuiTopLevel;
public:
  class NUI_API ChildIteratorBase
  {
  public:
    ChildIteratorBase()
    {
      mRefs = 0;
    }

    virtual ~ChildIteratorBase()
    {
      NGL_ASSERT(!mRefs);
    }

    void Acquire()
    {
      mRefs++;
    }

    uint32 Release()
    {
      NGL_ASSERT(mRefs > 0);
      mRefs--;
      int32 r = mRefs;
      if (!mRefs)
        delete this;
      return r;
    }

    bool IsValid() const
    {
      return mValid;
    }
        
  protected:
    void SetValid(bool set)
    {
      mValid = set;
    }
    
  private:
    uint32 mRefs;
    bool mValid;
    
  };

  class NUI_API Iterator
  {
  public:
    Iterator();
    Iterator(const Iterator& rIterator);
    Iterator& operator = (const Iterator& rIterator);
    virtual ~Iterator();

    virtual bool IsValid() const;
    virtual nuiWidgetPtr GetWidget() const;
    virtual nuiWidgetPtr operator*() const
    {
      return GetWidget();
    }
  protected:
    nuiWidgetList::iterator mIterator;
    bool mValid;
  private:
    friend class nuiSimpleContainer;
    bool SetElement(const nuiWidgetList::iterator& rIterator);
    void SetValid(bool Valid);
    nuiWidgetList::iterator& GetElement();
  };

  class NUI_API ConstIterator
  {
  public:
    ConstIterator();
    ConstIterator(const ConstIterator& rIterator);
    ConstIterator& operator = (const ConstIterator& rIterator);
    virtual ~ConstIterator();

    virtual bool IsValid() const;
    virtual nuiWidgetPtr GetWidget() const;
    virtual nuiWidgetPtr operator*() const
    {
      return GetWidget();
    }
  protected:
    nuiWidgetList::const_iterator mIterator;
    bool mValid;
  private:
    friend class nuiSimpleContainer;
    bool SetElement(const nuiWidgetList::const_iterator& rIterator);
    void SetValid(bool Valid);
    nuiWidgetList::const_iterator& GetElement();
  };

  typedef Iterator* IteratorPtr;
  typedef ConstIterator* ConstIteratorPtr;

  /** @name Life */
  //@{
  nuiContainer(); ///< Create an nuiObject and give him his parent.
  virtual bool Load(const nuiXMLNode* pNode); ///< Create an nuiObject from an XML description.
  virtual ~nuiContainer(); 
  //@}

  virtual bool SetObjectClass(const nglString& rName);
  virtual void SetObjectName(const nglString& rName);
  
  /** @name Serialization */
  //@{
  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive = false) const;
	virtual	void SerializeChildren(nuiXMLNode* pParentNode, bool Recursive = false) const; // top level children serialization
	virtual	nuiXMLNode* SerializeAttributes(nuiXMLNode* pParentNode, bool Recursive = false) const;
  //@}

  /** @name Object relation management */
  //@{
  virtual nuiWidgetPtr GetChild(int index); ///< Returns the child which has the given index (first child = 0). Return NULL in case of faillure.
  virtual nuiWidgetPtr GetChild(nuiSize X, nuiSize Y); ///< Returns the child which is under the pixel (X,Y) in this object or this if there is no such child. X and Y are given in the coordinate system of the parent object.
  
  nuiWidgetPtr GetChildIf(nuiSize X, nuiSize Y, TestWidgetFunctor* pFunctor); ///< Returns the child that satisfies the given functor object and that is under the pixel (X,Y) in this object or this if there is no such child. X and Y are given in the coordinate system of the parent object. rFunctor is a std::unary_functor<nuiWidgetPtr, bool> object defined by the user.
  virtual nuiWidgetPtr GetChild(const nglString& rName, bool deepsearch = true); ///< Find a child by its name property. Try to resolve path names like /window/fixed/toto or ../../tata if deepsearch is true
  nuiWidgetPtr SearchForChild(const nglString& rName, bool recurse = true);  ///< Find a child by its name property, recurse the search in the subchildren if asked politely. 
  virtual bool Clear(bool Delete = true) = 0; ///< Clear all children. By default the children are deleted unless Delete == false.
  nuiContainerPtr GetRoot() const;
  nuiWidgetPtr Find (const nglString& rName); ///< Finds a node given its full path relative to the current node. Eg. Find("background/color/red").

  virtual uint GetChildrenCount() const = 0; ///< Returns the number of children this object has.
  virtual IteratorPtr GetFirstChild() = 0; 
  virtual IteratorPtr GetLastChild() = 0; 
  virtual bool GetNextChild(IteratorPtr pIterator) = 0;
  virtual bool GetPreviousChild(IteratorPtr pIterator) = 0;
  virtual ConstIteratorPtr GetFirstChild() const = 0;
  virtual ConstIteratorPtr GetLastChild() const = 0;
  virtual bool GetNextChild(ConstIteratorPtr pIterator) const = 0;
  virtual bool GetPreviousChild(ConstIteratorPtr pIterator) const = 0;

  virtual IteratorPtr GetChildIterator(nuiWidgetPtr pChild);
  virtual ConstIteratorPtr GetChildIterator(nuiWidgetPtr pChild) const;
  virtual nuiWidgetPtr GetNextFocussableChild(nuiWidgetPtr pChild) const;
  virtual nuiWidgetPtr GetPreviousFocussableChild(nuiWidgetPtr pChild) const;
  //@}

  /** @name Outgoing events */
  //@{
  nuiWidgetAddedEventSource ChildAdded;
  nuiWidgetDeletedEventSource ChildDeleted;
  //@}

  // from nuiWidget:
  virtual void EnableAutoClipSelf(bool Set, bool Recurse = false);
  virtual void InvalidateChildren(bool Recurse);
  virtual void SilentInvalidateChildren(bool Recurse);
  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual void SetAlpha(float Alpha);
  virtual void SetEnabled(bool set);
  virtual void SetSelected(bool set);
  virtual bool Trash();

  virtual void OnChildHotRectChanged(nuiWidget* pChild, const nuiRect& rChildHotRect);
  virtual void SetVisible(bool Visible); ///< Show or hide the widget

  /** @name Private event management system (do not override unless you know what you're doing!!!) */
  //@{
  /// Beware: these three methods receive the mouse coordinates in the repair of the root object!
  virtual bool DispatchMouseClick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool DispatchMouseUnclick (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual nuiWidgetPtr DispatchMouseMove (nuiSize X, nuiSize Y);
  //@}

  virtual bool DelChild(nuiWidgetPtr pChild,bool Delete=false) = 0;

protected:
  
  void DrawChild(nuiDrawContext* pContext, nuiWidget* pChild); /// Draw the given widget as a child of this widget.
  bool DrawChildren(nuiDrawContext* pContext); ///< This helper method calls the DrawMethod on all the children of the nuiObject using the DrawChild method. 
  void BroadcastVisible();
  virtual void CallConnectTopLevel(nuiTopLevel* pTopLevel); ///< Connects container to Top Level, then asks children to connect.
  virtual void CallDisconnectTopLevel(nuiTopLevel* pTopLevel); ///< Disconnects container from Top Level, then asks children to disconnect too.
  virtual void CallOnTrash();
  void ChildrenCallOnTrash();
  virtual void InternalResetCSSPass();
  void InternalSetLayout(const nuiRect& rect, bool PositionChanged, bool SizeChanged);

};

#endif // __nuiContainer_h__
