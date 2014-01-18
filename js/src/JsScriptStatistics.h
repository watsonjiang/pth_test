#ifndef PALSCRIPTSTATISTICS_H
#define PALSCRIPTSTATISTICS_H

#include "JsTime.h"
#include <map>

namespace Js
{
   class ScriptStatistics
   {
   public:

      void
      countFunctionCall(
         const std::string&  theFunctionName,
         unsigned int   theNumberOfExecutedNodes,
         const Time&    theStartTime);

      void
      print(
         std::string&       theOutputString,
         time_t        theIntervalLength,
         const std::string& theLinePrefix = "") const;

      void
      reset();

   private:
      
      struct FunctionData
      {
         unsigned int  numberOfCalls;
         unsigned int  maxNumberOfExecutedNodes;
         double        totalNumberOfExecutedNodes;
         double        totalExecutionTime;
      };

      typedef std::map<std::string, FunctionData> FunctionMap;
      
      FunctionMap       functionMapM;
   };
}

#endif
