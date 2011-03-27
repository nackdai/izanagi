#include "RangeCoderDecoder.h"
#include "IoStreamUtil.h"

using namespace izanagi;

/**
* 復号化
*/
IZ_BOOL CRangeCoderDecoder::Decode(
	SDesc* pDesc,
	IInputStream* pInput,
	IZ_UINT nTotal,
	izanagi_tk::IOutputStream* pOutput)
{
	IZ_ASSERT(pDesc != IZ_NULL);
	IZ_ASSERT(pInput != IZ_NULL);
	IZ_ASSERT(pOutput != IZ_NULL);
	IZ_ASSERT(nTotal > 0);

	pDesc->compressed_size = pInput->GetSize();
	pDesc->decode_size = pOutput->GetCurPos();

	// 確率表
	IZ_UINT nCountTable[257];

	for (;;) {
		// 読み込み
		//IZ_UINT nReadSize = pInput->Read(m_pBuffer, 0, m_nBufferSize);

		// 初期確率表作成
		nCountTable[0] = 0;
		for (IZ_UINT i = 1; i < COUNTOF(nCountTable); i++) {
			nCountTable[i] = nCountTable[i - 1] + 1;
		}

		m_RCParam.low = 0;
		m_RCParam.range = RC_TOP;
		m_RCParam.tmp = 0;
		m_RCParam.buf = 0;

		// 最初の Low の読み込み
		{
			// 最初の１バイトは無視
			//CIoStreamUtil::ReadByte(pInput);

			// ２３ビット中１６ビット読み込む
			for (IZ_INT i = RC_SHIFT; CIoStreamUtil::IsEnableInput(pInput); i -= 8) {
				IZ_BYTE ch = CIoStreamUtil::ReadByte(pInput);
				m_RCParam.low |= ch;
				if (i < 8) {
					break;
				}

				m_RCParam.low <<= 8;
			}
		}

		// 残り７ビットを取得
		if (CIoStreamUtil::IsEnableInput(pInput)) {
			m_RCParam.buf = CIoStreamUtil::ReadByte(pInput);
			m_RCParam.low = (m_RCParam.low << 7) | ((m_RCParam.buf >> 1) & 0xff);
		}
		else {
			// もう入力バッファがない
			// ７ビットシフトするだけ
			m_RCParam.low <<= 7;
		}

		// 復号化
		for (IZ_UINT i = 0; i < nTotal; i++) {
			IZ_UINT nCurTotal = nCountTable[COUNTOF(nCountTable) - 1];

			// シンボルの出現位置を計算する
			IZ_UINT nPos = ComputeSimbolPos(pInput, nCurTotal);

			// 該当するシンボルを探す
			IZ_UINT ch = 0;
			for (; nCountTable[ch + 1] <= nPos; ch++) {
				;
			}

			// RangeCoder
			Decode(
				nCountTable[ch],
				nCountTable[ch + 1] - nCountTable[ch],
				nCurTotal);

			// シンボル出力
			VRETURN(CIoStreamUtil::WriteByte(pOutput, ch));

			// 確率表更新
			UpdateCount(
				ch, 
				nCountTable, 
				COUNTOF(nCountTable));
		}

		if (!CIoStreamUtil::IsEnableInput(pInput)) {
			// もう読み込めないので終了
			break;
		}
	}

	pDesc->decode_size = pOutput->GetCurPos() - pDesc->decode_size;

	return IZ_TRUE;
}

// シンボルの出現位置を計算する
IZ_UINT CRangeCoderDecoder::ComputeSimbolPos(
	IInputStream* pInput,
	IZ_UINT nTotal)
{
	// Low と Range の正規化
	IZ_BOOL result = Normalize(pInput);
	IZ_ASSERT(result);

	m_RCParam.tmp = m_RCParam.range / nTotal;
	IZ_UINT nTmp = m_RCParam.low / m_RCParam.tmp;

	return (nTmp >= nTotal ? nTotal - 1 : nTmp);
}

// Low と Range の正規化
IZ_BOOL CRangeCoderDecoder::Normalize(IInputStream* pInput)
{
	while (m_RCParam.range <= RC_BOTTOM) {
		m_RCParam.range <<= 8;

		// buf の残り１ビットを追加する
		m_RCParam.low = (m_RCParam.low << 8) | ((m_RCParam.buf << 7) & 0xff);

		if (CIoStreamUtil::IsEnableInput(pInput)) {
			m_RCParam.buf = CIoStreamUtil::ReadByte(pInput);
		}
		else {
			m_RCParam.buf = 0;
		}

		// ７ビット処理
		m_RCParam.low |= (m_RCParam.buf >> 1);
	}

	return IZ_TRUE;
}

// 復号化
void CRangeCoderDecoder::Decode(
	IZ_UINT nLow,
	IZ_UINT nRange,
	IZ_UINT nTotal)
{
	IZ_UINT nTmp = m_RCParam.tmp * nLow;

	m_RCParam.low -= nTmp;

	if (nLow + nRange < nTotal) {
		m_RCParam.range = m_RCParam.tmp * nRange;
	}
	else {
		m_RCParam.range -= nTmp;
	}
}
