#include <stdio.h>
#include <unittestdef.h>
#include "PalScannerUtilities.h"
#include "tracing.h"
using namespace Pal;
using namespace wpr_tracing;

void
parseNumber_test1(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  const char * str = "10";
  const char * str1 = str; 
  int line = 0;
  int col = 0;
  Number a = ScannerUtilities::getDecimalNumber(str, line, col);
  WPR_LOG(100, "%s = %f", str1, a);
}


void
parseNumber_test2(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  const char * str = "10.0";
  const char * str1 = str; 
  int line = 0;
  int col = 0;
  Number a = ScannerUtilities::getDecimalNumber(str, line, col);
  WPR_LOG(100, "%s = %f", str1, a);
}


void
parseNumber_test3(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  const char * str = "10E2";
  const char * str1 = str; 
  int line = 0;
  int col = 0;
  Number a = ScannerUtilities::getDecimalNumber(str, line, col);
  WPR_LOG(100, "%s = %f", str1, a);
}

void
parseNumber_test4(uts::TestContext& context)
{
  wpr_log_addConsoleAppender(100); 
  const char * str = "10.2E2";
  const char * str1 = str; 
  int line = 0;
  int col = 0;
  Number a = ScannerUtilities::getDecimalNumber(str, line, col);
  WPR_LOG(100, "%s = %f", str1, a);
}

DefineTestSuite(PalNumberParseTests, uts::root());
//DefineTestCase(parseNumber_test1, PalNumberParseTests);
//DefineTestCase(parseNumber_test2, PalNumberParseTests);
DefineTestCase(parseNumber_test3, PalNumberParseTests);
DefineTestCase(parseNumber_test4, PalNumberParseTests);
