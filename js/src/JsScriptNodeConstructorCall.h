#ifndef PALSCRIPTNODECONSTRUCTORCALL_H
#define PALSCRIPTNODECONSTRUCTORCALL_H

#include "JsScriptDefinitions.h"
#include "JsScriptNode.h"
#include "JsScriptNodeArray.h"
#include "JsScriptPredefinedNameTable.h"

namespace Js
{
   class ScriptNodeConstructorCall : public ScriptNode
   {
   public:

      ScriptNodeConstructorCall(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression,
         ScriptConstructorFct theConstructorFunction,
         const std::string&        theConstructorName);

      virtual
      ~ScriptNodeConstructorCall();

      /**
       * Execute the node. 
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

      /**
       * Get the argument array.
       */
      const ScriptNodeArray&
      getArguments() const;

      /**
       * Get the argument array.
       */
      ScriptNodeArray&
      getArguments();

      /**
       * Get the constructor function.
       * @param the constructor function.
       */
      ScriptConstructorFct
      getConstructorFunction();

      /**
       * Get the name of the constructor.
       * @param the name of the constructor.
       */
      const std::string&
      getConstructorName() const;

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

      ScriptNodeArray      argumentsM;
      ScriptConstructorFct constructorFunctionM;
      std::string               constructorNameM;
   };

   // -------------------------------------------------------------------------

   inline
   const ScriptNodeArray&
   ScriptNodeConstructorCall::getArguments() const
   {
      return argumentsM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptNodeArray&
   ScriptNodeConstructorCall::getArguments()
   {
      return argumentsM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptConstructorFct
   ScriptNodeConstructorCall::getConstructorFunction()
   {
      return constructorFunctionM;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeConstructorCall::getConstructorName() const
   {
      return constructorNameM;
   }
}

#endif
