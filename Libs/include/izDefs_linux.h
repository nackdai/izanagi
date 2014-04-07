#if !defined(__IZANAGI_DEFS_LINUX_H__)
#define __IZANAGI_DEFS_LINUX_H__

#include <sys/time.h>

#ifndef _UNICODE
    #define IZ_VSPRINTF     vsnprintf
    #define IZ_SPRINTF      snprintf
    #define IZ_FPRINTF      fprintf
#endif  // #ifdef _UNICODE

/////////////////////////////////////////////////////////////
// いろいろ便利系

#ifndef DEBUG_BREAK
    #define DEBUG_BREAK()   __builtin_trap()
#endif  // #ifndef DEBUG_BREAK

#define IZ_PRINTF   printf

typedef timeval IZ_TIME;

#define IzMain(screenWidth, screenHeight)\
    int _main(int argc, char* argv[], IZ_UINT __width, IZ_UINT __height);\
    int main(int argc, char* argv[])\
    {\
        return _main(argc, argv, screenWidth, screenHeight);\
    }\
    int _main(int argc, char* argv[], IZ_UINT __width, IZ_UINT __height)

#define IzGetSystemDataForMainFunc()  NULL
#define IzGetScreenWidth() __width
#define IzGetScreenHeight() __height

#define IZ_FILE_HANDLE FILE*
#define IZ_PLATFORM_PARAM   void*

#endif  // #if !defined(__IZANAGI_DEFS_LINUX_H__)
