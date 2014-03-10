#if !defined(__IZANAGI_DEFS_LINUX_H__)
#define __IZANAGI_DEFS_LINUX_H__

#include <sys/time.h>
#include <android/log.h>

#ifndef _UNICODE
    #define IZ_VSPRINTF     vsnprintf
    #define IZ_SPRINTF      snprintf
    #define IZ_FPRINTF      fprintf
#endif  // #ifdef _UNICODE

/////////////////////////////////////////////////////////////
// いろいろ便利系

#define IZ_LOG_TAG  "izanagi"

#define IZ_PRINTF(...)   __android_log_print(ANDROID_LOG_VERBOSE, IZ_LOG_TAG, __VA_ARGS__)

#ifdef __IZ_DEBUG__
    #define IZ_ASSERT(b)\
        if (!(b)) {\
            __android_log_assert("", IZ_LOG_TAG, "assert : %s(%d)\n", __FILE__, __LINE__);\
        }
#else
    #define IZ_ASSERT(b)
#endif

typedef timeval IZ_TIME;

#define IzMain() int main(int argc, char* argv[])

#define IzGetSystemDataForMainFunc()  NULL

#endif  // #if !defined(__IZANAGI_DEFS_LINUX_H__)
