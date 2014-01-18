#ifndef PALSCRIPTEXECUTIONCONTEXT_H
#define PALSCRIPTEXECUTIONCONTEXT_H

#include "JsScriptAsyncRespInterface.h"
#include "JsScriptDefinitions.h"
#include "JsScriptExecutionContextIdentifier.h"
#include "JsScriptExecutionTimeExceeded.h"
#include "JsScriptObject.h"
#include "JsScriptTraceContext.h"
#include <vector>
#include <list>
#include <limits.h>
namespace Js
{
   class Script;
   class ScriptNode;
   class ScriptRuntime;

   class ScriptExecutionContext
   {
   public:

      /**
       * Default constructor.
       */
      ScriptExecutionContext(ScriptRuntime* rt);

      /**
       * Destructor.
       */
      virtual
      ~ScriptExecutionContext();

      void
      addObjectToScopeChain(
         ScriptObject* theObject);

      ScriptValue
      asyncResp(
         const ScriptExecutionContextIdentifier* theIdentifier,
         const std::string&                           theFunctionName,
         const ScriptValueArray&                 theArguments,
         Script*                                 theScript);

      /**
       * Count the execution of one node of the script.
       * The nodes of the script engine call this function before they
       * execute a node.
       * @param theScriptNode the node that will be executed
       */
      void
      countNodeExecution(
         ScriptNode* theScriptNode);

      /**
       * Remove the specified property from the execution context.
       * @param thePropertyName The name of the property.
       * @return 'true'   if the removal was successful,
       *         'false', otherwise.
       */
      bool
      deleteProperty(
         const std::string& thePropertyName);

      /**
       * Disable the subscriber specific trace log.
       */
      void
      disableLog();

      /**
       * Try to enable the subscriber specific trace log.
       * @param theIdentifier the identifier (e.g. MSISDN,
       *                      PPP-username, ...)
       * @return 'true' if the trace log was enabled. 
       */
      bool
      enableLog(
         const std::string& theIdentifier);

      /**
       * Get a pointer to the asynchronous response interface.
       * @return a pointer to the asynchronous response interface.
       */
      ScriptAsyncRespInterface*
      getAsyncRespInterface() const;

      /**
       * Get a pointer to the context identifier.
       * @return a pointer to the context identifier.
       */
      const ScriptExecutionContextIdentifier*
      getContextIdentifier() const;

      /**
       * Return the value of the specified identifier.
       * @param theName the name of the identifier.
       * @return The value of the identifier.
       */
      ScriptValue
      getIdentifier(
         const std::string& theName);

      /**
       * Get the identifier of the subscriber specific trace log.
       * @return the identifier of the subscriber specific trace log.
       */
      const std::string&
      getLogIdentifier() const;

      /**
       * Get level of detail of the subscriber specific trace log.
       * @return the level of detail.
       */
      int
      getLogLevelOfDetail() const;

      /**
       * Return the number of script nodes that have been executed by
       * the script engine.
       * @see resetNumberOfExecutedNodes()
       * @return the number of executed script nodes
       */
      unsigned int
      getNumberOfExecutedNodes() const;

      /**
       * Get the names of all properties.
       * @param a vector with the names of all properties
       */
      void
      getPropertyNames(
         std::vector<std::string>& thePropertyNames) const;

      ScriptTraceContext*
      getTraceContext();

      /**
       * Get the current trace mode.
       * @return the current trace mode
       */
      ScriptTraceContext::ScriptTraceMode
      getTraceMode() const;

      virtual
      void
      internalTraceFunctionCall(
         const ScriptObject*     theObject,
         const std::string&           theFunctionName,
         const ScriptValueArray& theArguments);

      bool
      logIsEnabled() const;

      bool
      logIsEnabled(
         int theLevelOfDetail) const;

      virtual
      void
      print(
         const std::string& theText);

      void
      putExternalProperties(
         const ScriptObject& theProperties);

      void
      putExternalProperty(
         const std::string&      thePropertyName,
         const ScriptValue& theValue);

      void
      putExternalPropertyNoCheck(
         const std::string&      thePropertyName,
         const ScriptValue& theValue);

      /**
       * Put the specified property.
       */
      bool
      putProperty(
         const std::string&      thePropertyName,
         const ScriptValue& theValue);

      void
      putPropertyNoCheck(
         const std::string&      thePropertyName,
         const ScriptValue& theValue);

      int
      registerAsyncRespReceiver(
         ScriptAsyncRespInterface* theAsyncRespReceiver);

      void
      removeObjectFromScopeChain();

      /**
       * Reset counter, which counts the number of executed script
       * nodes.
       */
      void
      resetNumberOfExecutedNodes();

      /**
       * Store a pointer to the asynchronous response interface.
       * @param theAsynchronousResponseInterface a pointer to the
       *                                         asynchronous response
       *                                         interface.
       */ 
      void
      setAsyncRespInterface(
         ScriptAsyncRespInterface* theAsyncRespInterface);

      /**
       * Store a pointer to the context identifier.
       * @param theContextIdentifier a pointer to the context identifier.
       */ 
      void
      setContextIdentifier(
         const ScriptExecutionContextIdentifier* theContextIdentifier);

      /**
       * Set the maximum number of script nodes that the script engine
       * is allowed to execute for each external call.
       * @param theMaxNumberOfExecutedNodes the maximum number of
       *                                    executed nodes
       */
      void
      setMaxNumberOfExecutedNodes(
         unsigned int theMaxNumberOfExecutedNodes = UINT_MAX);

      void
      traceFunctionCall(
         const ScriptObject*     theObject,
         const std::string&           theFunctionName,
         const ScriptValueArray& theArguments);

      virtual
      void
      traceInstructionCall(
         const ScriptNode*  theInstruction);

      void
      unregisterAsyncRespReceiver(
         int theIndex);

      void
      writeLogRecord(
         TraceLogRecord* theLogRecord);

      ScriptRuntime*
      getRuntime();

   private:

      typedef std::list<ScriptObject*> ObjectList;
      typedef std::vector<ScriptAsyncRespInterface*> AsyncRespReceiverArray;

      void
      logFunctionCall(
         const ScriptObject*     theObject,
         const std::string&           theFunctionName,
         const ScriptValueArray& theArguments);

      ScriptAsyncRespInterface*                      asyncRespInterfaceM;
      AsyncRespReceiverArray                         asyncRespReceiverM;
      SmartPointer<ScriptExecutionContextIdentifier> contextIdentifierM;
      unsigned int                                   maxNumberOfExecutedNodesM;
      unsigned int                                   numberOfExecutedNodesM;
      ObjectList                                     scopeChainM;
      ScriptTraceContextPtr                          traceContextM;
      ScriptRuntime*                                 runtimeM;
   };

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::addObjectToScopeChain(
      ScriptObject* theObject)
   {
      theObject->AddRef();
      scopeChainM.push_front(theObject);
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::countNodeExecution(
      ScriptNode* theScriptNode)
   {
      if (traceContextM->getScriptTraceMode() == ScriptTraceContext::InstructionE)
      {
         traceInstructionCall(theScriptNode);
      }
      if (++numberOfExecutedNodesM > maxNumberOfExecutedNodesM)
      {
         throw ScriptExecutionTimeExceeded(theScriptNode);
      }
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptExecutionContext::deleteProperty(
      const std::string& thePropertyName)
   {
      return scopeChainM.front()->deletePropertyNoCheck(thePropertyName);
   }


   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::disableLog()
   {
      traceContextM->disableTraceLog();
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptExecutionContext::enableLog(
      const std::string& theIdentifier)
   {
      return traceContextM->enableTraceLog(theIdentifier);
   }

   // -------------------------------------------------------------------------

   inline
   ScriptAsyncRespInterface*
   ScriptExecutionContext::getAsyncRespInterface() const
   {
      return asyncRespInterfaceM;
   }

   // -------------------------------------------------------------------------

   inline
   const ScriptExecutionContextIdentifier*
   ScriptExecutionContext::getContextIdentifier() const
   {
      return contextIdentifierM;
   }

   // -------------------------------------------------------------------------

   inline
   const std::string&
   ScriptExecutionContext::getLogIdentifier() const
   {
      return traceContextM->getIdentifier();
   }

   // -------------------------------------------------------------------------

   inline
   int
   ScriptExecutionContext::getLogLevelOfDetail() const
   {
      return traceContextM->getLevelOfDetail();
   }

   // -------------------------------------------------------------------------

   inline
   unsigned int
   ScriptExecutionContext::getNumberOfExecutedNodes() const
   {
      return numberOfExecutedNodesM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptTraceContext*
   ScriptExecutionContext::getTraceContext()
   {
      return traceContextM;
   }

   // -------------------------------------------------------------------------

   inline
   ScriptTraceContext::ScriptTraceMode
   ScriptExecutionContext::getTraceMode() const
   {
      return traceContextM->getScriptTraceMode();
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptExecutionContext::logIsEnabled() const
   {
      return traceContextM->isEnabled();
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptExecutionContext::logIsEnabled(
      int theLevelOfDetail) const
   {
      return traceContextM->isEnabled(theLevelOfDetail);
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::putExternalProperty(
      const std::string&      thePropertyName,
      const ScriptValue& theValue)
   {
      scopeChainM.front()->putProperty(thePropertyName, 
                                       theValue,
                                       ScriptProperty::DontDeleteE |
                                       ScriptProperty::DontEnumE   |
                                       ScriptProperty::ReadOnlyE   |
                                       ScriptProperty::ExternalE);
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::putExternalPropertyNoCheck(
      const std::string&      thePropertyName,
      const ScriptValue& theValue)
   {
      scopeChainM.front()->putPropertyNoCheck(thePropertyName, 
                                              theValue,
                                              ScriptProperty::DontDeleteE |
                                              ScriptProperty::DontEnumE   |
                                              ScriptProperty::ReadOnlyE   |
                                              ScriptProperty::ExternalE);
   }

   // -------------------------------------------------------------------------

   inline
   bool
   ScriptExecutionContext::putProperty(
      const std::string&      thePropertyName,
      const ScriptValue& theValue)
   {
      return scopeChainM.front()->putProperty(thePropertyName, theValue) != 0;
   }
   
   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::putPropertyNoCheck(
      const std::string&      thePropertyName,
      const ScriptValue& theValue)
   {
      scopeChainM.front()->putPropertyNoCheck(thePropertyName, theValue);
   }
   
   // -------------------------------------------------------------------------

   inline
   int
   ScriptExecutionContext::registerAsyncRespReceiver(
      ScriptAsyncRespInterface* theAsyncRespReceiver)
   {
      for (int i = 0; i < asyncRespReceiverM.size(); i++)
      {
         if (asyncRespReceiverM[i] == 0)
         {
            asyncRespReceiverM[i] = theAsyncRespReceiver;
            return i;
         }
      }
      asyncRespReceiverM.push_back(theAsyncRespReceiver);
      return asyncRespReceiverM.size() - 1;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::removeObjectFromScopeChain()
   {
      scopeChainM.front()->Release();
      scopeChainM.pop_front();
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::resetNumberOfExecutedNodes()
   {
      numberOfExecutedNodesM = 0;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::setAsyncRespInterface(
      ScriptAsyncRespInterface* theAsyncRespInterface)
   {
      asyncRespInterfaceM = theAsyncRespInterface;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::setContextIdentifier(
      const ScriptExecutionContextIdentifier* theContextIdentifier)
   {
      contextIdentifierM = theContextIdentifier;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::setMaxNumberOfExecutedNodes(
      unsigned int theMaxNumberOfExecutedNodes)
   {
      maxNumberOfExecutedNodesM = theMaxNumberOfExecutedNodes;
      numberOfExecutedNodesM = 0;
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::traceFunctionCall(
      const ScriptObject*     theObject,
      const std::string&           theFunctionName,
      const ScriptValueArray& theArguments)
   {
      if (traceContextM->getScriptTraceMode() != ScriptTraceContext::NoneE)
      {
         internalTraceFunctionCall(theObject, theFunctionName, theArguments);
      }
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::unregisterAsyncRespReceiver(
      int theIndex)
   {
      assert(theIndex >= 0 && theIndex < asyncRespReceiverM.size());
      asyncRespReceiverM[theIndex] = 0; 
   }

   // -------------------------------------------------------------------------

   inline
   void
   ScriptExecutionContext::writeLogRecord(
      TraceLogRecord* theLogRecord)
   {
      traceContextM->writeLogRecord(theLogRecord);
   }

   inline
   ScriptRuntime*
   ScriptExecutionContext::getRuntime()
   {
      return runtimeM; 
   }
}

#endif
