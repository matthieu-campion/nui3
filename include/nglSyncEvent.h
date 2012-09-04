/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiNonCopyable.h"

class nglSyncEventPrivate;

class nglSyncEvent : nuiNonCopyable
{

public:
  nglSyncEvent();
  virtual ~nglSyncEvent();

  void Set();
  void Reset();
  void Pulse() // Set then Reset the event
  {
    Set();
    Reset();
  }

  bool Wait(uint32 time = std::numeric_limits<uint32>::max()); ///< Wait for the event to be set. The wait time is given in milliseconds. If time == ULONG_MAX the wait will be infinite. The method returns true if the event was set, false otherwise.

#ifdef WIN32
  HANDLE nglSyncEvent::GetNativeHandle() const;
#endif
private:
  nglSyncEventPrivate *mpData;
};



