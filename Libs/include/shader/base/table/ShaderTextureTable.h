#if !defined(__IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__

#include "izStd.h"
#include "SHDFormat.h"

namespace izanagi {
    /**
    * テクスチャテーブル
    */
    class CShaderTextureTable{
        friend class CShaderBasic;

    private:
        inline CShaderTextureTable();
        ~CShaderTextureTable() { Release(); }

        NO_COPIABLE(CShaderTextureTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT8* p);

        // サンプラ記述取得
        inline const S_SHD_TEXTURE* GetDesc(IZ_UINT idx) const;

        // インデックス取得
        inline IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
        inline IZ_INT GetIdxByNameKey(const CKey& cKey) const;
        inline IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;

        // テクスチャハンドルをセット
        inline IZ_BOOL SetTexture(IZ_UINT idx, graph::CBaseTexture* pTex);
        inline IZ_BOOL SetTextureByName(IZ_PCSTR pszName, graph::CBaseTexture* pTex);
        inline IZ_BOOL SetTextureBySemantic(IZ_PCSTR pszSemantic, graph::CBaseTexture* pTex);

        // テクスチャを取得
        inline graph::CBaseTexture* GetTexture(IZ_UINT idx);

        // テクスチャ数を取得
        inline IZ_UINT GetTexNum() const;

        inline void Release();
        inline void Clean();

        inline graph::CBaseTexture* GetTextureIfDirty(IZ_UINT idx);

    private:
        S_SHD_TEXTRUE_HEADER m_Header;

        // サンプラ記述
        S_SHD_TEXTURE* m_pDesc;
    };

    // inline *****************************

    // コンストラクタ
    CShaderTextureTable::CShaderTextureTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
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
            m_pDesc[idx].isDirty = IZ_TRUE;
            SAFE_REPLACE(m_pDesc[idx].tex, pTex);
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
            ret = m_pDesc[idx].tex;
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
            SAFE_RELEASE(m_pDesc[i].tex);
            m_pDesc[i].isDirty = IZ_FALSE;
        }
    }

    void CShaderTextureTable::Clean()
    {
        for (IZ_UINT i = 0; i < m_Header.numTexture; ++i) {
            m_pDesc[i].isDirty = IZ_FALSE;
        }
    }

    graph::CBaseTexture* CShaderTextureTable::GetTextureIfDirty(IZ_UINT idx)
    {
        graph::CBaseTexture* ret = IZ_NULL;

        if ((idx < m_Header.numTexture)
            && (m_pDesc[idx].isDirty))
        {
            ret = m_pDesc[idx].tex;
            IZ_ASSERT(ret != IZ_NULL);
        }

        return ret;
    }

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__)
