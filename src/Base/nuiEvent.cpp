/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nui.h"
#include "nuiApplication.h"
#include "nuiEvent.h"

using namespace std;

//class nuiEvent
nuiEvent::nuiEvent(int Type)
{ 
  mpSource = NULL; 
  mType = Type; 
  mpUser = NULL;
}

nuiEvent::~nuiEvent() 
{
}

int nuiEvent::GetType() const
{
  return mType;
}

void nuiEvent::SetSource(nuiEventSource* pSource) const
{
  mpSource = pSource;
}

const nuiEventSource* nuiEvent::GetSource() const
{
  return mpSource;
}

// class nuiEventSource
nuiEventSource::nuiEventSource()
{
  mEnumerating = false;
  mEnabled = true;
}

nuiEventSource::~nuiEventSource()
{
  std::vector<nuiEventTargetBase*>::iterator it;
  std::vector<nuiEventTargetBase*>::iterator end = mpTargets.end();

  mEnumerating = true;
  // Only add every target once, they will manage multiple event connection by them selves.
  for (it = mpTargets.begin(); it!=end; ++it) 
  {
    NGL_ASSERT(!(*it)->IsEnumerating());
    (*it)->Disconnect(*this);
  }
  mEnumerating = false;
  EmptyTrash();
}

void nuiEventSource::Connect(nuiEventTargetBase* t)
{
  std::vector<nuiEventTargetBase*>::iterator it;
  std::vector<nuiEventTargetBase*>::iterator end = mpTargets.end();

  // Only add every target once, they will manage multiple event connection by them selves.
  for (it = mpTargets.begin(); it!=end; ++it) 
  {
    if ((*it) == t)
      return;
  }
  mpTargets.insert(mpTargets.begin(), t);
}

void nuiEventSource::Disconnect(nuiEventTargetBase* t)
{
  //  OUT("Disconnecting 0x%x from 0x%x\n",t,this);
  if (!mEnumerating)
  {
    for (uint32 i = 0; i < mpTargets.size(); i++)
    {
      if (mpTargets[i] == t)
      {
        mpTargets.erase(mpTargets.begin() + i);
        return;
      }
    }    
  }
  else
    mpTrash.push_back(t);
}

bool nuiEventSource::SendEvent(const nuiEvent& rEvent)
{
  rEvent.SetSource(this);
  if (IsEnabled())
  {
    mEnumerating = true;
    std::vector<nuiEventTargetBase*>::const_iterator it;
    std::vector<nuiEventTargetBase*>::const_iterator end;
    it = mpTargets.begin();
    end=mpTargets.end();

    bool handled = false;
    while (it!=end && !handled)
    {
      handled =((*it)->OnEvent(rEvent));
      ++it;
    }

    mEnumerating = false;
    EmptyTrash();

    return handled;
  }

  return false;
}

bool nuiEventSource::operator() (const nuiEvent& rEvent)
{
  return SendEvent(rEvent);
}

uint nuiEventSource::GetTargetCount() const
{
  return (uint)mpTargets.size();
}

void nuiEventSource::EmptyTrash()
{
  if (!mEnumerating)
  {
    std::vector<nuiEventTargetBase*>::iterator it;
    std::vector<nuiEventTargetBase*>::iterator end;
    it=mpTrash.begin();
    end=mpTrash.end();
    while (it!=end)
    {
      Disconnect(*it);
      ++it;
    }
    mpTrash.clear();
  }
}

nuiEventSource::nuiEventSource(const nuiEventSource& rSource) 
{
}




//*
////////////////////////////////////////:
// TESTS:

#if 0
#define EVENT_BUTTONDOWN 0
#define EVENT_BUTTONUP 1

class myEvent : public nuiEvent
{
  myEvent(nuiEventSource* obj = NULL,int Type = -1) 
    : nuiEvent(obj,Type)
  { 
  }
};

class FakeButton
{
public:
  FakeButton()
  {
  }

  void TestButton()
  {
    EventButtonDown();
    EventButtonUp();
  }

  nuiSimpleEventSource<EVENT_BUTTONDOWN> EventButtonDown;
  nuiSimpleEventSource<EVENT_BUTTONUP> EventButtonUp;
};

class FakeTarget : public nuiEventTarget<FakeTarget>
{
public:
  FakeTarget(FakeButton* pButton)
  {
    Connect(pButton->EventButtonDown, &FakeTarget::ButtonDown);
    Connect(pButton->EventButtonUp, &FakeTarget::ButtonUp);
  }

  void ButtonDown(const nuiEvent& rEvent)
  {
    OUT("Received button down event!\n");
  }

  void ButtonUp(const nuiEvent& rEvent)
  {
    OUT("Received button up event!\n");
  }

  void ButtonDown2(const nuiEvent& rEvent)
  {
    OUT("Received button down event 2!!!\n");
  }

  void ButtonUp2(const nuiEvent& rEvent)
  {
    OUT("Received button up event 2!!!!\n");
  }

};

class FakeTarget2 : public nuiEventTarget<FakeTarget2>
{
public:
  FakeTarget2(FakeButton* pButton)
  {
    Connect(pButton->EventButtonDown, &FakeTarget2::ButtonDown);
    Connect(pButton->EventButtonUp, &FakeTarget2::ButtonUp);
  }

  void ButtonDown(const nuiEvent& rEvent)
  {
    OUT("FakeTarget2 Received button down event!\n");
  }

  void ButtonUp(const nuiEvent& rEvent)
  {
    OUT("FakeTarget2 Received button up event!\n");
  }

  void ButtonDown2(const nuiEvent& rEvent)
  {
    OUT("FakeTarget2 Received button down event 2!!!\n");
  }

  void ButtonUp2(const nuiEvent& rEvent)
  {
    OUT("FakeTarget2 Received button up event 2!!!!\n");
  }

};

void nuiTestEvents()
{
  FakeButton mybutton;
  {
    FakeTarget mytarget(&mybutton);

    mybutton.TestButton();

    mytarget.Connect(mybutton.EventButtonDown, &FakeTarget::ButtonDown2);
    mytarget.Connect(mybutton.EventButtonUp, &FakeTarget::ButtonUp2);

    mytarget.Disconnect(mybutton.EventButtonDown, &FakeTarget::ButtonDown);
    mytarget.Disconnect(mybutton.EventButtonUp, &FakeTarget::ButtonUp);

    mybutton.TestButton();
  }

  mybutton.TestButton();

}
//*/

#endif
