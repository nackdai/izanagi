#include "std/allocator/SimpleMemoryAllocator.h"

namespace izanagi
{
	// メモリ確保
	void* CSimpleMemoryAllocator::Alloc(size_t size, const IZ_CHAR* file, IZ_UINT line)
	{
		void* ret = ::malloc(size);
		IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

	void* CSimpleMemoryAllocator::AllocZero(size_t size, const IZ_CHAR* file, IZ_UINT line)
	{
		void* ret = Alloc(size, file, line);
		if (ret != IZ_NULL)
		{
			memset(ret, 0, size);
		}
		return ret;
	}

	// メモリ確保(アライン有り)
	void* CSimpleMemoryAllocator::AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file, IZ_UINT line)
	{
		// TODO
		IZ_ASSERT(IZ_FALSE);
		return IZ_NULL;
	}

	void* CSimpleMemoryAllocator::AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file, IZ_UINT line)
	{
		void* ret = AllocAlign(size, align, file, line);
		if (ret != IZ_NULL)
		{
			memset(ret, 0, size);
		}
		return ret;
	}

	void* CSimpleMemoryAllocator::Realloc(void* ptr, size_t size, const IZ_CHAR* file, IZ_UINT line)
	{
		void* ret = ::realloc(ptr, size);
		IZ_ASSERT(ret != IZ_NULL);
		return ret;
	}

	// メモリ解放
	IZ_BOOL CSimpleMemoryAllocator::Free(void* data)
	{
		if (data != IZ_NULL)
		{
			::free(data);
			return IZ_TRUE;
		}
		return IZ_FALSE;
	}
}	// namespace izanagi
