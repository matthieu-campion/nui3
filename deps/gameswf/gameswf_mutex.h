// tu_mutex.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_MUTEX_H
#define GAMESWF_MUTEX_H

#include "base/tu_config.h"
#include "gameswf_types.h"
#include "gameswf_log.h"

#if TU_CONFIG_LINK_TO_THREAD == 1

#include <SDL.h>
#include <SDL_thread.h>

namespace gameswf
{

	struct tu_thread
	{
		tu_thread(int (*fn)(void *), void* data)
		{
			IF_VERBOSE_ACTION(log_msg("gameswf is in multi thread mode\n"));
			m_thread = SDL_CreateThread(fn, data);
			assert(m_thread);
		}

		~tu_thread()
		{
			kill();
		}

		void wait()
		{
			SDL_WaitThread(m_thread, NULL);
			m_thread = NULL;
		}

		void kill()
		{
			SDL_KillThread(m_thread);
			m_thread = NULL;
		}

	private:

		SDL_Thread* m_thread;

	};

	struct tu_mutex
	{
		tu_mutex()
		{
			m_mutex = SDL_CreateMutex();
		}

		~tu_mutex() 
		{
			SDL_DestroyMutex(m_mutex);
		}

		inline void lock() 
		{
			SDL_LockMutex(m_mutex);
		}

		inline void unlock() 
		{
			SDL_UnlockMutex(m_mutex);
		}

		SDL_mutex* get_ptr()
		{
			return m_mutex;
		}

		private:

			SDL_mutex* m_mutex;
	};

	// like autoptr
	struct tu_autolock
	{
		tu_mutex& m_mutex;
		tu_autolock(tu_mutex& mutex) :
			m_mutex(mutex)
		{
			m_mutex.lock();
		}

		~tu_autolock()
		{
			m_mutex.unlock();
		}
	};

	struct tu_condition
	{
		tu_condition()
		{
			m_cond = SDL_CreateCond();
		}

		~tu_condition()
		{
			SDL_DestroyCond(m_cond);
		}

		// Wait on the condition variable cond and unlock the provided mutex.
		// The mutex must the locked before entering this function.
		void wait()
		{
			m_cond_mutex.lock();
			SDL_CondWait(m_cond, m_cond_mutex.get_ptr());
		}

		void signal()
		{
			SDL_CondSignal(m_cond);
		}

		SDL_cond* m_cond;
		tu_mutex m_cond_mutex;
	};

}

#elif TU_CONFIG_LINK_TO_THREAD == 2	// libpthread
// TODO
#else

namespace gameswf
{
	struct tu_thread
	{
		tu_thread(int (*fn)(void *), void* data)
		{
			IF_VERBOSE_ACTION(log_msg("gameswf is in single thread mode\n"));
			(fn)(data);
		}

		~tu_thread() {}

		void wait() {}
		void kill() {}
	};

	struct tu_mutex
	{
		tu_mutex() {}
		~tu_mutex() {}
		void lock() {}
		void unlock() {}
	};

	struct tu_condition
	{
		tu_condition() {}
		~tu_condition() {}

		// Wait on the condition variable cond and unlock the provided mutex.
		// The mutex must the locked before entering this function.
		void wait() {}
		void signal() {}
	};

    struct tu_autolock
    {
        tu_mutex& m_mutex;
        tu_autolock(tu_mutex& mutex) :
        m_mutex(mutex)
        {
            m_mutex.lock();
        }

        ~tu_autolock()
        {
            m_mutex.unlock();
        }
    };

}

#endif	// TU_CONFIG_LINK_TO_THREAD

#endif	// GAMESWF_MUTEX_H
