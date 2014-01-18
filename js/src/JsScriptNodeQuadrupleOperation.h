#ifndef PALSCRIPTNODEQUADRUPLEOPERATION_H
#define PALSCRIPTNODEQUADRUPLEOPERATION_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptNodeQuadrupleOperation : public ScriptNode
   {
   public:

      /**
       * Construct an operation node with four operands.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression indicates if this node can
       *                                    be used as a left-hand-side 
       *                                    expression.
       * @param theOperator                 the operator.
       * @param theOperand1                 the first operand of the operator. 
       * @param theOperand2                 the second operand of the operator.
       * @param theOperand3                 the third operand of the operator.
       * @param theOperand4                 the fourth operand of the operator.
       */
      ScriptNodeQuadrupleOperation(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptScanner::Token theOperator,
         ScriptNode*          theOperand1,
         ScriptNode*          theOperand2,
         ScriptNode*          theOperand3,
         ScriptNode*          theOperand4);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeQuadrupleOperation();

      /**
       * Executes the ternary operator.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);

      /**
       * Gets the 1st operand.
       * @return the 1st operand. 
       */
      ScriptNode*
      getOperand1();

      /**
       * Gets the 1st operand.
       * @return the 1st operand. 
       */
      const ScriptNode*
      getOperand1() const;

      /**
       * Gets the 2nd operand.
       * @return the 2nd operand. 
       */
      ScriptNode*
      getOperand2();

      /**
       * Gets the 2nd operand.
       * @return the 2nd operand. 
       */
      const ScriptNode*
      getOperand2() const;

      /**
       * Gets the 3rd operand.
       * @return the 3rd operand. 
       */
      ScriptNode*
      getOperand3();

      /**
       * Gets the 3rd operand.
       * @return the 3rd operand. 
       */
      const ScriptNode*
      getOperand3() const;

      /**
       * Gets the 4th operand.
       * @return the 4th operand. 
       */
      ScriptNode*
      getOperand4();

      /**
       * Gets the 4th operand.
       * @return the 4th operand. 
       */
      const ScriptNode*
      getOperand4() const;

      /**
       * Gets the operator.
       * @return the operator. 
       */
      ScriptScanner::Token
      getOperator() const;

      /**
       * Gets the type of the node.
       * @return the type of the node. 
       */
      Type
      getType() const;

      /**
       * Print a human-readable description of the node to a string.
       * This method is mainly used to debug the parser.
       * @param theOutputString the string to which the description is
       *                        appended.
       * @param theLinePrefix   the prefix that should to be added to
       *                        each line of the description except of
       *                        the first one.
       */
      void
      print(
         std::string&       theOutputString,
         const std::string& theLinePrefix = "",
         size_t        theMaxFilenameLength = 30) const;

   private:

      /**
       * Copy constructor not implemented.
       */
      ScriptNodeQuadrupleOperation(
         const ScriptNodeQuadrupleOperation& theOther);

      ScriptNode*          op1M;
      ScriptNode*          op2M;
      ScriptNode*          op3M;
      ScriptNode*          op4M;
      ScriptScanner::Token operatorM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand1()
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand1() const
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand2()
   {
      return op2M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand2() const
   {
      return op2M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand3()
   {
      return op3M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand3() const
   {
      return op3M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand4()
   {
      return op4M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeQuadrupleOperation::getOperand4() const
   {
      return op4M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeQuadrupleOperation::getOperator() const
   {
      return operatorM;
   }
}

#endif
