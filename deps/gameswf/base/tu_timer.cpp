// tu_timer.cpp	-- by Thatcher Ulrich <tu@tulrich.com>

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Utility/profiling timer.


#include <time.h>	// [ANSI/System V]
#include "base/tu_timer.h"


Uint64 tu_timer::get_systime()
// Returns the time as seconds elapsed since midnight, January 1, 1970.
{
	time_t ltime;
	time(&ltime);
	return ltime;
}

int tu_timer::get_date(Uint64 t)
// Returns the day of the month (an integer from 1 to 31)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_mday;
}

int tu_timer::get_day(Uint64 t)
// Returns the day of the week (0 for Sunday, 1 for Monday, and so on)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_wday;
}

int tu_timer::get_hours(Uint64 t)
// Returns the hour (an integer from 0 to 23)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_hour;
}

int tu_timer::get_fullyear(Uint64 t)
// Returns the full year (a four-digit number, such as 2000)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_year + 1900;
}

int tu_timer::get_milli(Uint64 t)
// Returns the milliseconds (an integer from 0 to 999)
{
	return 0;	// TODO
}

int tu_timer::get_month(Uint64 t)
// Returns the month (0 for January, 1 for February, and so on)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_mon;
}

int tu_timer::get_minutes(Uint64 t)
// Returns the minutes (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_min;
}

int tu_timer::get_seconds(Uint64 t)
// Returns the seconds (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_sec;
}

int tu_timer::get_year(Uint64 t)
// Returns the seconds (an integer from 0 to 59)
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return gmt->tm_year;
}

Uint64 tu_timer::get_time(Uint64 t)
// Returns the number of milliseconds since midnight January 1, 1970, universal time
{
	time_t ltime = t;
	struct tm* gmt = localtime(&ltime);
	return t * 1000;	// TODO: add milliseconds
}


#ifdef _WIN32

#include <windows.h>


uint64 tu_timer::get_ticks()
{
	return timeGetTime();
}


double tu_timer::ticks_to_seconds(uint64 ticks)
{
	return ticks * (1.0f / 1000.f);
}

double tu_timer::ticks_to_seconds()
{
	return get_ticks() * (1.0f / 1000.f);
}


void tu_timer::sleep(int milliseconds)
{
	::Sleep(milliseconds);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ use rdtsc?

	LARGE_INTEGER	li;
	QueryPerformanceCounter(&li);

	return li.QuadPart;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	LARGE_INTEGER	freq;
	QueryPerformanceFrequency(&freq);

	double	seconds = (double) ticks;
	seconds /= (double) freq.QuadPart;

	return seconds;
}

#else	// not _WIN32


#include <sys/time.h>
#include <unistd.h>


// The profile ticks implementation is just fine for a normal timer.


uint64 tu_timer::get_ticks()
{
	return profile_ticks_to_milliseconds(get_profile_ticks());
}


double tu_timer::ticks_to_seconds(uint64 ticks)
{
	return profile_ticks_to_seconds(ticks);
}


void tu_timer::sleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}


uint64	tu_timer::get_profile_ticks()
{
	// @@ TODO prefer rdtsc when available?

	// Return microseconds.
	struct timeval tv;
	uint64 result;
	
	gettimeofday(&tv, 0);

	result = tv.tv_sec * 1000000;
	result += tv.tv_usec;
	
	return result;
}


double	tu_timer::profile_ticks_to_seconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to seconds.
	return ticks / 1000000.0;
}

double	tu_timer::profile_ticks_to_milliseconds(uint64 ticks)
{
	// ticks is microseconds.  Convert to milliseconds.
	return ticks / 1000.0;
}

#endif	// not _WIN32


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
