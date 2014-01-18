#ifndef PALSCRIPTNODEIDDOTLITERAL_H
#define PALSCRIPTNODEIDDOTLITERAL_H

#include "JsScriptNode.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptNodeIdDotLiteral : public ScriptNode
   {
   public:

      /**
       * Construct a number literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theIdentifier               the identifier name.
       * @param theLiteral                  the literal name.
       */
      ScriptNodeIdDotLiteral(
         const ScriptString* theFile,
         int                 theLine,
         const std::string&       theIdentifier,
         const std::string&       theLitaral);

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
       * Get the name of the literal. 
       * @return the name of the literal. 
       */
      const std::string&
      getLiteral() const;

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
      ScriptNodeIdDotLiteral(
         const ScriptNodeIdDotLiteral& theOther);

      std::string identifierM;   
      std::string literalM;   
   };

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeIdDotLiteral::getIdentifier() const
   {
      return identifierM;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptNodeIdDotLiteral::getLiteral() const
   {
      return literalM;
   }
}

#endif
