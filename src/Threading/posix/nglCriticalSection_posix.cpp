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
	mutable pthread_mutexattr_t mta;
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
	pthread_mutexattr_init(&mta);
	pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&cmutex, &mta);
}

nglCriticalSectionPrivate::~nglCriticalSectionPrivate ()
{
	pthread_mutex_destroy(&cmutex);
	pthread_mutexattr_destroy(&mta);
}

void nglCriticalSectionPrivate::lock () const
{
	pthread_mutex_lock(&cmutex);
}

bool nglCriticalSectionPrivate::tryLock () const
{
	return (pthread_mutex_trylock(&cmutex) == 0);
}

void nglCriticalSectionPrivate::unlock () const
{
	pthread_mutex_unlock(&cmutex);
}










///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	nglCriticalSection class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

nglCriticalSection::nglCriticalSection(bool registerToThreadChecker)
  : nglLock(registerToThreadChecker)
{
	mpPrivate = new nglCriticalSectionPrivate();
}

nglCriticalSection::nglCriticalSection(const nglString& rName, bool registerToThreadChecker)
  : nglLock(rName, registerToThreadChecker)
{
	mpPrivate = new nglCriticalSectionPrivate();
}


nglCriticalSection::~nglCriticalSection()
{
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
	mpPrivate->lock();
}


// virtual from nglLock
void nglCriticalSection::_Unlock()
{
	mpPrivate->unlock();
}



// virtual from nglLock
bool nglCriticalSection::_TryLock(nglThread::ID threadID)
{
  return mpPrivate->tryLock();
}

















