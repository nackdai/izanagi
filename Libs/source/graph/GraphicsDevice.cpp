#include "graph/GraphicsDevice.h"
#include "graph/VertexBuffer.h"
#include "graph/IndexBuffer.h"
#include "graph/VertexDeclaration.h"
#include "graph/BaseTexture.h"
#include "graph/VertexShader.h"
#include "graph/PixelShader.h"
#include "graph/ShaderProgram.h"
#include "graph/2d/2DRenderer.h"

namespace izanagi
{
namespace graph
{
    // コンストラクタ
    CGraphicsDevice::CGraphicsDevice()
    {
        m_Allocator = IZ_NULL;

        m_RT = IZ_NULL;
        m_Depth = IZ_NULL;
    
        FILL_ZERO(m_Texture, sizeof(m_Texture));
        FILL_ZERO(m_SamplerState, sizeof(m_SamplerState));

        m_Flags.is_call_begin = IZ_FALSE;
        m_Flags.is_render_2d = IZ_FALSE;
        m_Flags.is_force_set_state = IZ_FALSE;
        m_Flags.is_lost_device = IZ_FALSE;
    }

    // デストラクタ
    CGraphicsDevice::~CGraphicsDevice()
    {
        SAFE_RELEASE(m_RT);
        SAFE_RELEASE(m_Depth);
    }

    C2DRenderer* CGraphicsDevice::Create2DRenderer()
    {
        C2DRenderer* ret = C2DRenderer::Create2DRenderer(
            this,
            m_Allocator);
        IZ_ASSERT(ret != IZ_NULL);

        return ret;
    }

    // シーン描画開始
    IZ_BOOL CGraphicsDevice::BeginScene(
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor/*= 0*/,
        IZ_FLOAT fClearZ/*= 1.0f*/,
        IZ_DWORD nClearStencil/*= 0*/)
    {
        return BeginScene(
                IZ_NULL, 0,
                IZ_NULL,
                nClearFlags,
                nClearColor,
                fClearZ,
                nClearStencil);
    }

    // シーン描画開始
    IZ_BOOL CGraphicsDevice::BeginScene(
        CRenderTarget** pRT,
        IZ_UINT nCount,
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor/*= 0*/,
        IZ_FLOAT fClearZ/*= 1.0f*/,
        IZ_DWORD nClearStencil/*= 0*/)
    {
        return BeginScene(
                pRT, nCount,
                IZ_NULL,
                nClearFlags,
                nClearColor,
                fClearZ,
                nClearStencil);
    }

    // テクスチャセット
    IZ_BOOL CGraphicsDevice::SetTexture(IZ_UINT nStage, CBaseTexture* pTex)
    {
        if (m_Texture[nStage] == pTex) {
            // すでにセットされている
            return IZ_TRUE;
        }

        VRETURN(SetTextureInternal(nStage, pTex));

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDevice::SetVertexBufferInstanced(
        IZ_UINT streamIdx,
        E_GRAPH_VB_USAGE usage,
        IZ_UINT divisor)
    {
        // Not supported...
        IZ_ASSERT(IZ_FALSE);
        return IZ_TRUE;
    }

    // テクスチャ取得
    CBaseTexture* CGraphicsDevice::GetTexture(IZ_UINT nStage)
    {
        IZ_ASSERT(nStage < TEX_STAGE_NUM);
        return m_Texture[nStage];
    }

    // シェーダプログラム取得
    CShaderProgram* CGraphicsDevice::GetShaderProgram()
    {
        return m_RenderState.curShader;
    }

    // レンダーターゲットセット
    IZ_BOOL CGraphicsDevice::PushRenderTarget(CRenderTarget** rt, IZ_UINT num)
    {
        IZ_ASSERT(rt != IZ_NULL);

        // TODO
        // MRTは許さない
        IZ_ASSERT(num == 1);
        IZ_ASSERT(num < MAX_MRT_NUM);

        IZ_BOOL ret = IZ_FALSE;

        // 現在のレンダーターゲットをプッシュ
        for (IZ_UINT i = 0; i < num; ++i) {
            CRenderTarget* curRT = GetRenderTarget(i);

            if ((rt != IZ_NULL) && (curRT != rt[i]))
            {
                ret = m_RTMgr[i].Push(curRT);
                if (!ret) {
                    break;
                }
            }
        }

        if (ret) {
            SetRenderTarget(rt, num);
        }

        return ret;
    }

    void CGraphicsDevice::SetRenderTarget(CRenderTarget** rt, IZ_UINT num)
    {
        // レンダーターゲットを入れ替える
        SetRenderTargetInternal(rt, num);

        // TODO
        // 強制的に０番目のサーフェスのサイズにビューポートを変換する
        const SViewport& curVp = GetViewport();
        SViewport vp;
        memcpy(&vp, &curVp, sizeof(vp));
        vp.x = vp.y = 0;
        vp.width = rt[0]->GetWidth();
        vp.height = rt[0]->GetHeight();
        SetViewport(vp);
    }

    // 深度・ステンシルセット
    IZ_BOOL CGraphicsDevice::PushDepthStencil(CRenderTarget* rt)
    {
        IZ_BOOL ret = IZ_FALSE;

        CRenderTarget* curDepthRT = GetDepthSrencil();

        // 現在の深度をプッシュ
        if ((rt != IZ_NULL)
            && (curDepthRT != rt))
        {
            ret = m_DepthMgr.Push(curDepthRT);
        }

        if (ret) {
            SetDepthStencil(rt);
        }

        return ret;
    }

}   // namespace graph
}   // namespace izanagi
