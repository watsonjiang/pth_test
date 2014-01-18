#include <stdio.h>
#include <unittestdef.h>
#include "PalTime.h"
#include "tracing.h"
#include "PalScriptPredefinedNameTable.h"
#include "PalScript.h"

using namespace Pal;
using namespace wpr_tracing;

void
parseScript_test1(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  ScriptPredefinedNameTable predefinedNameTable; 
  try{
     Script*  tmp = new Script(predefinedNameTable, "var a=1;");
  }catch(std::exception& e){
     WPR_LOG(100, "Parse failed. Got exception: %s", e.what());
     return;
  }
  WPR_LOG(100, "Parse OK."); 
}

DefineTestSuite(PalScriptTests, uts::root());
DefineTestCase(parseScript_test1, PalScriptTests);
