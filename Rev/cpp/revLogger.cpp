#include <stdio.h>
#include "revLogger.h"
#include "Log.h"

Logger::Logger() :
logTypeBits(LOG_TYPE_OUTPUT_TO_CONSOLE)
{}

Logger::~Logger()
{}


void Logger::Info(const char* format, ...)
{
    if(logTypeBits == 0) return;

    va_list va;
    va_start(va, format);
    vsprintf(logBuffer, format, va);
    if(logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGI("%s", logBuffer);
    va_end(va);
}

void Logger::Warning(const char* format, ...)
{
    if(logTypeBits == 0) return;

    va_list va;
    va_start(va, format);
    vsprintf(logBuffer, format, va);
    if(logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGW("%s", logBuffer);
    va_end(va);
}

void Logger::Error(const char* format, ...)
{
    if(logTypeBits == 0) return;

    va_list va;
    va_start(va, format);
    vsprintf(logBuffer, format, va);
    if(logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGE("%s", logBuffer);
    va_end(va);
}