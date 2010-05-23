#if !defined(__URANUS_IO_HEAP_INPUT_STREAM_H__)
#define __URANUS_IO_HEAP_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"
#include "std/allocator/MemoryAllocator.h"

namespace uranus {
	/**
	* ���ȈՃ��������̓X�g���[��
	* �i�������A�A���P�[�^���烁�����m�ہj
	*/
	class CHeapInputStream : public CBufferInputStream {
	public:
		inline CHeapInputStream();
		inline ~CHeapInputStream();

	public:
		// ������
		inline UN_BOOL Init(IMemoryAllocator* pAllocator, UN_UINT size);

		// �I������
		inline void Finalize();

	protected:
		IMemoryAllocator* m_pAllocator;
	};

	// inline ********************************

	// �R���X�g���N�^
	CHeapInputStream::CHeapInputStream()
	{
		m_pAllocator = UN_NULL;
	}

	// �f�X�g���N�^
	CHeapInputStream::~CHeapInputStream()
	{
		Finalize();
	}

	/**
	* ������
	*/
	UN_BOOL CHeapInputStream::Init(IMemoryAllocator* pAllocator, UN_UINT size)
	{
		if ((m_pAllocator != UN_NULL) && (m_pBuffer != UN_NULL)) {
			FREE(m_pAllocator, const_cast<UN_UINT8*>(m_pBuffer));
			m_pBuffer = UN_NULL;
		}

		m_pAllocator = pAllocator;

		// �������m��
		m_pBuffer = static_cast<UN_UINT8*>(ALLOC_ZERO(m_pAllocator, size));
		VRETURN(m_pBuffer != UN_NULL);

		m_nSize = size;
		m_nPos = 0;

		return UN_TRUE;
	}

	/**
	* �I������
	*/
	void CHeapInputStream::Finalize()
	{
		if ((m_pAllocator != UN_NULL) && (m_pBuffer != UN_NULL)) {
			FREE(m_pAllocator, const_cast<UN_UINT8*>(m_pBuffer));
		}
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_HEAP_INPUT_STREAM_H__)
