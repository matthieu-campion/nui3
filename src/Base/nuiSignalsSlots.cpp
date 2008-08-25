#include "nui.h"
#include "nuiSignalsSlots.h"

void nuiSignal::RemoveConnection(nuiSlotsSink &sink, const nuiFastDelegate::DelegateMemento &slot)
{
  sink.RemoveConnection(this, slot);
}

void nuiSignal::AddConnection(nuiSlotsSink &sink, const nuiFastDelegate::DelegateMemento &slot)
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
  std::map<nuiSignal*, nuiFastDelegate::DelegateMemento>::const_iterator it = mConnections.begin();
  std::map<nuiSignal*, nuiFastDelegate::DelegateMemento>::const_iterator end = mConnections.end();
  for(; it!= end; ++it)
  {
    (*it).first->DisconnectInternal((*it).second);
  }
  mConnections.clear();
}

void nuiSlotsSink::AddConnection(nuiSignal* pSignal, const nuiFastDelegate::DelegateMemento &slot) 
{
  mConnections.insert(std::make_pair<nuiSignal*, nuiFastDelegate::DelegateMemento>(pSignal, slot));
}

void nuiSlotsSink::RemoveConnection(nuiSignal* pSignal, const nuiFastDelegate::DelegateMemento &slot) 
{	
  std::map<nuiSignal*, nuiFastDelegate::DelegateMemento>::iterator it = mConnections.find(pSignal);
  if (it != mConnections.end())
    mConnections.erase(it);
}
