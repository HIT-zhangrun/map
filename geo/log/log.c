#include "log.h"
#include "time/timestamp.h"

const static char log_level[4][10] = {"INFO", "DEBUG", "WARN", "ERROR"};
INIT void log_init()
{
    FILE *file = fopen("log.log", "w");
    fclose(file);
}

void logger(LOG_LEVEL level, char *fromat, va_list args)
{
    FILE *file = fopen("log.log", "aw");

    static char buf[MAX_LOG_LEN] = {0};

    char time[20] = {0};
    get_timestamp_str_us(time, 10);

    uint32_t len = 0;

    len += snprintf(buf + len, MAX_LOG_LEN - len, "[%s] [%s]", time, log_level[level]);
    len += snprintf(buf + len, MAX_LOG_LEN - len, fromat + 2, args);
    len += snprintf(buf + len, MAX_LOG_LEN - len, "\n");

    printf("%s", buf);
    fwrite(buf, sizeof(char), len, file);

    fclose(file);
}

void print_log(LOG_LEVEL loglevel, char *fromat, ...)
{
    va_list args;

    va_start(args, fromat);

    logger(loglevel, fromat, args);

    va_end(args);
}
