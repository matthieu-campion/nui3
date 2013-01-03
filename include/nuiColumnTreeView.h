/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiColumnTreeView_h__
#define __nuiColumnTreeView_h__

#include "nuiTree.h"
#include "nuiTreeView.h"
#include "nuiContainer.h"

class NUI_API nuiColumnTreeView : public nuiTreeView
{
public:
  nuiColumnTreeView(nuiTreeNode* pTree = NULL);
  virtual ~nuiColumnTreeView();

  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiTreeNode* DrawColumn(nuiDrawContext* pContext, nuiTreeNode* pTree);
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);

  //! Received Events:
  bool KeyDown(const nglKeyEvent& rEvent);
  bool KeyUp(const nglKeyEvent& rEvent);
  
  bool MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved  (nuiSize X, nuiSize Y);

  nuiTreeNodePtr GetNodePath(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode, std::list<nuiTreeNodePtr>& rPath);
  nuiTreeNodePtr GetParentNode(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode);

  void SetInterline(float Set)  { mInterline = Set; InvalidateLayout(); }
  void SetHandleSize(float Set) { mHandleSize = Set; InvalidateLayout(); }
  void SetBarSize(float Set)    { mBarSize = Set; InvalidateLayout(); }

  float GetInterline() const  { return mInterline; }
  float GetHandleSize() const { return mHandleSize; }
  float GetBarSize() const    { return mBarSize; }

  void SetPreview(nuiWidgetPtr pPreview); ///< Set the widget that is used to display the preview for the currently selected item.
  nuiWidgetPtr GetPreview() const; ///< Set the widget that is used to display the preview for the currently selected item.

  virtual void SetTree(nuiTreeNodePtr pTree);

  void SetKey(const nglString& rKey);
  const nglString& GetKey() const;

  nuiMouseUnclicked ContextMenuRequested; ///< This event is called whenever an item is chosen with the right mouse button in order to display a context sensitive menu or do special actions.

  //! Selection support
  virtual nuiTreeNodePtr FindNode(nuiSize X, nuiSize Y);

protected:
  bool DispatchMouseClick(const nglMouseInfo& rInfo);
  nuiTreeNode* CalcColumnSize(nuiRect& rRect, nuiTreeNode* pTree);
  nuiTreeNode* SetColumnRect(const nuiRect& rRect, nuiTreeNode* pTree);
  void InvalidateColumn(const nuiEvent& rEvent);
  void CreateScrollBars();

  nuiSize mMinColumnSize;
  std::vector<nuiRect> mColumnSizes;
  std::vector<nuiScrollBar*> mpScrollBars;
  nuiEventSink<nuiColumnTreeView> mColumnTreeViewSink;

  float mInterline;
  float mHandleSize;
  float mBarSize;

  virtual void OnTreeChildAdded(const nuiEvent& rEvent);
  virtual void OnTreeChildDeleted(const nuiEvent& rEvent);
  virtual void OnTreeChanged(const nuiEvent& rEvent);

  uint32 GetDepth();
  void CalcHotRect();
  void CalcVerticalHotRect();
  bool mNeedCalcVerticalHotRect;

  nuiWidget* mpPreview;
  bool mActivateOnUnclick;
  
  nuiTimer* mpButtonTimer;
  int mButtonDepth;
  void InitTimer();
  enum TimedButtonEvents
  {
    eUpPressed,
    eDownPressed
  };
  TimedButtonEvents mTimedButtonEvent;
  void OnButtonTimerTick(const nuiEvent& rEvent);
  void StopButtonTimer(const nuiEvent& rEvent);
  
  void ArrowUpClicked(const nuiEvent& rEvent);
  void ArrowDownClicked(const nuiEvent& rEvent);

  nglString mKey;
};

class NUI_API nuiColumnTreeViewPreview : public nuiSimpleContainer
{
public:
  nuiColumnTreeViewPreview(nuiColumnTreeView* pParent);
  virtual ~nuiColumnTreeViewPreview();

  virtual bool UpdatePreview(nuiColumnTreeView* pTree, nuiTreeNodePtr pSelectedNode); ///< This method is called when the currently selected item has changed in the parent ColumnTreeView. The user is responsible for changing the representation of the preview widget by overridding it. Return false if the preview should not be displayed, true otherwise.

protected:
  virtual void OnUpdatePreview(const nuiEvent& rEvent);
  nuiEventSink<nuiColumnTreeViewPreview> mPreviewSink;
};



#endif
