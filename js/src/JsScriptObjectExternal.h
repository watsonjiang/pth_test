#ifndef PALSCRIPTOBJECTEXTERNAL_H
#define PALSCRIPTOBJECTEXTERNAL_H

#include "JsScriptObject.h"
#include "JsScriptValueArray.h"

namespace Js
{
   class ScriptObjectExternal : public ScriptObject
   {
   public:

      virtual
      ~ScriptObjectExternal();

      virtual
      ScriptValue
      call(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);

      virtual
      bool
      deleteProperty(
         const std::string& theName);

      /**
       * Flush all properties
       */
      virtual
      void
      flushAllProperties();

      virtual
      const char*
      getClass() const;

      virtual
      bool
      getNameOfFirstProperty(
         std::string& theName) const;

      virtual
      bool
      getNameOfNextProperty(
         std::string& theName) const;

      /**
       * Get the specified property.
       */
      virtual
      ScriptValue
      getProperty(
         const std::string& theName) const;

      /**
       * Convert the value to a value of type String.
       * @return the string value.
       */
      virtual
      ScriptValue
      getStringValue() const;

      /**
       * Check if the object has the specified property.
       */
      virtual
      bool
      hasProperty(
         const std::string& theName);

      /**
       * Check if this object is derived from the class 'theClassName'.
       * @param theClassName (in) The name of the class.
       * @return 'true'  if this object is derived from the the specified class
       *         'false' otherwise.
       */
      virtual
      bool
      isInstanceOf(
         const std::string& theClassName) const;

      /**
       * Put the specified property.
       */
      virtual
      bool
      putProperty(
         const std::string&      theName,
         const ScriptValue& theValue);

   protected:

      typedef int CompareFct(const void* theKey, const void* theElement); 

      ScriptObjectExternal();

      static
      void*
      bfindnext(
         const void* theKey,
         const void* theTable,
         size_t      theNumberOfElements,
         size_t      theElementWidth,
         CompareFct* theComparisonFunction);        

      static
      void*
      bsearch(
         const void* theKey,
         const void* theTable,
         size_t      theNumberOfElements,
         size_t      theElementWidth,
         CompareFct* theComparisonFunction);        

      virtual
      bool
      callExternalMethod(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor,
         ScriptValue&            theResult);

      virtual
      ScriptObject::UpdateResult
      deleteExternalProperty(
         const std::string& theName);

      virtual
      bool
      getExternalProperty(
         const std::string& theName,
         ScriptValue&  theValue) const;

      virtual
      bool
      getNameOfFirstExternalProperty(
         std::string& theName) const;

      virtual
      bool
      getNameOfNextExternalProperty(
         std::string& theName) const;

      virtual
      bool
      hasExternalProperty(
         const std::string& theName) const;

      ScriptValue
      makeScriptValue(
         const std::string& theString) const;

      ScriptValue
      makeScriptValue(
         const ScriptString* theString) const;

      ScriptValue
      makeScriptValue(
         const ScriptObject* theObject) const;

      virtual
      ScriptObject::UpdateResult
      setExternalProperty(
         const std::string& theName,
         ScriptValue   theValue);

   private:

      // only included to avoid compiler warning
      virtual
      ScriptValue
      call(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments) {return ScriptValue(ScriptValue::UndefinedE); };

      // only included to avoid compiler warning
      virtual
      void
      putProperty(
         const std::string&      theName,
         const ScriptValue& theValue,
         unsigned int       theAttributes) {};

   };

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptObjectExternal::makeScriptValue(
      const std::string& theString) const
   {
      if (theString.empty() == false)
      {
         return ScriptValue(theString);
      }
      else
      {
         return ScriptValue(ScriptValue::UndefinedE);
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptObjectExternal::makeScriptValue(
      const ScriptString* theString) const
   {
      if (theString != 0 &&
          theString->empty() == false)
      {
         return ScriptValue(theString);
      }
      else
      {
         return ScriptValue(ScriptValue::UndefinedE);
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptObjectExternal::makeScriptValue(
      const ScriptObject* theObject) const
   {
      if (theObject != 0)
      {
         return ScriptValue(const_cast<ScriptObject*>(theObject));
      }
      else
      {
         return ScriptValue(ScriptValue::UndefinedE);
      }
   }
}

#endif
