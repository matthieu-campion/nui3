/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */



#include "nuiTask.h"

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



void TestSink(int a)
{
  printf("TestSink: %d\n", a);
}

void TestTask()
{
  nuiTaskBase* pTask = nuiMakeTask(TestSink, 42);
  pTask->Run();
}
