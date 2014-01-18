#ifndef PALSCRIPTOBJECTDATE_H
#define PALSCRIPTOBJECTDATE_H

#include "JsScriptObjectExternal.h"
#include "JsSmartPointer.h"
#include "JsTime.h"


namespace Js
{

  /** \class ScriptObjectDate ScriptObjectDate.h "ScriptObjectDate.h"
   * \ingroup <<TODO>>Add software unit name here (e.g. PRPLMULTI)
   * <<TODO>>Briefly describe your class here.
   */
   class ScriptObjectDate : public ScriptObjectExternal
   {
   public:

      /**
       * Default constructor
       */
      ScriptObjectDate();

      ScriptObjectDate(
         Number theTime);
   
      ScriptObjectDate(
         const Time& theTime);

      ScriptObjectDate(
         Number theYear,
         Number theMonth,
         Number theDay,
         Number theHour,
         Number theMinute,
         Number theSecond);

      /**
       * Destructor.
       */
      virtual
      ~ScriptObjectDate();

      // ----------------------------------------------------------------------
      // THE SCRIPT ENGINE INTERFACE:
      // ----------------------------------------------------------------------

      /**
       * The name of this class.
       */
      static 
      const char* 
      ClassNameC;

      /**
       * Create an object.
       * Create a new onstance of the scriptable class.
       * @param theContext   (in) The execution context from which the
       *                          constructor is called.
       * @param theArguments (in) The values of the constructor arguments.
       * @return A pointer to the new object.
       */
      static
      ScriptObject*
      create(ScriptExecutionContext* theContext,
             const ScriptValueArray& theArguments);

      /**
       * Call a function.
       * Call the function 'theName'.
       * @param theName          (in)  The name of the function.
       * @param theContext       (in)  The execution context from which this
       *                               function is called.
       * @param theArguments     (in)  The values of the function arguments.
       * @param theIsConstructor (in)  'true' if the function is used
       *                               as a constructor, 'false' otherwise.
       * @param theResult        (out) The function's return value.
       * @return 'true'  if the function theName exists and can be called, 
       *         'false' otherwise.
       */
      bool
      callExternalMethod(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor,
         ScriptValue&            theResult);

      /**
       * Delete a property.
       * Delete property 'theName'.
       * @param theName (in) The name of the property.
       * @return ScriptObject::UpdatedE: 
       *            The value of the property has been deleted.
       *         ScriptObject::NotUpdatedE:
       *            The property does not exist in this object, but it
       *            might exist in a derived object.
       *         ScriptObject::UpdateNotAllowedE: 
       *            It is not allowed to delete this property.
       */
      ScriptObject::UpdateResult
      deleteExternalProperty(
         const std::string& theName);

      /**
       * Get the name of this class.
       * @return The name of this class.
       */
      const char*
      getClass() const;

      /**
       * Get the value of a property.
       * If property 'theName' exists then store its value in 'theValue'. 
       * @param theName  (in)  The name of the property.
       * @param theValue (out) The current value of the property.
       * @return 'true' if the property exists and can be read,
       *         'false' otherwise.
       */
      bool
      getExternalProperty(
         const std::string& theName,
         ScriptValue&  theValue) const;

      /**
       * Get name of first property.
       * If the object has properties then store the name of the first
       * property in 'theName'.
       * @param theName (out) The name of the first property.
       * @return 'true'  if the object has properties,
       *         'false' otherwise.
       */
      bool
      getNameOfFirstExternalProperty(
         std::string& theName) const;

      /**
       * Get name of next property.
       * Get the name of the property that comes after the propery
       * 'theName' and store it in 'theName'.
       * @param theName (in)  The name of the current property.
       *                (out) The name of the next property.
       * @return 'false' if the current property is the last property,
       *         'true'  otherwise.
       */
      bool
      getNameOfNextExternalProperty(
         std::string& theName) const;

      ScriptValue
      getStringValue() const;

      /**
       * Check if an object has a property.
       * Check if the object has the property 'theName'.
       * @param theName (in) The name of the property.
       * @return 'true' if the property exists, 'false' otherwise.
       */
      bool
      hasExternalProperty(
         const std::string& theName) const;

      /**
       * Check if this object is derived from the class 'theClassName'.
       * @param theClassName (in) The name of the class.
       * @return 'true'  if this object is derived from the the specified class
       *         'false' otherwise.
       */
      bool
      isInstanceOf(
         const std::string& theClassName) const;

      /**
       * Set the value of a property.
       * If property 'theName' exists, set its value to 'theValue'. If
       * the property does not exist, create it and set it's value to 'theValue'.
       * @param theName  (in) The name of the property.
       * @param theValue (in) The new value for the property.
       * @return ScriptObject::UpdatedE: 
       *            The value of the property has been set.
       *         ScriptObject::NotUpdatedE:
       *            The property does not exist in this object, but it
       *            might exist in a derived object.
       *         ScriptObject::UpdateNotAllowedE: 
       *            It is not allowed to set the value of this property or to
       *            create a property with this name.
       */
      ScriptObject::UpdateResult
      setExternalProperty(
         const std::string& theName,
         ScriptValue   theValue);

      // ----------------------------------------------------------------------
      // THE SCRIPTABLE FUNCTIONS AND PROPERTIES:
      // ----------------------------------------------------------------------

      // <<TODO>> Declare the methods, which implement the scriptable 
      //          functions of the class here.
      //          Sort the function names alphabetically.
      //
      // Template:
      //    ScriptValue
      //    scriptCall[FunctionName](
      //       ScriptExecutionContext* theContext, 
      //       const ScriptValueArray& theArguments,
      //       bool                         theIsConstructor);

      ScriptValue
      scriptCallGetDate(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetDay(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetFullYear(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetHours(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetMinutes(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetMonth(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetSeconds(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetTime(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetTimezoneOffset(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);

      ScriptValue
      scriptCallGetUTCDate(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCDay(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCFullYear(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCHours(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCMinutes(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCMonth(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallGetUTCSeconds(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallSetDate(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetFullYear(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetHours(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetMinutes(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetMonth(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetSeconds(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetTime(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor);

      ScriptValue
      scriptCallSetUTCDate(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetUTCFullYear(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetUTCHours(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetUTCMinutes(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetUTCMonth(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallSetUTCSeconds(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToAsctimeString(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToDateString(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToIso8601String(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToRfc1123String(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToRfc850String(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToString(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      ScriptValue
      scriptCallToTimeString(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
   
      // <<TODO>> Declare the methods, which are used to delete  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      // Template:
      //    void
      //    scriptDelete[PropertyName]();

      // <<TODO>> Declare the methods, which are used to get the values of  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      // Template:
      //    ScriptValue
      //    scriptGet[PropertyName]() const;


      // <<TODO>> Declare the methods, which are used to set the values of  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      //    void
      //    scriptSet[PropertyName](
      //       ScriptValue theValue);

      // ----------------------------------------------------------------------
      // C++ INTERFACE:
      // ----------------------------------------------------------------------

      time_t
      getTime() const;
      
   private:

      enum
      {
         SecondsPerDayE    = 86400,
         SecondsPerHourE   =  3600,
         SecondsPerMinuteE =    60
      };

      void
      setDate(
         Number theYear,
         Number theMonth,
         Number theDay,
         Number theHour,
         Number theMinute,
         Number theSecond);
   
      static
      Number
      dateFromTime(
         Number theTime);

      static
      Number
      day(
         Number theTime);

      static
      Number
      dayFromYear(
         Number theYear);

      static
      Number
      daysInYear(
         Number theYear);

      static
      Number
      dayWithinYear(
         Number theTime);

      static
      Number
      hourFromTime(
         Number theTime);
         
      static
      Number
      inLeapYear(
         Number theTime);

      static
      Number
      localTime(
         Number theTime);

      static
      Number
      minFromTime(
         Number theTime);

      static
      Number
      monthFromTime(
         Number theTime);

      static
      Number
      secFromTime(
         Number theTime);

      static
      Number
      timeFromYear(
         Number theYear);

      static
      Number
      timeWithinDay(
         Number theTime);

      static
      Number
      weekDay(
         Number theTime);

      static
      Number
      yearFromTime(
         Number theTime);

      time_t              timeM;
   };

   // A smart-pointer for objects of this class
   typedef SmartPointer<ScriptObjectDate> ScriptObjectDatePtr;

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::dateFromTime(
      Number theTime)
   {
      Number yearDay = dayWithinYear(theTime);
      Number leapYear = inLeapYear(theTime);
      if (yearDay < 31)
      {
         return yearDay + 1;
      }
      if (yearDay < 59 + leapYear)
      {
         return yearDay - 30;
      }
      if (yearDay < 90 + leapYear)
      {
         return yearDay - 58 - leapYear;
      }
      if (yearDay < 120 + leapYear)
      {
         return yearDay - 89 - leapYear;
      }
      if (yearDay < 151 + leapYear)
      {
         return yearDay - 119 - leapYear;
      }
      if (yearDay < 181 + leapYear)
      {
         return yearDay - 150 - leapYear;
      }
      if (yearDay < 212 + leapYear)
      {
         return yearDay - 180 - leapYear;
      }
      if (yearDay < 243 + leapYear)
      {
         return yearDay - 211 - leapYear;
      }
      if (yearDay < 273 + leapYear)
      {
         return yearDay - 242 - leapYear;
      }
      if (yearDay < 304 + leapYear)
      {
         return yearDay - 272 - leapYear;
      }
      if (yearDay < 334 + leapYear)
      {
         return yearDay - 303 - leapYear;
      }
      return yearDay - 333 - leapYear;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::day(
      Number theTime)
   {
      return theTime / SecondsPerDayE;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::dayFromYear(
      Number theYear)
   {
      return 365 * (theYear - 1970) + 
                   (theYear - 1969) / 4 - 
                   (theYear - 1901) / 100 + 
                   (theYear - 1601) / 400;
   }

   // -------------------------------------------------------------------------

   inline
   Number 
   ScriptObjectDate::daysInYear(
      Number theYear)
   {
      // ECMA-262 15.9.1.3
      UInt32 tmp = (UInt32) theYear;
      if ((tmp % 4) != 0)
      {
         return 365;
      }
      if ((tmp % 100) != 0)
      {
         return 366;
      }
      if ((tmp % 400) != 0)
      {
         return 365;
      }
      return 366;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::dayWithinYear(
      Number theTime)
   {
      return day(theTime) - dayFromYear(yearFromTime(theTime));
   }

   // -------------------------------------------------------------------------

   inline
   time_t
   ScriptObjectDate::getTime() const
   {
      return timeM;
   }
      
   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::hourFromTime(
      Number theTime)
   {
      return (UInt32)(theTime / SecondsPerHourE) % 24;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::inLeapYear(
      Number theTime)
   {
      if (daysInYear(yearFromTime(theTime)) == 365)
      {
         return 0;
      }
      else
      {
         return 1;
      }
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::localTime(
      Number theTime)
   {
      return theTime - Time::getTimeZoneDS();
   }
   
   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::minFromTime(
      Number theTime)
   {
      return (UInt32)(theTime / SecondsPerMinuteE) % 60;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::monthFromTime(
      Number theTime)
   {
      Number yearDay = dayWithinYear(theTime);
      Number leapYear = inLeapYear(theTime);
      if (yearDay < 31)
      {
         return 0;
      }
      yearDay -= leapYear;
      if (yearDay < 59)
      {
         return 1;
      }
      if (yearDay < 90)
      {
         return 2;
      }
      if (yearDay < 120)
      {
         return 3;
      }
      if (yearDay < 151)
      {
         return 4;
      }
      if (yearDay < 181)
      {
         return 5;
      }
      if (yearDay < 212)
      {
         return 6;
      }
      if (yearDay < 243)
      {
         return 7;
      }
      if (yearDay < 273)
      {
         return 8;
      }
      if (yearDay < 304)
      {
         return 9;
      }
      if (yearDay < 334)
      {
         return 10;
      }
      return 11; 
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::secFromTime(
      Number theTime)
   {
      return (UInt32)theTime % SecondsPerMinuteE;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::timeFromYear(
      Number theYear)
   {
      return SecondsPerDayE * dayFromYear(theYear);
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::timeWithinDay(
      Number theTime)
   {
      return (UInt32)theTime % SecondsPerDayE;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::weekDay(
      Number theTime)
   {
      return (UInt32)(day(theTime) + 4) % 7;
   }

   // -------------------------------------------------------------------------

   inline
   Number
   ScriptObjectDate::yearFromTime(
      Number theTime)
   {
      Number year = day(theTime) / 365 + 1970;
      while (timeFromYear(year) > theTime)
      {
         year--;
      } 

      return year;
   }
}

#endif
