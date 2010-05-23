#if !defined(__URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__)
#define __URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__

#include "unDefs.h"
#include "std/container/StdList.h"
#include "MemoryAllocator.h"

namespace uranus {
	/**
	* デフォルトメモリアロケータ
	*/
	class CStandardMemoryAllocator : public CDebugMemoryAllocator {
	protected:
		struct SHeapHeader;

	public:
		CStandardMemoryAllocator();
		CStandardMemoryAllocator(UN_UINT nBufSize, void* pBuf);

		~CStandardMemoryAllocator();

		NO_COPIABLE(CStandardMemoryAllocator);

	public:
		// 初期化
		UN_BOOL Init(UN_UINT nBufSize, void* pBuf);

		// メモリ確保
		void* Alloc(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);
		void* AllocZero(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		// メモリ確保(アライン有り)
		void* AllocAlign(size_t size, UN_UINT align, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);
		void* AllocAlignZero(size_t size, UN_UINT align, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		void* Realloc(void* ptr, size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		// メモリ解放
		UN_BOOL Free(void* data);
		
		UN_UINT GetSize();
		UN_UINT GetAllocatedSize();
		UN_UINT GetFreedSize();

		// ダンプ
		void Dump();

	protected:
		// メモリ確保
		void* AllocInternal(
			size_t size,
			const UN_CHAR* file,
			UN_UINT line);

		// メモリ確保(アライン有り)
		void* AllocInternal(
			size_t size,
			UN_UINT align,
			const UN_CHAR* file,
			UN_UINT line);

		// フリーリストに挿入
		void InsertFreeList(SHeapHeader* pFree);

		// Realloc可能なフリーのメモリ領域を探す
		SHeapHeader* FindReallocableFreedHeapHeader(void* ptr, size_t size);

	protected:
		// SHeapHeader探索
		inline SHeapHeader* FindAllocatedHeapHeader(void* ptr);

		// SHeapHeader確保
		inline SHeapHeader* GetHeapHeader(UN_UINT8* buf);

	protected:
		UN_UINT m_nBufSize;
		UN_UINT8* m_pBuf;

		CStdSimpleList<SHeapHeader> m_FreeList;
		CStdSimpleList<SHeapHeader> m_AllocList;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__)
