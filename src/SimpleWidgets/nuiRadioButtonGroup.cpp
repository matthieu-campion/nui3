/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/


#include "nui.h"
#include "nuiRadioButtonGroup.h"
#include "nuiRadioButton.h"


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
  mSink.Connect(pRadioButton->ButtonPressed, &nuiRadioButtonGroup::OnEvent, pRadioButton);
  mSink.Connect(pRadioButton->ButtonDePressed, &nuiRadioButtonGroup::OnEvent, pRadioButton);
}

void nuiRadioButtonGroup::RemoveRadioButton(nuiRadioButton *pRadioButton)
{
  mRadioButtons.erase(pRadioButton);
  pRadioButton->SetGroupManager(NULL);
  mSink.Disconnect(pRadioButton->ButtonPressed);
  mSink.Disconnect(pRadioButton->ButtonDePressed);
}

bool nuiRadioButtonGroup::OnEvent(const nuiEvent& rEvent)
{
  nuiRadioButton *pRadioButton = (nuiRadioButton*)rEvent.mpUser;

  if (pRadioButton->IsPressed())
  {
    std::set<nuiRadioButton*>::iterator it = mRadioButtons.begin();
    std::set<nuiRadioButton*>::iterator end = mRadioButtons.end();
    for (;it != end; ++it)
    {
      if ( (*it) != pRadioButton)
      {
        (*it)->SetPressed(false);
        //#FIXME : the following line has been added to fix a missing behavior. still has to be validated
        (*it)->Deactivated();
      }
    }
  }

  return false;
}

