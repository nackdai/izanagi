#if !defined(__IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__

#include "izStd.h"
#include "MTRLFormat.h"

namespace izanagi {
    class IInputStream;
    class graph::CGraphicsDevice;
    class IShader;

    /**
    */
    class CMaterial : public CObject {
    public:
        /** ファイルからインスタンス作成.
         */
        static CMaterial* CreateMaterial(
            IMemoryAllocator* pAllocator,
            IInputStream* pIn);

        /** 空のインスタンス作成.
         */
        static CMaterial* CreateMaterial(
            IMemoryAllocator* pAllocator,
            IZ_PCSTR pszName,
            IZ_UINT nTexNum,
            IZ_UINT nParamNum,
            IZ_UINT nParamBytes);

    private:
        // インスタンス作成共通処理.
        static CMaterial* CreateMaterial(
            IZ_UINT nTexNum,
            IZ_UINT nParamNum,
            IZ_UINT nParamBytes,
            IMemoryAllocator* pAllocator);

    private:
        CMaterial();
        ~CMaterial();

        NO_COPIABLE(CMaterial);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** 描画準備.
         */
        IZ_BOOL Prepare(graph::CGraphicsDevice* pDevice);

        /** マテリアルにテクスチャを追加.
         *
         * マテリアルに関連付けられていないテクスチャを追加
         */
        IZ_BOOL AddTexture(
            IZ_PCSTR pszName,
            const S_MTRL_TEXTURE_TYPE& type,
            graph::CBaseTexture* pTex);

        /** マテリアルに関連付けられているテクスチャをセット.
         */
        IZ_BOOL SetTexture(
            IZ_PCSTR pszName, 
            graph::CBaseTexture* pTex);

        /** マテリアルに関連付けられているテクスチャをセット.
         */
        IZ_BOOL SetTexture(
            IZ_UINT nKey,
            graph::CBaseTexture* pTex);

        /** マテリアルにシェーダを追加.
         *
         * マテリアルに関連付けられていないテクスチャを追加
         */
        IZ_BOOL AddShader(IShader* pShader);

        /** マテリアルに関連付けられているシェーダをセット.
         */
        IZ_BOOL SetShader(IShader* pShader);

        /** マテリアルにパラメータを追加.
         *
         * マテリアルに関連付けられていないパラメータを追加.
         */
        IZ_BOOL AddParameter(
            IZ_PCSTR pszName,
            E_MTRL_PARAM_TYPE type,
            IZ_UINT nNum,
            const void* pValue);

        /** マテリアルに関連付けられているパラメータをセット.
         */
        IZ_BOOL SetParameter(
            IZ_PCSTR pszName,
            IZ_UINT nBytes,
            const void* pValue);

        /** マテリアルに関連付けられているパラメータをセット.
         */
        IZ_BOOL SetParameter(
            IZ_UINT nKey,
            IZ_UINT nBytes,
            const void* pValue);

        /**
         * Return whether materal has textures, shaders that are specified.
         */
        IZ_BOOL IsValid() const;

        /** マテリアル名を強制変更.
         */
        void SetName(IZ_PCSTR pszName);

    public:
        /** マテリアル名取得.
         */
        IZ_PCSTR GetName() const { return m_Header.name.GetString(); }

        /** マテリアルキー取得.
         */
        IZ_UINT GetKey() const { return m_Header.keyMaterial; }

        IZ_UINT GetTexNum() const { return m_Header.numTex; }
#if 0
        IZ_UINT GetShaderNum() const { return m_Header.numShader; }
#endif
        IZ_UINT GetParamNum() const { return m_Header.numParam; }

        const S_MTRL_TEXTURE* GetTexInfoByIdx(IZ_UINT idx) const;
        const S_MTRL_TEXTURE* GetTexInfoByName(IZ_PCSTR pszName) const;
        const S_MTRL_TEXTURE* GetTexInfoByKey(const CKey& key) const;

#if 0
        const S_MTRL_SHADER* GetShaderInfoByIdx(IZ_UINT idx) const;
        const S_MTRL_SHADER* GetShaderInfoByName(IZ_PCSTR pszName) const;
        const S_MTRL_SHADER* GetShaderInfoByKey(const CKey& key) const;
#else
        const S_MTRL_SHADER* GetShaderInfo() const { return m_pShaderInfo; }
#endif

        const S_MTRL_PARAM* GetParamInfoByIdx(IZ_UINT idx) const;
        const S_MTRL_PARAM* GetParamInfoByName(IZ_PCSTR pszName) const;
        const S_MTRL_PARAM* GetParamInfoByKey(const CKey& key) const;

        graph::CBaseTexture* GetTextureByIdx(IZ_UINT idx);
        graph::CBaseTexture* GetTextureByName(IZ_PCSTR pszName);
        graph::CBaseTexture* GetTextureByKey(const CKey& key);

#if 0
        IShader* GetShaderByIdx(IZ_UINT idx);
        IShader* GetShaderByName(IZ_PCSTR pszName);
        IShader* GetShaderByKey(const CKey& key);
#else
        IShader* GetShader()
        {
            return (m_pShaderInfo != IZ_NULL ? m_pShaderInfo->shader : IZ_NULL);
        }
#endif

        /** Set technique index of shader.
         */
        IZ_BOOL SetShaderTechnique(IZ_UINT idx);

        /** Return technique index of shader.
         */
        IZ_INT GetShaderTechnique() const;

        const void* GetParamByIdx(IZ_UINT idx) const;
        const void* GetParamByName(IZ_PCSTR pszName) const;
        const void* GetParamByKey(const CKey& key) const;

        /** 半透明値をセット.
         */
        void SetAlpha(IZ_UINT8 alpha) { m_Alpha = alpha; }

        /** 半透明値を取得.
         */
        IZ_UINT8 GetAlpha() const { return m_Alpha; }

        /** 描画を有効にするかどうかを設定
         */
        void SetEnableRender(IZ_BOOL flag) { m_EnableRender = flag; }

        /** 描画が有効かどうか
         */
        IZ_BOOL EnableRender() const { return m_EnableRender; }

    private:
        IZ_BOOL Read(IInputStream* pIn);

        // S_MTRL_PARAMにバッファをセットする
        void AttachParamBuf();

    private:
        IMemoryAllocator* m_Allocator;

        IZ_BOOL m_EnableRender;

        // mtrlファイルから作成されたかどうか
        IZ_BOOL m_IsFromMtrlFile;

        IZ_UINT m_nAttachBufPos;

        S_MTRL_MATERIAL m_Header;

        S_MTRL_TEXTURE* m_pTexInfo;

        S_MTRL_SHADER* m_pShaderInfo;

        S_MTRL_PARAM* m_pParamInfo;
        IZ_UINT8* m_pParamBuf;

        // 透明度
        IZ_UINT8 m_Alpha;
    };
}   // namespace izanagi

#endif  // #ifndef(__IZANAGI_SCENEGRAPH_IZ_MATERIAL_H__)
