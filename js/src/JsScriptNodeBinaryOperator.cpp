#include "JsScriptException.h"
#include "JsScriptNodeBinaryOperator.h"
#include "JsScriptObject.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeBinaryOperator::ScriptNodeBinaryOperator(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression,
   ScriptScanner::Token theOperator,
   ScriptNode*          theOperand1,
   ScriptNode*          theOperand2)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   op1M(theOperand1),
   op2M(theOperand2),
   operatorM(theOperator)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeBinaryOperator::~ScriptNodeBinaryOperator()
{
   delete op1M;
   op1M = 0;
   delete op2M;
   op2M = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeBinaryOperator::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   { 
      switch (operatorM)
      {
         case ScriptScanner::LessThanE:
         {
            // ECMA-262 11.8.1
            // op1 '<' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 < op2;
         }
         case ScriptScanner::GreaterThanE:
         {
            // ECMA-262 11.8.2
            // op1 '>' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return op1 > op2;
         }
         case ScriptScanner::LessThanOrEqualToE:
         {
            // ECMA-262 11.8.3
            // op1 '<=' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 <= op2;
         }
         case ScriptScanner::GreaterThanOrEqualToE:
         {
            // ECMA-262 11.8.4
            // op1 '>=' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 >= op2;
         }
         case ScriptScanner::InstanceofE:
         {
            break;
         }
         case ScriptScanner::AdditionE:
         {
            // ECMA-262 11.6.1
            // op1 '+' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return op1 + op2;
         }
         case ScriptScanner::AdditionAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '+=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() + op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::AssignmentE:
         {
            // ECMA-262 11.13.1
            // op1 '=' op2
            ScriptValue op1Ref(op1M->execute(theContext));

            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            op1Ref.putValue(op2);
            return op2;
         }
         case ScriptScanner::BitwiseAndE:
         {
            // ECMA-262 11.10
            // op1 '&' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 & op2;
         }
         case ScriptScanner::BitwiseAndAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '&=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() & op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::BitwiseLeftShiftE:
         {
            //ECMA-262 11.7.1
            // op1 '<<' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 << op2;
         }
         case ScriptScanner::BitwiseLeftShiftAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '<<=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() << op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::BitwiseOrE:
         {
            // ECMA-262 11.10
            // op1 '|' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 | op2;
         }
         case ScriptScanner::BitwiseOrAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '|=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() | op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::BitwiseRightShiftE:
         {
            //ECMA-262 11.7.1
            // op1 '>>' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 >> op2;
         }
         case ScriptScanner::BitwiseRightShiftWithZeroFillE:
         {
            //ECMA-262 11.7.1
            // op1 '>>>' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1.shiftRightWithZeroFill(op2);
         }
         case ScriptScanner::BitwiseRightShiftAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '>>=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() >> op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::BitwiseRightShiftWithZeroFillAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '>>>=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue().shiftRightWithZeroFill(op2));
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::BitwiseXorE:
         {
            // ECMA-262 11.10
            // op1 '^' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 ^ op2;
         }
         case ScriptScanner::BitwiseXorAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '&=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
       
            ScriptValue result(op1Ref.getValue() ^ op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::DivisionE:
         {
            // ECMA-262 11.5.2
            // op1 '/' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return op1 / op2;
         }
         case ScriptScanner::DivisionAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '/=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            ScriptValue result(op1Ref.getValue() / op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::DoE:
         {
            // ECMA-262 12.6.2
            // do op1 while ( op2 )
            ScriptValue result;
            do
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
            while (op2M->execute(theContext).getValue() == true);        
            result.resetCommand();
            return result;
         }
         case ScriptScanner::DotE:
         {
            // ECMA-262 11.2.1
            // op1 '.' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
     
            return ScriptValue(op1, op2);
         }
         case ScriptScanner::EqualityE:
         {
            // ECMA-262 11.9.1 + 11.9.3
            // op1 '==' op2
            ScriptValue op1(op1M->execute(theContext));  
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
 
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 == op2;
         }
         case ScriptScanner::For001E:
         {
            // ECMA-262 12.6.3
            // for (  ;  ; op1 ) op2
            ScriptValue result;
            while (true)
            {
               result = op2M->execute(theContext);
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
               op1M->execute(theContext);
            }
            result.resetCommand();
            return result;
         }
         case ScriptScanner::For010E:
         {
            // ECMA-262 12.6.3
            // for ( ; op1 ; ) op2
            ScriptValue result;
            while (op1M->execute(theContext).getValue() == true)
            {
               result = op2M->execute(theContext);
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
            result.resetCommand();
            return ScriptValue::UndefinedC;
         }
         case ScriptScanner::For011E:
         {
            // ECMA-262 12.6.3
            // for ( ; op1 ; op2 ) { ; }
            while (op1M->execute(theContext).getValue() == true)
            {
               op2M->execute(theContext);
            }
            return ScriptValue::UndefinedC;
         }
         case ScriptScanner::For100E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ;  ;  ) op2
            op1M->execute(theContext);   
            ScriptValue result;
            while (true)
            {
               result = op2M->execute(theContext);
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
            result.resetCommand();
            return result;
         }
         case ScriptScanner::For110E:
         {
            // ECMA-262 12.6.3
            // for ( op1 ; op2 ; ) { ; }
            op1M->execute(theContext);   
            while (op2M->execute(theContext).getValue() == true)
            {
               // Empty
            }
            return ScriptValue::UndefinedC;
         }
         case ScriptScanner::ForInE:
         {
            // ECMA-262 12.6.4
            // for ( op1 in op2  ) ;
            ScriptValue op2(op2M->execute(theContext).getValue());
            ScriptObject* object = op2.toObject();
            
            std::string propertyName;
            if (object->getNameOfFirstProperty(propertyName) == false)
            {
               // There is no first property
               return ScriptValue::UndefinedC;
            }

            ScriptValue op1Ref(op1M->execute(theContext));
            do
            {
               op1Ref.putValue(propertyName);
            }
            while (object->getNameOfNextProperty(propertyName) == true);
            return ScriptValue::UndefinedC;            
         }
         case ScriptScanner::IfE:
         {
            // ECMA-262 11.9.1 + 11.9.3
            // if ( op1 ) op2
            ScriptValue result(op1M->execute(theContext));
            if (result.getDataType() == ScriptValue::ReferenceE)
            {
               result = result.getReferenceValue();
            }
            
            if (result.toBoolean() == true)
            {
               return op2M->execute(theContext);
            }
            return ScriptValue::UndefinedC;
         }
         case ScriptScanner::IfNotE:
         {
            // ECMA-262 11.9.1 + 11.9.3
            // if ( !op1 ) op2
            ScriptValue result(op1M->execute(theContext));
            if (result.getDataType() == ScriptValue::ReferenceE)
            {
               result = result.getReferenceValue();
            }
            
            if (result.toBoolean() == false)
            {
               return op2M->execute(theContext);
            }
            return ScriptValue::UndefinedC;
         }
         case ScriptScanner::InE:
         {
            // ECMA-262 11.8.7
            // op1 'in' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            if (op2.getDataType() != ScriptValue::ObjectE)
            {
               throw ScriptTypeError(
                  "Operand on right-hand side of 'in' operator is no object.",
                  getFile(),
                  getLine());
            }
            bool result = op2.toObject()->hasProperty(op1.toString());
            return ScriptValue(result);
         }
         case ScriptScanner::InequalityE:
         {
            // ECMA-262 11.9.2 + 11.9.3
            // op1 '!=' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return !(op1 == op2);
         }
         case ScriptScanner::LeftBracketE: 
         {
            // ECMA-262 11.2.1
            // op1 '[' op2 ']'
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return ScriptValue(op1, op2);
         }
         case ScriptScanner::LogicalAndE:
         {
            // ECMA-262 11.11
            // op1 '&&' op2
            if (op1M->execute(theContext).getValue() == false)
            {
               return ScriptValue(false);
            }
            else
            {
               if (op2M->execute(theContext).getValue() == false)
               {
                  return ScriptValue(false);
               }
               else
               {
                  return ScriptValue(true);
               }
            }
            break;
         }
         case ScriptScanner::LogicalOrE:
         {
            // ECMA-262 11.11
            // op1 '||' op2
            if (op1M->execute(theContext).getValue() == true)
            {
               return ScriptValue(true);
            }
            else
            {
               if (op2M->execute(theContext).getValue() == true)
               {
                  return ScriptValue(true);
               }
               else
               {
                  return ScriptValue(false);
               }
            }
            break;
         }
         case ScriptScanner::ModulusE:
         {
            // ECMA-262 11.5.3
            // op1 '%' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1 % op2;
         }
         case ScriptScanner::ModulusAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '*=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            ScriptValue result(op1Ref.getValue() % op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::MultiplicationE:
         {
            // ECMA-262 11.5.1
            // op1 '*' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return op1 * op2;
         }
         case ScriptScanner::MultiplicationAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '*=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            ScriptValue result(op1Ref.getValue() * op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::StrictEqualityE:
         {
            // ECMA-262 11.9.4 + 11.9.6
            // op1 '===' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return op1.strictlyEqual(op2);
         }
         case ScriptScanner::StrictInequalityE:
         {
            // ECMA-262 11.9.5 + 11.9.6
            // op1 '!==' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }

            return !op1.strictlyEqual(op2);
         }
         case ScriptScanner::SubtractionE:
         {
            // ECMA-262 11.6.2
            // op1 '-' op2
            ScriptValue op1(op1M->execute(theContext));
            if (op1.getDataType() == ScriptValue::ReferenceE)
            {
               op1 = op1.getReferenceValue();
            }
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            return op1 - op2;
         }
         case ScriptScanner::SubtractionAssignmentE:
         {
            // ECMA-262 11.13.2
            // op1 '-=' op2
            ScriptValue op1Ref(op1M->execute(theContext));
            ScriptValue op2(op2M->execute(theContext));
            if (op2.getDataType() == ScriptValue::ReferenceE)
            {
               op2 = op2.getReferenceValue();
            }
         
            ScriptValue result(op1Ref.getValue() - op2);
            op1Ref.putValue(result);
            return result;
         }
         case ScriptScanner::TryFinallyE:
         {
            // ECMA-262 12.14
            // try op1 finally op2
            ScriptValue result;
            try
            {
               result = op1M->execute(theContext);
               if (op2M != 0)
               {
                  op2M->execute(theContext);
               }
               return result;
            }
            catch (ScriptException&)
            {
               if (op2M != 0)
               {
                  op2M->execute(theContext);
               }
               throw;
            }
         }
         case ScriptScanner::WithE:
         {
            // ECMA-262 12.10
            // with ( op1 ) op2
            ScriptObject* obj = op1M->execute(theContext).getValue().toObject();

            ScriptValue result;
            theContext->addObjectToScopeChain(obj);
            try
            {
               result = op2M->execute(theContext);
            }
            catch (ScriptException&)
            {
               theContext->removeObjectFromScopeChain();
               throw;
            }
            theContext->removeObjectFromScopeChain();
            return result;
         }
         case ScriptScanner::WhileE:
         {
            // ECMA-262 12.6.2
            // while ( op1 ) op2
            ScriptValue result;
            while (op1M->execute(theContext).getValue() == true)
            {
               result = op2M->execute(theContext);
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
            result.resetCommand();
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
   ScriptValue result;
   return result;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeBinaryOperator::getType() const
{
   return ScriptNode::BinaryOperatorE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeBinaryOperator::print(
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
      case ScriptScanner::LessThanE:
      {
         theOutputString += "op1 < op2\n";
         break;
      }
      case ScriptScanner::GreaterThanE:
      {
         theOutputString += "op1 > op2\n";
         break;
      }
      case ScriptScanner::LessThanOrEqualToE:
      {
         theOutputString += "op1 <= op2\n";
         break;
      }
      case ScriptScanner::GreaterThanOrEqualToE:
      {
         theOutputString += "op1 >= op2\n";
         break;
      }
      case ScriptScanner::InstanceofE:
      {
         theOutputString += "op1 instanceof op2\n";
         break;
      }
      case ScriptScanner::AdditionE:
      {
         theOutputString += "op1 + op2\n";
         break;
      }
      case ScriptScanner::AdditionAssignmentE:
      {
         theOutputString += "op1 += op2\n";
         break;
      }
      case ScriptScanner::AssignmentE:
      {
         theOutputString += "op1 = op2\n";
         break;
      }
      case ScriptScanner::BitwiseAndE:
      {
         theOutputString += "op1 & op2\n";
         break;
      }
      case ScriptScanner::BitwiseAndAssignmentE:
      {
         theOutputString += "op1 &= op2\n";
         break;
      }
      case ScriptScanner::BitwiseLeftShiftE:
      {
         theOutputString += "op1 << op2\n";
         break;
      }
      case ScriptScanner::BitwiseLeftShiftAssignmentE:
      {
         theOutputString += "op1 <<= op2\n";
         break;
      }
      case ScriptScanner::BitwiseOrE:
      {
         theOutputString += "op1 | op2\n";
         break;
      }
      case ScriptScanner::BitwiseRightShiftE:
      {
         theOutputString += "op1 >> op2\n";
         break;
      }
      case ScriptScanner::BitwiseRightShiftAssignmentE:
      {
         theOutputString += "op1 >>= op2\n";
         break;
      }
      case ScriptScanner::BitwiseXorE:
      {
         theOutputString += "op1 ^ op2\n";
         break;
      }
      case ScriptScanner::BitwiseXorAssignmentE:
      {
         theOutputString += "op1 ^= op2\n";
         break;
      }
      case ScriptScanner::DivisionE:
      {
         theOutputString += "op1 / op2\n";
         break;
      }
      case ScriptScanner::DivisionAssignmentE:
      {
         theOutputString += "op1 /= op2\n";
         break;
      }
      case ScriptScanner::DoE:
      {
         theOutputString += "do op1 while ( op2 )\n";
         break;
      }
      case ScriptScanner::DotE:
      {
         theOutputString += "op1 . op2\n";
         break;
      }
      case ScriptScanner::EqualityE:
      {
         theOutputString += "op1 == op2\n";
         break;
      }
      case ScriptScanner::For001E:
      {
         theOutputString += "for ( ; ; op1 ) op2\n";
         break;
      }
      case ScriptScanner::For010E:
      {
         theOutputString += "for ( ; op1 ; ) op2\n";
         break;
      }
      case ScriptScanner::For011E:
      {
         theOutputString += "for ( ; op1 ; op2 ) ; \n";
         break;
      }
      case ScriptScanner::For100E:
      {
         theOutputString += "for ( op1 ; ; ) op2\n";
         break;
      }
      case ScriptScanner::For110E:
      {
         theOutputString += "for ( op1 ; op2 ; ) ; \n";
         break;
      }
      case ScriptScanner::IfE:
      {
         theOutputString += "if ( op1 ) op2\n";
         break;
      }
      case ScriptScanner::IfNotE:
      {
         theOutputString += "if ( ! op1 ) op2\n";
         break;
      }
      case ScriptScanner::InE:
      {
         theOutputString += "op1 in op2\n";
         break;
      }
      case ScriptScanner::InequalityE:
      {
         theOutputString += "op1 != op2\n";
         break;
      }
      case ScriptScanner::StrictEqualityE:
      {
         theOutputString += "op1 === op2\n";
         break;
      }
      case ScriptScanner::StrictInequalityE:
      {
         theOutputString += "op1 !== op2\n";
         break;
      }
      case ScriptScanner::LeftBracketE: 
      {
         theOutputString += "op1 [ op2 ]\n";
         break;
      }
      case ScriptScanner::LogicalAndE:
      {
         theOutputString += "op1 && op2\n";
         break;
      }
      case ScriptScanner::LogicalOrE:
      {
         theOutputString += "op1 || op2\n";
         break;
      }
      case ScriptScanner::ModulusE:
      {
         theOutputString += "op1 % op2\n";
         break;
      }
      case ScriptScanner::ModulusAssignmentE:
      {
         theOutputString += "op1 %= op2\n";
         break;
      }
      case ScriptScanner::MultiplicationE:
      {
         theOutputString += "op1 * op2\n";
         break;
      }
      case ScriptScanner::MultiplicationAssignmentE:
      {
         theOutputString += "op1 *= op2\n";
         break;
      }
      case ScriptScanner::SubtractionE:
      {
         theOutputString += "op1 - op2\n";
         break;
      }
      case ScriptScanner::SubtractionAssignmentE:
      {
         theOutputString += "op1 -= op2\n";
         break;
      }
      case ScriptScanner::TryFinallyE:
      {
         if (op2M != 0)
         {
            theOutputString += "try op1 finally op2\n";
         }
         else
         {
            theOutputString += "try op1 finally { }\n";
         }
         break;
      }
      case ScriptScanner::WithE:
      {
         theOutputString += "with ( op1 ) op2\n";
         break;
      }
      case ScriptScanner::WhileE:
      {
         theOutputString += "while ( op1 ) op2\n";
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
   if (op2M != 0)
   {
      op2M->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      op2M->print(theOutputString, 
                  theLinePrefix + "   ", 
                  theMaxFilenameLength);
   }
}


