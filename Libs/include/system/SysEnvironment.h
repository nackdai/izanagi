#if !defined(__IZANAGI_SYSTEM_SYS_ENVIRONMENT_H__)
#define __IZANAGI_SYSTEM_SYS_ENVIRONMENT_H__

#include "izDefs.h"

namespace izanagi
{
namespace sys
{
    /**
    */
    class CEnvironment {
    private:
        CEnvironment();
        ~CEnvironment();

        NO_COPIABLE(CEnvironment);

    public:
        // Return CPU num.
        static IZ_UINT GetProcessorNum();

        // Get display's pixel size.
        static IZ_UINT GetPixelWidth(void* nativeWndHandle);
        static IZ_UINT GetPixelHeight(void* nativeWndHandle);

        // Get display' physical size.
        static IZ_UINT GetPhysicalWidth(void* nativeWndHandle);
        static IZ_UINT GetPhysicalHeight(void* nativeWndHandle);

        // Get display's dpi.
        static IZ_UINT GetHorizontalDPI(void* nativeWndHandle);
        static IZ_UINT GetVerticalDPI(void* nativeWndHandle);

        // Get display's refresh rate.
        static IZ_UINT GetRefreshRate(void* nativeWndHandle);
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_ENVIRONMENT_H__)
