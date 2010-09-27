/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once


#include "nui.h"

#define DEFAULT_NUISCROLLBOX_SPACING nuiSize(5)

class nuiBox;

// This class implements a scrollable hbox or vbox with 2 buttons to scroll forward or backward.

//     What it looks like:
//
//          ______________________________________________________  _ _ _ _ _  ____________________________________
//          |                      |                                                         |                     |
//          | ScrollBackwardButton |     ScrollView containing the Widget (Hbox or VBox)     | ScrollForwardButton |
//          |                      |                                                         |                     |
//          ______________________________________________________  _ _ _ _ _  ____________________________________
//                            
//          ^                      ^                                                         ^                     ^
//                            
//     SCROLLBOX_SPACING      SCROLLBOX_SPACING                                   SCROLLBOX_SPACING        SCROLLBOX_SPACING

class nuiScrollBox : public nuiSimpleContainer
{
public:
  nuiScrollBox(nuiWidget* pBackward, nuiWidget* pForward, nuiOrientation Orientation, nuiWidget* pScrolledWidget);
  nuiScrollBox(const nglPath& rBackwardIconPath, const nglPath& rForwardIconPath, nuiOrientation Orientation, nuiWidget* pScrolledWidget);
  virtual ~nuiScrollBox();
  
  nuiOrientation GetOrientation() const;
  nuiBox* GetMainBox();
  nuiButton* GetBackwardButton();
  nuiButton* GetForwardButton();
  nuiScrollView* GetScrollView();
  nuiWidget* GetScrolledWidget();
  
  // Spacing / Sizing
  nuiSize GetBackwardButtonSpacing() const;
  nuiSize GetScrolledWidgetSpacing() const;
  nuiSize GetForwardButtonSpacing() const;
  
//  nuiSize GetBackwardButtonSize() const;
//  nuiSize GetForwardButtonSize() const;
  
  void SetBackwardButtonSpacing(nuiSize spacing);
  void SetScrolledWidgetSpacing(nuiSize spacing);
  void SetForwardButtonSpacing(nuiSize spacing);
  void SetSpacing(nuiSize spacing);
  
  // Hide or Show scrollbuttons when disabled
  void SetShowDisabledButtons(bool set);
  bool GetShowDisabledButtons() const;
  
  // Scrolling Management
  void OnScrollBackward(const nuiEvent& rEvent);
  void OnScrollForward(const nuiEvent& rEvent);
  void OnScrollViewChanged(const nuiEvent& rEvent);
  
protected:
  bool mShowDisabledButtons;
  nuiOrientation mOrientation;
  nuiBox* mpMainBox;
  nuiButton* mpBackwardButton;
  nuiButton* mpForwardButton;
  nuiScrollView* mpScrollView;
  nuiWidget* mpScrolledWidget;
  
  nuiEventSink<nuiScrollBox> mSink;
};
