#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef INIT
#define INIT __attribute((constructor))
#endif

/* 获取系统运行时间戳,单位:us */
uint64_t get_timestamp_us();

/* 获取系统运行时间戳,单位:ms */
uint64_t get_timestamp_ms();

/* 获取系统运行时间戳,单位:s */
uint64_t get_timestamp_s();

/* 获取系统运行时间戳,字符串形式,单位:s */
uint32_t get_timestamp_str_us(char *buf, uint32_t size);

#endif
