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

        PURE_VIRTUAL(
            CTexture* CreateTexture(
                IZ_UINT width,
                IZ_UINT height,
                E_GRAPH_PIXEL_FMT fmt,
                void* data));

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
        PURE_VIRTUAL(IZ_BOOL Present(void* nativeParam = nullptr));

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

        /** Get how to render 2D graphics.
         * 
         * @return See E_GRAPH_2D_RENDER_OP.
         */
        E_GRAPH_2D_RENDER_OP Get2DRenderOp() const;

        /** Set an user difinition shader for 2D rendering.
         *
         * If set IZ_NULL to vs or ps, this method does nothing to vertex shader or pixel shader.
         *
         * @param [in] vs User definition vertex shader.
         * @param [in] ps User definition pixel shader.
         */
        void SetUserDefs2DShader(
            CVertexShader* vs,
            CPixelShader* ps);

        /** Get an user definition shader.
         *
         * If user definition shader is set, you can get user definition shader.
         *
         * @return User definition shader program.
         */
        CShaderProgram* Get2DShaderProgramIfRenderOpIsUserDefs();

    public:
        /** Assigns a texture to a stage.
         *
         * @param [in] nStage Zero based sampler number. 
         * @param [in] pTex Pointer to a CBaseTexture.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL SetTexture(IZ_UINT nStage, CBaseTexture* pTex);

        /** Get a texture assigned to a stage.
         *
         * @param [in] nStage Zero based sampler number.
         * @return If there is a texture assigned to the specified stage, the return value is a pointer to a CBaseTexture. If not, the return value is IZ_NULL.
         */
        CBaseTexture* GetTexture(IZ_UINT nStage);

        /** Binds a vertex buffer.
         *
         * @param [in] nStreamIdx Specifies the data stream, in the range from 0 to the maximum number of streams -1.
         * @param [in] nOffsetByte Offset from the beginning of the stream to the beginning of the vertex data, in bytes. 
         * @param [in] stride Stride of the component, in bytes.
         * @param [in] pVB Pointer to a CVertexBuffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL SetVertexBuffer(
                IZ_UINT nStreamIdx,
                IZ_UINT nOffsetByte,
                IZ_UINT stride,
                CVertexBuffer* pVB));


        /** Sets the stream source frequency divider value. This may be used to draw several instances of geometry.
         *
         * @param [in] streamIdx Stream source index.
         * @param [in] offsetByte Offset from the beginning of the stream to the beginning of the vertex data, in bytes. 
         * @param [in] stride Stride of the component, in bytes.
         * @param [in] usage Usage of vertex buffer.
         * @param [in] divisor Specify the number of instances that will pass between updates of the generic attribute at slot streamIdx.
         * @param [in] vb Pointer to a CVertexBuffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        virtual IZ_BOOL SetVertexBufferInstanced(
            IZ_UINT streamIdx,
            IZ_UINT offsetByte,
            IZ_UINT stride,
            E_GRAPH_VB_USAGE usage,
            IZ_UINT divisor,
            CVertexBuffer* vb);

        /** Set a index buffer.
         *
         * @param [in] pIB Pointer to a CIndexBuffer.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(IZ_BOOL SetIndexBuffer(CIndexBuffer* pIB));

        /** Set a vertex declaration.
         *
         * @param [in] pVD Pointer to a CVertexDeclaration.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(IZ_BOOL SetVertexDeclaration(CVertexDeclaration* pVD));

        /** Set a shader program.
         *
         * @param [in] program Pointer to a CShaderProgram.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(IZ_BOOL SetShaderProgram(CShaderProgram* program));

        /** Get a set shader program 
         *
         * @return If there is a shader program set to this device, the return value is a pointer to a CShaderProgram. If not, the return value is IZ_NULL.
         */
        CShaderProgram* GetShaderProgram();

        /** Renders the specified geometric primitive into an array of vertices with indices.
         *
         * @param [in] prim_type Describing the type of primitive to render. See E_GRAPH_PRIM_TYPE.
         * @param [in] vtxOffset Offset from the start of the vertex buffer to the first vertex. 
         * @param [in] vtxNum Number of vertices to render.
         * @param [in] idxOffset Index of the first index to use when accesssing the vertex buffer.
         * @param [in] nPrimCnt Number of primitives to render.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL DrawIndexedPrimitive(
                E_GRAPH_PRIM_TYPE prim_type,
                IZ_UINT vtxOffset,
                IZ_UINT vtxNum,
                IZ_UINT idxOffset,
                IZ_UINT nPrimCnt));

        /** Renders the specified geometric primitive into an array of vertices without indices.
         *
         * @param [in] prim_type Describing the type of primitive to render. See E_GRAPH_PRIM_TYPE.
         * @param [in] idxOffset Offset from the start of the vertex buffer to the first vertex. 
         * @param [in] nPrimCnt Number of primitives to render.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        PURE_VIRTUAL(
            IZ_BOOL DrawPrimitive(
                E_GRAPH_PRIM_TYPE prim_type,
                IZ_UINT idxOffset,
                IZ_UINT nPrimCnt));

        virtual IZ_BOOL DrawIndexedInstancedPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT vtxOffset,
            IZ_UINT vtxNum,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt);

        virtual IZ_BOOL DrawInstancedPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            IZ_UINT idxOffset,
            IZ_UINT nPrimCnt);

        /** Renders the specified geometric primitive with user defined function.
         *
         * @param [in] prim_type Describing the type of primitive to render. See E_GRAPH_PRIM_TYPE.
         * @param [in] funcRenderer User defined function for rendering primitive.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        virtual IZ_BOOL DrawPrimitive(
            E_GRAPH_PRIM_TYPE prim_type,
            std::function<void(IZ_UINT)> funcRenderer)
        {
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

    public:
        /** Sets the viewport parameters.
         *
         * @param [in] vp Reference to SViewport, specifying the viewport parameters to set.
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL SetViewport(const SViewport& vp);

        /** Revert the render state to default values.
         */
        PURE_VIRTUAL(void SetDefaultRenderState());

        /** Sets stencil function.
         */
        void SetStencilFunc(
            E_GRAPH_CMP_FUNC cmp, 
            IZ_INT ref, 
            IZ_DWORD mask);

        /** Sets stencil operations.
         *
         * +--------+------------+-----------+-------------------------------------
         * | フラグ | ステンシル | Zバッファ |
         * +--------+------------+-----------+-------------------------------------
         * | pass	| ○         | ○        | 両方に通った時の振る舞い.
         * | zfail	| ○         | ×        | Zバッファで引っかかった時の振る舞い.
         * | fail	| ×         | ×        | 両方ダメだった時の振る舞い.
         * +--------+------------+-----------+-------------------------------------
         *
         * @param pass 両方に通った時の振る舞い. Stencil operation to perform if both the stencil and the depth (z) tests pass. 
         * @param zfail Zバッファで引っかかった時の振る舞い. Stencil operation to perform if the stencil test passes and the depth test (z-test) fails. 
         * @param fail 両方ダメだった時の振る舞い. Stencil operation to perform if the stencil test fails.
         */
        void SetStencilOp(
            E_GRAPH_STENCIL_OP pass,
            E_GRAPH_STENCIL_OP zfail,
            E_GRAPH_STENCIL_OP fail);

        /** Get the render state parameters.
         *
         * @return Reference to S_RENDER_STATE.
         */
        virtual const S_RENDER_STATE& GetRenderState() const
        {
            return m_RenderState;
        }

        /** Save the current render state parameters.
         *
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL SaveRenderState()
        {
            return m_RenderState.Save();
        }

        /** Load the saved render state parameters.
         *
         * @return If this method succeeds, the return value is true. If the method fails, the return value is false.
         */
        IZ_BOOL LoadRenderState()
        {
            return m_RenderState.Load(this);
        }

    public:
        /** Get the viewport parameters.
         *
         * @return Reference to the viewport parameters structure.
         */
        const SViewport& GetViewport() const
        {
            return m_RenderState.vp;
        }

        /** Set the rect parameters for scissor test.
         *
         * @param [in] Reference to SIntRect, specifying the rect parameters to set.
         */
        void SetScissorTestRect(const SIntRect& rc)
        {
            m_RenderState.SetScissorRect(this, rc);
        }

        /** Sets a render state parameter.
         *
         * @param [in] nState State variable that is being modified. See E_GRAPH_RENDER_STATE.
         * @param [in] val New value for the render state to be set. 
         */
        void SetRenderState(E_GRAPH_RENDER_STATE nState, IZ_DWORD val)
        {
            m_RenderState.SetRenderState(this, nState, val);
        }

        /** Sets the render state parameters.
         *
         * @param [in] sRS Referecnet to the render state parameters.
         */
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
