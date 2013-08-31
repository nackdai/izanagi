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

        static size_t ComputeBufferSize(IZ_UINT16 numPass);

    private:
        CShaderPassTable();
        ~CShaderPassTable() {}

        NO_COPIABLE(CShaderPassTable);

    private:
        // 初期化
        IZ_UINT8* Init(IZ_UINT8* p);
        IZ_UINT8* SetInternalBuffer(IZ_UINT8* p);

        // パス数取得
        IZ_UINT GetPassNum() const;

        // パス記述取得
        const S_SHD_PASS* GetDesc(IZ_UINT idx) const;

        IZ_UINT GetConstIdx(IZ_UINT passIdx, IZ_UINT constPos) const;
        IZ_UINT GetSmplIdx(IZ_UINT passIdx, IZ_UINT smplPos) const;

    private:
        struct SPassParameterHolder {
            S_SHD_PARAM_IDX* ptrConst;
            S_SHD_PARAM_IDX* ptrSampler;
        };

    private:
        S_SHD_PASS_HEADER m_Header;

        // パス記述
        S_SHD_PASS* m_pDesc;
        SPassParameterHolder* m_Holder;

        S_SHD_PARAM_IDX* m_UsedParamIdxList;
    };
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_PASS_TABLE_H__)
