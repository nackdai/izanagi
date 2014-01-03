#if !defined(__IZANAGI_DEFS_LINUX_H__)
#define __IZANAGI_DEFS_LINUX_H__

#ifndef _UNICODE
    #define IZ_VSPRINTF     vsnprintf
    #define IZ_SPRINTF      snprintf
    #define IZ_FPRINTF      fnprintf
#endif  // #ifdef _UNICODE

/////////////////////////////////////////////////////////////
// ‚¢‚ë‚¢‚ë•Ö—˜Œn

#ifndef DEBUG_BREAK
    #define DEBUG_BREAK()
#endif  // #ifndef DEBUG_BREAK

#define IZ_DEBUG_PRINT(str)    printf("%s", str)

#define IZ_PRINTF   printf

#endif  // #if !defined(__IZANAGI_DEFS_LINUX_H__)
