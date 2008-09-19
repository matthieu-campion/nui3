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
  static void InitSelection();
  static void InitMaps();
  static void InitIcons();
  

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


  // file selector
  static void FileSelector_FolderView(nuiWidget* pWidget);
  static void FileSelector_InfoView(nuiWidget* pWidget);
  static void FileSelector_FolderLabel(nuiWidget* pWidget);
  static void FileSelector_VolumeIcon(nuiWidget* pWidget);
  static void FileSelector_FolderIcon(nuiWidget* pWidget);
  static void FileSelector_TreeView(nuiWidget* pWidget);
  static void FileSelector_TreeFolderLabel(nuiWidget* pWidget);
  static void FileSelector_TreeFileLabel(nuiWidget* pWidget);
  static void FileSelector_TreeFolderIcon(nuiWidget* pWidget);
  static void FileSelector_TreeFileIcon(nuiWidget* pWidget);
  
  // special case
  static void MainWindow(nuiMainWindow* pWindow);

};