#include "JsScriptObjectSystemCall.h"

using namespace Js;

typedef ScriptValue (ScriptObjectSystemCall::*GetMethod)() const;
typedef void (ScriptObjectSystemCall::*SetMethod)(ScriptValue theValue);
typedef void (ScriptObjectSystemCall::*DeleteMethod)();
typedef ScriptValue (ScriptObjectSystemCall::*CallMethod)(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                         theIsConstructor);

struct PropertyDescriptor
{
   const char*  name;
   GetMethod    getMethod;
   SetMethod    setMethod;
   DeleteMethod deleteMethod;
   CallMethod   callMethod;
};

// ----------------------------------------------------------------------------
// routine to compare two properties based on an
// alphabetical ordering of the name field
static
int
property_compare(
   const void* thePropertyName,
   const void* thePropertyTableEntry)
{
   return strcmp((const char*)thePropertyName,
                 ((const PropertyDescriptor*)thePropertyTableEntry)->name);
}

// *******************************************************************
// *** THE PROPERTY DESCRIPTORS MUST BE ALPHABETICALLY SORTED !!!! ***
// *******************************************************************

static
const PropertyDescriptor
PropertyDescriptorsC[] =
{
   // <<TODO>> Declare the methods, which implement the scriptable
   //          module.
   //           
   // Function template:
   //   { "[FunctionName]",
   //     0,
   //     0,
   //     0,
   //     &ScriptObjectSystemCall::scriptCall[FunctionName]
   //   },
   //           
   // Read/write property template:
   //   { "[PropertyName]",
   //     &ScriptObjectSystemCall::scriptGet[PropertyName],
   //     &ScriptObjectSystemCall::scriptSet[PropertyName],
   //     0,
   //     0
   //   },
   //           
   // Read-only property template:
   //   { "[PropertyName]",
   //     &ScriptObjectSystemCall::scriptGet[PropertyName],
   //     0,
   //     0,
   //     0
   //   },
   //   
   // Deletable template:
   //   { "[PropertyName]",
   //     &ScriptObjectSystemCall::scriptGet[PropertyName],
   //     &ScriptObjectSystemCall::scriptSet[PropertyName],
   //     &ScriptObjectSystemCall::scriptDelete[PropertyName],
   //     0
   //   },
   { "call",
     0,
     0,
     0,
     &ScriptObjectSystemCall::scriptCallCall
   },
   { "returnCode",
      &ScriptObjectSystemCall::scriptGetReturnCode,
      0,
      0,
      0
   }
};

static
const size_t NumberOfPropertiesC =
   sizeof(PropertyDescriptorsC) / sizeof(PropertyDescriptor);

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectSystemCall::ClassNameC = "SystemCall";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// ----------------------------------------------------------------------------

ScriptObjectSystemCall::ScriptObjectSystemCall()
:   returnCodeM(0)
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectSystemCall::~ScriptObjectSystemCall()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject*
ScriptObjectSystemCall::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   return new ScriptObjectSystemCall;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::callExternalMethod(
   const std::string&           theName,
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                         theIsConstructor,
   ScriptValue&            theResult)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // might exist in the internal object
      return false;
   }
   else
   {
      // This property exists in the external object
      if (propertyDescriptor->callMethod != 0)
      {
         // This proerty is callable.
         theResult = (this->*(propertyDescriptor->callMethod))(theContext,
                                                               theArguments,
                                                               theIsConstructor);
         return true;
      }
      else
      {
         // This property is not callable.
         return false;
      }
   }
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectSystemCall::deleteExternalProperty(
   const std::string& theName)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // might exist in the internal object
      return ScriptObject::NotUpdatedE;
   }
   else
   {
      // This property exists in the external object - delete it
      if (propertyDescriptor->deleteMethod != 0)
      {
         // It is allowed to delete this property.
         (this->*(propertyDescriptor->deleteMethod))();
         return ScriptObject::UpdatedE;
      }
      else
      {
         // This property is read-only; it is not allowed to delete it.
         return ScriptObject::UpdateNotAllowedE;
      }
   }
}

// ----------------------------------------------------------------------------

const char*
ScriptObjectSystemCall::getClass() const
{
   return ClassNameC;
}
      
// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::getExternalProperty(
   const std::string& theName,
   ScriptValue&  theValue) const
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // this property does not exist
      return false;
   }
   else
   {
      // this is a known property - return its value
      if (propertyDescriptor->getMethod != 0)
      {
         theValue = (this->*(propertyDescriptor->getMethod))();
      }
      else
      {
         theValue = ScriptValue(ScriptValue::UndefinedE);
      }
      return true;
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::getNameOfFirstExternalProperty(
   std::string& theName) const
{
   for (size_t i = 0; i < NumberOfPropertiesC; i++)
   {
      const PropertyDescriptor* propertyDescriptor = &PropertyDescriptorsC[i];
      if (propertyDescriptor->getMethod != 0 ||
          propertyDescriptor->setMethod != 0)
      {
         // Enumerate only properties, which have a get or set method
         theName = propertyDescriptor->name;
         return true;
      }
   }
   return false;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::getNameOfNextExternalProperty(
   std::string& theName) const
{
   while (true)
   {
      PropertyDescriptor* propertyDescriptor;
      propertyDescriptor = (PropertyDescriptor*)
         bfindnext(theName.c_str(),
                   PropertyDescriptorsC,
                   NumberOfPropertiesC,
                   sizeof(PropertyDescriptor),
                   property_compare);
      if (propertyDescriptor == 0)
      {
         // The next property does not exist
         return false;
      }
      else
      {
         // Found the next property
         theName = propertyDescriptor->name;
         if (propertyDescriptor->getMethod != 0 ||
             propertyDescriptor->setMethod != 0)
         {
            // Enumerate only properties, which have a get or set method
            return true;
         }
      }
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::hasExternalProperty(
   const std::string& theName) const
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      return false;
   }
   return true;
}

// ----------------------------------------------------------------------------

bool
ScriptObjectSystemCall::isInstanceOf(
   const std::string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectExternal::isInstanceOf(theClassName);
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectSystemCall::setExternalProperty(
   const std::string& theName,
   ScriptValue   theValue)
{
   PropertyDescriptor* propertyDescriptor;
   propertyDescriptor = (PropertyDescriptor*)
                bsearch(theName.c_str(),
                        PropertyDescriptorsC,
                        NumberOfPropertiesC,
                        sizeof(PropertyDescriptor),
                        property_compare);
   if (propertyDescriptor == 0)
   {
      // This property does not exist in the external object, but it
      // is OK to create it in the internal object
      return ScriptObject::NotUpdatedE;
   }
   else
   {
      // This property exists in the external object - set its value
      if (propertyDescriptor->setMethod != 0)
      {
         // It is allowed to set this property.
         (this->*(propertyDescriptor->setMethod))(theValue);
         return ScriptObject::UpdatedE;
      }
      else
      {
         // This property is read-only; it is not allowed to set it.
         return ScriptObject::UpdateNotAllowedE;
      }
   }
}

// ----------------------------------------------------------------------------
// SCRIPT OBJECT METHOD CALL IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which implement the scriptable 
//          functions of the class here.
//          Sort the function names alphabetically.
//
// Template:
//    ScriptValue
//    ScriptObjectSystemCall::scriptCall[FunctionName](
//       ScriptExecutionContext* theContext,
//       const ScriptValueArray& theArguments,
//       bool                         theIsConstructor)
//   { 
//      return ScriptValue([function result]);
//   }

ScriptValue
ScriptObjectSystemCall::scriptCallCall(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor)
{ 
   if (theArguments.size() != 1)
   {
      throw ScriptTypeError(
         "SystemCall.call():"
         " Function requires one argument (command).");
   }
   
   std::string command = theArguments[0].getValue().toString();
   
   FILE* fd = popen(command.c_str(), "r");
   if (fd == 0)
   {
      std::string errorMessage("SystemCall.call(): Can't execute command '");
      errorMessage.append("'");
      errorMessage.append(command);
      errorMessage.append("'. popen() failed (errno=");
      {
         char buf[10] = {0};
         snprintf(buf, 10, "%d", errno);
         //errorMessage.appendNumber(errno);
         errorMessage.append(buf);
      }
      errorMessage.append(".");
      
      throw ScriptTypeError(errorMessage);
   }
   
   ScriptStringPtr result = new ScriptString;
   
   char resultBuffer[1024*64];

   // Read pipe until end of file.
   while (!feof(fd))
   {
      size_t numberOfBytesRead = 
         fread(resultBuffer, 1, sizeof(resultBuffer), fd);
      if (numberOfBytesRead > 0)   
      {
         result->append(resultBuffer, numberOfBytesRead);
      }
      else
      {
         break;
      }
   }

   returnCodeM = pclose(fd);
   if (((unsigned int)returnCodeM % 256) == 0)
   {
      returnCodeM = (unsigned int)returnCodeM >> 8;
   }

   return ScriptValue(result);
}

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY DELETE IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to delete  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    void
//    ScriptObjectSystemCall::scriptDelete[PropertyName]()
//    {
//       ...
//    }

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY GET IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to get the values of  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    ScriptValue
//    ScriptObjectSystemCall::scriptGet[PropertyName]() const 
//    {
//       return ScriptValue([property value]);
//    }
ScriptValue
ScriptObjectSystemCall::scriptGetReturnCode() const 
{
   return ScriptValue(returnCodeM);
}

// ----------------------------------------------------------------------------
// SCRIPT OBJECT PROPERTY SET IMPLEMENTATIONS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the methods, which are used to set the values of  
//          properties here.
//          Sort the property names alphabetically.
//
// Template:
//    void
//    ScriptObjectSystemCall::scriptSet[property value](
//       ScriptValue theValue)
//    {
//       ...;
//    }

// ----------------------------------------------------------------------------
// C++ INTERFACE:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the long methods (>= 10 lines of code) of your 
//          public C++ interface here (inline).
//          Sort the method names alphabetically.

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the private methods of your class here.
//          Sort the method names alphabetically.
