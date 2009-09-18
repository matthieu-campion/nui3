/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#include "nui.h"
#include "nuiSignalsSlots.h"

void nuiSignal::RemoveConnection(nuiSlotsSink &sink, const nuiDelegateMemento &slot)
{
  sink.RemoveConnection(this, slot);
}

void nuiSignal::AddConnection(nuiSlotsSink &sink, const nuiDelegateMemento &slot)
{
  sink.AddConnection(this, slot);
}

/////

nuiSlotsSink::nuiSlotsSink()
{
}

nuiSlotsSink::~nuiSlotsSink() 
{
  DisconnectAll();
}

void nuiSlotsSink::DisconnectAll()
{
  std::map<nuiSignal*, nuiDelegateMemento>::const_iterator it = mConnections.begin();
  std::map<nuiSignal*, nuiDelegateMemento>::const_iterator end = mConnections.end();
  for(; it!= end; ++it)
  {
    (*it).first->DisconnectInternal((*it).second);
  }
  mConnections.clear();
}

void nuiSlotsSink::AddConnection(nuiSignal* pSignal, const nuiDelegateMemento &slot) 
{
  mConnections.insert(std::make_pair<nuiSignal*, nuiDelegateMemento>(pSignal, slot));
}

void nuiSlotsSink::RemoveConnection(nuiSignal* pSignal, const nuiDelegateMemento &slot) 
{	
  std::map<nuiSignal*, nuiDelegateMemento>::iterator it = mConnections.find(pSignal);
  if (it != mConnections.end())
    mConnections.erase(it);
}

