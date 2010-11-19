/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */

#pragma once

#include "nuiFastDelegate.h"
#include "nuiRefCount.h"

class nuiTask : public nuiRefCount
{
public:
  nuiTask()
  : mCanceled(false)
  {
  }
  
  virtual ~nuiTask()
  {
  }
  
  void Cancel()
  {
    mCanceled = true;
  }
  
  bool IsCanceled() const
  {
    return mCanceled;
  }
  
  void Run() const
  {
    if (mCanceled)
      return;
    Execute();
  }
  
protected:
  virtual void Execute() const = 0;
  bool mCanceled;
};

// No Param:
template <class Ret>
class nuiTask0 : public nuiTask
{
public:
  typedef nuiFastDelegate0<Ret> Delegate;
  nuiTask0(const Delegate& rDelegate)
  : mDelegate(rDelegate)
  {    
  }
  
  
protected:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate();
  }
};

#define DEFPARAM(N,TYPE) \
  public:\
    TYPE GetParam##N() const  { return mP##N; }\
    void SetParam##N(TYPE P)  { mP##N = P; }\
  protected:\
    TYPE mP##N;

// 1 Param
template <class P0, class Ret>
class nuiTask1 : public nuiTask
{
public:
  typedef nuiFastDelegate1<P0, Ret> Delegate;
  nuiTask1(const Delegate& rDelegate, const P0& rP0)
  : mDelegate(rDelegate), mP0(rP0)
  {    
  }
  
  DEFPARAM(0, P0);
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0);
  }
};

// 2 Params
template <class P0, class P1, class Ret>
class nuiTask2 : public nuiTask
{
public:
  typedef nuiFastDelegate2<P0, P1, Ret> Delegate;
  nuiTask2(const Delegate& rDelegate, const P0& rP0, const P1& rP1)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);

private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1);
  }
};

// 3 Params
template <class P0, class P1, class P2, class Ret>
class nuiTask3 : public nuiTask
{
public:
  typedef nuiFastDelegate3<P0, P1, P2, Ret> Delegate;
  nuiTask3(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2);
  }
};

// 4 Params
template <class P0, class P1, class P2, class P3, class Ret>
class nuiTask4 : public nuiTask
{
public:
  typedef nuiFastDelegate4<P0, P1, P2, P3, Ret> Delegate;
  nuiTask4(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2), mP3(rP3)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  DEFPARAM(3, P3);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2, mP3);
  }
};


// 5 Params
template <class P0, class P1, class P2, class P3, class P4, class Ret>
class nuiTask5 : public nuiTask
{
public:
  typedef nuiFastDelegate5<P0, P1, P2, P3, P4, Ret> Delegate;
  nuiTask5(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2), mP3(rP3), mP4(rP4)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  DEFPARAM(3, P3);
  DEFPARAM(4, P4);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2, mP3, mP4);
  }
};



// 6 Params
template <class P0, class P1, class P2, class P3, class P4, class P5, class Ret>
class nuiTask6 : public nuiTask
{
public:
  typedef nuiFastDelegate6<P0, P1, P2, P3, P4, P5, Ret> Delegate;
  nuiTask6(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2), mP3(rP3), mP4(rP4), mP5(rP5)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  DEFPARAM(3, P3);
  DEFPARAM(4, P4);
  DEFPARAM(5, P5);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2, mP3, mP4, mP5);
  }
};



// 7 Params
template <class P0, class P1, class P2, class P3, class P4, class P5, class P6, class Ret>
class nuiTask7 : public nuiTask
{
public:
  typedef nuiFastDelegate7<P0, P1, P2, P3, P4, P5, P6, Ret> Delegate;
  nuiTask7(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5, const P6& rP6)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2), mP3(rP3), mP4(rP4), mP5(rP5), mP6(rP6)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  DEFPARAM(3, P3);
  DEFPARAM(4, P4);
  DEFPARAM(5, P5);
  DEFPARAM(6, P6);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2, mP3, mP4, mP5, mP6);
  }
};



// 8 Params
template <class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class Ret>
class nuiTask8 : public nuiTask
{
public:
  typedef nuiFastDelegate8<P0, P1, P2, P3, P4, P5, P6, P7, Ret> Delegate;
  nuiTask8(const Delegate& rDelegate, const P0& rP0, const P1& rP1, const P2& rP2, const P3& rP3, const P4& rP4, const P5& rP5, const P6& rP6, const P7& rP7)
  : mDelegate(rDelegate), mP0(rP0), mP1(rP1), mP2(rP2), mP3(rP3), mP4(rP4), mP5(rP5), mP6(rP6), mP7(rP7)
  {    
  }
  
  DEFPARAM(0, P0);
  DEFPARAM(1, P1);
  DEFPARAM(2, P2);
  DEFPARAM(3, P3);
  DEFPARAM(4, P4);
  DEFPARAM(5, P5);
  DEFPARAM(6, P6);
  DEFPARAM(7, P7);
  
private:
  Delegate mDelegate;
  virtual void Execute() const
  {
    mDelegate(mP0, mP1, mP2, mP3, mP4, mP5, mP6, mP7);
  }
};





// Task creation shortcurts:

// No Param
template <class RetType>
nuiTask0<RetType>* nuiMakeTask(RetType (*func)())
{ 
	return new nuiTask0<RetType>(func);
}

template <class X, class Y, class RetType>
nuiTask0<RetType>* nuiMakeTask(Y* x, RetType (X::*func)())
{ 
	return new nuiTask0<RetType>(nuiMakeDelegate(x, func));
}

template <class X, class Y, class RetType>
nuiTask0<RetType>* nuiMakeTask(Y* x, RetType (X::*func)() const)
{ 
	return new nuiTask0<RetType>(nuiMakeDelegate(x, func));
}


// 1 Param
template <class Param0, class RetType>
nuiTask1<Param0, RetType>* nuiMakeTask(RetType (*func)(Param0 p0), Param0 P0)
{ 
	return new nuiTask1<Param0, RetType>(func, P0);
}

template <class X, class Y, class Param0, class RetType>
nuiTask1<Param0, RetType>* nuiMakeTask(Y* x, RetType (X::*func)(Param0 p0), Param0 P0)
{ 
	return new nuiTask1<Param0, RetType>(nuiMakeDelegate(x, func), P0);
}

template <class X, class Y, class Param0, class RetType>
nuiTask1<Param0, RetType>* nuiMakeTask(Y* x, RetType (X::*func)(Param0 p0) const, Param0 P0)
{ 
	return new nuiTask1<Param0, RetType>(nuiMakeDelegate(x, func), P0);
}



// 2 Params
template <class Param0, class Param1, class RetType>
nuiTask2<Param0, Param1, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1),
                                               Param0 P0, Param1 P1)
{ 
	return new nuiTask1<Param1, RetType>(func, P0, P1);
}

template <class X, class Y, class Param0, class Param1, class RetType>
nuiTask2<Param0, Param1, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1),
                                               Param0 P0, Param1 P1)
{ 
	return new nuiTask2<Param0, Param1, RetType>(nuiMakeDelegate(x, func), P0, P1);
}

template <class X, class Y, class Param0, class Param1, class RetType>
nuiTask2<Param0, Param1, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1) const,
                                               Param0 P0, Param1 P1)
{ 
	return new nuiTask2<Param0, Param1, RetType>(nuiMakeDelegate(x, func), P0, P1);
}


// 3 Params
template <class Param0, class Param1, class Param2, class RetType>
nuiTask3<Param0, Param1, Param2, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2),
                                                       Param0 P0, Param1 P1, Param2 P2)
{ 
	return new nuiTask3<Param0, Param1, Param2, RetType>(func, P0, P1, P2);
}

template <class X, class Y, class Param0, class Param1, class Param2, class RetType>
nuiTask3<Param0, Param1, Param2, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2),
                                                       Param0 P0, Param1 P1, Param2 P2)
{ 
	return new nuiTask3<Param0, Param1, Param2, RetType>(nuiMakeDelegate(x, func), P0, P1, P2);
}

template <class X, class Y, class Param0, class Param1, class Param2, class RetType>
nuiTask3<Param0, Param1, Param2, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2) const,
                                                       Param0 P0, Param1 P1, Param2 P2)
{ 
	return new nuiTask3<Param0, Param1, Param2, RetType>(nuiMakeDelegate(x, func), P0, P1, P2);
}

// 4 Params
template <class Param0, class Param1, class Param2, class Param3, class RetType>
nuiTask4<Param0, Param1, Param2, Param3, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3)
{ 
	return new nuiTask4<Param0, Param1, Param2, Param3, RetType>(func, P0, P1, P2, P3);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class RetType>
nuiTask4<Param0, Param1, Param2, Param3, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3)
{ 
	return new nuiTask4<Param0, Param1, Param2, Param3, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class RetType>
nuiTask4<Param0, Param1, Param2, Param3, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3) const,
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3)
{ 
	return new nuiTask4<Param0, Param1, Param2, Param3, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3);
}

// 5 Params
template <class Param0, class Param1, class Param2, class Param3, class Param4, class RetType>
nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4)
{ 
	return new nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>(func, P0, P1, P2, P3, P4);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class RetType>
nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4)
{ 
	return new nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class RetType>
nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4) const,
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4)
{ 
	return new nuiTask5<Param0, Param1, Param2, Param3, Param4, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4);
}

// 6 Params
template <class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5)
{ 
	return new nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>(func, P0, P1, P2, P3, P4, P5);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5)
{ 
	return new nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const,
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5)
{ 
	return new nuiTask6<Param0, Param1, Param2, Param3, Param4, Param5, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5);
}

// 7 Params
template <class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class RetType>
nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6)
{ 
	return new nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>(func, P0, P1, P2, P3, P4, P5, P6);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class RetType>
nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6)
{ 
	return new nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5, P6);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class RetType>
nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6) const,
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6)
{ 
	return new nuiTask7<Param0, Param1, Param2, Param3, Param4, Param5, Param6, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5, P6);
}


// 8 Params
template <class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class RetType>
nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>* nuiMakeTask(RetType (*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6, Param7 P7)
{ 
	return new nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>(func, P0, P1, P2, P3, P4, P5, P6, P7);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class RetType>
nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7),
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6, Param7 P7)
{ 
	return new nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5, P6, P7);
}

template <class X, class Y, class Param0, class Param1, class Param2, class Param3, class Param4, class Param5, class Param6, class Param7, class RetType>
nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>* nuiMakeTask(Y*x, RetType (X::*func)(Param0 p0, Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5, Param6 p6, Param7 p7) const,
                                       Param0 P0, Param1 P1, Param2 P2, Param3 P3, Param4 P4, Param5 P5, Param6 P6, Param7 P7)
{ 
	return new nuiTask8<Param0, Param1, Param2, Param3, Param4, Param5, Param6, Param7, RetType>(nuiMakeDelegate(x, func), P0, P1, P2, P3, P4, P5, P6, P7);
}



#undef DEFPARAM
