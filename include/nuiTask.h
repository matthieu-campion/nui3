/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiFastDelegate.h"

class nuiTaskBase
{
public:
  nuiTaskBase();
  virtual ~nuiTaskBase();
  virtual void Run() const = 0;
};

template <class Ret>
class nuiTask0 : public nuiTaskBase
{
public:
  typedef nuiFastDelegate0<Ret> Delegate;
  nuiTask0(const Delegate& rDelegate)
  : mDelegate(rDelegate)
  {    
  }
  
  virtual void Run() const
  {
    mDelegate();
  }
  
private:
  Delegate mDelegate;
};

template <class P0, class Ret>
class nuiTask1 : public nuiTaskBase
{
public:
  typedef nuiFastDelegate1<P0, Ret> Delegate;
  nuiTask1(const Delegate& rDelegate, const P0& rP0)
  : mDelegate(rDelegate), mP0(rP0)
  {    
  }
  
  virtual void Run() const
  {
    mDelegate(mP0);
  }
  
private:
  Delegate mDelegate;
  P0 mP0;
};

template <class P0, class P1, class Ret>
class nuiTask2 : public nuiTaskBase
{
public:
  typedef nuiFastDelegate2<P0, P1, Ret> Delegate;
  nuiTask2(const Delegate& rDelegate, const P0& rP0, const P1& rP1)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1)
  {    
  }
  
  virtual void Run() const
  {
    mDelegate(mP0, mP1);
  }
  
private:
  Delegate mDelegate;
  P0 mP0;
  P1 mP1;
};



// Task creation shortcurts:
template <class Param1, class RetType>
nuiTask1<Param1, RetType>* nuiMakeTask(RetType (*func)(Param1 p1), Param1 P1)
{ 
	return new nuiTask1<Param1, RetType>(func, P1);
}

template <class X, class Y, class Param1, class RetType>
nuiTask1<Param1, RetType>* nuiMakeTask(Y* x, RetType (X::*func)(Param1 p1), Param1 P1)
{ 
	return new nuiTask1<Param1, RetType>(nuiMakeDelegate(x, func), P1);
}

template <class X, class Y, class Param1, class RetType>
nuiTask1<Param1, RetType>* nuiMakeTask(Y* x, RetType (X::*func)(Param1 p1) const, Param1 P1)
{ 
	return new nuiTask1<Param1, RetType>(nuiMakeDelegate(x, func), P1);
}

