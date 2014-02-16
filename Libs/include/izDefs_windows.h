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

#define IZ_DEBUG_PRINT(str)    ::OutputDebugString(str)

#define IZ_PRINTF   _OutputDebugString

typedef IZ_INT64 IZ_TIME;

#define IzMain() \
int main(int argc, char* argv[], HINSTANCE hInstance);\
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)\
{\
    return main(0, NULL, hInstance);\
}\
int main(int argc, char* argv[], HINSTANCE hInstance)\

#define IzGetSystemDataForMainFunc()  hInstance

#endif  // #if !defined(__IZANAGI_DEFS_WINDOWS_H__)
