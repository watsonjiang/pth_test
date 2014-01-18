#ifndef PALSCRIPTNODEIDENTIFIER_H
#define PALSCRIPTNODEIDENTIFIER_H

#include "JsScriptNode.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptNodeIdentifier : public ScriptNode
   {
   public:

      /**
       * Construct a number literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIdentifier               the identifier name.
       */
      ScriptNodeIdentifier(
         const ScriptString* theFile,
         int                 theLine,
         const std::string&       theIdentifier);

      /**
       * Pushes the a reference to the identifier on the stack of the
       * execution context.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);

      /**
       * Get the name of the identifier. 
       * @return the name of the identifier. 
       */
      const std::string&
      getIdentifier() const;

      /**
       * Gets the type of the node.
       * @return the type of the node. 
       */
      Type
      getType() const;

      /**
       * Get the value (name) of the identifier. 
       * @return the value (name) of the identifier. 
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

      /**
       * Copy constructor not implemented.
       */
      ScriptNodeIdentifier(
         const ScriptNodeIdentifier& theOther);

      std::string identifierM;   
   };

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeIdentifier::getIdentifier() const
   {
      return identifierM;
   }
}

#endif
