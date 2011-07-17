/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiStateDummy_h__
#define __nuiStateDummy_h__

#include "nuiContainer.h"

class NUI_API nuiStateDummy : public nuiSimpleContainer
{
public:
  nuiStateDummy(nuiWidgetPtr pNormal = NULL, nuiWidgetPtr pSelected = NULL, nuiWidgetPtr pDisabled = NULL);
  virtual ~nuiStateDummy();

  void SetDummy(bool Selected, bool Enabled, nuiWidgetPtr pWidget); ///< Set the dummy widget to display when the widget is in a particular state. 
  nuiWidgetPtr GetDummy(bool Selected, bool Enabled);  ///< Get the dummy widget to display when the widget is in a particular state. 

  virtual bool Draw(nuiDrawContext* pContext);
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual void SetEnabled(bool set);
  virtual void SetSelected(bool set);

  virtual void SetCombined(bool combined); ///< If set to true the combined flag will force combination of the state of this object with the state of its parent.
  virtual bool GetCombined(); ///< If set to true the combined flag will force combination of the state of this object with the state of its parent.

protected:
  nuiWidgetPtr mpState[4];
  bool mCombined;
};

#endif // __nuiStateDummy_h__
