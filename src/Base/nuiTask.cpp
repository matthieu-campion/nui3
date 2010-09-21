/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot
 
 licence: see nui3/LICENCE.TXT
 */



#include "nuiTask.h"



void TestSink(int a)
{
  printf("TestSink: %d\n", a);
}

void TestTask()
{
  nuiTaskBase* pTask = nuiMakeTask(TestSink, 42);
  pTask->Run();
}
