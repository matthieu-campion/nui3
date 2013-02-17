/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

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

nuiRadioButton::~nuiRadioButton()
{
  if (mpGroupManager)
    mpGroupManager->RemoveRadioButton(this);
}

// Keyboard events:
bool nuiRadioButton::KeyDown(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
  {
    mWasPressed = IsPressed(); 
    if (mCanToggle)
      SetPressed(!mWasPressed);
    else
      SetPressed(true);

    if (IsPressed() && !mWasPressed)
    {
      if (mpParent && !mpGroupManager)
      {
        nuiContainer::IteratorPtr pIt = mpParent->GetFirstChild();
        while (pIt->IsValid())
        {
          nuiRadioButton* pRadio = dynamic_cast<nuiRadioButton*>(pIt->GetWidget());
          if (pRadio && (pRadio != this) && pRadio->GetGroup() == GetGroup())
          {
            bool wasPressed = pRadio->IsPressed();
            pRadio->SetPressed(false);
            if (wasPressed)
            {
              pRadio->Deactivated();
            }
          }
          mpParent->GetNextChild(pIt);
        }
        delete pIt;
      }
      Activated();
    }
    else if (!IsPressed() && mWasPressed)
    {
      Deactivated();
    }
    
    return true;
  }
  
  return false;
}

bool nuiRadioButton::KeyUp(const nglKeyEvent& rEvent)
{
  if (rEvent.mKey == NK_ENTER || rEvent.mKey == NK_PAD_ENTER || rEvent.mKey == NK_SPACE)
    return true;
  
  return false;
}


bool nuiRadioButton::MouseClicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if (IsDisabled())
    return false;

  if (Button & nglMouseInfo::ButtonLeft)
  {
    mClicked = true;
    mWasPressed = IsPressed();
    if (mCanToggle)
      SetPressed(!mWasPressed);
    else
      SetPressed(true);
    Grab();
    Invalidate();
    return true;
  }
  return false;
}            

bool nuiRadioButton::MouseUnclicked  (nuiSize X, nuiSize Y, nglMouseInfo::Flags Button)
{
  if ( (Button & nglMouseInfo::ButtonLeft) && mClicked)
  {
    mClicked = false;
    Ungrab();
    if (IsInsideFromSelf(X,Y, GetActivationOffset()))
    {
      if (mCanToggle)
        SetPressed(!mWasPressed);
      else
        SetPressed(true);

      if (IsPressed() && !mWasPressed)
      {
        if (mpParent && !mpGroupManager)
        {
          nuiContainer::IteratorPtr pIt = mpParent->GetFirstChild();
          while (pIt->IsValid())
          {
            nuiRadioButton* pRadio = dynamic_cast<nuiRadioButton*>(pIt->GetWidget());
            if (pRadio && (pRadio != this) && pRadio->GetGroup() == GetGroup())
            {
              bool wasPressed = pRadio->IsPressed();
              pRadio->SetPressed(false);
              if (wasPressed) {
                pRadio->Deactivated();
              }
            }
            mpParent->GetNextChild(pIt);
          }
          delete pIt;
        }
        Activated();
      }
      else if (!IsPressed() && mWasPressed) {
        Deactivated();
      }
    }
    else
    {
      SetPressed(mWasPressed);
    }

    Invalidate();
    return true;
  }
  return false;
}

bool nuiRadioButton::MouseMoved(nuiSize X, nuiSize Y)
{
  if (IsDisabled())
    return false;
  
  if (mClicked)
  {
    if (IsInsideFromSelf(X,Y, GetActivationOffset()))
    {
      if (mCanToggle)
        SetPressed(!mWasPressed);
      else
        SetPressed(true);
    }
    else
    {
      SetPressed(mWasPressed);
    }
    return true;
  }
  return false;
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


