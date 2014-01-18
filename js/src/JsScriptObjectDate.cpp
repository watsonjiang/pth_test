#include "JsScriptObjectDate.h"
#include "JsScriptTypeError.h"

using namespace Js;

typedef ScriptValue (ScriptObjectDate::*GetMethod)() const;
typedef void (ScriptObjectDate::*SetMethod)(ScriptValue theValue);
typedef void (ScriptObjectDate::*DeleteMethod)();
typedef ScriptValue (ScriptObjectDate::*CallMethod)(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                         theIsConstructor);

struct PropertyDescriptor
{
   const char*  name;
   GetMethod    getMethod;
   SetMethod    setMethod;
   DeleteMethod deleteMethod;
   CallMethod   callMethod;
};

// ----------------------------------------------------------------------------
// routine to compare two properties based on an
// alphabetical ordering of the name field
static
int
property_compare(
   const void* thePropertyName,
   const void* thePropertyTableEntry)
{
   return strcmp((const char*)thePropertyName,
                 ((const PropertyDescriptor*)thePropertyTableEntry)->name);
}

// *******************************************************************
// *** THE PROPERTY DESCRIPTORS MUST BE ALPHABETICALLY SORTED !!!! ***
// *******************************************************************

static
const PropertyDescriptor
PropertyDescriptorsC[] =
{
   // <<TODO>> Declare the methods, which implement the scriptable
   //          module.
   //           
   // Function template:
   //   { "[FunctionName]",
   //     0,
   //     0,o
   //     0,
   //     &ScriptObjectDate::scriptCall[FunctionName]
   //   },
   //           
   // Read/write property template:
   //   { "[PropertyName]",
   //     &ScriptObjectDate::scriptGet[PropertyName],
   //     &ScriptObjectDate::scriptSet[PropertyName],
   //     0,
   //     0
   //   },
   //           
   // Read-only property template:
   //   { "[PropertyName]",
   //     &ScriptObjectDate::scriptGet[PropertyName],
   //     0,
   //     0,
   //     0
   //   },
   //   
   // Deletable template:
   //   { "[PropertyName]",
   //     &ScriptObjectDate::scriptGet[PropertyName],
   //     &ScriptObjectDate::scriptSet[PropertyName],
   //     &ScriptObjectDate::scriptDelete[PropertyName],
   //     0
   //   },
   { "getDate",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetDate
   },
   { "getDay",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetDay
   },
   { "getFullYear",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetFullYear
   },
   { "getHours",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetHours
   },
   { "getMinutes",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetMinutes
   },
   { "getMonth",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetMonth
   },
   { "getSeconds",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetSeconds
   },
   { "getTime",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetTime
   },
   { "getTimezoneOffset",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetTimezoneOffset
   },
   { "getUTCDate",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCDate
   },
   { "getUTCDay",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCDay
   },
   { "getUTCFullYear",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCFullYear
   },
   { "getUTCHours",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCHours
   },
   { "getUTCMinutes",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCMinutes
   },
   { "getUTCMonth",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCMonth
   },
   { "getUTCSeconds",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallGetUTCSeconds
   },
   { "setDate",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetDate
   },
   { "setFullYear",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetFullYear
   },
   { "setHours",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetHours
   },
   { "setMinutes",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetMinutes
   },
   { "setMonth",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetMonth
   },
   { "setSeconds",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetSeconds
   },
   { "setTime",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetTime
   },
   { "setUTCFullYear",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetUTCFullYear
   },
   { "setUTCHours",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetUTCHours
   },
   { "setUTCMinutes",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetUTCMinutes
   },
   { "setUTCMonth",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetUTCMonth
   },
   { "setUTCSeconds",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallSetUTCSeconds
   },
   { "toAsctimeString",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToAsctimeString
   },
   { "toDateString",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToDateString
   },
   { "toIso8601String",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToIso8601String
   },
   { "toRfc1123String",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToRfc1123String
   },
   { "toRfc850String",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToRfc850String
   },
   { "toString",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToString
   },
   { "toTimeString",
     0,
     0,
     0,
     &ScriptObjectDate::scriptCallToTimeString
   },
};

static
const size_t NumberOfPropertiesC =
   sizeof(PropertyDescriptorsC) / sizeof(PropertyDescriptor);

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectDate::ClassNameC = "Date";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// -----------------------------------o-----------------------------------------

ScriptObjectDate::ScriptObjectDate()
{
   timeM = time(&timeM);
}

// ----------------------------------------------------------------------------

ScriptObjectDate::ScriptObjectDate(
   Number theTime)
{
   timeM = theTime;
}

// ----------------------------------------------------------------------------

ScriptObjectDate::ScriptObjectDate(
   const Time& theTime)
{
   timeM = theTime.getSeconds();
}

// ----------------------------------------------------------------------------

ScriptObjectDate::ScriptObjectDate(
   Number theYear,
   Number theMonth,
   Number theDay,
   Number theHour,
   Number theMinute,
   Number theSecond)
{
   setDate(theYear, theMonth, theDay, theHour, theMinute, theSecond);
}

// ----------------------------------------------------------------------------

ScriptObjectDate::~ScriptObjectDate()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject*
ScriptObjectDate::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   if (theArguments.size() == 0)
   {
      return new ScriptObjectDate;
   }

   if (theArguments.size() == 1)
   {
      // TODO PARSE 
      if (theArguments[0].isString())
      {
         Time tempTime;
         try
         {
            tempTime.parse(theArguments[0].getValue().toString().c_str());
         }
         catch (std::exception& e)
         {
            std::string errorMessage(
               "Date(): "
               " Invalid date_string: '");
            errorMessage.append(theArguments[0].getValue().toString());
            errorMessage.append("'. ");
            errorMessage.append(e.what());
            throw ScriptTypeError(errorMessage);
         }

         return new ScriptObjectDate(tempTime);
      }
      else
      {
         return new ScriptObjectDate(theArguments[0].getValue().toNumber());
      }
   }
   if (theArguments.size() > 7)
   {
      throw ScriptTypeError(
         "Date():"
         " Constructor has to many arguments () or (date_string) or "
         " (year, month [, date [, hours [, minutes [, seconds [, ms ]]]]]).");
   }
   Number year = theArguments[0].getValue().toNumber();
   if (year >= 70 && year <= 99)
   {
      year += 1900;
   }
   else
   if (year < 1970 || year > 2038)
   {
      throw ScriptTypeError(
         "Date():"
         " Year must be beween 1970 and 2038.");
   }
   Number month = theArguments[1].getValue().toUInt32() % 12;
   Number date = 1;
   Number hours = 0;
   Number minutes = 0;
   Number seconds = 0;
   Number ms = 0; // NOT YET USED
   
   if (theArguments.size() >= 3)
   {
      date = theArguments[2].getValue().toUInt32() % 12;
   }
   
   if (theArguments.size() >= 4)
   {
      hours = theArguments[3].getValue().toNumber();
   }
   
   if (theArguments.size() >= 5)
   {
      minutes = theArguments[4].getValue().toNumber();
   }
   
   if (theArguments.size() >= 6)
   {
      seconds = theArguments[5].getValue().toNumber();
   }
   
   if (theArguments.size() >= 7)
   {
      ms = theArguments[7].getValue().toNumber();
   }
   
   return new ScriptObjectDate(year, month, date, hours, minutes, seconds);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectDate::callExternalMethod(
   const std::string&           theName,
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                         theIsConstructor,
   ScriptValue&            theResult)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // might exist in the internal object
      return false;
   }
   else
   {
      // This property exists in the external object
      if (propertyDescriptor->callMethod != 0)
      {
         // This proerty is callable.
         theResult = (this->*(propertyDescriptor->callMethod))(theContext,
                                                               theArguments,
                                                               theIsConstructor);
         return true;
      }
      else
      {
         // This property is not callable.
         return false;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectDate::deleteExternalProperty(
   const string& theName)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // might exist in the internal object
      return ScriptObject::NotUpdatedE;
   }
   else
   {
      // This property exists in the external object - delete it
      if (propertyDescriptor->deleteMethod != 0)
      {
         // It is allowed to delete this property.
         (this->*(propertyDescriptor->deleteMethod))();
         return ScriptObject::UpdatedE;
      }
      else
      {
         // This property is read-only; it is not allowed to delete it.
         return ScriptObject::UpdateNotAllowedE;
      }
   }
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectDate::getClass() const
{
   return ClassNameC;
}
      
// ----------------------------------------------------------------------------

bool
ScriptObjectDate::getExternalProperty(
   const string& theName,
   ScriptValue&  theValue) const
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // this property does not exist
      return false;
   }
   else
   {
      // this is a known property - return its value
      if (propertyDescriptor->getMethod != 0)
      {
         theValue = (this->*(propertyDescriptor->getMethod))();
      }
      else
      {
         theValue = ScriptValue(ScriptValue::UndefinedE);
      }
      return true;
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObjectDate::getNameOfFirstExternalProperty(
   string& theName) const
{
   for (size_t i = 0; i < NumberOfPropertiesC; i++)
   {
      const PropertyDescriptor* propertyDescriptor = &PropertyDescriptorsC[i];
      if (propertyDescriptor->getMethod != 0 ||
          propertyDescriptor->setMethod != 0)
      {
         // Enumerate only properties, which have a get or set method
         theName = propertyDescriptor->name;
         return true;
      }
   }
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectDate::getNameOfNextExternalProperty(
   string& theName) const
{
   while (true)
   {
      PropertyDescriptor* propertyDescriptor;
      propertyDescriptor = (PropertyDescriptor*)
         bfindnext(theName.c_str(),
                   PropertyDescriptorsC,
                   NumberOfPropertiesC,
                   sizeof(PropertyDescriptor),
                   property_compare);
      if (propertyDescriptor == 0)
      {
         // The next property does not exist
         return false;
      }
      else
      {
         // Found the next property
         theName = propertyDescriptor->name;
         if (propertyDescriptor->getMethod != 0 ||
             propertyDescriptor->setMethod != 0)
         {
            // Enumerate only properties, which have a get or set method
            return true;
         }
      }
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::getStringValue() const
{
   time_t localTime = Time(timeM).toLocalDSTime().getSeconds();
   char buffer[32];
   sprintf(buffer,
           "%04d-%02d-%02d %02d:%02d:%02d", 
           yearFromTime(localTime), 
           monthFromTime(localTime) + 1, 
           dateFromTime(localTime),
           hourFromTime(localTime),
           minFromTime(localTime),
           secFromTime(localTime));
   return ScriptValue(buffer);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectDate::hasExternalProperty(
   const string& theName) const
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      return false;
   }
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectDate::isInstanceOf(
   const string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectExternal::isInstanceOf(theClassName);
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectDate::setExternalProperty(
   const string& theName,
   ScriptValue   theValue)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // is OK to create it in the internal object
      return ScriptObject::NotUpdatedE;
   }
   else
   {
      // This property exists in the external object - set its value
      if (propertyDescriptor->setMethod != 0)
      {
         // It is allowed to set this property.
         (this->*(propertyDescriptor->setMethod))(theValue);
         return ScriptObject::UpdatedE;
      }
      else
      {
         // This property is read-only; it is not allowed to set it.
         return ScriptObject::UpdateNotAllowedE;
      }
   }
}

// ----------------------------------------------------------------------------
// SCRIPT OBJECT METHOD CALL IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which implement the scriptable 
//          functions of the class here.
//          Sort the function names alphabetically.
//
// Template:
//    ScriptValue
//    ScriptObjectDate::scriptCall[FunctionName](
//       ScriptExecutionContext* theContext,
//       const ScriptValueArray& theArguments,
//       bool                         theIsConstructor)
//   { 
//      return ScriptValue([function result]);
//   }

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetDate(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)dateFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetDay(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)weekDay(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetFullYear(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)yearFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetHours(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)hourFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetMinutes(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)minFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetMonth(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)monthFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetSeconds(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)secFromTime(localTime(timeM)));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetTime(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetTimezoneOffset(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   Number offset = timeM - localTime(timeM);
   
   return ScriptValue(offset / 60);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCDate(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)dateFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCDay(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)weekDay(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCFullYear(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)yearFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCHours(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)hourFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCMinutes(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)minFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCMonth(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)monthFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallGetUTCSeconds(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   return ScriptValue((Number)secFromTime(timeM));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetDate(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 1)
   {
      throw ScriptTypeError(
         "Date.setDate():"
         " Function requires one argument (date).");
   }
   Number date = theArguments[0].getValue().toNumber();
   
   time_t tempTime = localTime(timeM);
   setDate(yearFromTime(tempTime), 
           monthFromTime(tempTime), 
           date,
           hourFromTime(tempTime),
           minFromTime(tempTime),
           secFromTime(tempTime));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetFullYear(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setFullYear():"
         " Function requires between one and three arguments (year [, month [, date]]).");
   }
   Number year = theArguments[0].getValue().toNumber();
   if (year >= 70 && year <= 99)
   {
      year += 1900;
   }
   else
   if (year < 1970 || year > 2038)
   {
      throw ScriptTypeError(
         "Date.setFullYear():"
         " Year must be beween 1970 and 2038.");
   }
   Number month;
   Number date;
   time_t tempTime = localTime(timeM);
   if (theArguments.size() >= 2)
   {
      month = theArguments[1].getValue().toNumber();
   }
   else
   {
      month = monthFromTime(tempTime);
   }
   if (theArguments.size() >= 3)
   {
      date = theArguments[2].getValue().toNumber();
   }
   else
   {
      date = dateFromTime(tempTime);
   }
   
   setDate(year, 
           month, 
           date,
           hourFromTime(tempTime),
           minFromTime(tempTime),
           secFromTime(tempTime));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetHours(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 4)
   {
      throw ScriptTypeError(
         "Date.setHours():"
         " Function requires between one and four arguments (hour [, min [, sec [, ms]]]).");
   }
   Number hours = theArguments[0].getValue().toNumber();
   Number minutes;
   Number seconds;
   
   time_t tempTime = localTime(timeM);
   if (theArguments.size() >= 2)
   { 
      minutes = theArguments[1].getValue().toNumber();
   }
   else
   {
      minutes = minFromTime(tempTime);
   }
   
   if (theArguments.size() >= 3)
   { 
      seconds = theArguments[2].getValue().toNumber();
   }
   else
   {
      seconds = secFromTime(tempTime);
   }
   
   setDate(yearFromTime(tempTime), 
           monthFromTime(tempTime), 
           dateFromTime(tempTime),
           hours,
           minutes,
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetMinutes(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 3)
   {
      throw ScriptTypeError(
         "Date.setMinutes():"
         " Function requires between one and three arguments (min [, sec [, ms]]).");
   }
   Number minutes = theArguments[0].getValue().toNumber();
   Number seconds;
   
   time_t tempTime = localTime(timeM);
   if (theArguments.size() >= 2)
   { 
      seconds = theArguments[1].getValue().toNumber();
   }
   else
   {
      seconds = secFromTime(tempTime);
   }
   
   setDate(yearFromTime(tempTime), 
           monthFromTime(tempTime), 
           dateFromTime(tempTime),
           hourFromTime(tempTime),
           minutes,
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetMonth(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setMonth():"
         " Function requires one or two arguments (month [, date]).");
   }
   Number month = theArguments[0].getValue().toNumber();
   Number date;
   time_t tempTime = localTime(timeM);
   if (theArguments.size() >= 2)
   {
      date = theArguments[1].getValue().toNumber();
   }
   else
   {
      date = dateFromTime(tempTime);
   }
   
   setDate(yearFromTime(tempTime), 
           month, 
           date,
           hourFromTime(tempTime),
           minFromTime(tempTime),
           secFromTime(tempTime));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetSeconds(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setSeconds():"
         " Function requires one or two argument (sec [, ms]).");
   }
   Number seconds = theArguments[0].getValue().toNumber();
   
   time_t tempTime = localTime(timeM);
   setDate(yearFromTime(tempTime), 
           monthFromTime(tempTime), 
           dateFromTime(tempTime),
           hourFromTime(tempTime),
           minFromTime(tempTime),
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetTime(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 1)
   {
      throw ScriptTypeError(
         "Date.setTime():"
         " Function requires one argument (time).");
   }
   timeM = theArguments[0].getValue().toNumber();
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCDate(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 1)
   {
      throw ScriptTypeError(
         "Date.setUTCDate():"
         " Function requires one argument (date).");
   }
   Number date = theArguments[0].getValue().toNumber();
   
   setDate(yearFromTime(timeM), 
           monthFromTime(timeM), 
           date,
           hourFromTime(timeM),
           minFromTime(timeM),
           secFromTime(timeM));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCFullYear(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setUTCFullYear():"
         " Function requires between one and three arguments (year [, month [, date]]).");
   }
   Number year = theArguments[0].getValue().toNumber();
   if (year >= 70 && year <= 99)
   {
      year += 1900;
   }
   else
   if (year < 1970 || year > 2038)
   {
      throw ScriptTypeError(
         "Date.setUTCFullYear():"
         " Year must be beween 1970 and 2038.");
   }
   Number month;
   Number date;
   if (theArguments.size() >= 2)
   {
      month = theArguments[1].getValue().toNumber();
   }
   else
   {
      month = monthFromTime(timeM);
   }
   if (theArguments.size() >= 3)
   {
      date = theArguments[2].getValue().toNumber();
   }
   else
   {
      date = dateFromTime(timeM);
   }
   
   setDate(year, 
           month, 
           date,
           hourFromTime(timeM),
           minFromTime(timeM),
           secFromTime(timeM));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCHours(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 4)
   {
      throw ScriptTypeError(
         "Date.setUTCHours():"
         " Function requires between one and four arguments (hour [, min [, sec [, ms]]]).");
   }
   Number hours = theArguments[0].getValue().toNumber();
   Number minutes;
   Number seconds;
   
   if (theArguments.size() >= 2)
   { 
      minutes = theArguments[1].getValue().toNumber();
   }
   else
   {
      minutes = minFromTime(timeM);
   }
   
   if (theArguments.size() >= 3)
   { 
      seconds = theArguments[2].getValue().toNumber();
   }
   else
   {
      seconds = secFromTime(timeM);
   }
   
   setDate(yearFromTime(timeM), 
           monthFromTime(timeM), 
           dateFromTime(timeM),
           hours,
           minutes,
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCMinutes(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 3)
   {
      throw ScriptTypeError(
         "Date.setUTCMinutes():"
         " Function requires between one and three arguments (min [, sec [, ms]]).");
   }
   Number minutes = theArguments[0].getValue().toNumber();
   Number seconds;
   
   if (theArguments.size() >= 2)
   { 
      seconds = theArguments[1].getValue().toNumber();
   }
   else
   {
      seconds = secFromTime(timeM);
   }
   
   setDate(yearFromTime(timeM), 
           monthFromTime(timeM), 
           dateFromTime(timeM),
           hourFromTime(timeM),
           minutes,
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCMonth(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setUTCMonth():"
         " Function requires one or two arguments (month [, date]).");
   }
   Number month = theArguments[0].getValue().toNumber();
   Number date;
   if (theArguments.size() >= 2)
   {
      date = theArguments[1].getValue().toNumber();
   }
   else
   {
      date = dateFromTime(timeM);
   }
   
   setDate(yearFromTime(timeM), 
           month, 
           date,
           hourFromTime(timeM),
           minFromTime(timeM),
           secFromTime(timeM));
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallSetUTCSeconds(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() < 1 || theArguments.size() > 2)
   {
      throw ScriptTypeError(
         "Date.setUTCSeconds():"
         " Function requires one or two argument (sec [, ms]).");
   }
   Number seconds = theArguments[0].getValue().toNumber();
   
   setDate(yearFromTime(timeM), 
           monthFromTime(timeM), 
           dateFromTime(timeM),
           hourFromTime(timeM),
           minFromTime(timeM),
           seconds);
   return ScriptValue((Number)timeM);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToAsctimeString(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toAsctimeString():"
         " Function allows no arguments.");
   }
   time_t tempTime = localTime(timeM);
   return Js::ScriptValue(Js::Time(tempTime).toAsctimeString());
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToDateString(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toDateString():"
         " Function allows no arguments.");
   }
   
   time_t tempTime = localTime(timeM);
   char buffer[64];
   sprintf(buffer, 
           "%04d-%02d-%02d", 
           yearFromTime(tempTime), 
           monthFromTime(tempTime), 
           dateFromTime(tempTime));
   return ScriptValue(buffer);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToIso8601String(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toIso8601String():"
         " Function allows no arguments.");
   }
   return Js::ScriptValue(Js::Time(timeM).toIso8601());
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToRfc1123String(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toRfc1123String():"
         " Function allows no arguments.");
   }
   return Js::ScriptValue(Js::Time(timeM).toRfc1123String());
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToRfc850String(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toRfc850String():"
         " Function allows no arguments.");
   }
   return Js::ScriptValue(Js::Time(timeM).toRfc850String());
}
      
// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToString(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toString():"
         " Function allows no arguments.");
   }
   time_t tempTime = localTime(timeM);
   return Js::ScriptValue(Js::Time(tempTime).toString());
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectDate::scriptCallToTimeString(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "Date.toTimeString():"
         " Function allows no arguments.");
   }
   
   time_t tempTime = localTime(timeM);
   char buffer[64];
   sprintf(buffer, 
           "%02d:%02d:%02d", 
           hourFromTime(tempTime),
           minFromTime(tempTime),
           secFromTime(tempTime));
   return ScriptValue(buffer);
}

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY DELETE IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to delete  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    void
//    ScriptObjectDate::scriptDelete[PropertyName]()
//    {
//       ...
//    }

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY GET IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to get the values of  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    ScriptValue
//    ScriptObjectDate::scriptGet[PropertyName]() const 
//    {
//       return ScriptValue([property value]);
//    }

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY SET IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to set the values of  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    void
//    ScriptObjectDate::scriptSet[property value](
//       ScriptValue theValue)
//    {
//       ...;
//    }

// ----------------------------------------------------------------------------
// C++ INTERFACE:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the long methods (>= 10 lines of code) of your 
//          public C++ interface here (inline).
//          Sort the method names alphabetically.

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------

void
ScriptObjectDate::setDate(
   Number theYear,
   Number theMonth,
   Number theDay,
   Number theHour,
   Number theMinute,
   Number theSecond)
{
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
   timeM = Time::mkTime(tempTime) + Time::getTimeZoneDS();
}

