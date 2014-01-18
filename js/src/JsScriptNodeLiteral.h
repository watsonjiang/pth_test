#ifndef PALSCRIPTNODELITERAL_H
#define PALSCRIPTNODELITERAL_H

#include "JsScriptNode.h"
#include "JsScriptValue.h"

namespace Js
{
   class ScriptNodeLiteral : public ScriptNode
   {
   public:

      /**
       * Construct a null literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       */
      ScriptNodeLiteral(
         const ScriptString* theFile,
         int                 theLine);

      /**
       * Construct a boolean literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theBoolean                  the value
       */
      ScriptNodeLiteral(
         const ScriptString* theFile,
         int                 theLine,
         bool                theBoolean);

      /**
       * Construct a number literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theNumber                   the value
       */
      ScriptNodeLiteral(
         const ScriptString* theFile,
         int                 theLine,
         Number              theNumber);

      /**
       * Construct a string literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theString                   the value
       */
      ScriptNodeLiteral(
         const ScriptString* theFile,
         int                 theLine,
         const std::string&       theString);

      /**
       * Construct an arbitrary literal.
       * @param theLine                     the line number (in the script) 
       *                                    that created this node.
       * @param theDatatype                 the datatype
       */
      ScriptNodeLiteral(
         const ScriptString*   theFile,
         int                   theLine,
         ScriptValue::DataType theDatatype);

      /**
       * Pushes the literal on the stack of the execution context.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theExecutionContext);

      /**
       * Gets the type of the node.
       * @return the type of the node. 
       */
      Type
      getType() const;

      /**
       * Get the value of the literal. 
       * @return the value of the literal. 
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
      ScriptNodeLiteral(
         const ScriptNodeLiteral& theOther);

      ScriptValue valueM;   
   };
}

#endif
