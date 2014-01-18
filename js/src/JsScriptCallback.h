#ifndef PALSCRIPTCALLBACK_H
#define PALSCRIPTCALLBACK_H

#include "JsScriptAsyncRespInterface.h"
#include "JsScriptDefinitions.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptExecutionTimeExceeded.h"
#include <string>
#include <vector>

namespace Js
{
   class ScriptCallback
   {
   public:

      /**
       * Default constructor.
       */
      ScriptCallback();

      /**
       * Copy constructor.
       */
      ScriptCallback(
         const ScriptCallback& theOther);

      /** 
       * Initialise the callback function(s).
       * @param theContext           The caller's execution context.
       * @param theArguments         The arguments that the caller
       *                             passed to the function.
       *                             The first argument(s) must be the
       *                             name(s) of the callback function(s).
       * @param theNumberOfCallbacks The number of callback function
       *                             names that the caller passed to
       *                             the function.
       */
      ScriptCallback(
         const ScriptExecutionContext* theContext,
         const ScriptValueArray&       theArguments,
         size_t                        theNumberOfCallbacks = 1);

      /** 
       * Constructor for the C++-interface.
       * Initialise the callback function.
       * @param theAsyncRespInterface
       *           The caller's asyncResp interface.
       * @param theContextIdentifier
       *           The caller's context identifier.
       * @param theCallbackName
       *           The function name of the callback function.
       */
      ScriptCallback(
         Js::ScriptAsyncRespInterface*               theAsyncRespInterface,
         const Js::ScriptExecutionContextIdentifier* theContextIdentifier,
         const std::string&                           theCallbackName);
        
      /**
       * Destructor
       */
      ~ScriptCallback();

      /**
       * Call the callback function.
       * @param theCallbackFunctionIndex The index of the callback
       *                                 function, which should be called.
       *                                 0 indicates that the first
       *                                 callback function should be
       *                                 called, 1 indicated that the
       *                                 second callback function
       *                                 should be called, ... .       
       * @param theArguments             The arguments, which should
       *                                 be used when calling the
       *                                 callback function.
       * @return 'true'  if the callback function could be called,
       *         'false' otherwise.
       */
      bool
      call(
         size_t                  theCallbackFunctionIndex,
         const ScriptValueArray& theArguments) const;

      size_t
      getNumberOfCallbacks() const;

   private:

      ScriptAsyncRespInterface*                      asyncRespInterfaceM;
      std::vector<std::string>                                 callbackFunctionNameM;
      SmartPointer<ScriptExecutionContextIdentifier> contextIdentifierM;      
   };

   // -------------------------------------------------------------------------

   inline
   ScriptCallback::ScriptCallback()
   :  asyncRespInterfaceM(0)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptCallback::ScriptCallback(
      const ScriptCallback& theOther)
   :  asyncRespInterfaceM(theOther.asyncRespInterfaceM),
      contextIdentifierM(theOther.contextIdentifierM),
      callbackFunctionNameM(theOther.callbackFunctionNameM)
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   ScriptCallback::ScriptCallback(
      const ScriptExecutionContext* theContext,
      const ScriptValueArray&       theArguments,
      size_t                        theNumberOfCallbacks)
   {
      assert(theNumberOfCallbacks >0);
      assert(theNumberOfCallbacks <= theArguments.size());
      asyncRespInterfaceM = theContext->getAsyncRespInterface();
      contextIdentifierM = theContext->getContextIdentifier();
      for (size_t i = 0; i < theNumberOfCallbacks; i++)
      {
         assert(theArguments[i].getDataType() == ScriptValue::StringE);
         callbackFunctionNameM.push_back(theArguments[i].getValue().toString());
      }
   }

   // -------------------------------------------------------------------------

   inline
   ScriptCallback::ScriptCallback(
      Js::ScriptAsyncRespInterface*               theAsyncRespInterface,
      const Js::ScriptExecutionContextIdentifier* theContextIdentifier,
      const std::string&                           theCallbackName)
   {
      asyncRespInterfaceM = theAsyncRespInterface;
      contextIdentifierM  = theContextIdentifier;
      callbackFunctionNameM.push_back(theCallbackName);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptCallback::~ScriptCallback()
   {
      // Empty
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptCallback::call(
      size_t                  theCallbackFunctionIndex,
      const ScriptValueArray& theArguments) const
   {
      if (theCallbackFunctionIndex >= callbackFunctionNameM.size() ||
          asyncRespInterfaceM == 0)
      {
         return false;
      }

      asyncRespInterfaceM->asyncResp(
         contextIdentifierM,
         callbackFunctionNameM[theCallbackFunctionIndex],
         theArguments);

      return true;
   }

   // -------------------------------------------------------------------------

   inline
   size_t
   ScriptCallback::getNumberOfCallbacks() const
   {
      return callbackFunctionNameM.size();
   }

}

#endif
