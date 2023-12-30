#ifndef __LOG_H__
#define __LOG_H__

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#ifndef INIT
#define INIT __attribute((constructor))
#endif

#define MAX_LOG_LEN (400)

typedef enum LOG_LEVEL_
{
    LOG_TRACE = 0,
    LOG_DEBUG,
    LOG_INFO  ,
    LOG_WARN  ,
    LOG_ERROR ,
    LOG_FATAL ,
} LOG_LEVEL;


#define log_trace(...) print_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) print_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  print_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  print_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(fmt, ...) print_log(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(...) print_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/* 日志打印接口 */
void print_log(LOG_LEVEL loglevel, char *dir, uint32_t line, const char *fmt, ...);

#endif
