/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"


nglLock::nglLock(bool registerToThreadChecker)
{
  mRegisterToThreadChecker = registerToThreadChecker;

  if (mRegisterToThreadChecker)
    nglThreadChecker::AddRef();
}


nglLock::nglLock(const nglString& rName, bool registerToThreadChecker)
{
  mName = rName;
  mRegisterToThreadChecker = registerToThreadChecker;

  if (mRegisterToThreadChecker)
    nglThreadChecker::AddRef();
}



nglLock::~nglLock()
{
  if (mRegisterToThreadChecker)
    nglThreadChecker::ReleaseRef();
}


void nglLock::Lock()
{
  // for dead-lock checker
  nglThread::ID threadID = nglThread::GetCurThreadID();
  if (mRegisterToThreadChecker)
    nglThreadChecker::Wait(threadID, this);

  // call inherited Lock implementation (critical section or light lock)
  _Lock(threadID);

  if (mRegisterToThreadChecker)
    nglThreadChecker::Lock(threadID, this);
}

void nglLock::Unlock()
{
  // call inherited Unlock implementation (critical section or light lock)
  _Unlock();

  // for dead-lock checker
  if (mRegisterToThreadChecker)
  {
    nglThread::ID threadID = nglThread::GetCurThreadID();
    nglThreadChecker::Unlock(threadID, this);
  }
}


bool nglLock::TryLock()
{
  nglThread::ID threadID = nglThread::GetCurThreadID();

  // call inherited TryLock implementation (critical section or light lock)
  bool res = _TryLock(threadID);

  // for dead-lock checker
  if (res && mRegisterToThreadChecker)
    nglThreadChecker::Lock(threadID, this);

	return res;
}


const nglString& nglLock::GetName() const
{
  return mName;
}












