#include "system/SysEnvironment.h"

namespace izanagi
{
namespace sys
{
    // Return CPU num.
    IZ_UINT CEnvironment::GetProcessorNum()
    {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
    
        return info.dwNumberOfProcessors;
    }

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

    IZ_UINT CEnvironment::GetYear()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wYear;
    }

    IZ_UINT CEnvironment::GetMonth()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wMonth;
    }

    IZ_UINT CEnvironment::GetDay()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wDay;
    }

    IZ_UINT CEnvironment::GetHour()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wHour;
    }

    IZ_UINT CEnvironment::GetMinute()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wMinute;
    }

    IZ_UINT CEnvironment::GetSecond()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wSecond;
    }

    IZ_UINT CEnvironment::GetMilliseconds()
    {
        SYSTEMTIME st;
        GetSystemTime(&st);

        return st.wMilliseconds;
    }
}   // namespace sys
}   // namespace izanagi
