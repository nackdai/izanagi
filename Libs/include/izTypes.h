#if !defined(__IZANAGI_TYPES_H__)
#define __IZANAGI_TYPES_H__

#if defined(WINDOWS) || defined(WIN32) || defined(WIN64)
    #include "izTypes_windows.h"
#else
    #include "izTypes_linux.h"
#endif

#endif  // #if !defined(__IZANAGI_TYPES_H__)
