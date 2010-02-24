/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglReaderWriterLock.h"

nglReaderWriterLock::nglReaderWriterLock()
{
  StateData mState_ = {0,0,0,0};
  mState = mState_;
}

nglReaderWriterLock::~nglReaderWriterLock()
{
}

void nglReaderWriterLock::LockRead()
{
  nglCriticalSectionGuard lk(mStateChange);

  while(mState.mExclusive || mState.mExclusiveWaitingBlocked)
  {
    mSharedCond.Wait();
  }
  ++mState.mSharedCount;
}

bool nglReaderWriterLock::TryLockRead()
{
  nglCriticalSectionGuard lk(mStateChange);

  if(mState.mExclusive || mState.mExclusiveWaitingBlocked)
  {
    return false;
  }
  else
  {
    ++mState.mSharedCount;
    return true;
  }
}

void nglReaderWriterLock::UnlockRead()
{
  nglCriticalSectionGuard lk(mStateChange);
  bool const last_reader=!--mState.mSharedCount;

  if(last_reader)
  {
    if(mState.mUpgrade)
    {
      mState.mUpgrade = false;
      mState.mExclusive = true;
      mUpgradeCond.WakeOne();
    }
    else
    {
      mState.mExclusiveWaitingBlocked=false;
    }
    ReleaseWaiters();
  }
}

void nglReaderWriterLock::LockWrite()
{
  nglCriticalSectionGuard lk(mStateChange);

  while(mState.mSharedCount || mState.mExclusive)
  {
    mState.mExclusiveWaitingBlocked = true;
    mExclusiveCond.Wait();
  }
  mState.mExclusive = true;
}

bool nglReaderWriterLock::TryLockWrite()
{
  nglCriticalSectionGuard lk(mStateChange);

  if(mState.mSharedCount || mState.mExclusive)
  {
    return false;
  }
  else
  {
    mState.mExclusive=true;
    return true;
  }

}

void nglReaderWriterLock::UnlockWrite()
{
  nglCriticalSectionGuard lk(mStateChange);
  mState.mExclusive = false;
  mState.mExclusiveWaitingBlocked = false;
  ReleaseWaiters();
}

void nglReaderWriterLock::LockUpgrade()
{

  nglCriticalSectionGuard lk(mStateChange);
  while(mState.mExclusive || mState.mExclusiveWaitingBlocked || mState.mUpgrade)
  {
    mSharedCond.Wait();
  }
  ++mState.mSharedCount;
  mState.mUpgrade = true;
}

void nglReaderWriterLock::UnlockUpgrade()
{
  nglCriticalSectionGuard lk(mStateChange);
  mState.mUpgrade = false;
  bool const last_reader = !--mState.mSharedCount;

  if(last_reader)
  {
    mState.mExclusiveWaitingBlocked = false;
    ReleaseWaiters();
  }
}

void nglReaderWriterLock::UnlockUpgradeAndLock()
{
  nglCriticalSectionGuard lk(mStateChange);
  --mState.mSharedCount;
  while(mState.mSharedCount)
  {
    mUpgradeCond.Wait();
  }
  mState.mUpgrade = false;
  mState.mExclusive = true;
}

void nglReaderWriterLock::UnlockAndLockUpgrade()
{
  nglCriticalSectionGuard lk(mStateChange);
  mState.mExclusive = false;
  mState.mUpgrade = true;
  ++mState.mSharedCount;
  mState.mExclusiveWaitingBlocked = false;
  ReleaseWaiters();
}

void nglReaderWriterLock::UnlockAndLockRead()
{
  nglCriticalSectionGuard lk(mStateChange);
  mState.mExclusive = false;
  ++mState.mSharedCount;
  mState.mExclusiveWaitingBlocked = false;
  ReleaseWaiters();
}

void nglReaderWriterLock::UnlockUpgradeAndLockShared()
{
  nglCriticalSectionGuard lk(mStateChange);
  mState.mUpgrade = false;
  mState.mExclusiveWaitingBlocked = false;
  ReleaseWaiters();
}

void nglReaderWriterLock::ReleaseWaiters()
{
  mExclusiveCond.WakeOne();
  mSharedCond.WakeAll();
}
