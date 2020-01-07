#ifndef __REVLOGGER_H__
#define __REVLOGGER_H__

#include "revSingleton.h"
#include "revTypedef.h"

#ifdef _ENABLE_LOG
#define LOGI(...) Logger::Get().Info(__VA_ARGS__)
#define LOGW(...) Logger::Get().Warning(__VA_ARGS__)
#define LOGE(...) Logger::Get().Error(__VA_ARGS__)
#else
#define LOGI(...) static_cast<void>(0)
#define LOGW(...) static_cast<void>(0)
#define LOGE(...) static_cast<void>(0)
#endif

class Logger : public revSingleton<Logger>
{
public:
    Logger();
    ~Logger();

    enum LOG_TYPE
    {
        LOG_TYPE_OUTPUT_TO_CONSOLE  = 1,
        LOG_TYPE_OUTPUT_TO_FILE     = 1 << 1,
    };

    void Info(const char* format, ...);
    void Warning(const char* format,...);
    void Error(const char* format,...);



private:
    uint32 logTypeBits;
    char logBuffer[1024];
};


#endif
