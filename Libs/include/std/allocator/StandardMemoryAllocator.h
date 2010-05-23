#if !defined(__URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__)
#define __URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__

#include "unDefs.h"
#include "std/container/StdList.h"
#include "MemoryAllocator.h"

namespace uranus {
	/**
	* �f�t�H���g�������A���P�[�^
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
		// ������
		UN_BOOL Init(UN_UINT nBufSize, void* pBuf);

		// �������m��
		void* Alloc(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);
		void* AllocZero(size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		// �������m��(�A���C���L��)
		void* AllocAlign(size_t size, UN_UINT align, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);
		void* AllocAlignZero(size_t size, UN_UINT align, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		void* Realloc(void* ptr, size_t size, const UN_CHAR* file = UN_NULL, UN_UINT line = UN_NULL);

		// ���������
		UN_BOOL Free(void* data);
		
		UN_UINT GetSize();
		UN_UINT GetAllocatedSize();
		UN_UINT GetFreedSize();

		// �_���v
		void Dump();

	protected:
		// �������m��
		void* AllocInternal(
			size_t size,
			const UN_CHAR* file,
			UN_UINT line);

		// �������m��(�A���C���L��)
		void* AllocInternal(
			size_t size,
			UN_UINT align,
			const UN_CHAR* file,
			UN_UINT line);

		// �t���[���X�g�ɑ}��
		void InsertFreeList(SHeapHeader* pFree);

		// Realloc�\�ȃt���[�̃������̈��T��
		SHeapHeader* FindReallocableFreedHeapHeader(void* ptr, size_t size);

	protected:
		// SHeapHeader�T��
		inline SHeapHeader* FindAllocatedHeapHeader(void* ptr);

		// SHeapHeader�m��
		inline SHeapHeader* GetHeapHeader(UN_UINT8* buf);

	protected:
		UN_UINT m_nBufSize;
		UN_UINT8* m_pBuf;

		CStdSimpleList<SHeapHeader> m_FreeList;
		CStdSimpleList<SHeapHeader> m_AllocList;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STANDARD_MEMORY_ALLOCATOR_H__)
