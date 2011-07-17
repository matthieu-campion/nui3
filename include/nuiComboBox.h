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
  virtual ~nuiComboBox();
  
  
  nuiSimpleEventSource<0> ValueChanged;

  nuiWidget*    GetSelectedWidget();
  nuiTreeNode*  GetSelected();
  const nuiTreeNode* GetChoices() const;
  uint32        GetValue();
  void          SetSelected(nuiTreeNode* pSelection);
  void          SetSelected(const nuiTreeNode* pSelection);
  void          SetSelected(uint32 childIndex);
  void          SetValue(uint32 childIndex);

  void SetTree(nuiTreeNode* pTree);
  
  nuiSimpleEventSource<nuiWidgetSelected> SelectionChanged; ///< This event is called whenever selection of the items of the list have changed. 

protected:

  virtual nuiRect CalcIdealSize();
  virtual bool    Draw(nuiDrawContext* pContext);
  virtual bool    SetRect(const nuiRect& rRect);

  virtual bool    MouseClicked(nuiSize X, nuiSize Y, nglMouseInfo::Flags Button);

  virtual bool    KeyDown(const nglKeyEvent& rEvent);
  virtual bool    KeyUp(const nglKeyEvent& rEvent);
  
  void OnSelect(const nuiEvent& rEvent);

  void ReparentTree(nuiTreeNode* pTree);
  void UnparentTree(nuiTreeNode* pTree);
  nuiTreeNode*  mpChoicesTree;
  bool          mOwnTree;
  nuiTreeNode*  mpSelected;
  nuiTreeNode*  mpOldSelected;
  nuiSize       mHandleWidth;
  nuiSize       mHandleHeight;
  nuiSize       mHandleOffset;

  nuiEventSink<nuiComboBox> mComboBoxEvents;
};


#endif//__nuiComboBox_h__

