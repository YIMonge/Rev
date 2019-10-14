#ifndef __REVTHREAD_H__
#define __REVTHREAD_H__

#ifdef _WINDOWS
#include "../../PlatformLib/WindowsLib/include/Thread.h"
#elif defined _ANDROID
#include "../../PlatformLib/AndroidLib/include/Thread.h"
#elif defined _IOS
#include "../../PlatformLib/iOSLib/include/Thread.h"
#endif

typedef Thread revThread;


#endif