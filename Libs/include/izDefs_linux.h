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
    #define DEBUG_BREAK()
#endif  // #ifndef DEBUG_BREAK

#define IZ_PRINTF   printf

typedef timeval IZ_TIME;

#define IzMain(screenWidth, screenHeight) 
    static const IZ_UINT _izScreenWidth = screenWidth;\
    static const IZ_UINT _izScreenHeight = screenHeight;\
    int _main(int argc, char* argv[]);\
    int main(int argc, char* argv[])
    {\
        return _main(argc, argv);\
    }\
    int _main(int argc, char* argv[])

#define IzGetSystemDataForMainFunc()  NULL
#define IzGetScreenWidth() _izScreenWidth
#define IzGetScreenHeight() _izScreenHeight

#define FILE_HANDLE FILE*

#endif  // #if !defined(__IZANAGI_DEFS_LINUX_H__)
