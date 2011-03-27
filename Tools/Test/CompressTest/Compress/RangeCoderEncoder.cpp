#include "RangeCoderEncoder.h"
#include "IoStreamUtil.h"

using namespace izanagi;

/*
* 初期化
*/
IZ_BOOL CRangeCoderEncoder::Init(
	IMemoryAllocator* pAllocator,
	IZ_UINT nBufferSize)
{
	IZ_ASSERT(pAllocator != IZ_NULL);

	nBufferSize = (nBufferSize > 0 ? nBufferSize : 1024);

	m_pBuffer = (IZ_BYTE*)ALLOC_ZERO(pAllocator, nBufferSize);
	VRETURN(m_pBuffer);

	m_pAllocator = pAllocator;
	m_nBufferSize = nBufferSize;

	return IZ_TRUE;
}

void CRangeCoderEncoder::Clear()
{
	if (m_pAllocator != IZ_NULL) {
		FREE(m_pAllocator, m_pBuffer);
	}
}

/**
* 符号化
*/
IZ_BOOL CRangeCoderEncoder::Encode(
	SDesc* pDesc,
	IInputStream* pInput,
	izanagi_tk::IOutputStream* pOutput)
{
	IZ_ASSERT(pDesc != IZ_NULL);
	IZ_ASSERT(IsInitialized());
	IZ_ASSERT(pInput != IZ_NULL);
	IZ_ASSERT(pOutput != IZ_NULL);
	IZ_ASSERT(m_pBuffer != IZ_NULL);

	pDesc->org_size = 0;
	pDesc->compressed_size = pOutput->GetCurPos();

	// 確率表
	IZ_UINT nCountTable[257];

	for (;;) {
		// 読み込み
		IZ_UINT nReadSize = pInput->Read(m_pBuffer, 0, m_nBufferSize);

		// 初期確率表作成
		nCountTable[0] = 0;
		for (IZ_UINT i = 1; i < COUNTOF(nCountTable); i++) {
			nCountTable[i] = nCountTable[i - 1] + 1;
		}

		m_RCParam.low = 0;
		m_RCParam.range = RC_TOP;
		m_RCParam.tmp = 0;
		m_RCParam.buf = 0;

		// 符号化
		for (IZ_UINT i = 0; i < nReadSize; i++) {
			IZ_BYTE ch = m_pBuffer[i];
			IZ_UINT nTotal = nCountTable[COUNTOF(nCountTable) - 1];

			// RangeCoder
			Encode(
				nTotal,
				nCountTable[ch],
				nCountTable[ch + 1] - nCountTable[ch],
				pOutput);

			// 確率表更新
			UpdateCount(
				ch, 
				nCountTable, 
				COUNTOF(nCountTable));
		}

		pDesc->org_size += nReadSize;

		if (nReadSize != m_nBufferSize) {
			// 全て読み込んだので終了
			break;
		}
	}

	// 出力できていない分をフラッシュ
	VRETURN(Flush(pOutput));

	pDesc->compressed_size = pOutput->GetCurPos() - pDesc->compressed_size;

	return IZ_TRUE;
}

/**
* フラッシュ
*/
IZ_BOOL CRangeCoderEncoder::Flush(izanagi_tk::IOutputStream* pOutput)
{
	// Low と Range の正規化
	VRETURN(Normalize(pOutput));

	IZ_UINT tmp = (m_RCParam.low >> 23) & 0xff;
	tmp += 1;

	if (tmp > 0xff) {
		// 桁上がり
		VRETURN(CIoStreamUtil::WriteByte(pOutput, m_RCParam.buf + 1));

		for (IZ_UINT i = 0; i < m_RCParam.tmp; i++) {
			VRETURN(CIoStreamUtil::WriteByte(pOutput, 0));
		}
	}
	else {
		VRETURN(CIoStreamUtil::WriteByte(pOutput, m_RCParam.buf));

		for (IZ_UINT i = 0; i < m_RCParam.tmp; i++) {
			VRETURN(CIoStreamUtil::WriteByte(pOutput, 0xff));
		}
	}

	IZ_BYTE out = tmp & 0xff;
	IZ_OUTPUT_WRITE_VRETURN(pOutput, &out, 0, sizeof(out));

	return IZ_TRUE;
}

// 符号化
IZ_BOOL CRangeCoderEncoder::Encode(
	IZ_UINT nTotal,
	IZ_UINT nLow,
	IZ_UINT nRange,
	izanagi_tk::IOutputStream* pOutput)
{
	// Low と Range の正規化
	VRETURN(Normalize(pOutput));

	IZ_UINT nTmp = m_RCParam.range / nTotal;

	if (nLow == 0) {
		m_RCParam.range = nTmp * nRange;
	}
	else if ((nLow + nRange) == nRange) {
		m_RCParam.low += nTmp * nLow;
		m_RCParam.range -= nTmp * nLow;
	}
	else {
		m_RCParam.low += nTmp * nLow;
		m_RCParam.range = nTmp * nRange;
	}

	return IZ_TRUE;
}

// Low と Range の正規化
IZ_BOOL CRangeCoderEncoder::Normalize(izanagi_tk::IOutputStream* pOutput)
{
	while (m_RCParam.range <= RC_BOTTOM) {
		if (m_RCParam.low < (0xffU << RC_SHIFT)) {
			// 通常出力
			if (!m_bIsFirstOutByte) {
				VRETURN(CIoStreamUtil::WriteByte(pOutput, m_RCParam.buf));
			}

			m_bIsFirstOutByte = IZ_FALSE;

			for (IZ_UINT i = 0; i < m_RCParam.tmp; i++) {
				VRETURN(CIoStreamUtil::WriteByte(pOutput, 0xff));
			}

			m_RCParam.buf = (m_RCParam.low >> 23) & 0xff;
		}
		else if (m_RCParam.low & RC_TOP) {
			// 桁上がり
			if (!m_bIsFirstOutByte) {
				VRETURN(CIoStreamUtil::WriteByte(pOutput, m_RCParam.buf + 1));
			}

			m_bIsFirstOutByte = IZ_FALSE;

			for (IZ_UINT i = 0; i < m_RCParam.tmp; i++) {
				VRETURN(CIoStreamUtil::WriteByte(pOutput, 0));
			}

			m_RCParam.buf = (m_RCParam.low >> 23) & 0xff;
		}
		else {
			// 0xff or 0x00 になる可能性のあるバイト数
			m_RCParam.tmp++;
		}

		m_RCParam.range <<= 8;
		m_RCParam.low = (m_RCParam.low << 8) & (RC_TOP - 1);
	}

	return IZ_TRUE;
}
