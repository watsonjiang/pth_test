#ifndef PALSCRIPTNODEOBJECTINITIALISER_H
#define PALSCRIPTNODEOBJECTINITIALISER_H

#include "JsScriptNode.h"
#include "JsScriptScanner.h"

namespace Js
{
   class ScriptNodeObjectInitialiser : public ScriptNode
   {
   public:

      /**
       * Construct an array initialiser node.
       */
      ScriptNodeObjectInitialiser(
         const ScriptString*  theFile,
         int                  theLine,
         bool                 theIsLeftHandSideExpression);

      /**
       * Destructor.
       */
      virtual
      ~ScriptNodeObjectInitialiser();
      
      /**
       * Add element.
       * @param thePropertyName         the property name 
       *                                (first element has index 0)
       * @param theAssignmentExpression the assignment expression that 
       *                                calcuates the element value
       */
       void
       addElement(
          const std::string& thePropertyName,
          ScriptNode*   theAssignmentExpression);

      /**
       * Returns the result of the binary operation.
       */
      ScriptValue
      execute(
         ScriptExecutionContext* theContext);

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
      ScriptNodeObjectInitialiser(
         const ScriptNodeObjectInitialiser& theOther);
         
      typedef std::map<std::string, ScriptNode*> ObjectInitialiserMap;
      
      ObjectInitialiserMap objectInitialiserMapM;
   };
}

#endif
