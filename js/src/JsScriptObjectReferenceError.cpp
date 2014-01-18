#include "JsScriptObjectReferenceError.h"

using namespace Js;

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectReferenceError::ClassNameC = "ReferenceError";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObjectReferenceError::ScriptObjectReferenceError(
   ScriptValue   theMessage)
:  ScriptObjectError(theMessage, ClassNameC)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectReferenceError::~ScriptObjectReferenceError()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObjectReferenceError::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObjectReferenceError(theArguments[0].getValue());
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectReferenceError::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectReferenceError::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectError::isInstanceOf(theClassName);
}


