/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

/*!
\file  nglTime.h
\brief Portable time and date
*/

#ifndef __nglTime_h__
#define __nglTime_h__

//#include "nui.h"
#include "nglString.h"
#include <time.h>


//! Tolerance for time comparisons (5us)
#define NGL_TIME_EPSILON 5E-6


//! Time information 
/*!
nglTime::GetGMTime() and nglTime::GetLocalTime() can break up a time stamp
in calendar units.
*/
class nglTimeInfo
{
public:
  int Seconds;  ///< 0..59
  int Minutes;  ///< 0..59
  int Hours;    ///< 0..23
  int Day;      ///< 1..31
  int Month;    ///< 1..12 (unlike system's gmtime, 0..11)
  int Year;     ///< number of years since 1900
  int WeekDay;  ///< 0..6 (0 is Sunday)
  int DST;      ///< day light saving time (1..366)

private:
  void CopyToSys   (struct tm* pTM) const;
  void CopyFromSys (const struct tm* pTM);

  friend class nglTime;
};


//! Portable time and date
/*!
Time is represented in NGL by a floating point value which unit is the second.
The time origin is not portable : it will be different under Unix (mostly Epoch)
or Windows for instance. You should only rely on time differences or interpret
a nglTime as absolute time with the GetDate() method.

All arithmetic operations are allowed (+, -, /, * and so on) since nglTime
objets are automatically casted to the \e double type in arithmetic expressions.
The increment (+=) and decrement (-=) operators are implemented for completeness.

Usage example :

\code
  nglTime x;         // x holds the system current time (at x instantiation time !)
  nglTime y = 10;    // this is equivalent to nglTime y(10);
  nglTime z = x + y; // or nglTime z(x+y);
  x += y;
  y += 5;
  z = x - y;
\endcode

\b Precision : the nglTime implementation tries to be the more precise possible.
Windows and Linux (glibc 2.2) rely on the processor clock itself (via the RDTSC
instruction), thus you can expect 0.01µs precision on a several-hundred-MHz processor.
*/
class NGL_API nglTime
{
public:
  /** @name Useful constants */
  //@{
  static const nglTime Zero;    ///< zero delay
  static const nglTime Second;  ///< nglTime unit
  static const nglTime Minute;  ///< ... or 60 seconds
  static const nglTime Hour;    ///< ... or 3600 seconds
  static const nglTime Day;     ///< ... or 86400 seconds
  static const nglTime Week;    ///< ... or 604800 seconds
  static const nglTime Year;    ///< ... or 31557600 seconds (365,25 days)
  //@}

  /** @name Life cycle */
  //@{
  nglTime();
  ///< Get current system time (calendar time)
  nglTime(double Value);
  /*!< Create a time stamp from user value
    \param Value time stamp (in second)

    You may build nglTime objects from user values : note that only time
    stamps based on arithmetics calculus from system time (see nglTime())
    can be properly interpreted by GetDate().
  */
  nglTime(const nglTimeInfo& rInfo);  ///< Build a time stamp from a date
  nglTime(const nglTime& rTime);      ///< Copy constructor
  //@}

  /** @name Reading time */
  //@{
  double    GetValue();                       ///< Return time stamp value (unit : second)
  bool      GetGMTime   (nglTimeInfo& rInfo);  ///< Interpret current time stamp as calendar time and return GMT time info
  bool      GetLocalTime(nglTimeInfo& rInfo);  ///< Interpret current time stamp as calendar time and return local time info
  nglString GetGMTimeStr   (const char* pFormat); ///< Interpret current time stamp as calendar time and format a GMT date (see GetLocalTimeStr())
  nglString GetLocalTimeStr(const char* pFormat);
  /*!< Interpret current time stamp as calendar time and return formated time
    \param pFormat format string

    This method is based on the ANSI strftime (3). The \a pFormat format string can be
    composed of any printable characters and the following special sequences :

    - <b>%%a</b> : the abbreviated weekday name according to the current locale
    - <b>%%A</b> : the full weekday name according to the current locale
    - <b>%%b</b> : the abbreviated month name according to the current locale
    - <b>%%B</b> : the full month name according to the current locale
    - <b>%%c</b> : the preferred date and time representation for the current locale
    - <b>%%d</b> : the day of the month as a decimal number (range 01 to 31)
    - <b>%%H</b> : the hour as a decimal number using a 24-hour clock (range 00 to 23)
    - <b>%%I</b> : the hour as a decimal number using a 12-hour clock (range 01 to 12)
    - <b>%%j</b> : the day of the year as a decimal number (range 001 to 366)
    - <b>%%m</b> : the month as a decimal number (range 01 to 12)
    - <b>%%M</b> : the minute as a decimal number (range 00 to 59)
    - <b>%%p</b> : either `AM' or `PM' according to the given time value, or the corresponding
           strings for  the  current locale.  Noon is treated as `pm' and midnight as `am'
    - <b>%%S</b> : the second as a decimal number (range 00 to 59)
    - <b>%%U</b> : the  week  number of the current year as a decimal number, range 00 to 53,
           starting with the first Sunday as the first day of week 01
    - <b>%%w</b> : the day of the week as a decimal, range 0 to 6, Sunday being 0
    - <b>%%W</b> : the week number of the current year as a decimal number, range 00 to 53,
           starting with  the  first Monday as the first day of week 01
    - <b>%%x</b> : the preferred date representation for the current locale without the time
    - <b>%%X</b> : the preferred time representation for the current locale without the date
    - <b>%%y</b> : the year as a decimal number without a century (range 00 to 99)
    - <b>%%Y</b> : the year as a decimal number including the century
    - <b>%%z</b> : the time-zone as hour offset from GMT.  Required to emit RFC822-conformant
           dates (using "%%a, %%d %%b %%Y %%H:%%M:%%S %%z")
    - <b>%%Z</b> : the time zone or name or abbreviation
    - <b>%%</b> : a literal '%%' character
  */
  //@}

  const nglTime& operator+=(const nglTime& rTime);  ///< Increment operation
  const nglTime& operator+=(double Value);         ///< Increment operation
  const nglTime& operator-=(const nglTime& rTime);  ///< Decrement operation
  const nglTime& operator-=(double Value);         ///< Decrement operation
  operator double() const;      ///< Cast to \e double floating point value

  friend NGL_API nglTime  operator+(const nglTime& rLeft, const nglTime& rRight);
  friend NGL_API nglTime  operator+(const nglTime& rLeft, double Right);
  friend NGL_API nglTime  operator-(const nglTime& rLeft, const nglTime& rRight);
  friend NGL_API nglTime  operator-(const nglTime& rLeft, double Right);

private:
  double mValue;
#ifdef _WIN32_
  static double mLaunchValue;
  static int mLaunchDate;
  static __int64   mTimerFrequency;
  static double GetTime();
  friend class nglTimer;
#endif
};

#endif // __nglTime_h__
