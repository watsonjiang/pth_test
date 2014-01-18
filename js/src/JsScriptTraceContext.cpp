#include "JsScriptTraceContext.h"

using namespace Js;

#ifdef _DEBUG

#  define inline
#  include "JsScriptTraceContext.inl"

#endif

void
ScriptTraceContext::setDefaultTraceMode()
{
   //scriptTraceModeM = (ScriptTraceMode)ScriptManager::instance()->getTraceMode();
}

// ----------------------------------------------------------------------------

void
ScriptTraceContext::setTraceMode()
{
   /* TODO: implement setTraceMode
   std::string scriptOption = traceLogM->getOption(identifierM, "script");
   if (scriptOption.empty() == true)
   {
      setDefaultTraceMode();
   }
   else
   {
      scriptTraceModeM = (ScriptTraceMode)atoi(scriptOption.c_str());
      if (scriptTraceModeM < 0)
      {
         scriptTraceModeM = NoneE;
      }
      else
      if (scriptTraceModeM > 4)
      {
         scriptTraceModeM = PrintE;
      }
   }
   */
}
