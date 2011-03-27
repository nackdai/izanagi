#include <windows.h>
#include <winbase.h>
#include "system/SysCpu.h"

using namespace izanagi;

/**
* Return CPU num.
*/
IZ_UINT CSysCpu::GetCpuNum()
{
	SYSTEM_INFO info;
	::GetSystemInfo(&info);
	
	return info.dwNumberOfProcessors;
}
