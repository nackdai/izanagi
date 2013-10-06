#include "StringChunk.h"

CStringChunk CStringChunk::s_cInstance;

IZ_UINT CStringChunk::Register(IZ_PCSTR pszStr)
{
    size_t ret = m_StrBuf.size();

    size_t len = strlen(pszStr);

    if (len > 0) {
        for (size_t i = 0; i < len; i++) {
            m_StrBuf.push_back(pszStr[i]);
        }

        size_t nZeroLen = 0;

        if ((len & 0x03) == 0) {
            // 末尾に0を付加しつつ、４バイトアラインになるようにする
            nZeroLen = 4;
        }
        else {
            nZeroLen = len & 0x03;
            nZeroLen = (nZeroLen > 0 ? 4 - nZeroLen : 0);
        }

        for (size_t i = 0; i < nZeroLen; i++) {
            m_StrBuf.push_back(0);
        }
    }

    return static_cast<IZ_UINT>(ret);
}
