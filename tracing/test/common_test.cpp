#include <tracing.h>
#include <pth.h>
#include <stdio.h>
#include <unittestdef.h>
using namespace wpr_tracing;
using namespace uts;

void
func_log_test(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(120);
  WPR_LOG_FUNCTION(120);
}


void
log_test(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100);  
  WPR_LOG(100,"hello world 100");
}

DefineTestSuite(TracingTests, uts::root());
DefineTestCase(log_test, TracingTests);
DefineTestCase(func_log_test, TracingTests);
