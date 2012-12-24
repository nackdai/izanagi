#if !defined(__POSTEFFECT_CONVERTER_STRING_CHUNK_H__)
#define __POSTEFFECT_CONVERTER_STRING_CHUNK_H__

#include <vector>
#include "izDefs.h"

/**
*/
class CStringChunk {
private:
    static CStringChunk s_cInstance;

public:
    static CStringChunk& GetInstance() { return s_cInstance; }

private:
    CStringChunk() {}
    ~CStringChunk() {}

public:
    IZ_UINT Register(IZ_PCSTR pszStr);

    IZ_UINT GetBufferSize() { return static_cast<IZ_UINT>(m_StrBuf.size()); }
    IZ_PCSTR GetBuffer() { return &m_StrBuf[0]; }

private:
    std::vector<IZ_CHAR> m_StrBuf;
};

#endif  // #if !defined(__POSTEFFECT_CONVERTER_STRING_CHUNK_H__)
