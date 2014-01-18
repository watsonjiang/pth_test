#include "JsScriptObjectExternal.h"

using namespace Js;

ScriptObjectExternal::ScriptObjectExternal()
:  ScriptObject(ScriptObject::ExternalE)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectExternal::~ScriptObjectExternal()
{
   // Empty
}

// ----------------------------------------------------------------------------

void*
ScriptObjectExternal::bfindnext(
   const void* theKey,
   const void* theTable,
   size_t      theNumberOfElements,
   size_t      theElementWidth,
   CompareFct  theComparisonFunction)
{
   if (theNumberOfElements == 0)
   {
      // The table is empty
      return 0;
   }

   char* base = (char*)theTable;
   size_t two_width = theElementWidth + theElementWidth;
   char* last = base + theElementWidth * (theNumberOfElements - 1);
    
   void* nextElement = 0;
   while (last >= base)
   {
      char* p = base + theElementWidth * ((last - base) / two_width);
      int result = theComparisonFunction(theKey, (void*)p);
      
      if (result < 0)
      {
         last = p - theElementWidth;
         nextElement = p;
      }
      else
      {
         base = p + theElementWidth;
      }
   }
   return nextElement;
}

// ----------------------------------------------------------------------------

void*
ScriptObjectExternal::bsearch(
   const void* theKey,
   const void* theTable,
   size_t      theNumberOfElements,
   size_t      theElementWidth,
   CompareFct  theComparisonFunction)
{
   if (theNumberOfElements == 0)
   {
      // The table is empty
      return 0;
   }

   char* base = (char*)theTable;
   size_t two_width = theElementWidth + theElementWidth;
   char* last = base + theElementWidth * (theNumberOfElements - 1);
    
   while (last >= base)
   {
      char* p = base + theElementWidth * ((last - base) / two_width);
      int result = theComparisonFunction(theKey, (void*)p);
      
      if (result == 0)
      {
         // Key found
         return p; 
      }

      if (result < 0)
      {
         last = p - theElementWidth;
      }
      else
      {
         base = p + theElementWidth;
      }
   }
   
   // Key not found
   return 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectExternal::call(
   const std::string&           theName,
   ScriptExecutionContext* theContext, 
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{
   ScriptValue result;
   if (callExternalMethod(theName, 
                          theContext, 
                          theArguments, 
                          theIsConstructor, 
                          result) == true)
   {
      return result;
   }
   else
   {
      return ScriptObject::call(theName, 
                                theContext, 
                                theArguments, 
                                theIsConstructor);
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::deleteProperty(
   const std::string& theName)
{
   // Try to delete the property in the external object
   switch (deleteExternalProperty(theName))
   {
      case UpdatedE:
      {
         // Property exists in the external object and has been deleted
         return true;
      }
      case UpdateNotAllowedE:
      {
         // It is not allowed to delete this property.
         return false;
      }
      case NotUpdatedE:
      {
         // Property does not exist in the external object, but might
         // exist in the internal object
         break;
      }
   }

   return ScriptObject::deleteProperty(theName);
}

// ----------------------------------------------------------------------------

void
ScriptObjectExternal::flushAllProperties()
{
   ScriptObject::flushAllProperties();
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectExternal::getClass() const
{
   // ECMA-262 8.6.2
   return "External";
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::getNameOfFirstProperty(
   std::string& theName) const
{
   if (ScriptObject::getNameOfFirstProperty(theName) == true)
   {
      // Internal object has properties - check if external object
      // also has properties
      std::string tempName;
      if (getNameOfFirstExternalProperty(tempName) == true)
      {
         // External object has properties
         if (tempName < theName)
         {
            // First property of external object is before first
            // property of internal object
            theName = tempName;
         }
      }
      return true;
   }
   // Internal object has no properties - return first property of
   // external object
   return getNameOfFirstExternalProperty(theName);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::getNameOfNextProperty(
   std::string& theName) const
{
   std::string tempName(theName);
   if (ScriptObject::getNameOfNextProperty(theName) == true)
   {
      // Internal object has next property - check if external object
      // also has next property
      if (getNameOfNextExternalProperty(tempName) == true)
      {
         // External object has next property.
         if (tempName < theName)
         {
            // Next property of external object is before next
            // property of internal object
            theName = tempName;
         }
      }
      return true;
   }

   // Internal object has no next property - return next property of
   // external object
   return getNameOfNextExternalProperty(theName);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectExternal::getProperty(
   const std::string& theName) const
{
   ScriptValue value;
   if (getExternalProperty(theName, value) == true)
   {
      // Property exists in external object
      return value;
   }

   // Property does not exist in external object - try to get it from
   // the internal object
   return ScriptObject::getProperty(theName);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectExternal::getStringValue() const
{
   return ScriptObject::getStringValue();
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::hasProperty(
   const std::string& theName)
{
   // Check if the property exists in the external object
   if (hasExternalProperty(theName) == false)
   {
      return ScriptObject::hasProperty(theName);
   }
   return true;   
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::isInstanceOf(
   const std::string& theClassName) const
{
   if (theClassName == "External")
   {
      return true;
   }
   return ScriptObject::isInstanceOf(theClassName);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::putProperty(
   const std::string&      theName,
   const ScriptValue& theValue)
{
   // Try to set the property in the external object
   switch (setExternalProperty(theName, theValue))
   {
      case UpdatedE:
      {
         // Property exists in the external object and has been set
         return true;
      }
      case UpdateNotAllowedE:
      {
         // It is not allowed to set this property.
         return false;
      }
      case NotUpdatedE:
      {
         // Property does not exist in the external object, but it is
         // allowed to create it in the internal object
         break;
      }
   }

   return ScriptObject::putProperty(theName, theValue);
}

// ----------------------------------------------------------------------------
// PROTECTED METHODS:
// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::callExternalMethod(
   const std::string&           theName,
   ScriptExecutionContext* theContext, 
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor,
   ScriptValue&            theResult)
{
   return false;
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectExternal::deleteExternalProperty(
   const std::string& theName)
{
   return Js::ScriptObject::UpdateNotAllowedE;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::getExternalProperty(
   const std::string& theName,
   ScriptValue&  theValue) const
{
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::getNameOfFirstExternalProperty(
   std::string& theName) const
{
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::getNameOfNextExternalProperty(
   std::string& theName) const
{
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectExternal::hasExternalProperty(
   const std::string& theName) const
{
   return false;
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectExternal::setExternalProperty(
   const std::string& theName,
   ScriptValue   theValue)
{
   return Js::ScriptObject::UpdateNotAllowedE;
}
