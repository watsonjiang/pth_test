#ifndef PALSCRIPT_H
#define PALSCRIPT_H

#include "JsScriptDefinitions.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptNode.h"
#include "JsScriptPredefinedNameTable.h"
#include "JsScriptPreprocessor.h"
#include "JsScriptStatistics.h"
#include "JsScriptValue.h"
#include "JsScriptValueArray.h"
#include "JsTime.h"

namespace Js
{
   class Script  
   {
   public:

      /**
       * Create the script by parsing the contents of the specified file.
       */
      Script(
         const std::string&                    theFilename,
         const ScriptPredefinedNameTable& thePredefinedNameTable);


      /**
       * Create the script by parsing the contents of the string.
       */
      Script(
         const ScriptPredefinedNameTable& thePredefinedNameTable,
         const std::string&                    theScript);
         
      virtual
      ~Script();

      void
      countFunctionCall(
         const std::string&  theFunctionName,
         unsigned int   theNumberOfExecutedNodes,
         const Time&    theStartTime);

      /**
       * Execute the script.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);
      
      /**
       * Call the specified function of the script.
       * Note that the script has to be executed once for a certain
       * execution context before a certain function can be called.
       */
      ScriptValue
      executeFunction(         
         ScriptExecutionContext* theExecutionContext,
         const std::string&           theFunctionName,
         const ScriptValueArray& theArguments);
      
      /**
       * Call the specified function of the script.
       * Note that the script has to be executed once for a certain
       * execution context before a certain function can be called.
       */
      ScriptValue
      executeFunction(         
         ScriptExecutionContext* theExecutionContext,
         const std::string&           theFunctionName);
      
      const ScriptNode*
      getCode() const;

      const std::string&
      getFilename() const;

      /**
       * Get information (filename, modification time) about the
       * files, which are included in the script.
       */
      const ScriptPreprocessor::FileMap&
      getIncludedFiles() const;

      /**
       * Print a human-readable description of the script to a string.
       * @param theOutputString the string to which the description is
       *                        appended.
       */
      void
      print(
         std::string& theOutputString) const;

      /**
       * Print statistics about the script to a string.
       * @param theOutputString the string to which the description is
       *                        appended.
       */
      void
      printStatistics(
         std::string&       theOutputString,
         time_t        theIntervalLength,
         const std::string& theLinePrefix = "") const;

      void
      resetStatistics();

   private:

      ScriptNode*                 codeM;
      std::string                 filenameM;
      Time                        fileModificationTimeM;
      ScriptPreprocessor::FileMap includedFilesM;
      mutable Time                nextFileCheckDateM;
      ScriptStatistics            statisticsM;
   };
 
   // -------------------------------------------------------------------------

   inline
   void
   Script::countFunctionCall(
      const std::string&  theFunctionName,
      unsigned int   theNumberOfExecutedNodes,
      const Time&    theStartTime)
   {
      statisticsM.countFunctionCall(theFunctionName, 
                                    theNumberOfExecutedNodes, 
                                    theStartTime);
   }  

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   Script::getCode() const
   {
      return codeM;
   }
}

#endif
