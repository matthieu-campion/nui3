/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#pragma once

#include "nui.h"


class nuiWidget;
class nuiMainWindow;

typedef nuiFastDelegate::FastDelegate1<nuiWidget*> nuiDecorationDelegate;


class nuiDefaultDecoration
{
public:
  
  nuiDefaultDecoration();
  ~nuiDefaultDecoration();
  
  static void Init();
  static void InitColors();
  static void InitMaps();

  static void Window(nuiWidget* pWidget);

  static void Label(nuiWidget* pWidget);
  static void EditText(nuiWidget* pWidget);
  static void EditLine(nuiWidget* pWidget);

  static void SplitterHandle(nuiWidget* pWidget);

  static void TitledPane(nuiWidget* pWidget);
  static void FolderPane(nuiWidget* pWidget);

  static void Button(nuiWidget* pWidget);
  static void CloseButton(nuiWidget* pWidget);
  static void ToggleButton(nuiWidget* pWidget);

  // special case
  static void MainWindow(nuiMainWindow* pWindow);

};