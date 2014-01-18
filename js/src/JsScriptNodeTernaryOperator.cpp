#include "JsScriptNodeTernaryOperator.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeTernaryOperator::ScriptNodeTernaryOperator(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperator,
   ScriptNode*          theOperand1,
   ScriptNode*          theOperand2,
   ScriptNode*          theOperand3)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   op1M(theOperand1),
   op2M(theOperand2),
   op3M(theOperand3),
   operatorM(theOperator)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeTernaryOperator::~ScriptNodeTernaryOperator()
{
   delete op1M;
   op1M = 0;
   delete op2M;
   op2M = 0;
   delete op3M;
   op3M = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeTernaryOperator::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   {
      switch (operatorM)
      {
         case ScriptScanner::For011E:
         {
            // ECMA-262 12.6.3
            // for ( ; op1 ; op2 ) op3
            ScriptValue result;
            while (op1M->execute(theContext).getValue() == true)
            {
               result = op3M->execute(theContext);
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
               op2M->execute(theContext);
            }
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::For101E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ; ; op2 ) op3
            ScriptValue result;
            op1M->execute(theContext);   
            while (true)
            {
               result =op3M->execute(theContext);
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
               op2M->execute(theContext);
            }
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::For110E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ; op2 ; ) op3
            ScriptValue result;
            op1M->execute(theContext);   
            while (op2M->execute(theContext).getValue() == true)
            {
               result = op3M->execute(theContext);
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
         case ScriptScanner::For111E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ; op2 ; op3 ) { ; }
            op1M->execute(theContext);   
            while (op2M->execute(theContext).getValue() == true)
            {
               op3M->execute(theContext);
            }
            return ScriptValue(ScriptValue::UndefinedE);
         }
         case ScriptScanner::ForInE:
         {
            // ECMA-262 12.6.4
            // for ( op1 in op2  ) op3
            ScriptValue op2 = op2M->execute(theContext).getValue();
            ScriptObject* object = op2.toObject();
            
            std::string propertyName;
            if (object->getNameOfFirstProperty(propertyName) == false)
            {
               // There is no first property
               return ScriptValue(ScriptValue::UndefinedE);
            }

            ScriptValue result;
            ScriptValue op1Ref = op1M->execute(theContext);
            do
            {
               op1Ref.putValue(propertyName);
               result = op3M->execute(theContext);
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
            while (object->getNameOfNextProperty(propertyName) == true);
            return ScriptValue(ScriptValue::UndefinedE);            
         }
         case ScriptScanner::IfE:
         {
            // ECMA-262 11.9.1 + 11.9.3
            // if ( op1 ) op2 else op3
            if (op1M->execute(theContext).getValue() == true)
            {
               return op2M->execute(theContext);
            }
            else
            {
               return op3M->execute(theContext);
            }
            break;
         }
         case ScriptScanner::QuestionmarkE:
         {
            // ECMA-262 11.12
            // op1 ? op2 : op3
            if (op1M->execute(theContext).getValue() == true)
            {
               return op2M->execute(theContext);
            }
            else
            {
               return op3M->execute(theContext);
            }
            break;
         }
         case ScriptScanner::TryCatchE:
         {
            // ECMA-262 12.14
            // try op1 catch ( op2 ) op3
            ScriptValue result;
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
ScriptNodeTernaryOperator::getType() const
{
   return ScriptNode::TernaryOperatorE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeTernaryOperator::print(
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
      case ScriptScanner::For011E:
      {
         theOutputString += "for ( ; op1 ; op2 ) op3 \n";
         break;
      }
      case ScriptScanner::For101E:
      {
         theOutputString += "for ( op1 ; ; op2 ) op3 \n";
         break;
      }
      case ScriptScanner::For110E:
      {
         theOutputString += "for ( op1 ; op2 ; ) op3 \n";
         break;
      }
      case ScriptScanner::For111E:
      {
         theOutputString += "for ( op1 ; op2 ; op3) ;\n";
         break;
      }
      case ScriptScanner::ForInE:
      {
         theOutputString += "for ( op1 in op2 ) op3 \n";
         break;
      }
      case ScriptScanner::IfE:
      {
         theOutputString += "if ( op1 ) op2 else op3 \n";
         break;
      }
      case ScriptScanner::QuestionmarkE:
      {
         theOutputString += "op1 ? op2 : op3\n";
         break;
      }
      case ScriptScanner::TryCatchE:
      {
         if (op3M != 0)
         {
            theOutputString += "try op1 catch ( op2 ) op3\n";
         }
         else
         {
            theOutputString += "try op1 catch ( op2 ) { }\n";
         }
         break;
      }
      default:
      {
         // empty
      }
            // ECMA-262 12.14
            // try op1 catch ( op2 ) op3
   }
   printFileAndLine(theOutputString, theMaxFilenameLength);
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
                  theLinePrefix + "   ",
                  theMaxFilenameLength);
   }
}

