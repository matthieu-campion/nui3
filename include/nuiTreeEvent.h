/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiTreeEvent_h__
#define __nuiTreeEvent_h__

#include "nuiEvent.h"

class nuiTreeBase;

template <class T> class NUI_API nuiTreeEvent : public nuiEvent
{
public:
  nuiTreeEvent(nuiEventId Id, T* pParent, T* pChild)
    : nuiEvent(Id)
  {
    mpParent = pParent;
    mpChild  = pChild;
  }

  T* mpParent;
  T* mpChild;
};

template <nuiEventId Id, class T> class NUI_API nuiTreeEventSource : public nuiEventSource
{
public:
  nuiTreeEventSource()
  {
  }

  virtual bool operator() (T* pParent, T* pChild)
  {
    return SendEvent(nuiTreeEvent<T>(Id, pParent, pChild));
  }
};

typedef nuiTreeEventSource<nuiChildAdded, nuiTreeBase> nuiChildAddedEventSource;
typedef nuiTreeEventSource<nuiChildDeleted, nuiTreeBase> nuiChildDeletedEventSource;

#endif
