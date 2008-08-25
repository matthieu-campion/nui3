/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include <limits.h>

class nglSyncEventPrivate;

class nglSyncEvent
{

public:
  nglSyncEvent();
  virtual ~nglSyncEvent();

  void Set();
  void Reset();

  bool Wait(uint32 time = ULONG_MAX);

private:
  nglSyncEventPrivate *mpData;
};



