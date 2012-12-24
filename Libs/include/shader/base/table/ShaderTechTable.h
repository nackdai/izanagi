#if !defined(__IZANAGI_SHADER_SHADER_TECH_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_TECH_TABLE_H__

#include "SHDFormat.h"

namespace izanagi {
    /**
    * テクニックテーブル
    *
    * シェーダ内のテクニックを管理するクラス
    */
    class CShaderTechTable {
        friend class CShaderBasic;

    private:
        inline CShaderTechTable();
        ~CShaderTechTable() {}

        NO_COPIABLE(CShaderTechTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(IZ_UINT8* p);

        // テクニック記述取得
        inline const S_SHD_TECHNIQUE* GetDesc(IZ_UINT idx) const;

        // 所属パスインデックス取得
        inline IZ_UINT GetPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassPos) const;

        IZ_UINT GetTechNum() const { return m_Header.numTechnique; }

    private:
        S_SHD_TECH_HEADER m_Header;

        // テクニック記述
        S_SHD_TECHNIQUE* m_pDesc;
    };

    // inline ********************************

    // コンストラクタ
    CShaderTechTable::CShaderTechTable()
    {
        FILL_ZERO(&m_Header, sizeof(m_Header));
        m_pDesc = IZ_NULL;
    }

    // 初期化
    IZ_UINT8* CShaderTechTable::Init(IZ_UINT8* p)
    {
        // NOTE
        // +----------------+
        // |   テクニック   |
        // |    テーブル    |
        // +----------------+

        memcpy(&m_Header, p, sizeof(m_Header));
        p += sizeof(m_Header);

        if (m_Header.numTechnique > 0) {
            // テクニック記述
            m_pDesc = (S_SHD_TECHNIQUE*)p;
            p += sizeof(S_SHD_TECHNIQUE) * m_Header.numTechnique;
        }

        return p;
    }

    // テクニック記述取得
    const S_SHD_TECHNIQUE* CShaderTechTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_Header.numTechnique);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_SHD_TECHNIQUE* ret = &m_pDesc[idx];
        return ret;
    }

    // 所属パスインデックス取得
    IZ_UINT CShaderTechTable::GetPassIdx(IZ_UINT nTechIdx, IZ_UINT nPassPos) const
    {
        IZ_ASSERT(nTechIdx < m_Header.numTechnique);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        const S_SHD_TECHNIQUE& sDesc = m_pDesc[nTechIdx];

        IZ_ASSERT(nPassPos < sDesc.numPass);
        IZ_UINT ret = sDesc.posPass + nPassPos;

        return ret;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_TECH_TABLE_H__)
