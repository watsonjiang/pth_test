#include <stdio.h>
#include <unittestdef.h>
#include "PalTime.h"
#include "tracing.h"
using namespace Pal;
using namespace wpr_tracing;

void
getLocalTime_test(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  Time time = Time::getLocalTime();
  WPR_LOG(100, "time is %s", time.toAsctimeString().c_str()); 
}

DefineTestSuite(PalTimeTests, uts::root());
DefineTestCase(getLocalTime_test, PalTimeTests);
