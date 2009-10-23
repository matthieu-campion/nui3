/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "Gui/Toolbar.h"
#include "nuiButton.h"
#include "nuiToggleButton.h"


Toolbar::Toolbar()
: nuiHBox(0), mEventSink(this)
{
  SetObjectClass(_T("Toolbar"));
  SetExpand(nuiExpandShrinkAndGrow);
}


Toolbar::~Toolbar()
{
  
}

void Toolbar::AddButton(const nglString& rObjectClass)
{
  nuiButton* pButton = new nuiButton(rObjectClass);
  nuiHBox::AddCell(pButton);
  
  uint32 ID = mButtons.size();
  mButtons.push_back(std::make_pair(rObjectClass, pButton));
  
  mEventSink.Connect(pButton->Activated, &Toolbar::OnActivated, (void*)ID);
}


void Toolbar::AddToggleButton(const nglString& rObjectClass, bool initialPressed)
{
  nuiToggleButton* pButton = new nuiToggleButton(rObjectClass);
  nuiHBox::AddCell(pButton);
  
  uint32 ID = mButtons.size();
  mButtons.push_back(std::make_pair(rObjectClass, pButton));
  
  if (initialPressed)
    pButton->SetPressed(true);
  
  mEventSink.Connect(pButton->ButtonPressed, &Toolbar::OnActivated, (void*)ID);
  mEventSink.Connect(pButton->ButtonDePressed, &Toolbar::OnActivated, (void*)ID);
}

void Toolbar::Clear()
{
  mButtons.clear();
  mButtons.resize(0);
  nuiHBox::Clear(true);
}





const nglString& Toolbar::GetActivatedButton() const
{
  return mActivatedButton;
}



bool Toolbar::OnActivated(const nuiEvent& rEvent)
{
  // get button ID
  uint32 ID = (uint32)rEvent.mpUser;
  mActivatedButton = mButtons[ID].first;
  
  // report event to client
  Activated();
  return true;
}
