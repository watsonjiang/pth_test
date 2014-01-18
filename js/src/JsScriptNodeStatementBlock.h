#ifndef PALSCRIPTNODESTATEMENTBLOCK_H
#define PALSCRIPTNODESTATEMENTBLOCK_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptScanner.h"
#include <list>

namespace Js
{
   class ScriptNodeStatementBlock : public ScriptNode
   {
   public:

      typedef std::list<ScriptNode*> StatementList;

      /**
       * Construct a binary operator node.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression Is Left hand side expression.
       * @param theOperator                 The operator.
       */
      ScriptNodeStatementBlock(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptScanner::Token theOperator);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeStatementBlock();

      /**
       * Adds a statements to the statement block.
       */
      void
      addStatement(
         ScriptNode* theStatement);

      /**
       * Checks if the statement block is empty.
       * @return 'true'  if the statement block is empty,
       *         'false' otherwise.
       */
      bool
      empty() const;

      /**
       * Executes the statements in the statement block.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Gets the operator.
       * @return the operator. 
       */
      ScriptScanner::Token
      getOperator() const;

      /**
       * Gets the statement list.
       * @return the statement list.
       */
      StatementList&
      getStatementList();

      /**
       * Gets the statement list.
       * @return the statement list.
       */
      const StatementList&
      getStatementList() const;

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

      /**
       * Removes the first statement from the statement block
       * and returns a pointer to it.
       * @return 0 if the statement block is empty,
       *         a pointer to the first statement, otherwise.
       */
      ScriptNode*
      removeFirstStatement();

      /**
       * Return the number of statements in the statement list.
       * @return The number of statements in the statement list.
       */
      int
      size() const;

   private:

      /**
       * Copy constructor not implemented.
       */
      ScriptNodeStatementBlock(
         const ScriptNodeStatementBlock& theOther);

      ScriptScanner::Token operatorM;
      StatementList        statementListM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeStatementBlock::getOperator() const
   {
      return operatorM;
   }

   // -------------------------------------------------------------------------

   inline 
   ScriptNodeStatementBlock::StatementList&
   ScriptNodeStatementBlock::getStatementList()
   {
      return statementListM;
   }

   // -------------------------------------------------------------------------

   inline 
   const ScriptNodeStatementBlock::StatementList&
   ScriptNodeStatementBlock::getStatementList() const
   {
      return statementListM;
   }
}

#endif
