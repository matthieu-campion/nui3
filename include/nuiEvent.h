/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiEvent_h__
#define __nuiEvent_h__

//#include "nui.h"
#include <list>

class nuiEventSource;

/// This is the basic event object used in the user level event management system. 
class NUI_API nuiEvent
{
public:
  nuiEvent(int Type = -1);  
  virtual ~nuiEvent();

  int GetType() const;
  const nuiEventSource* GetSource() const;
  void SetSource(nuiEventSource* pObj) const;

  mutable void* mpUser; // This is the user defined data. It is defined on a connection basis.
private:
  int mType;
  mutable nuiEventSource* mpSource;
};

/// You are never supposed to use this class directly!!! NEVER EVER :). 
class NUI_API nuiEventTargetBase
{
public:
  virtual ~nuiEventTargetBase() {}
  virtual bool OnEvent(const nuiEvent& rEvent) = 0;
  virtual void Disconnect(nuiEventSource& rSource) = 0;
};

/// Do not touch this either!
class NUI_API nuiEventSource
{
public:  
  nuiEventSource();
  virtual ~nuiEventSource();

  virtual void Connect(nuiEventTargetBase* t);
  virtual void Disconnect(nuiEventTargetBase* t);
  virtual bool SendEvent(const nuiEvent& rEvent = nuiEvent(0));
  virtual bool operator() (const nuiEvent& rEvent = nuiEvent(0));
  uint GetTargetCount() const;

  void Enable(bool Set = true)
  {
    mEnabled = Set;
  }

  void Disable()
  {
    Enable(false);
  }

  bool IsEnabled()
  {
    return mEnabled;
  }

protected:
  mutable std::vector<nuiEventTargetBase*> mpTargets;

private:
  // Restrict access to some methods & constructors:
  nuiEventSource(const nuiEventSource& rSource) ;
  bool mEnabled;
};


template <class T> class NUI_API nuiEventTarget : public nuiEventTargetBase
{
public:
  typedef bool (T::*TargetFunc) (const nuiEvent& rEvent);

  nuiEventTarget (T* pTarget)
    : nuiEventTargetBase()
  {
    mpTarget = pTarget;
  }

  virtual ~nuiEventTarget()
  {
    DisconnectAll();
  }

  virtual void DisconnectAll()
  {
    LinksMap links(mpLinks);
		typename LinksMap::iterator it;
		typename LinksMap::iterator end = links.end();
    for (it = links.begin(); it != end; ++it)
    {
      Disconnect(*(it->first));
    }
  }

  virtual bool OnEvent(const nuiEvent& rEvent)
  {
    // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!
    NGL_ASSERT(mpTarget);
    
    bool handled = false;

    nuiEventSource* pSource = (nuiEventSource*)rEvent.GetSource();
    typename LinksMap::const_iterator it_source = mpLinks.find(pSource);

    if (it_source != mpLinks.end())
    {
      LinkList links(it_source->second);
      typename LinkList::const_iterator it   = links.begin();
      typename LinkList::const_iterator end  = links.end();

      for (; (it != end) && !handled; ++it)
      {
        Link* pLink = (*it);
        TargetFunc pFunc = pLink->mpTargetFunc;
        rEvent.mpUser = pLink->mpUser;
        // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!
        handled = (((T*)mpTarget)->*pFunc)( rEvent );
      }
    }
    return handled;
  }

  virtual void Connect(nuiEventSource& rSource, TargetFunc pTargetFunc, void* pUser=NULL)
  {
    NGL_ASSERT(mpTarget);
    // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!
    Link* pLink = new Link;
    pLink->mpTargetFunc = pTargetFunc;
    pLink->mpUser = pUser;

    rSource.Connect(this);

    LinkList& rLinkList(mpLinks[&rSource]);
    rLinkList.insert(rLinkList.begin(), pLink);
  }

  virtual void Disconnect(nuiEventSource& rSource)
  {
    NGL_ASSERT(mpTarget);
    // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!
    typename LinksMap::iterator it_source = mpLinks.find(&rSource);
    if (it_source != mpLinks.end())
    {
      const LinkList& rLinks = it_source->second;
      
      typename LinkList::const_iterator end = rLinks.end();
      for (typename LinkList::const_iterator it = rLinks.begin(); it != end; ++it)
        delete *it;

      mpLinks.erase(it_source);
    }
    rSource.Disconnect(this);
  }

  virtual void Disconnect(TargetFunc pTargetFunc)
  {
    NGL_ASSERT(mpTarget);
    // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!

    std::vector<nuiEventSource*> toErase;
    typename LinksMap::const_iterator end_source = mpLinks.end();
    for (typename LinksMap::iterator it_source = mpLinks.begin(); it_source != end_source; ++it_source)
    {
      LinkList& rLinks = it_source->second;

      typename LinkList::iterator end = rLinks.end();
      typename LinkList::iterator it = rLinks.begin();
      while (it != end)
      {
        Link* pLink = *it;
        if (pLink->mpTargetFunc == pTargetFunc)
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

  virtual void Disconnect(nuiEventSource& rSource, TargetFunc pTargetFunc)
  {
    NGL_ASSERT(mpTarget);
    // Beware!! If the debugger brings you here its probably that you forgot to call nuiEventSink<...>::SetTarget in some of your constructor code!!!

    typename LinksMap::iterator it_source = mpLinks.find(&rSource);
    if (it_source != mpLinks.end())
    {
      LinkList& rLinks(it_source->second);
      
      typename LinkList::const_iterator end = rLinks.end();
      for (typename LinkList::iterator it = rLinks.begin(); it != end;)
      {
        Link* pLink = *it;
        if (pLink->mpTargetFunc == pTargetFunc)
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

protected:
  
  // Target handling:
  void SetTarget(T* pTarget)
  {
    mpTarget = pTarget;
  }
  T* GetTarget()
  {
    return mpTarget;
  }
  
private:
  class NUI_API Link
  {
  public:
    TargetFunc  mpTargetFunc;
    void*       mpUser;
  };

  typedef std::vector<Link*> LinkList;
  typedef std::map<nuiEventSource*, LinkList> LinksMap;
  LinksMap mpLinks;

  T* mpTarget;
  
  nuiEventTarget(const nuiEventTarget<T>& rTarget)
  {
    // There is NO way to copy a target properly. 
  }
};

/// This template is the basis of the event reception system of nui.
template <class T> class NUI_API nuiEventSink : public nuiEventTarget<T>
{
public:
  nuiEventSink(T* pTarget)
  : nuiEventTarget<T>(pTarget)
  {
    SetTarget(pTarget);
  }
  virtual ~nuiEventSink()
  {
  }

  void SetTarget(T* pTarget)
  {
    nuiEventTarget<T>::SetTarget(pTarget);
  }
private:
  nuiEventSink(const nuiEventSink& rSink)
  {
    // No way to use the copy constructor. 
  }
};

/// This is a helper class that implements a simple event source with a fixed type.
template <int eventtype> class NUI_API nuiSimpleEventSource : public nuiEventSource
{
public:
  nuiSimpleEventSource()
  {
  }

  virtual ~nuiSimpleEventSource()
  {
  }

  virtual bool operator() ()
  {
    if (IsEnabled())
    {
      nuiEvent e(eventtype); 
      return SendEvent(e);
    }

    return false;
  }

  virtual bool operator() (const nuiEvent& rEvent)
  {
    if (IsEnabled())
    {
      return SendEvent(rEvent);
    }
    return false;
  }

private:
  nuiSimpleEventSource(const nuiSimpleEventSource& rSource)
  {
    // No way to implement this correctly. 
  }
};

#endif // __nuiEvent_h__
