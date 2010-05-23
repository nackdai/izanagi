#include <windows.h>
#include <winbase.h>
#include "system/SysCpu.h"

using namespace uranus;

/**
* Return CPU num.
*/
UN_UINT CSysCpu::GetCpuNum()
{
	SYSTEM_INFO info;
	::GetSystemInfo(&info);
	
	return info.dwNumberOfProcessors;
}
