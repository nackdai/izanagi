#include "BitOutputStream.h"

using namespace izanagi;

IZ_BOOL CBitOutputStream::PutByte(IZ_BYTE n)
{
	for (IZ_UINT i = 0; i < 8; i++) {
		VRETURN(PutBit((n >> i) & 0x01));
	}
	return IZ_TRUE;
}

IZ_BOOL CBitOutputStream::PutBit(IZ_BYTE n)
{
	IZ_BYTE nBit = (n == 0 ? 0 : 1);

	IZ_UINT nBufPos = m_nPos >> 3;
	m_nBuffer[nBufPos] = (m_nBuffer[nBufPos] << 1) | nBit;

	m_nPos++;
	if (m_nPos >= BUFFER_BIT) {
		VRETURN(Flush());
	}

	return IZ_TRUE;
}

IZ_BOOL CBitOutputStream::Flush()
{
	IZ_UINT nOutSize = m_nPos >> 3;

	IZ_OUTPUT_WRITE_VRETURN(m_pOutput, m_nBuffer, 0, nOutSize);

	FILL_ZERO(m_nBuffer, sizeof(m_nBuffer));
	m_nPos = 0;

	return IZ_TRUE;
}
