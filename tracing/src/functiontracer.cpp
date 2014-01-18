#include "tracing.h"
#ifdef WPR_ENABLE_FUNCTION_TRACING
namespace wpr_tracing
{
  // constructor
  FunctionTracer::FunctionTracer(const unsigned int peerId, const char * file,
                                 const char * function) :
    _peerId(peerId), _file(file), _function(function)
  {
      ::wpr_log(_peerId, _file, 0, _function, "->%s", _function);
  } 

  // destructor
  FunctionTracer::~FunctionTracer()
  {
      ::wpr_log(_peerId, _file, 0, _function, "<-%s", _function);
  }
}
#endif
