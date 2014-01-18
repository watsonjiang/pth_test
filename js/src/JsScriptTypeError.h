#ifndef PALSCRIPTTYPEERROR_H
#define PALSCRIPTTYPEERROR_H

#include "JsScriptException.h"
#include "JsScriptObjectTypeError.h"

namespace Js
{
   class ScriptTypeError : public ScriptException
   {
   public:

      ScriptTypeError(
         const std::string& theDescription);

      ScriptTypeError(
         const std::string&       theDescription,
         const ScriptString* theFile,
         int                 theLine);

      ScriptTypeError(
         const std::string&       theDescription,
         int                 theLine);

      virtual 
      ~ScriptTypeError()
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
   ScriptTypeError::ScriptTypeError(
      const std::string& theDescription)
   :  ScriptException(theDescription)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptTypeError::ScriptTypeError(
      const std::string&       theDescription,
      const ScriptString* theFile,
      int                 theLine)
   :  ScriptException(theDescription, theFile, theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptTypeError::ScriptTypeError(
      const std::string&       theDescription,
      int                 theLine)
   :  ScriptException(theDescription, theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

//    inline
//    ScriptTypeError::~ScriptTypeError()
//    throw ()
//    {
//       // Empty
//    }

   // -------------------------------------------------------------------------

   inline
   const char*
   ScriptTypeError::getName() const
   {
      return "TypeError";
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::Type
   ScriptTypeError::getType() const
   {
      return ScriptException::TypeErrorE;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptTypeError::toValue() const
   {
      return ScriptValue(new ScriptObjectTypeError(getDescription()));
   }
}

#endif
