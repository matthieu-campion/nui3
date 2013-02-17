/*
 NUI3 - C++ cross-platform GUI framework for OpenGL based applications
 Copyright (C) 2002-2003 Sebastien Metrot & Vincent Caron
 
 licence: see nui3/LICENCE.TXT
 */



#include "nui.h"

#if defined(_UNIX_) || (defined _CARBON_ && !macintosh) || defined(_CORE_FOUNDATION_)
#define _POSIX_WORLD_
#endif

#ifdef _POSIX_WORLD_
#  include <sys/time.h>
#elif macintosh
#  include <time.h>
#endif // _POSIX_WORLD_

const nglTime nglTime::Zero   = 0;
const nglTime nglTime::Second = 1;
const nglTime nglTime::Minute = 60;
const nglTime nglTime::Hour   = 3600;
const nglTime nglTime::Day    = 86400;
const nglTime nglTime::Week   = 604800;
const nglTime nglTime::Year   = 31557600;

#ifdef _WIN32_
__int64 nglTime::mLaunchValue = 0;
int64 nglTime::mLaunchDate = 0;
#endif // _WIN32_


/*
 * nglTimeInfo
 */

void nglTimeInfo::CopyFromSys (const struct tm* pTM)
{
  Seconds = pTM->tm_sec;
  Minutes = pTM->tm_min;
  Hours   = pTM->tm_hour;
  Day     = pTM->tm_mday;
  Month   = pTM->tm_mon+1; // tm_mon is [0..11]. Month is [1..12]
  Year    = pTM->tm_year;
  WeekDay = pTM->tm_wday;
  DST     = pTM->tm_isdst;
}

void nglTimeInfo::CopyToSys (struct tm* pTM) const
{
  pTM->tm_sec   = Seconds;
  pTM->tm_min   = Minutes;
  pTM->tm_hour  = Hours;
  pTM->tm_mday  = Day;
  pTM->tm_mon   = Month-1; // tm_mon is [0..11]. Month is [1..12]
  pTM->tm_year  = Year;
  pTM->tm_wday  = WeekDay;
  pTM->tm_isdst = DST;
}


/*
 * (nglTime) Life cycle
 */

nglTime::nglTime()
{
#ifdef _POSIX_WORLD_
  struct timeval tv;

  gettimeofday (&tv, NULL);
  mValue = (double)tv.tv_sec + ((double)tv.tv_usec / 1000000.0);
#endif // _POSIX_WORLD_

#if macintosh 
  Nanoseconds t = AbsoluteToNanoseconds(UpTime());
  mValue = ((double)UnsignedWideToUInt64(t)) / 1000000.0;
#endif // macintosh

#ifdef _WIN32_
  mValue = GetTime();
#endif // _WIN32_
}

nglTime::nglTime(double Value)
{
  mValue = Value;
}

nglTime::nglTime(const nglTimeInfo& rInfo)
{
  struct tm tinfo;

  rInfo.CopyToSys(&tinfo);
  mValue = mktime (&tinfo);
}

nglTime::nglTime(const nglTime& rTime)
{
  mValue = rTime.mValue;
}


/*
 * Retrieve time & date
 */

double nglTime::GetValue()
{
  return mValue;
}

bool nglTime::GetGMTime (nglTimeInfo& rInfo)
{
  struct tm* tinfo;
  time_t tstamp = (time_t)mValue;

  tinfo = gmtime (&tstamp);
  if (!tinfo)
     return false;

  rInfo.CopyFromSys(tinfo);
  return true;
}

bool nglTime::GetLocalTime (nglTimeInfo& rInfo)
{
  struct tm* tinfo;
  time_t tstamp = (time_t)mValue;

  tinfo = localtime (&tstamp);
  if (!tinfo)
   return false;

  rInfo.CopyFromSys(tinfo);
  return true;
}

#define BUFFER_SIZE 1024

nglString nglTime::GetGMTimeStr (const char* pFormat)
{
  char buffer[1024+1];
  struct tm* tinfo;
  time_t tstamp = (time_t)mValue;

  if (!(tinfo = gmtime (&tstamp)) ||
      !strftime (buffer, 1024, pFormat, tinfo))
    return nglString::Empty;
  buffer[BUFFER_SIZE] = 0;

  return nglString(buffer);
}

nglString nglTime::GetLocalTimeStr (const char* pFormat)
{
  char buffer[1024+1];
  struct tm* tinfo;
  time_t tstamp = (time_t)mValue;

  if (!(tinfo = localtime (&tstamp)) ||
      !strftime (buffer, 1024, pFormat, tinfo))
    return nglString::Empty;
  buffer[BUFFER_SIZE] = 0;

  return nglString(buffer);
}


/*
 * Operators
 */

const nglTime& nglTime::operator+=(const nglTime& rTime)
{
  mValue += rTime.mValue;
  return *this;
}

const nglTime& nglTime::operator+=(double Value)
{
  mValue += Value;
  return *this;
}

const nglTime& nglTime::operator-=(const nglTime& rTime)
{
  mValue -= rTime.mValue;
  return *this;
}

const nglTime& nglTime::operator-=(double Value)
{
  mValue -= Value;
  return *this;
}

nglTime::operator double() const
{
  return mValue;
}

NGL_API nglTime operator+(const nglTime& rLeft, const nglTime& rRight)
{
  nglTime result = rLeft;
  result += rRight;
  return result;
}

NGL_API nglTime operator+(const nglTime& rLeft, double Right)
{
  nglTime result = rLeft;
  result += Right;
  return result;
}

NGL_API nglTime operator-(const nglTime& rLeft, const nglTime& rRight)
{
  nglTime result = rLeft;
  result -= rRight;
  return result;
}

NGL_API nglTime operator-(const nglTime& rLeft, double Right)
{
  nglTime result = rLeft;
  result -= Right;
  return result;
}

#ifdef _WIN32_
__int64 nglTime::mTimerFrequency = 0;

double nglTime::GetTime()
{
  double t;

  if (!mLaunchDate)
  {
    if (!QueryPerformanceFrequency((LARGE_INTEGER *)&mTimerFrequency))
      mTimerFrequency = 0;

    if (mTimerFrequency)
      QueryPerformanceCounter((LARGE_INTEGER *)&mLaunchValue);
    else
      mLaunchValue = ((double)timeGetTime()) / 1000.0;

    mLaunchDate = time(NULL);
  }

  if (mTimerFrequency != 0)
  {
    __int64 tt;
    QueryPerformanceCounter((LARGE_INTEGER *)&tt);

    tt -= mLaunchValue;
    
    t = (double)tt;
    t /= mTimerFrequency;
  }
  else
  {
    t = ((double)timeGetTime()) / 1000.0;
  }
  t += mLaunchDate;
  return t;
}

#endif // _WIN32_
