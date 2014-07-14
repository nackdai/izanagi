#if !defined(__IZANAGI_DEFS_WINDOWS_H__)
#define __IZANAGI_DEFS_WINDOWS_H__

#ifndef WINDOWS
    #if defined(WIN32) || defined(WIN64)
        #define WINDOWS
    #endif
#endif  // #ifndef WINDOWS

#ifdef WINDOWS
    #include <windows.h>

    #ifdef _WINDLL
        #define IZ_API  __declspec(dllexport)
    #else
        #define IZ_API  __declspec(dllimport)
    #endif
#endif  // #ifdef WINDOWS

#ifndef _UNICODE
    #define IZ_VSPRINTF     vsprintf_s
    #define IZ_SPRINTF      sprintf_s
    #define IZ_FPRINTF      fprintf_s
#endif  // #ifdef _UNICODE

#ifndef DEBUG_BREAK
    //#define DEBUG_BREAK()     ::DebugBreak()
    //#define DEBUG_BREAK()       assert(false)
    #define DEBUG_BREAK()       __debugbreak()
#endif  // #ifndef DEBUG_BREAK

inline void _OutputDebugString(const char* format, ...)
{
    va_list argp;
    char buf[256];
    va_start(argp, format);
    IZ_VSPRINTF(buf, sizeof(buf), format, argp);
    va_end(argp);

    ::OutputDebugString(buf);
}

#define IZ_PRINTF   _OutputDebugString

typedef IZ_INT64 IZ_TIME;

#if 0
#define IzMain(screenWidth, screenHeight) \
    static const IZ_UINT _izScreenWidth = screenWidth;\
    static const IZ_UINT _izScreenHeight = screenHeight;\
    int main(int argc, char* argv[], HINSTANCE hInstance);\
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)\
    {\
        return main(0, NULL, hInstance);\
    }\
    int main(int argc, char* argv[], HINSTANCE hInstance)
#else
#define IzMain(screenWidth, screenHeight) \
    static const IZ_UINT _izScreenWidth = screenWidth;\
    static const IZ_UINT _izScreenHeight = screenHeight;\
    int _iz_main(int argc, char* argv[], HINSTANCE hInstance);\
    int main(int argc, char* argv[])\
    {\
        HINSTANCE hInstance = GetModuleHandle(NULL);\
        return _iz_main(0, NULL, hInstance);\
    }\
    int _iz_main(int argc, char* argv[], HINSTANCE hInstance)
#endif

#define IzGetSystemDataForMainFunc()  hInstance
#define IzGetScreenWidth() _izScreenWidth
#define IzGetScreenHeight() _izScreenHeight

#define IZ_FILE_HANDLE      FILE*
#define IZ_PLATFORM_PARAM   void*

#endif  // #if !defined(__IZANAGI_DEFS_WINDOWS_H__)
