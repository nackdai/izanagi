#if !defined(__IZANAGI_SHADER_SHADER_BASIC_H__)
#define __IZANAGI_SHADER_SHADER_BASIC_H__

#include "izStd.h"
#include "izGraph.h"

#include "Shader.h"
#include "table/ShaderParameterTable.h"
#include "table/ShaderSamplerTable.h"
#include "table/ShaderStringBuffer.h"
#include "table/ShaderPassTable.h"
#include "table/ShaderTechTable.h"
#include "table/ShaderTextureTable.h"
#include "table/ShaderTextureTable.h"
#include "table/ShaderAttrTable.h"
#include "ShaderPass.h"
#include "ShaderDefs.h"

namespace izanagi
{
    class IInputStream;

namespace shader
{
    /**
    */
    class CShaderBasic : public IShader {
    public:
        /**
        * Creates an instance of CShaderBasic.
        */
        template <typename _SHADER>
        static _SHADER* CreateShader(
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IInputStream* pIn)
        {
            void* pBuf = ALLOC_ZERO(pAllocator, sizeof(_SHADER));
            VRETURN_NULL(pBuf != IZ_NULL);

            _SHADER* pInstance = new(pBuf) _SHADER;
            {
                pInstance->AddRef();
                pInstance->m_Allocator = pAllocator;

                if (!pInstance->Init(pDevice, pIn)) {
                    SAFE_RELEASE(pInstance);
                    IZ_ASSERT(IZ_FALSE);
                }
            }

            return pInstance;
        }

    protected:
        CShaderBasic();
        virtual ~CShaderBasic();

        NO_COPIABLE(CShaderBasic);

    protected:
        IZ_BOOL Init(
            graph::CGraphicsDevice* pDevice,
            IInputStream* pIn);

        IZ_UINT8* CreatePass(
            graph::CGraphicsDevice* pDevice,
            IInputStream* pIn,
            IZ_UINT8* pBuffer);

    public:
        IZ_PCSTR GetName() const { return m_Header.name.GetString(); }
        IZ_UINT GetKey() const { return m_Header.nameKey; }

        /**
         * シェーダの名前を設定
         */
        IZ_BOOL SetName(IZ_PCSTR name);

        virtual IZ_UINT Begin(
            graph::CGraphicsDevice* device,
            IZ_UINT nTechIdx,
            IZ_BOOL bIsSaveState);
        virtual IZ_BOOL End(graph::CGraphicsDevice* device);

        virtual IZ_BOOL BeginPass(IZ_UINT nPassIdx);
        virtual IZ_BOOL EndPass();

        virtual IZ_BOOL CommitChanges(graph::CGraphicsDevice* device);

        virtual IZ_UINT GetTechNum() const;

        virtual IZ_UINT GetAttrNum() const;
        virtual IZ_PCSTR GetAttrName(IZ_UINT idx) const;
        virtual IZ_BOOL CmpAttr(const CShaderAttr& attr) const;
        virtual IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_UINT val) const;
        virtual IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_FLOAT val) const;
        virtual IZ_BOOL CmpAttrValue(IZ_PCSTR attrName, IZ_BOOL val) const;

    private:
        template <typename _T>
        IZ_BOOL CmpAttrValue(
            IZ_PCSTR attrName, 
            E_SHADER_PARAMETER_TYPE type,
            _T val) const;

    public:
        virtual IZ_SHADER_HANDLE GetParameterByName(IZ_PCSTR pszName);
        virtual IZ_SHADER_HANDLE GetParameterBySemantic(IZ_PCSTR pszSemantic);

        IZ_BOOL GetParameterDesc(
            IZ_SHADER_HANDLE handle, 
            SShaderParamDesc& sDesc) const;

        IZ_BOOL SetParamValue(
            IZ_SHADER_HANDLE hParam,
            const void* pValue,
            IZ_UINT nBytes);

        IZ_BOOL SetParamValue(
            IZ_PCSTR name,
            const void* value,
            IZ_UINT bytes);

        // シェーダで利用するテクスチャを設定.
        IZ_BOOL SetTexture(
            IZ_SHADER_HANDLE hTex,
            graph::CBaseTexture* pTex);

        // シェーダで利用するテクスチャを設定.
        IZ_BOOL SetTexture(
            IZ_PCSTR name,
            graph::CBaseTexture* tex);

        // シェーダで利用するテクスチャをサンプラに設定.
        IZ_BOOL SetTextureToSampler(
            IZ_SHADER_HANDLE hSmpl,
            graph::CBaseTexture* pTex);

        // シェーダで利用するテクスチャをサンプラに設定.
        IZ_BOOL SetTextureToSampler(
            IZ_PCSTR name,
            graph::CBaseTexture* tex);

    private:
        IZ_BOOL SetParamValue(
            graph::CGraphicsDevice* device,
            IZ_UINT idx,
            CShaderPass& cPass,
            graph::CShaderProgram* pShd);

    protected:
        IZ_UINT8* m_pBuffer;

        S_SHD_HEADER m_Header;

        CShaderStringBuffer   m_StringBuffer;   // strings of shader.
        CShaderParameterTable m_ParamTbl;       // paramters of shader.
        CShaderTextureTable   m_TexTbl;         // textures of shader.
        CShaderSamplerTable   m_SmplTbl;        // samplers of shader.
        CShaderPassTable      m_PassTbl;        // passed of shader.
        CShaderTechTable      m_TechTbl;        // techniques of shader.
        CShaderAttrTable      m_AttrTbl;

        CShaderPass* m_pPass;

        IZ_BOOL m_bIsSavedRS;
        IZ_INT16 m_nCurTech;
        IZ_INT16 m_nCurPass;
    };
}   // namespace shader
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SHADER_SHADER_BASIC_H__)
