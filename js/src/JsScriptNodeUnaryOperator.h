#ifndef PALSCRIPTNODEUNARYOPERATOR_H
#define PALSCRIPTNODEUNARYOPERATOR_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"

namespace Js
{
   class ScriptNodeUnaryOperator : public ScriptNode
   {
   public:

      /**
       * Construct a unary operator node.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression indicates if this node can
       *                                    be used as a left-hand-side 
       *                                    expression.
       * @param theOperator                 the binary operator.
       * @param theOperand1                 the operand of the operator.
       */
      ScriptNodeUnaryOperator(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptScanner::Token theOperator,
         ScriptNode*          theOperand1);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeUnaryOperator();

      /**
       * Pushes the literal on the stack of the execution context.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

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
      ScriptNodeUnaryOperator(
         const ScriptNodeUnaryOperator& theOther);

      ScriptNode*          op1M;
      ScriptScanner::Token operatorM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeUnaryOperator::getOperand1()
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeUnaryOperator::getOperand1() const
   {
      return op1M;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeUnaryOperator::getOperator() const
   {
      return operatorM;
   }
}

#endif
