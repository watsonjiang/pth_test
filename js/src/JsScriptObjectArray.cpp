#include "JsScannerUtilities.h"
#include "JsScriptObjectArray.h"
#include "JsScriptTypeError.h"

using namespace Js;

static const std::string LengthC("length");
static const std::string PopC("pop");
static const std::string PushC("push");

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectArray::ClassNameC = "Array";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObjectArray::ScriptObjectArray()
:  ScriptObject(ScriptObject::ArrayE),
   lengthM(0)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectArray::~ScriptObjectArray()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObjectArray::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   if (theArguments.size() == 0)
   {
      return new ScriptObjectArray;
   }

   if (theArguments.size() == 1)
   {
      ScriptValue lenOp = theArguments[0].getValue();
      if (lenOp.isNumber() == true)
      {
         int len = lenOp.toNumber();
         if (len < 0)
         {
            // FIXME should throw RangeError
            throw ScriptTypeError(
                  "Array():"
                  " The value of 'len' must be greater or equal to zero.");
         }
         ScriptObjectArray* result = new ScriptObjectArray;
         result->lengthM = len;
         return result;
      }
      else
      {
         ScriptObjectArray* result = new ScriptObjectArray;
         result->putPropertyNoCheck(0, lenOp);
         result->lengthM = 1;
         return result;
      }
   }
   
   ScriptObjectArray* result = new ScriptObjectArray;
   result->lengthM = theArguments.size();
   for (size_t i = 0; i < theArguments.size(); i++)
   {
      ScriptValue op = theArguments[i].getValue();
      result->putPropertyNoCheck(i, op);
   }

   return result;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectArray::call(
   const std::string&           theName,
   ScriptExecutionContext* theContext, 
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{
   if (theName == PushC)
   {
      if (theArguments.size() < 1)
      {
         throw ScriptTypeError(
            "Array.push():"
            " Function requires at least one argument (value, ...).");
      }
      for (int i = 0; i < theArguments.size(); i++)
      {
         std::string index;
         {
         char buf[10] = {0};
         snprintf(buf, 9, "%d", lengthM++);
         //index.appendNumber(lengthM++);
         index.append(buf);
         }
         ScriptObject::putPropertyNoCheck(index, theArguments[i]);
      }
      return ScriptValue::UndefinedC;
   }
   else
   if (theName == PopC)
   {
      if (theArguments.size() != 0)
      {
         throw ScriptTypeError(
            "Array.pop():"
            " Function requires allows no arguments.");
      }
      if (lengthM == 0)
      {
         return ScriptValue::UndefinedC;
      }
      std::string index;
      {
         char buf[10] = {0};
         snprintf(buf, 10, "%d", lengthM -1); 
         //index.appendNumber(lengthM - 1);
         index.append(buf);
      }
      ScriptValue result = ScriptObject::getProperty(index);
      ScriptObject::deleteProperty(index);
      lengthM--;
      return result;      
   }
   else
   {
      return ScriptObject::call(theName, 
                                theContext, 
                                theArguments, 
                                theIsConstructor);
   }
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectArray::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectArray::getProperty(
   const std::string& thePropertyName) const
{
   if (thePropertyName == LengthC)
   {
      return ScriptValue((Number)lengthM);
   }
   return ScriptObject::getProperty(thePropertyName);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectArray::getProperty(
   Number theIndex) const
{
   std::string propertyName;
   char buf[10] = {0};
   snprintf(buf, 10, "%d", theIndex);
   //propertyName.appendNumber(theIndex);
   propertyName.append(buf);
   return ScriptObject::getProperty(propertyName);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectArray::getStringValue() const
{
   ScriptString* result = new ScriptString;
   for (int i = 0; i < lengthM; i++)
   {
      result->append(getProperty(i).toString());
      if (i < lengthM - 1)
      {
         result->append(",");
      }
   }
   return ScriptValue(result);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectArray::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObject::isInstanceOf(theClassName);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectArray::putProperty(
   const std::string&      thePropertyName,
   const ScriptValue& theValue)
{
   if (thePropertyName == LengthC)
   {
      ScriptValue numberValue(theValue);
      Number newLength = numberValue.toNumber();
      if (newLength < lengthM)
      {
         Properties::iterator next;
         for (Properties::iterator iter = propertiesM.begin();
              iter != propertiesM.end();
              iter = next)
         {
            ScriptValue index = (*iter).first;
            
            next = iter;
            next++;
            if (index.toNumber() != NaNC &&
                index.toNumber() >= newLength)
            {
               propertiesM.erase(iter);
            }
         }
      }
      lengthM = newLength >= 0 ? newLength : 0;
      return true;
   }
   
   const char* currentChar = thePropertyName.c_str();
   ScannerUtilities::skipStringWhiteSpace(currentChar);
   switch (*currentChar)
   {
      case 0: // Empty string or only whitespace - no index
      case '-': // Negative number - no index
      {
         return ScriptObject::putProperty(thePropertyName, theValue);
      }
      case '+':
      {
         currentChar++; // 'eat' +
         break;
      }
   }
   try
   {
      Number index = ScannerUtilities::getNumber(currentChar);
      ScannerUtilities::skipStringWhiteSpace(currentChar);
      if (*currentChar == 0)
      {
         // OK! Nothing strange after digits - set new length
         index++;
         if (index > lengthM)
         {
            lengthM = index;
         }
      }
   }
   catch (ScannerException&)
   {
      // Ignore this
   }

   return ScriptObject::putProperty(thePropertyName, theValue);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectArray::putProperty(
   int                theIndex,
   const ScriptValue& theValue)
{
   string propertyName;
   char buf[10] = {0};
   snprintf(buf, 10, "%d", theIndex);
   //propertyName.appendNumber(theIndex);
   propertyName.append(buf);
   if (theIndex >= lengthM)
   {
      lengthM = theIndex + 1;
   }
   return ScriptObject::putProperty(propertyName, theValue);
}

// ----------------------------------------------------------------------------

void
ScriptObjectArray::read(
   const char*& theCurrentChar,
   int&         theLine,
   int&         theColumn)
{

   // Skip trailing spaces
   ScannerUtilities::skipStringWhiteSpace(theCurrentChar, theLine, theColumn);

   int index(0);
   while (*theCurrentChar != 0 && *theCurrentChar != ']')
   {
      // Parse property value
      ScriptValue value = readPropertyValue(theCurrentChar,
                                            theLine,
                                            theColumn);

      if (putProperty(index, value) == false)
      {
         // Can't put that property.
         string message = "Can't put property '";
         char buf[10] = {0};
         snprintf(buf, 10, "%d", index);
         //message.append(index);
         message.append(buf);
         message.append("'. Property is read-only.");
         throw ScannerException(message, theLine, theColumn);
      }

      ScannerUtilities::skipStringWhiteSpace(theCurrentChar,
                                             theLine,
                                             theColumn);

      // Skip optional separating ';' or ','
      if (*theCurrentChar == ';' || *theCurrentChar == ',')
      {
         theCurrentChar++;
         theColumn++;
      }

      ScannerUtilities::skipStringWhiteSpace(theCurrentChar,
                                             theLine,
                                             theColumn);
      index++;
   }
}

// ----------------------------------------------------------------------------
