#if !defined(__URANUS_IO_MEMORY_INPUT_STREAM_H__)
#define __URANUS_IO_MEMORY_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"

namespace uranus {
	/**
	* ���ȈՃ��������̓X�g���[��
	*/
	class CMemoryInputStream : public CBufferInputStream {
	public:
		CMemoryInputStream() {}
		inline CMemoryInputStream(const void* buf, UN_UINT size);
		
		~CMemoryInputStream() {}

	public:
		// ������
		inline void Init(const void* buf, size_t size);

		// �I������
		inline void Finalize();
	};

	// inline ********************************

	// �R���X�g���N�^
	CMemoryInputStream::CMemoryInputStream(const void* buf, UN_UINT size)
	{
		Init(buf, size);
	}

	/**
	* ������
	*/
	void CMemoryInputStream::Init(const void* buf, size_t size)
	{
		m_pBuffer = reinterpret_cast<const UN_UINT8*>(buf);
		m_nSize = static_cast<UN_UINT>(size);
		m_nPos = 0;
	}

	/**
	* �I������
	*/
	void CMemoryInputStream::Finalize()
	{
		m_pBuffer = UN_NULL;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_MEMORY_INPUT_STREAM_H__)
