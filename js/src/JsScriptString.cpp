#include "JsScannerUtilities.h"
#include "JsScriptString.h"

using namespace Js;

ScriptString::ScriptString()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptString::ScriptString(
   const std::string& theOther)
:   std::string(theOther)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptString::ScriptString(
   const ScriptString* theOther)
:   std::string(*theOther)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptString::ScriptString(
   const char* theFirst,
   const char* theLast)
:  std::string(theFirst, theLast)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptString::ScriptString(
   const char* theString)
:   std::string(theString)
{
   // Empty
}

ScriptString::ScriptString(
   const char*  theString,
   size_t       theNumberOfCharacters)
: std::string(theString, theNumberOfCharacters)
{
   // Empty
}
// ----------------------------------------------------------------------------

ScriptString::~ScriptString()
{
   // Empty
}

// ----------------------------------------------------------------------------

void
ScriptString::read(
   const char*& theCurrentChar, 
   int&         theLine, 
   int&         theColumn)
{
   assign(ScannerUtilities::getString(theCurrentChar, theLine, theColumn));
}

// ----------------------------------------------------------------------------

Number
ScriptString::toNumber() const
{
   // See ECMA-262 9.3.1
   const char* currentChar = c_str();
   ScannerUtilities::skipStringWhiteSpace(currentChar);
   bool isPositive = true;
   switch (*currentChar)
   {
      case 0: // Empty string or only whitespace
      {
         return 0;
      }
      case '+':
      {
         currentChar++; // 'eat' +
         isPositive = true;
         break;
      }
      case '-':
      {
         currentChar++; // 'eat' -
         isPositive = false;
         break;
      }
   }
   try
   {
      Number result = ScannerUtilities::getNumber(currentChar);
      ScannerUtilities::skipStringWhiteSpace(currentChar);
      if (*currentChar == 0)
      {
         // OK! Nothing strange after digits
         if (isPositive)
         {
            return result;
         }
         else
         {
            return -result;
         }
      }
   }
   catch (ScannerException&)
   {
      // Ignore this
   }

   // There is some strange stuff - Return "Not A Number"
   return NaNC;
}
