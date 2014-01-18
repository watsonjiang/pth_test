#include "JsScriptNodeFunction.h"
#include "JsScriptObjectArray.h"
#include "JsScriptObjectFunction.h"

using namespace Js;

static const std::string ArgumentsC("arguments");

ScriptNodeFunction::ScriptNodeFunction(
   const ScriptString* theFile,
   int                 theLine,
   bool                theIsLeftHandSideExpression,
   const std::string&       theName,
   bool                theNameIsVisible)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression),
   nameM(theName),
   nameIsVisibleM(theNameIsVisible)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeFunction::~ScriptNodeFunction()
{
   for (StatementList::iterator iter = statementListM.begin();
        iter != statementListM.end();
        iter++)
   {
      delete *iter;
   }
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunction::addParameter(
   const std::string& theParameterName)
{
   parameterListM.push_back(theParameterName);
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunction::addStatement(
   ScriptNode* theStatement)
{
   if (theStatement != 0)
   {
      statementListM.push_back(theStatement);
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeFunction::call(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   int argumentIndex = 0;
   for (ParameterList::const_iterator iter1 = parameterListM.begin();
        iter1 != parameterListM.end();
        iter1++)
   {
      if (argumentIndex < theArguments.size())
      {
         theContext->putPropertyNoCheck(*iter1, theArguments[argumentIndex]);
         argumentIndex++;
      }
      else
      {
         theContext->putPropertyNoCheck(*iter1, ScriptValue::UndefinedC);
      }
   }

   // Store all arguments in the "arguments" array
   ScriptObjectArray* argumentArray = new ScriptObjectArray;
   for (int i = 0; i < theArguments.size(); i++)
   {
      argumentArray->putPropertyNoCheck(i, theArguments[i]);
   }
   
   theContext->putPropertyNoCheck(ArgumentsC, argumentArray);

   if (nameIsVisibleM == false && nameM.empty() == false)
   {
      ScriptValue functionObject(new ScriptObjectFunction(this));
      theContext->putPropertyNoCheck(nameM, functionObject);
   }

   // Execute all statements of the function
   ScriptValue result;
   for (StatementList::iterator iter2 = statementListM.begin();
        iter2 != statementListM.end();
        iter2++)
   {
      result = (*iter2)->execute(theContext);
      if (result.getCommand() == ScriptValue::ReturnE)
      {
         result.resetCommand();
         return result;
      }
   }
   return ScriptValue::UndefinedC;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeFunction::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->countNodeExecution(this);
   ScriptValue functionObject(new ScriptObjectFunction(this));
   if (nameIsVisibleM == true)
   {
      theExecutionContext->putPropertyNoCheck(nameM, functionObject);
   }
   return functionObject;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeFunction::getType() const
{
   return ScriptNode::FunctionE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeFunction::getValue() const
{
   return ScriptValue::UndefinedC;
}

// ----------------------------------------------------------------------------

void
ScriptNodeFunction::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Function ";
   theOutputString += nameM + "(";
   ParameterList::const_iterator next1;
   for (ParameterList::const_iterator iter1 = parameterListM.begin();
        iter1 != parameterListM.end();
        iter1 = next1)
   {
      theOutputString += *iter1;
      next1 = iter1;
      next1++;
      if (next1 != parameterListM.end())
      {
         theOutputString += ", ";
      }
   }

   theOutputString += ")\n";
   StatementList::const_iterator next2;
   for (StatementList::const_iterator iter2 = statementListM.begin();
        iter2 != statementListM.end();
        iter2 = next2)
   {
      (*iter2)->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      next2 = iter2;
      next2++;
      if (next2 != statementListM.end())
      {
         (*iter2)->print(theOutputString, 
                         theLinePrefix + " | ", 
                         theMaxFilenameLength);
      }
      else
      {
         (*iter2)->print(theOutputString, 
                         theLinePrefix + "   ", 
                         theMaxFilenameLength);
         break;
      }
   }   
}
