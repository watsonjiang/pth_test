#include "JsScriptStatistics.h"
#include <stdio.h>

using namespace Js;


void
ScriptStatistics::countFunctionCall(
   const std::string&  theFunctionName,
   unsigned int   theNumberOfExecutedNodes,
   const Time&    theStartTime)
{
   Time deltaTime(Time::getUtcTime() - theStartTime);
   unsigned long executionTime = deltaTime.getSeconds() * 1000000 +
      deltaTime.getNanoSeconds() / 1000;
   
   
   // Try to insert a new function in the map.
   FunctionData tempData;
   std::pair<FunctionMap::iterator, bool> result =
      functionMapM.insert(FunctionMap::value_type(theFunctionName, 
                                                  tempData));
   if (result.second == true)
   {
      // Function does not yet exist in table
      FunctionData& functionData = (*result.first).second;
      functionData.numberOfCalls = 1;
      functionData.maxNumberOfExecutedNodes = theNumberOfExecutedNodes;
      functionData.totalNumberOfExecutedNodes = theNumberOfExecutedNodes;
      functionData.totalExecutionTime = executionTime;
   } 
   else
   {
      // Function exists in table
      FunctionData& functionData = (*result.first).second;
      functionData.numberOfCalls++;
      if (theNumberOfExecutedNodes > functionData.maxNumberOfExecutedNodes)
      {
         functionData.maxNumberOfExecutedNodes = theNumberOfExecutedNodes;
      }
      functionData.totalNumberOfExecutedNodes += theNumberOfExecutedNodes;
      
      functionData.totalExecutionTime += executionTime;
   } 
}

// ----------------------------------------------------------------------------

void
ScriptStatistics::print(
   std::string&       theOutputString,
   time_t        theIntervalLength,
   const std::string& theLinePrefix) const
{

   for (FunctionMap::const_iterator iter = functionMapM.begin();
        iter != functionMapM.end();
        iter++)
   {
      const std::string&       functionName = (*iter).first;
      const FunctionData& functionData = (*iter).second;
      theOutputString += theLinePrefix;
      theOutputString += functionName;
      theOutputString += "():\n";
      std::string newLinePrefix(theLinePrefix + "   ");
      theOutputString += newLinePrefix;
      theOutputString += "Number of function calls            = ";
      char buf[10] = {0};
      snprintf(buf, 10, "%d", functionData.numberOfCalls);
      //theOutputString.appendNumber(functionData.numberOfCalls);
      theOutputString.append(buf);
      if (theIntervalLength != 0)
      {
         double callsPerSecond = 
            (double)functionData.numberOfCalls / theIntervalLength;
         char buffer[256];
         sprintf(buffer, " (%.2f/sec)", callsPerSecond);
         theOutputString += buffer;
      }
      theOutputString += "\n";
      theOutputString += newLinePrefix;
      theOutputString += "Average number of instructions/call = ";
      double avgExecutedNodes = functionData.totalNumberOfExecutedNodes /
                                functionData.numberOfCalls;
      snprintf(buf, 10, "%d", (unsigned int)avgExecutedNodes);
      //theOutputString.appendNumber((unsigned int)avgExecutedNodes);
      theOutputString.append(buf);
      theOutputString += "\n";
      theOutputString += newLinePrefix;
      theOutputString += "Maximum number of instructions/call = ";
      snprintf(buf, 10, "%d", functionData.maxNumberOfExecutedNodes);
      //theOutputString.appendNumber(functionData.maxNumberOfExecutedNodes);
      theOutputString.append(buf);
      theOutputString += "\n";
      theOutputString += newLinePrefix;
      theOutputString += "Average execution time/call         = ";
      double avgExecutionTime = functionData.totalExecutionTime /
                                functionData.numberOfCalls;
      snprintf(buf, 10, "%d", (unsigned int)avgExecutionTime);
      //theOutputString.appendNumber((unsigned int)avgExecutionTime);
      theOutputString.append(buf);
      theOutputString += " usec\n";
   }
}

// ----------------------------------------------------------------------------

void
ScriptStatistics::reset()
{
   functionMapM.erase(functionMapM.begin(), functionMapM.end());
}

