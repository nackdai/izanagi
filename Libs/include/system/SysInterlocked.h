#if !defined(__IZANAGI_SYSTEM_SYS_INTERLOCKED_H__)
#define __IZANAGI_SYSTEM_SYS_INTERLOCKED_H__

#include "izStd.h"
#include "SysThreadDefs.h"

#if defined(WIN32) || defined(WIN64)

    IZ_C_ASSERT(sizeof(LONG) == sizeof(IZ_INT));

    /**
     */
    #define IZ_INTERLOCKED_INCREMENT(addend)    (IZ_INT)InterlockedIncrement(reinterpret_cast<LONG*>(addend))

    /**
     */
    #define IZ_INTERLOCKED_DECREMENT(addend)    (IZ_INT)InterlockedDecrement(reinterpret_cast<LONG*>(addend))

#else
#endif

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_INTERLOCKED_H__)
