#ifndef PALSCRIPTTRACECONTEXT_H
#define PALSCRIPTTRACECONTEXT_H

#include "JsSmartPointer.h"
#include "JsThreadSafeSmartPointerObject.h"
#include <stdlib.h>
#include <string>

namespace Js
{
   class TraceLogRecord;
   class ScriptTraceContext : public ThreadSafeSmartPointerObject
   {
   public:

      typedef enum
      {
         NoneE = 0,
         FunctionE = 1,
         FunctionAndArgsE = 2,
         InstructionE = 3,
         PrintE = 4
      } ScriptTraceMode;
      
      ScriptTraceContext();

      /**
       * Disable the subscriber specific trace log.
       */
      void
      disableTraceLog();

      /**
       * Enable the subscriber specific trace log.
       * @param theIdentifier the identifier (e.g. MSISDN, PPP-username, ...)
       */
      bool
      enableTraceLog(
         const std::string& theIdentifier);
      
      /**
       * Get the identifier of the subscriber specific trace log.
       * @return the identifier of the subscriber specific trace log.
       */
      const std::string&
      getIdentifier() const;

      /**
       * Get level of detail of the subscriber specific trace log.
       * @return the level of detail.
       */
      int
      getLevelOfDetail() const;

      /**
       * Get the value of a tracing option.
       * @param theOptionName the name of the tracing option
       *                      (e.g. MSISDN, user name, ...).
       * @return the value of the tracing option or an empty string
       */
      std::string
      getOption(
         const std::string& theOptionName) const;

      ScriptTraceMode
      getScriptTraceMode() const;

      /**
       * Check if the trace log is enabled.
       * @return 'true' if the trace log is enabled.
       */
      bool
      isEnabled() const;

      /**
       * Check if the trace log is enabled.
       * @param theLevelOfDetail the level of detail
       * @return 'true' if the trace log is enabled
       */
      bool
      isEnabled(
         int theLevelOfDetail) const;

      /**
       * Check if a tracing option exists.
       * @param theOptionName the name of the tracing option
       *                      (e.g. MSISDN, user name, ...).
       * @return 'true' if the option exists, 'false' otherwise
       */
      bool
      optionExists(
         const std::string& theOptionName) const;

      /**
       * Write a trace log record.
       * @param theLogRecord the trace log record
       */
      void
      writeLogRecord(
         TraceLogRecord* theLogRecord);

   private:

      /**
       * Copy constructor not implemented.
       */
      ScriptTraceContext(
         const ScriptTraceContext& theTraceContext);

      /**
       * Assignment operator not implemented.
       */
      ScriptTraceContext&
      operator=(
         const ScriptTraceContext& theTraceContext);

      void
      setDefaultTraceMode();

      void
      setTraceMode();

      std::string            identifierM;
      int               levelOfDetailM;      
      ScriptTraceMode   scriptTraceModeM;
      //TraceLog*         traceLogM;
   };

   typedef SmartPointer<ScriptTraceContext> ScriptTraceContextPtr;

#ifndef _DEBUG

#  include "JsScriptTraceContext.inl"

#endif

}

#endif
