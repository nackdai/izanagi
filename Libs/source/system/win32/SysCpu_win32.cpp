#include <windows.h>
#include <winbase.h>
#include "system/SysCpu.h"

namespace izanagi
{
namespace sys
{
    /**
    * Return CPU num.
    */
    IZ_UINT CSysCpu::GetCpuNum()
    {
        SYSTEM_INFO info;
        ::GetSystemInfo(&info);
    
        return info.dwNumberOfProcessors;
    }
}   // namespace sys
}   // namespace izanagi
