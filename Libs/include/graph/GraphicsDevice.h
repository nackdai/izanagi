#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"
#include "graph/RenderState.h"
#include "graph/internal/RenderTargetManager.h"

namespace izanagi
{
namespace graph
{
    class CBaseTexture;
    class CTexture;
    class CRenderTarget;
    class CCubeTexture;
    class CVertexBuffer;
    class CIndexBuffer;;
    class CVertexDeclaration;
    class CVertexShader;
    class CPixelShader;
    class C2DRenderer;
    struct SVertexElement;

    /** グラフィックスデバイス
     */
    class CGraphicsDevice : public CObject
    {
    protected:
        static CGraphicsDevice* s_Instance;

    public:
        /**
         */
        static CGraphicsDevice* CreateGraphicsDevice(IMemoryAllocator* allocator);

    protected:
        CGraphicsDevice();
        virtual ~CGraphicsDevice();

        NO_COPIABLE(CGraphicsDevice);

        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        PURE_VIRTUAL(void ClearRenderState());

    public:
        // メモリからテクスチャ作成
        PURE_VIRTUAL(
            CTexture* CreateTextureFromMemory(
                void* pData,
                IZ_UINT nDataSize,
                E_GRAPH_PIXEL_FMT fmt));

        // テクスチャ作成
        PURE_VIRTUAL(
            CTexture* CreateTexture(
                IZ_UINT nWidth,
                IZ_UINT nHeight,
                IZ_UINT nMipLevel,
                E_GRAPH_PIXEL_FMT fmt,
                E_GRAPH_RSC_USAGE nRscType));

        // キューブテクスチャ作成
        PURE_VIRTUAL(
            CCubeTexture* CreateCubeTexture(
                IZ_UINT nWidth,
                IZ_UINT nHeight,
                IZ_UINT nMipLevel,
                E_GRAPH_PIXEL_FMT fmt,
                IZ_BOOL bIsDynamic));

        // レンダーターゲット作成
        PURE_VIRTUAL(
            CRenderTarget* CreateRenderTarget(
                IZ_UINT nWidth, IZ_UINT nHeight,
                E_GRAPH_PIXEL_FMT fmt));
        
        // システムメモリ上にテクスチャ作成
        PURE_VIRTUAL(
            CTexture* CreateTextureOnSysMem(
                IZ_UINT nWidth, IZ_UINT nHeight,
                E_GRAPH_PIXEL_FMT fmt,
                IZ_UINT nMipLevel));

        // 深度・ステンシルサーフェス作成
        PURE_VIRTUAL(
            CRenderTarget* CreateDepthStencilSurface(
                IZ_UINT width, 
                IZ_UINT height,
                E_GRAPH_PIXEL_FMT fmt));
        
        // 頂点バッファ作成
        PURE_VIRTUAL(
            CVertexBuffer* CreateVertexBuffer(
                IZ_UINT stride,
                IZ_UINT vtxNum,
                E_GRAPH_RSC_USAGE usage));

        // インデックスバッファ作成
        PURE_VIRTUAL(
            CIndexBuffer* CreateIndexBuffer(
                IZ_UINT nIdxNum,
                E_GRAPH_INDEX_BUFFER_FMT fmt,
                E_GRAPH_RSC_USAGE usage));

        // 頂点シェーダ作成
        PURE_VIRTUAL(CVertexShader* CreateVertexShader(const void* pProgram));

        // ピクセルシェーダ作成
        PURE_VIRTUAL(CPixelShader* CreatePixelShader(const void* pProgram));

        // 頂点宣言作成
        PURE_VIRTUAL(
            CVertexDeclaration* CreateVertexDeclaration(
                const SVertexElement* pElem, 
                IZ_UINT nNum));

    public:
        // リセット
        PURE_VIRTUAL(IZ_BOOL Reset(const void* initialParam));

        // 描画開始
        PURE_VIRTUAL(
            IZ_BOOL BeginRender(
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor,
                IZ_FLOAT fClearZ,
                IZ_DWORD nClearStencil));

        // 描画終了
        PURE_VIRTUAL(void EndRender());

        // クリア
        PURE_VIRTUAL(
            void Clear(
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor,
                IZ_FLOAT fClearZ,
                IZ_DWORD nClearStencil));

        // シーン描画開始
        IZ_BOOL BeginScene(
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0);

        IZ_BOOL BeginScene(
            CRenderTarget** pRT,
            IZ_UINT nCount,
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0);

        PURE_VIRTUAL(
            IZ_BOOL BeginScene(
                CRenderTarget** pRT,
                IZ_UINT nCount,
                CRenderTarget* pDepth,
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor = 0,
                IZ_FLOAT fClearZ = 1.0f,
                IZ_DWORD nClearStencil = 0));

        // シーン描画終了
        void EndScene(IZ_UINT flag = 0xffffffff);

        // 同期
        PURE_VIRTUAL(IZ_BOOL Present());

    public:
        // 2D描画開始
        IZ_BOOL Begin2D();

        // 2D描画終了
        IZ_BOOL End2D();

        // 2D描画バッファフラッシュ
        IZ_BOOL Flush2D();

        // 2Dスプライト描画
        IZ_BOOL Draw2DSprite(
            const CFloatRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));
        IZ_BOOL Draw2DSpriteEx(
            const CIntRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

        // 2D矩形描画
        IZ_BOOL Draw2DRect(
            const CIntRect& rcDst,
            const IZ_COLOR color);

        // 2Dライン描画
        IZ_BOOL Draw2DLine(
            const CIntPoint& ptStart,
            const CIntPoint& ptGoal,
            const IZ_COLOR color);

        // 2D描画モード
        void Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp);
        E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

    public:
        // テクスチャセット
        IZ_BOOL SetTexture(IZ_UINT nStage, CBaseTexture* pTex);

        // テクスチャ取得
        CBaseTexture* GetTexture(IZ_UINT nStage);

        // 頂点バッファセット
        PURE_VIRTUAL(
            IZ_BOOL SetVertexBuffer(
                IZ_UINT nStreamIdx,
                IZ_UINT nOffsetByte,
                IZ_UINT stride,
                CVertexBuffer* pVB));

        // インデックスバッファセット
        PURE_VIRTUAL(IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB));

        // 頂点宣言セット
        PURE_VIRTUAL(IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD));

        // 頂点シェーダセット
        PURE_VIRTUAL(IZ_BOOL SetVertexShader(CVertexShader* pVS));

        // ピクセルシェーダセット
        PURE_VIRTUAL(IZ_BOOL SetPixelShader(CPixelShader* pPS));

        // インデックスバッファ描画
        PURE_VIRTUAL(
            IZ_BOOL DrawIndexedPrimitive(
                E_GRAPH_PRIM_TYPE prim_type,
                IZ_UINT vtxOffset,
                IZ_UINT vtxNum,
                IZ_UINT idxOffset,
                IZ_UINT nPrimCnt));

        // インデックスバッファなし描画
        PURE_VIRTUAL(
            IZ_BOOL DrawPrimitive(
                E_GRAPH_PRIM_TYPE prim_type,
                IZ_UINT idxOffset,
                IZ_UINT nPrimCnt));

    public:
        // ビューポートセット
        PURE_VIRTUAL(IZ_BOOL SetViewport(const SViewport& vp));

        // デフォルトのレンダーステートを設定
        PURE_VIRTUAL(void SetDefaultRenderState());

        // レンダーステート取得
        virtual const S_RENDER_STATE& GetRenderState() const
        {
            return m_RenderState;
        }

        // レンダーステート保存
        IZ_BOOL SaveRenderState()
        {
            return m_RenderState.Save();
        }

        // レンダーステート復帰
        IZ_BOOL LoadRenderState()
        {
            return m_RenderState.Load(this);
        }

    public:
        // ビューポート取得
        const SViewport& GetViewport() const
        {
            return m_RenderState.vp;
        }

        // シザー矩形
        void SetScissorTestRect(const SIntRect& rc)
        {
            m_RenderState.SetScissorRect(this, rc);
        }

        // レンダーステート設定
        void SetRenderState(E_GRAPH_RENDER_STATE nState, IZ_DWORD val)
        {
            m_RenderState.SetRenderState(this, nState, val);
        }

        PURE_VIRTUAL(void SetRenderState(const S_RENDER_STATE& sRS));

    public:
        // レンダーターゲット取得
        CRenderTarget* GetRenderTarget(IZ_UINT idx)
        {
            IZ_ASSERT(idx < MAX_MRT_NUM);
            return m_RenderState.curRT[idx];
        }

        CRenderTarget* GetDepthSrencil()
        {
            return m_RenderState.curDepth;
        }

        // サンプラステート取得
        const S_SAMPLER_STATE& GetSamplerState(IZ_UINT nStage) const
        {
            IZ_ASSERT(nStage < TEX_STAGE_NUM);
            return m_SamplerState[nStage];
        }

        // リセット用コールバックセット
        virtual void SetResetCallBack(GraphicsDeviceResetCallBack pCallBack)
        {
            // Nothing...
        }

        // デバイスロスト用コールバックセット
        virtual void SetLostDeviceCallBack(GraphicsDeviceLostDeviceCallBack pCallBack)
        {
            // Nothing...
        }

        PURE_VIRTUAL(IZ_UINT GetBackBufferWidth() const);
        PURE_VIRTUAL(IZ_UINT GetBackBufferHeight() const);

        IMemoryAllocator* GetDeviceAllocator()
        {
            return m_Allocator;
        }

        virtual void* GetPlatformInterface() { return IZ_NULL; }

    protected:
        PURE_VIRTUAL(IZ_BOOL SetTextureInternal(IZ_UINT nStage, CBaseTexture* pTex));
        PURE_VIRTUAL(void SetRenderTargetInternal(CRenderTarget** rt, IZ_UINT num));

        // レンダーターゲットセット
        IZ_BOOL PushRenderTarget(CRenderTarget** rt, IZ_UINT num);
        void SetRenderTarget(CRenderTarget** rt, IZ_UINT num);

        // 深度・ステンシルセット
        IZ_BOOL PushDepthStencil(CRenderTarget* rt);
        PURE_VIRTUAL(void SetDepthStencil(CRenderTarget* rt));

    protected:
        enum {
            // レンダーターゲットのキュー数
            RT_QUEUE_MAX = 4,
        };

    protected:
        IMemoryAllocator* m_Allocator;

        // レンダーステート
        CRenderState m_RenderState;

        // テクスチャ
        CBaseTexture* m_Texture[TEX_STAGE_NUM];
        S_SAMPLER_STATE m_SamplerState[TEX_STAGE_NUM];

        // フレームバッファ
        CRenderTarget* m_RT;
        CRenderTarget* m_Depth;

        // レンダーターゲット管理
        CRenderTargetManager<RT_QUEUE_MAX> m_RTMgr[MAX_MRT_NUM];
        CRenderTargetManager<RT_QUEUE_MAX> m_DepthMgr;

        // 2D描画用
        C2DRenderer* m_2DRenderer;

        // フラグ
        struct {
            IZ_UINT is_call_begin       : 1;    // BeginSceneを呼んだかどうか
            IZ_UINT is_render_2d        : 1;    // 2D描画中か
            IZ_UINT is_force_set_state  : 1;    // 強制的にステートを設定するかどうか
            IZ_UINT is_lost_device      : 1;    // デバイスロストしているかどうか
            IZ_UINT reserved            : 28;
        } m_Flags;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
