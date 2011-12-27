#if !defined(__IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
#define __IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__

#include "izDefs.h"

#define ALLOC(a, size)		(a)->Alloc(size, __FILE__, __LINE__)
#define ALLOC_ZERO(a, size)	(a)->AllocZero(size, __FILE__, __LINE__)
#define REALLOC(a, p, size)	(a)->Realloc(p, size, __FILE__, __LINE__)
#define FREE(a, mem)		if ((mem != IZ_NULL) && ((a) != IZ_NULL)) { (a)->Free(mem); }

namespace izanagi {
	/**
	* メモリアロケータインターフェース
	*/
	class IMemoryAllocator {
	protected:
		IMemoryAllocator() {}
		virtual ~IMemoryAllocator() {}

		NO_COPIABLE(IMemoryAllocator);

	public:
		virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;
		virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;

		virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;

		virtual IZ_BOOL Free(void* data) = 0;

		virtual void SetDebugAllocAssert(IZ_UINT64 idx) = 0;
		virtual IZ_UINT GetSize() = 0;
		virtual IZ_UINT GetAllocatedSize() = 0;
		virtual IZ_UINT GetFreedSize() = 0;
		virtual void Dump() = 0;
	};

	/**
	* デバッグ用メモリアロケータ共通ベースクラス
	*/
	class CDebugMemoryAllocator : public IMemoryAllocator {
	protected:
		CDebugMemoryAllocator()
		{
#ifdef __IZ_DEBUG__
			m_nDebugIdx = IZ_UINT64_MAX;
			m_nAllocCnt = 0;
#endif	// #ifdef __IZ_DEBUG__
		}
		virtual ~CDebugMemoryAllocator() {}

	protected:
		void IncrementAllocCnt()
		{
#ifdef __IZ_DEBUG__
			IZ_ASSERT(m_nDebugIdx != m_nAllocCnt);
			++m_nAllocCnt;
#endif	// #ifdef __IZ_DEBUG__
		}

		IZ_UINT64 GetAllocCnt()
		{
#ifdef __IZ_DEBUG__
			return m_nAllocCnt;
#else
			return 0;
#endif
		}

	public:
		void SetDebugAllocAssert(IZ_UINT64 idx)
		{
#ifdef __IZ_DEBUG__
			m_nDebugIdx = idx;
#endif
		}

	protected:
#ifdef __IZ_DEBUG__
		IZ_UINT64 m_nDebugIdx;
		IZ_UINT64 m_nAllocCnt;
#endif	// #ifdef __IZ_DEBUG__

		struct SHeapHeaderWithDebugInfo {
#ifdef __IZ_DEBUG__
			IZ_UINT idx;
			IZ_UINT line;
			const IZ_CHAR* file;
#endif	// #ifdef __IZ_DEBUG__

			void ClearDebugInfo()
			{
#ifdef __IZ_DEBUG__
				idx = 0;
				line = 0;
				file = IZ_NULL;
#endif	// #ifdef __IZ_DEBUG__
			}

			void SetDebugInfo(IZ_UINT idx, IZ_UINT line, const IZ_CHAR* file)
			{
#ifdef __IZ_DEBUG__
				this->idx = idx;
				this->line = line;
				this->file = file;
#endif	// #ifdef __IZ_DEBUG__
			}

			void SetDebugInfo(IZ_UINT line, const IZ_CHAR* file)
			{
#ifdef __IZ_DEBUG__
				this->line = line;
				this->file = file;
#endif	// #ifdef __IZ_DEBUG__
			}
		};
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
