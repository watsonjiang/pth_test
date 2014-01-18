#ifndef PALSCRIPTEXECUTIONTIMEEXCEEDED_H
#define PALSCRIPTEXECUTIONTIMEEXCEEDED_H

#include "JsScriptDefinitions.h"
#include <exception>
#include <string>

namespace Js
{
   class ScriptNode;

   class ScriptExecutionTimeExceeded : public std::exception
   {
   public:

      ScriptExecutionTimeExceeded(
         ScriptNode* theScriptNode);

      virtual
      ~ScriptExecutionTimeExceeded()
         throw ();

      const ScriptNode*
      getScriptNode() const;

   private:
      std::string errorTextM;
      ScriptNode* scriptNodeM;
   };
}

#endif
