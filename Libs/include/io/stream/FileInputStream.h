#if !defined(__URANUS_IO_FILE_INPUT_STREAM_H__)
#define __URANUS_IO_FILE_INPUT_STREAM_H__

#include "InputStream.h"

namespace uranus {
	/**
	* ���ȈՃt�@�C������
	*/
	class CFileInputStream : public IInputStream {
	public:
		inline CFileInputStream();
		inline ~CFileInputStream();

		NO_COPIABLE(CFileInputStream);

	public:
		// �t�@�C���I�[�v��
		inline UN_BOOL Open(UN_PCSTR pszFile);

		// �t�@�C���N���[�Y
		inline void Close();

		// �ǂݍ���
		inline UN_UINT Read(void* buf, UN_INT offset, size_t size);

		// �T�C�Y�擾
		inline UN_LONG GetSize();

		// ���݈ʒu�擾
		inline UN_LONG GetCurPos();

		// �V�[�N
		inline UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		// �I������
		inline void Finalize();

	protected:
		FILE* m_pFile;
		UN_LONG m_nSize;
		UN_LONG m_nPos;
	};

	// inline **************************************

	// �R���X�g���N�^
	CFileInputStream::CFileInputStream()
	{
		m_pFile = UN_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	// �f�X�g���N�^
	CFileInputStream::~CFileInputStream()
	{
		Close();
	}

	/**
	* �t�@�C���I�[�v��
	*/
	UN_BOOL CFileInputStream::Open(UN_PCSTR pszFile)
	{
		UN_BOOL ret = UN_FALSE;

		if (m_pFile != UN_NULL) {
			UN_ASSERT(UN_FALSE);
			Close();
		}

		fopen_s(&m_pFile, pszFile, "rb");
		ret = (m_pFile != UN_NULL);

		if (ret) {
			// �t�@�C���T�C�Y�擾
			fseek(m_pFile, 0, SEEK_END);
			m_nSize = ftell(m_pFile);
			fseek(m_pFile, 0, SEEK_SET);
		}

		return ret;
	}

	/**
	* �t�@�C���N���[�Y
	*/
	void CFileInputStream::Close()
	{
		if (m_pFile != UN_NULL) {
			fclose(m_pFile);
		}

		m_pFile = UN_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	/**
	* �ǂݍ���
	*/
	UN_UINT CFileInputStream::Read(void* buf, UN_INT offset, size_t size)
	{
		UN_ASSERT(m_pFile != UN_NULL);

		if (offset > 0) {
			VRETURN(Seek(offset, E_IO_STREAM_SEEK_POS_CUR));
		}
		UN_UINT ret = (UN_UINT)fread(buf, 1, size, m_pFile);

		if (ret == size) {
			// TODO
			// �t�@�C���ʒu�X�V
			m_nPos = m_nPos + (UN_INT)size;
		}
		else {
			m_nPos = m_nSize;
		}

		return ret;
	}

	/**
	* �T�C�Y�擾
	*/
	UN_LONG CFileInputStream::GetSize()
	{
		return m_nSize;
	}

	/**
	* ���݈ʒu�擾
	*/
	UN_LONG CFileInputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* �V�[�N
	*/
	UN_BOOL CFileInputStream::Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		UN_ASSERT(m_pFile != UN_NULL);

		UN_UINT nPos = SEEK_SET;
		UN_LONG offset = static_cast<UN_LONG>(nOffset);

		switch (nSeekPos) {
		case E_IO_STREAM_SEEK_POS_START:
			nPos = SEEK_SET;
			offset = CLAMP(offset, 0, m_nSize);
			break;
		case E_IO_STREAM_SEEK_POS_CUR:
			nPos = SEEK_CUR;
			offset = CLAMP(offset, -m_nPos, m_nSize - m_nPos);
			break;
		case E_IO_STREAM_SEEK_POS_END:
			nPos = SEEK_END;
			offset = CLAMP(offset, -m_nSize, 0);
			break;
		}

		// TODO
		UN_ASSERT(IS_IN_BOUND(offset, UN_INT32_MIN, UN_INT32_MAX));

		UN_BOOL ret = UN_TRUE;
		if (nOffset != 0) {
			// �V�[�N
			ret = (fseek(m_pFile, static_cast<UN_INT>(offset), nPos) == 0);

			if (ret) {
				// ���݈ʒu�X�V
				switch (nSeekPos) {
				case E_IO_STREAM_SEEK_POS_START:
					m_nPos = offset;
					break;
				case E_IO_STREAM_SEEK_POS_CUR:
					m_nPos += offset;
					break;
				case E_IO_STREAM_SEEK_POS_END:
					m_nPos = m_nSize + offset;
					break;
				}
			}
		}
		UN_ASSERT(ret);

		return ret;
	}

	/**
	* �I������
	*/
	void CFileInputStream::Finalize()
	{
		Close();
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_FILE_INPUT_STREAM_H__)
