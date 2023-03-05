#include "timestamp.h"

static uint64_t system_start_us = 0;
INIT void time_init()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    system_start_us = tv.tv_sec * 1000000 + tv.tv_usec;
}

uint64_t get_timestamp_us()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    uint64_t timestamp_us = tv.tv_sec * 1000000 + tv.tv_usec;

    return timestamp_us - system_start_us;
}

uint64_t get_timestamp_ms()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    uint64_t timestamp_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return timestamp_ms - system_start_us / 1000;
}

uint64_t get_timestamp_s()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    uint64_t timestamp_s = tv.tv_sec;

    return timestamp_s - system_start_us / 1000000;
}

uint32_t get_timestamp_str_us(char *buf, uint32_t size)
{
    struct timeval tv;
    uint32_t len = 0;
    gettimeofday(&tv, NULL);

    uint64_t us = get_timestamp_us();
    uint64_t ms = us / 1000;
    uint64_t s  = us / 1000000;

    len += snprintf(buf + len, size - len, "%ld", us / 1000000);
    len += snprintf(buf + len, size - len, ".");
    len += snprintf(buf + len, size - len, "%06ld", us - s * 1000000);

    return len;
}
