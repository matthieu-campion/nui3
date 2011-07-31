/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiEditLine_h__
#define __nuiEditLine_h__

#include "nuiEditText.h"

/// This is a simple widget for editing one line of text. Pressing enter fires the "Selected" event.
class NUI_API nuiEditLine : public nuiEditText
{
public:
  nuiEditLine(const nglString& rText = _T(""));
  virtual ~nuiEditLine();
  
  void LimitCharacterCount(int maxDisplayedChar);
  int GetCharacterCountLimit();
  
  virtual nuiRect CalcIdealSize();
  
  virtual bool TextInput(const nglString& rUnicodeString);
  virtual bool KeyDown(const nglKeyEvent& rEvent);
  virtual bool KeyUp  (const nglKeyEvent& rEvent);
  
  nuiSimpleEventSource<nuiActionAborted> Aborted;
  nuiSimpleEventSource<nuiWidgetActivated> Activated;
  nuiSimpleEventSource<0> HistoryUp;
  nuiSimpleEventSource<0> HistoryDown;
  nuiSimpleEventSource<0> AutoComplete;
  
protected:
    int mMaxDisplayedChar;
  
private:
  
  void InitAttributes();
  
};

#endif // __nuiEditLine_h__
