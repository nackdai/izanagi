#include <stdafx.h>
#include "DataBuffer.h"

CDataBuffer CDataBuffer::s_cInstance;

IZ_UINT CDataBuffer::Register(const void* p, size_t nSize)
{
	IZ_ASSERT((nSize & 0x03) == 0);

	IZ_UINT ret = static_cast<IZ_UINT>(m_Buffer.size());

	const IZ_UINT8* tmp = reinterpret_cast<const IZ_UINT8*>(p);

	for (size_t i = 0; i < nSize; i++) {
		m_Buffer.push_back(tmp[i]);
	}

	return ret;
}
