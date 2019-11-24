#ifndef __LOG_H__
#define __LOG_H__

#ifdef _ENABLE_LOG
#include <android/log.h>

#define NATIVE_LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "Rev", __VA_ARGS__))
#define NATIVE_LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, "Rev", __VA_ARGS__))
#define NATIVE_LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, "Rev", __VA_ARGS__))

#endif
#endif