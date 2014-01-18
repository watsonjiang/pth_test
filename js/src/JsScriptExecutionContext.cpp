#include "JsScript.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptRuntime.h"
#include "JsScriptNode.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptReference.h"
#include "JsScriptObjectMath.h"

using namespace Js;

ScriptExecutionContext::ScriptExecutionContext(ScriptRuntime * rt)
:  maxNumberOfExecutedNodesM(UINT_MAX),
   numberOfExecutedNodesM(0),
   traceContextM(new ScriptTraceContext),
   runtimeM(rt)
{
   ScriptObject* globalObject = new ScriptObject(ScriptObject::ActivationE);
   globalObject->AddRef();
   scopeChainM.push_front(globalObject);
   //init build-in object Math
   ScriptObject* mathObj = new ScriptObjectMath();
   putProperty("Math", ScriptValue(mathObj)); 

}

// ----------------------------------------------------------------------------

ScriptExecutionContext::~ScriptExecutionContext()
{
   for (ObjectList::iterator iter = scopeChainM.begin();
        iter != scopeChainM.end();
        iter++)
   {
      delete *iter;
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptExecutionContext::asyncResp(
   const ScriptExecutionContextIdentifier* theIdentifier,
   const std::string&                           theFunctionName,
   const ScriptValueArray&                 theArguments,
   Script*                                 theScript)
{
   ScriptValue result;
   unsigned int currentLevel = contextIdentifierM->getNumberOfIdentifiers();
   if (theIdentifier->getNumberOfIdentifiers() > currentLevel)
   {
      unsigned int currentId = theIdentifier->getIdentifier(currentLevel);
      if (currentId < asyncRespReceiverM.size() && asyncRespReceiverM[currentId] != 0)
      {
            result = asyncRespReceiverM[currentId]->asyncResp(theIdentifier,
                                                              theFunctionName,
                                                              theArguments);
      }
      else
      {
         return false;
      }
   }
   traceFunctionCall(0, theFunctionName, theArguments);

   ScriptValue fctRef(getIdentifier(theFunctionName));
   if (fctRef.getBase() == 0)
   {
      // Function name does not exist
      return false;
   }
   ScriptValue fct(fctRef.getValue());
   if (fct.getDataType() != ScriptValue::ObjectE)
   {
      // Function name exists, but does not refer to an object
      return false;
   }
   ScriptObject* obj = fct.toObject();
   if (obj->getObjectType() != ScriptObject::FunctionE)
   {
      // Function name refers to a non-function object
      return false;
   }

   ScriptObjectActivation* activationObject = new ScriptObjectActivation;

   addObjectToScopeChain(activationObject);
   
   resetNumberOfExecutedNodes();

   Time startTime(Time::getUtcTime());

   result = obj->call(this, theArguments);

   theScript->countFunctionCall(
      theFunctionName, 
      getNumberOfExecutedNodes(), 
      startTime);

   removeObjectFromScopeChain();

   return result;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptExecutionContext::getIdentifier(
   const std::string& theName)
{
   for (ObjectList::iterator iter = scopeChainM.begin();
        iter != scopeChainM.end();
        iter++)
   {
      if ((*iter)->hasProperty(theName) != 0)
      {
         return ScriptValue(*iter, theName);
      }
   }
   return ScriptValue(0, theName);
}

// ----------------------------------------------------------------------------

void
ScriptExecutionContext::getPropertyNames(
   std::vector<std::string>& thePropertyNames) const
{
   scopeChainM.front()->getPropertyNames(thePropertyNames);
}

// ----------------------------------------------------------------------------

void
ScriptExecutionContext::internalTraceFunctionCall(
   const ScriptObject*     theObject,
   const std::string&           theFunctionName,
   const ScriptValueArray& theArguments)
{
   if (traceContextM->isEnabled() == true)
   {
      logFunctionCall(theObject, theFunctionName, theArguments);
      return;
   }
   std::string output;
   if (contextIdentifierM != 0)
   {
      output.append(contextIdentifierM->toString());
      output.append(" ");
   }
   if (theObject != 0)
   {
      output.append(theObject->getClass());
      output.append(".");
   }
   output.append(theFunctionName);
   output.append("(");
   if (traceContextM->getScriptTraceMode() == ScriptTraceContext::FunctionAndArgsE)
   {
      for (ScriptValueArray::const_iterator iter = theArguments.begin();
           iter != theArguments.end();
           iter++)
      {
         if (iter != theArguments.begin())
         {
            output.append(", ");
         }
         ScriptValue arg = (*iter).getValue();
         if (arg.getDataType() == ScriptValue::ObjectE)
         {
            output.append("{");
            output.append(arg.toString());
            output.append("}");
         }
         else
            if (arg.getDataType() == ScriptValue::StringE)
            {
               output.append("\"");
               output.append(arg.toString());
               output.append("\"");
            }
            else
            {
               output.append(arg.toString());
            }
      }
   }
   output.append(")");
   print(output);
}

// ----------------------------------------------------------------------------

void
ScriptExecutionContext::logFunctionCall(
   const ScriptObject*     theObject,
   const std::string&           theFunctionName,
   const ScriptValueArray& theArguments)
{
  /* 
   std::string heading;
   if (traceContextM->getLevelOfDetail() < 2)
   {
      heading.append(contextIdentifierM->toString());
      heading.append(" ");
   }

   if (theObject != 0)
   {
      heading.append(theObject->getClass());
      heading.append(".");
   }
   heading.append(theFunctionName);
   heading.append("()");
   
   TraceLogRecord* logRecord =
      new TraceLogRecord(
         "Script Engine",
         contextIdentifierM->toString(),
         heading,
         1);
   if (traceContextM->getScriptTraceMode() != ScriptTraceContext::FunctionAndArgsE ||
      theArguments.size() == 0)
   {
      traceContextM->writeLogRecord(logRecord);
      return;
   }

   TraceLogDefinitionList* list = new TraceLogDefinitionList;
   
   int index = 1;
   for (ScriptValueArray::const_iterator iter = theArguments.begin();
        iter != theArguments.end();
        iter++)
   {
      std::string argumentName("arg");
      argumentName.appendNumber(index++);

      ScriptValue arg = (*iter).getValue();
      std::string argumentValue;

      if (arg.getDataType() == ScriptValue::ObjectE)
      {
         argumentValue.append("{");
         argumentValue.append(arg.toString());
         argumentValue.append("}");
      }
      else
      if (arg.getDataType() == ScriptValue::StringE)
      {
         argumentValue.append("\"");
         argumentValue.append(arg.toString());
         argumentValue.append("\"");
      }
      else
      {
         argumentValue.append(arg.toString());
      }
      list->appendText(argumentName, argumentValue);
   }

   logRecord->appendObject(list);
   traceContextM->writeLogRecord(logRecord);
  */
  //TODO: implement logFunctionCall
}


// ----------------------------------------------------------------------------

void
ScriptExecutionContext::print(
   const std::string& theText)
{
  runtimeM->print(theText);
}

// ----------------------------------------------------------------------------

void
ScriptExecutionContext::putExternalProperties(
   const ScriptObject& theProperties)
{
   std::string propertyName;
   if (theProperties.getNameOfFirstProperty(propertyName) == false)
   {
      // object has no properties
      return;
   }
   do
   {
      ScriptValue propertyValue(theProperties.getProperty(propertyName));

      scopeChainM.front()->putProperty(propertyName, 
                                       propertyValue,
                                       ScriptProperty::DontDeleteE |
                                       ScriptProperty::DontEnumE   |
                                       ScriptProperty::ReadOnlyE   |
                                       ScriptProperty::ExternalE);
   }
   while (theProperties.getNameOfNextProperty(propertyName) == true);
}

// ----------------------------------------------------------------------------

void
ScriptExecutionContext::traceInstructionCall(
   const ScriptNode*  theInstruction)
{
/*
   if (traceContextM->isEnabled() == true)
   {
      std::string heading;
      if (traceContextM->getLevelOfDetail() < 2)
      {
         heading.append(contextIdentifierM->toString());
         heading.append(" ");
      }
      heading.append(*(theInstruction->getFile()));
      heading.append(":");
      heading.appendNumber(theInstruction->getLine());
      
      TraceLogRecord* logRecord =
         new TraceLogRecord(
            "Script Engine",
            contextIdentifierM->toString(),
            heading,
            1);
      traceContextM->writeLogRecord(logRecord);
   }
   else
   {
      std::string output;
      if (contextIdentifierM != 0)
      {
         output.append(contextIdentifierM->toString());
         output.append(' ');
      }
      output.append(*(theInstruction->getFile()));
      output.append(':');
      output.appendNumber(theInstruction->getLine());
      print(output);
   }
*/
 //TODO: implement this funciton.
}

