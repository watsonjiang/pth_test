#include "JsScriptExecutionTimeExceeded.h"
#include "JsScriptNode.h"

using namespace Js;

ScriptExecutionTimeExceeded::ScriptExecutionTimeExceeded(
   ScriptNode* theScriptNode)
:  scriptNodeM(theScriptNode)
{
   errorTextM = "Maximum execution time of script exceeded. Current line: ";
   char buf[10] = {0};
   snprintf(buf, 10, "%d", scriptNodeM->getLine());
   errorTextM.append(buf);
}

// ----------------------------------------------------------------------------

ScriptExecutionTimeExceeded::~ScriptExecutionTimeExceeded()
   throw ()
{
   // Empty
}

// ----------------------------------------------------------------------------

const ScriptNode*
ScriptExecutionTimeExceeded::getScriptNode() const
{
   return scriptNodeM;
}
