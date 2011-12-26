#if !defined(__IZANAGI_IO_FILE_OUTPUT_STREAM_H__)
#define __IZANAGI_IO_FILE_OUTPUT_STREAM_H__

#include "OutputStream.h"

namespace izanagi {
	/**
	*/
	class CFileOutputStream : public IOutputStream {
	public:
		inline CFileOutputStream();
		inline ~CFileOutputStream();

		NO_COPIABLE(CFileOutputStream);

	public:
		inline IZ_BOOL Open(IZ_PCSTR pszPath);

		inline void Finalize();

		// 出力
		inline IZ_UINT Write(const void* buf, IZ_UINT offset, size_t size);

		// サイズ取得
		inline IZ_UINT GetSize();

		// 現在位置取得
		inline IZ_UINT GetCurPos();

		// シーク
		inline IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		inline IZ_BOOL IsValid();

		IZ_BOOL IsEnableOutput() { return IsValid(); }

	private:
		FILE* m_pFile;
		IZ_UINT m_nPos;
	};

	// inline *******************************

	// コンストラクタ
	CFileOutputStream::CFileOutputStream()
	{
		m_pFile = IZ_NULL;
		m_nPos = 0;
	}

	// デストラクタ
	CFileOutputStream::~CFileOutputStream()
	{
		Finalize();
	}

	/**
	* ファイルオープン
	*/
	IZ_BOOL CFileOutputStream::Open(IZ_PCSTR pszFile)
	{
		IZ_BOOL ret = IZ_FALSE;

		if (m_pFile != IZ_NULL) {
			IZ_ASSERT(IZ_FALSE);
			Finalize();
		}

		fopen_s(&m_pFile, pszFile, "wb");
		ret = (m_pFile != IZ_NULL);

		return ret;
	}

	/**
	* ファイルクローズ
	*/
	void CFileOutputStream::Finalize()
	{
		if (m_pFile != IZ_NULL) {
			fclose(m_pFile);
		}

		m_pFile = IZ_NULL;
		m_nPos = 0;
	}

	/**
	* 出力
	*/
	IZ_UINT CFileOutputStream::Write(const void* buf, IZ_UINT offset, size_t size)
	{
		IZ_ASSERT(m_pFile != IZ_NULL);
		IZ_ASSERT(size > 0);

		if (offset > 0) {
			Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
		}

		IZ_UINT ret = (IZ_UINT)fwrite(buf, 1, size, m_pFile);
		IZ_ASSERT(ret == size);

		if (ret == size) {
			m_nPos += offset + size;
		}

		return ret;
	}

	/**
	* サイズ取得
	*/
	IZ_UINT CFileOutputStream::GetSize()
	{
		IZ_ASSERT(m_pFile != IZ_NULL);

		IZ_UINT ret = ftell(m_pFile);
		return ret;
	}

	/**
	* 現在位置取得
	*/
	IZ_UINT CFileOutputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* シーク
	*/
	IZ_BOOL CFileOutputStream::Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		IZ_ASSERT(m_pFile != IZ_NULL);

		IZ_UINT nPos = SEEK_SET;

		switch (nSeekPos) {
		case E_IO_STREAM_SEEK_POS_START:
			nPos = SEEK_SET;
			break;
		case E_IO_STREAM_SEEK_POS_CUR:
			nPos = SEEK_CUR;
			break;
		case E_IO_STREAM_SEEK_POS_END:
			// 出力時にはファイル終端は存在しないので・・・
			VRETURN(IZ_FALSE);
			break;
		}

		IZ_BOOL ret = IZ_TRUE;
		{
			// シーク
			ret = (fseek(m_pFile, nOffset, nPos) == 0);

			if (ret) {
				// 現在位置更新
				switch (nSeekPos) {
				case E_IO_STREAM_SEEK_POS_START:
					m_nPos = nOffset;
					break;
				case E_IO_STREAM_SEEK_POS_CUR:
					m_nPos += nOffset;
					break;
				case E_IO_STREAM_SEEK_POS_END:
					VRETURN(IZ_FALSE);
					break;
				}
			}
		}
		IZ_ASSERT(ret);

		return ret;
	}

	IZ_BOOL CFileOutputStream::IsValid()
	{
		return (m_pFile != NULL);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_FILE_OUTPUT_STREAM_H__)
