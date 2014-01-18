#ifndef __WPR_TRACEPEER_H__
#define __WPR_TRACEPEER_H__
#include <cstdarg>
#include <list>

namespace wpr_tracing 
{
  class TraceAppender;
  /**
   * TracePeer is logic container to collect log messages from
   * business modules. 
   */
  class TracePeer
  {
  private:
    TracePeer(const unsigned int peerId);  // private constructor to make sure 
                                           // no one can create
                                           // peer except TraceManager

  public:
    void
    traceInfo(const char * srcfile, const int line,
              const char * function, const char * format, va_list ap);
    
  private:
    unsigned int  _peerId;
    std::list<TraceAppender *> _appenderList;

  friend class TraceManager;
  };
}
#endif // tracePeer_h
