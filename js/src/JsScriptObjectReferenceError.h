#ifndef PALSCRIPTOBJECTREFERENCEERROR_H
#define PALSCRIPTOBJECTREFERENCEERROR_H

#include "JsScriptObjectError.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptValueArray.h"

namespace Js
{
  /** \class ScriptObjectReferenceError ScriptObjectReferenceError.h "ScriptObjectReferenceError.h"
   * \ingroup PALSCRIPT
   * ReferenceError object
   */
   class ScriptObjectReferenceError : public ScriptObjectError
   {
   public:

      ScriptObjectReferenceError(
         ScriptValue theMessage);

      virtual
      ~ScriptObjectReferenceError();

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
