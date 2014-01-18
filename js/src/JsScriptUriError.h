#ifndef PALSCRIPTURIERROR_H
#define PALSCRIPTURIERROR_H

#include "JsScriptException.h"
#include "JsScriptObjectUriError.h"

namespace Js
{
   class ScriptUriError : public ScriptException
   {
   public:

      ScriptUriError(
         const std::string& theDescription);

      ScriptUriError(
         const std::string&       theDescription,
         const ScriptString* theFile,
         int                 theLine);

      ScriptUriError(
         const std::string&       theDescription,
         int                 theLine);

      virtual 
      ~ScriptUriError()
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
   ScriptUriError::ScriptUriError(
      const std::string& theDescription)
   :  ScriptException(theDescription)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptUriError::ScriptUriError(
      const std::string&       theDescription,
      const ScriptString* theFile,
      int                 theLine)
   :  ScriptException(theDescription, theFile, theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptUriError::ScriptUriError(
      const std::string&       theDescription,
      int                 theLine)
   :  ScriptException(theDescription, theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   const char*
   ScriptUriError::getName() const
   {
      return "URIError";
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::Type
   ScriptUriError::getType() const
   {
      return ScriptException::UriErrorE;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScriptUriError::toValue() const
   {
      return ScriptValue(new ScriptObjectUriError(getDescription()));
   }
}

#endif
