#ifndef PALSCRIPTOBJECTARRAY_H
#define PALSCRIPTOBJECTARRAY_H

#include "JsScriptObject.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptValueArray.h"

namespace Js
{
  /** \class ScriptObjectArray ScriptObjectArray.h "ScriptObjectArray.h"
   * \ingroup PALSCRIPT
   * Array object
   */
   class ScriptObjectArray : public ScriptObject
   {
   public:

      ScriptObjectArray();

      virtual
      ~ScriptObjectArray();

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
       * Create a new instance of the scriptable class.
       * @param theContext   (in) The execution context from which the
       *                          constructor is called.
       * @param theArguments (in) The values of the constructor arguments.
       * @return A pointer to the new object.
       */
      static
      ScriptObject* 
      create(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments);

      ScriptValue
      call(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);

      /**
       * Get the name of this class.
       * @return The name of this class.
       */
      const char*
      getClass() const;

      /**
       * Get the length of the array.
       * @return The length of the array.
       */
      int
      getLength() const;

      /**
       * Get the specified property.
       */
      ScriptValue
      getProperty(
         const std::string& thePropertyName) const;

      /**
       * Get the specified property.
       */
      ScriptValue
      getProperty(
         Number theIndex) const;

      /**
       * Convert the value to a value of type String.
       * @return the string value.
       */
      ScriptValue
      getStringValue() const;

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
       * Put the specified property.
       */
      bool
      putProperty(
         const std::string&      thePropertyName,
         const ScriptValue& theValue);

      /**
       * Put the specified property.
       */
      bool
      putProperty(
         int                theIndex,
         const ScriptValue& theValue);

      void
      putPropertyNoCheck(
         int                theIndex,
         const ScriptValue& theValue);

      /**
       * Read object from the specified C-string.
       */
      virtual
      void
      read(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

   private:

      // only included to avoid compiler warning
      virtual
      void
      putProperty(
         const std::string&      theName,
         const ScriptValue& theValue,
         unsigned int       theAttributes) {};

      int                 lengthM;
   };

   // A smart-pointer for objects of this class
   typedef SmartPointer<ScriptObjectArray> ScriptObjectArrayPtr;

   // ----------------------------------------------------------------------------

   inline
   int
   ScriptObjectArray::getLength() const
   {
      return lengthM;
   }

   // ----------------------------------------------------------------------------

   inline
   void
   ScriptObjectArray::putPropertyNoCheck(
      int                theIndex,
      const ScriptValue& theValue)
   {
      std::string propertyName;
      char buf[10] = {0};
      snprintf(buf, 9, "%d", theIndex); 
      propertyName.append(buf);
      if (theIndex >= lengthM)
      {
         lengthM = theIndex + 1;
      }
      ScriptObject::putPropertyNoCheck(propertyName, theValue);
   }
}

#endif
