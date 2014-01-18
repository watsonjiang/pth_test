#include "JsScriptNodeUnaryOperator.h"
#include "JsScriptObjectError.h"
#include "JsScriptObjectReferenceError.h"
#include "JsScriptObjectTypeError.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeUnaryOperator::ScriptNodeUnaryOperator(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperator,
   ScriptNode*          theOperand1)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   op1M(theOperand1),
   operatorM(theOperator)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeUnaryOperator::~ScriptNodeUnaryOperator()
{
   delete op1M;
   op1M = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeUnaryOperator::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   {
      switch (operatorM)
      {
         case ScriptScanner::BitwiseNotE:
         {
            // ECMA-262 11.4.8
            // '~' op1
            ScriptValue op1 = op1M->execute(theContext).getValue();
            
            return ~op1;
         }
         case ScriptScanner::DecrementE:
         {
            // ECMA-262 11.4.5
            // '--' op1
            ScriptValue op1Ref = op1M->execute(theContext);
            
            ScriptValue result = op1Ref.getValue() - 1;
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::DeleteE:
         {
            // ECMA-262 11.4.1
            // 'delete' op1
            ScriptValue op1Ref = op1M->execute(theContext);
            return op1Ref.deleteOperator();
         }
         case ScriptScanner::DoE:
         {
            // ECMA-262 12.6.2
            // do { ; } while ( op1 )
            do
            {
               // Empty
            }
            while (op1M->execute(theContext).getValue() == true);        
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::For000E:
         {
            // ECMA-262 12.6.3
            // for ( ;  ;  ) op1
            ScriptValue result;
            while (true)
            {
               result = op1M->execute(theContext);
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
            }
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::For010E:
         {
            // ECMA-262 12.6.3
            // for ( ; op1 ; ) { ; }
            while (op1M->execute(theContext).getValue() == true)
            {
               // Empty
            }
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::IncrementE:
         {
            // ECMA-262 11.4.4
            // '++' op1
            ScriptValue op1Ref = op1M->execute(theContext);
            
            ScriptValue result = op1Ref.getValue() + 1;
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::LogicalNotE:
         {
            // ECMA-262 11.4.9
            // '!' op1
            ScriptValue op1 = op1M->execute(theContext).getValue();
            
            return !op1;
         }
         case ScriptScanner::NewErrorE:
         {
            // ECMA-262 15.11.2.1
            // new Error ( op1 )
            ScriptValue op1 = op1M->execute(theContext).getValue();

            ScriptValue result(new ScriptObjectError(op1));
            return result;
         }
         case ScriptScanner::NewReferenceErrorE:
         {
            // ECMA-262 15.11.6.3
            // new ReferenceError ( op1 )
            ScriptValue op1 = op1M->execute(theContext).getValue();

            ScriptValue result(new ScriptObjectReferenceError(op1));
            return result;
         }
         case ScriptScanner::NewTypeErrorE:
         {
            // ECMA-262 15.11.6.3
            // new TypeError ( op1 )
            ScriptValue op1 = op1M->execute(theContext).getValue();

            ScriptValue result(new ScriptObjectTypeError(op1));
            return result;
         }
         case ScriptScanner::PostDecrementE:
         {
            // ECMA-262 11.3.2
            // op1 '--'
            ScriptValue op1Ref = op1M->execute(theContext);
            
            ScriptValue result = op1Ref.getValue();
            op1Ref.putValue(result - 1);
            return result;
         }
         case ScriptScanner::PostIncrementE:
         {
            // ECMA-262 11.3.1
            // op1 '++'
            ScriptValue op1Ref = op1M->execute(theContext);
            
            ScriptValue result = op1Ref.getValue();
            op1Ref.putValue(result + 1);
            return result;
         }
         case ScriptScanner::ReturnE:
         {
            // ECMAS-262 12.9
            ScriptValue op1 = op1M->execute(theContext).getValue();
            op1.setCommand(ScriptValue::ReturnE);

            return op1;
         }
         case ScriptScanner::SubtractionE:
         {
            // ECMA-262 11.4.7
            // '-' op1
            ScriptValue op1 = op1M->execute(theContext).getValue();
            
            return -op1;
         }
         case ScriptScanner::ThrowE:
         {
            // 'throw' op1 
            ScriptValue op1 = op1M->execute(theContext).getValue();

            throw ScriptUserDefinedException(op1, getFile(), getLine());
         }
         case ScriptScanner::TypeofE:
         {
            // ECMA-262 11.4.3
            // 'typeof' op1
            ScriptValue op1 = op1M->execute(theContext).getValue();
            return op1.getTypeof();
         }
         case ScriptScanner::VoidE:
         {
            // ECMA-262 11.4.2
            // 'void' op1
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::WhileE:
         {
            // ECMA-262 12.6.2
            // while ( op1 ) { ; }
            while (op1M->execute(theContext).getValue() == true)
            {
               // Empty
            }
            return ScriptValue(ScriptValue::UndefinedE);
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

   return ScriptValue::UndefinedC;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeUnaryOperator::getType() const
{
   return ScriptNode::UnaryOperatorE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeUnaryOperator::print(
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
      case ScriptScanner::BitwiseNotE:
      {
         theOutputString += "~ op1\n";
         break;
      }
      case ScriptScanner::DecrementE:
      {
         theOutputString += "-- op1\n";
         break;
      }
      case ScriptScanner::DeleteE:
      {
         theOutputString += "delete op1\n";
         break;
      }
      case ScriptScanner::DoE:
      {
         theOutputString += "do { ; } while ( op1 )\n";
         break;
      }
      case ScriptScanner::For000E:
      {
         theOutputString += "for ( ; ; ) op1\n";
         break;
      }
      case ScriptScanner::For010E:
      {
         theOutputString += "for ( ; op1 ; ) ;\n";
         break;
      }
      case ScriptScanner::IncrementE:
      {
         theOutputString += "++ op1\n";
         break;
      }
      case ScriptScanner::LogicalNotE:
      {
         theOutputString += "! op1\n";
         break;
      }
      case ScriptScanner::NewErrorE:
      {
         theOutputString += "new Error ( op 1)\n";
         break;
      }
      case ScriptScanner::NewReferenceErrorE:
      {
         theOutputString += "new ReferenceError ( op 1)\n";
         break;
      }
      case ScriptScanner::NewTypeErrorE:
      {
         theOutputString += "new TypeError ( op 1)\n";
         break;
      }
      case ScriptScanner::PostDecrementE:
      {
         theOutputString += "op1 --\n";
         break;
      }
      case ScriptScanner::PostIncrementE:
      {
         theOutputString += "op1 ++\n";
         break;
      }
      case ScriptScanner::ReturnE:
      {
         theOutputString += "return op1\n";
         break;
      }
      case ScriptScanner::SubtractionE:
      {
         theOutputString += "- op1\n";
         break;
      }
      case ScriptScanner::ThrowE:
      {
         theOutputString += "throw op1\n";
         break;
      }
      case ScriptScanner::TypeofE:
      {
         theOutputString += "typeof op1\n";
         break;
      }
      case ScriptScanner::VoidE:
      {
         theOutputString += "void op1\n";
         break;
      }
      case ScriptScanner::WhileE:
      {
         theOutputString += "while ( op1 ) ;\n";
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
               theLinePrefix + "   ",
               theMaxFilenameLength);
}

