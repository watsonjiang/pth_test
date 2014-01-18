#ifndef PALSCRIPTNODENOARGOPERATION_H
#define PALSCRIPTNODENOARGOPERATION_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"

namespace Js
{
   class ScriptNodeNoArgOperation : public ScriptNode
   {
   public:

      /**
       * Construct an operatoration with no arguments.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression indicates if this node can
       *                                    be used as a left-hand-side 
       *                                    expression.
       * @param theOperation                the operation.
       */
      ScriptNodeNoArgOperation(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptScanner::Token theOperation);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeNoArgOperation();

      /**
       * Executes the operation.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Gets the operator.
       * @return the operator. 
       */
      ScriptScanner::Token
      getOperation() const;

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
      ScriptNodeNoArgOperation(
         const ScriptNodeNoArgOperation& theOther);

      ScriptScanner::Token operationM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptScanner::Token
   ScriptNodeNoArgOperation::getOperation() const
   {
      return operationM;
   }
}

#endif
