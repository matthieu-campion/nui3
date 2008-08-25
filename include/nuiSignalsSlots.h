#ifndef SIGNALS_SLOTS_H
#define SIGNALS_SLOTS_H

#include <list>
#include <map>
#include "nuiFastDelegate.h"

class nuiNonCopyable
{
protected:
  nuiNonCopyable()
  {
  }
  ~nuiNonCopyable()
  {
  }
private:
	nuiNonCopyable( const nuiNonCopyable& );
	const nuiNonCopyable& operator=( const nuiNonCopyable& );
};

class nuiSlotsSink;

class nuiSignal
{
  friend class nuiSlotsSink;
public:
  virtual ~nuiSignal()
  {
  }
protected:
  nuiSignal()
  {
  }

  virtual void DisconnectInternal(const nuiFastDelegate::DelegateMemento &slot) = 0;

  void RemoveConnection(nuiSlotsSink &rSink, const nuiFastDelegate::DelegateMemento &rSlot);
  void AddConnection(nuiSlotsSink &rSink, const nuiFastDelegate::DelegateMemento &rSlot);
};

// N=0
template<class RetType = nuiFastDelegate::detail::DefaultVoid>
class nuiSignal0 : public nuiSignal, private nuiNonCopyable 
{
  friend class nuiSlotsSink;
public:
  typedef typename nuiFastDelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
  typedef typename nuiFastDelegate::FastDelegate0<DesiredRetType> Slot;

  nuiSignal0()
  {
  }

  ~nuiSignal0()
  {
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    for(; it != end; ++it)
    {
      RemoveConnection(*(*it).second, (*it).first.GetMemento());
    }
  }

  void operator()() const
  {
    //#FIXME: temp hack to avoid modification of the map (connection/deconnection of slots) during iteration
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    std::vector<Slot> tmp(mSlots.size());
    for(uint i = 0; it != end; ++it, i++)
      tmp[i] = (*it).first;
    
    for(uint i = 0; i < tmp.size(); i++)
      tmp[i]();
  }

  bool IsConnected ()
  {
    return (!mSlots.empty());
  }

private:
  void Connect(nuiSlotsSink &rSink, const Slot &rSlot)
  {
    mSlots.insert(std::make_pair<Slot, nuiSlotsSink*>(rSlot, &rSink));
    AddConnection(rSink, rSlot.GetMemento());
  }

  void Disconnect(nuiSlotsSink &rSink, const Slot &rSlot)
  {
    RemoveConnection(rSink, rSlot.GetMemento());
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(rSlot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

  virtual void DisconnectInternal(const nuiFastDelegate::DelegateMemento &rSlot_)
  {
    Slot slot;
    slot.SetMemento(rSlot_);
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

private:
  std::map<Slot, nuiSlotsSink*> mSlots;
};

// N=1
template<typename Param1, typename RetType=nuiFastDelegate::detail::DefaultVoid>
class nuiSignal1 : public nuiSignal, nuiNonCopyable  
{
  friend class nuiSlotsSink;
public:
  typedef typename nuiFastDelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
  typedef typename nuiFastDelegate::FastDelegate1<Param1, DesiredRetType> Slot;

  nuiSignal1()
  {
  }

  ~nuiSignal1()
  {
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    for(; it != end; ++it)
    {
      RemoveConnection(*(*it).second, (*it).first.GetMemento());
    }
  }

  void operator()(Param1 param1) const
  {
    //#FIXME: temp hack to avoid modification of the map (connection/deconnection of slots) during iteration
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    std::vector<Slot> tmp(mSlots.size());
    for(uint i = 0; it != end; ++it, i++)
      tmp[i] = (*it).first;
    
    for(uint i = 0; i < tmp.size(); i++)
      tmp[i](param1);
  }

  bool IsConnected ()
  {
    return (!mSlots.empty());
  }

private:	
  void Connect(nuiSlotsSink &sink, const Slot &slot)
  {
    mSlots.insert(std::make_pair<Slot, nuiSlotsSink*>(slot, &sink));
    AddConnection(sink, slot.GetMemento());
  }

  void Disconnect(nuiSlotsSink &sink, const Slot &slot)
  {
    RemoveConnection(sink, slot.GetMemento());
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

  virtual void DisconnectInternal(const nuiFastDelegate::DelegateMemento &slot_)
  {
    Slot slot;
    slot.SetMemento(slot_);
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

private:
  std::map<Slot, nuiSlotsSink*> mSlots;
};

// N=2
template<class Param1, class Param2, class RetType=nuiFastDelegate::detail::DefaultVoid>
class nuiSignal2 : public nuiSignal, nuiNonCopyable 
{
  friend class nuiSlotsSink;
public:
  typedef typename nuiFastDelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
  typedef typename nuiFastDelegate::FastDelegate2<Param1, Param2, DesiredRetType> Slot;

  nuiSignal2()
  {
  }

  ~nuiSignal2()
  {
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    for(; it != end; ++it)
    {
      RemoveConnection(*(*it).second, (*it).first.GetMemento());
    }
  }

  void operator()(Param1 param1, Param2 param2) const
  {
    //#FIXME: temp hack to avoid modification of the map (connection/deconnection of slots) during iteration
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    std::vector<Slot> tmp(mSlots.size());
    for(uint i = 0; it != end; ++it, i++)
      tmp[i] = (*it).first;
    
    for(uint i = 0; i < tmp.size(); i++)
      tmp[i](param1, param2);
  }


  bool IsConnected ()
  {
    return (!mSlots.empty());
  }

private:	
  void Connect(nuiSlotsSink &sink, const Slot &slot)
  {
    mSlots.insert(std::make_pair<Slot, nuiSlotsSink*>(slot, &sink));
    AddConnection(sink, slot.GetMemento());
  }

  void Disconnect(nuiSlotsSink &sink, const Slot &slot)
  {
    RemoveConnection(sink, slot.GetMemento());
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

  virtual void DisconnectInternal(const nuiFastDelegate::DelegateMemento &slot_)
  {
    Slot slot;
    slot.SetMemento(slot_);
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

private:
  std::map<Slot, nuiSlotsSink*> mSlots;
};

// N=3
template<class Param1, class Param2, class Param3, class RetType=nuiFastDelegate::detail::DefaultVoid>
class nuiSignal3 : public nuiSignal, nuiNonCopyable  
{
  friend class nuiSlotsSink;
public:
  typedef typename nuiFastDelegate::detail::DefaultVoidToVoid<RetType>::type DesiredRetType;
  typedef typename nuiFastDelegate::FastDelegate3<Param1, Param2, Param3, DesiredRetType> Slot;

  nuiSignal3()
  {
  }

  ~nuiSignal3()
  {
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    for(; it != end; ++it)
    {
      RemoveConnection(*(*it).second, (*it).first.GetMemento());
    }
  }

  void operator()(Param1 param1, Param2 param2, Param3 param3) const
  {
    //#FIXME: temp hack to avoid modification of the map (connection/deconnection of slots) during iteration
    typename std::map<Slot, nuiSlotsSink*>::const_iterator it = mSlots.begin();
    typename std::map<Slot, nuiSlotsSink*>::const_iterator end = mSlots.end();
    std::vector<Slot> tmp(mSlots.size());
    for(uint i = 0; it != end; ++it, i++)
      tmp[i] = (*it).first;
    
    for(uint i = 0; i < tmp.size(); i++)
      tmp[i](param1, param2, param3);
  }

  bool IsConnected ()
  {
    return (!mSlots.empty());
  }

private:
  void Connect(nuiSlotsSink &sink, const Slot &slot)
  {
    mSlots.insert(std::make_pair<Slot, nuiSlotsSink*>(slot, &sink));
    AddConnection(sink, slot.GetMemento());
  }

  void Disconnect(nuiSlotsSink &sink, const Slot &slot)
  {
    RemoveConnection(sink, slot.GetMemento());
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

  virtual void DisconnectInternal(const nuiFastDelegate::DelegateMemento &slot_)
  {
    Slot slot;
    slot.SetMemento(slot_);
    typename std::map<Slot, nuiSlotsSink*>::iterator it = mSlots.find(slot);
    if (it != mSlots.end())
      mSlots.erase(it);
  }

private:
  std::map<Slot, nuiSlotsSink*> mSlots;
};


class nuiSlotsSink
{
  friend class nuiSignal;
public:
  nuiSlotsSink();
  ~nuiSlotsSink();

  void DisconnectAll();
  
  // N=0
  template<class RetType> void Connect(nuiSignal0<RetType> &signal, const typename nuiSignal0<RetType>::Slot &slot)
  {
    signal.Connect(*this, slot);
  }
  template<class RetType> void Disconnect(nuiSignal0<RetType> &signal, const typename nuiSignal0<RetType>::Slot &slot)
  {
    signal.Disconnect(*this, slot);
  }

  // N=1
  template<class Param1, class RetType> void Connect(nuiSignal1<Param1, RetType> &signal, const typename nuiSignal1<Param1, RetType>::Slot &slot)
  {
    signal.Connect(*this, slot);
  }
  template<class Param1, class RetType> void Disconnect(nuiSignal1<Param1, RetType> &signal, const typename nuiSignal1<Param1, RetType>::Slot &slot)
  {
    signal.Disconnect(*this, slot);
  }

  // N=2
  template<class Param1, class Param2, class RetType> void Connect(nuiSignal2<Param1, Param2, RetType> &signal, const typename nuiSignal2<Param1, Param2, RetType>::Slot &slot)
  {
    signal.Connect(*this, slot);
  }
  template<class Param1, class Param2, class RetType> void Disconnect(nuiSignal2<Param1, Param2, RetType> &signal, const typename nuiSignal2<Param1, Param2, RetType>::Slot &slot)
  {
    signal.Disconnect(*this, slot);
  }

  // N=3
  template<class Param1, class Param2, class Param3, class RetType> void Connect(nuiSignal3<Param1, Param2, Param3, RetType> &signal, const typename nuiSignal3<Param1, Param2, Param3, RetType>::Slot &slot)
  {
    signal.Connect(*this, slot);
  }
  template<class Param1, class Param2, class Param3, class RetType> void Disconnect(nuiSignal3<Param1, Param2, Param3, RetType> &signal, const typename nuiSignal3<Param1, Param2, Param3, RetType>::Slot &slot)
  {
    signal.Disconnect(*this, slot);
  }

private:
  void AddConnection(nuiSignal* pSignal, const nuiFastDelegate::DelegateMemento &slot);
  void RemoveConnection(nuiSignal* pSignal, const nuiFastDelegate::DelegateMemento &slot);

private:
  std::map<nuiSignal*, nuiFastDelegate::DelegateMemento> mConnections;
};


#endif // SIGNALS_SLOTS_H
