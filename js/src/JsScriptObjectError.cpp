#include "JsScriptObjectError.h"

using namespace Js;

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectError::ClassNameC = "Error";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------


ScriptObjectError::ScriptObjectError(
   ScriptValue   theMessage,
   const std::string& theName)
:  ScriptObject(ScriptObject::ErrorE)
{
   putProperty("message", theMessage);
   putProperty("name", theName);
}

// ----------------------------------------------------------------------------

ScriptObjectError::~ScriptObjectError()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObjectError::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObjectError(theArguments[0].getValue());
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectError::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectError::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObject::isInstanceOf(theClassName);
}

