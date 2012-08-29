/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nui.h"


class nuiFlowView : public nuiSimpleContainer
{
public:
  nuiFlowView(float IdealWidth = 320, float HSpace = 0.0f, float VSpace = 2.0f);
  ~nuiFlowView();
  
  virtual nuiRect CalcIdealSize();
  virtual bool SetRect(const nuiRect& rRect);
  virtual bool Draw(nuiDrawContext* pContext);
  
  void SetIdealWidth(float IdealWidth);
  float GetIdealWidth() const;
  float GetVSpace() const;
  float GetHSpace() const;
  void SetVSpace(float interline);
  void SetHSpace(float InterWidget);
  
protected:
  nuiRect Layout(bool setLayout, float IdealWidth);
  void LayoutLine(nuiWidgetList& line, float& x, float &y, float& w, float& h, float& HSpace, float &VSpace, bool setLayout);
  float mIdealWidth;
  float mVSpace;
  float mHSpace;
};
