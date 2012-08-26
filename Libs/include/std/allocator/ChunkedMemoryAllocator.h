#if !defined(__IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "std/collection/StdList.h"
#include "std/collection/StdSimpleList.h"
#include "MemoryAllocator.h"

namespace izanagi {
	/**
	*/
	class CChunkedMemoryAllocator
		: public IZ_MEMORY_ALLOCATOR_BASE
	{
	protected:
		struct SHeapHeader;

	public:
		CChunkedMemoryAllocator();
		CChunkedMemoryAllocator(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize);

		~CChunkedMemoryAllocator();

		NO_COPIABLE(CChunkedMemoryAllocator);

	public:
		// 初期化
		IZ_BOOL Init(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize);

		// メモリ確保
		void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);
		void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

		void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

		// メモリ解放
		IZ_BOOL Free(void* data);
		
		IZ_UINT GetSize();
		IZ_UINT GetAllocatedSize();
		IZ_UINT GetFreedSize();

		// ダンプ
		IZ_BOOL Dump();

	protected:
		// メモリ確保
		void* AllocInternal(
			size_t size,
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

		IZ_UINT m_nChunkSize;

		CStdSimpleList<SHeapHeader> m_FreeList;
		CStdSimpleList<SHeapHeader> m_AllocList;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__)
