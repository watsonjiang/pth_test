#include "JsScriptNodeLiteral.h"
using namespace Js;

ScriptNodeLiteral::ScriptNodeLiteral(
   const ScriptString*  theFile,
   int                  theLine)
:  ScriptNode(theFile, theLine, false),
   valueM(ScriptValue::NullE)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeLiteral::ScriptNodeLiteral(
   const ScriptString* theFile,
   int                 theLine,
   bool                theBoolean)
:  ScriptNode(theFile, theLine, false),
   valueM(theBoolean)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeLiteral::ScriptNodeLiteral(
   const ScriptString* theFile,
   int                 theLine,
   Number              theNumber)
:  ScriptNode(theFile, theLine, false),
   valueM(theNumber)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeLiteral::ScriptNodeLiteral(
   const ScriptString* theFile,
   int                 theLine,
   const std::string&       theString)
:  ScriptNode(theFile, theLine, false),
   valueM(theString)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptNodeLiteral::ScriptNodeLiteral(
   const ScriptString*   theFile,
   int                   theLine,
   ScriptValue::DataType theDatatype)
:  ScriptNode(theFile, theLine, false),
   valueM(theDatatype)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeLiteral::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->countNodeExecution(this);
   return valueM; 
}

// ----------------------------------------------------------------------------

ScriptNode::Type
ScriptNodeLiteral::getType() const
{
   return ScriptNode::LiteralE;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNodeLiteral::getValue() const
{
   return valueM;
}

// ----------------------------------------------------------------------------

void
ScriptNodeLiteral::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix,
   size_t        theMaxFilenameLength) const
{
   if (theLinePrefix.empty() == true)
   { 
      printFileAndLine(theOutputString, theMaxFilenameLength);
   }
   theOutputString += "Literal ";
   switch (valueM.getDataType())
   {
      case ScriptValue::BooleanE:
      case ScriptValue::NullE:
      case ScriptValue::NumberE:
      case ScriptValue::UndefinedE:
      {
         theOutputString += getValue().toString();
         break;
      }
      case ScriptValue::StringE:
      {
         theOutputString += "\"";
         std::string tempString = getValue().toString();
         for (std::string::const_iterator iter = tempString.begin();
              iter != tempString.end();
              iter++)
         {
            if (isprint((unsigned char)*iter))
            {
               theOutputString += *iter; 
            }
            else
            {
               theOutputString += "\\0x";
               char buf[10] = {0};
               snprintf(buf, 9, "%02x", (unsigned char)(*iter));
               //theOutputString.appendNumber((unsigned char)(*iter), 16, 2, true);
               theOutputString.append(buf);
            }
         }
         theOutputString += "\"";
         break;
      }
      default:
      {
         // empty
      }
   }
   theOutputString += "\n";
}


