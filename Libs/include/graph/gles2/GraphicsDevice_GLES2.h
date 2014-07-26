#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_GLES2_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_GLES2_H__

#include "graph/GraphDefs.h"
#include "graph/GraphicsDevice.h"
#include "graph/RenderState.h"
#include "graph/ParamValueConverter.h"

namespace izanagi
{
namespace graph
{
    class CTextureGLES2;
    class CVertexBufferGLES2;
    class CIndexBufferGLES2;
    class CFrameBufferObject;

    // グラフィックスデバイス
    class CGraphicsDeviceGLES2 : public CGraphicsDevice
    {
        friend class CGraphicsDevice;
        friend class CTextureGLES2;
        friend class CVertexBufferGLES2;
        friend class CIndexBufferGLES2;

    protected:
        CGraphicsDeviceGLES2();
        virtual ~CGraphicsDeviceGLES2();

    public:
        // メモリからテクスチャ作成
        virtual CTexture* CreateTextureFromMemory(
            void* pData,
            IZ_UINT nDataSize,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ作成
        virtual CTexture* CreateTexture(
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            IZ_UINT nMipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE nRscType);

        virtual CTexture* CreateTexture(
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt,
            void* data);

        // キューブテクスチャ作成
        virtual CCubeTexture* CreateCubeTexture(
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            IZ_UINT nMipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            IZ_BOOL bIsDynamic);

        // レンダーターゲット作成
        virtual CRenderTarget* CreateRenderTarget(
            IZ_UINT nWidth, IZ_UINT nHeight,
            E_GRAPH_PIXEL_FMT fmt);

        // 頂点バッファ作成
        virtual CVertexBuffer* CreateVertexBuffer(
            IZ_UINT nStride,
            IZ_UINT nVtxNum,
            E_GRAPH_RSC_USAGE nCreateType);

        // インデックスバッファ作成
        virtual CIndexBuffer* CreateIndexBuffer(
            IZ_UINT nIdxNum,
            E_GRAPH_INDEX_BUFFER_FMT fmt,
            E_GRAPH_RSC_USAGE nCreateType);

        // シェーダプログラム作成
        virtual CShaderProgram* CreateShaderProgram();

        // 頂点シェーダ作成
        virtual CVertexShader* CreateVertexShader(const void* pProgram);

        // ピクセルシェーダ作成
        virtual CPixelShader* CreatePixelShader(const void* pProgram);

        // 頂点宣言作成
        virtual CVertexDeclaration* CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum);

    private:
        // リソース挿入
        template <class _T>
        _T* InsertResource(_T* p, _T* pListTop);

        // リソース削除
        template <class _T>
        _T* RemoveResource(_T* p, _T* pListTop);

        template <class _T>
        void DisableResource(_T* pList);

        template <class _T>
        void RestoreResource(_T* pList);

        void RemoveTexture(CTextureGLES2* p);
        void RemoveVertexBuffer(CVertexBufferGLES2* p);
        void RemoveIndexBuffer(CIndexBufferGLES2* p);

    protected:
        // 本体作成
        IZ_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);

        void OnTerminate();

    public:
        // リセット
        virtual IZ_BOOL Reset(const void* initialParam);

        virtual void Terminate();
        
        // 描画開始
        virtual IZ_BOOL BeginRender(
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor,
            IZ_FLOAT fClearZ,
            IZ_DWORD nClearStencil);

        // 描画終了
        virtual void EndRender();

        // クリア
        virtual void Clear(
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor,
            IZ_FLOAT fClearZ,
            IZ_DWORD nClearStencil);

        virtual IZ_BOOL BeginScene(
            CRenderTarget** pRT,
            IZ_UINT nCount,
            CRenderTarget* pDepth,
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0);

        // シーン描画終了
        virtual void EndScene(IZ_UINT flag = 0xffffffff);

        // 同期
        virtual IZ_BOOL Present();

    public:
        // 頂点バッファセット
        virtual IZ_BOOL SetVertexBuffer(
            IZ_UINT nStreamIdx,
            IZ_UINT nOffsetByte,
            IZ_UINT nStride,
            CVertexBuffer* pVB);

        // インデックスバッファセット
        virtual IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB);

        // シェーダプログラムセット
        virtual IZ_BOOL SetShaderProgram(CShaderProgram* program);

        // 頂点宣言セット
        virtual IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD);

        // インデックスバッファ描画
        virtual IZ_BOOL DrawIndexedPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT vtxOffset,
            IZ_UINT nVtxNum,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt);

        // インデックスバッファなし描画
        virtual IZ_BOOL DrawPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt);      

    public:
        // ビューポートセット
        virtual IZ_BOOL SetViewport(const SViewport& vp);

        // デフォルトのレンダーステートを設定
        virtual void SetDefaultRenderState();

        virtual void SetRenderState(const S_RENDER_STATE& sRS);

    private:
        // サンプラステート設定
        template <typename _SS, typename _T>
        void SetSamplerStateAddr(
            IZ_BOOL isPlane,
            _SS nSSType,
            _T& old_val, _T new_val);

        // サンプラステート設定
        template <typename _SS, typename _T>
        void SetSamplerStateFilter(
            IZ_BOOL isPlane,
            _SS nSSType,
            _T& old_val, _T new_val);

    public:
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

        virtual IZ_UINT GetBackBufferWidth() const
        {
            return m_ScreenWidth;
        }

        virtual IZ_UINT GetBackBufferHeight() const
        {
            return m_ScreenHeight;
        }

        // TODO
        virtual void* GetPlatformInterface() { return IZ_NULL; }

    private:
        virtual IZ_BOOL SetTextureInternal(IZ_UINT nStage, CBaseTexture* pTex);
        virtual void SetRenderTargetInternal(CRenderTarget** rt, IZ_UINT num);

        // 深度・ステンシルセット
        virtual void SetDepthStencil(CRenderTarget* rt);

    protected:
        static CFrameBufferObject* CreateFBO(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator);

        static CRenderTarget* CreateDummyRenderTarget(
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height);

    protected:
        Display m_Display;
        Surface m_Surface;
        Context m_Context;

        IZ_UINT m_ScreenWidth;
        IZ_UINT m_ScreenHeight;

        IZ_BOOL m_IsDirtyShaderProgram;
        IZ_BOOL m_IsDirtyTex[TEX_STAGE_NUM];

        GLint m_SamplerHandle[TEX_STAGE_NUM];

        IZ_BOOL m_TexSamplerParamInitialized[TEX_STAGE_NUM];

        CFrameBufferObject* m_FBO;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_GLES2_H__)
