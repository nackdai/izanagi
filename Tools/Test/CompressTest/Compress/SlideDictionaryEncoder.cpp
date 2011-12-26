#include "SlideDictionaryEncoder.h"
#include "IoStreamUtil.h"

using namespace izanagi;

/**
* 初期化
*/
IZ_BOOL CSlideDictionaryEncoder::Init(
	IMemoryAllocator* pAllocator,
	IZ_UINT nDictionarySize,
	IZ_UINT nMaxMatchLen)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(nDictionarySize > 1);
	IZ_ASSERT(nMaxMatchLen > 1);
	IZ_ASSERT(nDictionarySize * 2 > nMaxMatchLen);

	Clear();

	// 辞書
	IZ_BOOL ret = InitDictionary(pAllocator, nDictionarySize);
	VGOTO(ret, __EXIT__);

	m_nMaxMatchLen = nMaxMatchLen;

__EXIT__:
	return ret;
}

/**
* 圧縮
*/
IZ_BOOL CSlideDictionaryEncoder::Encode(
	SDesc* pDesc,
	IInputStream* pInput,
	izanagi_tk::IOutputStream* pOutput)
{
	IZ_ASSERT(m_pDic != IZ_NULL);
	IZ_ASSERT(pInput != IZ_NULL);
	IZ_ASSERT(pOutput != IZ_NULL);
	IZ_ASSERT(pDesc != IZ_NULL);

	pDesc->org_size = 0;
	pDesc->compressed_size = pOutput->GetCurPos();

	m_nPos = 0;
	m_nReadSize = 0;

	// 読み込み
	ReadData(pInput);

	SMatchInfo sMatchInfo;

	for (; m_nPos < m_nReadSize; ) {
		// 探索
		Search(sMatchInfo);

		if (sMatchInfo.len < 2) {
			// ２文字以下ならそのまま出力

			IZ_UINT16 n = m_pDic[m_nPos];
			VRETURN(CIoStreamUtil::Write(pOutput, n));

			Next(pInput);

			continue;
		}

		// SMatchInfo 出力
		{
			IZ_UINT16 nLen = sMatchInfo.len + THRESHOLD_CODE;

			IZ_OUTPUT_WRITE_VRETURN(
				pOutput,
				&nLen,
				0,
				sizeof(nLen));
			IZ_OUTPUT_WRITE_VRETURN(
				pOutput,
				&sMatchInfo.dist,
				0,
				sizeof(sMatchInfo.dist));
		}

		// 処理位置をすすめる
		for (IZ_UINT i = 0; i < sMatchInfo.len; i++) {
			if (!Next(pInput)) {
				break;
			}
		}
	}

	pDesc->compressed_size = pOutput->GetCurPos() - pDesc->compressed_size;

	return IZ_TRUE;
}

// 処理位置を次の位置に移動
IZ_BOOL CSlideDictionaryEncoder::Next(IInputStream* pInput)
{
	IZ_ASSERT(m_pDic != IZ_NULL);
	IZ_ASSERT(pInput != IZ_NULL);

	m_nPos++;

	if (m_nPos < m_nDictionarySize * 2 - m_nMaxMatchLen) {
		// まだ辞書をスライドさせる必要がない
		return IZ_TRUE;
	}

	// 位置を前半分に移動
	m_nPos -= m_nDictionarySize;

	// 辞書をスライド
	// 後半分を前に持ってくる
	memcpy(&m_pDic[0], &m_pDic[m_nDictionarySize], m_nDictionarySize);

	// 読み込みサイズを減らす
	m_nReadSize = (m_nReadSize < m_nDictionarySize
					? 0
					: m_nReadSize - m_nDictionarySize);

	// 空いた後半分に追加読み込み
	IZ_UINT nReadSize = ReadData(pInput);

	return (nReadSize > 0);
}

// データ読み込み
IZ_UINT CSlideDictionaryEncoder::ReadData(IInputStream* pInput)
{
	IZ_ASSERT(pInput != IZ_NULL);

	if (CIoStreamUtil::IsEnableInput(pInput)) {
		IZ_BYTE* pBuf = &m_pDic[m_nReadSize];
		IZ_UINT nReadSize = m_nDictionarySize * 2 - m_nReadSize;

		IZ_UINT ret = pInput->Read(pBuf, 0, nReadSize);
		m_nReadSize += ret;

		return ret;
	}

	return 0;
}

// 辞書内を探す
void CSlideDictionaryEncoder::Search(SMatchInfo& refMatchInfo)
{
	refMatchInfo.len = 0;
	refMatchInfo.dist = 0;

	// どこまで探索するか
	const IZ_INT nMin = (m_nPos < m_nDictionarySize + 1
							? 0
							: m_nPos - m_nDictionarySize - 1);

	for (IZ_INT i = m_nPos - 1; i >= nMin; i--) {
		// 一致の可能性があるところを探す
		if (m_pDic[m_nPos] == m_pDic[i]) {
			// 一致長
			IZ_UINT len = 1;

			// どのくらい一致するか
			for (; len < m_nMaxMatchLen && i + len < m_nReadSize; len++) {
				if (m_pDic[m_nPos + len] != m_pDic[i + len]) {
					break;
				}
			}

			// より長い一致
			if (refMatchInfo.len < len) {
				refMatchInfo.len = len;				// 一致長
				refMatchInfo.dist = m_nPos - i;		// 相対距離
			}
		}
	}
}
