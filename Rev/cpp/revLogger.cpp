#include <stdio.h>
#include "revLogger.h"
#include "Log.h"
#include <stdarg.h>

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
    if(logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGI(format, va);
    va_end(va);
}

void Logger::Warning(const char* format, ...)
{
    if(logTypeBits == 0) return;

    va_list va;
    va_start(va, format);
    if (logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGW(format, va);
    va_end(va);
}

void Logger::Error(const char* format, ...)
{
    if(logTypeBits == 0) return;

    va_list va;
    va_start(va, format);
    if (logTypeBits & LOG_TYPE_OUTPUT_TO_CONSOLE) NATIVE_LOGE(format, va);
    va_end(va);
}