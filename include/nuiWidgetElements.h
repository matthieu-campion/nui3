/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiWidgetElements_h__
#define __nuiWidgetElements_h__

enum nuiWidgetElement
{
  eActiveWindowShade=0,
  eInactiveWindowShade,

  eActiveWindowBg,
  eInactiveWindowBg,

  eActiveWindowFg,
  eInactiveWindowFg,

  eActiveWindowTitle,
  eInactiveWindowTitle,

  eScrollBarFg,
  eScrollBarFgHover,
  eScrollBarBg,
  eScrollBarBgHover,

  eSplitterBarFg,
  eSplitterBarBg,
  eSplitterBarHover,

  eSliderBarFg,
  eSliderBarBg,
  eSliderBarBgHover,

  eNormalButtonBg,
  eSelectedButtonBg,
  eDisabledButtonBg,
  eHoverNormalButtonBg,
  eHoverSelectedButtonBg,
  eHoverDisabledButtonBg,

  eKnobBg,
  eKnobMarker,

  eNormalTabBg,
  eSelectedTabBg,
  eDisabledTabBg,
  eHoverNormalTabBg,
  eHoverSelectedTabBg,
  eHoverDisabledTabBg,

  eTableColumnBg,
  eSelectedTableColumnBg,
  eSelectedTableLineBg,

  eTreeViewHandle,
  eTreeViewSelection,

  eSelectedActiveBackground,
  eSelectedInactiveBackground,
  
  eNormalTextFg,
  eSelectedTextFg,
  eDisabledTextFg,
  eNormalTextBg,
  eSelectedTextBg,
  eDisabledTextBg,

  eSelectionMarkee,

  eProgressBg,
  eProgressFg,
  eProgressBorder,

  eMenuBg,
  eMenuFg,
  eMenuTitleBg,

  eShapeFill,
  eShapeStroke,
  eBorder,

  eDisabledImage,
  eSelectedImage,

  eComboBoxBg,
  eComboBoxLining,
  
  eToolTipBg,
  eToolTipFg,
  eToolTipBorder,
  
  eMatrixViewHeaderBorder,
  eMatrixViewHeaderBackground,
  eMatrixViewBorder,
  eMatrixViewBackground,
  
  
  

  eLastWidgetElement
};

#endif // ifdef __nuiWidgetElements_h__
