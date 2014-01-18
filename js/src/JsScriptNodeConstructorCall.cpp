#include "JsScriptNodeConstructorCall.h"
#include "JsScriptException.h"
#include "JsScriptReferenceError.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeConstructorCall::ScriptNodeConstructorCall(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptConstructorFct theConstructorFunction,
   const std::string&        theConstructorName)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   constructorFunctionM(theConstructorFunction),
   constructorNameM(theConstructorName)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeConstructorCall::~ScriptNodeConstructorCall()
{
   for (ScriptNodeArray::iterator iter = argumentsM.begin();
        iter != argumentsM.end();
        iter++)
   {
      delete *iter;
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeConstructorCall::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);

   ScriptObject* newObject;
   try
   {
      ScriptValueArray arguments;
      for (size_t i = 0; i < argumentsM.size(); i++)
      {
         arguments.push_back(argumentsM[i]->execute(theContext));
      }
      newObject = constructorFunctionM(theContext, arguments);
   }
   catch (ScriptReferenceError& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }
   catch (ScriptTypeError& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }
   catch (ScriptUserDefinedException& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }

   return ScriptValue(newObject);
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeConstructorCall::getType() const
{
   return ScriptNode::FunctionCallE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeConstructorCall::getValue() const
{
   return ScriptValue(ScriptValue::UndefinedE);
}

// ----------------------------------------------------------------------------

void
ScriptNodeConstructorCall::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }

   if (argumentsM.empty())
   {
      theOutputString += "new " + constructorNameM + "()\n";
   }
   else
   {
      theOutputString += "new " + constructorNameM + "(...)\n";
   }

   for (ScriptNodeArray::const_iterator iter = argumentsM.begin();
        iter != argumentsM.end();
        iter++)
   {
      (*iter)->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      (*iter)->print(theOutputString, 
                     theLinePrefix + " | ", 
                     theMaxFilenameLength);
   }
}

