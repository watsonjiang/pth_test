#include "JsScriptNodeIdDotLiteral.h"

using namespace Js;

// ----------------------------------------------------------------------------

ScriptNodeIdDotLiteral::ScriptNodeIdDotLiteral(
   const ScriptString* theFile,
   int                 theLine,
   const std::string&       theIdentifier,
   const std::string&       theLiteral)
:  ScriptNode(theFile, theLine, true),
   identifierM(theIdentifier),
   literalM(theLiteral)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeIdDotLiteral::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->countNodeExecution(this);
   ScriptValue op1 = theExecutionContext->getIdentifier(identifierM);
   if (op1.getDataType() == ScriptValue::ReferenceE)
   {
      op1 = op1.getReferenceValue();
   }
   return ScriptValue(op1.toObject(), literalM);
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeIdDotLiteral::getType() const
{
   return ScriptNode::IdDotLiteralE;
}

// ----------------------------------------------------------------------------

void
ScriptNodeIdDotLiteral::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += identifierM + " . " + literalM + "'\n";
}


