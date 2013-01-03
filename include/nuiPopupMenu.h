/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiPopupMenu_h__
#define __nuiPopupMenu_h__

#include "nuiContainer.h"
#include "nuiTree.h"
#include "nuiTreeView.h"
#include "nuiScrollBar.h"

class nuiTimer;
class nuiMenuRect;

class NUI_API nuiPopupMenu : public nuiSimpleContainer
{
public:
  nuiPopupMenu(nuiWidget* pParent = NULL, nuiTreeNode* pTree = NULL, const nuiRect& rRect = nuiRect(), bool OwnTree = true);
  virtual ~nuiPopupMenu();

  virtual bool Draw(nuiDrawContext* pContext);

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  void SetTrashRemoval(bool); ///< If this is true the menu will be trashed as soon as a menu entry have been selected. Otherwise the MenuRemove call back will be called.
  virtual bool IsParentActive() const;
  // commented out: see definition
  // virtual bool DelChild(nuiWidgetPtr pChild,bool Delete = false);

  // accessors
  void ShowFirstNode(bool show);
  nuiTreeNode* GetSelectedNode() { return mpSelectedNode; };
  int GetIdInTree(nuiTreeNode* pNode);
  std::list<nuiTreeNode*> GetTreeNodesCourse();
  std::list<int> GetTreeIdsCourse();

  //! Received Events:
  bool MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseUnclicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  bool MouseMoved(nuiSize X, nuiSize Y);
  bool KeyDown(const nglKeyEvent& rEvent);


  nuiTreeNode* GetTree(); ///< Return the tree model used by this widget.
  const nuiTreeNode* GetSelectedNode() const;

  void SetKey(const nglString& rKey);
  const nglString& GetKey() const;

  // Sinkable events
  nuiSimpleEventSource<nuiMenuDone> MenuDone;
  //  nuiSimpleEventSource<nuiNodeDClicked> NodeDClicked;

protected:
  virtual bool DrawTree(nuiDrawContext* pContext, nuiTreeNode* pTree, uint depth);
  virtual void CalcTreeSize(nuiRect& rRect, nuiTreeNode* pTree, uint32& cpt);
  virtual void SetTreeRect(uint depth, nuiSize X, nuiSize Y, nuiTreeNode* pTree);
  virtual void ReparentTree(nuiTreeNode* pTree);
  virtual void UnparentTree(nuiTreeNode* pTree);
  virtual void RemoveMenu(); ///< Handles removal of the menu once an item have been selected.
  // bool DelChild(nuiTreePtr pTree, nuiWidgetPtr pChild, bool Delete);

  nuiTreeNode* GetParentNode(nuiTreeNodePtr pParent, nuiTreeNodePtr pNode);
  nuiTreeNode* mpTree;
  nuiEventSink<nuiPopupMenu> mPopupTreeSink;

  virtual void OnSelectionTimer(const nuiEvent& rEvent);
  virtual void OnTreeChanged(const nuiEvent& rEvent);
  virtual void OnTreeChildAdded(const nuiEvent& rEvent);
  virtual void OnTreeChildDeleted(const nuiEvent& rEvent);
  virtual void OnScrollBarChange(const nuiEvent& rEvent);
  virtual void OnScrollTimer(const nuiEvent& rEvent);
  bool mScrollUp;

  friend class nuiMenuRect;
private:
  nuiTreeNode* GetNextNode(nuiTreeNode* pParent, nuiTreeNode* pNode, int increment);
  nuiSize      AdjustRectsPos(nuiSize& rX, nuiSize& rY, uint depth, nuiRect CurRect);
  nuiSize      AdjustPosToSize(nuiMenuRect* rPosRect);
  void         AdjustScrolling(nuiTreeNode* pNode);
  nuiTreeNode* SearchNode(nuiSize X, nuiSize Y, int cpt);
  int          SearchRectIndex(nuiSize X, nuiSize Y);
  void         FillSelectedNodes();

  void         IncrementScrollBar(bool increment);
  void         ResetScrolling(uint32 rect);

  nuiRect      mInitialPos;
  int          mXdir, mYdir;

  nuiTreeNode* mpSelectedNode;
  nuiTreeNode* mpNewSelectedNode;
  std::list<nuiTreeNode*>    mSelectedNodes;
  std::vector<nuiMenuRect*>  mRects;
  std::vector<nuiScrollBar*> mSBarsPool;

  bool mTrashRemoval; ///< If this is true the menu will be trashed as soon as a menu entry have been selected. Otherwise the MenuRemove call back will be called.
  bool mShowFirstNode;
  bool mWheelMoved;  
  bool mFirstClick;
  bool mOwnTree;
  bool mScrolledFromKeyboard;
  nuiTimer mSelectionTimer;
  nuiTimer mScrollTimer;
  nglTime mStartTime;
  double mDelayTime;

  nuiWidget*  mpOldFocused; // Used to set focus back to the old focused widget

  nglString mKey;
};


class NUI_API nuiMenuRect
{
public:
  nuiMenuRect(nuiPopupMenu* pParent, uint32 depth)
  {
    mDepth = depth;
    if (!pParent->mSBarsPool.empty())
    {
      mpSBar = pParent->mSBarsPool.back();
      pParent->mSBarsPool.pop_back();
    }
    else
    {
      mpSBar = new nuiScrollBar(nuiVertical);
      pParent->AddChild(mpSBar);
      mpSBar->SetVisible(false);
      mpSBar->SetEnabled(false);
    }
    mpFromNode = NULL;
    mHasNonEmpty = false;
  }
  virtual ~nuiMenuRect()
  {
  }
  uint32        mDepth;
  nuiRect        mRect;
  nuiScrollBar*  mpSBar;
  nuiTreeNode*  mpFromNode;
  bool          mHasNonEmpty;
protected:
private:
  nuiMenuRect(const nuiMenuRect& rRect);
  void operator =(const nuiMenuRect& rRect);
};

#endif//__nuiPopupMenu_h__

