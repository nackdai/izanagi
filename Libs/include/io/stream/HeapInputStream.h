#if !defined(__URANUS_IO_HEAP_INPUT_STREAM_H__)
#define __URANUS_IO_HEAP_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"
#include "std/allocator/MemoryAllocator.h"

namespace uranus {
	/**
	* 超簡易メモリ入力ストリーム
	* （ただし、アロケータからメモリ確保）
	*/
	class CHeapInputStream : public CBufferInputStream {
	public:
		inline CHeapInputStream();
		inline ~CHeapInputStream();

	public:
		// 初期化
		inline UN_BOOL Init(IMemoryAllocator* pAllocator, UN_UINT size);

		// 終了処理
		inline void Finalize();

	protected:
		IMemoryAllocator* m_pAllocator;
	};

	// inline ********************************

	// コンストラクタ
	CHeapInputStream::CHeapInputStream()
	{
		m_pAllocator = UN_NULL;
	}

	// デストラクタ
	CHeapInputStream::~CHeapInputStream()
	{
		Finalize();
	}

	/**
	* 初期化
	*/
	UN_BOOL CHeapInputStream::Init(IMemoryAllocator* pAllocator, UN_UINT size)
	{
		if ((m_pAllocator != UN_NULL) && (m_pBuffer != UN_NULL)) {
			FREE(m_pAllocator, const_cast<UN_UINT8*>(m_pBuffer));
			m_pBuffer = UN_NULL;
		}

		m_pAllocator = pAllocator;

		// メモリ確保
		m_pBuffer = static_cast<UN_UINT8*>(ALLOC_ZERO(m_pAllocator, size));
		VRETURN(m_pBuffer != UN_NULL);

		m_nSize = size;
		m_nPos = 0;

		return UN_TRUE;
	}

	/**
	* 終了処理
	*/
	void CHeapInputStream::Finalize()
	{
		if ((m_pAllocator != UN_NULL) && (m_pBuffer != UN_NULL)) {
			FREE(m_pAllocator, const_cast<UN_UINT8*>(m_pBuffer));
		}
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_HEAP_INPUT_STREAM_H__)
