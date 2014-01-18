#ifndef PALSCRIPTOBJECT_H
#define PALSCRIPTOBJECT_H

#include "JsScriptDefinitions.h"
#include "JsScriptProperty.h"
#include "JsScriptValue.h"
#include "JsScriptValueArray.h"
#include "JsSmartPointer.h"
#include <map>

namespace Js
{
   class ScriptExecutionContext;

   /** \class ScriptObject ScriptObject.h "ScriptObject.h"
    * \ingroup PALSCRIPT
    * Object object
    */
   class ScriptObject : public ThreadSafeSmartPointerObject
   {
   public:

      typedef enum
      {
         ActivationE,   // Activation object
         ArrayE,        // Array object
         DateE,         // Date object
         ErrorE,        // Error object
         ExternalE,     // External object
         FunctionE,     // Function object
         ObjectE        // Plain object
      } ObjectType;

      typedef enum
      {
         UpdatedE,        
         NotUpdatedE,
         UpdateNotAllowedE
      } UpdateResult;

      ScriptObject(
         ObjectType theObjectType);

      /**
       * Destructor.
       */
      virtual
      ~ScriptObject();

      /**
       * Equality operator.
       * @param theOther the other script value.
       * @return 'true'   if this value is equal to the other value.
       *         'false', otherwise.
       */
      virtual
      bool
      operator==(
         const ScriptValue& theOther) const;

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

      virtual
      ScriptValue
      call(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);

      virtual
      ScriptValue
      call(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments);

      /**
       * Remove the specified property from the object.
       * @param theName The name of the property.
       * @return 'true'   if the removal was successful,
       *         'false', otherwise.
       */
      virtual
      bool
      deleteProperty(
         const std::string& theName);

      /**
       * Remove the specified property from the object unconditionally.
       * @param theName The name of the property.
       * @return 'true'   if the removal was successful,
       *         'false', otherwise.
       */
      virtual
      bool
      deletePropertyNoCheck(
         const std::string& theName);

      /**
       * Flush all properties.
       */
      virtual
      void
      flushAllProperties();

      /**
       * Convert the value to a value of type Boolean.
       * @return the boolean value.
       */
      virtual
      bool
      getBooleanValue() const;

      /**
       * Get the name of this class.
       * @return The name of this class.
       */
      virtual
      const char*
      getClass() const;

      /**
       * Get the name of the first property that can be enumerated.
       * @param theName If the method returns 'true' then this parameter
       *                contains the name of the first enumeratable
       *                property.
       *                Otherwise this parameter is unchanged.
       * @return 'true'   if a property that can be enumerated exist,
       *         'false', otherwise.
       */
      virtual
      bool
      getNameOfFirstProperty(
         std::string& theName) const;

      /**
       * Get the name of the nect property that can be enumerated.
       * @param theName If the method returns 'true' then this parameter
       *                contains the name of the first enumeratable
       *                property.
       *                Otherwise this parameter is unchanged.
       * @return 'true'   if a property that can be enumerated exist
       *                  after the specified property.
       *         'false', otherwise.
       */
      virtual
      bool
      getNameOfNextProperty(
         std::string& theName) const; 

      /**
       * Convert the value to a value of type Number.
       * @return the number value.
       */
      virtual
      Number
      getNumberValue() const;

      virtual
      ObjectType
      getObjectType() const;

      /**
       * Get the value of the specified property.
       */
      virtual
      ScriptValue
      getProperty(
         const std::string& theName) const;

      /**
       * Get the names of all properties.
       */
      void
      getPropertyNames(
         std::vector<std::string>& theNames) const;

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
         const std::string& theName) const;

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
       * Print a human-readable description of the object to a string.
       * @param theOutputString the string to which the description is
       *                        appended.
       * @param theLinePrefix   the prefix that should to be added to
       *                        each line of the description except of
       *                        the first one.
       */
      void
      print(
         std::string&       theOutputString,
         const std::string& theLinePrefix = "") const;

      /**
       * Put the specified property.
       */
      virtual
      bool
      putProperty(
         const std::string&      theName,
         const ScriptValue& theValue);

      /**
       * Put the specified property.
       */
      virtual
      void
      putProperty(
         const std::string&      theName,
         const ScriptValue& theValue,
         unsigned int       theAttributes);

      void
      putPropertyNoCheck(
         const std::string&      theName,
         const ScriptValue& theValue);

      void
      putPropertyNoCheck(
         const std::string&      theName,
         const ScriptValue& theValue,
         unsigned int       theAttributes);

      /**
       * Read object from the specified C-string.
       */
      virtual
      void
      read(
         const char*& theCurrentChar, 
         int&         theLine, 
         int&         theColumn);

      static
      ScriptValue
      readPropertyValue(
         const char*& theCurrentChar,
         int&         theLine,
         int&         theColumn);

   protected:

      ScriptProperty*
      getPropertyObject(
         const std::string& theName);

      const ScriptProperty*
      getPropertyObject(
         const std::string& theName) const;

      ScriptObject*
      getPrototypeObject();

      const ScriptObject*
      getPrototypeObject() const;

      std::string 
      readPropertyName(
         const char*& theCurrentChar, 
         int&         theLine, 
         int&         theColumn);

      UpdateResult
      updateProperty(
         const std::string& theName,
         ScriptValue   theValue);

      typedef std::map<std::string, ScriptProperty> Properties;

      ObjectType          objectTypeM;
      Properties          propertiesM;      

      static const std::string PrototypeC;
   };

   typedef SmartPointer<ScriptObject> ScriptObjectPtr;

   // -------------------------------------------------------------------------

   inline
   ScriptProperty*
   ScriptObject::getPropertyObject(
      const std::string& theName)
   {
      Properties::iterator iter1 = propertiesM.find(theName);
      if (iter1 != propertiesM.end())
      {
         // Property exists
         return &((*iter1).second);
      }
      return 0;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptProperty*
   ScriptObject::getPropertyObject(
      const std::string& theName) const
   {
      Properties::const_iterator iter1 = propertiesM.find(theName);
      if (iter1 != propertiesM.end())
      {
         // Property exists
         return &((*iter1).second);
      }
      return 0;
   }
   
   // -------------------------------------------------------------------------

   inline
   ScriptObject*
   ScriptObject::getPrototypeObject()
   {
      Properties::iterator iter1 = propertiesM.find(PrototypeC);
      if (iter1 != propertiesM.end())
      {
         // Property exists
         return (*iter1).second.getValue().toObject();
      }
      return 0;
   }
   
   // -------------------------------------------------------------------------

   inline
   const ScriptObject*
   ScriptObject::getPrototypeObject() const 
   {
      Properties::const_iterator iter1 = propertiesM.find(PrototypeC);
      if (iter1 != propertiesM.end())
      {
         // Property exists
         return (*iter1).second.getValue().toObject();
      }
      return 0;
   }

   // -------------------------------------------------------------------------

}


#endif
