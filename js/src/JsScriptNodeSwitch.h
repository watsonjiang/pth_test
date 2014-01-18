#ifndef PALSCRIPTNODESWITCH_H
#define PALSCRIPTNODESWITCH_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptScanner.h"
#include "JsScriptValue.h"
#include <vector>

namespace Js
{
   class ScriptNodeSwitch : public ScriptNode
   {
   public:

      typedef std::vector<ScriptNode*> StatementList;

      struct CaseStatement
      {
         ScriptNode* expression;
         int         statementIndex;
      };

      typedef std::list<CaseStatement> CaseStatementList;

      /**
       * Construct an operation node with four operands.
       * @param theLine             the line number (in the script) 
       *                            that created this node.
       * @param theSwitchExpression the switch expression.
       */
      ScriptNodeSwitch(
         const ScriptString* theFile,
         int                 theLine,
         ScriptNode*         theSwitchExpression);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeSwitch();

      bool
      addCaseStatement(
         ScriptNode* theCaseExpression);

      bool
      addDefaultStatement();

      void
      addStatement(
         ScriptNode* theCaseStatement);

      /**
       * Executes the switch.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);

      /**
       * Get the case statement list.
       * @return the case statement list.
       */
      CaseStatementList
      getCaseStatements();

      /**
       * Get the case statement list.
       * @return the case statement list.
       */
      const CaseStatementList&
      getCaseStatements() const;


      /**
       * Get the index of the default statement.
       * @return the index of the default statement.
       */
      int
      getDefaultStatementIndex() const;

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
       * Gets the switch expression.
       * @return the switch expression.
       */
      ScriptNode*
      getSwitchExpression();

      /**
       * Gets the switch expression.
       * @return the switch expression.
       */
      const ScriptNode*
      getSwitchExpression() const;

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
      ScriptNodeSwitch(
         const ScriptNodeSwitch& theOther);

      CaseStatementList caseStatementsM;
      int               currentStatementIndexM;
      int               defaultStatementIndexM;
      StatementList     statementListM;
      ScriptNode*       switchExpressionM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNodeSwitch::CaseStatementList
   ScriptNodeSwitch::getCaseStatements()
   {
      return caseStatementsM;
   }
   
   // -------------------------------------------------------------------------

   inline
   const ScriptNodeSwitch::CaseStatementList&
   ScriptNodeSwitch::getCaseStatements() const
   {
      return caseStatementsM;
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptNodeSwitch::getDefaultStatementIndex() const
   {
      return defaultStatementIndexM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNodeSwitch::StatementList&
   ScriptNodeSwitch::getStatementList()
   {
      return statementListM;
   }
   
   // -------------------------------------------------------------------------

   inline
   const ScriptNodeSwitch::StatementList&
   ScriptNodeSwitch::getStatementList() const
   {
      return statementListM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeSwitch::getSwitchExpression()
   {
      return switchExpressionM;
   }
   
   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeSwitch::getSwitchExpression() const
   {
      return switchExpressionM;
   }
}

#endif
