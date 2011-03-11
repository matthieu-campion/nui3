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

/*! A multiple reader/one writer lock for multithreading.

    A multiple reader/one writer lock for multithreading.
    There can be at most one writer but any number of readers.
 */
class nglReaderWriterLock : nuiNonCopyable
{
public:

  nglReaderWriterLock(); ///< Create a Reader/Writer lock. The object is unlocked by default.

  void LockRead(); ///< Acquire the lock as a reader. There can be any number of readers. This method returns only once the locking operation succeeded.
  bool TryLockRead(); ///< Try to acquire the lock as a reader. There can be any number of readers. If the object is already locked by a writer the operation will fail and the method returns false. Otherwise it returns true which indicates that you have successfully acquired the reader lock. this method returns immediately.
  void UnlockRead(); ///< Release the lock that was previously acquired as a reader. If there is no other readers the lock will be lockable by a writer.

  void LockWrite(); ///< Acquire the lock as a writer. There can be only one writer. You can only be a writer is no other reader and not other writer exists. This method returns only when the lock operation succeeded.
  bool TryLockWrite(); ///< Try to acquire the lock as a writer. If the operation succeeds, the method returns true and the lock is acquired. If it fails, false is returned. There can be only one writer. You can only be a writer is no other reader and not other writer exists. This method returns immediately.
  void UnlockWrite(); ///< Release the lock that was previously acquired as a writer. Once the write lock is released the object can be acquired for reading or writing.


private:

  nglAtomic mReaders;
  nglThread::ID mWriter;

  nglCriticalSection mCS;

  nglCondition mWaitForRead;
  nglCondition mWaitForWrite;
};

