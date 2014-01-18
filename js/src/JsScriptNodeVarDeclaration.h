#ifndef PALSCRIPTNODEVARDECLARATION_H
#define PALSCRIPTNODEVARDECLARATION_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"

namespace Js
{
   class ScriptNodeVarDeclaration : public ScriptNode
   {
   public:

      /**
       * Construct var declaration node.
       * @param theLine         the line number (in the script) 
       *                        that created this node.
       * @param theOperator     the binary operator.
       * @param theIdentifier   the identifier.
       * @param theInitialValue the initial value.
       */
      ScriptNodeVarDeclaration(
         const ScriptString*  theFile,
         int                  theLine,
         ScriptScanner::Token theOperator,
         std::string               theIdentifier,
         ScriptNode*          theInitialValue);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeVarDeclaration();

      /**
       * Returns the result of the var declaration.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Get the name of the identifier. 
       * @return the name of the identifier. 
       */
      const std::string&
      getIdentifier() const;

      /**
       * Gets the initial value.
       * @return the initial value. 
       */
      ScriptNode*
      getInitialValue();

      /**
       * Gets the initial value.
       * @return the initial value. 
       */
      const ScriptNode*
      getInitialValue() const;

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
      ScriptNodeVarDeclaration(
         const ScriptNodeVarDeclaration& theOther);

      std::string               identifierM;
      ScriptNode*          initialValueM;
      ScriptScanner::Token operatorM;
   };

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeVarDeclaration::getIdentifier() const
   {
      return identifierM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeVarDeclaration::getInitialValue()
   {
      return initialValueM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeVarDeclaration::getInitialValue() const
   {
      return initialValueM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeVarDeclaration::getOperator() const
   {
      return operatorM;
   }
}

#endif
