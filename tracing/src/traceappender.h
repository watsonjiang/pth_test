#ifndef __WPR_TRACEAPPENDER_H__
#define __WPR_TRACEAPPENDER_H__

#define MAX_BUFFER_SIZE 1024
namespace wpr_tracing
{
  class TraceMsg;
  class TraceAppender
  {
  public:
    virtual ~TraceAppender(){}; //a trival virtual destrcutor to make sure 
                                //decendant class can be properly destrcuted.
    virtual
    void
    traceInfo(const TraceMsg& msg) = 0;
  };

  class ConsoleTraceAppender : public TraceAppender
  {
  public:
    void
    traceInfo(const TraceMsg& msg);
  };

  class FileTraceAppender : public TraceAppender
  {
  public:
    FileTraceAppender(const char * filename);
    ~FileTraceAppender();
    void
    traceInfo(const TraceMsg& msg);
  private:
    int _fd;
  };
}

#endif
