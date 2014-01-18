#ifndef PALSCRIPTREFERENCEERROR_H
#define PALSCRIPTREFERENCEERROR_H

#include "JsScriptException.h"
#include "JsScriptObjectReferenceError.h"

namespace Js
{
   class ScriptReferenceError : public ScriptException
   {
   public:

      ScriptReferenceError(
         const std::string& theDescription);

      ScriptReferenceError(
         const std::string&  theDescription,
         const ScriptString* theFile,
         int                 theLine);

      virtual 
      ~ScriptReferenceError()
         throw ();

      const char*
      getName() const;

      ScriptException::Type
      getType() const;

      ScriptValue
      toValue() const;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptReferenceError::ScriptReferenceError(
      const std::string& theDescription)
   :  ScriptException(theDescription)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptReferenceError::ScriptReferenceError(
      const std::string&  theDescription,
      const ScriptString* theFile,
      int                 theLine)
   :  ScriptException(theDescription, theFile, theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

//    inline
//    ScriptReferenceError::~ScriptReferenceError()
//    throw ()
//    {
//       // Empty
//    }

   // -------------------------------------------------------------------------

   inline
   const char*
   ScriptReferenceError::getName() const
   {
      return "ReferenceError";
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::Type
   ScriptReferenceError::getType() const
   {
      return ScriptException::ReferenceErrorE;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptReferenceError::toValue() const
   {
      return ScriptValue(new ScriptObjectReferenceError(getDescription()));
   }
}

#endif
