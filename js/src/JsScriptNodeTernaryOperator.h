#ifndef PALSCRIPTNODETERNARYOPERATOR_H
#define PALSCRIPTNODETERNARYOPERATOR_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"

namespace Js
{
   class ScriptNodeTernaryOperator : public ScriptNode
   {
   public:

      /**
       * Construct a ternary operator node.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression indicates if this node can
       *                                    be used as a left-hand-side 
       *                                    expression.
       * @param theOperator                 the binary operator.
       * @param theOperand1                 the first operand of the operator. 
       * @param theOperand2                 the second operand of the operator.
       * @param theOperand3                 the third operand of the operator.
       */
      ScriptNodeTernaryOperator(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptScanner::Token theOperator,
         ScriptNode*          theOperand1,
         ScriptNode*          theOperand2,
         ScriptNode*          theOperand3);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeTernaryOperator();

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
      ScriptNodeTernaryOperator(
         const ScriptNodeTernaryOperator& theOther);

      ScriptNode*          op1M;
      ScriptNode*          op2M;
      ScriptNode*          op3M;
      ScriptScanner::Token operatorM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeTernaryOperator::getOperand1()
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeTernaryOperator::getOperand1() const
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeTernaryOperator::getOperand2()
   {
      return op2M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeTernaryOperator::getOperand2() const
   {
      return op2M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeTernaryOperator::getOperand3()
   {
      return op3M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeTernaryOperator::getOperand3() const
   {
      return op3M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeTernaryOperator::getOperator() const
   {
      return operatorM;
   }
}

#endif
