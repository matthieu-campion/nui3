/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

class nglSyncEventPrivate
{
  friend class nglSyncEvent;

private:
  pthread_cond_t mCondition;
  pthread_mutex_t mMutex;
  bool mSet;
  
};

nglSyncEvent::nglSyncEvent()
{
  mpData = new nglSyncEventPrivate;
  pthread_cond_init(&mpData->mCondition, 0);
  pthread_mutex_init(&mpData->mMutex, 0);
  mpData->mSet = false;
}

nglSyncEvent::~nglSyncEvent()
{
  pthread_cond_destroy(&mpData->mCondition);
  pthread_mutex_destroy(&mpData->mMutex);
  delete mpData;
  mpData = NULL;
}

void nglSyncEvent::Set()
{
  NGL_ASSERT(mpData);
  pthread_mutex_lock(&mpData->mMutex);
  mpData->mSet = true;
  pthread_cond_broadcast(&mpData->mCondition);  
  pthread_mutex_unlock(&mpData->mMutex);
}

void nglSyncEvent::Reset()
{
  NGL_ASSERT(mpData);
  pthread_mutex_lock(&mpData->mMutex);
  mpData->mSet = false;
  pthread_mutex_unlock(&mpData->mMutex);
}

bool nglSyncEvent::Wait(uint32 time)
{
  NGL_ASSERT(mpData);
  pthread_mutex_lock(&mpData->mMutex);
  bool test = mpData->mSet;
  
  bool result;
  if (test == false) 
  { // If the nglSyncEvent has not been already set
    
    int ret;
    
    if (time != ULONG_MAX)
    {
      struct timeval tv;
      gettimeofday(&tv, 0);
      
      timespec ti;
      ti.tv_nsec = ( tv.tv_usec + ( time % 1000 ) * 1000 ) * 1000;
      ti.tv_sec = tv.tv_sec + (time / 1000) + ( ti.tv_nsec / 1000000000 );
      ti.tv_nsec %= 1000000000;
      
      ret = pthread_cond_timedwait(&mpData->mCondition, &mpData->mMutex, &ti);
    } 
    else 
    {
      ret = pthread_cond_wait(&mpData->mCondition, &mpData->mMutex);
    }
    
    if (ret && ret != ETIMEDOUT) 
    {
      NGL_ASSERT(0);
    }
    result = (ret == 0);
  }
  else 
  {
    result = true;
  }
  pthread_mutex_unlock(&mpData->mMutex);
  return result;
}


