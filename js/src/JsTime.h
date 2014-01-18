#ifndef PALTIME_H
#define PALTIME_H

#ifdef WIN32

#   include "JsWinInclude.h"

#endif

#include <time.h>

#include "JsTimeException.h"

namespace Js
{
   struct pal_timespec
   {
      time_t tv_sec;
      long   tv_nsec;
   };

   typedef enum
   {
      AsctimeTimeE,
      Rfc1123TimeE,
      Rfc850TimeE,
      InternalTimeE,
      Iso8601,
      UnknownTimeE
   } TimeType;

   /** \class Time Js.h "Js.h"
	 * \ingroup PALTIME
    * The class Time is a container for a time value.
    * All times are measured in seconds elapsed since
    * midnight (00:00:00), January 1, 1970, Coordinated Universal Time,
    * according to the system clock.
    * @author ERATHH EIP/CN/PDA Thorsten Herber.
    * @version PA1
    */
   class Time
   {
   public:

      /**
       * Default constructor.
       */
      Time();

      /**
       * Constructor.
       * @param theSeconds The number of seconds since 1970-01-01 00:00:00.
       * @param theNanoSeconds The nano seconds.
       */
      Time(
         time_t theSeconds,
         long   theNanoSeconds = 0);

      /**
       * Constructor.
       * @param theYear   The year (1970..2038).
       * @param theMonth  The month (1..12).
       * @param theDay    The day (1..31).
       * @param theHour   The hour (0..24).
       * @param theMinute The minute (0..59).
       * @param theSecond The second (0..59).
       * @param theNanoSeconds The nano seconds.
       */
      Time(
         int  theYear,
         int  theMonth,
         int  theDay,
         int  theHour,
         int  theMinute,
         int  theSecond,
         long theNanoSeconds = 0);

      /**
       * Constructor.
       * @param Time string that shall be parsed
       * @param unsigned int switch varible
       *        value = 0   (RFC1123 string parsing)
       *        Other values presently not supported.
       */
      Time(
         std::string& theTimeString,
         unsigned int theParseIndex);

      /**
       * Copy constructor.
       * @param theOther The time to be copied.
       */
      Time(const Time& theOther);

      /**
       * Assignment operator.
       * @param theOther The new time.
       * @return The new time.
       */
      Time&
      operator=(const Time& theOther);

      /**
       * Equals operator.
       * @param theOther The time to compare with.
       * @return 'true' if time is equal to other time,
       *         'false' otherwise.
       */
      bool
      operator==(
         const Time& theOther) const;

      /**
       * Not equals operator.
       * @param theOther The time to compare with.
       * @return 'true' if time is not equal to other time,
       *         'false' otherwise.
       */
      bool
      operator!=(
         const Time& theOther) const;

      /**
       * Less than operator.
       * @param theOther The time to compare with.
       * @return 'true' if time is less than other time,
       *         'false' otherwise.
       */
      bool
      operator<(
         const Time& theOther) const;

      /**
       * Less or equal operator.
       * @param theOther The time to compare with.
       * @return 'true' if time is less than or equal to other time,
       *         'false' otherwise.
       */
      bool
      operator<=(
         const Time& theOther) const;

      /**
       * Greater than operator.
       * @param theOther The time to compare with.
       * @return 'true' if time greater than other time,
       *         'false' otherwise.
       */
      bool
      operator>(
         const Time& theOther) const;

      /**
       * Greater or equal operator.
       * @param theOther The time to compare with.
       * @return 'true' if time is greater than or equal to other time,
       *         'false' otherwise.
       */
      bool
      operator>=(
         const Time& theOther) const;

      /**
       * Subtracts a time from this time.
       * @param theOther The time to subtract.
       * @return The new time.
       */
      Time&
      operator-=(
         const Time& theOther);

      /**
       * Adds another time from this time.
       * @param theOther The time to add.
       * @return The new time.
       */
      Time&
      operator+=(
         const Time& theOther);

      /**
       * Multiplies this time by a value.
       * @param theValue The value.
       * @return The new time.
       */
      Time&
      operator*=(
         unsigned int theValue);

      /**
       * Divides this time by a value.
       * @param theValue The value.
       * @return The new time.
       */
      Time&
      operator/=(
         unsigned int theValue);

      /**
       * Returns the components (year, month, day, ..) of the time.
       * @param theYear   The year (1970..2038).
       * @param theMonth  The month (1..12).
       * @param theDay    The day (1..31).
       * @param theHour   The hour (0..24).
       * @param theMinute The minute (0..59).
       * @param theSecond The second (0..59).
       * @param theNanoSeconds The nano seconds.
       */
      void
      getComponents(
         int&  theYear,
         int&  theMonth,
         int&  theDay,
         int&  theHour,
         int&  theMinute,
         int&  theSecond,
         long& theNanoSeconds) const;


      static std::string
      getApacheTime();

      /**
       * Returns the current local time.
       * @return The current local time.
       */
      static
      Time
      getLocalTime();

      /**
       * Returns the current local time with updated current daylight saving time.
       * @return The current local time with updated current daylight saving time.
       */
      static
      Time
      getLocalDSTime();

      /**
       * Returns the number of nano seconds.
       * @return The number of nano seconds.
       */
      long
      getNanoSeconds() const;

      /**
       * Returns the number of seconds since 1970-01-01 00:00:00.
       * @return The number of seconds.
       */
      time_t
      getSeconds() const;

      int
      getHour() const;

      int
      getMinute() const;

      /**
       * Returns the difference between the local time and UTC time in seconds.
       * @return The time difference.
       */
      static
      time_t
      getTimeZoneDS();
      
      /**
       * Returns the current Coordinated Universal Time (UTC) time.
       * @return The current UTC time.
       */
      static
      Time
      getUtcTime();

      /**
       * Parses the string and sets the time to the corresponding time.
       * The string must be in asctime(), RFC 1123, RFC 850 or internal
       * format.
       * The methods throws a TimeException if the string does not
       * contain a valid time.
       * @return AsctimeTimeE if the string is in asctime() format.
       *         Rfc1123TimeE if the string is in RFC 1123 time format.
       *         Rfc850TimeE  if the string is in RFC 850 time format.
       *         InternalTimeE if the string is in the internal time format.
       */
      TimeType
      parse(
         const char* theTimeAsString);

      /**
       * Sets the components (year, month, day, ..) of the time.
       * @param theYear   The year (1970..2038).
       * @param theMonth  The month (1..12).
       * @param theDay    The day (1..31).
       * @param theHour   The hour (0..24).
       * @param theMinute The minute (0..59).
       * @param theSecond The second (0..59).
       * @param theNanoSeconds The nano seconds.
       */
      void
      setComponents(
         int  theYear,
         int  theMonth,
         int  theDay,
         int  theHour,
         int  theMinute,
         int  theSecond,
         long theNanoSeconds = 0);

      /**
       * Returns the time as a string in asctime() format.
       * @return The time as string.
       */
      std::string
      toAsctimeString() const;

      /**
       * Returns the time as a string in ISO8601 format.
       * @return The time as string.
       */
      std::string
      toIso8601() const;

      /**
       * Converts the UTC time to a local time.
       * @return UTC time converted to local time.
       */
      Time
      toLocalTime() const;

      /**
       * Converts the UTC time to a local time with updated current daylight saving time.
       * @return UTC time converted to local time.
       */
      Time
      toLocalDSTime() const;

      /**
       * Returns the time as a string in RFC 1123 format.
       * @return The time as string.
       */
      std::string
      toRfc1123String() const;

      /**
       * Returns the time as a string in RFC 850 format.
       * @return The time as string.
       */
      std::string
      toRfc850String() const;
      
      /**
       * Returns the time as a string in internal format
       * (YYYY-MM-DD hh:mm:ss.nnnnnnnnn).
       * @return The time as string.
       */
      std::string
      toString() const;

      /**
       * Converts the local time to a UTC time.
       * @return Local time converted to UTC time.
       */
      Time
      toUtcTime() const;

      /**
       * Subtracts two times.
       * @param theLhs The left-hand-side time.
       * @param theRhs The right-hand-side time.
       * @return theLhs - theRhs.
       */
      friend
      Time
      operator-(
         const Time& theLhs,
         const Time& theRhs);

      /**
       * Adds two times.
       * @param theLhs The left-hand-side time.
       * @param theRhs The right-hand-side time.
       * @return theLhs + theRhs.
       */
      friend
      Time
      operator+(
         const Time& theLhs,
         const Time& theRhs);

      /**
       * Multiplies a time by a value.
       * @param  theTime The time.
       * @param  theValue The value.
       * @return theTime * theValue.
       */
      friend
      Time
      operator*(
         const Time&  theTime,
         unsigned int theValue);

      /**
       * Divides a time by a value.
       * @param  theTime The time.
       * @param  theValue The value.
       * @return theTime / theValue.
       */
      friend
      Time
      operator/(
         const Time&  theTime,
         unsigned int theValue);

      static
      time_t
      mkTime(const struct tm& theTimeStruct);

   private:

#ifdef WIN32

      struct pal_timespec timeM;

#else // UNIX

      static time_t   timeZoneM;
      static time_t   nextZoneUpdateM; // used in getLocalDSTime
      static time_t   timeZoneDSM;  // used in getLocalDSTime
      static bool     timeZoneSetM;
      struct timespec timeM;

#endif
      static int timeZoneHourM;
      static int timeMinutesM;
   };

   // ------------------------------------------------------------------------

   inline
   Time::Time()
   {
      timeM.tv_sec = 0;
      timeM.tv_nsec = 0;
   }

   // ------------------------------------------------------------------------

   inline
   Time::Time(
      time_t theSeconds,
      long   theNanoSeconds)
   {
      timeM.tv_sec = theSeconds;
      timeM.tv_nsec = theNanoSeconds;
   }

   // ------------------------------------------------------------------------

   inline
   Time::Time(
      const Time& theOther)
   :  timeM(theOther.timeM)
   {
      // empty
   }

   // ------------------------------------------------------------------------

   inline
   Time&
   Time::operator=(
      const Time& theOther)
   {
      timeM = theOther.timeM;
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator==(
      const Time& theOther) const
   {
      return (timeM.tv_sec == theOther.timeM.tv_sec) &&
             (timeM.tv_nsec == theOther.timeM.tv_nsec);
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator!=(
      const Time& theOther) const
   {
      return (timeM.tv_sec != theOther.timeM.tv_sec) ||
             (timeM.tv_nsec != theOther.timeM.tv_nsec);
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator<(
      const Time& theOther) const
   {
      if (timeM.tv_sec < theOther.timeM.tv_sec)
      {
         return true;
      }
      else
      {
         if (timeM.tv_sec > theOther.timeM.tv_sec)
         {
            return false;
         }
         else
         {
            return timeM.tv_nsec < theOther.timeM.tv_nsec;
         }
      }
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator<=(
      const Time& theOther) const
   {
      if (timeM.tv_sec < theOther.timeM.tv_sec)
      {
         return true;
      }
      else
      {
         if (timeM.tv_sec > theOther.timeM.tv_sec)
         {
            return false;
         }
         else
         {
            return timeM.tv_nsec <= theOther.timeM.tv_nsec;
         }
      }
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator>(
      const Time& theOther) const
   {
      if (timeM.tv_sec > theOther.timeM.tv_sec)
      {
         return true;
      }
      else
      {
         if (timeM.tv_sec < theOther.timeM.tv_sec)
         {
            return false;
         }
         else
         {
            return timeM.tv_nsec > theOther.timeM.tv_nsec;
         }
      }
   }

   // ------------------------------------------------------------------------

   inline
   bool
   Time::operator>=(
      const Time& theOther) const
   {
      if (timeM.tv_sec > theOther.timeM.tv_sec)
      {
         return true;
      }
      else
      {
         if (timeM.tv_sec < theOther.timeM.tv_sec)
         {
            return false;
         }
         else
         {
            return timeM.tv_nsec >= theOther.timeM.tv_nsec;
         }
      }
   }

   // ------------------------------------------------------------------------

   inline
   Time&
   Time::operator-=(
      const Time& theOther)
   {
      timeM.tv_nsec -= theOther.timeM.tv_nsec;
      if (timeM.tv_nsec < 0)
      {
         timeM.tv_nsec += 1000000000L;
         timeM.tv_sec--;
      }
      timeM.tv_sec -= theOther.timeM.tv_sec;
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   Time&
   Time::operator+=(
      const Time& theOther)
   {
      timeM.tv_nsec += theOther.timeM.tv_nsec;
      if (timeM.tv_nsec >= 1000000000L)
      {
         timeM.tv_nsec -= 1000000000L;
         timeM.tv_sec++;
      }
      timeM.tv_sec += theOther.timeM.tv_sec;
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   Time&
   Time::operator*=(
      unsigned int theValue)
   {
      timeM.tv_sec *= theValue;
      //int64_t nanoSeconds = timeM.tv_nsec * theValue;
      long long nanoSeconds = timeM.tv_nsec * theValue;
      timeM.tv_nsec = nanoSeconds % 1000000000L;
      timeM.tv_sec += nanoSeconds / 1000000000L;
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   Time&
   Time::operator/=(
      unsigned int theValue)
   {
      unsigned long remainder = timeM.tv_sec % theValue;
      timeM.tv_sec /= theValue;
      timeM.tv_nsec /= theValue;
      if (remainder > 0)
      {
         timeM.tv_nsec += (1000000000L / theValue) * remainder;
      }
      return *this;
   }

   // ------------------------------------------------------------------------

   inline
   long
   Time::getNanoSeconds() const
   {
      return timeM.tv_nsec;
   }

   // ------------------------------------------------------------------------

   inline
   time_t
   Time::getSeconds() const
   {
      return timeM.tv_sec;
   }

   // ------------------------------------------------------------------------

   inline
   Time
   operator-(
      const Time& theLhs,
      const Time& theRhs)
   {
      Time result(theLhs);
      result -= theRhs;
      return result;
   }

   // ------------------------------------------------------------------------

   inline
   Time
   operator+(
      const Time& theLhs,
      const Time& theRhs)
   {
      Time result(theLhs);
      result += theRhs;
      return result;
   }

   // ------------------------------------------------------------------------

   inline
   Time
   operator*(
      const Time&  theTime,
      unsigned int theValue)
   {
      Time result;
      result.timeM.tv_sec = theTime.timeM.tv_sec * theValue;
      //int64_t nanoSeconds = theTime.timeM.tv_nsec * theValue;
      long long nanoSeconds = theTime.timeM.tv_nsec * theValue;
      result.timeM.tv_nsec = nanoSeconds % 1000000000L;
      result.timeM.tv_sec += nanoSeconds / 1000000000L;
      return result;
   }

   // ------------------------------------------------------------------------

   inline
   Time
   operator/(
      const Time&  theTime,
      unsigned int theValue)
   {
      Time result;
      unsigned long remainder = theTime.timeM.tv_sec % theValue;
      result.timeM.tv_sec = theTime.timeM.tv_sec / theValue;
      result.timeM.tv_nsec = theTime.timeM.tv_nsec / theValue;
      if (remainder > 0)
      {
         result.timeM.tv_nsec += (1000000000L / theValue) * remainder;
      }
      return result;
   }
}

#endif
