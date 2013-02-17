/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


class nglConditionPrivate
{
  friend class nglCondition;

private:
  pthread_cond_t condition;
};

/**
* Create a nglCondition 
* nglConditions are usefull to synchronise threads
*/
nglCondition::nglCondition()
{
  mpData = new nglConditionPrivate;
  pthread_cond_init(&mpData->condition, 0);
}

/**
* Destroy a nglCondition and wake all the threads waitng for this nglCondition
*/
nglCondition::~nglCondition()
{
	int ret = pthread_cond_destroy(&mpData->condition);
	if (ret)
	{
		// seems we have threads waiting on us, lets wake them up
		pthread_cond_broadcast(&mpData->condition);
	}
  delete mpData;
}

void nglCondition::WakeOne()
{
  pthread_cond_signal(&mpData->condition);
}

void nglCondition::WakeAll()
{
  pthread_cond_broadcast(&mpData->condition);
}

/**
* Wait until the nglCondition is set or the timeout is reached
* @param time Time in milliseconds to wait before the function exit if the nglCondition is not set (use ULONG_MAX for an infinite wait (by default))
* @return true is the nglCondition was set otherwise false
*/
bool nglCondition::Wait(uint32 time)
{
	pthread_mutex_t mutex;
	pthread_mutex_init( &mutex, 0 );
	pthread_mutex_lock( &mutex );
	
	int ret;
	if (time != ULONG_MAX) 
	{
		struct timeval tv;
		gettimeofday(&tv, 0);
		
		timespec ti;
		ti.tv_nsec = ( tv.tv_usec + ( time % 1000 ) * 1000 ) * 1000;
		ti.tv_sec = tv.tv_sec + (time / 1000) + ( ti.tv_nsec / 1000000000 );
		ti.tv_nsec %= 1000000000;
		
		ret = pthread_cond_timedwait(&mpData->condition, &mutex, &ti);
	} 
	else 
	{
		ret = pthread_cond_wait(&mpData->condition, &mutex);
	}
	if (ret && ret != ETIMEDOUT) 
	{
		NGL_ASSERT(0);
	}
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
	
	return (ret == 0);
}
	

