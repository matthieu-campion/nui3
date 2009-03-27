/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"
#include "nglThread.h"

#include <iostream>
#include <cstring>

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include "nglThreadChecker.h"


static void *start_thread(void *arg); 

class nglThreadPrivate 
{
  friend class nglThread;
  
public:
  static inline void Start(nglThread * pThread) 
  {
    pThread->OnStart();
    pThread->mState = nglThread::Closed;
    if (pThread->GetAutoDelete())
      delete pThread;
  }

  static inline void exit(nglThread * pThread)
  {
    pThread->mpData->thread = NULL;
  }

  nglThread::ID GetThreadID()
  {
    return thread;  
  }

private:

  pthread_t thread;
};


static void *start_thread(void *arg) 
{
  nglThread *thread = (nglThread *)arg;

  // register the thread by the nglThreadChecker
  nglThreadChecker::RegisterThread(thread->GetID(), thread->GetName());  
  
  nglThreadPrivate::Start(thread);

  // warn the nglThreadChecker that the thread stoped
  nglThreadChecker::UnregisterThread(thread->GetID());
  
  pthread_exit(0);
  nglThreadPrivate::exit(thread);
  nglDelThreadFromGlobalList(thread);
  return 0;
}

nglThread::nglThread(Priority priority)
{
  mpData = new nglThreadPrivate();
  
  mpData->thread = 0;
  mState = Stopped;
  mPriority = priority;
  mAutoDelete = false;
}

nglThread::nglThread(const nglString& rName, Priority priority)
{
  mName = rName;
  mpData = new nglThreadPrivate();
  
  mpData->thread = 0;
  mState = Stopped;
  mPriority = priority;
  mAutoDelete = false;
}


nglThread::~nglThread()
{
  delete mpData;
}

bool nglThread::Start()
{
  
  int ret = 0;
  //if (!mpData->thread) // If the thread is not created, do it:
  {
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);
    //		pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    ret = pthread_create(&mpData->thread, &attr, start_thread, this);
    nglAddThreadInGlobalList(this);
    
    if (ret != 0) 
    {
      NGL_ASSERT(0);
    }
    
    //		pthread_attr_destroy(&attr);
  }
  mState = Running;
  
  return true;
}

bool nglThread::Join()
{
  bool res = true;
  
  if (pthread_join(mpData->thread, NULL) != 0)
  {
    res = false;
  }
  return res;
}

void nglThread::OnStart()
{
  // Do nothing by default
}

bool nglThread::IsCurrent() const
{
  return (pthread_self() == mpData->thread);
}

nglThread::State nglThread::GetState() const
{
  return mState;
}

nglThread::Priority nglThread::GetPriority() const
{
  return mPriority;
}

#ifndef PTHREAD_MIN_PRIORITY
#define PTHREAD_MIN_PRIORITY 1
#endif

#ifndef PTHREAD_MAX_PRIORITY
#define PTHREAD_MAX_PRIORITY 127
#endif

bool nglThread::SetPriority(Priority priority)
{
	int policy;
	struct sched_param sched;
	pthread_getschedparam(mpData->GetThreadID(), &policy, &sched);
	sched.sched_priority = PTHREAD_MIN_PRIORITY + (priority * (PTHREAD_MAX_PRIORITY - PTHREAD_MIN_PRIORITY) / (Critical - Lowest));
  if (sched.sched_priority < PTHREAD_MIN_PRIORITY)
    sched.sched_priority = PTHREAD_MIN_PRIORITY;
  if (sched.sched_priority > PTHREAD_MAX_PRIORITY)
    sched.sched_priority = PTHREAD_MAX_PRIORITY;
 	return !pthread_setschedparam(mpData->GetThreadID(), policy, &sched);
}

void nglThread::Sleep(uint32 secs)
{
  sleep(secs);
}

void nglThread::MsSleep(uint32 msecs)
{
  usleep(1000 * msecs);
}

void nglThread::USleep(uint32 usecs)
{
  usleep(usecs);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		GetCurThread
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	 Return current active thread
//
nglThread* nglThread::GetCurThread()
{
  return nglGetThreadFromGlobalList( pthread_self() );
}


// static, Return current thread ID (system calls)
nglThread::ID nglThread::GetCurThreadID()
{
  return pthread_self();
}



nglThread::ID nglThread::GetID() const
{
  return mpData->GetThreadID();
}

