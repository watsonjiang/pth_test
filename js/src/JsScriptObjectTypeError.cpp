#include "JsScriptObjectTypeError.h"

using namespace Js;

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectTypeError::ClassNameC = "TypeError";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObjectTypeError::ScriptObjectTypeError(
   ScriptValue   theMessage)
:  ScriptObjectError(theMessage, ClassNameC)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectTypeError::~ScriptObjectTypeError()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObjectTypeError::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObjectTypeError(theArguments[0].getValue());
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectTypeError::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectTypeError::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectError::isInstanceOf(theClassName);
}

