#ifndef PALSCRIPTOBJECTERROR_H
#define PALSCRIPTOBJECTERROR_H

#include "JsScriptObject.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptValueArray.h"

namespace Js
{

  /** \class ScriptObjectError ScriptObjectError.h "ScriptObjectError.h"
   * \ingroup PALSCRIPT
   * Error object
   */
   class ScriptObjectError : public ScriptObject
   {
   public:

      ScriptObjectError(
         ScriptValue   theMessage,
         const std::string& theName = "Error");

      virtual
      ~ScriptObjectError();

      // ----------------------------------------------------------------------
      // THE SCRIPT ENGINE INTERFACE:
      // ----------------------------------------------------------------------

      /**
       * The name of this class.
       */
      static 
      const char* 
      ClassNameC;

      /**
       * Create an object.
       * Create a new instance of the scriptable class.
       * @param theContext   (in) The execution context from which the
       *                          constructor is called.
       * @param theArguments (in) The values of the constructor arguments.
       * @return A pointer to the new object.
       */
      static
      ScriptObject* 
      create(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments);

      /**
       * Get the name of this class.
       * @return The name of this class.
       */
      virtual
      const char*
      getClass() const;

      /**
       * Check if this object is derived from the class 'theClassName'.
       * @param theClassName (in) The name of the class.
       * @return 'true'  if this object is derived from the the specified class
       *         'false' otherwise.
       */
      bool
      isInstanceOf(
         const std::string& theClassName) const;
   };
}

#endif
