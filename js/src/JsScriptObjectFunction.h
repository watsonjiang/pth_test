#ifndef PALSCRIPTOBJECTFUNCTION_H
#define PALSCRIPTOBJECTFUNCTION_H

#include "JsScriptNodeFunction.h"
#include "JsScriptObject.h"
#include "JsScriptValueArray.h"

namespace Js
{
   class ScriptObjectFunction : public ScriptObject
   {
   public:

      ScriptObjectFunction(
         ScriptNodeFunction* theFunction);

      virtual
      ~ScriptObjectFunction();

      virtual
      ScriptValue
      call(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments);

      virtual
      const char*
      getClass() const;

   private:

      ScriptNodeFunction* functionM;
   };
}

#endif
