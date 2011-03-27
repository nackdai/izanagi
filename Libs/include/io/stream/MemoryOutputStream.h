#if !defined(__IZANAGI_IO_MEMORY_OUTPUT_STREAM_H__)
#define __IZANAGI_IO_MEMORY_OUTPUT_STREAM_H__

#include "OutputStream.h"

namespace izanagi {
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
		inline IZ_UINT Write(const void* buf, IZ_UINT offset, size_t size);

		// サイズ取得
		inline IZ_UINT GetSize();

		// 現在位置取得
		inline IZ_UINT GetCurPos();

		// シーク
		inline IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

		inline IZ_BOOL IsValid();

		IZ_BOOL IsEnableOutput() { return (IsValid() && (m_nPos < m_nSize)); }

		// バッファ取得
		inline void* GetBuffer();
		inline void* GetBufferCurPos();

		void Reset() { m_nPos = 0; }

	private:
		IZ_UINT8* m_pBuffer;

		IZ_INT m_nSize;
		IZ_INT m_nPos;
	};

	// inline *******************************

	// コンストラクタ
	CMemoryOutputStream::CMemoryOutputStream()
	{
		Init(IZ_NULL, 0);
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
		m_pBuffer = reinterpret_cast<IZ_UINT8*>(buf);
		m_nSize = static_cast<IZ_UINT>(size);
		m_nPos = 0;
	}

	/**
	* 終了処理
	*/
	void CMemoryOutputStream::Finalize()
	{
		m_pBuffer = IZ_NULL;
		m_nSize = 0;
		m_nPos = 0;
	}

	/**
	* 出力
	*/
	IZ_UINT CMemoryOutputStream::Write(const void* buf, IZ_UINT offset, size_t size)
	{
		IZ_ASSERT(m_pBuffer != IZ_NULL);
		IZ_ASSERT(size > 0);

		if (offset > 0) {
			Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
		}

		IZ_INT ret = 0;

		if (m_nPos < m_nSize) {
			ret = (IZ_UINT)size;
			if (m_nPos + ret > m_nSize) {
				ret = m_nSize - m_nPos;
			}
			if (ret > 0) {
				memcpy(m_pBuffer + m_nPos, buf, ret);
				m_nPos += ret;
			}
		}
#if 0
		else {
			IZ_ASSERT(IZ_FALSE);
		}
#endif

		return (IZ_UINT)ret;
	}

	/**
	* サイズ取得
	*/
	IZ_UINT CMemoryOutputStream::GetSize()
	{
		return m_nSize;
	}

	/**
	* 現在位置取得
	*/
	IZ_UINT CMemoryOutputStream::GetCurPos()
	{
		return m_nPos;
	}

	/**
	* シーク
	*/
	IZ_BOOL CMemoryOutputStream::Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
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

		return IZ_TRUE;
	}

	IZ_BOOL CMemoryOutputStream::IsValid()
	{
		return (m_pBuffer != IZ_NULL);
	}

	/**
	* バッファ取得
	*/
	void* CMemoryOutputStream::GetBuffer()
	{
		return m_pBuffer;
	}

	/**
	* バッファ取得
	*/
	void* CMemoryOutputStream::GetBufferCurPos()
	{
		IZ_ASSERT(m_nPos < m_nSize);
		IZ_ASSERT(m_pBuffer != IZ_NULL);
		return (m_pBuffer + m_nPos);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_MEMORY_OUTPUT_STREAM_H__)
