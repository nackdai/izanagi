#if !defined(__URANUS_IO_MEMORY_OUTPUT_STREAM_H__)
#define __URANUS_IO_MEMORY_OUTPUT_STREAM_H__

#include "OutputStream.h"

namespace uranus {
	/**
	*/
	class CMemoryOutputStream : public IOutputStream {
	public:
		inline CMemoryOutputStream();
		inline CMemoryOutputStream(void* buf, size_t size);

		~CMemoryOutputStream()
		{
			Finalize();
		}

		NO_COPIABLE(CMemoryOutputStream);

	public:
		// 初期化
		inline void Init(void* buf, size_t size);

		inline void Finalize();

		// 出力
		inline UN_UINT Write(const void* buf, UN_UINT offset, size_t size);

		// サイズ取得
		inline UN_UINT GetSize();

		// 現在位置取得
		inline UN_UINT GetCurPos();

		// シーク
		inline UN_BOOL Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		inline UN_BOOL IsValid();

		// バッファ取得
		inline void* GetBuffer();

	private:
		UN_UINT8* m_pBuffer;

		UN_INT m_nSize;
		UN_INT m_nPos;
	};

	// inline *******************************

	// コンストラクタ
	CMemoryOutputStream::CMemoryOutputStream()
	{
		Init(UN_NULL, 0);
	}

	CMemoryOutputStream::CMemoryOutputStream(void* buf, size_t size)
	{
		Init(buf, size);
	}

	/**
	* 初期化
	*/
	void CMemoryOutputStream::Init(void* buf, size_t size)
	{
		m_pBuffer = reinterpret_cast<UN_UINT8*>(buf);
		m_nSize = static_cast<UN_UINT>(size);
		m_nPos = 0;
	}

	/**
	* 終了処理
	*/
	void CMemoryOutputStream::Finalize()
	{
		m_pBuffer = UN_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	/**
	* 出力
	*/
	UN_UINT CMemoryOutputStream::Write(const void* buf, UN_UINT offset, size_t size)
	{
		UN_ASSERT(m_pBuffer != UN_NULL);
		UN_ASSERT(size > 0);

		if (offset > 0) {
			Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
		}

		UN_INT ret = 0;

		if (m_nPos < m_nSize) {
			ret = (UN_UINT)size;
			if (m_nPos + ret > m_nSize) {
				ret = m_nSize - m_nPos;
			}
			if (ret > 0) {
				memcpy(m_pBuffer + m_nPos, buf, ret);
				m_nPos += ret;
			}
		}

		return (UN_UINT)ret;
	}

	/**
	* サイズ取得
	*/
	UN_UINT CMemoryOutputStream::GetSize()
	{
		return m_nSize;
	}

	/**
	* 現在位置取得
	*/
	UN_UINT CMemoryOutputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* シーク
	*/
	UN_BOOL CMemoryOutputStream::Seek(UN_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
	{
		if (nOffset != 0) {
			switch (nSeekPos) {
			case E_IO_STREAM_SEEK_POS_START:
				nOffset = CLAMP(nOffset, 0, m_nSize);
				m_nPos = nOffset;
				break;
			case E_IO_STREAM_SEEK_POS_CUR:
				nOffset = CLAMP(nOffset, -m_nPos, m_nSize - m_nPos);
				m_nPos += nOffset;
				break;
			case E_IO_STREAM_SEEK_POS_END:
				nOffset = CLAMP(nOffset, 0, m_nSize);
				m_nPos = m_nSize - nOffset;
				break;
			}
		}

		return UN_TRUE;
	}

	UN_BOOL CMemoryOutputStream::IsValid()
	{
		return (m_pBuffer != UN_NULL);
	}

	/**
	* バッファ取得
	*/
	void* CMemoryOutputStream::GetBuffer()
	{
		return m_pBuffer;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_MEMORY_OUTPUT_STREAM_H__)
