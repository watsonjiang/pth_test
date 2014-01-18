#ifndef PALSCRIPTRUNTIME_H
#define PALSCRIPTRUNTIME_H

#include "JsScriptDefinitions.h"
#include "JsScriptValue.h"
#include "JsScriptPredefinedNameTable.h"
#include "JsScannerException.h"
#include <string>

namespace Js
{
   class Script;
   class ScriptRuntime 
   {
   public:
     
      ScriptRuntime();

      virtual
      ~ScriptRuntime();

      bool
      addConstant(
         const std::string& theName,         
         ScriptValue        theValue);

      bool
      addConstructorFct(
         const std::string&        theName,         
         ScriptConstructorFct      theFct,
         int                       theNumberOfArguments);

      Script*
      getScript(const std::string& theFileName,
                std::string&       theErrorMessage);
      
      void
      releaseScript(Script* script); 

      ScriptExecutionContext*
      newContext();

      int
      runScript(ScriptExecutionContext* ctx, 
                Script*                 script,
                std::string&            theErrorMessage );

      void
      releaseContext(ScriptExecutionContext* ctx);

      void
      print(
         const std::string& theText);

      void
      removeConstructorFct(
         const std::string& theName);
   
   private:
      
      void
      makeErrorMessage(
         std::string&          theErrorMessage,
         const std::string&    theFilename,
         const std::exception& theException);

      void
      makeErrorMessage(
         std::string&                 theErrorMessage,
         const std::string&           theFilename,
         const ScannerException&      theException);

      ScriptPredefinedNameTable      predefinedNameTableM;
   };

}

#endif
