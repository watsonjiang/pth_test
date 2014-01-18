#include "JsScriptValueReader.h"
#include "JsScriptObjectArray.h"

using namespace Js;


// ---------------------------------------------------------------------

ScriptValueReader::ScriptValueReader(
		Js::ScriptValue theValue)
: valueM(theValue)
{
	// Empty
}

// ---------------------------------------------------------------------

ScriptValueReader::ScriptValueReader(
		const ScriptValueReader& theOther)
: valueM(theOther.valueM),
  propertyNamesM(theOther.propertyNamesM)
{
	// Empty
}

// ---------------------------------------------------------------------

ScriptValueReader&
ScriptValueReader::operator=(
		const ScriptValueReader& theOther)
{
   valueM = theOther.valueM;
   propertyNamesM = theOther.propertyNamesM;
   return *this;
}

// ---------------------------------------------------------------------

bool
ScriptValueReader::arrayElementIsString(
      unsigned index) const
{
   bool lengthValid;
   if(index < getLength(lengthValid) && lengthValid)
   {
      const Js::ScriptObject* theObject = valueM.toObject();
      const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
      Js::ScriptValue element = theArray->getProperty(index);
      return element.isString();
   }
   return false;
}

// ---------------------------------------------------------------------

ScriptValueReader
ScriptValueReader::getArray(
        const std::string& attributeName,
        bool&              isValid) const
{
   Js::ScriptValue property;
   if (valueM.isObject())
   {
      const Js::ScriptObject* theObject = valueM.toObject();
      property = theObject->getProperty(attributeName);
      if (property.isObject() )
      {
         theObject = property.toObject();
         if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
         {
            isValid = true;
            return ScriptValueReader(property);
         }
      }
   }
   isValid = false;
   return ScriptValueReader(property);
}

// ---------------------------------------------------------------------

ScriptValueReader
ScriptValueReader::getArray(
        unsigned index,
        bool&    isValid) const
{
    bool lengthValid;
    if(index < getLength(lengthValid) && lengthValid)
    {
        isValid = true;
        const Js::ScriptObject* theObject = valueM.toObject();
        const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
        Js::ScriptValue element = theArray->getProperty(index);
        theObject = element.toObject();
        if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
        {
           isValid = true;
           return ScriptValueReader(element);
        }
    }
    isValid = false;
    return ScriptValueReader(Js::ScriptValue::UndefinedC);
}

// ---------------------------------------------------------------------

bool
ScriptValueReader::getBoolean(
		const std::string& attributeName,
		bool&              isValid,
		bool               defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		Js::ScriptValue property = theObject->getProperty(attributeName);
		if (property.isBoolean())
		{
			isValid = true;
			return property.toBoolean();
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

bool
ScriptValueReader::getBoolean(
		unsigned index,
		bool&    isValid,
		bool     defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
		{
			const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
			if (index < theArray->getLength())
			{
				Js::ScriptValue element = theArray->getProperty(index);
				if (element.isBoolean())
				{
					isValid = true;
					return element.toBoolean();
				}
			}
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

unsigned
ScriptValueReader::getLength(
		bool& isValid) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
		{
			const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
			isValid = true;
			return theArray->getLength();
		}
	}
	isValid = false;
	return 0;
}

// ---------------------------------------------------------------------

Js::Number
ScriptValueReader::getNumber(
		const std::string& attributeName,
		bool&              isValid,
		Js::Number        defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		Js::ScriptValue property = theObject->getProperty(attributeName);
		if (property.isNumber())
		{
			isValid = true;
			return property.toNumber();
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

Js::Number
ScriptValueReader::getNumber(
		unsigned    index,
		bool&       isValid,
		Js::Number defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
		{
			const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
			if (index < theArray->getLength())
			{
				Js::ScriptValue element = theArray->getProperty(index);
				if (element.isNumber())
				{
					isValid = true;
					return element.toNumber();
				}
			}
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

ScriptValueReader
ScriptValueReader::getObject(
		const std::string& attributeName,
		bool&              isValid) const
{
   Js::ScriptValue property;
   if (valueM.isObject())
   {
      const Js::ScriptObject* theObject = valueM.toObject();
      property = theObject->getProperty(attributeName);
      if (property.isObject() )
      {
         theObject = property.toObject();
         if (theObject->getObjectType() != Js::ScriptObject::ArrayE)
         {
            isValid = true;
            return ScriptValueReader(property);
         }
      }
   }
   isValid = false;
   return ScriptValueReader(property);
}

// ---------------------------------------------------------------------

ScriptValueReader
ScriptValueReader::getObject(
		unsigned index,
		bool&    isValid) const
{
	bool lengthValid;
	if(index < getLength(lengthValid) && lengthValid)
	{
		isValid = true;
	    const Js::ScriptObject* theObject = valueM.toObject();
	    const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
	    Js::ScriptValue element = theArray->getProperty(index);
	    theObject = element.toObject();
	    if (theObject->getObjectType() != Js::ScriptObject::ArrayE)
	    {
	       isValid = true;
	       return ScriptValueReader(element);
	    }
	}
	isValid = false;
	return ScriptValueReader(Js::ScriptValue::UndefinedC);
}

// ---------------------------------------------------------------------

std::string
ScriptValueReader::getPropertyName(
		unsigned index,
		bool& isValid) const
{
	if (index < propertyNamesM.size())
	{
		isValid = true;
		return propertyNamesM[index];
	}
	isValid = false;
	return "";
}

// ---------------------------------------------------------------------

unsigned
ScriptValueReader::getPropertyCount(
		bool& isValid) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		theObject->getPropertyNames(propertyNamesM);
		isValid = true;
		return propertyNamesM.size();
	}
	isValid = false;
	return 0;
}

// ---------------------------------------------------------------------

std::string
ScriptValueReader::getString(
		const std::string& attributeName,
		bool&              isValid,
		std::string        defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		Js::ScriptValue property = theObject->getProperty(attributeName);
		if (property.isString())
		{
			isValid = true;
			return property.toString();
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

std::string
ScriptValueReader::getString(
		unsigned    index,
		bool&       isValid,
		std::string defaultValue) const
{
	if (valueM.isObject())
	{
		const Js::ScriptObject* theObject = valueM.toObject();
		if (theObject->getObjectType() == Js::ScriptObject::ArrayE)
		{
			const Js::ScriptObjectArray* theArray = (Js::ScriptObjectArray*) theObject;
			if (index < theArray->getLength())
			{
				Js::ScriptValue element = theArray->getProperty(index);
				if (element.isString())
				{
					isValid = true;
					return element.toString();
				}
			}
		}
	}
	isValid = false;
	return defaultValue;
}

// ---------------------------------------------------------------------

bool
ScriptValueReader::hasProperty(
      const std::string& thePropertyName) const
{
   if (valueM.isObject())
   {
      const Js::ScriptObject* object = valueM.toObject();
      return object->hasProperty(thePropertyName);
   }
   return false;
}
