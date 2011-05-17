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
  mCancel = false;
}

nuiEvent::nuiEvent(const nuiEvent& rEvent)
{ 
  mpSource = rEvent.mpSource; 
  mType = rEvent.mType; 
  mpUser = rEvent.mpUser;
  mArguments = rEvent.mArguments;
  mCancel = rEvent.mCancel;
}

void nuiEvent::Cancel() const
{
  mCancel = true;
}

bool nuiEvent::IsCanceled() const
{
  return mCancel;
}

nuiEvent::~nuiEvent() 
{
  for (uint32 i = 0; i < mArguments.size(); i++)
    delete mArguments[i];
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

uint32 nuiEvent::GetArgCount() const
{
  return mArguments.size();
}

void nuiEvent::AddArgument(const nuiVariant& rArg)
{
  mArguments.push_back(new nuiVariant(rArg));
}

const nuiVariant& nuiEvent::operator[](uint32 index) const
{
  return *(mArguments[index]);
}


// class nuiEventSource
nuiEventSource::nuiEventSource()
{
  mEnabled = true;
}

nuiEventSource::~nuiEventSource()
{
  DisconnectAll();
}

void nuiEventSource::DisconnectAll()
{
  // Only add every target once, they will manage multiple event connection by them selves.
  while (!mpTargets.empty()) 
  {
    nuiEventTargetBase* pETB = mpTargets[mpTargets.size() - 1];
    pETB->DisconnectSource(*this);
  }
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
  for (uint32 i = 0; i < mpTargets.size(); i++)
  {
    if (mpTargets[i] == t)
    {
      mpTargets.erase(mpTargets.begin() + i);
      return;
    }
  }    
}

bool nuiEventSource::SendEvent(const nuiEvent& rEvent)
{
  if (IsEnabled() && !mpTargets.empty())
  {
    rEvent.SetSource(this);
    std::vector<nuiEventTargetBase*> targets(mpTargets);
    std::vector<nuiEventTargetBase*>::const_iterator it = targets.begin();
    std::vector<nuiEventTargetBase*>::const_iterator end = targets.end();

    bool handled = false;
    while (it != end && !handled)
    {
      ((*it)->OnEvent(rEvent));
      handled = rEvent.IsCanceled();
      ++it;
    }

    return handled;
  }

  return false;
}

bool nuiEventSource::operator() (const nuiEvent& rEvent)
{
  return SendEvent(rEvent);
}

bool nuiEventSource::operator() (const nuiVariant& rP0)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  return SendEvent(e);
}

bool nuiEventSource::operator() (const nuiVariant& rP0, const nuiVariant& rP1)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  e.AddArgument(rP1);
  return SendEvent(e);
}

bool nuiEventSource::operator() (const nuiVariant& rP0, const nuiVariant& rP1, const nuiVariant& rP2)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  e.AddArgument(rP1);
  e.AddArgument(rP2);
  return SendEvent(e);
}

bool nuiEventSource::operator() (const nuiVariant& rP0, const nuiVariant& rP1, const nuiVariant& rP2, const nuiVariant& rP3)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  e.AddArgument(rP1);
  e.AddArgument(rP2);
  e.AddArgument(rP3);
  return SendEvent(e);
}

bool nuiEventSource::operator() (const nuiVariant& rP0, const nuiVariant& rP1, const nuiVariant& rP2, const nuiVariant& rP3, const nuiVariant& rP4)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  e.AddArgument(rP1);
  e.AddArgument(rP2);
  e.AddArgument(rP3);
  e.AddArgument(rP4);
  return SendEvent(e);
}

bool nuiEventSource::operator() (const nuiVariant& rP0, const nuiVariant& rP1, const nuiVariant& rP2, const nuiVariant& rP3, const nuiVariant& rP4, const nuiVariant& rP5)
{
  nuiEvent e(0);
  e.AddArgument(rP0);
  e.AddArgument(rP1);
  e.AddArgument(rP2);
  e.AddArgument(rP3);
  e.AddArgument(rP4);
  e.AddArgument(rP5);
  return SendEvent(e);
}


uint nuiEventSource::GetTargetCount() const
{
  return (uint)mpTargets.size();
}

nuiEventSource::nuiEventSource(const nuiEventSource& rSource) 
{
}

////////////////////////////////////////////////////
//class NUI_API nuiEventTargetBase
nuiEventTargetBase::nuiEventTargetBase(void* pTarget)
{
  mpTarget = pTarget;
}

nuiEventTargetBase::~nuiEventTargetBase()
{
  DisconnectAll();
}

void nuiEventTargetBase::DisconnectAll()
{
  LinksMap links(mpLinks);
  LinksMap::iterator it;
  LinksMap::iterator end = links.end();
  for (it = links.begin(); it != end; ++it)
  {
    DisconnectSource(*(it->first));
  }
}

bool nuiEventTargetBase::OnEvent(const nuiEvent& rEvent)
{
  bool handled = false;
  
  nuiEventSource* pSource = (nuiEventSource*)rEvent.GetSource();
  LinksMap::const_iterator it_source = mpLinks.find(pSource);
  
  if (it_source != mpLinks.end())
  {
    LinkList links(it_source->second);
    LinkList::const_iterator it   = links.begin();
    LinkList::const_iterator end  = links.end();
    
    for (; (it != end) && !handled; ++it)
    {
      Link* pLink = (*it);
      nuiDelegateMemento pFunc = pLink->mTargetFunc;
      rEvent.mpUser = pLink->mpUser;
      
      handled = CallEvent((void*)mpTarget, pFunc, rEvent);
    }
  }
  return handled;
}

bool nuiEventTargetBase::CallEvent(void* pTarget, nuiDelegateMemento pFunc, const nuiEvent& rEvent)
{
  Delegate del;
  del.SetMemento(pFunc);
  del(rEvent);
  return rEvent.IsCanceled();
}

void nuiEventTargetBase::Connect(nuiEventSource& rSource, const nuiDelegateMemento& rTargetFunc, void* pUser)
{
  Link* pLink = new Link;
  pLink->mTargetFunc = rTargetFunc;
  pLink->mpUser = pUser;
  
  rSource.Connect(this);
  
  LinkList& rLinkList(mpLinks[&rSource]);
  rLinkList.insert(rLinkList.begin(), pLink);
}

void nuiEventTargetBase::DisconnectSource(nuiEventSource& rSource)
{
  LinksMap::iterator it_source = mpLinks.find(&rSource);
  if (it_source != mpLinks.end())
  {
    const LinkList& rLinks = it_source->second;
    
    LinkList::const_iterator end = rLinks.end();
    for (LinkList::const_iterator it = rLinks.begin(); it != end; ++it)
      delete *it;
    
    mpLinks.erase(it_source);
  }
  rSource.Disconnect(this);
}

void nuiEventTargetBase::Disconnect(const nuiDelegateMemento& rTFunc)
{
  std::vector<nuiEventSource*> toErase;
  LinksMap::const_iterator end_source = mpLinks.end();
  for (LinksMap::iterator it_source = mpLinks.begin(); it_source != end_source; ++it_source)
  {
    LinkList& rLinks = it_source->second;
    
    LinkList::iterator end = rLinks.end();
    LinkList::iterator it = rLinks.begin();
    while (it != end)
    {
      Link* pLink = *it;
      if (pLink->mTargetFunc.IsEqual(rTFunc))
      {
        delete pLink;
        it = rLinks.erase(it);
      }
      else
        ++it;
    }
    if (rLinks.empty())
      toErase.push_back(it_source->first);
  }
  
  for (uint32 i = 0; i < toErase.size(); i++)
  {
    nuiEventSource* pSource = toErase[i];
    pSource->Disconnect(this);
    mpLinks.erase(pSource);
  }
}

void nuiEventTargetBase::Disconnect(nuiEventSource& rSource, const nuiDelegateMemento& rTFunc)
{
  LinksMap::iterator it_source = mpLinks.find(&rSource);
  if (it_source != mpLinks.end())
  {
    LinkList& rLinks(it_source->second);
    
    LinkList::const_iterator end = rLinks.end();
    for (LinkList::iterator it = rLinks.begin(); it != end;)
    {
      Link* pLink = *it;
      if (pLink->mTargetFunc.IsEqual(rTFunc))
      {
        delete pLink;
        it = rLinks.erase(it);
        break; // assumes no one will connect the same target func to the same source?
      }
      else
        ++it;
    }
    
    if (rLinks.empty())
    {
      mpLinks.erase(it_source);
      rSource.Disconnect(this);
    }
  }
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
