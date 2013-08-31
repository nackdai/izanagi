#if !defined(__IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__)
#define __IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__

#include "izStd.h"
#include "shader/base/SHDFormat.h"

namespace izanagi
{
namespace shader
{
    /**
    * テクスチャテーブル
    */
    class CShaderTextureTable {
        friend class CShaderBasic;

        static size_t ComputeBufferSize(IZ_UINT16 numTexture);

    private:
        CShaderTextureTable();
        ~CShaderTextureTable() { Release(); }

        NO_COPIABLE(CShaderTextureTable);

    private:
        // 初期化
        IZ_UINT8* Init(IZ_UINT8* p);
        IZ_UINT8* SetInternalBuffer(IZ_UINT8* p);

        // サンプラ記述取得
        const S_SHD_TEXTURE* GetDesc(IZ_UINT idx) const;

        // インデックス取得
        IZ_INT GetIdxByName(IZ_PCSTR pszName) const;
        IZ_INT GetIdxByNameKey(const CKey& cKey) const;
        IZ_INT GetIdxBySemantic(IZ_PCSTR pszSemantic) const;

        // テクスチャハンドルをセット
        IZ_BOOL SetTexture(IZ_UINT idx, graph::CBaseTexture* pTex);
        IZ_BOOL SetTextureByName(IZ_PCSTR pszName, graph::CBaseTexture* pTex);
        IZ_BOOL SetTextureBySemantic(IZ_PCSTR pszSemantic, graph::CBaseTexture* pTex);

        // テクスチャを取得
        graph::CBaseTexture* GetTexture(IZ_UINT idx);

        // テクスチャ数を取得
        IZ_UINT GetTexNum() const;

        void Release();
        void Clean();

        graph::CBaseTexture* GetTextureIfDirty(IZ_UINT idx);

    private:
        struct STextureHolder {
            IZ_BOOL isDirty;
            graph::CBaseTexture* tex;
        };

    private:
        S_SHD_TEXTRUE_HEADER m_Header;

        // サンプラ記述
        S_SHD_TEXTURE* m_pDesc;
        STextureHolder* m_Holder;
    };
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_TEXTURE_TABLE_H__)
