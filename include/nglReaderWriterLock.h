/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

#include "nglLock.h"
#include "nglGuard.h"
#include "nglCondition.h"

class nglReaderWriterLock
{
public:
  nglReaderWriterLock();
  ~nglReaderWriterLock();
  void LockRead();
  bool TryLockRead();
  void UnlockRead();
  void LockWrite();
  bool TryLockWrite();
  void UnlockWrite();
  void LockUpgrade();
  void UnlockUpgrade();
  void UnlockUpgradeAndLock();
  void UnlockAndLockUpgrade();
  void UnlockAndLockRead();
  void UnlockUpgradeAndLockShared();

private:
  struct StateData
  {
    uint32 mSharedCount;
    bool mExclusive;
    bool mUpgrade;
    bool mExclusiveWaitingBlocked;
  };

  StateData mState;
  nglCriticalSection mStateChange;
  nglCondition mSharedCond;
  nglCondition mExclusiveCond;
  nglCondition mUpgradeCond;

  void ReleaseWaiters();

};

