#include "shader/base/table/ShaderTextureTable.h"

namespace izanagi
{
namespace shader
{
    size_t CShaderTextureTable::ComputeBufferSize(IZ_UINT16 numTexture)
    {
        size_t ret = sizeof(STextureHolder) * numTexture;
        return ret;
    }

    // コンストラクタ
    CShaderTextureTable::CShaderTextureTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
        m_Holder = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CShaderTextureTable::Init(IZ_UINT8* p)
    {
        // NOTE
        // +------------------+
        // |テクスチャテーブル|
        // +------------------+

        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        if (m_Header.numTexture > 0) {
            // サンプラ記述
            m_pDesc = (S_SHD_TEXTURE*)p;
            p += sizeof(S_SHD_TEXTURE) * m_Header.numTexture;
        }

        return p;
    }

    IZ_UINT8* CShaderTextureTable::SetInternalBuffer(IZ_UINT8* p)
    {
        if (m_Header.numTexture > 0) {
            m_Holder = (STextureHolder*)p;
            p += sizeof(STextureHolder) * m_Header.numTexture;
        }

        return p;
    }

    // サンプラ記述取得
    const S_SHD_TEXTURE* CShaderTextureTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numTexture);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_SHD_TEXTURE* ret = &m_pDesc[idx];
        return ret;
    }

    // インデックス取得
    IZ_INT CShaderTextureTable::GetIdxByName(IZ_PCSTR pszName) const
    {
        IZ_UINT ret = GetIdxByNameKey(CKey(pszName));
        return ret;
    }

    IZ_INT CShaderTextureTable::GetIdxByNameKey(const CKey& cKey) const
    {
        IZ_UINT nKey = cKey.GetValue();

        for (IZ_UINT i = 0; i < m_Header.numTexture; ++i) {
            const S_SHD_TEXTURE& sDesc = m_pDesc[i];
            if (sDesc.keyName == nKey) {
                return i;
            }
        }

        return -1;
    }

    IZ_INT CShaderTextureTable::GetIdxBySemantic(IZ_PCSTR pszSemantic) const
    {
        IZ_ASSERT(m_pDesc != IZ_NULL);

        IZ_UINT nKey = CKey::GenerateValue(pszSemantic);

        for (IZ_UINT i = 0; i < m_Header.numTexture; ++i) {
            const S_SHD_TEXTURE& sDesc = m_pDesc[i];
            if (sDesc.keySemantic == nKey) {
                return i;
            }
        }

        return -1;
    }

    // テクスチャハンドルをセット
    IZ_BOOL CShaderTextureTable::SetTexture(IZ_UINT idx, graph::CBaseTexture* pTex)
    {
        IZ_ASSERT(m_pDesc != IZ_NULL);

        IZ_BOOL ret = (idx < m_Header.numTexture);
        if (ret) {
            m_Holder[idx].isDirty = IZ_TRUE;
            SAFE_REPLACE(m_Holder[idx].tex, pTex);
        }
        return ret;
    }

    IZ_BOOL CShaderTextureTable::SetTextureByName(IZ_PCSTR pszName, graph::CBaseTexture* pTex)
    {
        IZ_INT nIdx = GetIdxByName(pszName);

        IZ_BOOL ret = (nIdx >= 0);
        VRETURN(ret);

        if (ret) {
            ret = SetTexture(nIdx, pTex);
            VRETURN(ret);
        }

        return ret;
    }

    IZ_BOOL CShaderTextureTable::SetTextureBySemantic(IZ_PCSTR pszSemantic, graph::CBaseTexture* pTex)
    {
        IZ_INT nIdx = GetIdxBySemantic(pszSemantic);

        IZ_BOOL ret = (nIdx >= 0);
        VRETURN(ret);

        if (ret) {
            ret = SetTexture(nIdx, pTex);
            VRETURN(ret);
        }

        return ret;
    }

    // テクスチャハンドルを取得
    graph::CBaseTexture* CShaderTextureTable::GetTexture(IZ_UINT idx)
    {
        IZ_ASSERT(m_pDesc != IZ_NULL);

        graph::CBaseTexture* ret = IZ_NULL;

        if (idx < m_Header.numTexture) {
            ret = m_Holder[idx].tex;
        }

        return ret;
    }

    // テクスチャ数を取得
    IZ_UINT CShaderTextureTable::GetTexNum() const
    {
        return m_Header.numTexture;
    }

    void CShaderTextureTable::Release()
    {
        for (IZ_UINT i = 0; i < m_Header.numTexture; ++i) {
            SAFE_RELEASE(m_Holder[i].tex);
            m_Holder[i].isDirty = IZ_FALSE;
        }
    }

    void CShaderTextureTable::Clean()
    {
        for (IZ_UINT i = 0; i < m_Header.numTexture; ++i) {
            m_Holder[i].isDirty = IZ_FALSE;
        }
    }

    graph::CBaseTexture* CShaderTextureTable::GetTextureIfDirty(IZ_UINT idx)
    {
        graph::CBaseTexture* ret = IZ_NULL;

        if ((idx < m_Header.numTexture)
            && (m_Holder[idx].isDirty))
        {
            ret = m_Holder[idx].tex;
            IZ_ASSERT(ret != IZ_NULL);
        }

        return ret;
    }
}   // namespace shader
}   // namespace izanagi
