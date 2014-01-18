#ifndef PALSCRIPTASYNCRESPINTERFACE_H
#define PALSCRIPTASYNCRESPINTERFACE_H

#include "JsScriptValueArray.h"
#include "JsThreadSafeSmartPointerObject.h"

namespace Js
{
   using namespace std;
   class ScriptAsyncRespInterface
   {
   public:

      virtual
      ScriptValue
      asyncResp(
         const ThreadSafeSmartPointerObject* theContextIdentifier,
         const string&                       theFunctionName,
         const ScriptValueArray&             theArguments) = 0;
   };
}

#endif
