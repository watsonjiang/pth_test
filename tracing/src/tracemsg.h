/*
 * tracemsg.h
 *
 *  Created on: May 2, 2009
 *      Author: eqijian
 */

#ifndef __WPR_TRACEMSG_H__
#define __WPR_TRACEMSG_H__
namespace wpr_tracing
{
  struct TraceMsg
  {
    const unsigned int peerId;
    const char * srcfile;
    const unsigned int line;
    const char * function;
    const char * format;
    void * ap; //ref to va_list struct
  };
}

#endif /* __WPR_TRACEMSG_H_ */
