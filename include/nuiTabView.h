/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTabView_h__
#define __nuiTabView_h__

#include "nuiContainer.h"
#include <vector>

class nuiGradient;
class nuiFont;
class nuiLabel;

class NUI_API TabEvent : public nuiEvent
{
public:
  TabEvent(const uint& index);
  uint mTabIndex;
};



class NUI_API nuiTabView : public nuiSimpleContainer
{
public:
  nuiTabView(nuiPosition tabPosition, bool decoratedBackground = true);
  virtual ~nuiTabView();

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);
  
  virtual bool MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
    
  bool OnIconClicked(const nuiEvent& rEvent);

  virtual void AddTab(const nglString& rTitle, nuiWidget* pContents);
  virtual void AddTab(nuiWidget* pTitle, nuiWidget* pContents);
  virtual void InsertTab(nuiWidget* pTitle, nuiWidget* pContents, uint position);
  virtual void RemoveTab(nuiWidget* pTab, bool trashit = true);
  virtual void RemoveTab(const uint& tab_index, bool trashit = true);
   
  uint GetTabCount();

  nuiPosition GetTabPosition();
  
  nuiOrientation GetOrientation();
  void SetTabPosition(nuiPosition tabPosition);

  void SelectTab(const uint& index);
  uint GetSelectedTab() const { return mCurrentTabIndex; }
  void EnableChildrenRectUnion(bool set);
  bool GetChildrenRectUnion() const;

  void SetFoldable(bool Foldable);
  bool GetFoldable() const;

  void SetFolded(bool set, bool Animate = true);
  bool GetFolded() const;

  nuiEventSink<nuiTabView> mTabViewEvents;
  nuiSimpleEventSource<0> TabSelect;
protected:
  uint mCurrentTabIndex;
  nuiPosition mTabPosition;

  std::vector<nuiWidget*> mTabs;
  std::vector<nuiWidget*> mIcons;
  nuiRect mIdealIconsRect;
  nuiRect mIdealTabsRect;

  bool mChildrenRectUnion;
  bool mFoldable;
  bool mFolded;
  bool mSliding;
  bool mDecoratedBackground;
  float mFoldRatio;
};

#endif//__nuiTabView_h__

