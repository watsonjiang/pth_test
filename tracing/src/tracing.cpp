#include "tracing.h"
#include "tracemanager.h"
#include "tracepeer.h"
#include <cstdarg>

using namespace wpr_tracing;

//main interface to log info.
void 
wpr_log(const unsigned int peerId, 
        const char * srcfile, const int line,
        const char * function, const char * format, ...)
{
  va_list ap;
  va_start(ap,format);
  TraceManager::getInstance()->getTracePeer(peerId)->traceInfo(srcfile, line, function, format, ap);
  va_end(ap);
}

void 
wpr_log_addConsoleAppender(const unsigned int peerId)   
{
  TraceManager::getInstance()->addConsoleAppender(peerId);
}
