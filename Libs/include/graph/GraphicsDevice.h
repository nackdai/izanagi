#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_H__

#include "izDefs.h"
#include "izStd.h"
#include "graph/GraphDefs.h"
#include "graph/RenderState.h"
#include "graph/RenderTargetManager.h"

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
    class CShaderProgram;
    class CVertexShader;
    class CPixelShader;
    class C2DRenderer;
    struct SVertexElement;

    /** Applications use this class for rendering primitives, creating resourcing, managing rendering state etc.
     */
    class CGraphicsDevice : public CObject
    {
    protected:
        static CGraphicsDevice* s_Instance;

    public:
        /** Create an instance of graphics device.
         *
         * @param [in] allocator Memory allocator.
         * @return If the method succeeds, the return value is a pointer to an instance of graphics device. If the method fails, the return value is IZ_NULL.
         */
        static CGraphicsDevice* CreateGraphicsDevice(IMemoryAllocator* allocator);

    protected:
        CGraphicsDevice();
        virtual ~CGraphicsDevice();

        NO_COPIABLE(CGraphicsDevice);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** Create a texture resource from memory data.
         *
         * @param [in] pData
         * @param [in] nDataSize
         * @param [in] fmt The format of all levels in the texture.
         * @return If the method succeeds, the return value is a pointer to a texture resource. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CTexture* CreateTextureFromMemory(
                void* pData,
                IZ_UINT nDataSize,
                E_GRAPH_PIXEL_FMT fmt));

        /** Create a texture resource.
         *
         * @param [in] nWidth Width of the top-level of the texture, in pixels.
         * @param [in] nHeight Height of the top-level of the texture, in pixels.
         * @param [in] nMipLevel Number of mipmap levels in the texture.
         * @param [in] fmt The format of all levels in the texture.
         * @param [in] nRscType 
         * @return If the method succeeds, the return value is a pointer to a texture resource. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CTexture* CreateTexture(
                IZ_UINT nWidth,
                IZ_UINT nHeight,
                IZ_UINT nMipLevel,
                E_GRAPH_PIXEL_FMT fmt,
                E_GRAPH_RSC_USAGE nRscType));

        /** Create a cube texture resource.
         *
         * @param [in] nWidth Width of the top-level of the texture, in pixels.
         * @param [in] nHeight Height of the top-level of the texture, in pixels.
         * @param [in] nMipLevel Number of mipmap levels in the texture.
         * @param [in] fmt The format of all levels in the texture.
         * @param [in] bIsDynamic 
         * @return If the method succeeds, the return value is a pointer to a cube texture resource. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CCubeTexture* CreateCubeTexture(
                IZ_UINT nWidth,
                IZ_UINT nHeight,
                IZ_UINT nMipLevel,
                E_GRAPH_PIXEL_FMT fmt,
                IZ_BOOL bIsDynamic));

        /** Create a render target resource.
         *
         * @param [in] nWidth Width of the top-level of the texture, in pixels.
         * @param [in] nHeight Height of the top-level of the texture, in pixels.
         * @param [in] fmt The format of all levels in the texture.
         * @return If the method succeeds, the return value is a pointer to a render target resource. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CRenderTarget* CreateRenderTarget(
                IZ_UINT nWidth, IZ_UINT nHeight,
                E_GRAPH_PIXEL_FMT fmt));
                
        /** Create a vertex buffer.
         *
         * @param [in] stride Size of each vertex.
         * @param [in] vtxNum Number of vertices.
         * @param [in] usage 
         * @return If the method succeeds, the return value is a pointer to a vertex buffer. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CVertexBuffer* CreateVertexBuffer(
                IZ_UINT stride,
                IZ_UINT vtxNum,
                E_GRAPH_RSC_USAGE usage));

        /** Create a index buffer.
         *
         * @param [in] nIdxNum Number of indices.
         * @param [in] fmt The format of the index buffe.
         * @param [in] usage 
         * @return If the method succeeds, the return value is a pointer to a index buffer. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CIndexBuffer* CreateIndexBuffer(
                IZ_UINT nIdxNum,
                E_GRAPH_INDEX_BUFFER_FMT fmt,
                E_GRAPH_RSC_USAGE usage));

        /** Create a shader program object.
         *
         * @return [in] If the method succeeds, the return value is a pointer to a shader program object. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(CShaderProgram* CreateShaderProgram());

        /** Create a vertex shader.
         *
         * @param [in] pProgram Pointer to a shader source (binary or string, it is specified by graphics platform).
         * @return [in] If the method succeeds, the return value is a pointer to a vertex shader. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(CVertexShader* CreateVertexShader(const void* pProgram));

        /** Create a pixel shader.
         *
         * @param [in] pProgram Pointer to a shader source (binary or string, it is specified by graphics platform).
         * @return [in] If the method succeeds, the return value is a pointer to a pixel shader. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(CPixelShader* CreatePixelShader(const void* pProgram));

        /** Create a vertex shader declaration.
         *
         * @param [in] pElem An array of SVertexElement vertex element.
         * @param [in] nNum Number of SVertexElement vertex elements in an array.
         * @return If the method succeeds, the return value is a pointer to a vertex shader declaration. If the method fails, the return value is IZ_NULL.
         */
        PURE_VIRTUAL(
            CVertexDeclaration* CreateVertexDeclaration(
                const SVertexElement* pElem, 
                IZ_UINT nNum));

    public:
        /** Reset status of a graphics device.
         *
         * @param [in] initialParam Pointer to parameters to initialize a graphics device. This parameters are changed by graphics platform.
         * @return If the method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(IZ_BOOL Reset(const void* initialParam));

        /** Terminate a graphics device.
         *
         * If you want to finish an application, you must call this method.
         */
        PURE_VIRTUAL(void Terminate());

        /** Begin frame rendering.
         *
         * You have to call this method before rendering frame.
         *
         * @param [in] nClearFlags Combination of one or more E_GRAPH_CLEAR_FLAG flags that specify the surface(s) that will be cleared.
         * @param [in] nClearColor ARGB color to clear a render target.
         * @param [in] fClearZ New z value which ranges from 0 to 1 to clear the depth buffer.
         * @param [in] nClearStencil New value which ranges from 0 to 2^(n-1) (n is the bit depth of the stencil buffer) to clear the stencil buffer.
         * @return
         */
        PURE_VIRTUAL(
            IZ_BOOL BeginRender(
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor,
                IZ_FLOAT fClearZ,
                IZ_DWORD nClearStencil));

        /** End frame rendering.
         *
         * You have to call this method after rendering frame, before calling CGraphicsDevice::Present.
         */
        PURE_VIRTUAL(void EndRender());

        /** Clears one or more surfaces such as a render target, multiple render targets, a stencil buffer, and a depth buffer.
         *
         * @attention This method clears the entire surface. You can not specify the rectangle to clear currently.
         *
         * @param [in] nClearFlags Combination of one or more E_GRAPH_CLEAR_FLAG flags that specify the surface(s) that will be cleared.
         * @param [in] nClearColor ARGB color to clear a render target.
         * @param [in] fClearZ New z value which ranges from 0 to 1 to clear the depth buffer.
         * @param [in] nClearStencil New value which ranges from 0 to 2^(n-1) (n is the bit depth of the stencil buffer) to clear the stencil buffer.
         */
        PURE_VIRTUAL(
            void Clear(
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor,
                IZ_FLOAT fClearZ,
                IZ_DWORD nClearStencil));

        /** Begin a scene and clear one or more surfaces.
         *
         * @param [in] nClearFlags Combination of one or more E_GRAPH_CLEAR_FLAG flags that specify the surface(s) that will be cleared.
         * @param [in] nClearColor ARGB color to clear a render target.
         * @param [in] fClearZ New z value which ranges from 0 to 1 to clear the depth buffer.
         * @param [in] nClearStencil New value which ranges from 0 to 2^(n-1) (n is the bit depth of the stencil buffer) to clear the stencil buffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL BeginScene(
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0);

        /** Begin a scene with specifed render targets for color surface and clear one or more surfaces.
         *
         * You can specify the render target to render a scene.
         *
         * @param [in] pRT An array of the render target.
         * @param [in] nCount Number of the render target in an array.
         * @param [in] nClearFlags Combination of one or more E_GRAPH_CLEAR_FLAG flags that specify the surface(s) that will be cleared.
         * @param [in] nClearColor ARGB color to clear a render target.
         * @param [in] fClearZ New z value which ranges from 0 to 1 to clear the depth buffer.
         * @param [in] nClearStencil New value which ranges from 0 to 2^(n-1) (n is the bit depth of the stencil buffer) to clear the stencil buffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL BeginScene(
            CRenderTarget** pRT,
            IZ_UINT nCount,
            IZ_DWORD nClearFlags,
            IZ_COLOR nClearColor = 0,
            IZ_FLOAT fClearZ = 1.0f,
            IZ_DWORD nClearStencil = 0);

        /** Begin a scene with specifed render targets for color surface and specified render target for depth surface. Clear one or more surfaces.
         *
         * You can specify the render target to render a scene.
         * And, you can specify the render target for depth.
         *
         * @param [in] pRT An array of the render target.
         * @param [in] nCount Number of the render target in an array.
         * @param [in] pDepth A pointer to the depth stencil surface
         * @param [in] nClearFlags Combination of one or more E_GRAPH_CLEAR_FLAG flags that specify the surface(s) that will be cleared.
         * @param [in] nClearColor ARGB color to clear a render target.
         * @param [in] fClearZ New z value which ranges from 0 to 1 to clear the depth buffer.
         * @param [in] nClearStencil New value which ranges from 0 to 2^(n-1) (n is the bit depth of the stencil buffer) to clear the stencil buffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL BeginScene(
                CRenderTarget** pRT,
                IZ_UINT nCount,
                CRenderTarget* pDepth,
                IZ_DWORD nClearFlags,
                IZ_COLOR nClearColor = 0,
                IZ_FLOAT fClearZ = 1.0f,
                IZ_DWORD nClearStencil = 0));

        /** End a scene that was begun by calling CGraphicsDevice::BeginScene.
         *
         * @param [in] flag Combination of one or more E_GRAPH_END_SCENE_FLAG flags that specify the surface(s) that will be removed.
         */
        PURE_VIRTUAL(void EndScene(IZ_UINT flag = 0xffffffff));

        /** Presents the contents of the next buffer in the sequence of back buffers owned by the device.
         */
        PURE_VIRTUAL(IZ_BOOL Present());

    public:
        /** Begin to render 2D graphics.
         *
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL Begin2D();

        /** End 2D rendering that was begun by calling CGraphicsDevice::Begin2D.
         */
        IZ_BOOL End2D();

        /** Flush 2D rendering commands.
         *
         * Usually you don't need to call this method.
         * If you want to render 2D graphics immediately, you should call this method.
         */
        IZ_BOOL Flush2D();

        /** Render a 2D sprite (rectangle with texture).
         *
         * @attention You can call this method between CGraphicsDevice::Begin2D and CGraphicsDevice::End2D.
         *
         * @param [in] rcSrc Texture coordinate which ranges from 0 to 1 to render the sprite.
         * @param [in] rcDst Rectangle in the screen.
         * @param [in] color Vertex color of the sprite.
         */
        IZ_BOOL Draw2DSprite(
            const CFloatRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

        /** Render a 2D sprite (rectangle with texture).
         *
         * @attention You can call this method between CGraphicsDevice::Begin2D and CGraphicsDevice::End2D.
         *
         * @param [in] rcSrc Rectangle in the texture to render the sprite.
         * @param [in] rcDst Rectangle in the screen.
         * @param [in] color Color of the sprite.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL Draw2DSpriteEx(
            const CIntRect& rcSrc,
            const CIntRect& rcDst,
            const IZ_COLOR color = IZ_COLOR_RGBA(255, 255, 255, 255));

        /** Render a 2D rectangle (without texture).
         *
         * @attention You can call this method between CGraphicsDevice::Begin2D and CGraphicsDevice::End2D.
         *
         * @param [in] rcDst Rectangle in the screen.
         * @param [in] color Color of the sprite.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL Draw2DRect(
            const CIntRect& rcDst,
            const IZ_COLOR color);

        /** Render a line.
         *
         * @attention You can call this method between CGraphicsDevice::Begin2D and CGraphicsDevice::End2D.
         *
         * @param [in] ptStart Start point of the line.
         * @param [in] ptEnd End point of the line.
         * @param [in] color Color of the line.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL Draw2DLine(
            const CIntPoint& ptStart,
            const CIntPoint& ptGoal,
            const IZ_COLOR color);

        /** Set how to render 2D graphics.
         *
         * @param [in] nOp See E_GRAPH_2D_RENDER_OP.
         */
        void Set2DRenderOp(E_GRAPH_2D_RENDER_OP nOp);

        /**
         */
        E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

        /** ユーザー定義のシェーダをセット
         */
        void SetUserDefs2DShader(
            CVertexShader* vs,
            CPixelShader* ps);

        /** 描画モードがユーザー定義の時のみ2D用のシェーダープログラムを取得
         */
        CShaderProgram* Get2DShaderProgramIfRenderOpIsUserDefs();

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

       virtual IZ_BOOL SetVertexBufferInstanced(
            IZ_UINT streamIdx,
            E_GRAPH_VB_USAGE usage,
            IZ_UINT divisor);

        // インデックスバッファセット
        PURE_VIRTUAL(IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB));

        // 頂点宣言セット
        PURE_VIRTUAL(IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD));

        // シェーダプログラムセット
        PURE_VIRTUAL(IZ_BOOL SetShaderProgram(CShaderProgram* program));

        // シェーダプログラム取得
        CShaderProgram* GetShaderProgram();

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

        /** Save screen shot.
         */
        virtual IZ_BOOL SaveScreenShot(
            IZ_PCSTR fileName,
            E_GRAPH_IMAGE_FILE_FMT fmt)
        {
            return IZ_TRUE;
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
            IZ_UINT is_terminated       : 1;
            IZ_UINT reserved            : 28;
        } m_Flags;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_H__)
