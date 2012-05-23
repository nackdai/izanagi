#include "std/allocator/STLMemoryAllocator.h"

namespace izanagi
{
	IMemoryAllocator* STLMemoryAllocatorProxy::s_Allocator = IZ_NULL;
	IZ_ULONG STLMemoryAllocatorProxy::s_AllocatedSize = 0;
}
