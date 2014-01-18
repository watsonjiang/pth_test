#include "JsScriptNode.h"
using namespace Js;

ScriptNode::~ScriptNode()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptNode::getValue() const
{
   return ScriptValue::UndefinedC;
}

// ----------------------------------------------------------------------------

void
ScriptNode::printFileAndLine(
   std::string& theOutputString,
   size_t  theMaxFilenameLength) const
{
   std::string filename;
   if (fileM != 0)
   {
      filename = *fileM;
   }

   if (filename.size() > theMaxFilenameLength)
   {
      // trim filename if it is too long
      filename.erase(theMaxFilenameLength - 3);
      filename.append("...");
   }

   // file filename with spaces if it is to short
   while (filename.size() < theMaxFilenameLength)
   {
      filename.append(" ");
   }

   theOutputString.append(filename);
   theOutputString.append(" ");
   char buf[10] = {0};
   snprintf(buf, 9, "%4d", lineM);
   //theOutputString.appendNumber(lineM, 10, 4, true);
   theOutputString.append(buf);
   theOutputString.append(" ");
}
   
