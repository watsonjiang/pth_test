#ifndef PALSCRIPTNODEFUNCTION_H
#define PALSCRIPTNODEFUNCTION_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptValueArray.h"
#include <list>

namespace Js
{
   class ScriptNodeFunction : public ScriptNode
   {
   public:

      typedef std::list<std::string>      ParameterList;
      typedef std::list<ScriptNode*> StatementList;

      ScriptNodeFunction(
         const ScriptString* theFile,
         int                 theLine,
         bool                theIsLeftHandSideExpression,
         const std::string&       theName,
         bool                theNameIsVisible);

      virtual
      ~ScriptNodeFunction();

      /**
       * Adds a formal parameter.
       */
      void
      addParameter(
         const std::string& theParameterName);

      /**
       * Adds a statements to the function body.
       */
      void
      addStatement(
         ScriptNode* theStatement);

      /**
       * Call the function. 
       */
      ScriptValue
      call(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments);

      /**
       * Execute the node. 
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);

      /**
       * Get the name of the function. 
       * @return the name of the function. 
       */
      const std::string&
      getName() const;

      /**
       * Get the 'name-is-visible' flag. 
       * @return 'true' if the name is visible, 'false' otherwise. 
       */
      bool
      getNameIsVisibleFlag() const;

      /**
       * Gets the parameter list.
       * @return the parameter list. 
       */
      const ParameterList&
      getParameterList() const;

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
       * Get the value of the node. 
       * @return the value of the node evaluation. 
       */
      ScriptValue
      getValue() const;

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

      std::string        nameM;
      bool          nameIsVisibleM;
      ParameterList parameterListM;
      StatementList statementListM;
   };

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeFunction::getName() const
   {
      return nameM;
   }
   
   // -------------------------------------------------------------------------

   inline
   bool
   ScriptNodeFunction::getNameIsVisibleFlag() const
   {
      return nameIsVisibleM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNodeFunction::ParameterList&
   ScriptNodeFunction::getParameterList() const
   {
      return parameterListM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNodeFunction::StatementList&
   ScriptNodeFunction::getStatementList()
   {
      return statementListM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNodeFunction::StatementList&
   ScriptNodeFunction::getStatementList() const
   {
      return statementListM;
   }
}

#endif
