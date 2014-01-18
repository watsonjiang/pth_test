#ifndef PALSCRIPTOBJECTECHOSERVER_H
#define PALSCRIPTOBJECTECHOSERVER_H

#include "PalScriptObjectExternal.h"
#include "PalSmartPointer.h"

namespace Pal
{

  /** \class ScriptObjectEchoServer ScriptObjectEchoServer.h "ScriptObjectEchoServer.h"
   * \ingroup JS
   * a simple echo server. 
   */
   class ScriptObjectEchoServer : public ScriptObjectExternal
   {
   public:

      /**
       * Default constructor
       */
      ScriptObjectEchoServer();

      /**
       * Destructor.
       */
      virtual
      ~ScriptObjectEchoServer();

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
       * Create a new onstance of the scriptable class.
       * @param theContext   (in) The execution context from which the
       *                          constructor is called.
       * @param theArguments (in) The values of the constructor arguments.
       * @return A pointer to the new object.
       */
      static
      ScriptObject*
      create(ScriptExecutionContext* theContext,
             const ScriptValueArray& theArguments);

      /**
       * Call a function.
       * Call the function 'theName'.
       * @param theName          (in)  The name of the function.
       * @param theContext       (in)  The execution context from which this
       *                               function is called.
       * @param theArguments     (in)  The values of the function arguments.
       * @param theIsConstructor (in)  'true' if the function is used
       *                               as a constructor, 'false' otherwise.
       * @param theResult        (out) The function's return value.
       * @return 'true'  if the function theName exists and can be called, 
       *         'false' otherwise.
       */
      bool
      callExternalMethod(
         const std::string&           theName,
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                         theIsConstructor,
         ScriptValue&            theResult);

      /**
       * Delete a property.
       * Delete property 'theName'.
       * @param theName (in) The name of the property.
       * @return ScriptObject::UpdatedE: 
       *            The value of the property has been deleted.
       *         ScriptObject::NotUpdatedE:
       *            The property does not exist in this object, but it
       *            might exist in a derived object.
       *         ScriptObject::UpdateNotAllowedE: 
       *            It is not allowed to delete this property.
       */
      ScriptObject::UpdateResult
      deleteExternalProperty(
         const std::string& theName);

      /**
       * Get the name of this class.
       * @return The name of this class.
       */
      const char*
      getClass() const;

      /**
       * Get the value of a property.
       * If property 'theName' exists then store its value in 'theValue'. 
       * @param theName  (in)  The name of the property.
       * @param theValue (out) The current value of the property.
       * @return 'true' if the property exists and can be read,
       *         'false' otherwise.
       */
      bool
      getExternalProperty(
         const std::string& theName,
         ScriptValue&  theValue) const;

      /**
       * Get name of first property.
       * If the object has properties then store the name of the first
       * property in 'theName'.
       * @param theName (out) The name of the first property.
       * @return 'true'  if the object has properties,
       *         'false' otherwise.
       */
      bool
      getNameOfFirstExternalProperty(
         std::string& theName) const;

      /**
       * Get name of next property.
       * Get the name of the property that comes after the propery
       * 'theName' and store it in 'theName'.
       * @param theName (in)  The name of the current property.
       *                (out) The name of the next property.
       * @return 'false' if the current property is the last property,
       *         'true'  otherwise.
       */
      bool
      getNameOfNextExternalProperty(
         std::string& theName) const;

      ScriptValue
      getStringValue() const;

      /**
       * Check if an object has a property.
       * Check if the object has the property 'theName'.
       * @param theName (in) The name of the property.
       * @return 'true' if the property exists, 'false' otherwise.
       */
      bool
      hasExternalProperty(
         const std::string& theName) const;

      /**
       * Check if this object is derived from the class 'theClassName'.
       * @param theClassName (in) The name of the class.
       * @return 'true'  if this object is derived from the the specified class
       *         'false' otherwise.
       */
      bool
      isInstanceOf(
         const std::string& theClassName) const;

      /**
       * Set the value of a property.
       * If property 'theName' exists, set its value to 'theValue'. If
       * the property does not exist, create it and set it's value to 'theValue'.
       * @param theName  (in) The name of the property.
       * @param theValue (in) The new value for the property.
       * @return ScriptObject::UpdatedE: 
       *            The value of the property has been set.
       *         ScriptObject::NotUpdatedE:
       *            The property does not exist in this object, but it
       *            might exist in a derived object.
       *         ScriptObject::UpdateNotAllowedE: 
       *            It is not allowed to set the value of this property or to
       *            create a property with this name.
       */
      ScriptObject::UpdateResult
      setExternalProperty(
         const std::string& theName,
         ScriptValue   theValue);

      // ----------------------------------------------------------------------
      // THE SCRIPTABLE FUNCTIONS AND PROPERTIES:
      // ----------------------------------------------------------------------

      // <<TODO>> Declare the methods, which implement the scriptable 
      //          functions of the class here.
      //          Sort the function names alphabetically.
      //
      // Template:
      //    ScriptValue
      //    scriptCall[FunctionName](
      //       ScriptExecutionContext* theContext, 
      //       const ScriptValueArray& theArguments,
      //       bool                         theIsConstructor);

      ScriptValue
      scriptCallListen(
         ScriptExecutionContext* theContext, 
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor);
      
      // <<TODO>> Declare the methods, which are used to delete  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      // Template:
      //    void
      //    scriptDelete[PropertyName]();

      // <<TODO>> Declare the methods, which are used to get the values of  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      // Template:
      //    ScriptValue
      //    scriptGet[PropertyName]() const;
      ScriptValue
         scriptGetHandlerScript() const;

      // <<TODO>> Declare the methods, which are used to set the values of  
      //          properties here.
      //          Sort the property names alphabetically.
      //
      //    void
      //    scriptSet[PropertyName](
      //       ScriptValue theValue);
      void
      scriptSetHandlerScript(
         ScriptValue theValue);

      // ----------------------------------------------------------------------
      // C++ INTERFACE:
      // ----------------------------------------------------------------------
   
   private:
      std::string handlerScript;      
   };

   // A smart-pointer for objects of this class
   typedef SmartPointer<ScriptObjectEchoServer> ScriptObjectEchoServerPtr;

}

#endif
