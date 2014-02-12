#if !defined(__IZANAGI_SHADER_SHADER_STRING_BUFFER_H__)
#define __IZANAGI_SHADER_SHADER_STRING_BUFFER_H__

#include "izDefs.h"

namespace izanagi
{
namespace shader
{
    /**
    */
    class CShaderStringBuffer {
        friend class CShaderBasic;

    private:
        inline CShaderStringBuffer();
        ~CShaderStringBuffer() {}

        NO_COPIABLE(CShaderStringBuffer);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT size, IZ_UINT8* p);

        // 文字列取得
        inline IZ_PCSTR GetString(IZ_UINT pos) const;

    private:
        // バッファサイズ
        IZ_UINT m_nSize;

        // バッファ
        IZ_INT8* m_pBuffer;
    };

    // inline *********************************

    // コンストラクタ
    CShaderStringBuffer::CShaderStringBuffer()
    {
        m_nSize = 0;
        m_pBuffer = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CShaderStringBuffer::Init(IZ_UINT size, IZ_UINT8* p)
    {
        m_nSize = size;
        m_pBuffer = reinterpret_cast<IZ_INT8*>(p);

        return (p + size);
    }

    // 文字列取得
    IZ_PCSTR CShaderStringBuffer::GetString(IZ_UINT pos) const
    {
        IZ_PCSTR ret = IZ_NULL;

        if (pos < m_nSize) {
            ret = (IZ_PCSTR)(m_pBuffer + pos);
        }

        return ret;
    }
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_STRING_BUFFER_H__)
