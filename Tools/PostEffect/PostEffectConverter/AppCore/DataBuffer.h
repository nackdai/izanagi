#if !defined(__POSTEFFECT_CONVERTER_DATA_BUFFER_H__)
#define __POSTEFFECT_CONVERTER_DATA_BUFFER_H__

#include <vector>
#include "izDefs.h"

class CDataBuffer {
	static CDataBuffer s_cInstance;

public:
	static CDataBuffer& GetInstance() { return s_cInstance; }

private:
	CDataBuffer() {}
	~CDataBuffer() {}

public:
	IZ_UINT Register(const void* p, size_t nSize);

	IZ_UINT GetBufferSize() { return static_cast<IZ_UINT>(m_Buffer.size()); }
	const void* GetBuffer() { return &m_Buffer[0]; }

private:
	std::vector<IZ_UINT8> m_Buffer;
};

#endif	// #if !defined(__POSTEFFECT_CONVERTER_DATA_BUFFER_H__)
