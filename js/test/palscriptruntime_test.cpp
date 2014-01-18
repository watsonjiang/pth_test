#include <stdio.h>
#include <unittestdef.h>
#include "PalTime.h"
#include "tracing.h"
#include "PalScriptRuntime.h"
#include "PalScript.h"

using namespace Pal;

void
getScript_test1(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  ScriptRuntime *rt = new ScriptRuntime();
  std::string error;
  Script* script = rt->getScript("test.js", error);
  if(NULL == script){  
     WPR_LOG(100, "%s", error.c_str());
     fail_if(true);
     return;
  }
  std::string tmp;
  script->print(tmp);
  //WPR_LOG(100, "%s", tmp.c_str());
  ScriptExecutionContext* ctx = rt->newContext();
  int rs = rt->runScript(ctx, script, error);
  if(rs != 0){
     WPR_LOG(100, "%s", error.c_str());
  } 
}

DefineTestSuite(PalRuntimeTests, uts::root());
DefineTestCase(getScript_test1, PalRuntimeTests);
