#if !defined(__IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__

#include "shader/base/SHDFormat.h"

namespace izanagi
{
namespace shader
{
    // シェーダ定数テーブル
    class CShaderParameterTable {
        friend class CShaderBasic;

        static size_t ComputeBufferSize(IZ_UINT16 numParam);

    private:
        CShaderParameterTable();
        ~CShaderParameterTable() {}

        NO_COPIABLE(CShaderParameterTable);

    private:
        // 初期化
        IZ_UINT8* Init(IZ_UINT8* p);
        IZ_UINT8* SetInternalBuffer(IZ_UINT8* p);

        // パラメータセット
        IZ_BOOL SetParam(
            IZ_UINT idx,
            const void* pValue,
            IZ_UINT nBytes);

        // パラメータ取得
        const void* GetParam(
            IZ_UINT idx,
            IZ_UINT* pBytes) const;

        // パラメータ記述取得
        const S_SHD_PARAMETER* GetDesc(IZ_UINT idx) const;

        // パラメータインデックス取得
        IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
        IZ_INT GetIdxByNameKey(const CKey& cKey) const;

        IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;
        IZ_INT GetIdxBySemanticKey(const CKey& cKey) const;

        // パラメータアノテーション取得
        const S_SHD_PARAM_ANN* GetAnn(IZ_UINT idx) const;

        void Reset(IZ_UINT idx);
        void ResetAll();

    private:
        struct SParameterHolder {
            void* ptrParam;
        };

    private:
        S_SHD_PARAM_HEADER m_Header;

        // パラメータ記述
        S_SHD_PARAMETER* m_pDesc;

        // パラメータアノテーション
        S_SHD_PARAM_ANN* m_pAnn;

        // パラメータバッファ
        IZ_UINT8* m_pParamBuffer;

        // パラメータバッファサイズ
        IZ_UINT m_nParamBufSize;

        SParameterHolder* m_ParamHolder;
    };
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_PARAMETER_TABLE_H__)
