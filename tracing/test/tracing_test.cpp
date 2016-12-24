#include <tracing.h>
#include <cstdarg>
#include <stdio.h>
using namespace wpr_tracing;

void
func_log_test()
{
  wpr_log_addConsoleAppender(120);
  WPR_LOG_FUNCTION(120);
}


void
log_test()
{
  wpr_log_addConsoleAppender(100);  
  WPR_LOG(100,"hello world 100");
}

int
main()
{
  log_test();
  func_log_test();
} 
