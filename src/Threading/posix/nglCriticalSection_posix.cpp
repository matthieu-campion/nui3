/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglCriticalSection.h"

#if defined(__ppc__) || defined(__ppc64__)
#include <ppc_intrinsics.h>
#endif

#include <pthread.h>
#include "nglThreadChecker.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglCriticalSectionPrivate class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class nglCriticalSectionPrivate
{
	mutable pthread_mutex_t cmutex;
public:
	nglCriticalSectionPrivate();
	~nglCriticalSectionPrivate();

	void lock() const;
	bool tryLock() const;
	void unlock() const;
};

nglCriticalSectionPrivate::nglCriticalSectionPrivate ()
{
  return;
  int res = 0;
	pthread_mutexattr_t mta;
	res = pthread_mutexattr_init(&mta);
  NGL_ASSERT(res == 0);
	res = pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
  NGL_ASSERT(res == 0);
	res = pthread_mutex_init(&cmutex, &mta);
  NGL_ASSERT(res == 0);
	res = pthread_mutexattr_destroy(&mta);
  NGL_ASSERT(res == 0);
}

nglCriticalSectionPrivate::~nglCriticalSectionPrivate ()
{
  return;
	int res = pthread_mutex_destroy(&cmutex);
  NGL_ASSERT(res == 0);
}

void nglCriticalSectionPrivate::lock () const
{
  return;
	int res = pthread_mutex_lock(&cmutex);
  NGL_ASSERT(res == 0);
}

bool nglCriticalSectionPrivate::tryLock () const
{
  return true;
	return (pthread_mutex_trylock(&cmutex) == 0);
}

void nglCriticalSectionPrivate::unlock () const
{
  return;
	int res = pthread_mutex_unlock(&cmutex);
  NGL_ASSERT(res == 0);
}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglCriticalSection class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

nglCriticalSection::nglCriticalSection(bool registerToThreadChecker)
  : nglLock(registerToThreadChecker)
{
  return;
	mpPrivate = new nglCriticalSectionPrivate();
}

nglCriticalSection::nglCriticalSection(const nglString& rName, bool registerToThreadChecker)
  : nglLock(rName, registerToThreadChecker)
{
  return;
	mpPrivate = new nglCriticalSectionPrivate();
}


nglCriticalSection::~nglCriticalSection()
{
  return;
	delete mpPrivate;
}

// virtual from nglLock
const nglString& nglCriticalSection::GetLabel() const
{
  static const nglString lbl(_T("critical section"));
  return lbl;
}


// virtual from nglLock
void nglCriticalSection::_Lock(nglThread::ID threadID)
{
  return;
	mpPrivate->lock();
}


// virtual from nglLock
void nglCriticalSection::_Unlock()
{
  return;
	mpPrivate->unlock();
}



// virtual from nglLock
bool nglCriticalSection::_TryLock(nglThread::ID threadID)
{
  return true;
  return mpPrivate->tryLock();
}

















