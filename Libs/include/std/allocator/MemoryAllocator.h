#if !defined(__IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
#define __IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__

#include "izDefs.h"

#define ALLOC(a, size)		(a)->Alloc(size, __FILE__, __LINE__)
#define ALLOC_ZERO(a, size)	(a)->AllocZero(size, __FILE__, __LINE__)
#define REALLOC(a, p, size)	(a)->Realloc(p, size, __FILE__, __LINE__)
#if __IZ_DEBUG__
    #define FREE(a, mem)		if (mem != IZ_NULL) { IZ_ASSERT(a != IZ_NULL); (a)->Free(mem); }
#else
    #define FREE(a, mem)		if ((mem != IZ_NULL) && ((a) != IZ_NULL)) { (a)->Free(mem); }
#endif

namespace izanagi {
	/** メモリアロケータインターフェース.
	 */
	class IMemoryAllocator {
	protected:
		IMemoryAllocator() {}
		virtual ~IMemoryAllocator() {}

		NO_COPIABLE(IMemoryAllocator);

	public:
		/** メモリ確保.
		 */
		virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;

		/** メモリを確保して、ゼロで埋める.
		 */
		virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;

		/** メモリ再確保.
		 */
		virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL) = 0;

		/** メモリ解放.
		 */
		virtual IZ_BOOL Free(void* data) = 0;

		/** 確保可能な総メモリ量を取得.
		 */
		virtual IZ_UINT GetSize() = 0;

		/** 確保中メモリ量を取得.
		 */
		virtual IZ_UINT GetAllocatedSize() = 0;

		/** 確保されていないメモリ量を取得.
		 */
		virtual IZ_UINT GetFreedSize() = 0;

		/** メモリ確保状況のダンプ.
		 */
		virtual IZ_BOOL Dump()
		{
			// Nothing is done.
			return IZ_TRUE;
		}

		/** メモリ確保時にアサートで停止するインデックスを指定する.
		 */
		virtual void SetAllocAssert(IZ_UINT64 idx)
		{
			// Nothing is done...
		}

	protected:
		/** メモリ確保回数を増やす.
		 */
		virtual void IncrementAllocCnt()
		{
			// Nothing is done...
		}

		/** メモリ確保回数を取得.
		 */
		virtual IZ_UINT64 GetAllocCnt()
		{
			return IZ_UINT64_MAX;
		}

	protected:
		struct SHeapHeaderDummy {
			void ClearDebugInfo() {}
			void SetDebugInfo(IZ_UINT idx, IZ_UINT line, const IZ_CHAR* file) {}
			void SetDebugInfo(IZ_UINT line, const IZ_CHAR* file) {}
			void Dump() {}
		};
	};

	/** デバッグ用メモリアロケータ共通ベースクラス.
	 */
	class CDebugMemoryAllocator : public IMemoryAllocator {
	protected:
		CDebugMemoryAllocator()
		{
			m_nDebugIdx = IZ_UINT64_MAX;
			m_nAllocCnt = 0;
		}
		virtual ~CDebugMemoryAllocator() {}

	protected:
		// メモリ確保回数を増やす.
		virtual void IncrementAllocCnt()
		{
			IZ_ASSERT(m_nDebugIdx != m_nAllocCnt);
			++m_nAllocCnt;
		}

		// メモリ確保回数を取得.
		virtual IZ_UINT64 GetAllocCnt()
		{
			return m_nAllocCnt;
		}

	public:
		// メモリ確保時にアサートで停止するインデックスを指定する.
		virtual void SetAllocAssert(IZ_UINT64 idx)
		{
			m_nDebugIdx = idx;
		}

	protected:
		// アサートで停止するインデックス
		IZ_UINT64 m_nDebugIdx;

		// メモリ確保回数
		IZ_UINT64 m_nAllocCnt;

		// デバッグ情報を持つヒープヘッダ
		struct SHeapHeaderWithDebugInfo {
			IZ_UINT idx;			// インデックス
			IZ_UINT line;			// ファイル内行位置
			const IZ_CHAR* file;	// ファイル名

			void ClearDebugInfo()
			{
				idx = 0;
				line = 0;
				file = IZ_NULL;
			}

			void SetDebugInfo(IZ_UINT idx, IZ_UINT line, const IZ_CHAR* file)
			{
				this->idx = idx;
				this->line = line;
				this->file = file;
			}

			void SetDebugInfo(IZ_UINT line, const IZ_CHAR* file)
			{
				this->line = line;
				this->file = file;
			}

			void Dump()
			{
				IZ_PRINTF(
					"idx[%d] file[%s] line[%d]",
					idx, file, line);
			}
		};
	};
}	// namespace izanagi

#ifdef __IZ_DEBUG__
	#define IZ_MEMORY_ALLOCATOR_BASE	izanagi::CDebugMemoryAllocator
	#define IZ_DEBUG_HEAP_HEADER_BASE	izanagi::CDebugMemoryAllocator::SHeapHeaderWithDebugInfo
#else
	#define IZ_MEMORY_ALLOCATOR_BASE	izanagi::IMemoryAllocator
	#define IZ_DEBUG_HEAP_HEADER_BASE	izanagi::IMemoryAllocator::SHeapHeaderDummy
#endif

#endif	// #if !defined(__IZANAGI_STD_MEMORY_ALLOCATOR_INTERFACE_H__)
