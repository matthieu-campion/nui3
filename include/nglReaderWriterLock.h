/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglLock.h"
#include "nglGuard.h"
#include "nglSyncEvent.h"

class nglReaderWriterLock
{
public:
  nglReaderWriterLock();

  void ReadLock();
  void ReadUnlock();

  void WriteLock();
  void WriteUnlock();

private:
  void PulseWaitingThreads();
  bool WriteLockPreCondition(nglThread::ID CurrentThreadId);
  bool ReadLockPreCondition(nglThread::ID CurrentThreadId);

  nglSyncEvent mSyncEvent;
  nglCriticalSection mCS;
  int32 mLockCount;
  int32 mWaitingReaders;
  int32 mWaitingWriters;
  nglThread::ID mWriterThreadId;
};
