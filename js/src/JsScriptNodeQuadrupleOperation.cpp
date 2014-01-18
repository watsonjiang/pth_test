#include "JsScriptNodeQuadrupleOperation.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeQuadrupleOperation::ScriptNodeQuadrupleOperation(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperator,
   ScriptNode*          theOperand1,
   ScriptNode*          theOperand2,
   ScriptNode*          theOperand3,
   ScriptNode*          theOperand4)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   op1M(theOperand1),
   op2M(theOperand2),
   op3M(theOperand3),
   op4M(theOperand4),
   operatorM(theOperator)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeQuadrupleOperation::~ScriptNodeQuadrupleOperation()
{
   delete op1M;
   op1M = 0;
   delete op2M;
   op2M = 0;
   delete op3M;
   op3M = 0;
   delete op4M;
   op4M = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeQuadrupleOperation::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   {
      switch (operatorM)
      {
         case ScriptScanner::For111E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ; op2 ; op3 ) op4
            ScriptValue result;
            op1M->execute(theContext);   
            while (op2M->execute(theContext).getValue() == true)
            {
               result = op4M->execute(theContext);
               ScriptValue::Command command = result.getCommand();
               if (command == ScriptValue::BreakE)
               {
                  break;
               }
               else
               if (command == ScriptValue::ReturnE)
               {
                  return result;
               }
               op3M->execute(theContext);
            }
            result.resetCommand();
            return result;
         }
         case ScriptScanner::TryCatchFinallyE:
         {
            // ECMA-262 12.14
            // try op1 catch ( op2 ) op3 finally op4
            ScriptValue result;
            try
            {
               try
               {
                  result = op1M->execute(theContext);
               }
               catch (ScriptException& e)
               {
                  if (op3M != 0)
                  {
                     ScriptObjectActivation* activationObject = 
                        new ScriptObjectActivation;
                  
                     activationObject->putProperty(op2M->getValue().toString(), 
                                                   e.toValue(),
                                                   ScriptProperty::DontDeleteE);

                     theContext->addObjectToScopeChain(activationObject);
                  
                     try
                     {
                        result = op3M->execute(theContext);
                     }
                     catch (ScriptException& e)
                     {
                        theContext->removeObjectFromScopeChain();
                        throw;
                     }
                     
                     theContext->removeObjectFromScopeChain();
                  }
               }
            }
            catch (ScriptException&)
            {
               if (op4M != 0)
               {
                  op4M->execute(theContext);
               }
               throw;
            }
            if (op4M != 0)
            {
               op4M->execute(theContext);
            }
            return result;
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
ScriptNodeQuadrupleOperation::getType() const
{
   return ScriptNode::QuadrupleOperationE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeQuadrupleOperation::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   switch (operatorM)
   {
      case ScriptScanner::For111E:
      {
         theOutputString += "for ( op1 ; op2 ; op3) op4\n";
         break;
      }
      case ScriptScanner::TryCatchFinallyE:
      {
         if (op3M != 0 && op4M != 0)
         {
            theOutputString += "try op1 catch ( op2 ) op3 finally op4\n";
         }
         else
         if (op3M != 0 && op4M == 0)
         {
            theOutputString += "try op1 catch ( op2 ) op3 finally { }\n";
         }
         else
         if (op3M == 0 && op4M != 0)
         {
            theOutputString += "try op1 catch ( op2 ) { } finally op4\n";
         }
         else
         if (op3M == 0 && op4M == 0)
         {
            theOutputString += "try op1 catch ( op2 ) { } finally { }\n";
         }
         break;
      }
      default:
      {
         // empty
      }
   }
   op1M->printFileAndLine(theOutputString, theMaxFilenameLength);
   theOutputString += theLinePrefix + " +-";
   op1M->print(theOutputString, 
               theLinePrefix + " | ",
               theMaxFilenameLength);
   op2M->printFileAndLine(theOutputString, theMaxFilenameLength);
   theOutputString += theLinePrefix + " +-";
   op2M->print(theOutputString, 
               theLinePrefix + " | ",
               theMaxFilenameLength);
   if (op3M != 0)
   {
      op3M->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      op3M->print(theOutputString, 
                  theLinePrefix + " | ",
                  theMaxFilenameLength);
   }
   if (op4M != 0)
   {
      op4M->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      op4M->print(theOutputString, 
                  theLinePrefix + "   ",
                  theMaxFilenameLength);
   }
}

