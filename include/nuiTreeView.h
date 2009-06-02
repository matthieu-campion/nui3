/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTreeView_h__
#define __nuiTreeView_h__

#include "nuiTree.h"
#include "nuiContainer.h"

#if !defined _NODND_
#include "nglDragAndDropObjects.h"
#endif

typedef class nuiTreeNode* nuiTreeNodePtr;

class NUI_API nuiTreeNode : public nuiTree<nuiWidget>
{
public:
  nuiTreeNode(nuiWidgetPtr pElement, bool Opened = false, bool Selected = false, bool DrawTreeHandle = true, bool alwaysDisplayTreeHandle=false);
  nuiTreeNode(const nglString& rLabelName, bool Opened = false, bool Selected = false, bool DrawTreeHandle = true, bool alwaysDisplayTreeHandle=false);
  virtual ~nuiTreeNode();

  bool IsOpened() const;
  virtual void Open(bool Opened);

  bool IsSelected() const;
  virtual void Select(bool Select, bool Temporary = false);
  void Select(const nuiRect& rRect, bool Sel, bool Temporary = false);
  void OpenAllChildren(bool Opened);
  void SelectAllChildren(bool Selected);
  void SelectAllVisibleChildren(bool Selected);
  void GetSelected(std::list<nuiTreeNodePtr>& rSelected, bool IncludeInvisibleItem = false); ///< Return the list of selected nodes in this sub tree.

  void EnableSelection(bool enable=true);
  bool IsSelectionEnabled();

  bool IsTreeHandleDrawned() const;

  nuiSize GetPosition() const;
  nuiSize GetSize() const;
    
  void SetPosition(nuiSize Position);
  void SetSize(nuiSize Size);

  nuiTreeNodePtr FindNode(nuiSize X, nuiSize Y);

  nuiTreeNodePtr GetNextInParent();
  nuiTreeNodePtr GetPreviousInParent();
  nuiTreeNodePtr GetLastOpenChild();
  nuiTreeNodePtr GetNextOpen();
  nuiTreeNodePtr GetPreviousOpen();

  virtual nuiWidgetPtr GetSubElement(uint32 index);
  
  
  nuiSimpleEventSource<nuiWidgetActivated> Activated;
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged;
  nuiSimpleEventSource<nuiWidgetSelected> Opened;
  nuiSimpleEventSource<nuiWidgetSelected> Closed;
protected:
  bool mOpened;
  bool mSelected;
  bool mTempSelected;
  bool mDrawTreeHandle;
  bool mSelectionEnabled;
  bool mAlwaysDisplayTreeHandle;

  friend class nuiTreeView;
  friend class nuiColumnTreeView;
  friend class nuiPopupMenu;

  nuiSize mPosition;
  nuiSize mSize;

}; ///< This is the element that can be added to a tree view. 





/// implements a nuiTreeNode with an additional user value, that can be retrieved when the treenode is selected
template <class T> class NUI_API nuiTreeNodeValue : public nuiTreeNode
{
public:

  nuiTreeNodeValue (nuiWidgetPtr pElement, const T& rValue, bool Opened = false, bool Selected = false, bool DrawTreeHandle = true, bool alwaysDisplayTreeHandle=false)
  : nuiTreeNode(pElement, Opened, Selected, DrawTreeHandle, alwaysDisplayTreeHandle),
    mValue(rValue)
  {
  }
  
  nuiTreeNodeValue(const nglString& rLabelName, const T& rValue, bool Opened = false, bool Selected = false, bool DrawTreeHandle = true, bool alwaysDisplayTreeHandle=false)
  : nuiTreeNode(rLabelName, Opened, Selected, DrawTreeHandle, alwaysDisplayTreeHandle),
    mValue(rValue)
  {
  }


  virtual ~nuiTreeNodeValue()
  {
  }

  T& GetValue()
  {
    return mValue;
  }

  const T& GetValue() const
  {
    return mValue;
  }

protected:
  T mValue;
};



#if !defined _NODND_
typedef nuiFastDelegate1<nuiTreeNode*,nglDragAndDrop*>  DragStartDelegate;
typedef nuiFastDelegate3<nuiTreeNode*,nglDragAndDrop*,const nglString&>  DragRequestDataDelegate;
typedef nuiFastDelegate2<nuiTreeNode*,bool>  DragStopDelegate;
#endif

class NUI_API nuiTreeView : public nuiSimpleContainer
{
public:
  nuiTreeView(nuiTreeNode* pTree = NULL, bool displayRoot=true);
  bool Load(const nuiXMLNode* pNode);
  virtual ~nuiTreeView();

  virtual nuiXMLNode* Serialize(nuiXMLNode* pParentNode, bool Recursive) const;

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  //! Received Events:
  virtual bool KeyDown(const nglKeyEvent& rEvent);
  virtual bool KeyUp(const nglKeyEvent& rEvent);
  
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);

  bool DispatchMouseClick(const nglMouseInfo& rInfo);

  //! Rendering modifiers:
  void SetDepthInset(uint32 depth, nuiSize inset);
  nuiSize GetDepthInset(uint32 depth);

  //! Selection support
  virtual nuiTreeNodePtr FindNode(nuiSize X, nuiSize Y);
  virtual void Select(const nuiRect& rRect, bool Sel, bool TempSelection = false);
  virtual void Select(nuiTreeNode* pNode, bool Sel, bool TempSelection = false);
  virtual void SelectAll(bool Selected);
  virtual bool IsMultiSelectable();
  virtual bool IsDeSelectable();
  virtual void SetMultiSelectable(bool Set);
  virtual void SetDeSelectable(bool Set);
  void StartMultiSelection();
  void StopMultiSelection();
  bool IsMultiSelecting() const;

  virtual void SetTree(nuiTreeNodePtr pTree, bool DeleteOldTree = true); ///< Change the tree to be displayed in the widget.
  nuiTreeNodePtr GetTree(); ///< Return the tree model used by this widget.
  void GetSelected(std::list<nuiTreeNodePtr>& rSelected, bool IncludeInvisibleItem = false); ///< Return the currently selected nodes (by default only return the visible ones).
  const nuiTreeNode* GetSelectedNode() const;
  nuiTreeNode* GetSelectedNode();

  const nuiColor& GetHandleColor();
  void SetHandleColor(const nuiColor& rColor);
  
  void EnableSubElements(uint32 count);
  
  nuiMouseClicked Clicked; ///< This event is called whenever an item is clicked.
  
  nuiSimpleEventSource<nuiWidgetActivated> Activated; ///< This event is called whenever an item is chosen, that is to say, double clicked. This can only happend in a mono-selection tree.
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the tree have changed. 
  nuiSimpleEventSource<nuiWidgetSelected> SelectionDone; ///< This event is called whenever a selection has been finished by an unclick

#if !defined _NODND_
  void SetDragStartDelegate(const DragStartDelegate& rDelegate);
  void SetDragRequestDataDelegate(const DragRequestDataDelegate& rDelegate);
  void SetDragStopDelegate(const DragStopDelegate& rDelegate);
#endif
  
protected:
  virtual bool DrawTree(nuiDrawContext* pContext, uint32 Depth, nuiTreeNode* pTree);
  virtual void CalcTreeSize(nuiRect& rRect, uint32 Depth, nuiTreeNode* pTree);
  virtual void SetTreeRect(nuiSize& Y, uint32 Depth, nuiTreeNode* pTree);
  virtual void ReparentTree(nuiTreeNode* pTree);

  nuiTreeNodePtr mpTree;
  nuiEventSink<nuiTreeView> mTreeViewSink;

  virtual bool OnTreeChanged(const nuiEvent& rEvent);
  virtual bool OnTreeChildAdded(const nuiEvent& rEvent);
  virtual bool OnTreeChildDeleted(const nuiEvent& rEvent);
  
  void InitAttributes();


    
  bool mDisplayRoot;  
  bool mMultiSelectable;
  bool mInMultiSelection;
  bool mDeSelectable;
  
  nuiColor mHandleColor;


  nuiSize mClickX;
  nuiSize mClickY;
  nuiSize mNewX;
  nuiSize mNewY;
  nuiSize mOldX;
  nuiSize mOldY;

  bool mClicked;
  bool mDrawMarkee;
  std::vector<nuiSize> mDepthInsets;

  nuiTreeNode* mpSelectedNode;

  class SubElement
  {
  public:
    SubElement(nuiSize width);
    nuiSize mWidth;
    nuiSize mIdealWidth;
    nuiSize mPosition;
    nuiSize mMinWidth;
    nuiSize mMaxWidth;
  };
  nuiSize mTreeIdealWidth;
  std::vector<SubElement> mSubElements;
  static nuiSize mDefaultSubElementWidth;
  
private:
  
  
#if !defined _NODND_
  
  virtual void OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType); ///< This method is called on the drag and drop source widget by the window manager whenever the drag & drop operation was accepted by the user (by releasing the mouse button on a widget that support the dragged object type). This is the last time the source widget is allowed to place data in the drag and dropped object. 
  virtual void OnDragStop(bool canceled); ///< called when a drag operation is interupted or finished
  
  DragStartDelegate mDragStartDelegate;
  DragRequestDataDelegate mDragRequestDataDelegate;
  DragStopDelegate mDragStopDelegate;
  
  bool mDragging;
  nglDragAndDrop* mpDraggedObject;
#endif
};

#endif
