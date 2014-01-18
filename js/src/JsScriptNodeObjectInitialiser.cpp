#include "JsScriptNodeObjectInitialiser.h"
#include "JsScriptObjectArray.h"
#include "JsScriptReferenceError.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"
#include "JsScriptUserDefinedException.h"

using namespace Js;

ScriptNodeObjectInitialiser::ScriptNodeObjectInitialiser(
   const ScriptString*  theFile,
   int                  theLine,
   bool                 theIsLeftHandSideExpression)
:  ScriptNode(theFile, theLine, theIsLeftHandSideExpression)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeObjectInitialiser::~ScriptNodeObjectInitialiser()
{
   for (ObjectInitialiserMap::iterator iter = objectInitialiserMapM.begin();
        iter != objectInitialiserMapM.end();
        iter++)
   {
      delete (*iter).second;
   }
   objectInitialiserMapM.clear();
}

// ----------------------------------------------------------------------------

void
ScriptNodeObjectInitialiser::addElement(
   const std::string& thePropertyName,
   ScriptNode*   theAssignmentExpression)
{
   objectInitialiserMapM.insert(
      ObjectInitialiserMap::value_type(thePropertyName, 
                                      theAssignmentExpression));
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeObjectInitialiser::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   try
   { 
      ScriptObjectPtr object = new ScriptObject(ScriptObject::ObjectE);
      for (ObjectInitialiserMap::iterator iter = objectInitialiserMapM.begin();
           iter != objectInitialiserMapM.end();
           iter++)
      {
         ScriptValue result = (*iter).second->execute(theContext).getValue();
         object->putPropertyNoCheck((*iter).first, result);
      }
      return ScriptValue(object);
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
ScriptNodeObjectInitialiser::getType() const
{
   return ScriptNode::ObjectInitialiserE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeObjectInitialiser::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Object Initialiser\n";

   ObjectInitialiserMap::const_iterator next;
   for (ObjectInitialiserMap::const_iterator iter = objectInitialiserMapM.begin();
        iter != objectInitialiserMapM.end();
        iter = next)
   {
      (*iter).second->printFileAndLine(theOutputString, 
                                       theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      theOutputString += (*iter).first;
      theOutputString += ": ";
      next = iter;
      next++;
      if (next != objectInitialiserMapM.end())
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


