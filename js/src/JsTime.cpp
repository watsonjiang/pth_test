#include "JsTime.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef UNIX
#   include <sys/types.h>
#endif // UNIX

using namespace Js;

typedef struct
{
   const char*  nameOfMonth;
   unsigned int month;        // months since January [0-11]
} MonthInfo;

typedef struct
{
   const char*  nameOfDay;
   TimeType     timeType;
   unsigned int weekDay;      // days since Sunday [0-6]
} WeekDayInfo;

typedef struct
{
   bool isLeapYear;
   long accumulatedDays;
} YearDays;

typedef struct
{
   int daysNormalYear;
   int accumulatedDaysNormalYear;
   int daysLeapYear;
   int accumulatedDaysLeapYear;
} MonthDays;

const MonthInfo MonthInfoC[] =
{
   { "Apr",  3 },
   { "Aug",  7 },
   { "Dec", 11 },
   { "Feb",  1 },
   { "Jan",  0 },
   { "Jul",  6 },
   { "Jun",  5 },
   { "Mar",  2 },
   { "May",  4 },
   { "Nov", 10 },
   { "Oct",  9 },
   { "Sep",  8 },
};

const MonthInfo MonthNamesC[] =
{
   { "Null", 0},   
   { "Jan", 1},
   { "Feb", 2},
   { "Mar", 3},
   { "Apr", 4},
   { "May", 5},
   { "Jun", 6},
   { "Jul", 7},
   { "Aug", 8},
   { "Sep", 9},
   { "Oct", 10},
   { "Nov", 11},
   { "Dec", 12},
};


const WeekDayInfo WeekDayInfoC[] =
{
   { "Fri",       UnknownTimeE, 5 },
   { "Friday",    Rfc850TimeE,  5 },
   { "Mon",       UnknownTimeE, 1 },
   { "Monday",    Rfc850TimeE,  1 },
   { "Sat",       UnknownTimeE, 6 },
   { "Saturday",  Rfc850TimeE,  6 },
   { "Sun",       UnknownTimeE, 0 },
   { "Sunday",    Rfc850TimeE,  0 },
   { "Thu",       UnknownTimeE, 4 },
   { "Thursday",  Rfc850TimeE,  4 },
   { "Tue",       UnknownTimeE, 2 },
   { "Tuesday",   Rfc850TimeE,  2 },
   { "Wed",       UnknownTimeE, 3 },
   { "Wednesday", Rfc850TimeE,  3 },
};

// ---------------------------------------------------------------------------

const YearDays YearDaysC[] =
{
   { false,     0 }, // dummy
   { false,   365 }, // 1970
   { false,   730 }, // 1971
   { true,   1096 }, // 1972 (leap year)
   { false,  1461 }, // 1973
   { false,  1826 }, // 1974
   { false,  2191 }, // 1975
   { true,   2557 }, // 1976 (leap year)
   { false,  2922 }, // 1977
   { false,  3287 }, // 1978
   { false,  3652 }, // 1979
   { true,   4018 }, // 1980 (leap year)
   { false,  4383 }, // 1981
   { false,  4748 }, // 1982
   { false,  5113 }, // 1983
   { true,   5479 }, // 1984 (leap year)
   { false,  5844 }, // 1985
   { false,  6209 }, // 1986
   { false,  6574 }, // 1987
   { true,   6940 }, // 1988 (leap year)
   { false,  7305 }, // 1989
   { false,  7670 }, // 1990
   { false,  8035 }, // 1991
   { true,   8401 }, // 1992 (leap year)
   { false,  8766 }, // 1993
   { false,  9131 }, // 1994
   { false,  9496 }, // 1995
   { true,   9862 }, // 1996 (leap year)
   { false, 10227 }, // 1997
   { false, 10592 }, // 1998
   { false, 10957 }, // 1999
   { true,  11323 }, // 2000 (leap year)
   { false, 11688 }, // 2001
   { false, 12053 }, // 2002
   { false, 12418 }, // 2003
   { true,  12784 }, // 2004 (leap year)
   { false, 13149 }, // 2005
   { false, 13514 }, // 2006
   { false, 13879 }, // 2007
   { true,  14245 }, // 2008 (leap year)
   { false, 14610 }, // 2009
   { false, 14975 }, // 2010
   { false, 15340 }, // 2011
   { true,  15706 }, // 2012 (leap year)
   { false, 16071 }, // 2013
   { false, 16436 }, // 2014
   { false, 16801 }, // 2015
   { true,  17167 }, // 2016 (leap year)
   { false, 17532 }, // 2017
   { false, 17897 }, // 2018
   { false, 18262 }, // 2019
   { true,  18628 }, // 2020 (leap year)
   { false, 18993 }, // 2021
   { false, 19358 }, // 2022
   { false, 19723 }, // 2023
   { true,  20089 }, // 2024 (leap year)
   { false, 20454 }, // 2025
   { false, 20819 }, // 2026
   { false, 21184 }, // 2027
   { true,  21550 }, // 2028 (leap year)
   { false, 21915 }, // 2029
   { false, 22280 }, // 2030
   { false, 22645 }, // 2031
   { true,  23011 }, // 2032 (leap year)
   { false, 23376 }, // 2033
   { false, 23741 }, // 2034
   { false, 24106 }, // 2035
   { true,  24472 }, // 2036 (leap year)
   { false, 24837 }, // 2037
   { false, 25202 }, // 2038
};

// ---------------------------------------------------------------------------

const MonthDays MonthDaysC[] =
{
   {0,    0,  0,   0 }, // dummy
   {31,  31, 31,  31 }, // Jan
   {28,  59, 29,  60 }, // Feb
   {31,  90, 31,  91 }, // Mar
   {30, 120, 30, 121 }, // Apr
   {31, 151, 31, 152 }, // May
   {30, 181, 30, 182 }, // Jun
   {31, 212, 31, 213 }, // Jul
   {31, 243, 31, 244 }, // Aug
   {30, 273, 30, 274 }, // Sep
   {31, 304, 31, 305 }, // Oct
   {30, 334, 30, 335 }, // Nov
   {31, 365, 31, 366 }, // Dec
};

// ---------------------------------------------------------------------------

inline
static
void
gmtime(
   time_t     theTime,
   struct tm& theTimeStruct)
{
   theTimeStruct.tm_sec = theTime % 60;
   theTime /= 60;
   theTimeStruct.tm_min = theTime % 60;
   theTime /= 60;
   theTimeStruct.tm_hour = theTime % 24;
   theTime /= 24;
   theTimeStruct.tm_wday = (theTime + 4) % 7;

   // find year - improvement needed (e.g. binary search)
   for (theTimeStruct.tm_year = 0;
        theTimeStruct.tm_year < (int)sizeof(YearDaysC) / (int)sizeof(YearDays);
        theTimeStruct.tm_year++)
   {
      if (theTime < YearDaysC[theTimeStruct.tm_year].accumulatedDays)
      {
         break;
      }
   }
   theTime -= YearDaysC[theTimeStruct.tm_year - 1].accumulatedDays;
   theTimeStruct.tm_yday = theTime;

   // find month - improvement needed (e.g. binary search)
   for (theTimeStruct.tm_mon = 0;
        theTimeStruct.tm_mon < (int)sizeof(MonthDaysC) / (int)sizeof(MonthDays);
        theTimeStruct.tm_mon++)
   {
      if (YearDaysC[theTimeStruct.tm_year].isLeapYear == false)
      {
         if (theTime < MonthDaysC[theTimeStruct.tm_mon].accumulatedDaysNormalYear)
         {
            theTime -= MonthDaysC[theTimeStruct.tm_mon - 1].accumulatedDaysNormalYear;
            break;
         }
      }
      else
      {
         if (theTime < MonthDaysC[theTimeStruct.tm_mon].accumulatedDaysLeapYear)
         {
            theTime -= MonthDaysC[theTimeStruct.tm_mon - 1].accumulatedDaysLeapYear;
            break;
         }
      }
   }
   theTimeStruct.tm_year += 69;
   theTimeStruct.tm_mon -= 1;
   theTimeStruct.tm_mday = theTime + 1;
   theTimeStruct.tm_isdst = 0;
}

// ---------------------------------------------------------------------------

time_t
Time::mkTime(const struct tm& theTimeStruct)
{
   time_t result;
   int year = theTimeStruct.tm_year - 69;
   int month = theTimeStruct.tm_mon;
   int day = theTimeStruct.tm_mday;
   if (year < 1 || year > 2038 - 1900 - 69)
   {
      return -1;
   }
   result = YearDaysC[year - 1].accumulatedDays;
   if (theTimeStruct.tm_mon < 0 || theTimeStruct.tm_mon > 11)
   {
      return -1;
   }
   if (YearDaysC[year].isLeapYear == false)
   {
      // normal year
      result += MonthDaysC[month].accumulatedDaysNormalYear;
      if (day < 1 ||  day > MonthDaysC[month + 1].daysNormalYear)
      {
         return -1;
      }
      result += day - 1;
   }
   else
   {
      // leap year
      result += MonthDaysC[month].accumulatedDaysLeapYear;
      if (day < 1 || day > MonthDaysC[month + 1].daysLeapYear)
      {
         return -1;
      }
      result += day - 1;
   }
   result *= 24 * 60 * 60;
   result += theTimeStruct.tm_hour * 60 * 60;
   result += theTimeStruct.tm_min * 60;
   result += theTimeStruct.tm_sec;
   return result;
}

// ---------------------------------------------------------------------------

inline
static
void
skipSpaces(const char*& theInput)
{
   // skip spaces
   while (*theInput == ' ' || *theInput == '\t')
   {
      theInput++;
   }
}

// ---------------------------------------------------------------------------

inline
static
void
parseChar(
   const char*& theInput,
   char         theCharacter)
{
   if (*theInput++ != theCharacter)
   {
      throw TimeException("Js::Time::parseChar()");
   }
}
// ---------------------------------------------------------------------------

inline
static
int
parseMonth(
   const char*& theInput)
{
   std::string month;

   // parse the month
   while (isalpha(*theInput))
   {
      month.append(1, *theInput++);
   }

   // find the month
   for (unsigned int i = 0; i < 12; i++)
   {
#ifdef WIN32

      if (!_stricmp(MonthInfoC[i].nameOfMonth, month.c_str()))
      {
         // found
         return MonthInfoC[i].month;
      }

#else // UNIX

      if (!strcasecmp(MonthInfoC[i].nameOfMonth, month.c_str()))
      {
         // found
         return MonthInfoC[i].month;
      }

#endif
   }

   // not found
   throw TimeException("Js::Time::parseMonth()");
}

// ---------------------------------------------------------------------------

inline
static
int
parseWeekDay(const char*& theInput,
             TimeType&    theTimeType)
{
   std::string weekDay;

   // parse the weekday
   while (isalpha(*theInput))
   {
      weekDay.append(1, *theInput++);
   }

   // find the weekday
   for (unsigned int i = 0; i < 14; i++)
   {
#ifdef WIN32

      if (!_stricmp(WeekDayInfoC[i].nameOfDay, weekDay.c_str()))
      {
         // found
         theTimeType = WeekDayInfoC[i].timeType;
         return WeekDayInfoC[i].weekDay;
      }

#else // UNIX

      if (!strcasecmp(WeekDayInfoC[i].nameOfDay, weekDay.c_str()))
      {
         // found
         theTimeType = WeekDayInfoC[i].timeType;
         return WeekDayInfoC[i].weekDay;
      }

#endif
   }

   // not found
   throw TimeException("Js::Time::parseWeekDay()");
   return 0;
}

// ---------------------------------------------------------------------------

inline
static
int
parse1or2Digit(const char*& theInput)
{
   int value;

   if (isdigit(*theInput))
   {
      // first digit
      value = *theInput - '0';
      theInput++;
      if (isdigit(*theInput))
      {
         // second digit
         value = value * 10 + *theInput - '0';
         theInput++;
      }
      return value;
   }
   else
   {
      // error - no digit
      throw TimeException("Js::Time::parse1or2Digit()");
      return 0;
   }
}


// ---------------------------------------------------------------------------

inline
static
int
parse2Digit(const char*& theInput)
{
   int value = 0;
   for (int i = 0; i < 2; i++)
   {
      if (isdigit(*theInput))
      {
         // second digit
         value = value * 10 + *theInput - '0';
         theInput++;
      }
      else
      {
         // error - no digit
         throw TimeException("Js::Time::parse2Digit()");
      }
   }
   return value;
}

// ---------------------------------------------------------------------------

inline
static
int
parse4Digit(const char*& theInput)
{
   int value = 0;
   for (int i = 0; i < 4; i++)
   {
      if (isdigit(*theInput))
      {
         // second digit
         value = value * 10 + *theInput - '0';
         theInput++;
      }
      else
      {
         // error - no digit
         throw TimeException("Js::Time::parse4Digit");
      }
   }
   return value;
}

// ---------------------------------------------------------------------------

inline
static
long
parse9Digit(const char*& theInput)
{
   long value = 0;
   for (int i = 0; i < 9; i++)
   {
      if (isdigit(*theInput))
      {
         // second digit
         value = value * 10 + *theInput - '0';
         theInput++;
      }
      else
      {
         // no digit
         return value;
      }
   }
   return value;
}

// ---------------------------------------------------------------------------

#ifndef WIN32
time_t Time::timeZoneM = 0;
bool   Time::timeZoneSetM = false;

time_t Time::nextZoneUpdateM = 0;
time_t Time::timeZoneDSM = 0;

#endif

int Time::timeZoneHourM = 0;
int Time::timeMinutesM = 0;

// ---------------------------------------------------------------------------

Time::Time(
   std::string& theTimeString,
   unsigned int theParseIndex)
{
   std::string tmpStr;
   bool statusOk = true;
   int  theYear;
   int  theMonth;
   int  theDay;
   int  theHour;
   int  theMinute;
   int  theSecond;

   std::string::iterator Year1It;
   std::string::iterator Year2It;
   std::string::iterator Month1It;
   std::string::iterator Month2It;
   std::string::iterator Day1It;
   std::string::iterator Day2It;
   std::string::iterator Hour1It;
   std::string::iterator Hour2It;
   std::string::iterator Minute1It;
   std::string::iterator Minute2It;
   std::string::iterator Second1It;
   std::string::iterator Second2It;
 
   std::string::iterator it = theTimeString.begin();
   std::string::iterator endIt = theTimeString.end();
   switch (theParseIndex)
   {
      // Rfc1123 time string 
      case 0:
      {
         while ((it != endIt) && ((*it) != ','))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            it++;
         }
         while ((it != endIt) && ((*it) == ' '))
         {
            it++;
         }
         if (it != endIt)
         {
            Day1It = it;
            it++;
         }
         else
         {
            statusOk = false;
         }
         while ((it != endIt) && ((*it) != ' '))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            Day2It = it;
            it++;
         }
         while ((it != endIt) && ((*it) == ' '))
         {
            it++;
         }
         if (it != endIt)
         {
            Month1It = it;
            it++;
         }
         else
         {
            statusOk = false;
         }
         while ((it != endIt) && ((*it) != ' '))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            Month2It = it;
            it++;
         }
         while ((it != endIt) && ((*it) == ' '))
         {
            it++;
         }
         if (it != endIt)
         {
            Year1It = it;
            it++;
         }
         else
         {
            statusOk = false;
         }
         while ((it != endIt) && ((*it) != ' '))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            Year2It = it;
            it++;
         }
         while ((it != endIt) && ((*it) == ' '))
         {
            it++;
         }
         if (it != endIt)
         {
            Hour1It = it;
            it++;
         }
         else
         {
            statusOk = false;
         }
         while ((it != endIt) && ((*it) != ':'))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            Hour2It = it;
            it++;
            if (it != endIt)
            {
               Minute1It = it;
            }
         }
         while ((it != endIt) && ((*it) != ':'))
         {
            it++;
         }
         if (it != endIt)
         {
            Minute2It = it;
            it++;
            if (it != endIt)
            {
               Second1It = it;
            }
         }
         else
         {
            statusOk = false;
         }
         while ((it != endIt) && ((*it) != ' '))
         {
            it++;
         }
         if (it == endIt)
         {
            statusOk = false;
         }
         else
         {
            Second2It = it;
         }
         if (statusOk)
         {
            tmpStr.assign(Year1It,Year2It);
            theYear = atoi(tmpStr.c_str());

            tmpStr.assign(Month1It,Month2It);
            // find the month
            for (unsigned int i = 0; i < 12; i++)
            {
               if (!strcasecmp(MonthInfoC[i].nameOfMonth, tmpStr.c_str()))
               {
                  // found
                  theMonth = MonthInfoC[i].month;
                  break;
               }
            }

            tmpStr.assign(Day1It,Day2It);
            theDay = atoi(tmpStr.c_str());

            tmpStr.assign(Hour1It,Hour2It);
            theHour = atoi(tmpStr.c_str());

            tmpStr.assign(Minute1It,Minute2It);
            theMinute = atoi(tmpStr.c_str());

            tmpStr.assign(Second1It,Second2It);
            theSecond = atoi(tmpStr.c_str());

            struct tm tempTime;
            tempTime.tm_year = theYear - 1900;
            tempTime.tm_mon = theMonth;
            tempTime.tm_mday = theDay;
            tempTime.tm_hour = theHour;
            tempTime.tm_min = theMinute;
            tempTime.tm_sec = theSecond;
            tempTime.tm_isdst = 0;
            tempTime.tm_wday = 0;
            tempTime.tm_yday = 0;
            timeM.tv_sec = mkTime(tempTime);
            timeM.tv_nsec = 0;
            if (timeM.tv_sec == (time_t)-1)
            {
               throw TimeException("Js::Time::Time(theYear....theSeconds)");
            }
         }
         break;
      }
      // Implement any other supported timeStrings here
      // case 1:
      // {
      //    parse code.
      //    break;
      // }
      default:
      {
         statusOk = false;
         break;
      }
   }
   if (!statusOk)
   {
      throw TimeException("Js::Time::Time(std::string& theTimeString)");
   }
}

// ---------------------------------------------------------------------------

Time::Time(
   int  theYear,
   int  theMonth,
   int  theDay,
   int  theHour,
   int  theMinute,
   int  theSecond,
   long theNanoSeconds)
{
   struct tm tempTime;
   tempTime.tm_year = theYear - 1900;
   tempTime.tm_mon = theMonth - 1;
   tempTime.tm_mday = theDay;
   tempTime.tm_hour = theHour;
   tempTime.tm_min = theMinute;
   tempTime.tm_sec = theSecond;
   tempTime.tm_isdst = 0;
   tempTime.tm_wday = 0;
   tempTime.tm_yday = 0;
   timeM.tv_sec = mkTime(tempTime);
   timeM.tv_nsec = theNanoSeconds;
   if (timeM.tv_sec == (time_t)-1)
   {
      throw TimeException("Js::Time::Time()");
   }
}

// ---------------------------------------------------------------------------

void
Time::getComponents(
   int&  theYear,
   int&  theMonth,
   int&  theDay,
   int&  theHour,
   int&  theMinute,
   int&  theSecond,
   long& theNanoSeconds) const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);
   theYear = timeStruct.tm_year + 1900;
   theMonth = timeStruct.tm_mon + 1;
   theDay = timeStruct.tm_mday;
   theHour = timeStruct.tm_hour;
   theMinute = timeStruct.tm_min;
   theSecond = timeStruct.tm_sec;
   theNanoSeconds = timeM.tv_nsec;
}

// ---------------------------------------------------------------------------

int
Time::getHour() const
{
   return ((timeM.tv_sec / 3600) % 24);
}

int
Time::getMinute() const
{
   return ((timeM.tv_sec / 60) % 60);
}

// ---------------------------------------------------------------------------

Time
Time::getLocalTime()
{
   Time currentTime;

#ifdef WIN32
   SYSTEMTIME localTime;
   GetLocalTime(&localTime);
   FILETIME filetime;
   SystemTimeToFileTime(&localTime, &filetime);
   INT64 ntTime = filetime.dwHighDateTime;
   ntTime <<= 32;
   ntTime += filetime.dwLowDateTime;
   currentTime.timeM.tv_sec = (ntTime / 10000000) - 11644473600L;
   currentTime.timeM.tv_nsec = (ntTime % 10000000) * 100;
#else // UNIX
   clock_gettime(CLOCK_REALTIME, &currentTime.timeM);

   if (timeZoneSetM == false)
   {
      time_t tempTime = currentTime.timeM.tv_sec;

      struct tm gt;
      gmtime_r(&tempTime, &gt);
      
      struct tm lt;
      localtime_r(&tempTime, &lt);

      timeZoneM = mkTime(gt) - mkTime(lt);
      timeZoneSetM = true;
   }

   currentTime.timeM.tv_sec -= timeZoneM;
#endif
   return currentTime;
}

// ---------------------------------------------------------------------------

Time
Time::getLocalDSTime()
{
   Time currentTime;

#ifdef WIN32
   SYSTEMTIME localTime;
   GetLocalTime(&localTime);
   FILETIME filetime;
   SystemTimeToFileTime(&localTime, &filetime);
   INT64 ntTime = filetime.dwHighDateTime;
   ntTime <<= 32;
   ntTime += filetime.dwLowDateTime;
   currentTime.timeM.tv_sec = (ntTime / 10000000) - 11644473600L;
   currentTime.timeM.tv_nsec = (ntTime % 10000000) * 100;
#else // UNIX
   clock_gettime(CLOCK_REALTIME, &currentTime.timeM);

   {
      if (currentTime.timeM.tv_sec > nextZoneUpdateM)
      {
         time_t tempTime = currentTime.timeM.tv_sec;

         struct tm gt;
         gmtime_r(&tempTime, &gt);

         struct tm lt;
         localtime_r(&tempTime, &lt);

         timeZoneDSM = mkTime(gt) - mkTime(lt);
         nextZoneUpdateM = currentTime.timeM.tv_sec + 10;
      }

      currentTime.timeM.tv_sec -= timeZoneDSM;
   }
#endif
   return currentTime;
}

// ----------------------------------------------------------------------------

time_t
Time::getTimeZoneDS()
{
#ifdef WIN32
#error Not Yet implemented for WIN32
#else // UNIX
   struct timespec currentTime;
   clock_gettime(CLOCK_REALTIME, &currentTime);

   {
      if (currentTime.tv_sec > nextZoneUpdateM)
      {
         time_t tempTime = currentTime.tv_sec;

         struct tm gt;
         gmtime_r(&tempTime, &gt);

         struct tm lt;
         localtime_r(&tempTime, &lt);

         timeZoneDSM = mkTime(gt) - mkTime(lt);
         nextZoneUpdateM = currentTime.tv_sec + 10;
      }

      return timeZoneDSM;
   }
#endif
}

// ----------------------------------------------------------------------------

Time
Time::getUtcTime()
{
   Time currentTime;

#ifdef WIN32
   FILETIME filetime;
   GetSystemTimeAsFileTime(&filetime);
   INT64 ntTime = filetime.dwHighDateTime;
   ntTime <<= 32;
   ntTime += filetime.dwLowDateTime;
   currentTime.timeM.tv_sec = (ntTime / 10000000) - 11644473600L;
   currentTime.timeM.tv_nsec = (ntTime % 10000000) * 100;
#else // WIN32
   clock_gettime(CLOCK_REALTIME, &currentTime.timeM);
#endif // UNIX
   return currentTime;
}

// ---------------------------------------------------------------------------

void
Time::setComponents(
   int  theYear,
   int  theMonth,
   int  theDay,
   int  theHour,
   int  theMinute,
   int  theSecond,
   long theNanoSeconds)
{
   struct tm tempTime;
   tempTime.tm_year = theYear - 1900;
   tempTime.tm_mon = theMonth - 1;
   tempTime.tm_mday = theDay;
   tempTime.tm_hour = theHour;
   tempTime.tm_min = theMinute;
   tempTime.tm_sec = theSecond;
   tempTime.tm_isdst = 0;
   tempTime.tm_wday = 0;
   tempTime.tm_yday = 0;
   timeM.tv_sec = mkTime(tempTime);
   timeM.tv_nsec = theNanoSeconds;
   if (timeM.tv_sec == (time_t)-1)
   {
      throw TimeException("Js::Time::Time()");
   }
}


std::string
Time::getApacheTime()
{

   Time currentTime;
   std::string timeString;

#ifdef WIN32
   SYSTEMTIME localTime;
   GetLocalTime(&localTime);
   FILETIME filetime;
   SystemTimeToFileTime(&localTime, &filetime);
   INT64 ntTime = filetime.dwHighDateTime;
   ntTime <<= 32;
   ntTime += filetime.dwLowDateTime;
   currentTime.timeM.tv_sec = (ntTime / 10000000) - 11644473600L;
   currentTime.timeM.tv_nsec = (ntTime % 10000000) * 100;
#else // UNIX
   clock_gettime(CLOCK_REALTIME, &currentTime.timeM);

   {
      if (currentTime.timeM.tv_sec > nextZoneUpdateM)
      {
         time_t tempTime = currentTime.timeM.tv_sec;

         struct tm gt;
         gmtime_r(&tempTime, &gt);

         struct tm lt;
         localtime_r(&tempTime, &lt);

         timeZoneDSM = mkTime(gt) - mkTime(lt);
         nextZoneUpdateM = currentTime.timeM.tv_sec + 10;

         int timeZoneSeconds = -timeZoneDSM;
         timeZoneHourM = timeZoneSeconds/3600;
         timeMinutesM = (timeZoneSeconds - (timeZoneHourM * 3600))/60;
      }

      currentTime.timeM.tv_sec -= timeZoneDSM;
      
   }
#endif
    
   struct tm timeStruct;
   gmtime(currentTime.timeM.tv_sec, timeStruct);
   int theYear = timeStruct.tm_year + 1900;
   int theMonth = timeStruct.tm_mon + 1;
   int theDay = timeStruct.tm_mday;
   int theHour = timeStruct.tm_hour;
   int theMinute = timeStruct.tm_min;
   int theSecond = timeStruct.tm_sec;

   char buf[10] = {0};
   snprintf(buf, 10, "%02d", theDay);
   timeString.append(buf);
   //timeString.appendNumber(theDay,10,2,true);
   timeString.append("/");
   timeString.append(MonthNamesC[theMonth].nameOfMonth);
   timeString.append("/");
   snprintf(buf, 10, "%d", theYear);
   timeString.append(buf);
   //timeString.appendNumber(theYear);
   timeString.append(":");
   snprintf(buf, 10, "%02d", theHour);
   //timeString.appendNumber(theHour,10,2,true);
   timeString.append(buf);
   timeString.append(":");
   snprintf(buf, 10, "%02d", theMinute);
   //timeString.appendNumber(theMinute,10,2,true);
   timeString.append(buf);
   timeString.append(":");
   snprintf(buf, 10, "%02d", theSecond);
   //timeString.appendNumber(theSecond,10,2,true);
   timeString.append(buf);
   timeString.append(" ");

   if (timeZoneHourM >= 0)
   {
      timeString.append("+");
      snprintf(buf, 10, "%02d", timeZoneHourM);
      timeString.append(buf);
      //timeString.appendNumber(timeZoneHourM,10,2,true);
      snprintf(buf, 10, "%02d", timeMinutesM);
      timeString.append(buf);
      //timeString.appendNumber(timeMinutesM,10,2,true);
   }
   else
   {
      timeString.append("-");
      snprintf(buf, 10, "%02d", -timeZoneHourM);
      timeString.append(buf);
      //timeString.appendNumber(-timeZoneHourM,10,2,true);
      snprintf(buf, 10, "%02d", -timeMinutesM);
      timeString.append(buf);
      //timeString.appendNumber(-timeMinutesM,10,2,true);
   }

   return timeString;
}

// ---------------------------------------------------------------------------

std::string
Time::toAsctimeString() const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);

   char buffer[64];
   strftime(buffer,
            sizeof(buffer),
            "%a %b %d %H:%M:%S %Y",
            &timeStruct);
   return std::string(buffer);
}

// ---------------------------------------------------------------------------

std::string
Time::toIso8601() const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);

   char buffer[64];
   strftime(buffer,
            sizeof(buffer),
            "%Y-%m-%dT%H:%M:%SZ",
            &timeStruct);
   return std::string(buffer);
}

// ---------------------------------------------------------------------------

Time
Time::toLocalTime() const
{
   Time result(timeM.tv_sec, timeM.tv_nsec);

#ifdef WIN32

   TIME_ZONE_INFORMATION timeZoneInformation;
   memset(&timeZoneInformation, 0, sizeof(timeZoneInformation));
   if (GetTimeZoneInformation(&timeZoneInformation) != TIME_ZONE_ID_INVALID)
   {
      result.timeM.tv_sec -= timeZoneInformation.Bias * 60;
   }

#else // UNIX

   if (timeZoneSetM == false)
   {
      time_t tempTime = result.timeM.tv_sec;
      struct tm gt;
      gmtime_r(&tempTime, &gt);
      
      struct tm lt;
      localtime_r(&tempTime, &lt);

      timeZoneM = mkTime(gt) - mkTime(lt);
      timeZoneSetM = true;
   }

   result.timeM.tv_sec -= timeZoneM;
#endif

   return result;
}

// ---------------------------------------------------------------------------

Time
Time::toLocalDSTime() const
{
   Time result(timeM.tv_sec, timeM.tv_nsec);

#ifdef WIN32

   TIME_ZONE_INFORMATION timeZoneInformation;
   memset(&timeZoneInformation, 0, sizeof(timeZoneInformation));
   if (GetTimeZoneInformation(&timeZoneInformation) != TIME_ZONE_ID_INVALID)
   {
      result.timeM.tv_sec -= timeZoneInformation.Bias * 60;
   }

#else // UNIX

   {
      if (result.timeM.tv_sec > nextZoneUpdateM)
      {
         time_t tempTime = result.timeM.tv_sec;
         struct tm gt;
         gmtime_r(&tempTime, &gt);
         
         struct tm lt;
         localtime_r(&tempTime, &lt);

         timeZoneDSM = mkTime(gt) - mkTime(lt);
         nextZoneUpdateM = result.timeM.tv_sec + 10;
      }

      result.timeM.tv_sec -= timeZoneDSM;
   }

#endif

   return result;
}

// ---------------------------------------------------------------------------

std::string
Time::toRfc1123String() const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);

   char buffer[64];
   strftime(buffer,
            sizeof(buffer),
            "%a, %d %b %Y %H:%M:%S GMT",
            &timeStruct);
   return std::string(buffer);
}

// ---------------------------------------------------------------------------

std::string
Time::toRfc850String() const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);

   char buffer[64];
   strftime(buffer,
            sizeof(buffer),
            "%A, %d-%b-%y %H:%M:%S GMT",
            &timeStruct);
   return std::string(buffer);
}

// ---------------------------------------------------------------------------

std::string
Time::toString() const
{
   struct tm timeStruct;
   gmtime(timeM.tv_sec, timeStruct);

   char buffer[128];
/*
  strftime(buffer,
            sizeof(buffer),
            "%Y-%m-%d %H:%M:%S",
            &timeStruct);
*/
   sprintf(buffer,
           "%4d-%02d-%02d %02d:%02d:%02d.%09ld",
           timeStruct.tm_year + 1900,
           timeStruct.tm_mon + 1,
           timeStruct.tm_mday,
           timeStruct.tm_hour,
           timeStruct.tm_min,
           timeStruct.tm_sec,
           timeM.tv_nsec);
/*
  char nsbuffer[16];
   sprintf(nsbuffer, ".%09d", timeM.tv_nsec);
   strcat(buffer, nsbuffer);
*/
   return std::string(buffer);
}

// ---------------------------------------------------------------------------

Time
Time::toUtcTime() const
{
   Time result(timeM.tv_sec, timeM.tv_nsec);

#ifdef WIN32

   TIME_ZONE_INFORMATION timeZoneInformation;
   memset(&timeZoneInformation, 0, sizeof(timeZoneInformation));
   if (GetTimeZoneInformation(&timeZoneInformation) != TIME_ZONE_ID_INVALID)
   {
      result.timeM.tv_sec += timeZoneInformation.Bias * 60;
   }

#else // UNIX

   if (timeZoneSetM == false)
   {
      time_t tempTime = result.timeM.tv_sec;
      struct tm gt;
      gmtime_r(&tempTime, &gt);

      struct tm lt;
      localtime_r(&tempTime, &lt);

      timeZoneM = mkTime(gt) - mkTime(lt);
      timeZoneSetM = true;
   }

   result.timeM.tv_sec += timeZoneM;
#endif

   return result;
}

// ---------------------------------------------------------------------------

TimeType
Time::parse(
   const char* theTimeAsString)
{
   timeM.tv_sec = 0;
   timeM.tv_nsec = 0;

   TimeType     timeType;
   struct tm    tempTime;
   memset(&tempTime, 0, sizeof(tempTime));

   if (isdigit(*theTimeAsString))
   {
      timeType = InternalTimeE;
      int len = strlen(theTimeAsString);
      char theLastChar = theTimeAsString[ len - 1 ];

      if( theLastChar = 'Z')
      {
    	  timeType = Iso8601;
      }
   }
   else
   {
      // parse the weekday
      tempTime.tm_wday = parseWeekDay(theTimeAsString, timeType);

      skipSpaces(theTimeAsString);

      if (*theTimeAsString == ',')
      {
         theTimeAsString++;
         if (timeType == UnknownTimeE)
         {
            // short weekday an ',' - RFC 1123 data
            timeType = Rfc1123TimeE;
         }
      }
      else
      {
         if (timeType == UnknownTimeE)
         {
            // short weekday and no ',' - asctime date
            timeType = AsctimeTimeE;
         }
         else
         {
            // error - missing ',' after RFC 850 date
            throw TimeException(
               "Js::Time::parse() - missing ',' after RFC 850 date");
         }
      }
   }

   switch (timeType)
   {
      case Rfc1123TimeE:
      {
         // wkday "," <?> SP (2DIGIT | 1DIGIT) SP month SP 4DIGIT SP time SP "GMT"
         skipSpaces(theTimeAsString);

         // day
         tempTime.tm_mday = parse1or2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // month
         tempTime.tm_mon = parseMonth(theTimeAsString);

         skipSpaces(theTimeAsString);

         // year
         tempTime.tm_year = parse4Digit(theTimeAsString);
         tempTime.tm_year -= 1900;

         skipSpaces(theTimeAsString);

         // hour
         tempTime.tm_hour = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // minute
         tempTime.tm_min = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // second
         tempTime.tm_sec = parse2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // GMT
         parseChar(theTimeAsString, 'G');
         parseChar(theTimeAsString, 'M');
         parseChar(theTimeAsString, 'T');
         break;
      }

      // ---------------------------------------------------------------------

      case Rfc850TimeE:
      {
         // weekday "," <?> SP 2DIGIT-month-2DIGIT SP time SP "GMT"
         skipSpaces(theTimeAsString);

         // day
         tempTime.tm_mday = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // month
         tempTime.tm_mon = parseMonth(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // year
         tempTime.tm_year = parse2Digit(theTimeAsString);
         tempTime.tm_year += 2000;
         if (tempTime.tm_year > 2050)
         {
            tempTime.tm_year -= 100;
         }
         tempTime.tm_year -= 1900;

         skipSpaces(theTimeAsString);

         // hour
         tempTime.tm_hour = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // minute
         tempTime.tm_min = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // second
         tempTime.tm_sec = parse2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // GMT
         parseChar(theTimeAsString, 'G');
         parseChar(theTimeAsString, 'M');
         parseChar(theTimeAsString, 'T');
         break;
      }

      // ---------------------------------------------------------------------

      case AsctimeTimeE:
      {
         // wkday SP <?> month SP (2DIGIT | SP (1DIGIT)) SP time SP 4DIGIT

         // month
         tempTime.tm_mon = parseMonth(theTimeAsString);

         skipSpaces(theTimeAsString);

         // day
         tempTime.tm_mday = parse1or2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // hour
         tempTime.tm_hour = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // minute
         tempTime.tm_min = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // second
         tempTime.tm_sec = parse2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // year
         tempTime.tm_year  = parse4Digit(theTimeAsString);
         tempTime.tm_year -= 1900;
         break;
      }

      // ---------------------------------------------------------------------

      case InternalTimeE:
      {
         // 4DIGIT-2DIGIT-2DIGIT SP time . 9DIGIT

         // year
         tempTime.tm_year  = parse4Digit(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // month
         tempTime.tm_mon = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // day
         tempTime.tm_mday = parse2Digit(theTimeAsString);

         skipSpaces(theTimeAsString);

         // hour
         tempTime.tm_hour = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // minute
         tempTime.tm_min = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // second
         tempTime.tm_sec = parse2Digit(theTimeAsString);

         if (*theTimeAsString == '.')
         {
            theTimeAsString++;
            timeM.tv_nsec = parse9Digit(theTimeAsString);
         }
         tempTime.tm_mon -= 1;
         tempTime.tm_year -= 1900;
         break;
      }
      // ---------------------------------------------------------------------

      case Iso8601:
      {
         // 4DIGIT-2DIGIT-2DIGIT SP time Z

         // year
         tempTime.tm_year  = parse4Digit(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // month
         tempTime.tm_mon = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, '-');

         // day
         tempTime.tm_mday = parse2Digit(theTimeAsString);

         // A separating 'T' move to the next char.
         parseChar(theTimeAsString, 'T');

         // hour
         tempTime.tm_hour = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // minute
         tempTime.tm_min = parse2Digit(theTimeAsString);

         parseChar(theTimeAsString, ':');

         // second
         tempTime.tm_sec = parse2Digit(theTimeAsString);

         tempTime.tm_mon -= 1;
         tempTime.tm_year -= 1900;

         break;
      }


      default:
      {
         // empty
      }
   }
   timeM.tv_sec = mkTime(tempTime);
   if (timeM.tv_sec == -1)
   {
      throw TimeException("Js::Time::parse()");
   }
   return timeType;
}

