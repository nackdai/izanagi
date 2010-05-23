#if !defined(__URANUS_IO_FILE_OUTPUT_STREAM_H__)
#define __URANUS_IO_FILE_OUTPUT_STREAM_H__

#include "OutputStream.h"

namespace uranus {
	/**
	*/
	class CFileOutputStream : public IOutputStream {
	public:
		inline CFileOutputStream();
		inline ~CFileOutputStream();

		NO_COPIABLE(CFileOutputStream);

	public:
		inline UN_BOOL Open(UN_PCSTR pszPath);

		inline void Finalize();

		// �o��
		inline UN_UINT Write(const void* buf, UN_UINT offset, size_t size);

		// �T�C�Y�擾
		inline UN_UINT GetSize();

		// ���݈ʒu�擾
		inline UN_UINT GetCurPos();

		// �V�[�N
		inline UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		inline UN_BOOL IsValid();

	private:
		FILE* m_pFile;
		UN_UINT m_nPos;
	};

	// inline *******************************

	// �R���X�g���N�^
	CFileOutputStream::CFileOutputStream()
	{
		m_pFile = UN_NULL;
		m_nPos = 0;
	}

	// �f�X�g���N�^
	CFileOutputStream::~CFileOutputStream()
	{
		Finalize();
	}

	/**
	* �t�@�C���I�[�v��
	*/
	UN_BOOL CFileOutputStream::Open(UN_PCSTR pszFile)
	{
		UN_BOOL ret = UN_FALSE;

		if (m_pFile != UN_NULL) {
			UN_ASSERT(UN_FALSE);
			Finalize();
		}

		fopen_s(&m_pFile, pszFile, "wb");
		ret = (m_pFile != UN_NULL);

		return ret;
	}

	/**
	* �t�@�C���N���[�Y
	*/
	void CFileOutputStream::Finalize()
	{
		if (m_pFile != UN_NULL) {
			fclose(m_pFile);
		}

		m_pFile = UN_NULL;
		m_nPos = 0;
	}

	/**
	* �o��
	*/
	UN_UINT CFileOutputStream::Write(const void* buf, UN_UINT offset, size_t size)
	{
		UN_ASSERT(m_pFile != UN_NULL);
		UN_ASSERT(size > 0);

		if (offset > 0) {
			Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
		}

		UN_UINT ret = (UN_UINT)fwrite(buf, 1, size, m_pFile);
		UN_ASSERT(ret == size);

		if (ret == size) {
			m_nPos += offset + size;
		}

		return ret;
	}

	/**
	* �T�C�Y�擾
	*/
	UN_UINT CFileOutputStream::GetSize()
	{
		UN_ASSERT(m_pFile != UN_NULL);

		UN_UINT ret = ftell(m_pFile);
		return ret;
	}

	/**
	* ���݈ʒu�擾
	*/
	UN_UINT CFileOutputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* �V�[�N
	*/
	UN_BOOL CFileOutputStream::Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		UN_ASSERT(m_pFile != UN_NULL);

		UN_UINT nPos = SEEK_SET;

		switch (nSeekPos) {
		case E_IO_STREAM_SEEK_POS_START:
			nPos = SEEK_SET;
			break;
		case E_IO_STREAM_SEEK_POS_CUR:
			nPos = SEEK_CUR;
			break;
		case E_IO_STREAM_SEEK_POS_END:
			// �o�͎��ɂ̓t�@�C���I�[�͑��݂��Ȃ��̂ŁE�E�E
			VRETURN(UN_FALSE);
			break;
		}

		UN_BOOL ret = UN_TRUE;
		{
			// �V�[�N
			ret = (fseek(m_pFile, nOffset, nPos) == 0);

			if (ret) {
				// ���݈ʒu�X�V
				switch (nSeekPos) {
				case E_IO_STREAM_SEEK_POS_START:
					m_nPos = nOffset;
					break;
				case E_IO_STREAM_SEEK_POS_CUR:
					m_nPos += nOffset;
					break;
				case E_IO_STREAM_SEEK_POS_END:
					VRETURN(UN_FALSE);
					break;
				}
			}
		}
		UN_ASSERT(ret);

		return ret;
	}

	UN_BOOL CFileOutputStream::IsValid()
	{
		return (m_pFile != NULL);
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_FILE_OUTPUT_STREAM_H__)
