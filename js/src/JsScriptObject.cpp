#include "JsScannerUtilities.h"
#include "JsScriptExecutionContext.h"
#include "JsScriptObject.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptObjectArray.h"
#include "JsScriptTypeError.h"
#include <iomanip>
#include <limits>
#include <sstream>

using namespace Js;

const std::string 
ScriptObject::PrototypeC("prototype");

static const std::string ThisC("this");

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObject::ClassNameC = "Object";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObject::ScriptObject(
   ObjectType theObjectType)
:  objectTypeM(theObjectType)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObject::~ScriptObject()
{
   // Empty
}

// ----------------------------------------------------------------------------

bool
ScriptObject::operator==(
   const ScriptValue& theOther) const
{
   if (theOther.getDataType() == ScriptValue::ObjectE)
   {
      return this == &((const ScriptObject&)theOther);
   }
   return false;
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject* 
ScriptObject::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObject(ScriptObject::ObjectE);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObject::call(
   const std::string&           theName,
   ScriptExecutionContext* theContext, 
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{
   ScriptValue fctRef(getProperty(theName));
   ScriptValue fct(fctRef.getValue());
   if (fct.getDataType() != ScriptValue::ObjectE)
   {
      std::string errorMessage("Identifier '" + 
                          theName + 
                          "' does not refer to a function."); 
      throw ScriptTypeError(errorMessage);
   }
   ScriptObject* obj = fct.toObject();
   if (obj->getObjectType() != ScriptObject::FunctionE)
   {
      std::string errorMessage("Identifier '" + 
                          theName + 
                          "' does not refer to a function."); 
      throw ScriptTypeError(errorMessage);
   }

   ScriptObjectActivation* activationObject = new ScriptObjectActivation;

   if (theIsConstructor == false)
   {
      // Function is not used as a constructor. 
      // Determine what the 'this' object is.
      if (getObjectType() != ScriptObject::ActivationE)
      {
         activationObject->putProperty(ThisC, this);
      }
      theContext->addObjectToScopeChain(activationObject);

      ScriptValue result(obj->call(theContext, theArguments));

      theContext->removeObjectFromScopeChain();

      return result;      
   }
   else
   {
      // Function is used as a constructor.
      // Create a new object
      ScriptObject* newObject = new ScriptObject(ScriptObject::ObjectE);
      ScriptValue result(newObject);
      newObject->putProperty(
         PrototypeC,
         ScriptValue(obj),
         ScriptProperty::DontEnumE | 
         ScriptProperty::DontDeleteE | 
         ScriptProperty::ReadOnlyE);

      activationObject->putProperty(ThisC, newObject);

      theContext->addObjectToScopeChain(activationObject);

      obj->call(theContext, theArguments);

      theContext->removeObjectFromScopeChain();

      return result;      
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObject::call(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return ScriptValue(ScriptValue::UndefinedE);
}

// ----------------------------------------------------------------------------

bool
ScriptObject::deleteProperty(
   const std::string& theName)
{
   // ECMA-262 8.6.2.5
   ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      if (property->dontDelete() == true)
      {
         // Property must not be deleted
         return false;
      } 
      else
      {
         // Property may be be deleted
         propertiesM.erase(theName);
         return true;
      }
   }
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObject::deletePropertyNoCheck(
   const std::string& theName)
{
   // ECMA-262 8.6.2.5
   ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property may be be deleted, ignore possible "don't delete" property.
      propertiesM.erase(theName);
      return true;
   }
   return false;
}

// ----------------------------------------------------------------------------

void
ScriptObject::flushAllProperties()
{
   Properties::iterator next;
   for (Properties::iterator iter = propertiesM.begin();
        iter != propertiesM.end();
        iter = next)
   {
      next = iter;
      next++;
      if ((*iter).second.getValue().getDataType() == ScriptValue::ObjectE)
      {
         (*iter).second.getValue().toObject()->flushAllProperties();
      }
      propertiesM.erase(iter);
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObject::getBooleanValue() const
{
   // See ECMA-262 9.2
   return true;
}

// ----------------------------------------------------------------------------

const char*
ScriptObject::getClass() const
{
   return ClassNameC;
}

// ----------------------------------------------------------------------------

bool
ScriptObject::getNameOfFirstProperty(
   std::string& theName) const
{
   for (Properties::const_iterator iter = propertiesM.begin();
        iter != propertiesM.end();
        iter++)
   {
      if ((*iter).second.dontEnum() == false)
      {
         theName = (*iter).first;
         return true;
      }
   }
   return false;  
}

// ----------------------------------------------------------------------------

bool
ScriptObject::getNameOfNextProperty(
   std::string& theName) const
{
   Properties::const_iterator iter = propertiesM.upper_bound(theName);
   while (iter != propertiesM.end())
   {
      if ((*iter).second.dontEnum() == false)
      {
         theName = (*iter).first;
         return true;
      }
      iter++;
   }
   return false;  
}

// ----------------------------------------------------------------------------

Number
ScriptObject::getNumberValue() const
{
   // See ECMA-262 9.3
   return propertiesM.size();
}

// ----------------------------------------------------------------------------

ScriptObject::ObjectType
ScriptObject::getObjectType() const
{
   return objectTypeM;
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObject::getProperty(
   const std::string& theName) const
{
   // ECMA-262 8.6.2.1
   const ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      return property->getValue();   
   }
   // Property does not exist - check if it exists in prototype
   const ScriptObject* prototype = getPrototypeObject();
   if (prototype == 0)
   {
      // No prototype
      return ScriptValue(ScriptValue::UndefinedE);
   } 
   // Check if the value exists in the prototype
   return prototype->getProperty(theName);
}

// ----------------------------------------------------------------------------

void
ScriptObject::getPropertyNames(
   std::vector<std::string>& theNames) const
{
   for (Properties::const_iterator iter = propertiesM.begin();
        iter != propertiesM.end();
        iter++)
   {
      std::string name = (*iter).first;
      theNames.push_back(name);
   }
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObject::getStringValue() const
{
   // See ECMA-262 9.8
   ScriptString* result = new ScriptString;
   std::string propertyName;
   if (getNameOfFirstProperty(propertyName) == false)
   {
      // There are no properties
      return ScriptValue(result);
   }

   bool isFirstProperty = true;
   do
   {
      if (isFirstProperty == false)
      {
         result->append("; ");
      }
      else
      {
         isFirstProperty = false;
      }
      result->append("\"");
      result->append(propertyName);
      result->append("\" = ");
      ScriptValue value = getProperty(propertyName).getValue();
      switch (value.getDataType())
      {
         case ScriptValue::ObjectE:
         {
            result->append("{ ");
            result->append(value.toString());
            result->append(" }");
            break;
         }
         case ScriptValue::StringE:
         {
            result->append("\"");
            result->append(value.toString());
            result->append("\"");
            break;
         }
         default:
         {
            result->append(value.toString());
            break;
         }
      }
   }
   while (getNameOfNextProperty(propertyName) == true);
   return ScriptValue(result);
}

// ----------------------------------------------------------------------------

bool
ScriptObject::hasProperty(
   const std::string& theName) const
{
   // ECMA-262 8.6.2.4
   const ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      return true;   
   }

   // Property does not exist - check if it exists in prototype
   const ScriptObject* prototype = getPrototypeObject();
   if (prototype == 0)
   {
      // No prototype
      return false;
   } 
   // Check if the value exists in the prototype
   return prototype->hasProperty(theName);
}

// ----------------------------------------------------------------------------

bool
ScriptObject::isInstanceOf(
   const std::string& theClassName) const
{
   return (theClassName == ClassNameC);
}

// ----------------------------------------------------------------------------

void
ScriptObject::print(
   std::string&       theOutputString,
   const std::string& theLinePrefix) const
{
   for (Properties::const_iterator iter = propertiesM.begin();
        iter != propertiesM.end();
        iter++)
   {
      if ((*iter).second.dontEnum() == true)
      {
         continue;
      }
      if (iter != propertiesM.begin())
      {
         theOutputString += ";\n";
      }
      theOutputString += theLinePrefix;
      theOutputString += "\"";
      theOutputString += (*iter).first;
      theOutputString += "\" = ";
      ScriptValue value = (*iter).second.getValue();
      switch (value.getDataType())
      {
         case ScriptValue::ObjectE:
         {
            theOutputString += "\n";
            theOutputString += theLinePrefix;
            theOutputString += "{\n";
//            theOutputString += theLinePrefix;
            value.toObject()->print(theOutputString, theLinePrefix + "   ");
            theOutputString += "\n";
            theOutputString += theLinePrefix;
            theOutputString += "}";
            break;
         }
         case ScriptValue::StringE:
         {
            theOutputString += "\"";
            theOutputString += value.toString();
            theOutputString += "\"";
            break;
         }
         default:
         {
            theOutputString += value.toString();
            break;
         }
      }
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObject::putProperty(
   const std::string&      theName,
   const ScriptValue& theValue)
{
   // ECMA-262 8.6.2.2
   ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      return property->putValue(theValue);
   }

   // Property does not exist - check if it exists in prototype
   ScriptObject* prototype = getPrototypeObject();
   if (prototype != 0)
   {
      // Prototype exists
      switch (prototype->updateProperty(theName, theValue))
      {
         case UpdatedE:
         {
            // Property exists in the prototype and has been updated
            return true;
         }
         case UpdateNotAllowedE:
         {
            // It is not allowed to create or update this property.
            return false;
         }
         case NotUpdatedE:
         {
            // Property does not exist in the prototype, but it is
            // allowed to create it in this object
            break;
         }
      }
   } 

   // Create property
   propertiesM.insert(Properties::value_type(theName, 
                                             ScriptProperty(theValue)));
   return true;
}

// ----------------------------------------------------------------------------

void
ScriptObject::putProperty(
   const std::string&      theName,
   const ScriptValue& theValue,
   unsigned int       theAttributes)
{
   // ECMA-262 8.6.2.2
   ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      property->putValue(theValue, theAttributes);
   }

   // Create property
   propertiesM.insert(Properties::value_type(theName, 
                                             ScriptProperty(theValue, 
                                                            theAttributes)));
}

// ----------------------------------------------------------------------------

void
ScriptObject::read(
   const char*& theCurrentChar, 
   int&         theLine, 
   int&         theColumn)
{

   // Skip trailing spaces
   ScannerUtilities::skipStringWhiteSpace(theCurrentChar, theLine, theColumn);

   while (*theCurrentChar != 0 && *theCurrentChar != '}' && *theCurrentChar != ']')
   {
      // Parse property name
      std::string name = readPropertyName(theCurrentChar, theLine, theColumn);

      // Skip spaces
      ScannerUtilities::skipStringWhiteSpace(theCurrentChar, 
                                             theLine, 
                                             theColumn);

      // Skip mandatory separating '=' or ':'
      if (*theCurrentChar != '=' && *theCurrentChar != ':')
      {
         // There is some strange stuff; no '=' or ':'
         std::string error = "Found: '";
         error += *theCurrentChar;
         error += "', ':' or '=' expected.";
         error += " Property Name: ";
         error += name;
         throw ScannerException(error.c_str(), theLine, theColumn);
      }
      theCurrentChar++;
      theColumn++;

      // Skip spaces
      ScannerUtilities::skipStringWhiteSpace(theCurrentChar, 
                                             theLine, 
                                             theColumn);

      // Parse property value
      ScriptValue value = readPropertyValue(theCurrentChar, 
                                            theLine, 
                                            theColumn);

      if (putProperty(name, value) == false)
      {
         // Can't put that property.
         std::string message = "Can't put property '";
         message.append(name);
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
   }
}

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

std::string 
ScriptObject::readPropertyName(
   const char*& theCurrentChar, 
   int&         theLine, 
   int&         theColumn)
{
   std::string result;
   switch (*theCurrentChar)
   {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         // Property name is a number
         //result.appendNumber(ScannerUtilities::getNumber(theCurrentChar, 
         //                                                theLine, 
         //                                               theColumn));
         std::stringstream ss; 
         ss << std::setprecision(std::numeric_limits<Number>::digits10) 
            << ScannerUtilities::getNumber(theCurrentChar,
                                           theLine,
                                           theColumn);
         result.append(ss.str());
         break; 
      }
      case '"':
      case '\'':
      {
         // Property name is a string
         result = ScannerUtilities::getString(theCurrentChar, 
                                              theLine, 
                                              theColumn); 
         break;
      }
      default:
      {
         // Property name is an identifier
         result = ScannerUtilities::getIdentifier(theCurrentChar, 
                                                  theLine, 
                                                  theColumn);
         break;
      }
   }
   if (result.empty() == false)
   {
      return result;
   }

   // Property name missing
   throw ScannerException("Property name is empty", theLine, theColumn);
}

// ----------------------------------------------------------------------------

ScriptValue
ScriptObject::readPropertyValue(
   const char*& theCurrentChar, 
   int&         theLine, 
   int&         theColumn)
{
   switch (*theCurrentChar)
   {
      case '"':
      case '\'':
      {
         // Property value is a string
         ScriptValue string(ScriptValue::StringE);
         string.read(theCurrentChar, theLine, theColumn);
         return string; 
      }
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      {
         // Property value is a number
         ScriptValue number(ScriptValue::NumberE);
         number.read(theCurrentChar, theLine, theColumn);
         return number; 
      }
      case 'f': // 'false'
      case 't': // 'true'
      {
         // Property value is a boolean
         ScriptValue boolean(ScriptValue::BooleanE);
         boolean.read(theCurrentChar, theLine, theColumn);
         return boolean; 
      }
      case 'n': // 'null'
      {
         // Property value is undefined
         ScriptValue nullValue(ScriptValue::NullE);
         nullValue.read(theCurrentChar, theLine, theColumn);
         return nullValue;
      }
      case 'u': // 'undefined'
      {
         // Property value is undefined
         ScriptValue undefined(ScriptValue::UndefinedE);
         undefined.read(theCurrentChar, theLine, theColumn);
         return undefined; 
      }
      case '{':
      {
         // Property value is an object
         theCurrentChar++;
         theColumn++;
         ScriptValue object(ScriptValue::ObjectE);
         object.read(theCurrentChar, theLine, theColumn);
         if (*theCurrentChar != '}')
         {
            // '}' missing
            throw ScannerException("'}' expected", theLine, theColumn);
         }
         theCurrentChar++;
         theColumn++;
         return object; 
      }
      case '[':
      {
         // Property value is an Array
         theCurrentChar++;
         theColumn++;

         ScriptObjectArray* array = new ScriptObjectArray();
         array->read(theCurrentChar, theLine, theColumn);

         if (*theCurrentChar != ']')
         {
            // ']' missing
            throw ScannerException("']' expected", theLine, theColumn);
         }

         ScriptValue object(array);
         theCurrentChar++;
         theColumn++;
         return object;
      }
      default:
      {
         throw ScannerException("Property value invalid", theLine, theColumn);
      }
   }
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObject::updateProperty(
   const std::string& theName,
   ScriptValue   theValue)
{
   ScriptProperty* property = getPropertyObject(theName);
   if (property != 0)
   {
      // Property exists
      if (property->putValue(theValue) == true)
      {
         return UpdatedE;
      }
      else
      {
         return UpdateNotAllowedE;
      }
   }

   // Property does not exist - check if it exists in prototype
   ScriptObject* prototype = getPrototypeObject();
   if (prototype != 0)
   {
      // Prototype exists
      return prototype->updateProperty(theName, theValue);
   } 

   // Prototype does not exist
   return NotUpdatedE;
}
        
// ----------------------------------------------------------------------------

void
ScriptObject::putPropertyNoCheck(
   const std::string&      theName,
   const ScriptValue& theValue,
   unsigned int       theAttributes)
{
   std::pair<Properties::iterator, bool> result =
      propertiesM.insert(Properties::value_type(theName, 
                                                ScriptProperty(theValue, 
                                                               theAttributes)));
   if (result.second == false)
   {
      (*result.first).second = ScriptProperty(theValue, 
                                              theAttributes);
   }
}
        
// ----------------------------------------------------------------------------

void
ScriptObject::putPropertyNoCheck(
   const std::string&      theName,
   const ScriptValue& theValue)
{
   std::pair<Properties::iterator, bool> result =
      propertiesM.insert(Properties::value_type(theName, 
                                                ScriptProperty(theValue)));
   if (result.second == false)
   {
      (*result.first).second = theValue;
   }
}

// -------------------------------------------------------------------------
