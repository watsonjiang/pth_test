#include "JsScriptObjectMath.h"
#include "JsScriptTypeError.h"

using namespace Js;

typedef ScriptValue (ScriptObjectMath::*GetMethod)() const;
typedef void (ScriptObjectMath::*SetMethod)(ScriptValue theValue);
typedef void (ScriptObjectMath::*DeleteMethod)();
typedef ScriptValue (ScriptObjectMath::*CallMethod)(
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
   //     &ScriptObjectMath::scriptCall[FunctionName]
   //   },
   //           
   // Read/write property template:
   //   { "[PropertyName]",
   //     &ScriptObjectMath::scriptGet[PropertyName],
   //     &ScriptObjectMath::scriptSet[PropertyName],
   //     0,
   //     0
   //   },
   //           
   // Read-only property template:
   //   { "[PropertyName]",
   //     &ScriptObjectMath::scriptGet[PropertyName],
   //     0,
   //     0,
   //     0
   //   },
   //   
   // Deletable template:
   //   { "[PropertyName]",
   //     &ScriptObjectMath::scriptGet[PropertyName],
   //     &ScriptObjectMath::scriptSet[PropertyName],
   //     &ScriptObjectMath::scriptDelete[PropertyName],
   //     0
   //   },
   { "E",
     &ScriptObjectMath::scriptGetE,
     0,
     0,
     0
   },
   { "LN10",
     &ScriptObjectMath::scriptGetLn10,
     0,
     0,
     0
   },
   { "LN2",
     &ScriptObjectMath::scriptGetLn2,
     0,
     0,
     0
   },
   { "LOG10E",
     &ScriptObjectMath::scriptGetLog10E,
     0,
     0,
     0
   },
   { "LOG2E",
     &ScriptObjectMath::scriptGetLog2E,
     0,
     0,
     0
   },
   { "PI",
     &ScriptObjectMath::scriptGetPi,
     0,
     0,
     0
   },
   { "SQRT1_2",
     &ScriptObjectMath::scriptGetSqrt1_2,
     0,
     0,
     0
   },
   { "SQRT2",
     &ScriptObjectMath::scriptGetSqrt2,
     0,
     0,
     0
   },
   { "abs",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallAbs 
   },
   { "acos",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallAcos 
   },
   { "asin",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallAsin 
   },
   { "atan",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallAtan 
   },
   { "ceil",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallCeil 
   },
   { "cos",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallCos 
   },
   { "exp",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallExp
   },
   { "floor",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallFloor
   },
   { "log",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallLog
   },
   { "max",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallMax
   },
   { "min",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallMin
   },
   { "pow",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallPow
   },
   { "random",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallRandom
   },
   { "round",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallRound
   },
   { "sin",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallSin
   },
   { "tan",
     0,
     0,
     0,
     &ScriptObjectMath::scriptCallTan
   }
};

static
const size_t NumberOfPropertiesC =
   sizeof(PropertyDescriptorsC) / sizeof(PropertyDescriptor);

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectMath::ClassNameC = "Math";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// -----------------------------------o-----------------------------------------

ScriptObjectMath::ScriptObjectMath()
{
}

// ----------------------------------------------------------------------------

ScriptObjectMath::~ScriptObjectMath()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject*
ScriptObjectMath::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   //ECMA262 15.8
   //The Math object does not have a [[Construct]] internal property; 
   //it is not possible to use the Math object as a constructor with the new operator.
   throw ScriptTypeError(
         "Math is not a function.");
}

// ----------------------------------------------------------------------------

bool
ScriptObjectMath::callExternalMethod(
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
ScriptObjectMath::deleteExternalProperty(
   const string& theName)
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
ScriptObjectMath::getClass() const
{
   return ClassNameC;
}
      
// ----------------------------------------------------------------------------

bool
ScriptObjectMath::getExternalProperty(
   const string& theName,
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
ScriptObjectMath::getNameOfFirstExternalProperty(
   string& theName) const
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
ScriptObjectMath::getNameOfNextExternalProperty(
   string& theName) const
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

ScriptValue
ScriptObjectMath::getStringValue() const
{
   return ScriptValue(ScriptValue::UndefinedE);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectMath::hasExternalProperty(
   const string& theName) const
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
ScriptObjectMath::isInstanceOf(
   const string& theClassName) const
{
   if (ClassNameC == theClassName)
   {
      return true;
   }
   return ScriptObjectExternal::isInstanceOf(theClassName);
}

// ----------------------------------------------------------------------------

ScriptObject::UpdateResult
ScriptObjectMath::setExternalProperty(
   const string& theName,
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
//    ScriptObjectMath::scriptCall[FunctionName](
//       ScriptExecutionContext* theContext,
//       const ScriptValueArray& theArguments,
//       bool                         theIsConstructor)
//   { 
//      return ScriptValue([function result]);
//   }

// ----------------------------------------------------------------------------

ScriptValue
ScriptObjectMath::scriptCallAbs(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   if(theArguments.size() == 0)
   {
      throw ScriptTypeError(
         "Math.abs:"
         " One argument expected.");
   }
   if(theArguments.size() > 1)
   {
      throw ScriptTypeError(
         "Math.abs:"
         " Only one argument expected.");
   }
   if(!theArguments[0].getValue().isNumber())
   {
      return ScriptValue(NaNC); 
   }
   Number x = theArguments[0].getValue().toNumber();
   if(x == NaNC)
   {
      return ScriptValue(NaNC);
   }
   if(x < 0)
   {
      return ScriptValue(-x); 
   }  
  
   return ScriptValue(x);
}

ScriptValue
ScriptObjectMath::scriptCallAcos(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallAsin(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallAtan(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallAtan2(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallCeil(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallCos(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallExp(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallFloor(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallLog(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallMax(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallMin(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallPow(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallRandom(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallRound(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallSin(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallSqrt(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
}

ScriptValue
ScriptObjectMath::scriptCallTan(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
   return ScriptValue(ScriptValue::UndefinedC);
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
//    ScriptObjectMath::scriptDelete[PropertyName]()
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
//    ScriptObjectMath::scriptGet[PropertyName]() const 
//    {
//       return ScriptValue([property value]);
//    }

ScriptValue
ScriptObjectMath::scriptGetE() const 
{
   return ScriptValue(2.7182818284590452354);
}

ScriptValue
ScriptObjectMath::scriptGetLn10() const
{
   return ScriptValue(2.302585092994046);
}

ScriptValue
ScriptObjectMath::scriptGetLn2() const
{
   return ScriptValue(0.6931471805599453);
}

ScriptValue
ScriptObjectMath::scriptGetLog2E() const
{
   return ScriptValue(1.4426950408889634);
}

ScriptValue
ScriptObjectMath::scriptGetLog10E() const
{
   return ScriptValue(0.4342944819032518);
}

ScriptValue
ScriptObjectMath::scriptGetPi() const
{
   return ScriptValue(3.1415926535897932);
}

ScriptValue
ScriptObjectMath::scriptGetSqrt1_2() const
{
   return ScriptValue(0.7071067811865476);
}

ScriptValue
ScriptObjectMath::scriptGetSqrt2() const
{
   return ScriptValue(1.4142135623730951);
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
//    ScriptObjectMath::scriptSet[property value](
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
