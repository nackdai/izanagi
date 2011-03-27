#if !defined(__IZANAGI_COMPRESS_SLIDE_DICTIONARY_BASE_H__)
#define __IZANAGI_COMPRESS_SLIDE_DICTIONARY_BASE_H__

#include "izStd.h"
#include "izIo.h"

namespace izanagi {
	/**
	*/
	class CSlideDictionaryBase {
	protected:
		CSlideDictionaryBase()
		{
			m_pAllocator = IZ_NULL;

			m_pDic = IZ_NULL;
			m_nDictionarySize = 0;
			m_nMaxMatchLen = 0;

			m_nPos = 0;
			m_nReadSize = 0;
		}

		~CSlideDictionaryBase()
		{
			Clear();
		}

		NO_COPIABLE(CSlideDictionaryBase);

	protected:
		// 初期化
		IZ_BOOL InitDictionary(
			IMemoryAllocator* pAllocator,
			IZ_UINT nDictionarySize)
		{
			IZ_ASSERT(pAllocator != IZ_NULL);
			IZ_ASSERT(nDictionarySize > 0);

			// 倍取る
			IZ_UINT nAllocDicSize = nDictionarySize << 1;

			// 辞書
			m_pDic = reinterpret_cast<IZ_BYTE*>(ALLOC_ZERO(pAllocator, nAllocDicSize));
			IZ_BOOL ret = (m_pDic != IZ_NULL);

			if (ret) {
				m_pAllocator = pAllocator;
				m_nDictionarySize = nDictionarySize;
			}

			return ret;
		}

		void Clear()
		{
			if (m_pAllocator != IZ_NULL) {
				FREE(m_pAllocator, m_pDic);
			}

			m_pDic = IZ_NULL;
			m_nDictionarySize = 0;
		}

	protected:
		enum {
			THRESHOLD_CODE = 256,
		};

	protected:
		IMemoryAllocator* m_pAllocator;

		IZ_BYTE* m_pDic;			// 辞書
		IZ_UINT m_nDictionarySize;	// 辞書サイズ
		IZ_UINT m_nMaxMatchLen;		// 最大一致長

		IZ_UINT m_nPos;			// 処理位置
		IZ_UINT m_nReadSize;	// 読み込みデータサイズ
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_COMPRESS_SLIDE_DICTIONARY_BASE_H__)
