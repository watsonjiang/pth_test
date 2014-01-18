#include "JsScriptRuntime.h"
#include "JsScript.h"
#include "JsScriptObjectArray.h"
#include "JsScriptObjectDate.h"
#include "JsScriptObjectMath.h"
#include "JsScriptObjectError.h"
#include "JsScriptObjectReferenceError.h"
#include "JsScriptObjectSystemCall.h"
#include "JsScriptObjectTypeError.h"
#include "JsScriptObjectUriError.h"

using namespace Js;


// ----------------------------------------------------------------------------

bool
ScriptRuntime::addConstant(
   const std::string& theName,         
   ScriptValue   theValue)
{
   return predefinedNameTableM.addConstant(theName, 
                                           theValue);
}

// ----------------------------------------------------------------------------

bool
ScriptRuntime::addConstructorFct(
   const std::string&        theName,         
   ScriptConstructorFct theFct,
   int                  theNumberOfAttributes)
{
   return predefinedNameTableM.addConstructorFct(theName, 
                                                 theFct, 
                                                 theNumberOfAttributes);
}

// ----------------------------------------------------------------------------

Script*
ScriptRuntime::getScript(
   const std::string& theFileName,
   std::string&       theErrorMessage)
{
   Script* script;
   try
   {
      script = new Script(theFileName, predefinedNameTableM);
   }
   catch (ScannerException& e)
   {
      makeErrorMessage(theErrorMessage, theFileName, e);
      return 0;
   }
   catch (std::exception& e)
   {
      makeErrorMessage(theErrorMessage, theFileName, e);
      return 0;
   }
   return script;
}

//-----------------------------------------------------------------------------

void
ScriptRuntime::releaseScript(
   Script* script)
{
   delete script;
}

// ----------------------------------------------------------------------------

ScriptExecutionContext*
ScriptRuntime::newContext()
{
   return new ScriptExecutionContext(this);
}

// ----------------------------------------------------------------------------

void
ScriptRuntime::releaseContext(ScriptExecutionContext* ctx)
{
   delete ctx;
}

// ----------------------------------------------------------------------------

int
ScriptRuntime::runScript(ScriptExecutionContext* ctx,
                         Script*                 script,
                         std::string&            theErrorMessage)
{
   try{
      script->execute(ctx);
   }
   catch (ScannerException& e)
   {
      makeErrorMessage(theErrorMessage, script->getFilename(), e);
      return -1;
   }
   catch (std::exception& e)
   {
      makeErrorMessage(theErrorMessage, script->getFilename(), e);
      return -1;
   }
   return 0; 
}

// ----------------------------------------------------------------------------

void
ScriptRuntime::print(
   const std::string& theText)
{
   printf("%s\n", theText.c_str());
}

// ----------------------------------------------------------------------------

void
ScriptRuntime::removeConstructorFct(
   const std::string& theName)
{
   predefinedNameTableM.removeConstructorFct(theName);
}

ScriptRuntime::ScriptRuntime()
{
   predefinedNameTableM.addConstructorFct(
      ScriptObjectArray::ClassNameC, 
      ScriptObjectArray::create, 
      -1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectDate::ClassNameC, 
      ScriptObjectDate::create, 
      -1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectMath::ClassNameC, 
      ScriptObjectMath::create, 
      -1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectError::ClassNameC, 
      ScriptObjectError::create, 
      1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectSystemCall::ClassNameC,
      ScriptObjectSystemCall::create,
      0);
   predefinedNameTableM.addConstructorFct(
      ScriptObject::ClassNameC, 
      ScriptObject::create, 
      0);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectReferenceError::ClassNameC, 
      ScriptObjectReferenceError::create, 
      1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectTypeError::ClassNameC, 
      ScriptObjectTypeError::create, 
      1);
   predefinedNameTableM.addConstructorFct(
      ScriptObjectUriError::ClassNameC, 
      ScriptObjectUriError::create, 
      1);
}

// ----------------------------------------------------------------------------

ScriptRuntime::~ScriptRuntime()
{
}

// ----------------------------------------------------------------------------
// PRIVATE METHODS:
// ----------------------------------------------------------------------------


void
ScriptRuntime::makeErrorMessage(
   std::string&          theErrorMessage,
   const std::string&    theFilename,
   const std::exception& theException)
{
   theErrorMessage = "Failed to load script '";
   theErrorMessage += theFilename;
   theErrorMessage += "': ";
   theErrorMessage += theException.what();
}

// ----------------------------------------------------------------------------

void
ScriptRuntime::makeErrorMessage(
   std::string&                 theErrorMessage,
   const std::string&           theFilename,
   const ScannerException& theException)
{
   theErrorMessage = "Failed to compile script '";
   theErrorMessage += theFilename;
   theErrorMessage += "': ";
   theErrorMessage += theException.what();
   theErrorMessage += "\nIn line ";
   char buf[10] = {0};
   snprintf(buf, 10, "%d", theException.getLine());
   theErrorMessage.append(buf);
   theErrorMessage += ", column ";
   snprintf(buf, 10, "%d", theException.getColumn());
   theErrorMessage.append(buf);
   const ScriptString* file = theException.getFile();
   if (file != 0)
   {
      theErrorMessage += " of file '";
      theErrorMessage += *file;
      theErrorMessage += "'";
   }
   theErrorMessage += ".";
}

