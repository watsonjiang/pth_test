#include "tracemsg.h"
#include "traceappender.h"
#include <cstdarg>
#include <pth.h>
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


void testFileAppender(const char * format, ...)
{
   va_list ap;
   va_start(ap, format);
   TraceMsg msg = {
                   1,
                   "src1.cpp",
                   100,
                   "test",
                   format,
                   (void *)&ap
                 };
     traceAppender->traceInfo(msg);   
   va_end(ap);
}

void * threadFunc(void *)
{
  for(int i=0; i<10000; i++){
    testFileAppender("%s tid %d", "hello non-block file io.", pth_self());
  }
}

void testfunc2(uts::TestContext& context)
{
  traceAppender = new FileTraceAppender("nouse");
  pth_t tid[4];
  for(int i=0; i<4; i++){
    tid[i] = pth_spawn(PTH_ATTR_DEFAULT, threadFunc, NULL); 
  }
  for(int i=0; i<4; i++){
    pth_join(tid[i], NULL);
  }
}


DefineTestSuite(TracingTests1, uts::root());
DefineTestCase(testfunc1, TracingTests1);
DefineTestCase(testfunc2, TracingTests1);
