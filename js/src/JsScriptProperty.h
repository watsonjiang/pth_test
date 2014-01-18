#ifndef PALSCRIPTPROPERY_H
#define PALSCRIPTPROPERY_H

#include "JsScriptValue.h"

namespace Js
{
   class ScriptProperty
   {
   public:

      enum
      {
         DontDeleteE = 1,
         DontEnumE   = 2,
         ReadOnlyE   = 4,
         ExternalE   = 8
      };

      ScriptProperty();

      ScriptProperty(
         const ScriptValue& theValue);

      ScriptProperty(
         const ScriptValue& theValue,
         unsigned int       theAttributes);

      ScriptProperty(
         const ScriptProperty& theOther);

      ScriptProperty&
      operator=(
         const ScriptProperty& theOther);

      ScriptProperty&
      operator=(
         const ScriptValue& theOther);

      bool
      dontDelete() const;

      bool
      dontEnum() const;

      unsigned int
      getAttributes() const;

      const ScriptValue&
      getValue() const;

      ScriptValue&
      getValue();

      bool
      isExternal() const;

      bool
      isReadOnly() const;

      bool
      putValue(
         const ScriptValue& theValue);

      void
      putValue(
         const ScriptValue& theValue,
         unsigned int       theAttributes);

   private:

      unsigned int  attributesM;
      ScriptValue   valueM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptProperty::ScriptProperty()
   :  attributesM(0),
      valueM(ScriptValue::UndefinedE)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptProperty::ScriptProperty(
      const ScriptValue& theValue)
   :  attributesM(0),
      valueM(theValue)
      
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptProperty::ScriptProperty(
      const ScriptValue& theValue,
      unsigned int       theAttributes)
   :  attributesM(theAttributes),
      valueM(theValue)
      
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptProperty::ScriptProperty(
      const ScriptProperty& theOther)
   :  attributesM(theOther.attributesM),
      valueM(theOther.valueM)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptProperty&
   ScriptProperty::operator=(
      const ScriptProperty& theOther)
   {
      attributesM = theOther.attributesM;
      valueM = theOther.valueM;
      return *this;
   }

   // -------------------------------------------------------------------------
   
   inline
   ScriptProperty&
   ScriptProperty::operator=(
      const ScriptValue& theOther)
   {
      attributesM = 0;
      valueM = theOther;
      return *this;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptProperty::dontDelete() const
   {
      return (attributesM & DontDeleteE) != 0;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptProperty::dontEnum() const
   {
      return (attributesM & DontEnumE) != 0;
   }

   // -------------------------------------------------------------------------

   inline
   unsigned int
   ScriptProperty::getAttributes() const
   {
      return attributesM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptValue&
   ScriptProperty::getValue() const
   {
      return valueM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue&
   ScriptProperty::getValue()
   {
      return valueM;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptProperty::isExternal() const
   {
      return (attributesM & ExternalE) != 0;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptProperty::isReadOnly() const
   {
      return (attributesM & ReadOnlyE) != 0;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptProperty::putValue(
      const ScriptValue& theValue)
   {
      if ((attributesM & ReadOnlyE) != 0)
      {
         // Property is read-only
         return false;
      }
      valueM = theValue;
      return true;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptProperty::putValue(
      const ScriptValue& theValue,
      unsigned int       theAttributes)
   {
      valueM = theValue;
      attributesM = theAttributes;
   }

}

#endif
