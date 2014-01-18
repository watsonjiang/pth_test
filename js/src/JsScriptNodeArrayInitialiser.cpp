#include "JsScriptNodeArrayInitialiser.h"
#include "JsScriptObjectArray.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeArrayInitialiser::ScriptNodeArrayInitialiser(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeArrayInitialiser::~ScriptNodeArrayInitialiser()
{
   for (ArrayInitialiserMap::iterator iter = arrayInitialiserMapM.begin();
        iter != arrayInitialiserMapM.end();
        iter++)
   {
      delete (*iter).second;
   }
   arrayInitialiserMapM.clear();
}

// ----------------------------------------------------------------------------

void
ScriptNodeArrayInitialiser::addElement(
   int         theIndex,
   ScriptNode* theAssignmentExpression)
{
   arrayInitialiserMapM.insert(
      ArrayInitialiserMap::value_type(theIndex, 
                                      theAssignmentExpression));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeArrayInitialiser::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   { 
      ScriptObjectArrayPtr array = new ScriptObjectArray;
      for (ArrayInitialiserMap::iterator iter = arrayInitialiserMapM.begin();
           iter != arrayInitialiserMapM.end();
           iter++)
      {
         ScriptValue result = (*iter).second->execute(theContext).getValue();
         array->putPropertyNoCheck((*iter).first, result);
      }
      return ScriptValue(array);
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
   ScriptValue result;
   return result;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeArrayInitialiser::getType() const
{
   return ScriptNode::ArrayInitialiserE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeArrayInitialiser::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Array Initialiser\n";

   ArrayInitialiserMap::const_iterator next;
   for (ArrayInitialiserMap::const_iterator iter = arrayInitialiserMapM.begin();
        iter != arrayInitialiserMapM.end();
        iter = next)
   {
      (*iter).second->printFileAndLine(theOutputString, 
                                       theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      char buf[10] = {0};
      snprintf(buf, 10, "%d", (*iter).first);
      theOutputString.append(buf);
      theOutputString += ": ";
      next = iter;
      next++;
      if (next != arrayInitialiserMapM.end())
      {
         (*iter).second->print(theOutputString, 
                               theLinePrefix + " | ",
                               theMaxFilenameLength);
      }
      else
      {
         (*iter).second->print(theOutputString, 
                               theLinePrefix + "   ",
                               theMaxFilenameLength);
         break;
      }
   }
}


