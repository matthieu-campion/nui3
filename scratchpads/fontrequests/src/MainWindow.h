/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiMainWindow.h"
#include "nuiComboBox.h"
#include "nuiScrollView.h"

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
  
  std::vector<nuiComboBox*> mpCombos;
  
  nuiScrollView* mpFontScroll;
  
  nuiPanose mPanose;
};

