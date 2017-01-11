#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/PixelShader_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/RenderTarget_GLES2.h"

#include "graph/GraphUtil.h"

namespace izanagi
{
namespace graph
{
    /**
    * メモリからテクスチャ作成
    */
    CTexture* CGraphicsDeviceGLES2::CreateTextureFromMemory(
        void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        CTexture* pTexture = CTextureGLES2::CreateTextureFromMemory(
                                this,
                                m_Allocator,
                                data,
                                dataSize,
                                fmt);
        return pTexture;
    }

    /**
    * テクスチャ作成
    */
    CTexture* CGraphicsDeviceGLES2::CreateTexture(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        CTextureGLES2* pTexture = CTextureGLES2::CreateTexture(
                                this,
                                m_Allocator,
                                width, height,
                                mipLevel,
                                fmt,
                                rscType);

        return pTexture;
    }

    CTexture* CGraphicsDeviceGLES2::CreateTexture(
        IZ_UINT width,
        IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt,
        void* data)
    {
        CTexture* texture = CreateTexture(
            width, height, 1,
            fmt,
            E_GRAPH_RSC_USAGE_STATIC);
        VRETURN_NULL(texture != IZ_NULL);

        if (data) {
            void* dst = IZ_NULL;
            VRETURN_NULL(
                texture->Lock(0, (void**)&dst, IZ_FALSE));

            IZ_UINT bpp = CGraphUtil::GetBPP(fmt);
            IZ_UINT size = width * height * bpp;

            memcpy(dst, data, size);

            texture->Unlock(0);
        }

        return texture;
    }

    /**
    * キューブテクスチャ作成
    */
    CCubeTexture* CGraphicsDeviceGLES2::CreateCubeTexture(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        IZ_BOOL bIsDynamic)
    {
        CCubeTexture* pTexture = CCubeTextureGLES2::CreateCubeTexture(
                                    this,
                                    m_Allocator,
                                    width,
                                    height,
                                    mipLevel,
                                    fmt);
        return pTexture;
    }

    /**
    * レンダーターゲット作成
    */
    CRenderTarget* CGraphicsDeviceGLES2::CreateRenderTarget(
        IZ_UINT width, IZ_UINT height,
        E_GRAPH_PIXEL_FMT fmt)
    {
        CRenderTargetGLES2* pRT = IZ_NULL;

        if (fmt == E_GRAPH_PIXEL_FMT_D24S8) {
            pRT = CRenderTargetGLES2::CreateDepthStencilRenderTarget(
                this,
                m_Allocator,
                width,
                height);
        }
        else {
            pRT = CRenderTargetGLES2::CreateRenderTarget(
                this,
                m_Allocator,
                width,
                height,
                fmt);
        }

        return pRT;
    }

    /**
    * 頂点バッファ作成
    */
    CVertexBuffer* CGraphicsDeviceGLES2::CreateVertexBuffer(
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        CVertexBufferGLES2* pVB = CVertexBufferGLES2::CreateVertexBuffer(
                                this,
                                m_Allocator,
                                stride,
                                vtxNum,
                                usage);

        return pVB;
    }

    /**
    * インデックスバッファ作成
    */
    CIndexBuffer* CGraphicsDeviceGLES2::CreateIndexBuffer(
        IZ_UINT nIdxNum,
        E_GRAPH_INDEX_BUFFER_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        CIndexBufferGLES2* pIB = CIndexBufferGLES2::CreateIndexBuffer(
                                this,
                                m_Allocator,
                                nIdxNum,
                                fmt,
                                usage);

        return pIB;
    }

    // シェーダプログラム作成
    CShaderProgram* CGraphicsDeviceGLES2::CreateShaderProgram()
    {
        CShaderProgram* ret = CShaderProgramGLES2::CreateShaderProgram(m_Allocator);
        return ret;
    }

    /**
    * 頂点シェーダ作成
    */
    CVertexShader* CGraphicsDeviceGLES2::CreateVertexShader(const void* pProgram)
    {
        CVertexShader* ret = CVertexShaderGLES2::CreateVertexShader(
                                this,
                                m_Allocator,
                                pProgram);
        return ret;
    }

    /**
    * ピクセルシェーダ作成
    */
    CPixelShader* CGraphicsDeviceGLES2::CreatePixelShader(const void* pProgram)
    {
        CPixelShader* ret = CPixelShaderGLES2::CreatePixelShader(
                                this,
                                m_Allocator,
                                pProgram);
        return ret;
    }

    /**
    * 頂点宣言作成
    */
    CVertexDeclaration* CGraphicsDeviceGLES2::CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum)
    {
        CVertexDeclaration* ret = CVertexDeclarationGLES2::CreateVertexDeclaration(
                                    m_Allocator,
                                    pElem,
                                    nNum);
        return ret;
    }
}   // namespace graph
}   // namespace izanagi
