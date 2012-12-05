#if !defined(__IZANAGI_IO_FILE_INPUT_STREAM_H__)
#define __IZANAGI_IO_FILE_INPUT_STREAM_H__

#include "InputStream.h"

namespace izanagi {
	/**
	* 超簡易ファイル入力
	*/
	class CFileInputStream : public IInputStream {
	public:
		inline CFileInputStream();
		inline ~CFileInputStream();

		NO_COPIABLE(CFileInputStream);

	public:
		// ファイルオープン
		inline IZ_BOOL Open(IZ_PCSTR pszFile);

		// ファイルクローズ
		inline void Close();

		// 読み込み
		inline IZ_UINT Read(void* buf, IZ_INT offset, size_t size);

		// サイズ取得
		inline IZ_LONG GetSize();

		// 現在位置取得
		inline IZ_LONG GetCurPos();

		// シーク
		inline IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		// 終了処理
		inline void Finalize();

	protected:
		FILE* m_pFile;
		IZ_LONG m_nSize;
		IZ_LONG m_nPos;
	};

	// inline **************************************

	// コンストラクタ
	CFileInputStream::CFileInputStream()
	{
		m_pFile = IZ_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	// デストラクタ
	CFileInputStream::~CFileInputStream()
	{
		Close();
	}

	/**
	* ファイルオープン
	*/
	IZ_BOOL CFileInputStream::Open(IZ_PCSTR pszFile)
	{
		IZ_BOOL ret = IZ_FALSE;

		if (m_pFile != IZ_NULL) {
			IZ_ASSERT(IZ_FALSE);
			Close();
		}

		fopen_s(&m_pFile, pszFile, "rb");
		ret = (m_pFile != IZ_NULL);

		if (ret) {
			// ファイルサイズ取得
			fseek(m_pFile, 0, SEEK_END);
			m_nSize = ftell(m_pFile);
			fseek(m_pFile, 0, SEEK_SET);
		}

		return ret;
	}

	/**
	* ファイルクローズ
	*/
	void CFileInputStream::Close()
	{
		if (m_pFile != IZ_NULL) {
			fclose(m_pFile);
		}

		m_pFile = IZ_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	/**
	* 読み込み
	*/
	IZ_UINT CFileInputStream::Read(void* buf, IZ_INT offset, size_t size)
	{
		IZ_ASSERT(m_pFile != IZ_NULL);

		if (offset > 0) {
			VRETURN(Seek(offset, E_IO_STREAM_SEEK_POS_CUR));
		}
		IZ_UINT ret = (IZ_UINT)fread(buf, 1, size, m_pFile);

		if (ret == size) {
			// TODO
			// ファイル位置更新
			m_nPos = m_nPos + (IZ_INT)size;
		}
		else {
			m_nPos = m_nSize;
		}

		return ret;
	}

	/**
	* サイズ取得
	*/
	IZ_LONG CFileInputStream::GetSize()
	{
		return m_nSize;
	}

	/**
	* 現在位置取得
	*/
	IZ_LONG CFileInputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* シーク
	*/
	IZ_BOOL CFileInputStream::Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		IZ_ASSERT(m_pFile != IZ_NULL);

		IZ_UINT nPos = SEEK_SET;
		IZ_LONG offset = static_cast<IZ_LONG>(nOffset);

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
		IZ_ASSERT(IS_IN_BOUND(offset, IZ_INT32_MIN, IZ_INT32_MAX));

		IZ_BOOL ret = IZ_TRUE;
		if (nOffset != 0) {
			// シーク
			ret = (fseek(m_pFile, static_cast<IZ_INT>(offset), nPos) == 0);

			if (ret) {
				// 現在位置更新
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
		IZ_ASSERT(ret);

		return ret;
	}

	/**
	* 終了処理
	*/
	void CFileInputStream::Finalize()
	{
		Close();
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_FILE_INPUT_STREAM_H__)
