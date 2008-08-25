// as_date.cpp	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// The Date class lets you retrieve date and time values relative to universal time
// (Greenwich mean time, now called universal time or UTC) 
// or relative to the operating system on which Flash Player is running

#include "gameswf_as_classes/as_date.h"
#include "gameswf_log.h"
#include "gameswf_function.h"

namespace gameswf
{

	// Date([yearOrTimevalue:Number], [month:Number], [date:Number],
	// [hour:Number], [minute:Number], [second:Number], [millisecond:Number])
	void	as_global_date_ctor(const fn_call& fn)
	{
		smart_ptr<as_date>	obj = new as_date(fn);
		fn.result->set_as_object(obj.get_ptr());
	}

	// getDate() : Number
	// Returns the day of the month (an integer from 1 to 31)
	void	as_date_getdate(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_date(t));
	}

	// getDay() : Number
	// Returns the day of the week (0 for Sunday, 1 for Monday, and so on)
	void	as_date_getday(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_day(t));
	}

	// getFullYear() : Number
	// Returns the full year (a four-digit number, such as 2000)
	void	as_date_getfullyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_fullyear(t));
	}

	// getHours() : Number
	// Returns the hour (an integer from 0 to 23)
	void	as_date_gethours(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_hours(t));
	}

	// getMilliseconds() : Number
	// Returns the milliseconds (an integer from 0 to 999)
	void	as_date_getmilli(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_milli(t));
	}

	// getMinutes() : Number
	// Returns the minutes (an integer from 0 to 59)
 	void	as_date_getminutes(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_minutes(t));
	}

	// getMonth() : Number
	// Returns the month (0 for January, 1 for February, and so on)
	void	as_date_getmonth(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_month(t));
	}

	// getSeconds() : Number
	// Returns the seconds (an integer from 0 to 59)
	void	as_date_getseconds(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_seconds(t));
	}

	// getTime() : Number
	// Returns the number of milliseconds since midnight January 1, 1970, universal time
	void	as_date_gettime(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_double((double) tu_timer::get_time(t));
	}

	// getYear() : Number
	// Returns the year - 1900
	void	as_date_getyear(const fn_call& fn)
	{
		as_date* dt = cast_to<as_date>(fn.this_ptr);
		assert(dt);

		Uint64 t = dt->get_time();
		fn.result->set_int(tu_timer::get_year(t));
	}

	Uint64 as_date::get_time() const
	{
		return m_time;
	}

	as_date::as_date(const fn_call& fn) :
		as_object(fn.get_player()),
		m_time(tu_timer::get_systime())
	{
		if (fn.nargs > 0)
		{
			log_error("Additional parameters are not supported yet\n");
		}

		builtin_member("getDate", as_date_getdate);
		builtin_member("getDay", as_date_getday);
		builtin_member("getFullYear", as_date_getfullyear);
		builtin_member("getHours", as_date_gethours);
		builtin_member("getMilliseconds", as_date_getmilli);
		builtin_member("getMinutes", as_date_getminutes);
		builtin_member("getMonth", as_date_getmonth);
		builtin_member("getSeconds", as_date_getseconds);
		builtin_member("getTime", as_date_gettime);
		builtin_member("getYear", as_date_getyear);


/*
 getTimezoneOffset() : Number
 Returns the difference, in minutes, between the computer's local time and universal time.
 

 getUTCDate() : Number
 Returns the day of the month (an integer from 1 to 31) in the specified Date object, according to universal time.
 

 getUTCDay() : Number
 Returns the day of the week (0 for Sunday, 1 for Monday, and so on) of the specified Date object, according to universal time.
 

 getUTCFullYear() : Number
 Returns the four-digit year of the specified Date object, according to universal time.
 

 getUTCHours() : Number
 Returns the hour (an integer from 0 to 23) of the specified Date object, according to universal time.
 

 getUTCMilliseconds() : Number
 Returns the milliseconds (an integer from 0 to 999) of the specified Date object, according to universal time.
 

 getUTCMinutes() : Number
 Returns the minutes (an integer from 0 to 59) of the specified Date object, according to universal time.
 

 getUTCMonth() : Number
 Returns the month (0 [January] to 11 [December]) of the specified Date object, according to universal time.
 

 getUTCSeconds() : Number
 Returns the seconds (an integer from 0 to 59) of the specified Date object, according to universal time.
 

 getUTCYear() : Number
 Returns the year of this Date according to universal time (UTC).
 

 setDate(date:Number) : Number
 Sets the day of the month for the specified Date object, according to local time, and returns the new time in milliseconds.
 

 setFullYear(year:Number, [month:Number], [date:Number]) : Number
 Sets the year of the specified Date object, according to local time and returns the new time in milliseconds.
 

 setHours(hour:Number) : Number
 Sets the hours for the specified Date object according to local time and returns the new time in milliseconds.
 

 setMilliseconds(millisecond:Number) : Number
 Sets the milliseconds for the specified Date object according to local time and returns the new time in milliseconds.
 

 setMinutes(minute:Number) : Number
 Sets the minutes for a specified Date object according to local time and returns the new time in milliseconds.
 

 setMonth(month:Number, [date:Number]) : Number
 Sets the month for the specified Date object in local time and returns the new time in milliseconds.
 

 setSeconds(second:Number) : Number
 Sets the seconds for the specified Date object in local time and returns the new time in milliseconds.
 

 setTime(millisecond:Number) : Number
 Sets the date for the specified Date object in milliseconds since midnight on January 1, 1970, and returns the new time in milliseconds.
 

 setUTCDate(date:Number) : Number
 Sets the date for the specified Date object in universal time and returns the new time in milliseconds.
 

 setUTCFullYear(year:Number, [month:Number], [date:Number]) : Number
 Sets the year for the specified Date object (my_date) in universal time and returns the new time in milliseconds.
 

 setUTCHours(hour:Number, [minute:Number], [second:Number], [millisecond:Number]) : Number
 Sets the hour for the specified Date object in universal time and returns the new time in milliseconds.
 

 setUTCMilliseconds(millisecond:Number) : Number
 Sets the milliseconds for the specified Date object in universal time and returns the new time in milliseconds.
 

 setUTCMinutes(minute:Number, [second:Number], [millisecond:Number]) : Number
 Sets the minute for the specified Date object in universal time and returns the new time in milliseconds.
 

 setUTCMonth(month:Number, [date:Number]) : Number
 Sets the month, and optionally the day, for the specified Date object in universal time and returns the new time in milliseconds.
 

 setUTCSeconds(second:Number, [millisecond:Number]) : Number
 Sets the seconds for the specified Date object in universal time and returns the new time in milliseconds.
 

 setYear(year:Number) : Number
 Sets the year for the specified Date object in local time and returns the new time in milliseconds.
 

 toString() : String
 Returns a string value for the specified date object in a readable format.
 
static
 UTC(year:Number, month:Number, [date:Number], [hour:Number], [minute:Number], [second:Number], [millisecond:Number]) : Number
 Returns the number of milliseconds between midnight on January 1, 1970, universal time, and the time specified in the parameters.
 

 valueOf() : Number
 */

	}
};
