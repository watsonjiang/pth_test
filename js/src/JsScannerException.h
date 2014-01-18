#ifndef PALSCANNEREXCEPTION_H
#define PALSCANNEREXCEPTION_H

#include "JsScriptException.h"

namespace Js
{
   class ScannerException : public ScriptException
   {
   public:

      ScannerException(
         const std::string&  theDescription,
         const ScriptString* theFile,
         int                 theLine = 0,
         int                 theColumn = 0);

      ScannerException(
         const std::string& theDescription,
         int                theLine = 0,
         int                theColumn = 0);

      virtual 
      ~ScannerException()
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
   ScannerException::ScannerException(
      const std::string&  theDescription,
      const ScriptString* theFile,
      int                 theLine,
      int                 theColumn)
   :  ScriptException(theDescription, theFile, theLine, theColumn)
   {
      // Empty
   }

   // -------------------------------------------------------------------------
 
   inline
   ScannerException::ScannerException(
      const std::string&  theDescription,
      int                 theLine,
      int                 theColumn)
   :  ScriptException(theDescription, theLine, theColumn)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

//    inline
//    ScannerException::~ScannerException()
//    throw ()
//    {
//       // Empty
//    }

   // -------------------------------------------------------------------------

   inline
   const char*
   ScannerException::getName() const
   {
      return "SyntaxError";
   }

   // -------------------------------------------------------------------------

   inline
   ScriptException::Type
   ScannerException::getType() const
   {
      return ScriptException::SyntaxErrorE;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptValue
   ScannerException::toValue() const
   {
      return ScriptValue("SyntaxError: " + getDescription());
   }
}

#endif
