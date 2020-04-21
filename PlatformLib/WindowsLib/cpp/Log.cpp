#include "Log.h"
#include <stdarg.h>
#include <stdio.h>
#include <Window.h>
#include "revTypedef.h"


void Log(const char* TAG, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    const uint32 MAX_LOG_LENGTH = 256;
    char buffer[MAX_LOG_LENGTH];

    int32 l = sprintf_s(buffer, MAX_LOG_LENGTH, TAG);
    if (l != EOF && l < MAX_LOG_LENGTH) {
        l += vsprintf_s(buffer + l, MAX_LOG_LENGTH - l, format, ap);
    }
    if (l != EOF && l < MAX_LOG_LENGTH - 1) {
        buffer[l] = '\n';
        buffer[l + 1] = '\0';
        OutputDebugString(buffer);
    }
    else {
        OutputDebugString("[ERROR] : too long log");
    }
    va_end(ap);
}
