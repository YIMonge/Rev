#ifndef __REVLOGGER_H__
#define __REVLOGGER_H__



class Logger
{
public:
    Logger();
    ~Logger();

    enum LOG_TYPE
    {
        LOG_TYPE_OUTPUT_TO_CONSOLE,
        LOG_TYPE_OUTPUT_TO_FILE,
        LOG_TYPE_NUM,
    };

private:
    uint32 logTypeBits;
};


#endif
