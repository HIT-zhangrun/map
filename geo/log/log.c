#include "log.h"
#include "time/timestamp.h"
#include <stdio.h>

const static char log_level[6][10] = {"TRACE", "INFO", "DEBUG", "WARN", "ERROR", "FATAL"};
INIT void log_init()
{
    FILE *file = fopen("log.log", "w");
    fclose(file);
}

void logger(LOG_LEVEL level, const char *dir, int line, const char *fromat, va_list args)
{
    FILE *file = fopen("log.log", "aw");

    static char buf[MAX_LOG_LEN] = {0};

    char time[20] = {0};
    get_timestamp_str_us(time, 10);

    uint32_t len = 0;

    len += snprintf(buf + len, MAX_LOG_LEN - len, "[%s] [%s]", time, log_level[level]);
    len += snprintf(buf + len, MAX_LOG_LEN - len, "[%s:%d]", dir, line);
    len += snprintf(buf + len, MAX_LOG_LEN - len, fromat, args);
    len += snprintf(buf + len, MAX_LOG_LEN - len, "\n");

    printf("%s", buf);
    fwrite(buf, sizeof(char), len, file);

    fclose(file);
}

void print_log(LOG_LEVEL loglevel, char *dir, uint32_t line, const char *fromat, ...)
{
    va_list args;

    va_start(args, fromat);

    printf(fromat, args);
    printf("\n");
    printf("%s\n", fromat);
    printf("%d\n", va_arg(args, int));

    // logger(loglevel, dir, line, fromat, args);

    va_end(args);
}
