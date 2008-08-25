#include "nui.h"
#include "nglSyncEvent.h"

#include <windows.h>


class nglSyncEventPrivate 
{
  friend class nglSyncEvent;

private:

  HANDLE mEvent;

};

nglSyncEvent::nglSyncEvent()
{
  mpData = new nglSyncEventPrivate;
  mpData->mEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

nglSyncEvent::~nglSyncEvent()
{
  CloseHandle(mpData->mEvent);  
  delete mpData;
}


void nglSyncEvent::Set()
{
  SetEvent(mpData->mEvent);
}

void nglSyncEvent::Reset()
{
  ResetEvent(mpData->mEvent);
}

bool nglSyncEvent::Wait(uint32 time)
{
  DWORD res;
  if (time == ULONG_MAX) 
  {
    res = WaitForSingleObject(mpData->mEvent, INFINITE);
  }
  else 
  {
    res = WaitForSingleObject(mpData->mEvent, time);
  }  
  return (res == WAIT_OBJECT_0);
}


