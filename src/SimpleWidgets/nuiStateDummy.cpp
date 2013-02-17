/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

nuiStateDummy::nuiStateDummy(nuiWidgetPtr pNormal, nuiWidgetPtr pSelected, nuiWidgetPtr pDisabled)
: nuiSimpleContainer()
{
  SetObjectClass(_T("nuiStateDummy"));

  mCombined = false;
  mpState[1 + 0] = pNormal;
  mpState[1 + 2] = pSelected;
  mpState[0 + 0] = pDisabled;
  mpState[0 + 2] = pDisabled;
  
  int i;
  for (i=0; i < 3; i++)
    if (mpState[i])
      AddChild(mpState[i]);
}

nuiStateDummy::~nuiStateDummy()
{
}

void nuiStateDummy::SetDummy(bool Selected, bool Enabled, nuiWidgetPtr pWidget)
{
  nuiWidgetPtr pOld = mpState[(Enabled?1:0) + (Selected?2:0)];
  mpState[(Enabled?1:0) + (Selected?2:0)] = pWidget;
  if (pWidget)
    AddChild(pWidget);
  if (pOld)
  {
    // Destroy the widget if it is not used anymore here:
    int i;
    for (i = 0; i < 3 && mpState[i] != pOld; i++) ;
    if (i == 3)
      pOld->Trash();
  }
}

nuiWidgetPtr nuiStateDummy::GetDummy(bool Selected, bool Enabled)
{
  return mpState[(Enabled?1:0) + (Selected?2:0)];
}

bool nuiStateDummy::Draw(nuiDrawContext* pContext)
{
  nuiWidgetPtr pWidget = mpState[(IsEnabled(mCombined)?1:0) + (IsSelected(mCombined)?2:0)];

  if (pWidget)
  {
    DrawChild(pContext, pWidget);
  }
  return true;
}

nuiRect nuiStateDummy::CalcIdealSize()
{
  nuiWidgetPtr pWidget = mpState[(IsEnabled(mCombined)?1:0) + (IsSelected(mCombined)?2:0)];

  if (pWidget)
  {
    mIdealRect = pWidget->GetIdealRect();
//    NGL_OUT(_T("Dummy: %s (%f,%f - %f,%f\n"), nuiGetState(GetState(mCombined)).GetChars(),ideal.mLeft,ideal.mTop,ideal.GetWidth(),ideal.GetHeight());
  }

  return mIdealRect;
}

bool nuiStateDummy::SetRect(const nuiRect& rRect)
{
  nuiWidget::SetRect(rRect);
  nuiWidgetPtr pWidget = mpState[(IsEnabled(mCombined)?1:0) + (IsSelected(mCombined)?2:0)];
  if (pWidget)
    pWidget->SetLayout(rRect.Size());
  return true;
}

void nuiStateDummy::SetEnabled(bool set)
{
  nuiSimpleContainer::SetEnabled(set);
  InvalidateLayout();
}

void nuiStateDummy::SetSelected(bool set)
{
  nuiSimpleContainer::SetSelected(set);
  InvalidateLayout();
}

void nuiStateDummy::SetCombined(bool combined)
{
  if (mCombined == combined)
    return;
  mCombined = combined;
  InvalidateLayout();
}

bool nuiStateDummy::GetCombined()
{
  return mCombined;
}
