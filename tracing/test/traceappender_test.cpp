#include "tracemsg.h"
#include "traceappender.h"
#include <cstdarg>
#include <stdio.h>
#include <unittestdef.h>
using namespace wpr_tracing;

TraceAppender * traceAppender = NULL;

void testConsoleAppender(const char * format, ...) 
{

  char filename[100] = {0};

  va_list ap;
  va_start(ap, format);
  TraceMsg msg = {
                   1,
                   "src.cpp",
                   100,
                   "test",
                   format,
                   (void *)&ap
                 };
  traceAppender->traceInfo(msg);
  va_end(ap);

}


void testfunc1(uts::TestContext& context)
{
  traceAppender = new ConsoleTraceAppender();
  testConsoleAppender("%s", "hello,world");
  delete traceAppender;
}


DefineTestSuite(TracingTests1, uts::root());
DefineTestCase(testfunc1, TracingTests1);
