#include "JsScript.h"
#include "JsScriptObjectActivation.h"
#include "JsScriptParser.h"

static const std::string InitC("<init>");

using namespace Js;

const unsigned int ScriptCheckIntervalC = 10;

Script::Script(
   const std::string&                    theFilename,
   const ScriptPredefinedNameTable& thePredefinedNameTable)
:  codeM(0),
   filenameM(theFilename),
   nextFileCheckDateM(Time::getUtcTime() + ScriptCheckIntervalC)
{
   ScriptParser parser(thePredefinedNameTable);
   codeM = parser.parseFile(theFilename);
   includedFilesM = parser.getIncludedFiles();
   fileModificationTimeM = parser.getFileModificationTime();
}

// ----------------------------------------------------------------------------

Script::Script(
   const ScriptPredefinedNameTable& thePredefinedNameTable,
   const std::string&                    theScript)
:  codeM(0),
   nextFileCheckDateM(Time::getUtcTime() + ScriptCheckIntervalC)
{
   ScriptParser parser(thePredefinedNameTable);
   codeM = parser.parse(theScript);
   includedFilesM = parser.getIncludedFiles();
}
         
// ----------------------------------------------------------------------------

Script::~Script()
{
   delete codeM;
   codeM = 0;
}

// ----------------------------------------------------------------------------

ScriptValue
Script::execute(
   ScriptExecutionContext* theExecutionContext)
{
   theExecutionContext->resetNumberOfExecutedNodes();
   Time startTime(Time::getUtcTime());
   ScriptValue result;
   if (codeM != 0)
   {
      result = codeM->execute(theExecutionContext);
   }
   statisticsM.countFunctionCall(
      InitC, 
      theExecutionContext->getNumberOfExecutedNodes(), 
      startTime);
   return result;
}
      
// ----------------------------------------------------------------------------

ScriptValue
Script::executeFunction(
   ScriptExecutionContext* theExecutionContext,
   const std::string&           theFunctionName,
   const ScriptValueArray& theArguments)
{
   theExecutionContext->traceFunctionCall(0, theFunctionName, theArguments);

   ScriptValue fctRef(theExecutionContext->getIdentifier(theFunctionName));
   if (fctRef.getBase() == 0)
   {
      // Function name does not exist
      return ScriptValue::UndefinedC;
   }
   ScriptValue fct(fctRef.getValue());
   if (fct.getDataType() != ScriptValue::ObjectE)
   {
      // Function name exists, but does not refer to an object
      return ScriptValue::UndefinedC;
   }
   ScriptObject* obj = fct.toObject();
   if (obj->getObjectType() != ScriptObject::FunctionE)
   {
      // Function name refers to a non-function object
      return ScriptValue::UndefinedC;
   }

   ScriptObjectActivation* activationObject = new ScriptObjectActivation;

   theExecutionContext->addObjectToScopeChain(activationObject);
   
   theExecutionContext->resetNumberOfExecutedNodes();

   Time startTime(Time::getUtcTime());

   ScriptValue result(obj->call(theExecutionContext, theArguments));

   statisticsM.countFunctionCall(
      theFunctionName, 
      theExecutionContext->getNumberOfExecutedNodes(), 
      startTime);

   theExecutionContext->removeObjectFromScopeChain();

   return result;      
}

// ----------------------------------------------------------------------------

ScriptValue
Script::executeFunction(
   ScriptExecutionContext* theExecutionContext,
   const std::string&           theFunctionName)
{
   ScriptValueArray arguments;
   return executeFunction(theExecutionContext, theFunctionName, arguments);
}

// ----------------------------------------------------------------------------

const std::string&
Script::getFilename() const
{
   return filenameM;
}

// ----------------------------------------------------------------------------

const ScriptPreprocessor::FileMap&
Script::getIncludedFiles() const
{
   return includedFilesM;
}

// ----------------------------------------------------------------------------

void
Script::print(
   std::string& theOutputString) const
{
   if (codeM != 0)
   {
      size_t maxFilenameLength = filenameM.size();
      for (ScriptPreprocessor::FileMap::const_iterator iter = includedFilesM.begin();
           iter != includedFilesM.end();
           iter++)
      {
         const std::string& includeFilename = (*iter).first;
         if (includeFilename.size() > maxFilenameLength)
         {
            maxFilenameLength = includeFilename.size();
         }   
      }
      codeM->print(theOutputString, "", maxFilenameLength);
   }
}

// ----------------------------------------------------------------------------

void
Script::printStatistics(
   std::string&       theOutputString,
   time_t        theIntervalLength,
   const std::string& theLinePrefix) const
{
   theOutputString += theLinePrefix + filenameM + ":\n";
   statisticsM.print(theOutputString, 
                     theIntervalLength, 
                     theLinePrefix + "   ");
}

// ----------------------------------------------------------------------------

void
Script::resetStatistics()
{
   statisticsM.reset();
}

