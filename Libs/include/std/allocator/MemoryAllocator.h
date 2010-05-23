#if !defined(__URANUS_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
#define __URANUS_STD_MEMORY_ALLOCATOR_INTERFACE_H__

#include "unDefs.h"

#define ALLOC(a, size)		(a)->Alloc(size, __FILE__, __LINE__)
#define ALLOC_ZERO(a, size)	(a)->AllocZero(size, __FILE__, __LINE__)
#define REALLOC(a, p, size)	(a)->Realloc(p, size, __FILE__, __LINE__)
#define FREE(a, mem)		if ((mem != UN_NULL) && ((a) != UN_NULL)) { (a)->Free(mem); }

namespace uranus {
	/**
	* メモリアロケータインターフェース
	*/
	class IMemoryAllocator {
	protected:
		IMemoryAllocator() {}
		virtual ~IMemoryAllocator() {}

		NO_COPIABLE(IMemoryAllocator);

	public:
		virtual void* Alloc(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL) = 0;
		virtual void* AllocZero(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL) = 0;

		virtual void* Realloc(void* ptr, size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL) = 0;

		virtual UN_BOOL Free(void* data) = 0;

		virtual void SetDebugAllocAssert(UN_UINT64 idx) = 0;
		virtual UN_UINT GetSize() = 0;
		virtual UN_UINT GetAllocatedSize() = 0;
		virtual UN_UINT GetFreedSize() = 0;
		virtual void Dump() = 0;
	};

	/**
	* デバッグ用メモリアロケータ共通ベースクラス
	*/
	class CDebugMemoryAllocator : public IMemoryAllocator {
	protected:
		CDebugMemoryAllocator()
		{
#ifdef __UN_DEBUG__
			m_nDebugIdx = UN_UINT64_MAX;
			m_nAllocCnt = 0;
#endif	// #ifdef __UN_DEBUG__
		}
		virtual ~CDebugMemoryAllocator() {}

	protected:
		void IncrementAllocCnt()
		{
#ifdef __UN_DEBUG__
			UN_ASSERT(m_nDebugIdx != m_nAllocCnt);
			m_nAllocCnt++;
#endif	// #ifdef __UN_DEBUG__
		}

		UN_UINT64 GetAllocCnt()
		{
#ifdef __UN_DEBUG__
			return m_nAllocCnt;
#else
			return 0;
#endif
		}

	public:
		void SetDebugAllocAssert(UN_UINT64 idx)
		{
#ifdef __UN_DEBUG__
			m_nDebugIdx = idx;
#endif
		}

	protected:
#ifdef __UN_DEBUG__
		UN_UINT64 m_nDebugIdx;
		UN_UINT64 m_nAllocCnt;
#endif	// #ifdef __UN_DEBUG__

		struct SHeadHeaderWithDebugInfo {
#ifdef __UN_DEBUG__
			UN_UINT cnt;
			UN_UINT line;
			const UN_CHAR* file;
#endif	// #ifdef __UN_DEBUG__

			void ClearDebugInfo()
			{
#ifdef __UN_DEBUG__
				cnt = 0;
				line = 0;
				file = UN_NULL;
#endif	// #ifdef __UN_DEBUG__
			}

			void SetDebugInfo(UN_UINT cnt, UN_UINT line, const UN_CHAR* file)
			{
#ifdef __UN_DEBUG__
				this->cnt = cnt;
				this->line = line;
				this->file = file;
#endif	// #ifdef __UN_DEBUG__
			}

			void SetDebugInfo(UN_UINT line, const UN_CHAR* file)
			{
#ifdef __UN_DEBUG__
				this->line = line;
				this->file = file;
#endif	// #ifdef __UN_DEBUG__
			}
		};
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
