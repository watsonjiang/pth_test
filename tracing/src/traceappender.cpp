/*
 * traceappender.cpp
 *
 *  Created on: May 1, 2009
 *      Author: eqijian
 */
#include "traceappender.h"
#include "tracing.h"
#include <cstdarg>
#include "tracemsg.h"
#include <time.h>
#include <sys/time.h>
#include <pth.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define  MAX_BUFFER_SIZE  1024

namespace wpr_tracing
{

  static
  inline int
  printTimeStamp(char *p, int max_len)
  {
    struct timeval now;
    gettimeofday(&now, NULL);
    struct tm local_now;
    localtime_r(&now.tv_sec, &local_now);
    int n = strftime(p, max_len, "%x %X", &local_now);
    max_len = max_len - n;
    p = p + n;
    n += snprintf(p, max_len, ".%ld", (long) (now.tv_usec) / 1000);
    return n;
  }

  static
  inline
  void
  buildTraceMsg(char* buf, unsigned int buf_size, const TraceMsg& msg) 
  {
    char *p = buf;
    unsigned int max_len = buf_size;
    unsigned int n = 0;
    n = printTimeStamp(buf, max_len);
    p = p + n;
    max_len = max_len - n;
    n = snprintf(p, max_len, " (%s:%d): ", msg.srcfile, msg.line);
    p = p + n;
    max_len = max_len - n;

    vsnprintf(p, max_len, msg.format, *((va_list *)msg.ap));
     

    return;
  }

  void
  ConsoleTraceAppender::traceInfo(const TraceMsg& msg)
  {
    char msgbuf[MAX_BUFFER_SIZE + 1] =
      { 0 };

    buildTraceMsg(msgbuf, MAX_BUFFER_SIZE, msg);

    write(STDOUT_FILENO, msgbuf, strlen(msgbuf));
    write(STDOUT_FILENO, "\n", 1);
    return;
  }
 
  FileTraceAppender::FileTraceAppender(const char * filename)
  :_fd(-1)
  {
    _fd = open(filename, O_WRONLY|O_APPEND|O_CREAT|O_NONBLOCK); 
  }

  FileTraceAppender::~FileTraceAppender()
  {
    if(_fd!=-1){
      close(_fd);
    }
  }
  
  void
  FileTraceAppender::traceInfo(const TraceMsg& msg)
  {
    if( _fd == -1){
      return;
    } 

    char msgbuf[MAX_BUFFER_SIZE + 1] =
      { 0 };

    buildTraceMsg(msgbuf, MAX_BUFFER_SIZE, msg);

    write(_fd, msgbuf, strlen(msgbuf)); //file io is always 'blocking'
    write(_fd, "\n", 1);
    return;
  }
}
