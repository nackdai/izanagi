#include <unistd.h>
#include "system/SysEnvironment.h"

namespace izanagi
{
namespace sys
{
    // Return CPU num.
    IZ_UINT CEnvironment::GetProcessorNum()
    {
        IZ_UINT num = 0;

#if 0
        __asm
        {
            mov eax,01h //01h is for getting number of cores present in the processor
            cpuid
            mov num,ebx
        }
#else
        num = sysconf(_SC_NPROCESSORS_ONLN);
#endif

        return num;
    }

#if 0
    /**
    * Get display's pixel width.
    */
    IZ_UINT CEnvironment::GetPixelWidth(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, HORZRES);
        return ret;
    }

    /**
    * Get display's pixel height.
    */
    IZ_UINT CEnvironment::GetPixelHeight(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, VERTRES);
        return ret;
    }

    /**
    * Get display' physical width.
    */
    IZ_UINT CEnvironment::GetPhysicalWidth(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, HORZSIZE);
        return ret;
    }

    /**
    * Get display' physical height.
    */
    IZ_UINT CEnvironment::GetPhysicalHeight(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, VERTSIZE);
        return ret;
    }

    /**
    * Get display's horizontal dpi.
    */
    IZ_UINT CEnvironment::GetHorizontalDPI(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, LOGPIXELSX);
        return ret;
    }

    /**
    * Get display's vertical dpi.
    */
    IZ_UINT CEnvironment::GetVerticalDPI(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, LOGPIXELSY);
        return ret;
    }

    /**
    * Get display's refresh rate.
    */
    IZ_UINT CEnvironment::GetRefreshRate(void* nativeWndHandle)
    {
        HDC hdc = (HDC)nativeWndHandle;
        IZ_ASSERT(hdc != IZ_NULL);
        IZ_UINT ret = ::GetDeviceCaps(hdc, VREFRESH);
        return ret;
    }
#endif
}   // namespace sys
}   // namespace izanagi
