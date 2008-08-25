/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiComboBox_h__
#define __nuiComboBox_h__

//#include "nui.h"
#include "nuiWidget.h"

#include "nuiPopupMenu.h"

class NUI_API nuiComboBox : public nuiSimpleContainer
{
public:
  nuiComboBox();
  nuiComboBox(nuiTreeNode* pChoicesTree, bool ownTree = false);
  bool Load(nuiXMLNode* pNode);
  virtual ~nuiComboBox();
  
  virtual nuiRect CalcIdealSize();
  virtual bool    Draw(nuiDrawContext* pContext);
  virtual bool    SetRect(const nuiRect& rRect);

  virtual bool    MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);
  
  nuiSimpleEventSource<0> ValueChanged;
  bool OnSelect(const nuiEvent& rEvent);

  void SetMenuTextColor(const nuiColor &NormalTextColor, const nuiColor &SelectedTextColor, const nuiColor &DisabledTextColor);

  nuiWidget*    GetSelectedWidget();
  nuiTreeNode*  GetSelected();
  const nuiTreeNode* GetChoices() const;
  uint32        GetValue();
  void          SetSelected(nuiTreeNode* pSelection);
  void          SetSelected(uint32 childIndex);
  void          SetValue(uint32 childIndex);

  void          SetHandleSize(nuiSize width, nuiSize height);
  void          GetHandleSize(nuiSize& rWidth, nuiSize& rHeight);
  void          SetHandleOffset(nuiSize offset);
  nuiSize       GetHandleWidth() const;
  nuiSize       GetHandleHeight() const;
  nuiSize       GetHandleOffset() const;

  bool IsBoxDisplayed() const;
  void SetBoxDisplayed(bool set);

  void SetTree(nuiTreeNode* pTree);
  
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the list have changed. 

protected:
  void ReparentTree(nuiTreeNode* pTree);
  void UnparentTree(nuiTreeNode* pTree);
  nuiTreeNode*  mpChoicesTree;
  bool          mOwnTree;
  nuiTreeNode*  mpSelected;
  nuiTreeNode*  mpOldSelected;
  nuiSize       mHandleWidth;
  nuiSize       mHandleHeight;
  nuiSize       mHandleOffset;

  bool mBoxDisplayed;

  nuiColor mMenuText[3];
  bool mHasCustomMenuTextFg;

  nuiEventSink<nuiComboBox> mComboBoxEvents;
};


#endif//__nuiComboBox_h__

