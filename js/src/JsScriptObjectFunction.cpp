#include "JsScriptObjectFunction.h"

using namespace Js;

static const string PrototypeC("prototype");

ScriptObjectFunction::ScriptObjectFunction(
   ScriptNodeFunction* theFunction)
:  ScriptObject(ScriptObject::FunctionE),
   functionM(theFunction)
{
   ScriptObject* prototype = new ScriptObject(ScriptObject::ObjectE);
   putPropertyNoCheck(
      PrototypeC,
      ScriptValue(prototype),
      ScriptProperty::DontEnumE | 
      ScriptProperty::DontDeleteE | 
      ScriptProperty::ReadOnlyE);
}

// ----------------------------------------------------------------------------

ScriptObjectFunction::~ScriptObjectFunction()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectFunction::call(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return functionM->call(theContext, theArguments);
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectFunction::getClass() const
{
   // ECMA-262 8.6.2
   return "Function";
}

