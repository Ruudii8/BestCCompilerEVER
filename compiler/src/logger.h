#pragma once

#include <stdarg.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"   


typedef struct
{
    void (*debug) (int line, int col, const char *msg, ...);
    void (*error) (int line, int col, const char *msg, ...);
    void (*warn) (int line, int col, const char *msg, ...);
    void (*info) (int line, int col, const char *msg, ...);
    int errorCount;
    int warningCount;

} logger;

extern logger log;
