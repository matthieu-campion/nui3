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

class NUI_API nuiSimpleIcon : public nuiSimpleContainer
{
public:
  nuiSimpleIcon(const nglString& rName, nuiPosition pos);
  ~nuiSimpleIcon();

  virtual nuiRect CalcIdealSize();
  bool SetRect(const nuiRect& rRect);
  bool Draw(nuiDrawContext* pContext);
  
  void SetFontSize(const nuiSize& rFontSize);

  void SetColor(nuiWidgetElement elem, const nuiColor& rColor); // react to these: eSelectedTabBg, eNormalTabBg, eSelectedTextFg, eNormalTextFg

  void SetPosition(nuiPosition pos);
  nuiPosition GetPosition();
  nuiOrientation GetOrientation();

  void SetRadius(float radius);

protected:
  nglString       mName;
  nuiPosition     mPosition;
  nuiOrientation  mOrientation;
  nuiGradient*    mpGradient;
  nuiGradient*    mpSelectedGradient;
  nuiSize         mFontSize;
  nuiFont*        mpFont;
  nuiLabel*       mpLabel;
  nuiShape*       mpShape;
  float           mRadius;

  void ResetGradient();
  void ResetSelectedGradient();
};

class NUI_API nuiTabView : public nuiSimpleContainer
{
public:
  nuiTabView(nuiPosition pos);
  virtual ~nuiTabView();

  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);
  
  virtual bool MouseClicked   (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  virtual bool MouseUnclicked (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
    
  bool OnIconClicked(const nuiEvent& rEvent);

  virtual void AddTab(nuiWidget* pTab, nuiWidget* pIcon);
  virtual void InsertTab(nuiWidget* pTab, nuiWidget* pIcon, uint position);
  virtual void RemoveTab(nuiWidget* pTab, bool trashit = true);
  virtual void RemoveTab(const uint& tab_index, bool trashit = true);
   
  uint GetTabCount();

  nuiPosition GetPosition();
  
  nuiOrientation GetOrientation();
  void SetPosition(nuiPosition position);

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
  nuiPosition mPosition;

  std::vector<nuiWidget*> mTabs;
  std::vector<nuiWidget*> mIcons;
  nuiRect mIdealIconsRect;
  nuiRect mIdealTabsRect;

  bool mChildrenRectUnion;
  bool mFoldable;
  bool mFolded;
  bool mSliding;
  float mFoldRatio;
};

#endif//__nuiTabView_h__

