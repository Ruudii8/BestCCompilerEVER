#include <stdio.h> 
#include <string.h>

#include "logger.h"


void debug(int line, int col, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    writeLog(line, col, "debug", WHITE, msg, args);
    va_end(args);
}

void error(int line, int col, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    writeLog(line, col, "error", RED, msg, args);
    va_end(args);
    log.errorCount++;
}

void warn(int line, int col, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    writeLog(line, col, "warning", YELLOW, msg, args);
    va_end(args);
}

void info(int line, int col, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    writeLog(line, col, "info", BLUE, msg, args);
    va_end(args);
}


void writeLog(int line, int col, const char *logType, const char *logColor, const char *msg, va_list args)
{
    printf("%s%d:%d: %s: ", logColor, line, col, logType, msg);
    vprintf(msg, args);
    printf("%s\n",RESET);

}

logger log = {debug, error, warn, info, 0, 0};
 