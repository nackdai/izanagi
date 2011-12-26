#include "compress/CompressSlideDictionaryDecoder.h"
#include "compress/CompressIoStreamUtil.h"

using namespace izanagi;

/**
* 初期化
*/
IZ_BOOL CCompressSlideDictionaryDecoder::Init(
	IMemoryAllocator* pAllocator,
	IZ_UINT nDictionarySize)
{
	Clear();

	IZ_BOOL ret = InitDictionary(pAllocator, nDictionarySize);
	IZ_ASSERT(ret);
	return ret;
}

/**
* 復号化
*/
IZ_BOOL CCompressSlideDictionaryDecoder::Decode(
	IInputStream* pInput,
	IOutputStream* pOutput)
{
	IZ_ASSERT(m_pDic != IZ_NULL);
	IZ_ASSERT(pInput != IZ_NULL);
	IZ_ASSERT(pOutput != IZ_NULL);

	m_nPos = 0;

	for (;;) {
		IZ_UINT16 code = CIoStreamUtil::ReadByte2(pInput);

		if (code < THRESHOLD_CODE) {
			// コードそのもの
			IZ_BYTE c = static_cast<IZ_BYTE>(code & 0xff);

			// 出力
			VRETURN(CIoStreamUtil::WriteByte(pOutput, c));

			// 辞書に登録
			m_pDic[m_nPos++] = c;
			if (m_nPos >= m_nDictionarySize) {
				m_nPos = 0;
			}

			continue;
		}

		IZ_INT nLen = code - THRESHOLD_CODE;				// 一致長
		IZ_INT nDist = CIoStreamUtil::ReadByte2(pInput);	// 相対距離

		// 展開 & 辞書に登録
		for (IZ_INT i = 0; i < nLen; ++i) {
			IZ_INT pos = m_nPos - nDist;
			if (pos < 0) {
				pos += m_nDictionarySize;
			}

			IZ_BYTE c = m_pDic[pos];

			// 出力
			VRETURN(CIoStreamUtil::WriteByte(pOutput, c));

			// 辞書に登録
			m_pDic[m_nPos++] = c;
			if (m_nPos >= m_nDictionarySize) {
				m_nPos = 0;
			}
		}

		if (!CIoStreamUtil::IsEnableInput(pInput)) {
			// 全て処理した
			break;
		}
	}

	return IZ_TRUE;
}
