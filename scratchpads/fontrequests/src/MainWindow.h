/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiComboBox.h"
#include "nuiScrollView.h"

class nuiVBox;
class nuiEditLine;
class nuiToggleButton;

class MainWindow : public nuiMainWindow
{
public:
  MainWindow(const nglContextInfo& rContext, const nglWindowInfo& rInfo, bool ShowFPS = false, const nglContext* pShared = NULL);
  ~MainWindow();

  void OnCreation();
  void OnClose();
  
  
private:

  bool OnRequestChanged(const nuiEvent& rEvent);
  void UpdateFontList(uint8 bytes[10]);

  nuiEventSink<MainWindow> mEventSink;

  nuiVBox* mpRequestBox;
  
  nuiComboBox* mpPanoseFamily;
  nuiComboBox* mpPanoseSerif;
  nuiComboBox* mpPanoseWeight;
  nuiComboBox* mpPanoseProportion;
  nuiComboBox* mpPanoseContrast;
  nuiComboBox* mpPanoseStrokeVariation;
  nuiComboBox* mpPanoseArmStyle;
  nuiComboBox* mpPanoseLetterForm;
  nuiComboBox* mpPanoseMidLine;
  nuiComboBox* mpPanoseXHeight;
  
  nuiEditLine* mpName;
  nuiEditLine* mpGenericName;
  nuiEditLine* mpStyle;
  nuiToggleButton* mpItalic;
  nuiToggleButton* mpItalicSet;
  nuiToggleButton* mpBold;
  nuiToggleButton* mpBoldSet;
  nuiToggleButton* mpMonospace;
  nuiToggleButton* mpMonospaceSet;
  nuiToggleButton* mpScalable;
  nuiToggleButton* mpScalableSet;
  nuiEditLine* mpMustHaveGlyphs;
  
  std::vector<nuiComboBox*> mpCombos;
  
  nuiScrollView* mpFontScroll;
  
  nuiPanose mPanose;
  
  nuiEditLine* AddEditLine(const nglString& rName);
  nuiToggleButton* AddCheckBox(const nglString& rName, bool pressed, nuiToggleButton*& pWidget, nuiToggleButton*& pWidgetSet);
  void AddNamedBox(const nglString& rName, nuiWidget* pEditor);
  
};

