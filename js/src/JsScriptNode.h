#ifndef PALSCRIPTNODE_H
#define PALSCRIPTNODE_H

#include "JsScriptExecutionContext.h"
#include "JsScriptString.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptNode
   {
   public:

      typedef enum
      {
         BinaryOperatorE,
         FunctionE,
         FunctionCallE,
         IdDotLiteralE,
         IdentifierE,
         LiteralE,
         NoArgOperationE,
         QuadrupleOperationE,
         StatementBlockE,
         SwitchE,
         TernaryOperatorE,
         UnaryOperatorE,
         VarDeclarationE,
         ArrayInitialiserE,
         ObjectInitialiserE
      } Type;

      /**
       * Constructor.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIsLeftHandSideExpression indicates if this node can
       *                                    be used as a left-hand-side 
       *                                    expression.
       */
      ScriptNode(
         const ScriptString* theFile,
         int                 theLine,
         bool                theIsLeftHandSideExpression = false);

      virtual
      ~ScriptNode();

      /**
       * Execute the node. 
       */
      virtual
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext) = 0;

      /**
       * Gets the file that created this node.
       * @return the file. 
       */
      const ScriptString*
      getFile() const;

      /**
       * Gets the line of the script that created this node.
       * @return the line of the script. 
       */
      int
      getLine() const;

      /**
       * Gets the type of the node.
       * @return the type of the node. 
       */
      virtual
      Type
      getType() const = 0;

      /**
       * Get the value of the node. 
       * @return the value of the node evaluation. 
       */
      virtual
      ScriptValue
      getValue() const;

      /**
       * Checks if the value of the node can be used as
       * the left-hand-side of an assignment expression. 
       * @return 'true'  if the value can be used as the left-hand-side
       *                 of an assignment expression, 
       *         'false' otherwise. 
       */
      bool
      isLeftHandSideExpression() const;

      /**
       * Print a human-readable description of the node to a string.
       * This method is mainly used to debug the parser.
       * @param theOutputString the string to which the description is
       *                        appended.
       * @param theLinePrefix   the prefix that should to be added to
       *                        each line of the description except of
       *                        the first one.
       */
      virtual
      void
      print(
         std::string&       theOutputString,
         const std::string& theLinePrefix = "",
         size_t        theMaxFilenameLength = 30) const = 0;
         
      virtual
      void
      printFileAndLine(
         std::string& theOutputString,
         size_t  theMaxFilenameLength = 30) const;
         
      /**
       * Sets the line and file of the script that created this node.
       * @param theLine The line of the script. 
       * @param theFile The file. 
       */
      void
      setLine(
         int                 theLine,
         const ScriptString* theFile);

   private:
  
      ScriptStringPtr fileM;
      bool            isLeftHandSideExpressionM;
      int             lineM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNode::ScriptNode(
      const ScriptString* theFile,
      int                 theLine,
      bool                theIsLeftHandSideExpression)
   :  fileM(theFile),
      isLeftHandSideExpressionM(theIsLeftHandSideExpression),
      lineM(theLine)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptString*
   ScriptNode::getFile() const
   {
      return fileM;
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptNode::getLine() const
   {
      return lineM;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptNode::setLine(
      int                 theLine,
      const ScriptString* theFile)
   {
      lineM = theLine;
   }

}

#endif
