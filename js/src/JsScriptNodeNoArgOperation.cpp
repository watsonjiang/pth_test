#include "JsScriptNodeNoArgOperation.h"
#include "JsScriptObject.h"
#include "JsScriptObjectArray.h"
#include "JsScriptObjectDate.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeNoArgOperation::ScriptNodeNoArgOperation(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperation)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   operationM(theOperation)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeNoArgOperation::~ScriptNodeNoArgOperation()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeNoArgOperation::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   {
      switch (operationM)
      {
         case ScriptScanner::BreakE:
         {
            // ECMAS-262 12.8
            return ScriptValue(ScriptValue::BreakE, 
                               ScriptValue::UndefinedE);
         }
         case ScriptScanner::ContinueE:
         {
            // ECMAS-262 12.7
            return ScriptValue(ScriptValue::ContinueE, 
                               ScriptValue::UndefinedE);
         }
         case ScriptScanner::NewArrayE:
         {
            // ECMA-262 15.4.2.1
            // new Array ()
            ScriptValue op1(new ScriptObjectArray());
            return op1;
         }
         case ScriptScanner::NewDateE:
         {
            // ECMA-262 15.2.2.1
            // new Date ()
            ScriptValue op1(new ScriptObjectDate());
            return op1;
         }
         case ScriptScanner::NewObjectE:
         {
            // ECMA-262 15.2.2.1
            // new Object ()
            ScriptValue op1(new ScriptObject(ScriptObject::ObjectE));
            return op1;
         }
         case ScriptScanner::ReturnE:
         {
            // ECMAS-262 12.9
            return ScriptValue(ScriptValue::ReturnE, 
                               ScriptValue::UndefinedE);
         }
         default:
         {
            // empty
         }
      }
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

   return ScriptValue((Number)0);
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeNoArgOperation::getType() const
{
   return ScriptNode::NoArgOperationE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeNoArgOperation::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   switch (operationM)
   {
      case ScriptScanner::BreakE:
      {
         theOutputString += "break\n";
         break;      
      }
      case ScriptScanner::ContinueE:
      {
         theOutputString += "continue\n";
         break;      
      }
      case ScriptScanner::NewArrayE:
      {
         theOutputString += "new Array ()\n";
         break;
      }
      case ScriptScanner::NewDateE:
      {
         theOutputString += "new Date ()\n";
         break;
      }
      case ScriptScanner::NewObjectE:
      {
         theOutputString += "new Object ()\n";
         break;
      }
      case ScriptScanner::ReturnE:
      {
         theOutputString += "return\n";
         break;
      }
      default:
      {
         // empty
      }
   }
}

