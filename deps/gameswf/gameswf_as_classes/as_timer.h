// gameswf_timers.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

#ifndef GAMESWF_AS_TIMER_H
#define GAMESWF_AS_TIMER_H

#include "gameswf_action.h"	// for as_object

namespace gameswf 
{
	struct as_timer : public as_object
	{
		// Unique id of a gameswf resource
		enum { m_class_id = AS_TIMER };
		virtual bool is(int class_id) const
		{
			if (m_class_id == class_id) return true;
			else return as_object::is(class_id);
		}

		float m_interval;	// sec
		as_value m_func;
		float m_delta_time;
		array<as_value> m_param;

		as_timer(as_value& func, double intarval, const fn_call& fn);
		~as_timer();

		virtual void advance(float delta_time);

		void clear();
	};


}

#endif
