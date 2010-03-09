/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglCondition.h"

class nglConditionPrivate
{
public:
  friend class nglCondition;
private:
  HANDLE condition;
};

/**
* Create a condition 
* Conditions are usefull to synchronise threads
*/
nglCondition::nglCondition()
{
  mpData = new nglConditionPrivate;
  mpData->condition = CreateEvent(NULL, FALSE, FALSE, NULL);
}

/**
* Destroy a condition and wake all the threads waitng for this condition
*/
nglCondition::~nglCondition()
{
  WakeAll();
  CloseHandle(mpData->condition);  
  delete mpData;
}


void nglCondition::WakeOne()
{
  PulseEvent(mpData->condition);
}

void nglCondition::WakeAll()
{
  SetEvent(mpData->condition);
  ResetEvent(mpData->condition);
}

/**
* Wait until the condition is set or the timeout is reached
* @param time Time in milliseconds to wait before the function exit if the condition is not set (use ULONG_MAX for an infinite wait (by default))
* @return true is the condition was set otherwise false
*/
bool nglCondition::Wait(uint32 time)
{
  DWORD res;
  if (time == ULONG_MAX)
  {
    res = WaitForSingleObject(mpData->condition, INFINITE);
  }
  else
  {
    res = WaitForSingleObject(mpData->condition, time);
  }  
  return (res == WAIT_OBJECT_0);
}


