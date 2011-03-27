#include "BitInputStream.h"

using namespace izanagi;

IZ_BYTE CBitInputStream::GetBit()
{
	IZ_ASSERT(IsEnableInput());

	if (m_nPos == 0) {
		FILL_ZERO(m_nBuffer, sizeof(m_nBuffer));
		
		if(!IZ_INPUT_READ(m_pInput, m_nBuffer, 0, sizeof(m_nBuffer))) {
			IZ_ASSERT(IZ_FALSE);
		}
	}

	IZ_UINT nBufPos = m_nPos >> 3;
	IZ_BYTE ret = (m_nBuffer[nBufPos] >> (m_nPos & 0x07)) & 0x01;

	m_nPos = (m_nPos >= BUFFER_BIT ? 0 : m_nPos);

	return ret;
}
