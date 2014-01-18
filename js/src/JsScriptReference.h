#ifndef PALSCRIPTREFERENCE_H
#define PALSCRIPTREFERENCE_H

#include "JsScriptDefinitions.h"
#include "JsScriptObject.h"
#include "JsScriptReferenceError.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptReference :  public ThreadSafeSmartPointerObject
   {
   public:

      /**
       * Create an empty reference.
       */
      ScriptReference();

      /**
       * Create to the specified property of the specified object.
       * @param theObject       a pointer to the object.
       * @param thePropertyName the name of the property.
       */
      ScriptReference(
         ScriptObject* theObject,
         const std::string& thePropertyName);

      /**
       * Copy constructor.
       */
      ScriptReference(
         const ScriptReference& theOther);

      /**
       * Destructor.
       */
      virtual
      ~ScriptReference();

      /**
       * Equality operator.
       * @param theOther the other script value.
       * @return 'true' if this value is equal to the other value.
       */
      bool
      operator==(
         const ScriptValue& theOther) const;

      /**
       * Delete operator.
       * @return 'true'  if the property that it referenced could be deleted,
       *         'false' otherwise.
       */
      ScriptValue
      deleteOperator();

      /**
       * Returns the base of the reference.
       */
      ScriptObject*
      getBase();

      /**
       * Returns the data type of the value.
       * @return ReferenceE.
       */
      ScriptValue::DataType
      getDataType() const;

      /**
       * Returns the property name of the reference.
       */
      const std::string&
      getPropertyName() const;

      /**
       * Returns the value of the property.
       * @return Value of property.
       */
      ScriptValue
      getValue() const;

      /**
       * Sets the value of the property.
       * @param theValue The value of the property.
       */
      void
      putValue(
         const ScriptValue& theValue);

   private:

      ScriptObject* objectM;
      std::string        propertyNameM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptReference::ScriptReference()
   :  objectM(0)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptReference::ScriptReference(
      ScriptObject* theObject,
      const std::string& thePropertyName)
   :  objectM(theObject),
      propertyNameM(thePropertyName)
   {
      if (objectM != 0)
      {
         objectM->AddRef();
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptReference::ScriptReference(
      const ScriptReference& theOther)
   :  objectM(theOther.objectM),
      propertyNameM(theOther.propertyNameM)
      
   {
      if (objectM != 0)
      {
         objectM->AddRef();
      }
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptReference::operator==(
      const ScriptValue& theOther) const
   {
      if (theOther.getDataType() == ScriptValue::ReferenceE)
      {
         return objectM == ((const ScriptReference&)theOther).objectM;
      }
      return false;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptReference::~ScriptReference()
   {
      if (objectM != 0)
      {
         objectM->Release();
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptReference::deleteOperator()
   {
      if (objectM != 0)
      {
         return ScriptValue(objectM->deleteProperty(propertyNameM));
      }
      return ScriptValue(false);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptObject*
   ScriptReference::getBase()
   {
      return objectM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue::DataType
   ScriptReference::getDataType() const
   {
      return ScriptValue::ReferenceE;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptReference::getPropertyName() const
   {
      return propertyNameM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptReference::getValue() const
   {
      if (objectM == 0)
      {
         throw ScriptReferenceError(
            "Can't get property '" + propertyNameM + "'.");
      }
      return objectM->getProperty(propertyNameM);   
   }
   
   // -------------------------------------------------------------------------

   inline
   void
   ScriptReference::putValue(
      const ScriptValue& theValue)
   {
      if (objectM == 0)
      {
         throw ScriptReferenceError(
            "Can't set property '" + propertyNameM + "'.");
      }
      objectM->putProperty(propertyNameM, theValue);
   }
}

#endif
