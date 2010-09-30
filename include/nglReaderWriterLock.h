/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nuiNonCopyable.h"
#include "nglLock.h"
#include "nglGuard.h"
#include "nglCondition.h"

class nglReaderWriterLock : nuiNonCopyable
{
public:

  nglReaderWriterLock();

  void LockRead();
  void UnlockRead();
  void LockWrite();
  void UnlockWrite();

private:

  nglAtomic mReaders;
  nglThread::ID mWriter;

  nglCriticalSection mCS;

  nglCondition mWaitForRead;
  nglCondition mWaitForWrite;
};

