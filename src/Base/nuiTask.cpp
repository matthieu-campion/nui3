/*
 *  nuiTask.cpp
 *  nui3
 *
 *  Created by Sébastien Métrot on 9/20/10.
 *  Copyright 2010 libNUI. All rights reserved.
 *
 */

#include "nuiTask.h"

void TestSink(int a)
{
  printf("TestSink: %d\n", a);
}

void TestTask()
{
  nuiTaskBase* pTask = new nuiTask1<int, void>(TestSink, 42);
  pTask->Run();
}
