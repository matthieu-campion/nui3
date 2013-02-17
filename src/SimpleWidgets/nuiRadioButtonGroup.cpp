/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"


nuiRadioButtonGroup::nuiRadioButtonGroup()
: mSink(this)
{
  Init();
}

nuiRadioButtonGroup::nuiRadioButtonGroup(nuiXMLNode* pNode)
: mSink(this)
{
  Init();
}

nuiRadioButtonGroup::~nuiRadioButtonGroup()
{
  std::set<nuiRadioButton*>::iterator it = mRadioButtons.begin();
  std::set<nuiRadioButton*>::iterator end = mRadioButtons.end();
  for (;it != end; ++it)
  {
    (*it)->SetGroupManager(NULL);
  }
}

void nuiRadioButtonGroup::Init()
{
}

void nuiRadioButtonGroup::AddRadioButton(nuiRadioButton *pRadioButton)
{
  mRadioButtons.insert(pRadioButton);
  pRadioButton->SetGroupManager(this);
  mSink.Connect(pRadioButton->Activated, &nuiRadioButtonGroup::OnActivated, pRadioButton);
}

void nuiRadioButtonGroup::RemoveRadioButton(nuiRadioButton *pRadioButton)
{
  mRadioButtons.erase(pRadioButton);
  pRadioButton->SetGroupManager(NULL);
  mSink.DisconnectSource(pRadioButton->Activated);
}

void nuiRadioButtonGroup::OnActivated(const nuiEvent& rEvent)
{
  nuiRadioButton *pRadioButton = (nuiRadioButton*)rEvent.mpUser;
  NGL_ASSERT(pRadioButton->IsPressed());

  std::set<nuiRadioButton*>::iterator it = mRadioButtons.begin();
  std::set<nuiRadioButton*>::iterator end = mRadioButtons.end();
  for (;it != end; ++it)
  {
    if ( (*it) != pRadioButton)
    {
      bool wasPressed = (*it)->IsPressed();
      (*it)->SetPressed(false);
      if (wasPressed) {
        (*it)->Deactivated();
      }
    }
  }
}

