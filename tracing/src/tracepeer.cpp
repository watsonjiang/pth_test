#include "tracepeer.h"
#include "traceappender.h"
#include "tracemsg.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>


namespace wpr_tracing
{

  TracePeer::TracePeer(const unsigned int peerId)
  :_peerId(peerId)
  {
  }

  void
  TracePeer::traceInfo(const char * srcfile,
      const int line, const char * function, const char * format, va_list ap)
  {
    TraceMsg traceMsg = {
                         _peerId,
                         srcfile,
                         line,
                         function,
                         format,
                         ap
                        };
    for(std::list<TraceAppender *>::iterator it=_appenderList.begin();
        it!=_appenderList.end();
        ++it){
        (*it)->traceInfo(traceMsg);
    }
    va_end(ap);
  }

}

