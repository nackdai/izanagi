#if !defined(__POSTEFFECT_CONST_TABLE_H__)
#define __POSTEFFECT_CONST_TABLE_H__

#include "posteffect/PESFormat.h"
#include "posteffect/util/PostEffectUtilityParam.h"

namespace izanagi {
    /**
    * シェーダ定数テーブル
    */
    class CPostEffectConstTable {
        friend class CPostEffectShader;

    private:
        inline CPostEffectConstTable();
        ~CPostEffectConstTable() {}

        NO_COPIABLE(CPostEffectConstTable);

    private:
        // 初期化
        inline IZ_UINT8* Init(
            const S_PES_HEADER& sHeader,
            IZ_UINT8* p);

        // パラメータセット
        inline void SetParam(
            IZ_UINT idx,
            const void* pValue,
            IZ_UINT nBytes);

        // パラメータ取得
        inline const void* GetParam(
            IZ_UINT idx,
            IZ_UINT* pBytes) const;

        // パラメータ記述取得
        inline const S_PES_PARAMETER* GetDesc(IZ_UINT idx) const;

        // パラメータインデックス取得
        inline IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
        inline IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;

        // パラメータアノテーション取得
        inline const S_PES_PARAM_ANN* GetAnn(IZ_UINT idx) const;

        inline void Reset(IZ_UINT idx);
        inline void ResetAll();

    private:
        // パラメータ数
        IZ_UINT16 m_nParamNum;

        // パラメータアノテーション数
        IZ_UINT16 m_nParamAnnNum;

        // パラメータ記述
        S_PES_PARAMETER* m_pDesc;

        // パラメータアノテーション
        S_PES_PARAM_ANN* m_pAnn;

        // パラメータバッファ
        IZ_UINT8* m_pParamBuffer;

        // パラメータバッファサイズ
        IZ_UINT m_nParamBufSize;
    };

    // inline ***********************************

    // コンストラクタ
    CPostEffectConstTable::CPostEffectConstTable()
    {
        m_nParamNum = 0;
        m_nParamAnnNum = 0;
        m_pDesc = IZ_NULL;
        m_pAnn = IZ_NULL;
        m_pParamBuffer = IZ_NULL;
        m_nParamBufSize = 0;
    }

    // 初期化
    IZ_UINT8* CPostEffectConstTable::Init(
        const S_PES_HEADER& sHeader,
        IZ_UINT8* p)
    {
        // NOTE
        // +----------------+
        // |   パラメータ   |
        // |    テーブル    |
        // +----------------+
        // | アノテーション |
        // |  (パラメータ)  |
        // |    テーブル    |
        // +----------------+
        // |パラメータ初期値|
        // |    バッファ    |
        // +----------------+

        m_nParamNum = sHeader.numParam;
        m_nParamAnnNum = sHeader.numParamAnn;

        // パラメータ記述
        if (m_nParamNum > 0) {
            m_pDesc = (S_PES_PARAMETER*)p;
            p += sizeof(S_PES_PARAMETER) * m_nParamNum;
        }

        // パラメータアノテーション
        if (m_nParamAnnNum > 0) {
            m_pAnn = (S_PES_PARAM_ANN*)p;
            p += sizeof(S_PES_PARAM_ANN) * m_nParamAnnNum;
        }

        // パラメータバッファ
        if (sHeader.sizeValueBuffer > 0) {
            m_pParamBuffer = p;
            m_nParamBufSize = sHeader.sizeValueBuffer;
            p += sHeader.sizeValueBuffer;
        }

        return p;
    }

    // パラメータセット
    void CPostEffectConstTable::SetParam(
        IZ_UINT idx,
        const void* pValue,
        IZ_UINT nBytes)
    {
        IZ_ASSERT(idx < m_nParamNum);
        IZ_ASSERT(m_pParamBuffer != IZ_NULL);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_PES_PARAMETER& sDesc = m_pDesc[idx];
        IZ_ASSERT(sDesc.Bytes == nBytes);
        IZ_ASSERT(sDesc.Pos + sDesc.Bytes <= m_nParamBufSize);

        memcpy(&m_pParamBuffer[sDesc.Pos], pValue, nBytes);

        sDesc.isDirty = IZ_TRUE;
    }

    // パラメータ取得
    const void* CPostEffectConstTable::GetParam(
        IZ_UINT idx,
        IZ_UINT* pBytes) const
    {
        IZ_ASSERT(idx < m_nParamNum);
        IZ_ASSERT(m_pParamBuffer != IZ_NULL);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        const S_PES_PARAMETER& sDesc = m_pDesc[idx];
        
        if (pBytes != IZ_NULL) {
            // パラメータのバイト数を返す
            *pBytes = sDesc.Bytes;
        }

        return reinterpret_cast<const void*>(&m_pParamBuffer[sDesc.Pos]);
    }

    // パラメータ記述取得
    const S_PES_PARAMETER* CPostEffectConstTable::GetDesc(IZ_UINT idx) const
    {
        IZ_ASSERT(idx < m_nParamNum);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_PES_PARAMETER* ret = &m_pDesc[idx];
        return ret;
    }

    // パラメータインデックス取得
    IZ_INT CPostEffectConstTable::GetIdxByName(IZ_PCSTR pszName) const
    {
        IZ_UINT nKey = CKey::GenerateValue(pszName);

        for (IZ_UINT i = 0; i < m_nParamNum; ++i) {
            const S_PES_PARAMETER& sDesc = m_pDesc[i];
            if (sDesc.keyName == nKey) {
                return i;
            }
        }

        return -1;
    }

    IZ_INT CPostEffectConstTable::GetIdxBySemantic(IZ_PCSTR pszSemantic) const
    {
        IZ_UINT nKey = CKey::GenerateValue(pszSemantic);

        for (IZ_UINT i = 0; i < m_nParamNum; ++i) {
            const S_PES_PARAMETER& sDesc = m_pDesc[i];
            if (sDesc.keySemantic == nKey) {
                return i;
            }
        }

        return -1;
    }

    // パラメータアノテーション取得
    const S_PES_PARAM_ANN* CPostEffectConstTable::GetAnn(IZ_UINT idx) const
    {
        IZ_ASSERT(m_pAnn != IZ_NULL);

        const S_PES_PARAMETER* pDesc = GetDesc(idx);

        S_PES_PARAM_ANN* ret = IZ_NULL;
        if (pDesc->hasAnn) {
            ret = &m_pAnn[pDesc->AnnotationIdx];
        }
        return ret;
    }

    void CPostEffectConstTable::Reset(IZ_UINT idx)
    {
        IZ_ASSERT(idx < m_nParamNum);
        IZ_ASSERT(m_pDesc != IZ_NULL);

        S_PES_PARAMETER& sDesc = m_pDesc[idx];
        sDesc.isDirty = IZ_FALSE;
    }

    void CPostEffectConstTable::ResetAll()
    {
        IZ_ASSERT(m_pDesc != IZ_NULL);

        for (IZ_UINT i = 0; i < m_nParamNum; ++i) {
            S_PES_PARAMETER& sDesc = m_pDesc[i];
            sDesc.isDirty = IZ_FALSE;
        }
    }
}   // namespace izanagi

#endif  // #if !defined(__POSTEFFECT_CONST_TABLE_H__)
