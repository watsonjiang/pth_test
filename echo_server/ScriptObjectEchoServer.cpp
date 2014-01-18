#include "ScriptObjectEchoServer.h"
#include "PalScriptTypeError.h"
#include "pth.h"
#include "sys/socket.h"
#include <netinet/in.h>
#include <netdb.h>
#include "PalScriptRuntime.h"
using namespace Pal;

typedef ScriptValue (ScriptObjectEchoServer::*GetMethod)() const;
typedef void (ScriptObjectEchoServer::*SetMethod)(ScriptValue theValue);
typedef void (ScriptObjectEchoServer::*DeleteMethod)();
typedef ScriptValue (ScriptObjectEchoServer::*CallMethod)(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments,
   bool                    theIsConstructor);

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
   //     &ScriptObjectEchoServer::scriptCall[FunctionName]
   //   },
   //           
   // Read/write property template:
   //   { "[PropertyName]",
   //     &ScriptObjectEchoServer::scriptGet[PropertyName],
   //     &ScriptObjectEchoServer::scriptSet[PropertyName],
   //     0,
   //     0
   //   },
   //           
   // Read-only property template:
   //   { "[PropertyName]",
   //     &ScriptObjectEchoServer::scriptGet[PropertyName],
   //     0,
   //     0,
   //     0
   //   },
   //   
   // Deletable template:
   //   { "[PropertyName]",
   //     &ScriptObjectEchoServer::scriptGet[PropertyName],
   //     &ScriptObjectEchoServer::scriptSet[PropertyName],
   //     &ScriptObjectEchoServer::scriptDelete[PropertyName],
   //     0
   //   },
   { "handlerScript",
     &ScriptObjectEchoServer::scriptGetHandlerScript,
     &ScriptObjectEchoServer::scriptSetHandlerScript,
     0,
     0
   },
   { "listen",
     0,
     0,
     0,
     &ScriptObjectEchoServer::scriptCallListen
   }
};

static
const size_t NumberOfPropertiesC =
   sizeof(PropertyDescriptorsC) / sizeof(PropertyDescriptor);

// ----------------------------------------------------------------------------
// THE NAME OF THIS CLASS:
// ----------------------------------------------------------------------------

const char* 
ScriptObjectEchoServer::ClassNameC = "EchoServer";

// ----------------------------------------------------------------------------
// CONSTRUCTORS AND DESTRUCTORS:
// -----------------------------------o-----------------------------------------

ScriptObjectEchoServer::ScriptObjectEchoServer()
{
   // Empty
}

// ----------------------------------------------------------------------------

ScriptObjectEchoServer::~ScriptObjectEchoServer()
{
   // Empty
}

// ----------------------------------------------------------------------------
// THE FOLLOWING METHODS ARE CALLED BY THE SCRIPT ENGINE:
// ----------------------------------------------------------------------------

ScriptObject*
ScriptObjectEchoServer::create(
   ScriptExecutionContext* theContext,
   const ScriptValueArray& theArguments)
{
   if (theArguments.size() == 0)
   {
      return new ScriptObjectEchoServer;   
   } 

   if (theArguments.size() != 0)
   {
      throw ScriptTypeError(
         "EchoServer():"
         " Constructor has no argument. "
         );
   }
}

// ----------------------------------------------------------------------------

bool
ScriptObjectEchoServer::callExternalMethod(
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
ScriptObjectEchoServer::deleteExternalProperty(
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
ScriptObjectEchoServer::getClass() const
{
   return ClassNameC;
}
      
// ----------------------------------------------------------------------------

bool
ScriptObjectEchoServer::getExternalProperty(
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
ScriptObjectEchoServer::getNameOfFirstExternalProperty(
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
ScriptObjectEchoServer::getNameOfNextExternalProperty(
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
ScriptObjectEchoServer::getStringValue() const
{
   return ScriptValue(ScriptValue::UndefinedE);
}

// ----------------------------------------------------------------------------

bool
ScriptObjectEchoServer::hasExternalProperty(
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
ScriptObjectEchoServer::isInstanceOf(
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
ScriptObjectEchoServer::setExternalProperty(
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
//    ScriptObjectEchoServer::scriptCall[FunctionName](
//       ScriptExecutionContext* theContext,
//       const ScriptValueArray& theArguments,
//       bool                         theIsConstructor)
//   { 
//      return ScriptValue([function result]);
//   }

// ----------------------------------------------------------------------------

static
void*
echo_handler_func(void *arg);

ScriptValue
ScriptObjectEchoServer::scriptCallListen(
         ScriptExecutionContext* theContext,
         const ScriptValueArray& theArguments,
         bool                    theIsConstructor)
{
    int port = (int)theArguments[0].getValue().toNumber(); 
    string addr = theArguments[1].getValue().toString();

    pth_attr_t attr;
    struct sockaddr_in sar;
    struct protoent *pe;
    struct sockaddr_in peer_addr;
    socklen_t peer_len;
    int sa, sw;
     

    pth_init();
    signal(SIGPIPE, SIG_IGN);

    attr = pth_attr_new();
    pth_attr_set(attr, PTH_ATTR_NAME, "echo_handler");
    pth_attr_set(attr, PTH_ATTR_STACK_SIZE, 64 * 1024);
    pth_attr_set(attr, PTH_ATTR_JOINABLE, FALSE);
    
    pe = getprotobyname("tcp");
    sa = socket(AF_INET, SOCK_STREAM, pe->p_proto);
    sar.sin_family = AF_INET;
    sar.sin_addr.s_addr = INADDR_ANY;
    sar.sin_port = htons(port);
    bind(sa, (struct sockaddr *)&sar, sizeof(struct sockaddr_in));
    listen(sa, 10);

    for(;;) {
       peer_len = sizeof(peer_addr);
       sw = pth_accept(sa, (struct sockaddr *)&peer_addr, &peer_len);
       pth_spawn(attr, echo_handler_func, (void*)handlerScript.c_str());
    }

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
//    ScriptObjectEchoServer::scriptDelete[PropertyName]()
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
//    ScriptObjectEchoServer::scriptGet[PropertyName]() const 
//    {
//       return ScriptValue([property value]);
//    }

ScriptValue
ScriptObjectEchoServer::scriptGetHandlerScript() const 
{
   return ScriptValue(handlerScript.c_str());
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
//    ScriptObjectEchoServer::scriptSet[property value](
//       ScriptValue theValue)
//    {
//       ...;
//    }
void
ScriptObjectEchoServer::scriptSetHandlerScript(
   ScriptValue theValue)
{
   handlerScript = theValue.toString();
}


// ----------------------------------------------------------------------------
// C++ INTERFACE:
// ----------------------------------------------------------------------------

// <<TODO>> Implement the long methods (>= 10 lines of code) of your 
//          public C++ interface here (inline).
//          Sort the method names alphabetically.

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------
static
void*
echo_handler_func(void *arg)
{
  const char * scriptfile = (const char *)arg;

  ScriptRuntime *rt = new ScriptRuntime();
  std::string error;
  Script* script = rt->getScript(scriptfile, error);
  if(NULL == script){
     printf("%s\n", error.c_str());
     return (void*)NULL;
  }
  ScriptExecutionContext* ctx = rt->newContext();
  int rs = rt->runScript(ctx, script, error);
  if(rs != 0){
     printf("%s\n", error.c_str());
  }
  delete ctx;
  delete rt;
}

