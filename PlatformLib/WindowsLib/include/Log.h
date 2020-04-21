#ifndef __LOG_H__
#define __LOG_H__

extern void Log(const char* TAG, const char* format, ...);

// TODO:
#define NATIVE_LOGI(...) Log("[INFO]", __VA_ARGS__)
#define NATIVE_LOGW(...) Log("[WARNING]", __VA_ARGS__)
#define NATIVE_LOGE(...) Log("[ERROR]", __VA_ARGS__)

#endif
