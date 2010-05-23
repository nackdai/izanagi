#if !defined(__URANUS_IO_MEMORY_INPUT_STREAM_H__)
#define __URANUS_IO_MEMORY_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"

namespace uranus {
	/**
	* 超簡易メモリ入力ストリーム
	*/
	class CMemoryInputStream : public CBufferInputStream {
	public:
		CMemoryInputStream() {}
		inline CMemoryInputStream(const void* buf, UN_UINT size);
		
		~CMemoryInputStream() {}

	public:
		// 初期化
		inline void Init(const void* buf, size_t size);

		// 終了処理
		inline void Finalize();
	};

	// inline ********************************

	// コンストラクタ
	CMemoryInputStream::CMemoryInputStream(const void* buf, UN_UINT size)
	{
		Init(buf, size);
	}

	/**
	* 初期化
	*/
	void CMemoryInputStream::Init(const void* buf, size_t size)
	{
		m_pBuffer = reinterpret_cast<const UN_UINT8*>(buf);
		m_nSize = static_cast<UN_UINT>(size);
		m_nPos = 0;
	}

	/**
	* 終了処理
	*/
	void CMemoryInputStream::Finalize()
	{
		m_pBuffer = UN_NULL;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_MEMORY_INPUT_STREAM_H__)
