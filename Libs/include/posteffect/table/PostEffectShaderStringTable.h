#if !defined(__IZANAGI_POSTEFFECT_STRING_TABLE_H__)
#define __IZANAGI_POSTEFFECT_STRING_TABLE_H__

#include "izStd.h"

namespace izanagi {
    /**
    * 文字列テーブル
    */
    class CPostEffectStringTable{
        friend class CPostEffectShader;

    private:
        inline CPostEffectStringTable();
        ~CPostEffectStringTable() {}

        NO_COPIABLE(CPostEffectStringTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT size, IZ_UINT8* p);

        // 文字列取得
        inline IZ_PCSTR GetString(IZ_UINT pos) const;

    private:
        // バッファサイズ
        IZ_UINT m_nSize;

        // バッファ
        IZ_INT8* m_pStringBuffer;
    };

    // inline *********************************

    // コンストラクタ
    CPostEffectStringTable::CPostEffectStringTable()
    {
        m_nSize = 0;
        m_pStringBuffer = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CPostEffectStringTable::Init(IZ_UINT size, IZ_UINT8* p)
    {
        m_nSize = size;
        m_pStringBuffer = (IZ_INT8*)p;

        return (p + size);
    }

    // 文字列取得
    IZ_PCSTR CPostEffectStringTable::GetString(IZ_UINT pos) const
    {
        IZ_PCSTR ret = IZ_NULL;

        if (pos < m_nSize) {
            ret = m_pStringBuffer + pos;
        }

        return ret;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_STRING_TABLE_H__)
