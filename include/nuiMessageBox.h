/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once


#include "nuiModalContainer.h"
#include "nuiHBox.h"


/*
NOTE ABOUT CSS DECORATION: here are the object names that can be used in a css file to decorate the message box:
nuiMessageBox (object class)
"nuiMessageBox::Client"
"nuiMessageBox::Title"
"nuiMessageBox::Contents"
"nuiMessageBox::Icon"
"nuiMessageBox::Message" : constructor with const nglString& rMessage parameter
"nuiMessageBox::ButtonBox"
"nuiMessageBox::Button"


*/

enum nuiMessageBoxType
{
  eMB_None,
  eMB_OK,
  eMB_OKCancel,
  eMB_Cancel,
  eMB_YesNo,
  eMB_RetryOKCancel,
  eMB_Custom
};



class nuiMessageBox : public nuiModalContainer
{
public:
  
  typedef uint Button;
  
  
  static const Button ButtonOK;
  static const Button ButtonCancel;
  static const Button ButtonRetry;
  static const Button ButtonYes;
  static const Button ButtonNo;
  
  nuiMessageBox(nuiContainer * pParent, const nglString& rTitle, const nglString& rMessage, nuiMessageBoxType type, bool showIcon = true);
  nuiMessageBox(nuiContainer * pParent, const nglString& rTitle, nuiWidget* pContents, nuiMessageBoxType type, bool showIcon = true);
  virtual ~nuiMessageBox();
  


  Button QueryUser();
  
  void AddButton(const nglString& rName, Button btn);
  void AddButton(nuiWidget* pWidget, Button btn);
  
  // key events
  virtual bool KeyUp(const nglKeyEvent& rEvent);
  virtual bool KeyDown(const nglKeyEvent& rEvent);
  
  static Button Do(nuiContainer * pParent, const nglString& rTitle, const nglString& rMessage, nuiMessageBoxType type);
private:

  void Init(const nglString& rTitle, nuiWidget* pContents, nuiMessageBoxType type, bool showIcon);


  nuiMessageBoxType mType;
  
  nuiHBox* mpBtnBox;
  
  Button mClickedButton;
  
  nuiEventSink<nuiMessageBox> mSink;
  
  bool OnOK(const nuiEvent& rEvent);
  bool OnCancel(const nuiEvent& rEvent);
  bool OnRetry(const nuiEvent& rEvent);
  bool OnCustom(const nuiEvent& rEvent);
  
  nuiWidget* CreateOK();
  nuiWidget* CreateCancel();
  nuiWidget* CreateRetry();
  nuiWidget* CreateYes();
  nuiWidget* CreateNo();
  nuiWidget* CreateCustom(const nglString& rName, Button btn);
  nuiWidget* CreateCustom(nuiWidget* pWidget, Button btn);
  
  bool mKeyDown;
};

