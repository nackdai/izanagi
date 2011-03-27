#if !defined(__IZANAGI_IO_MEMORY_INPUT_STREAM_H__)
#define __IZANAGI_IO_MEMORY_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"

namespace izanagi {
	/**
	* 超簡易メモリ入力ストリーム
	*/
	class CMemoryInputStream : public CBufferInputStream {
	public:
		CMemoryInputStream() {}
		inline CMemoryInputStream(const void* buf, IZ_UINT size);
		
		~CMemoryInputStream() {}

	public:
		// 初期化
		inline void Init(const void* buf, size_t size);

		// 終了処理
		inline void Finalize();
	};

	// inline ********************************

	// コンストラクタ
	CMemoryInputStream::CMemoryInputStream(const void* buf, IZ_UINT size)
	{
		Init(buf, size);
	}

	/**
	* 初期化
	*/
	void CMemoryInputStream::Init(const void* buf, size_t size)
	{
		m_pBuffer = reinterpret_cast<const IZ_UINT8*>(buf);
		m_nSize = static_cast<IZ_UINT>(size);
		m_nPos = 0;
	}

	/**
	* 終了処理
	*/
	void CMemoryInputStream::Finalize()
	{
		m_pBuffer = IZ_NULL;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_MEMORY_INPUT_STREAM_H__)
