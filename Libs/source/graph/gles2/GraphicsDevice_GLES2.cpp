#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/Surface_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/VertexShader_GLES2.h"
#include "graph/gles2/PixelShader_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/2DRenderer_GLES2.h"

namespace izanagi
{
namespace graph
{
    CGraphicsDevice* CGraphicsDevice::s_Instance = IZ_NULL;

    /**
    * インスタンス作成
    */
    CGraphicsDevice* CGraphicsDevice::CreateGraphicsDevice(IMemoryAllocator* allocator)
    {
        if (s_Instance != IZ_NULL) {
            // 作成済みなので何もしない
            return s_Instance;
        }

        IZ_ASSERT(allocator != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CGraphicsDeviceGLES2* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CGraphicsDeviceGLES2));
        VGOTO(buf != IZ_NULL, __EXIT__);

        // インスタンス作成
        instance = new(buf) CGraphicsDeviceGLES2;
        {
            instance->m_Allocator = allocator;

            // IDirect3D9 インターフェースの取得
            instance->m_D3D = Direct3DCreate9(D3D_SDK_VERSION);
            result = (instance->m_D3D != IZ_NULL);
            IZ_ASSERT(result);

            // 現在の画面モードの取得
            if (result) {
                HRESULT hr = instance->m_D3D->GetAdapterDisplayMode(
                                D3DADAPTER_DEFAULT,
                                &instance->m_DisplayMode);
                result = SUCCEEDED(hr);
                IZ_ASSERT(result);
            }

            instance->AddRef();
        }

        if (!result) {
            goto __EXIT__;
        }

        s_Instance = instance;
    
    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL) {
                allocator->Free(buf);
            }
        }

        return instance;
    }

    // コンストラクタ
    CGraphicsDeviceGLES2::CGraphicsDeviceGLES2()
    {
        m_D3D = IZ_NULL;

        m_Device = IZ_NULL;
        FILL_ZERO(&m_PresentParameters, sizeof(m_PresentParameters));

        m_hFocusWindow = IZ_NULL;;

        m_ResetCallBack = IZ_NULL;
        m_LostDeviceCallBack = IZ_NULL;
    }

    // デストラクタ
    CGraphicsDeviceGLES2::~CGraphicsDeviceGLES2()
    {
        SAFE_RELEASE(m_Device);
        SAFE_RELEASE(m_D3D);

        ClearRenderState();
    }

    void CGraphicsDeviceGLES2::ClearRenderState()
    {
        SetVertexShader(IZ_NULL);
        SetPixelShader(IZ_NULL);
        SetVertexBuffer(0, 0, 0, IZ_NULL);
        SetIndexBuffer(IZ_NULL);

        SAFE_RELEASE(m_RenderState.curVD);

        for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
            // チェック
            IZ_ASSERT(m_RTMgr[i].IsEmpty());

            SAFE_RELEASE(m_RenderState.curRT[i]);
        }

        // チェック
        IZ_ASSERT(m_DepthMgr.IsEmpty());

        SAFE_RELEASE(m_RenderState.curDepth);

        for (IZ_UINT i = 0; i < TEX_STAGE_NUM; ++i) {
            SetTexture(i, IZ_NULL);
        }
    }

    /**
    * リセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::Reset(const void* initialParam)
    {
        // NOTE
        // izanagiでは左手座標系なので
        // カリングの標準はCounterClockWiseにする
        glFrontFace(GL_CCW);

        const SGraphicsDeviceInitParams& param = *(reinterpret_cast<const SGraphicsDeviceInitParams*>(initialParam));

        IZ_BOOL ret = IZ_TRUE;

        if (m_Device == IZ_NULL) {
            // 本体作成
            ret = CreateBody(param);

            if (ret) {
                // 2D描画初期化
                m_2DRenderer = C2DRendererGLES2::Create2DRenderer(this, m_Allocator);
                ret = (m_2DRenderer != IZ_NULL);
                IZ_ASSERT(ret);
            }
        }
        else {
            // リセット
            ret = ResetInternal(param);
        }

        if (ret) {
            // ビューポート
            SViewport vp;
            {
                vp.x = 0;
                vp.y = 0;
                vp.width = m_PresentParameters.BackBufferWidth;
                vp.height = m_PresentParameters.BackBufferHeight;
                vp.minZ = 0.0f;
                vp.maxZ = 1.0f;
            }
            SetViewport(vp);

            // レンダーステートの現在値を取得
            m_RenderState.GetParamsFromGraphicsDevice(this);

            // ステート
            // 強制設定
            m_Flags.is_force_set_state = IZ_TRUE;
            SetDefaultRenderState();
            m_Flags.is_force_set_state = IZ_FALSE;

            // サーフェスのリセット
            //（フレームバッファのリセット）
            {
                CSurfaceGLES2*& renderTarget = reinterpret_cast<CSurfaceGLES2*&>(m_RT);
                CSurfaceGLES2*& deptthStencil = reinterpret_cast<CSurfaceGLES2*&>(m_Depth);

                if (m_RT != IZ_NULL) {
                    renderTarget->Reset(IZ_NULL, 0);
                }
                else {
                    m_RT = CSurfaceGLES2::CreateSurface(m_Allocator);
                }

                if (m_Depth != IZ_NULL) {
                    deptthStencil->Reset(IZ_NULL, 0);
                }
                else {
                    m_Depth = CSurfaceGLES2::CreateSurface(m_Allocator);
                }

                ret = ((m_RT != IZ_NULL) && (m_Depth != IZ_NULL));
                IZ_ASSERT(ret);
            
                if (ret) {
                    // フレームバッファサーフェースを取ってくる・・・
                    m_Device->GetRenderTarget(0, &renderTarget->m_Surface);
                    m_Device->GetDepthStencilSurface(&deptthStencil->m_Surface);

                    // 明示的に記述をセットする
                    renderTarget->SetDesc();
                    deptthStencil->SetDesc();

                    // 現在のレンダーターゲットとして保持
                    SAFE_REPLACE(m_RenderState.curRT[0], m_RT);
                    SAFE_REPLACE(m_RenderState.curDepth, m_Depth);
                }
            }
        }

        return ret;
    }

    // 本体作成
    IZ_BOOL CGraphicsDeviceGLES2::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        IZ_ASSERT(m_D3D != IZ_NULL);

        IZ_BOOL ret = IZ_TRUE;

        m_hFocusWindow = sParams.hFocusWindow;

        // D3DPRESENT_PARAMETERS の設定
        {
            FILL_ZERO(&m_PresentParameters, sizeof(m_PresentParameters));

            m_PresentParameters.BackBufferCount = 2;

            m_PresentParameters.Windowed = sParams.Windowed;                    // 画面モード(ウインドウモード)
            m_PresentParameters.BackBufferWidth = sParams.BackBufferWidth;      // バックバッファの幅
            m_PresentParameters.BackBufferHeight = sParams.BackBufferHeight;    // バックバッファの高さ

            // バックバッファのフォーマット
            m_PresentParameters.BackBufferFormat = m_DisplayMode.Format;

            m_PresentParameters.MultiSampleType = sParams.MultiSampleType;      // マルチ・サンプリングの種類

            // スワップエフェクトの種類
            m_PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;

            m_PresentParameters.hDeviceWindow = sParams.hDeviceWindow;

#if 1
            // Zバッファの自動作成
            m_PresentParameters.EnableAutoDepthStencil = IZ_TRUE;
            //m_PresentParameters.AutoDepthStencilFormat = D3DFMT_D24S8;
            m_PresentParameters.AutoDepthStencilFormat = sParams.DepthStencilFormat;
#else
            // Zバッファを自動作成しない
            m_PresentParameters.EnableAutoDepthStencil = IZ_FALSE;
#endif

            // VSyncのタイプ
            m_PresentParameters.PresentationInterval = sParams.PresentationInterval;
        }

        if (FAILED(m_D3D->CreateDevice(
                    sParams.Adapter, 
                    sParams.DeviceType,
                    sParams.hFocusWindow,
                    sParams.BehaviorFlags,
                    &m_PresentParameters,
                    &m_Device)))
        {
            // 結局失敗した
            ret = IZ_FALSE;
        }

        return ret;
    }

    template <typename _T>
    void CGraphicsDeviceGLES2::DisableResource(_T* pList)
    {
        for (_T* p = pList; p != IZ_NULL; p = p->m_Next) {
            p->Disable();
        }
    }

    template <typename _T>
    void CGraphicsDeviceGLES2::RestoreResource(_T* pList)
    {
        for (_T* p = pList; p != IZ_NULL; p = p->m_Next) {
            p->Restore();
        }
    }

    IZ_BOOL CGraphicsDeviceGLES2::ResetInternal(const SGraphicsDeviceInitParams& sParams)
    {
        CSurfaceGLES2* renderTarget = reinterpret_cast<CSurfaceGLES2*>(m_RT);
        CSurfaceGLES2* deptthStencil = reinterpret_cast<CSurfaceGLES2*>(m_Depth);

        // リソースを解放する
        {
            renderTarget->ReleaseResource();
            deptthStencil->ReleaseResource();

            m_2DRenderer->DisableResource();

            DisableResource(m_ResetTexture);
            DisableResource(m_ResetVB);
            DisableResource(m_ResetIB);
        }

        // クリアされるものを保存しておく
        IZ_UINT nBackBufferCount = m_PresentParameters.BackBufferCount;
        D3DFORMAT fmt = m_PresentParameters.BackBufferFormat;

        // 新しく設定されるもの
        m_PresentParameters.BackBufferWidth = sParams.BackBufferWidth;
        m_PresentParameters.BackBufferHeight = sParams.BackBufferHeight;

        HRESULT hr = m_Device->Reset(&m_PresentParameters);
        IZ_ASSERT(hr != D3DERR_DRIVERINTERNALERROR);

        IZ_BOOL ret = SUCCEEDED(hr);

        if (ret) {
            // クリアされたので設定値に変更する
            m_PresentParameters.BackBufferCount = nBackBufferCount;
            m_PresentParameters.BackBufferFormat = fmt;

            // リソースをリセットする
            {
                renderTarget->Reset(IZ_NULL, 0);
                deptthStencil->Reset(IZ_NULL, 0);

                m_2DRenderer->RestoreResource();

                RestoreResource(m_ResetTexture);
                RestoreResource(m_ResetVB);
                RestoreResource(m_ResetIB);
            }

            // コールバック
            if (m_ResetCallBack != IZ_NULL) {
                ret = (*m_ResetCallBack)();
                IZ_ASSERT(ret);
            }
        }

        return ret;
    }

    /**
    * 描画開始
    */
    IZ_BOOL CGraphicsDeviceGLES2::BeginRender(
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor,
        IZ_FLOAT fClearZ,
        IZ_DWORD nClearStencil)
    {
        IZ_ASSERT(m_Device != NULL);

        IZ_BOOL ret = IZ_FALSE;

        if (!m_Flags.is_lost_device) {
            // デバイスロストしてないので、通常の描画開始処理を行う
            m_Device->BeginScene();
            m_Flags.is_call_begin = IZ_TRUE;

            Clear(
                nClearFlags,
                nClearColor,
                fClearZ,
                nClearStencil);

            // フレームバッファサーフェスを現在のサーフェスとしてセット
            SAFE_REPLACE(m_RenderState.curRT[0], m_RT);
            SAFE_REPLACE(m_RenderState.curDepth, m_Depth);

            // 2D処理開始
            m_2DRenderer->BeginFrame();

            ret = IZ_TRUE;
        }

        return ret;
    }

    /**
    * 描画終了
    */
    void CGraphicsDeviceGLES2::EndRender()
    {
        // 念のため
        EndScene();

        if (m_Flags.is_call_begin) {
            m_Device->EndScene();
        }

        m_Flags.is_call_begin = IZ_FALSE;

        // クリアしてみる
        ClearRenderState();
    }

    #ifndef _CONV_CLEAR_FLAG
    #define _CONV_CLEAR_FLAG(flag, mask, dst)   ((flag) & (mask) ? (dst) : 0)
    #else
    IZ_C_ASSERT(IZ_FALSE);
    #endif

    /**
    * クリア
    */
    void CGraphicsDeviceGLES2::Clear(
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor,
        IZ_FLOAT fClearZ,
        IZ_DWORD nClearStencil)
    {
        IZ_ASSERT(m_Device != NULL);

        if (nClearFlags > 0) {
            // DirectX のクリアフラグに変換する
            IZ_DWORD nDXClearFlag = 0;
            nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_COLOR,   D3DCLEAR_TARGET);
            nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_DEPTH,   D3DCLEAR_ZBUFFER);
            nDXClearFlag |= _CONV_CLEAR_FLAG(nClearFlags, E_GRAPH_CLEAR_FLAG_STENCIL, D3DCLEAR_STENCIL);

            m_Device->Clear(
                0,
                NULL,
                nDXClearFlag,
                nClearColor,
                fClearZ,
                nClearStencil);
        }
    }

    IZ_BOOL CGraphicsDeviceGLES2::BeginScene(
        CSurface** pRT,
        IZ_UINT nCount,
        CSurface* pDepth,
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor/*= 0*/,
        IZ_FLOAT fClearZ/*= 1.0f*/,
        IZ_DWORD nClearStencil/*= 0*/)
    {
        IZ_ASSERT(m_Device != NULL);

        // TODO
        // MRTは無しで・・・
        IZ_ASSERT(nCount <= 1);

        IZ_BOOL ret = IZ_TRUE;

        if ((nCount > 0) && (pRT != IZ_NULL)) {
            // レンダーターゲットセット
            PushRenderTarget(pRT, nCount);
        }
        if (pDepth != IZ_NULL) {
            // 深度・ステンシルセット
            PushDepthStencil(pDepth);
        }

        // クリア
        Clear(
            nClearFlags,
            nClearColor,
            fClearZ,
            nClearStencil);

        return ret;
    }

    /**
    * シーン描画終了
    */
    void CGraphicsDeviceGLES2::EndScene(IZ_UINT flag/* = 0xffffffff*/)
    {
        CSurface* pRTList[MAX_MRT_NUM];
        memset(pRTList, 0, sizeof(pRTList));

        IZ_UINT nRTNum = 0;

        // レンダーターゲット
        for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
            if ((flag & (1 << i)) > 0) {
                pRTList[i] = m_RTMgr[i].Pop();
                nRTNum = (pRTList[i] != IZ_NULL ? nRTNum + 1 : nRTNum);
            }
        }

        if (nRTNum > 0) {
            SetRenderTarget(pRTList, nRTNum);
        }

        if ((flag & E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL) > 0) {
            // 深度・ステンシル
            CSurface* pDepth = m_DepthMgr.Pop();
            if (pDepth != IZ_NULL) {
                SetDepthStencil(pDepth);
            }
        }
    }

    /**
    * 同期
    */
    IZ_BOOL CGraphicsDeviceGLES2::Present()
    {
        IZ_BOOL ret = IZ_TRUE;

        HRESULT hr = m_Device->Present(
                        IZ_NULL,
                        IZ_NULL,
                        m_hFocusWindow,
                        IZ_NULL);

        if (hr == D3DERR_DEVICELOST) {
            // デバイスロストした
            m_Flags.is_lost_device = IZ_TRUE;

            // デバイスロストしたときのコールバック
            if (m_LostDeviceCallBack != IZ_NULL) {
                (*m_LostDeviceCallBack)();
            }

            SGraphicsDeviceInitParams sParams;
            {
                sParams.BackBufferWidth = m_PresentParameters.BackBufferWidth;
                sParams.BackBufferHeight = m_PresentParameters.BackBufferHeight;
            }

            // 復帰をこころみる
            ret = Reset(&sParams);

            if (ret) {
                // 復帰した
                m_Flags.is_lost_device = IZ_FALSE;
            }
        }
        else {
            ret = SUCCEEDED(hr);
        }

        IZ_ASSERT(ret);
        return ret;
    }

    void CGraphicsDeviceGLES2::SetDepthStencil(CSurface* pSurface)
    {
        IZ_ASSERT(pSurface != IZ_NULL);

        if (m_RenderState.curDepth != pSurface) {
            // レンダーターゲットを入れ替える
            CSurfaceGLES2* gles2Surface = reinterpret_cast<CSurfaceGLES2*>(pSurface);
            m_Device->SetDepthStencilSurface(gles2Surface->GetRawInterface());
            SAFE_REPLACE(m_RenderState.curDepth, pSurface);
        }
    }

    /**
    * 頂点バッファセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetVertexBuffer(
        IZ_UINT nStreamIdx,
        IZ_UINT nOffsetByte,
        IZ_UINT nStride,
        CVertexBuffer* pVB)
    {
        if (m_RenderState.curVB == pVB) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CVertexBufferGLES2* gles2VB = reinterpret_cast<CVertexBufferGLES2*>(pVB);

        {
            HRESULT hr = m_Device->SetStreamSource(
                            nStreamIdx,
                            pVB != IZ_NULL ? gles2VB->GetRawInterface() : IZ_NULL,
                            nOffsetByte,
                            nStride);
            VRETURN(SUCCEEDED(hr));

            // 現在設定されているものとして保持
            SAFE_REPLACE(m_RenderState.curVB, pVB);
        }

        return IZ_TRUE;
    }

    /**
    * インデックスバッファセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetIndexBuffer(CIndexBuffer* pIB)
    {
        if (m_RenderState.curIB == pIB) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CIndexBufferGLES2* gles2IB = reinterpret_cast<CIndexBufferGLES2*>(pIB);

        {
            HRESULT hr = m_Device->SetIndices(
                            pIB != IZ_NULL ? gles2IB->GetRawInterface() : IZ_NULL);
            VRETURN(SUCCEEDED(hr));

            // 現在設定されているものとして保持
            SAFE_REPLACE(m_RenderState.curIB, pIB);
        }

        return IZ_TRUE;
    }

    /**
    * 頂点シェーダセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetVertexShader(CVertexShader* pVS)
    {
        if (m_RenderState.curVS == pVS) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CVertexShaderGLES2* gles2VS = reinterpret_cast<CVertexShaderGLES2*>(pVS);

        {
            HRESULT hr = m_Device->SetVertexShader(
                            pVS != IZ_NULL ? gles2VS->GetRawInterface() : IZ_NULL);
            VRETURN(SUCCEEDED(hr));

            // 現在設定されているものとして保持
            SAFE_REPLACE(m_RenderState.curVS, pVS);
        }

        return IZ_TRUE;
    }

    /**
    * ピクセルシェーダセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetPixelShader(CPixelShader* pPS)
    {
        if (m_RenderState.curPS == pPS) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CPixelShaderGLES2* gles2PS = reinterpret_cast<CPixelShaderGLES2*>(pPS);

        {
            HRESULT hr = m_Device->SetPixelShader(
                            pPS != IZ_NULL ? gles2PS->GetRawInterface() : IZ_NULL);
            VRETURN(SUCCEEDED(hr));

            // 現在設定されているものとして保持
            SAFE_REPLACE(m_RenderState.curPS, pPS);
        }

        return IZ_TRUE;
    }

    /**
    * 頂点宣言セット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetVertexDeclaration(CVertexDeclaration* pVD)
    {
        if (m_RenderState.curVD == pVD) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CVertexDeclarationGLES2* gles2VD = reinterpret_cast<CVertexDeclarationGLES2*>(pVD);

        if (pVD != IZ_NULL) {
            HRESULT hr = m_Device->SetVertexDeclaration(gles2VD->GetRawInterface());
            VRETURN(SUCCEEDED(hr));
        }

        // 現在設定されているものとして保持
        SAFE_REPLACE(m_RenderState.curVD, pVD);

        return IZ_TRUE;
    }

    /**
    * インデックスバッファ描画
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawIndexedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT nBaseIdx,
        IZ_UINT nMinIdx,
        IZ_UINT nVtxNum,
        IZ_UINT nStartIdx,
        IZ_UINT nPrimCnt)
    {
        HRESULT hr = m_Device->DrawIndexedPrimitive(
                        IZ_GET_TARGET_PRIM_TYPE(prim_type),
                        nBaseIdx,
                        nMinIdx,
                        nVtxNum,
                        nStartIdx,
                        nPrimCnt);
        VRETURN(SUCCEEDED(hr));

        return IZ_TRUE;
    }

    /**
    * インデックスバッファなし描画
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT nStartIdx,
        IZ_UINT nPrimCnt)
    {
        HRESULT hr = m_Device->DrawPrimitive(
                        IZ_GET_TARGET_PRIM_TYPE(prim_type),
                        nStartIdx,
                        nPrimCnt);
        VRETURN(SUCCEEDED(hr));

        return IZ_TRUE;
    }

    /**
    * ビューポートセット
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetViewport(const SViewport& vp)
    {
        if (m_Flags.is_render_2d) {
            // TODO
            // 2D描画中は不可
            return IZ_TRUE;
        }

        IZ_BOOL ret = IZ_TRUE;
        HRESULT hr = S_OK;

        if ((m_RenderState.vp.width != vp.width)
            || (m_RenderState.vp.height != vp.height)
            || (m_RenderState.vp.x != vp.x)
            || (m_RenderState.vp.y != vp.y)
            || (m_RenderState.vp.minZ != vp.minZ)
            || (m_RenderState.vp.maxZ != vp.maxZ))
        {
            D3D_VIEWPORT sD3DViewport;
            {
                sD3DViewport.X = vp.x;
                sD3DViewport.Y = vp.y;
                sD3DViewport.Width = vp.width;
                sD3DViewport.Height = vp.height;
                sD3DViewport.MinZ = vp.minZ;
                sD3DViewport.MaxZ = vp.maxZ;
            }

            hr = m_Device->SetViewport(&sD3DViewport);

            ret = SUCCEEDED(hr);
            if (ret) {
                memcpy(&m_RenderState.vp, &vp, sizeof(vp));
            }
        }

        IZ_ASSERT(ret);
        return ret;
    }

    /**
    * デフォルトのレンダーステートを設定
    */
    void CGraphicsDeviceGLES2::SetDefaultRenderState()
    {
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ZWRITEENABLE, IZ_TRUE);
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ZENABLE, IZ_TRUE);
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ZFUNC, E_GRAPH_CMP_FUNC_LESSEQUAL);

        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ALPHATESTENABLE, IZ_TRUE);
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ALPHAREF, 0);
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ALPHAFUNC, E_GRAPH_CMP_FUNC_ALWAYS);

        CGraphicsDevice::SetRenderState(E_GRAPH_RS_ALPHABLENDENABLE, IZ_TRUE);
        CGraphicsDevice::SetRenderState(E_GRAPH_RS_BLENDMETHOD, E_GRAPH_ALPHA_BLEND_NORMAL);

        CGraphicsDevice::SetRenderState(E_GRAPH_RS_FILLMODE, E_GRAPH_FILL_MODE_SOLID);

        CGraphicsDevice::SetRenderState(E_GRAPH_RS_CULLMODE, E_GRAPH_CULL_DEFAULT);

        // うーん・・・
        for (IZ_UINT i = 0; i < TEX_STAGE_NUM; ++i) {
            // MIN_FILTER
            SetSamplerStateFilter(
                i, 
                E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
                m_SamplerState[i].minFilter,
                E_GRAPH_TEX_FILTER_LINEAR);

            // MAG_FILTER
            SetSamplerStateFilter(
                i,
                E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
                m_SamplerState[i].magFilter,
                E_GRAPH_TEX_FILTER_LINEAR);

            // MIP_FILTER
            SetSamplerStateFilter(
                i,
                E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,
                m_SamplerState[i].mipFilter,
                E_GRAPH_TEX_FILTER_LINEAR);

            // ADDRESS_U
            SetSamplerStateAddr(
                i,
                E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU,
                m_SamplerState[i].addressU,
                E_GRAPH_TEX_ADDRESS_CLAMP);

            // ADDRESS_V
            SetSamplerStateAddr(
                i, 
                E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
                m_SamplerState[i].addressV,
                E_GRAPH_TEX_ADDRESS_CLAMP);
        }
    }

    /**
    * レンダーステート一括セット
    */
    void CGraphicsDeviceGLES2::SetRenderState(const S_RENDER_STATE& sRS)
    {
        SetViewport(sRS.vp);

        for (IZ_UINT i = 0; i < E_GRAPH_RS_NUM; ++i) {
            CGraphicsDevice::SetRenderState(
                static_cast<E_GRAPH_RENDER_STATE>(i),
                sRS.dwRS[i]);
        }

        // シザー矩形
        SetScissorTestRect(sRS.rcScissor);
    }

    IZ_BOOL CGraphicsDeviceGLES2::SetTextureInternal(IZ_UINT nStage, CBaseTexture* pTex)
    {
        HRESULT hr = m_Device->SetTexture(
            nStage,
            pTex != NULL ? pTex->GetTexHandle() : NULL);
        VRETURN(SUCCEEDED(hr));

        // 保持しておく
        m_Texture[nStage] = pTex;

        // うーん・・・
        // ステート
        if (pTex != NULL) {
            // MIN_FILTER
            SetSamplerStateFilter(
                nStage,
                E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
                m_SamplerState[nStage].minFilter,
                pTex->GetState().minFilter);

            // MAG_FILTER
            SetSamplerStateFilter(
                nStage,
                E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
                m_SamplerState[nStage].magFilter,
                pTex->GetState().magFilter);

            // MIP_FILTER
            if (pTex->GetMipMapNum() > 1) {
                SetSamplerStateFilter(
                    nStage,
                    E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,
                    m_SamplerState[nStage].mipFilter,
                    pTex->GetState().mipFilter);
            }

            // ADDRESS_U
            SetSamplerStateAddr(
                nStage,
                E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU,
                m_SamplerState[nStage].addressU,
                pTex->GetState().addressU);

            // ADDRESS_V
            SetSamplerStateAddr(
                nStage,
                E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
                m_SamplerState[nStage].addressV,
                pTex->GetState().addressV);
        }

        return IZ_TRUE;
    }

    void CGraphicsDeviceGLES2::SetRenderTargetInternal(CSurface** pSurface, IZ_UINT num)
    {
        // レンダーターゲットを入れ替える
        for (IZ_UINT i = 0; i < num; ++i) {
            if (m_RenderState.curRT[i] != pSurface[i]) {
                CSurfaceGLES2* gles2Surface = reinterpret_cast<CSurfaceGLES2*>(pSurface[i]);
                IZ_ASSERT(gles2Surface->GetRawInterface() != IZ_NULL);

                m_Device->SetRenderTarget(i, gles2Surface->GetRawInterface());
                SAFE_REPLACE(m_RenderState.curRT[i], pSurface[i]);
            }
        }
    }
}   // namespace graph
}   // namespace izanagi
