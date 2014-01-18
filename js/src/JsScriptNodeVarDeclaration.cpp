#include "JsScriptNodeVarDeclaration.h"
#include "JsScriptScanner.h"
#include "JsScriptTypeError.h"

using namespace Js;

ScriptNodeVarDeclaration::ScriptNodeVarDeclaration(
   const ScriptString*  theFile,
   int                  theLine,
   ScriptScanner::Token theOperator,
   std::string               theIdentifier,
   ScriptNode*          theInitialValue)
:  ScriptNode(theFile, theLine, false),
   identifierM(theIdentifier),
   initialValueM(theInitialValue),
   operatorM(theOperator)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeVarDeclaration::~ScriptNodeVarDeclaration()
{
   delete initialValueM;
   initialValueM = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeVarDeclaration::execute(
   ScriptExecutionContext* theContext)
{
   theContext->countNodeExecution(this);
   ScriptValue value;
   switch (operatorM)
   {
      case ScriptScanner::ForVarE:
      {
         if (initialValueM == 0)
         {
            value = ScriptValue::UndefinedC;
         }
         else
         {
            value = initialValueM->execute(theContext).getValue();
         }
         theContext->putPropertyNoCheck(identifierM, value);
         return theContext->getIdentifier(identifierM);
      }
      case ScriptScanner::VarE:
      {
         if (initialValueM == 0)
         {
            value = ScriptValue::UndefinedC;
         }
         else
         {
            value = initialValueM->execute(theContext).getValue();
         }
         theContext->putPropertyNoCheck(identifierM, value);
      }
      default:
      {
         // empty
      }
   }
   return value;
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeVarDeclaration::getType() const
{
   return ScriptNode::VarDeclarationE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeVarDeclaration::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   if (initialValueM == 0)
   {
      theOutputString += "var " + identifierM + "\n";
   }
   else
   {
      theOutputString += "var " + identifierM + " =\n";
      initialValueM->printFileAndLine(theOutputString, theMaxFilenameLength);
      theOutputString += theLinePrefix + " +-";
      initialValueM->print(theOutputString, 
                           theLinePrefix + "   ",
                           theMaxFilenameLength);
   }
}

