#include "JsScriptObjectUriError.h"

using namespace Js;

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectUriError::ClassNameC = "URIError";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObjectUriError::ScriptObjectUriError(
   ScriptValue   theMessage)
:  ScriptObjectError(theMessage, ClassNameC)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectUriError::~ScriptObjectUriError()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObjectUriError::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObjectUriError(theArguments[0].getValue());
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectUriError::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectUriError::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectError::isInstanceOf(theClassName);
}

