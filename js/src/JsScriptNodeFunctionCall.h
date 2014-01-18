#ifndef PALSCRIPTNODEFUNCTIONCALL_H
#define PALSCRIPTNODEFUNCTIONCALL_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptNodeArray.h"

namespace Js
{
   class ScriptNodeFunctionCall : public ScriptNode
   {
   public:

      ScriptNodeFunctionCall(
         const ScriptString* theFile,
         int                 theLine,
         bool                theIsLeftHandSideExpression,
         ScriptNode*         theFunction);

      virtual
      ~ScriptNodeFunctionCall();

      /**
       * Adds an parameter.
       */
      void
      addArgument(
         ScriptNode* theArgument);

      /**
       * Execute the node. 
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Gets the argument list.
       * @return the argument list. 
       */
      ScriptNodeArray&
      getArgumentList();

      /**
       * Gets the argument list.
       * @return the argument list. 
       */
      const ScriptNodeArray&
      getArgumentList() const;

      /**
       * Gets the function.
       * @return the function. 
       */
      ScriptNode*
      getFunction();

      /**
       * Gets the function.
       * @return the function. 
       */
      const ScriptNode*
      getFunction() const;

      /**
       * Gets the  flag that indicates that this function is used as 
       * a constructor.
       * @return 'true'  if this function is used as a constructor.
       *         'false' otherwise.
       */
      bool
      getIsConstructorFlag() const;

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

      /**
       * Set the flag that indicates that this function is used as 
       * a constructor.
       */
      void
      setIsConstructorFlag();

   private:

      ScriptNodeArray argumentListM;
      ScriptNode*     functionM;
      bool            isConstructorM;
   };

   // -------------------------------------------------------------------------

   inline
   ScriptNodeArray&
   ScriptNodeFunctionCall::getArgumentList()
   {
      return argumentListM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNodeArray&
   ScriptNodeFunctionCall::getArgumentList() const
   {
      return argumentListM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNode*
   ScriptNodeFunctionCall::getFunction()
   {
      return functionM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptNode*
   ScriptNodeFunctionCall::getFunction() const
   {
      return functionM;
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptNodeFunctionCall::getIsConstructorFlag() const
   {
      return isConstructorM;
   }
}

#endif
