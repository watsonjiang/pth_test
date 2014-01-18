#include "JsScannerUtilities.h"
#include "JsScriptObject.h"
#include "JsScriptReference.h"
#include "JsScriptReferenceError.h"
#include "JsScriptTypeError.h"
#include "JsScriptValue.h"
#include <iomanip>
#include <limits>
#include <sstream>

using namespace Js;

const ScriptValue ScriptValue::UndefinedC;

// ----------------------------------------------------------------------------

ScriptValue::ScriptValue(
   ScriptValue& theObject,
   ScriptValue& thePropertyName)
:  cmdAndDataTypeM(UndefinedE)
{
   valueM.referenceValue = 
      new ScriptReference(theObject.toObject(), thePropertyName.toString());
   valueM.referenceValue->AddRef();
   cmdAndDataTypeM = ReferenceE;
}

// ----------------------------------------------------------------------------

ScriptValue::ScriptValue(
   Command      theCommand,
   ScriptValue& theObject,
   ScriptValue& thePropertyName)
:  cmdAndDataTypeM(UndefinedE)
{
   valueM.referenceValue = 
      new ScriptReference(theObject.toObject(), thePropertyName.toString());
   valueM.referenceValue->AddRef();
   cmdAndDataTypeM = theCommand | ReferenceE;
}

// ----------------------------------------------------------------------------

ScriptValue::ScriptValue(
   const ScriptObject* theObject,
   const std::string&       thePropertyName)
:  cmdAndDataTypeM(ReferenceE)
{
   valueM.referenceValue = new ScriptReference(const_cast<ScriptObject*>(theObject), 
                                               thePropertyName);
   valueM.referenceValue->AddRef();
}

// ----------------------------------------------------------------------------

ScriptValue::ScriptValue(
   Command             theCommand,
   const ScriptObject* theObject,
   const std::string&       thePropertyName)
:  cmdAndDataTypeM(theCommand | ReferenceE)
{
   valueM.referenceValue = 
      new ScriptReference(const_cast<ScriptObject*>(theObject), 
                          thePropertyName);
   valueM.referenceValue->AddRef();
}

// ----------------------------------------------------------------------------

ScriptValue::~ScriptValue()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case ObjectE:
      {
         if (valueM.objectValue != 0)
         {
            valueM.objectValue->Release();
         }
         break;
      }
      case ReferenceE:
      {
         if (valueM.referenceValue != 0)
         {
            valueM.referenceValue->Release();
         }
         break;
      }
      case StringE:
      {
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         break;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptValue&
ScriptValue::operator=(
   const ScriptValue& theOther)
{
   if (this == &theOther)
   {
      return *this;
   }
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case ObjectE:
      {
         if (valueM.objectValue != 0)
         {
            valueM.objectValue->Release();
         }
         break;
      }
      case ReferenceE:
      {
         if (valueM.referenceValue != 0)
         {
            valueM.referenceValue->Release();
         }
         break;
      }
      case StringE:
      {
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         break;
      }
   }
   cmdAndDataTypeM = theOther.cmdAndDataTypeM;
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         valueM.booleanValue = theOther.valueM.booleanValue;
         break;
      }
      case NumberE:
      {
         valueM.numberValue = theOther.valueM.numberValue;
         break;
      }
      case ObjectE:
      {
         if (theOther.valueM.objectValue != 0)
         {
            valueM.objectValue = theOther.valueM.objectValue;
            valueM.objectValue->AddRef();
         }
         else
         {
            cmdAndDataTypeM &= NotDataTypeMaskE;
            cmdAndDataTypeM |= NullE;
         }
         break;
      }
      case ReferenceE:
      {
         if (theOther.valueM.referenceValue != 0)
         {
            valueM.referenceValue = theOther.valueM.referenceValue;
            valueM.referenceValue->AddRef();
         }
         else
         {
            cmdAndDataTypeM &= NotDataTypeMaskE;
            cmdAndDataTypeM |= NullE;
         }
         break;
      }
      case StringE:
      {
         if (theOther.valueM.stringValue != 0)
         {
            valueM.stringValue = theOther.valueM.stringValue;
            valueM.stringValue->AddRef();
         }
         else
         {
            cmdAndDataTypeM &= NotDataTypeMaskE;
            cmdAndDataTypeM |= NullE;
         }
         break;
      }
   }
   return *this;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::deleteOperator()
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == ReferenceE)
   {
      return valueM.referenceValue->deleteOperator();
   }
   else
   {
      return ScriptValue(false);
   }
}

// ----------------------------------------------------------------------------

void
ScriptValue::format(
   std::string&       theFormatedValue,
   const std::string& theLinePrefix)
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         if (valueM.booleanValue == true)
         {
            theFormatedValue.append("true");
         }
         else
         {
            theFormatedValue.append("false");
         }
         break;
      }
      case NullE:
      {
         theFormatedValue.append("null");
         break;
      }
      case NumberE:
      {
         if (valueM.numberValue == NaNC)
         {
            theFormatedValue.append("NaN");
         }
         else
         if (valueM.numberValue == InfinityC)
         {
            theFormatedValue.append("Infinity");
         }
         else
         {
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<Number>::digits10)
               << valueM.numberValue;
            theFormatedValue.append(ss.str());
         }
         break;
      }
      case ObjectE:
      {
         theFormatedValue.append("{");
         std::string propertyName;
         if (valueM.objectValue->getNameOfFirstProperty(propertyName) == false)
         {
            theFormatedValue.append(" }");
            return;
         }

         theFormatedValue.append("\n");

         bool isFirstProperty = true;
         do
         {
            if (isFirstProperty == false)
            {
               theFormatedValue.append(";\n");
            }
            else
            {
               isFirstProperty = false;
            }
            theFormatedValue.append(theLinePrefix + "   ");
            theFormatedValue.append("\"");
            theFormatedValue.append(propertyName);
            theFormatedValue.append("\" = ");
            ScriptValue value = valueM.objectValue->getProperty(propertyName).getValue();
            if (value.isObject() == true)
            {
               theFormatedValue.append("\n");
               theFormatedValue.append(theLinePrefix + "   ");
            }
            value.format(theFormatedValue,
                         theLinePrefix + "   ");
         }
         while (valueM.objectValue->getNameOfNextProperty(propertyName) == true);
         theFormatedValue.append("\n");
         theFormatedValue.append(theLinePrefix);
         theFormatedValue.append("}");
         break;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         theFormatedValue.append("\"");
         theFormatedValue.append(*valueM.stringValue);
         theFormatedValue.append("\"");
         break;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.8
         theFormatedValue.append("undefined");
         break;
      }
   }
}

// ----------------------------------------------------------------------------

void
ScriptValue::jsonFormat(
   std::string&             theFormatedValue,
   const ScriptString* theFile,
   int&                theLine,
   int&                theColumn,
   const std::string&       theFormatOption,
   const std::string&       theLinePrefix)
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         if (valueM.booleanValue == true)
         {
            theFormatedValue.append("true");
         }
         else
         {
            theFormatedValue.append("false");
         }
         break;
      }
      case NullE:
      {
         theFormatedValue.append("null");
         break;
      }
      case NumberE:
      {
         if (valueM.numberValue == NaNC)
         {
            theFormatedValue.append("NaN");
         }
         else
         if (valueM.numberValue == InfinityC)
         {
            theFormatedValue.append("Infinity");
         }
         else
         {
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<Number>::digits10)
               << valueM.numberValue;
            theFormatedValue.append(ss.str());
         }
         break;
      }
      case ObjectE:
      {
         bool jb(false);
         if(0 == theFormatOption.compare("jb"))
         {
            jb = true;
         }

         if(valueM.objectValue->getObjectType() == ScriptObject::ArrayE)
         {
            // Array
            theFormatedValue.append("[");
         }
         else
         {
            // Plain object
            theFormatedValue.append("{");
         }

         std::string propertyName;
         if (valueM.objectValue->getNameOfFirstProperty(propertyName) == false)
         {
            if(valueM.objectValue->getObjectType() == ScriptObject::ArrayE)
            {
               // Array
               theFormatedValue.append("]");
            }
            else
            {
               // Plain object
               theFormatedValue.append("}");
            }
            return;
         }

         if(jb == true)
         {
            theFormatedValue.append("\n");
         }


         bool isFirstProperty = true;
         do
         {
            if (isFirstProperty == false)
            {
               if(jb == true)
               {
                  theFormatedValue.append(",\n");
               }
               else
               {
                  theFormatedValue.append(",");
               }
            }
            else
            {
               isFirstProperty = false;
            }

            ScriptValue value = valueM.objectValue->getProperty(propertyName).getValue();
            if(valueM.objectValue->getObjectType() != ScriptObject::ArrayE)
            {
               // Only add the name if the object is not an array.
               if(jb == true)
               {
                  theFormatedValue.append(theLinePrefix + "   ");
               }
               theFormatedValue.append("\"");
               theFormatedValue.append(propertyName);
               theFormatedValue.append("\":");

               if(value.isObject() == true && jb == true)
               {
                  theFormatedValue.append("\n");
               }

            }
            if (value.isObject() == true && jb == true)
            {
               theFormatedValue.append(theLinePrefix + "   ");
            }
            value.jsonFormat(
                  theFormatedValue,
                  theFile,
                  theLine,
                  theColumn,
                  theFormatOption,
                  theLinePrefix + "   ");
         }
         while (valueM.objectValue->getNameOfNextProperty(propertyName) == true);

         if(jb == true)
         {
            theFormatedValue.append("\n");
            theFormatedValue.append(theLinePrefix);
         }

         if(valueM.objectValue->getObjectType() == ScriptObject::ArrayE)
         {
            // Array
            theFormatedValue.append("]");
         }
         else
         {
            // Plain object
            theFormatedValue.append("}");
         }
         break;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         theFormatedValue.append("\"");
         theFormatedValue.append(*valueM.stringValue);
         theFormatedValue.append("\"");
         break;
      }
      case UndefinedE:
      {
         // JSON does not define
         throw ScriptTypeError("'undefined' cannot be converted to JSON", theFile, theLine);
         break;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptObject*
ScriptValue::getBase()
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == ReferenceE)
   {
      return valueM.referenceValue->getBase();
   }
   else
   {
      return 0;
   }
}

// ----------------------------------------------------------------------------
static std::string EmptyStringC;

const std::string&
ScriptValue::getPropertyName() const
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == ReferenceE)
   {
      return valueM.referenceValue->getPropertyName();
   }
   else
   {
      return EmptyStringC;
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::getTypeof() const
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         return ScriptValue("boolean");
      }
      case NullE:
      {
         return ScriptValue("object");
      }
      case NumberE:
      {
         return ScriptValue("number");
      }
      case ObjectE:
      {
         return ScriptValue("object");
      }
      case ReferenceE:
      {
         return getValue().getTypeof();
      }
      case StringE:
      {
         return ScriptValue("string");
      }
      case UndefinedE:
      default:
      {
         return ScriptValue("undefined");
      }
   }
}

// ----------------------------------------------------------------------------

void
ScriptValue::putValue(
   const ScriptValue& theValue)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == ReferenceE)
   {
      valueM.referenceValue->putValue(theValue);
   }
   else
   {
      throw ScriptReferenceError(
         "Can't assign a value to an expression,"
         " which is no left-hand-side expression.");
   }
}

// ----------------------------------------------------------------------------

void
ScriptValue::print() const
{
   ScriptValue tempValue(*this);
   tempValue.toString();
   printf(tempValue.valueM.stringValue->c_str());
}

// ----------------------------------------------------------------------------

void
ScriptValue::read(
   const char*& theCurrentChar, 
   int&         theLine, 
   int&         theColumn)
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         std::string value = ScannerUtilities::getIdentifier(theCurrentChar,
                                                        theLine, 
                                                        theColumn);
         if (value == "true")
         {
            valueM.booleanValue = true;
         }
         else
         if (value == "false")
         {
            valueM.booleanValue = false;
         }
         else
         {
            // Neither 'true' nor 'false'
            throw ScannerException("'true' or 'false' expected", 
                                   theLine, 
                                   theColumn);
         }
         break;
      }
      case NullE:
      {
         std::string value = ScannerUtilities::getIdentifier(theCurrentChar, 
                                                        theLine, 
                                                        theColumn);
         if (value != "null")
         {
            // Value is not 'undefined'
            throw ScannerException("'null' expected", 
                                   theLine, 
                                   theColumn);
         }
         break;
      }
      case NumberE:
      {
         valueM.numberValue = ScannerUtilities::getNumber(theCurrentChar,
                                                          theLine, 
                                                          theColumn);
         break;
      }
      case ObjectE:
      {
         if (valueM.objectValue != 0)
         {
            valueM.objectValue->Release();
         }
         valueM.objectValue = new ScriptObject(ScriptObject::ObjectE);
         valueM.objectValue->AddRef();
         valueM.objectValue->read(theCurrentChar, 
                                  theLine, 
                                  theColumn);
         break;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         valueM.stringValue = new ScriptString;
         valueM.stringValue->AddRef();
         valueM.stringValue->read(theCurrentChar, 
                                  theLine, 
                                  theColumn);
         break;
      }
      case UndefinedE:
      {
         std::string value = ScannerUtilities::getIdentifier(theCurrentChar, 
                                                        theLine, 
                                                        theColumn);
         if (value != "undefined")
         {
            // Value is not 'undefined'
            throw ScannerException("'undefined' expected", 
                                   theLine, 
                                   theColumn);
         }
         break;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator+(
   ScriptValue& theRhs)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == StringE || 
       (theRhs.cmdAndDataTypeM & DataTypeMaskE) == StringE)
   {
      return ScriptValue(toString() + theRhs.toString());
   }
   else
   {
      return ScriptValue(toNumber() + theRhs.toNumber());
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator==(
   ScriptValue& theRhs)
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == BooleanE)
         {
            return ScriptValue(toBoolean() == theRhs.toBoolean());
         }
         else
         {
            toNumber();
            return *this == theRhs;
         }
         break;
      }
      case NullE:
      {
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == NullE || 
             (theRhs.cmdAndDataTypeM & DataTypeMaskE) == UndefinedE)
         {
            return ScriptValue(true);
         }
         else
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == BooleanE)
         {
            return ScriptValue(toBoolean() == theRhs.toBoolean());
         }
         break;
      }
      case NumberE:
      {
         return ScriptValue(toNumber() == theRhs.toNumber());
      }
      case ObjectE:
      {
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == ObjectE)
         {
            return toObject() == theRhs.toObject();
         }
         break;;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == StringE)
         {
            return ScriptValue(toString() == theRhs.toString());
         }
         else
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == NumberE)
         {
            return ScriptValue(toNumber() == theRhs.toNumber());
         }
         else
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == BooleanE)
         {
            theRhs.toNumber();
            return *this == theRhs;
         }
         break;
      }
      case UndefinedE:
      {
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == UndefinedE || 
             (theRhs.cmdAndDataTypeM  & DataTypeMaskE) == NullE)
         {
            return ScriptValue(true);
         }
         else
         if ((theRhs.cmdAndDataTypeM & DataTypeMaskE) == BooleanE)
         {
            toBoolean();
            return ScriptValue(valueM.booleanValue == 
                               theRhs.valueM.booleanValue);
         }
         break;
      }
   }
   return ScriptValue(false);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator<(
   ScriptValue& theRhs)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == StringE && 
       (theRhs.cmdAndDataTypeM  & DataTypeMaskE) == StringE)
   {
      return ScriptValue(toString() < theRhs.toString());
   }
   else
   {
      return ScriptValue(toNumber() < theRhs.toNumber());
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator<=(
   ScriptValue& theRhs)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == StringE && 
       (theRhs.cmdAndDataTypeM & DataTypeMaskE) == StringE)
   {
      return ScriptValue(!(toString() > theRhs.toString()));
   }
   else
   {
      return ScriptValue(toNumber() <= theRhs.toNumber());
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator>(
   ScriptValue& theRhs)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == StringE && 
       (theRhs.cmdAndDataTypeM & DataTypeMaskE) == StringE)
   {
      return ScriptValue(toString() > theRhs.toString());
   }
   else
   {
      return ScriptValue(toNumber() > theRhs.toNumber());
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::operator>=(
   ScriptValue& theRhs)
{
   if ((cmdAndDataTypeM & DataTypeMaskE) == StringE && 
       (theRhs.cmdAndDataTypeM & DataTypeMaskE) == StringE)
   {
      return ScriptValue(!(toString() < theRhs.toString()));
   }
   else
   {
      return ScriptValue(toNumber() >= theRhs.toNumber());
   }
}

// ----------------------------------------------------------------------------
// PRIVATE METHODS
// ----------------------------------------------------------------------------

void
ScriptValue::addRefObjectValue() const
{
   valueM.objectValue->AddRef();
}

// ----------------------------------------------------------------------------

void
ScriptValue::addRefReferenceValue() const
{
   valueM.referenceValue->AddRef();
}

// ----------------------------------------------------------------------------

bool
ScriptValue::convertToBoolean()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         // See ECMA-262 9.2
         return valueM.booleanValue;
      }
      case NullE:
      {
         // See ECMA-262 9.2
         valueM.booleanValue = false;
         break;
      }
      case NumberE:
      {
         // See ECMA-262 9.2
         if (valueM.numberValue == 0 || valueM.numberValue == NaNC)
         {
            valueM.booleanValue = false;
         }
         else
         {
            valueM.booleanValue = true;
         }
         break;
      }
      case ObjectE:
      {
         // See ECMA-262 9.2
         if (valueM.objectValue != 0)
         {
            valueM.objectValue->Release();
         }
         valueM.booleanValue = true;
         break;
      }
      case ReferenceE:
      {
         // TO BE DONE!!!
         if (valueM.referenceValue != 0)
         {
            valueM.referenceValue->Release();
            valueM.referenceValue = 0;
         }
         break;
      }
      case StringE:
      {
         // See ECMA-262 9.2
         bool result = valueM.stringValue->size() > 0;
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         valueM.booleanValue = result;
         break;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.2
         valueM.booleanValue = false;
         break;
      }
   }
   cmdAndDataTypeM = BooleanE;
   return valueM.booleanValue;
}

// ----------------------------------------------------------------------------

Number
ScriptValue::convertToNumber()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         // See ECMA-262 9.3
         if (valueM.booleanValue == true)
         {
            valueM.numberValue = 1;
         }
         else
         {
            valueM.numberValue = 0;
         }
         break;
      }
      case NullE:
      {
         // See ECMA-262 9.3
         valueM.numberValue = 0;
         break;
      }
      case NumberE:
      {
         // See ECMA-262 9.3
         return valueM.numberValue;
      }
      case ObjectE:
      {
         // See ECMA-262 9.3
         // TO BE DONE!!!
         if (valueM.objectValue != 0)
         {
            valueM.objectValue->Release();
            valueM.objectValue = 0;
         }
         break;
      }
      case ReferenceE:
      {
         // TO BE DONE!!!
         if (valueM.referenceValue != 0)
         {
            valueM.referenceValue->Release();
            valueM.referenceValue = 0;
         }
         break;
      }
      case StringE:
      {
         // See ECMA-262 9.3
         Number result = valueM.stringValue->toNumber();
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         valueM.numberValue = result;
         break;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.3
         valueM.numberValue =  NaNC;
         break;
      }
   }
   cmdAndDataTypeM = NumberE;
   return valueM.numberValue;
}

// ----------------------------------------------------------------------------

ScriptObject*
ScriptValue::convertToObject()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         ScriptObject* tempObject = new ScriptObject(ScriptObject::ObjectE);
         tempObject->putProperty("value", *this);
         valueM.objectValue = tempObject;
         valueM.objectValue->AddRef();
         break;
      }
      case NullE:
      {
         // See ECMA-262 9.9
         throw ScriptTypeError(
            "Can't convert a null value into an object.",
            0);
      }
      case NumberE:
      {
         ScriptObject* tempObject = new ScriptObject(ScriptObject::ObjectE);
         tempObject->putProperty("value", *this);
         valueM.objectValue = tempObject;
         valueM.objectValue->AddRef();
         break;
      }
      case ObjectE:
      {
         return valueM.objectValue;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         ScriptObject* tempObject = new ScriptObject(ScriptObject::ObjectE);
         tempObject->putProperty("value", *this);
         if (valueM.stringValue != 0)
         {
            valueM.stringValue->Release();
         }
         valueM.objectValue = tempObject;
         valueM.objectValue->AddRef();
         break;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.9
         throw ScriptTypeError(
            "Can't convert an undefined value into an object.",
            0);
         break;
      }
   }
   cmdAndDataTypeM = ObjectE;
   return valueM.objectValue;
}

// ----------------------------------------------------------------------------

ScriptString*
ScriptValue::convertToScriptString()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         // See ECMA-262 9.8
         if (valueM.booleanValue == true)
         {
            valueM.stringValue = new ScriptString("true");
         }
         else
         {
            valueM.stringValue = new ScriptString("false");
         }
         valueM.stringValue->AddRef();
         break;
      }
      case NullE:
      {
         // See ECMA-262 9.8
         valueM.stringValue = new ScriptString("null");
         valueM.stringValue->AddRef();
         break;
      }
      case NumberE:
      {
         // See ECMA-262 9.8
         if (valueM.numberValue == NaNC)
         {
            valueM.stringValue = new ScriptString("NaN");
         }
         else
         if (valueM.numberValue == InfinityC)
         {
            valueM.stringValue = new ScriptString("Infinity");
         }
         else
         {
            ScriptString* tempString = new ScriptString;
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<Number>::digits10)
               << valueM.numberValue;
            tempString->append(ss.str());
            valueM.stringValue = tempString;
         }
         valueM.stringValue->AddRef();         
         break;
      }
      case ObjectE:
      {
         *this = valueM.objectValue->getStringValue();
         break;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         // See ECMA-262 9.8
         return valueM.stringValue;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.8
         valueM.stringValue = new ScriptString("undefined");
         valueM.stringValue->AddRef();
         break;
      }
   }
   cmdAndDataTypeM = StringE;
   return valueM.stringValue;
}

// ----------------------------------------------------------------------------

const std::string&
ScriptValue::convertToString()
{
   switch (cmdAndDataTypeM & DataTypeMaskE)
   {
      case BooleanE:
      {
         // See ECMA-262 9.8
         if (valueM.booleanValue == true)
         {
            valueM.stringValue = new ScriptString("true");
         }
         else
         {
            valueM.stringValue = new ScriptString("false");
         }
         valueM.stringValue->AddRef();
         break;
      }
      case NullE:
      {
         // See ECMA-262 9.8
         valueM.stringValue = new ScriptString("null");
         valueM.stringValue->AddRef();
         break;
      }
      case NumberE:
      {
         // See ECMA-262 9.8
         if (valueM.numberValue == NaNC)
         {
            valueM.stringValue = new ScriptString("NaN");
         }
         else
         if (valueM.numberValue == InfinityC)
         {
            valueM.stringValue = new ScriptString("Infinity");
         }
         else
         {
            ScriptString* tempString = new ScriptString;
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<Number>::digits10)
               << valueM.numberValue; 
            tempString->append(ss.str());
            valueM.stringValue = tempString;
         }
         valueM.stringValue->AddRef();         
         break;
      }
      case ObjectE:
      {
         *this = valueM.objectValue->getStringValue();
         break;
      }
      case ReferenceE:
      {
         break;
      }
      case StringE:
      {
         // See ECMA-262 9.8
         return *valueM.stringValue;
      }
      case UndefinedE:
      {
         // See ECMA-262 9.8
         valueM.stringValue = new ScriptString("undefined");
         valueM.stringValue->AddRef();
         break;
      }
   }
   cmdAndDataTypeM = StringE;
   return *valueM.stringValue;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptValue::getReferenceValue() const
{
   return valueM.referenceValue->getValue();
}

// ----------------------------------------------------------------------------

void
ScriptValue::releaseObjectValue() const
{
   valueM.objectValue->Release();
}

// ----------------------------------------------------------------------------

void
ScriptValue::releaseReferenceValue() const
{
   valueM.referenceValue->Release();
}

