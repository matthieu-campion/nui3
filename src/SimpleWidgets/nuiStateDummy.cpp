/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nuiStateDummy.h"
#include "nuiXML.h"

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

bool nuiStateDummy::Load(const nuiXMLNode* pNode)
{
  nuiSimpleContainer::Load(pNode);
  SetObjectClass(_T("nuiStateDummy"));

  mCombined = false;
  mpState[1 + 0]   = GetChild(pNode->GetAttribute(_T("NormalChild")),   false);
  mpState[1 + 2] = GetChild(pNode->GetAttribute(_T("SelectedChild")), false);
  mpState[0 + 0] = GetChild(pNode->GetAttribute(_T("DisabledChild")), false);
  mpState[0 + 2] = GetChild(pNode->GetAttribute(_T("DisabledSelectedChild")), false);

  return true;
}

nuiXMLNode* nuiStateDummy::Serialize(nuiXMLNode* pParentNode, bool Recursive) const
{
  nuiXMLNode* pNode = nuiSimpleContainer::Serialize(pParentNode, Recursive);

  if (mpState[1 + 0])
    pNode->SetAttribute(_T("NormalChild"),   mpState[1 + 0]->GetObjectName());
  if (mpState[1 + 2])
    pNode->SetAttribute(_T("SelectedChild"), mpState[1 + 2]->GetObjectName());
  if (mpState[0 + 0])
    pNode->SetAttribute(_T("DisabledChild"), mpState[0 + 0]->GetObjectName());
  if (mpState[0 + 2])
    pNode->SetAttribute(_T("DisabledSelectedChild"), mpState[0 + 2]->GetObjectName());

  return pNode;
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
//    NGL_OUT(_T("Dummy: %ls (%f,%f - %f,%f\n"), nuiGetState(GetState(mCombined)).GetChars(),ideal.mLeft,ideal.mTop,ideal.GetWidth(),ideal.GetHeight());
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
