#ifndef __WPR_TRACING_H__
#define __WPR_TRACING_H__

//note: for better c compatibility, should avoid to use namespace in interface. 

//main interface to log info.
extern "C"{
void 
wpr_log(const unsigned int peerId, 
        const char * srcfile, const int line,
        const char * function, const char * format, ...);

void
wpr_log_addConsoleAppender(const unsigned int peerId);
} //extern "C"

#ifndef WPR_ENABLE_TRACING
#define WPR_ENABLE_TRACING 1
#endif

#ifndef WPR_ENABLE_FUNCTION_TRACING
#define WPR_ENABLE_FUNCTION_TRACING 1
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef WPR_LOG_TRACE
#  if WPR_ENABLE_TRACING == 1
#    define WPR_LOG_TRACE(logger, file, line, function, format, ...)\
        {\
        if( NULL != logger )\
        {\
        wpr_log(logger, file, line, function, format, ##__VA_ARGS__);\
        }\
        }
#  else
#    define WPR_LOG_TRACE(logger, file, line, function, const char * format, ...) {}/* empty */
#  endif
#endif

#ifndef WPR_LOG
#define WPR_LOG(logger, format, ...)\
    WPR_LOG_TRACE(logger, __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__);
#endif

//function tracing.
#ifdef WPR_ENABLE_FUNCTION_TRACING
#ifndef WPR_LOG_FUNCTION_CALL_IN
#define WPR_LOG_FUNCTION_CALL_IN(logger, format, ...)\
    WPR_LOG(logger, format, ##__VA_ARGS__);
#endif

#ifndef WPR_LOG_FUNCTION_CALL_RETURN
#define WPR_LOG_FUNCTION_CALL_RETURN(logger, format, ...)\
    WPR_LOG(logger, format, ##__VA_ARGS__);
#endif

#ifndef WPR_TMPVAR
#  define WPR_TMPVAR(a) __WPR_TMPVAR_##a
#endif

#ifndef WPR_FUNCTION_TRACER
#  define WPR_FUNCTION_TRACER(logger) \
    FunctionTracer WPR_TMPVAR(__LINE__)(logger, __FILE__, __FUNCTION__);
#endif

#ifndef WPR_LOG_FUNCTION
#  define WPR_LOG_FUNCTION(logger)\
    WPR_FUNCTION_TRACER(logger)
#endif

namespace wpr_tracing
{
  class FunctionTracer
  {
    public:
       FunctionTracer(const unsigned int peerId, const char * file,
              const char * function);
       ~FunctionTracer();

    private:
      unsigned int _peerId;
      const char * _file;
      const char * _function;
  };
}
#else
# ifndef LOG_FUNCTION
#  define LOG_FUNCTION(logger){} /*empty*/
# endif
#endif   //function tracing.

#endif

