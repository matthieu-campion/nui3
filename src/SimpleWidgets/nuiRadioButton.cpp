/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "ngl.h"
#include "nuiRadioButton.h"
#include "nuiLabel.h"
#include "nuiDrawContext.h"
#include "nuiXML.h"
#include "nuiRadioButtonGroup.h"

nuiRadioButton::nuiRadioButton()
  : nuiToggleButton()
{
  SetObjectClass(_T("nuiRadioButton"));
  SetGroup(_T("Default"));
  mpGroupManager = NULL;
  mCanToggle = false;
}

nuiRadioButton::nuiRadioButton(const nglString& rText)
  : nuiToggleButton(rText)
{
  SetObjectClass(_T("nuiRadioButton"));
  SetGroup(_T("Default"));
  mpGroupManager = NULL;
  mCanToggle = false;
}

nuiRadioButton::nuiRadioButton(nuiStateDecoration* pDecoration)
  : nuiToggleButton(pDecoration)
{
  SetObjectClass(_T("nuiRadioButton"));
  SetGroup(_T("Default"));
  mpGroupManager = NULL;
  mCanToggle = false;
}

nuiRadioButton::nuiRadioButton(const nglImage& rImage)
  : nuiToggleButton(rImage)
{
  SetObjectClass(_T("nuiRadioButton"));
  SetGroup(_T("Default"));
  mpGroupManager = NULL;
  mCanToggle = false;
}

bool nuiRadioButton::Load(const nuiXMLNode* pNode)
{
  nuiToggleButton::Load(pNode);
  SetObjectClass(_T("nuiRadioButton"));
  SetGroup(nuiGetString(pNode, _T("Group"), _T("Default")));
  mpGroupManager = NULL;
  mCanToggle = false;
  
  return true;
}

nuiRadioButton::~nuiRadioButton()
{
  if (mpGroupManager)
    mpGroupManager->RemoveRadioButton(this);
}

bool nuiRadioButton::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if ( (Button & nglMouseInfo::ButtonLeft) && mClicked)
  {
    mClicked = false;
    Ungrab();
    if (mRect.Size().IsInside(X,Y))
    {
      if (mCanToggle)
        SetPressed(!IsPressed());
      else
        SetPressed(true);

      if (IsPressed())
        Activated();
      else
        Deactivated();
    }
    else
    {
      SetSelected(IsPressed());
    }

    Invalidate();
    return true;
  }
  return false;
}


void nuiRadioButton::SetPressed(bool Pressed)
{
  if (mPressed == Pressed)
    return;

  mPressed = Pressed;
  if (Pressed)
  {
    if (mpParent && !mpGroupManager)
    {
      nuiContainer::IteratorPtr pIt = mpParent->GetFirstChild();
      while (pIt->IsValid())
      {
        nuiRadioButton* pRadio = dynamic_cast<nuiRadioButton*>(pIt->GetWidget());
        if (pRadio && (pRadio != this) && pRadio->GetGroup() == GetGroup())
          pRadio->SetPressed(false);

        mpParent->GetNextChild(pIt);
      }
      delete pIt;
    }

    SetSelected(true);
    ButtonPressed();
  }
  else
  {    
    SetSelected(false);
    ButtonDePressed();
  }

  Invalidate();
}

void nuiRadioButton::SetGroup(const nglString& rGroupName)
{
  SetProperty(_T("Group"), rGroupName);
}

void nuiRadioButton::SetGroupManager(nuiRadioButtonGroup* pGroup)
{
  mpGroupManager = pGroup;
}

nuiRadioButtonGroup* nuiRadioButton::GetGroupManager()
{
  return mpGroupManager;
}

const nglString& nuiRadioButton::GetGroup()
{
  return GetProperty(_T("Group"));
}


bool nuiRadioButton::SetParent(nuiContainerPtr pParent)
{
  bool res = nuiToggleButton::SetParent(pParent);

  if (mpParent && mPressed)
  {
    nuiContainer::IteratorPtr pIt = mpParent->GetFirstChild();
    while (pIt->IsValid())
    {
      nuiRadioButton* pRadio = dynamic_cast<nuiRadioButton*>(pIt->GetWidget());
      if (pRadio && (pRadio != this) && pRadio->GetGroup() == GetGroup())
        pRadio->SetPressed(false);

      mpParent->GetNextChild(pIt);
    }
    delete pIt;
  }

  return res;
}

void nuiRadioButton::EnableToggleable(bool set)
{
  mCanToggle = set;
}

bool nuiRadioButton::IsToggleable() const
{
  return mCanToggle;
}


