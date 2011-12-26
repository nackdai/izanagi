#if !defined(__IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "std/container/StdList.h"
#include "MemoryAllocator.h"

namespace izanagi {
	/**
	* デフォルトメモリアロケータ
	*/
	class CStandardMemoryAllocator : public CDebugMemoryAllocator {
	protected:
		struct SHeapHeader;

	public:
		CStandardMemoryAllocator();
		CStandardMemoryAllocator(IZ_UINT nBufSize, void* pBuf);

		~CStandardMemoryAllocator();

		NO_COPIABLE(CStandardMemoryAllocator);

	public:
		// 初期化
		IZ_BOOL Init(IZ_UINT nBufSize, void* pBuf);

		// メモリ確保
		void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);
		void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

		// メモリ確保(アライン有り)
		void* AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);
		void* AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

		void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

		// メモリ解放
		IZ_BOOL Free(void* data);
		
		IZ_UINT GetSize();
		IZ_UINT GetAllocatedSize();
		IZ_UINT GetFreedSize();

		// ダンプ
		void Dump();

	protected:
		// メモリ確保
		void* AllocInternal(
			size_t size,
			const IZ_CHAR* file,
			IZ_UINT line);

		// メモリ確保(アライン有り)
		void* AllocInternal(
			size_t size,
			IZ_UINT align,
			const IZ_CHAR* file,
			IZ_UINT line);

		// フリーリストに挿入
		void InsertFreeList(SHeapHeader* pFree);

		// Realloc可能なフリーのメモリ領域を探す
		SHeapHeader* FindReallocableFreedHeapHeader(void* ptr, size_t size);

	protected:
		// SHeapHeader探索
		inline SHeapHeader* FindAllocatedHeapHeader(void* ptr);

		// SHeapHeader確保
		inline SHeapHeader* GetHeapHeader(IZ_UINT8* buf);

	protected:
		IZ_UINT m_nBufSize;
		IZ_UINT8* m_pBuf;

		CStdSimpleList<SHeapHeader> m_FreeList;
		CStdSimpleList<SHeapHeader> m_AllocList;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__)
