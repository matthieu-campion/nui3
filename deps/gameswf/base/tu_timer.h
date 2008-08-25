// tu_timer.h	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Utility/profiling timer.


#ifndef TU_TIMER_H
#define TU_TIMER_H

#include "base/tu_types.h"

namespace tu_timer
{

	// General-purpose wall-clock timer.  May not be hi-res enough
	// for profiling.
	exported_module uint64 get_ticks();
	exported_module double ticks_to_seconds(uint64 ticks);

	// current ticks to seconds
	exported_module double ticks_to_seconds();

	// Sleep the current thread for the given number of
	// milliseconds.  Don't rely on the sleep period being very
	// accurate.
	exported_module void sleep(int milliseconds);
	
	// Hi-res timer for CPU profiling.

	// Return a hi-res timer value.  Time 0 is arbitrary, so
	// generally you want to call this at the start and end of an
	// operation, and pass the difference to
	// profile_ticks_to_seconds() to find out how long the
	// operation took.
	exported_module uint64	get_profile_ticks();

	// Convert a hi-res ticks value into seconds.
	exported_module double	profile_ticks_to_seconds(uint64 profile_ticks);

	exported_module double	profile_ticks_to_milliseconds(uint64 ticks);

	// Returns the systime
	exported_module Uint64 get_systime();

	// Returns the day of the month (an integer from 1 to 31)
	int get_date(Uint64 t);

	// Returns the day of the week (0 for Sunday, 1 for Monday, and so on)
	int get_day(Uint64 t);

	// Returns the hour (an integer from 0 to 23)
	int get_hours(Uint64 t);

	// Returns the full year (a four-digit number, such as 2000)
	int get_fullyear(Uint64 t);

	// Returns the year - 1900
	int get_year(Uint64 t);
	
	// Returns the milliseconds (an integer from 0 to 999)
	int get_milli(Uint64 t);

	// Returns the month (0 for January, 1 for February, and so on)
	int get_month(Uint64 t);

	// Returns the minutes (an integer from 0 to 59)
	int get_minutes(Uint64 t);

	// Returns the seconds (an integer from 0 to 59)
	int get_seconds(Uint64 t);

	// Returns the number of milliseconds since midnight January 1, 1970, universal time
	Uint64 get_time(Uint64 t);

};


#endif // TU_TIMER_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
