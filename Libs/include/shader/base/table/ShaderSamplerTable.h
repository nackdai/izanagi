#if !defined(__IZANAGI_SHADER_SHADER_SAMPLER_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_SAMPLER_TABLE_H__

#include "shader/base/SHDFormat.h"

namespace izanagi {
    /**
    * サンプラテーブル
    *
    * シェーダ内のサンプラを管理するクラス
    */
    class CShaderSamplerTable {
        friend class CShaderBasic;
        friend class CShaderPass;

    private:
        inline CShaderSamplerTable();
        ~CShaderSamplerTable() {}

        NO_COPIABLE(CShaderSamplerTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT8* p);

        // サンプラ記述取得
        inline const S_SHD_SAMPLER* GetDesc(IZ_UINT idx) const;

        // パラメータインデックス取得
        inline IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
        inline IZ_INT GetIdxByNameKey(const CKey& cKey) const;

    private:
        S_SHD_SAMPLER_HEADER m_Header;

        // サンプラ記述
        S_SHD_SAMPLER* m_pDesc;
    };

    // inline *****************************

    // コンストラクタ
    CShaderSamplerTable::CShaderSamplerTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CShaderSamplerTable::Init(IZ_UINT8* p)
    {
        // NOTE
        // +----------------+
        // |サンプラテーブル|
        // +----------------+

        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        if (m_Header.numSampler > 0) {
            // サンプラ記述
            m_pDesc = (S_SHD_SAMPLER*)p;
            p += sizeof(S_SHD_SAMPLER) * m_Header.numSampler;
        }

        return p;
    }

    // サンプラ記述取得
    const S_SHD_SAMPLER* CShaderSamplerTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numSampler);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_SHD_SAMPLER* ret = &m_pDesc[idx];
        return ret;
    }

    // パラメータインデックス取得
    IZ_INT CShaderSamplerTable::GetIdxByName(IZ_PCSTR pszName) const
    {
        IZ_UINT ret = GetIdxByNameKey(CKey(pszName));
        return ret;
    }

    IZ_INT CShaderSamplerTable::GetIdxByNameKey(const CKey& cKey) const
    {
        IZ_UINT nKey = cKey.GetValue();

        for (IZ_UINT i = 0; i < m_Header.numSampler; ++i) {
            const S_SHD_SAMPLER& sDesc = m_pDesc[i];
            if (sDesc.keyName == nKey) {
                return i;
            }
        }

        return -1;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_SAMPLER_TABLE_H__)
