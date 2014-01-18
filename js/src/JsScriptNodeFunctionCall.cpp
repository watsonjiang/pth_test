#include "JsScriptNodeFunctionCall.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptObjectArray.h"
#include "JsScriptReferenceError.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

static const std::string PrototypeC("prototype");
static const std::string ThisC("this");

ScriptNodeFunctionCall::ScriptNodeFunctionCall(
   const ScriptString* theFile,
   int                 theLine,
   bool                theIsLeftHandSideExpression,
   ScriptNode*         theFunction)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   functionM(theFunction),
   isConstructorM(false)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeFunctionCall::~ScriptNodeFunctionCall()
{
   for (ScriptNodeArray::iterator iter = argumentListM.begin();
        iter != argumentListM.end();
        iter++)
   {
      delete *iter;
   }
   delete functionM;
   functionM = 0;
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunctionCall::addArgument(
   ScriptNode* theArgument)
{
   argumentListM.push_back(theArgument);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeFunctionCall::execute(
   ScriptExecutionContext* theContext)
{
   try
   {
      theContext->countNodeExecution(this);
      ScriptValue fctRef = functionM->execute(theContext);
      ScriptObject* object = fctRef.getBase();

      // Create argument array
      ScriptValueArray arguments;
      arguments.reserve(argumentListM.size());

      // Evaluate function call arguments
      for (ScriptNodeArray::iterator iter = argumentListM.begin();
           iter != argumentListM.end();
           iter++)
      {
         ScriptValue result((*iter)->execute(theContext));
         if (result.getDataType() == ScriptValue::ReferenceE)
         {
            result = result.getReferenceValue();
         }
         arguments.push_back(result);
      }

      const std::string fctName = fctRef.getPropertyName();

      theContext->traceFunctionCall(object, fctName, arguments);
      
      if (object != 0)
      {
         return object->call(fctName, 
                             theContext, 
                             arguments, 
                             isConstructorM);
      }
      else
      {
         ScriptValue fct = fctRef.getValue();
         if (fct.getDataType() != ScriptValue::ObjectE)
         {
            std::string errorMessage("Identifier '");
            errorMessage += fctName;
            errorMessage += "' does not refer to a function."; 
            throw ScriptTypeError(errorMessage, getFile(), getLine());
         }
         object = fct.toObject();
         if (object->getObjectType() != ScriptObject::FunctionE)
         {
            std::string errorMessage("Identifier '");
            errorMessage += fctName;
            errorMessage += "' does not refer to a function."; 
            throw ScriptTypeError(errorMessage, getFile(), getLine());
         }
         
         ScriptObjectActivation* activationObject = new ScriptObjectActivation;
         
         if (isConstructorM == false)
         {
            // Function is not used as a constructor. 
            theContext->addObjectToScopeChain(activationObject);
            
            ScriptValue result(object->call(theContext, arguments));
            
            theContext->removeObjectFromScopeChain();
            
            return result;      
         }
         else
         {
            // Function is used as a constructor.
            // Create a new object
            ScriptObject* newObject = new ScriptObject(ScriptObject::ObjectE);
            ScriptValue result(newObject);
            newObject->putProperty(
               PrototypeC,
               ScriptValue(object),
               ScriptProperty::DontEnumE | 
               ScriptProperty::DontDeleteE | 
               ScriptProperty::ReadOnlyE);
            
            activationObject->putProperty(ThisC, newObject);
            
            theContext->addObjectToScopeChain(activationObject);
            
            object->call(theContext, arguments);
            
            theContext->removeObjectFromScopeChain();
            
            return result;      
         }
      }
   }
   catch (ScriptReferenceError& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }
   catch (ScriptTypeError& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }
   catch (ScriptUserDefinedException& e)
   {
      if (e.getLine() == 0)
      {
         e.setLineAndFile(getLine(), getFile());
      }
      throw e;
   }
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeFunctionCall::getType() const
{
   return ScriptNode::FunctionCallE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeFunctionCall::getValue() const
{
   return ScriptValue::UndefinedC;
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunctionCall::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   if (isConstructorM == true)
   {
      theOutputString += "new ";
   }
   if (argumentListM.size() == 0)
   {
      theOutputString += "op1 ( )\n";
   }
   else
   {
      theOutputString += "op1 ( op2, ... , opn )\n";
   }
   if (functionM != 0)
   {
      functionM->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      functionM->print(theOutputString, 
                       theLinePrefix + " | ",
                       theMaxFilenameLength);
   }
   for (ScriptNodeArray::const_iterator iter = argumentListM.begin();
        iter != argumentListM.end();
        iter++)
   {
      (*iter)->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      (*iter)->print(theOutputString, 
                     theLinePrefix + " | ",
                     theMaxFilenameLength);
   }
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunctionCall::setIsConstructorFlag()
{
   isConstructorM = true;
}

