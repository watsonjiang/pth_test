#include "JsScriptNodeIdentifier.h"

using namespace Js;

// ----------------------------------------------------------------------------

ScriptNodeIdentifier::ScriptNodeIdentifier(
   const ScriptString* theFile,
   int                 theLine,
   const std::string&       theIdentifier)
:  ScriptNode(theFile, theLine, true),
   identifierM(theIdentifier)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeIdentifier::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->countNodeExecution(this);
   return theExecutionContext->getIdentifier(identifierM);
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeIdentifier::getType() const
{
   return ScriptNode::IdentifierE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeIdentifier::getValue() const
{
   return ScriptValue(identifierM);
}

// ----------------------------------------------------------------------------

void
ScriptNodeIdentifier::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Identifier '";
   theOutputString += identifierM + "'\n";
}


