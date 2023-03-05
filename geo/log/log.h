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

#define MAX_LOG_LEN (200)

typedef enum LOG_LEVEL_
{
    INFO =0,
    DEBUG ,
    WARN  ,
    ERROR ,
    ALL     =255
} LOG_LEVEL;

/* 日志打印接口 */
void print_log(LOG_LEVEL loglevel, char *fromat, ...);

#endif
