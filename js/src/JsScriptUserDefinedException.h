#ifndef PALSCRIPTUSERDEFINEDEXCEPTION_H
#define PALSCRIPTUSERDEFINEDEXCEPTION_H

#include "JsScriptException.h"

namespace Js
{
   class ScriptUserDefinedException : public ScriptException
   {
   public:

      ScriptUserDefinedException(
         ScriptValue theValue);

      ScriptUserDefinedException(
         ScriptValue         theValue,
         const ScriptString* theFile,
         int                 theLine);

      ScriptUserDefinedException(
         const ScriptUserDefinedException& theOther);

      virtual 
      ~ScriptUserDefinedException()
         throw ();

      const char*
      getName() const;

      ScriptException::Type
      getType() const;

      ScriptValue
      toValue() const;

   private:

      ScriptValue valueM;
   };

   // -------------------------------------------------------------------------
 
   inline
   ScriptUserDefinedException::ScriptUserDefinedException(
      ScriptValue theValue)
   :  ScriptException(theValue.getValue().toString().c_str()),
      valueM(theValue)
   {
      setDescription(theValue.getValue().toString().c_str());
   }

   // -------------------------------------------------------------------------

   inline
   ScriptUserDefinedException::ScriptUserDefinedException(
      ScriptValue         theValue,
      const ScriptString* theFile,
      int                 theLine)
   :  ScriptException(theValue.getValue().toString().c_str(), theFile, theLine),
      valueM(theValue)
   {
      // Empty
   }

   // -------------------------------------------------------------------------
 
   inline
   ScriptUserDefinedException::ScriptUserDefinedException(
      const ScriptUserDefinedException& theOther)
   :  ScriptException(theOther),
      valueM(theOther.valueM)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptUserDefinedException::~ScriptUserDefinedException()
   throw ()
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   const char*
   ScriptUserDefinedException::getName() const
   {
      return "UserDefinedException";
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::Type
   ScriptUserDefinedException::getType() const
   {
      return ScriptException::UserDefinedExceptionE;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptUserDefinedException::toValue() const
   {
      return valueM;
   }
}

#endif
