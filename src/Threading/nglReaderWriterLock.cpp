/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglReaderWriterLock.h"


nglReaderWriterLock::nglReaderWriterLock()
{
  mReaders = 0;
  mWriter = 0;
}

void nglReaderWriterLock::LockRead()
{ 
  printf("Check %s %d\n", __FILE__, __LINE__);
  bool ok = false;
  do 
  {
    {
      printf("Check %s %d\n", __FILE__, __LINE__);
      nglCriticalSectionGuard guard(mCS);

      printf("Check %s %d\n", __FILE__, __LINE__);
      if (!mWriter)
      {
        mReaders++;
        ok = true;
      }
    }

    printf("Check %s %d\n", __FILE__, __LINE__);
    if (mWriter)
      mWaitForRead.Wait();

  } while (!ok);
  printf("Check %s %d\n", __FILE__, __LINE__);
}

bool nglReaderWriterLock::TryLockRead()
{ 
  printf("Check %s %d\n", __FILE__, __LINE__);
  bool ok = false;
  {
    printf("Check %s %d\n", __FILE__, __LINE__);
    nglCriticalSectionGuard guard(mCS);
    printf("Check %s %d\n", __FILE__, __LINE__);
    
    if (!mWriter)
    {
      printf("Check %s %d\n", __FILE__, __LINE__);
      mReaders++;
      ok = true;
    }
  }

  printf("Check %s %d\n", __FILE__, __LINE__);
  return ok;  
}

void nglReaderWriterLock::UnlockRead()
{
  printf("Check %s %d\n", __FILE__, __LINE__);
  nglCriticalSectionGuard guard(mCS);

  printf("Check %s %d\n", __FILE__, __LINE__);
  ngl_atomic_dec(mReaders);
  if (!mReaders)
    mWaitForWrite.WakeOne();
  printf("Check %s %d\n", __FILE__, __LINE__);
}

void nglReaderWriterLock::LockWrite()
{
  printf("Check %s %d\n", __FILE__, __LINE__);
  do 
  {
    {
      nglCriticalSectionGuard guard(mCS);

      printf("Check %s %d\n", __FILE__, __LINE__);
      if (!mWriter && !mReaders)
      {
        printf("Check %s %d\n", __FILE__, __LINE__);
        mWriter = nglThread::GetCurThreadID();
      }
    }

    printf("Check %s %d\n", __FILE__, __LINE__);
    printf ("Writer: %d / Current: %d\n", mWriter, nglThread::GetCurThreadID());
    if (mWriter != nglThread::GetCurThreadID())
      mWaitForWrite.Wait();

    printf("Check %s %d\n", __FILE__, __LINE__);
  } while (mWriter != nglThread::GetCurThreadID());
  printf("Check %s %d\n", __FILE__, __LINE__);
}

bool nglReaderWriterLock::TryLockWrite()
{
  printf("Check %s %d\n", __FILE__, __LINE__);
  {
    nglCriticalSectionGuard guard(mCS);
    
    printf("Check %s %d\n", __FILE__, __LINE__);
    if (!mWriter && !mReaders)
    {
      printf("Check %s %d\n", __FILE__, __LINE__);
      mWriter = nglThread::GetCurThreadID();
    }
  }
  
  printf("Check %s %d\n", __FILE__, __LINE__);
  return mWriter == nglThread::GetCurThreadID();
}


void nglReaderWriterLock::UnlockWrite()
{
  printf("Check %s %d\n", __FILE__, __LINE__);
  nglCriticalSectionGuard guard(mCS);

  printf("Check %s %d\n", __FILE__, __LINE__);
  mWriter = NULL;
  if (!mReaders)
    mWaitForWrite.WakeOne();
  else
    mWaitForRead.WakeAll();
  printf("Check %s %d\n", __FILE__, __LINE__);
}



