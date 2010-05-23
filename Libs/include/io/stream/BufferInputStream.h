#if !defined(__URANUS_IO_BUFFER_INPUT_STREAM_H__)
#define __URANUS_IO_BUFFER_INPUT_STREAM_H__

#include "InputStream.h"

namespace uranus {
	/**
	*/
	class CBufferInputStream : public IInputStream {
	protected:
		inline CBufferInputStream();
		virtual ~CBufferInputStream() {}

	public:
		// �ǂݍ���
		inline UN_UINT Read(void* buf, UN_INT offset, size_t size);

		// �T�C�Y�擾
		inline UN_LONG GetSize();

		// ���݈ʒu�擾
		inline UN_LONG GetCurPos();

		// �V�[�N
		inline UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		// �o�b�t�@�擾
		inline const void* GetBuffer();

	protected:
		const UN_UINT8* m_pBuffer;

		UN_LONG m_nSize;
		UN_LONG m_nPos;
	};

	// inline ********************************

	// �R���X�g���N�^
	CBufferInputStream::CBufferInputStream()
	{
		m_pBuffer = UN_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	/**
	* �ǂݍ���
	*/
	UN_UINT CBufferInputStream::Read(void* buf, UN_INT offset, size_t size)
	{
		UN_ASSERT(m_pBuffer != UN_NULL);

		if (offset > 0) {
			Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
		}

		UN_LONG ret = 0;

		if (m_nPos < m_nSize) {
			ret = (UN_INT)size;
			if (m_nPos + ret > m_nSize) {
				ret = m_nSize - m_nPos;
			}

			UN_ASSERT(IS_IN_BOUND(ret, 0, UN_UINT32_MAX));
			memcpy(buf, m_pBuffer + m_nPos, static_cast<size_t>(ret));

			m_nPos += (UN_UINT)size;
			m_nPos = MIN(m_nPos, m_nSize);
		}

		return (UN_UINT)ret;
	}

	/**
	* �T�C�Y�擾
	*/
	UN_LONG CBufferInputStream::GetSize()
	{
		return m_nSize;
	}

	/**
	* ���݈ʒu�擾
	*/
	UN_LONG CBufferInputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* �V�[�N
	*/
	UN_BOOL CBufferInputStream::Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		UN_LONG offset = static_cast<UN_LONG>(nOffset);

		if (nOffset != 0) {
			switch (nSeekPos) {
			case E_IO_STREAM_SEEK_POS_START:
				offset = CLAMP(offset, 0, m_nSize);
				m_nPos = nOffset;
				break;
			case E_IO_STREAM_SEEK_POS_CUR:
				offset = CLAMP(offset, -m_nPos, m_nSize - m_nPos);
				m_nPos += nOffset;
				break;
			case E_IO_STREAM_SEEK_POS_END:
				offset = CLAMP(offset, -m_nSize, 0);
				m_nPos = m_nSize + offset;
				break;
			}
		}

		return UN_TRUE;
	}

	/**
	* �o�b�t�@�擾
	*/
	const void* CBufferInputStream::GetBuffer()
	{
		return m_pBuffer;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_BUFFER_INPUT_STREAM_H__)
