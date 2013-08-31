#if !defined(__IZANAGI_SHADER_SHADER_PASS_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_PASS_TABLE_H__

#include "shader/base/SHDFormat.h"

namespace izanagi
{
namespace shader
{
    /**
    * パステーブル
    *
    * テクニックの持つパスを管理するクラス
    */
    class CShaderPassTable {
        friend class CShaderBasic;

    private:
        inline CShaderPassTable();
        ~CShaderPassTable() {}

        NO_COPIABLE(CShaderPassTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT8* p);

        // パス数取得
        inline IZ_UINT GetPassNum() const;

        // パス記述取得
        inline const S_SHD_PASS* GetDesc(IZ_UINT idx) const;

    private:
        S_SHD_PASS_HEADER m_Header;

        // パス記述
        S_SHD_PASS* m_pDesc;
    };

    // inline *******************************************

    // コンストラクタ
    CShaderPassTable::CShaderPassTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CShaderPassTable::Init(IZ_UINT8* p)
    {
        // NOTE
        // +----------------+
        // |  パステーブル  |
        // +----------------+
        // |パスで利用される|
        // |  パラメータと  |
        // |   サンプラの   |
        // |  インデックス  |
        // +----------------+

        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        // パス記述
        if (m_Header.numPass > 0) {
            m_pDesc = (S_SHD_PASS*)p;
            p += sizeof(S_SHD_PASS) * m_Header.numPass;
        }

        // 参照するパラメータ or サンプラインデックス
        if (m_Header.numPass > 0) {
            // TODO
            // この条件でいいのか？
            S_SHD_PARAM_IDX* pUsedParamIdxList = reinterpret_cast<S_SHD_PARAM_IDX*>(p);

            IZ_UINT nPos = 0;
            
            for (IZ_UINT i = 0; i < m_Header.numPass; ++i) {
                S_SHD_PASS& sDesc = m_pDesc[i];

                if (sDesc.numConst > 0) {
                    sDesc.ptrConst = pUsedParamIdxList + nPos;
                    p += sizeof(S_SHD_PARAM_IDX) * sDesc.numConst;
                    nPos += sDesc.numConst;
                }

                if (sDesc.numSampler > 0) {
                    sDesc.ptrSampler = pUsedParamIdxList + nPos;
                    p += sizeof(S_SHD_PARAM_IDX) * sDesc.numSampler;
                    nPos += sDesc.numSampler;
                }
            }
        }
        
        return p;
    }

    // パス数取得
    IZ_UINT CShaderPassTable::GetPassNum() const
    {
        return m_Header.numPass;
    }

    // パス記述取得
    const S_SHD_PASS* CShaderPassTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numPass);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_SHD_PASS* ret = &m_pDesc[idx];
        return ret;
    }
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_PASS_TABLE_H__)
