/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglReaderWriterLock.h"

nglReaderWriterLock::nglReaderWriterLock()
{
  mLockCount = 0;
  mWaitingReaders = 0;
  mWaitingWriters = 0;
  mWriterThreadId = 0;
}

void nglReaderWriterLock::ReadLock()
{
  nglThread::ID currentId = nglThread::GetCurThreadID();

  {
    nglCriticalSectionGuard g(mCS);
    // wait until there are no more pending writers, and no writer other than me has the lock
    if (!ReadLockPreCondition(currentId))
    {
      mWaitingReaders++;
      do
      {
        mSyncEvent.Wait();
      } while (!ReadLockPreCondition(currentId));
      mWaitingReaders--;
    }
    mLockCount++;
  }
}

void nglReaderWriterLock::ReadUnlock()
{
  {
    nglCriticalSectionGuard g(mCS);
    if (mLockCount > 0)
    {
      mLockCount--;
      if (mLockCount == 0)
      {
        PulseWaitingThreads();
      }
    }
    else
    {
      NGL_ASSERT(!"Unbalanced acquire/release read lock detected");
    }
  }
}

void nglReaderWriterLock::WriteLock()
{
  nglThread::ID currentId = nglThread::GetCurThreadID();

  {
    nglCriticalSectionGuard g(mCS);
    // Wait for other readers or writers to become ready
    if (!WriteLockPreCondition(currentId))
    {
      mWaitingWriters++;
      do
      {
        mSyncEvent.Wait();
        mSyncEvent.Reset();
      } while (!WriteLockPreCondition(currentId));
      mWaitingWriters--;
    }
    // the following only gets executed if no exception was thrown:
    mLockCount++;
    mWriterThreadId = currentId;
  }
}

void nglReaderWriterLock::WriteUnlock()
{
  nglThread::ID currentId = nglThread::GetCurThreadID();

  {
    nglCriticalSectionGuard g(mCS);
    if (mLockCount > 0)
    {
      if (mWriterThreadId == currentId)
      {
        // writelock was owned by me
        mLockCount--;

        if (mLockCount == 0)
        {
          mWriterThreadId = 0;
          PulseWaitingThreads();
        }
      }
      else
      {
        NGL_ASSERT(!"The calling thread attempted to release a write lock but does not own the lock for the specified object");
      }
    }
    else
    {
      NGL_ASSERT(!"Unbalanced acquire/release write lock detected");
    }
  }
}

void nglReaderWriterLock::PulseWaitingThreads()
{
  if (mWaitingReaders > 0 || mWaitingWriters > 0)
  {
    mSyncEvent.Set();
  }
}

bool nglReaderWriterLock::WriteLockPreCondition(nglThread::ID currentThreadId)
{
  return (mLockCount == 0 || mWriterThreadId == currentThreadId);
}

bool nglReaderWriterLock::ReadLockPreCondition(nglThread::ID currentThreadId)
{
  return (mWaitingWriters == 0 && (mWriterThreadId == 0 || mWriterThreadId == currentThreadId));
}

