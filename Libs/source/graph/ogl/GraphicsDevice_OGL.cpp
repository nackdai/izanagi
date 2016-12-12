#include "graph/ogl/GraphicsDevice_OGL.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/FrameBufferObject.h"
#include "graph/gles2/RenderTarget_GLES2.h"
#include "graph/2d/2DRenderer.h"

#include "graph/ogl/VertexBuffer_OGL.h"
#include "graph/ogl/IndexBuffer_OGL.h"
#include "graph/ogl/VertexDeclaration_OGL.h"
#include "graph/ogl/Texture_OGL.h"

namespace izanagi
{
namespace graph
{
    CGraphicsDevice* CGraphicsDevice::s_Instance = IZ_NULL;

    // インスタンス作成
    CGraphicsDevice* CGraphicsDevice::CreateGraphicsDevice(IMemoryAllocator* allocator)
    {
        if (s_Instance != IZ_NULL) {
            // 作成済みなので何もしない
            return s_Instance;
        }

        IZ_ASSERT(allocator != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CGraphicsDeviceOGL* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CGraphicsDeviceOGL));
        VGOTO(buf != IZ_NULL, __EXIT__);

        // インスタンス作成
        instance = new(buf) CGraphicsDeviceOGL;
        {
            instance->m_Allocator = allocator;
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
    CGraphicsDeviceOGL::CGraphicsDeviceOGL()
    {
        FILL_ZERO(&m_dirty, sizeof(m_dirty));
    }

    // デストラクタ
    CGraphicsDeviceOGL::~CGraphicsDeviceOGL()
    {
    }

    // リセット
    IZ_BOOL CGraphicsDeviceOGL::Reset(const void* initialParam)
    {
        const SGraphicsDeviceInitParams& param = *(reinterpret_cast<const SGraphicsDeviceInitParams*>(initialParam));

        // 本体作成
        IZ_BOOL ret = CreateBody(param);

        if (ret) {
            // Depth clip range is [0, 1] instead of [-1, 1].
            CALL_GL_API(::glClipControl(
                GL_LOWER_LEFT,
                GL_ZERO_TO_ONE));

            // 2D描画初期化
            m_2DRenderer = C2DRenderer::Create2DRenderer(this, m_Allocator);
            VRETURN(m_2DRenderer != IZ_NULL);

            // VAOを使わずに毎回更新する.
            auto vd = (CVertexDeclarationOGL*)m_2DRenderer->getVD();
            vd->enableForceUpdate();

            // FBO
            m_FBO = CGraphicsDeviceGLES2::CreateFBO(this, m_Allocator);
            VRETURN(m_FBO != IZ_NULL);
        }

        if (ret) {
            // NOTE
            // izanagiでは左手座標系なので
            // カリングの標準はCounterClockWiseにする
            CALL_GL_API(::glFrontFace(GL_CW));

            // ビューポート
            SViewport vp;
            {
                vp.x = 0;
                vp.y = 0;
                vp.width = param.screenWidth;
                vp.height = param.screenHeight;
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

            // DummyRenderTarget
            {
                m_RT = CGraphicsDeviceGLES2::CreateDummyRenderTarget(
                    m_Allocator,
                    param.screenWidth, param.screenHeight);
                VRETURN(m_RT != IZ_NULL);

                m_Depth = CGraphicsDeviceGLES2::CreateDummyRenderTarget(
                    m_Allocator,
                    param.screenWidth, param.screenHeight);
                VRETURN(m_Depth != IZ_NULL);
                
                // 現在のレンダーターゲットとして保持
                SAFE_REPLACE(m_RenderState.curRT[0], m_RT);
                SAFE_REPLACE(m_RenderState.curDepth, m_Depth);
            }
        }

        return ret;
    }

    IZ_BOOL CGraphicsDeviceOGL::BeginScene(
        CRenderTarget** pRT,
        IZ_UINT nCount,
        CRenderTarget* pDepth,
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor/*= 0*/,
        IZ_FLOAT fClearZ/*= 1.0f*/,
        IZ_DWORD nClearStencil/*= 0*/)
    {
        VRETURN(CheckRenderTargetCount(nCount));

        IZ_BOOL ret = IZ_TRUE;

        if ((nCount > 0) && (pRT != IZ_NULL)) {
            // レンダーターゲットセット
            PushRenderTarget(pRT, nCount);
        }
        if (pDepth != IZ_NULL) {
            // 深度・ステンシルセット
            PushDepthStencil(pDepth);
        }
        else {
            if (!m_rtInternalDepth) {
                m_rtInternalDepth = CreateRenderTarget(
                    GetBackBufferWidth(),
                    GetBackBufferHeight(),
                    E_GRAPH_PIXEL_FMT_D24S8);
                IZ_ASSERT(m_rtInternalDepth);
            }
            PushDepthStencil(m_rtInternalDepth);
            m_isUseInternalDepth = IZ_TRUE;
        }

        // オフスクリーン開始（設定されていれば）
        IZ_ASSERT(m_FBO != IZ_NULL);
        m_FBO->StartOffScreen();

        // クリア
        Clear(
            nClearFlags,
            nClearColor,
            fClearZ,
            nClearStencil);

        return ret;
    }

    void CGraphicsDeviceOGL::EndScene(IZ_UINT flag/* = 0xffffffff*/)
    {
        if (m_isUseInternalDepth) {
            auto fbo = m_FBO->GetRawInterface();

            auto width = GetBackBufferWidth();
            auto height = GetBackBufferHeight();

            CALL_GL_API(::glBlitNamedFramebuffer(
                fbo, 0,
                0, 0,
                width, height,
                0, 0,
                width, height,
                GL_DEPTH_BUFFER_BIT,
                GL_NEAREST));
        }

        // オフスクリーン終了
        IZ_ASSERT(m_FBO != IZ_NULL);
        m_FBO->EndOffScreen();

        CRenderTarget* pRTList[MAX_MRT_NUM];

        IZ_UINT nRTNum = 0;

        // レンダーターゲット
        for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
            CRenderTarget* rt = m_RTMgr[i].GetCurrent();

            if ((flag & (1 << i)) > 0) {
                CRenderTarget* tmp = m_RTMgr[i].Pop();
                if (tmp != IZ_NULL) {
                    rt = tmp;
                }
            }

            pRTList[i] = rt;
            nRTNum++;
        }

        if (pRTList[0] == IZ_NULL) {
            pRTList[0] = m_RT;
            //nRTNum += 1;
        }

        if (nRTNum > 0) {
            CheckRenderTargetCount(nRTNum);
            SetRenderTarget(pRTList, nRTNum);
        }

        if ((flag & E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL) > 0
            || m_isUseInternalDepth)
        {
            // 深度・ステンシル
            CRenderTarget* pDepth = m_DepthMgr.Pop();
            if (pDepth != IZ_NULL) {
                SetDepthStencil(pDepth);
            }

            m_isUseInternalDepth = IZ_FALSE;
        }
    }

    // 本体作成
    IZ_BOOL CGraphicsDeviceGLES2::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        glewExperimental = GL_TRUE;
        GLenum result = glewInit();
        VRETURN(result == GLEW_OK);

        auto version = ::glGetString(GL_VERSION);
        IZ_PRINTF("GL Version(%s)\n", version);

        m_ScreenWidth = sParams.screenWidth;
        m_ScreenHeight = sParams.screenHeight;

        return IZ_TRUE;
    }

    void CGraphicsDeviceOGL::OnTerminate()
    {
        SAFE_RELEASE(m_rtInternalDepth);

        for (IZ_UINT i = 0; i < COUNTOF(m_RenderState.curVB); i++) {
            SAFE_RELEASE(m_RenderState.curVB[i]);
        }
    }

#ifdef __IZ_GLUT__
#include <GL/glut.h>

    // 同期
    IZ_BOOL CGraphicsDeviceGLES2::Present(void* nativeParam/*= nullptr*/)
    {
        glutSwapBuffers();
        return IZ_TRUE;
    }
#else
#include <GLFW/glfw3.h>

    // 同期
    IZ_BOOL CGraphicsDeviceGLES2::Present(void* nativeParam/*= nullptr*/)
    {
        GLFWwindow* window = (GLFWwindow*)nativeParam;
        glfwSwapBuffers(window);
        return IZ_TRUE;
    }
#endif

    // 頂点バッファ作成
    CVertexBuffer* CGraphicsDeviceOGL::CreateVertexBuffer(
        IZ_UINT stride,
        IZ_UINT vtxNum,
        E_GRAPH_RSC_USAGE usage)
    {
        CVertexBufferOGL* pVB = CVertexBufferOGL::CreateVertexBuffer(
            this,
            m_Allocator,
            stride,
            vtxNum,
            usage);

        return pVB;
    }

    // インデックスバッファ作成
    CIndexBuffer* CGraphicsDeviceOGL::CreateIndexBuffer(
        IZ_UINT nIdxNum,
        E_GRAPH_INDEX_BUFFER_FMT fmt,
        E_GRAPH_RSC_USAGE usage)
    {
        CIndexBufferOGL* pIB = CIndexBufferOGL::CreateIndexBuffer(
            this,
            m_Allocator,
            nIdxNum,
            fmt,
            usage);

        return pIB;
    }

    // 頂点宣言作成
    CVertexDeclaration* CGraphicsDeviceOGL::CreateVertexDeclaration(const SVertexElement* pElem, IZ_UINT nNum)
    {
        CVertexDeclaration* ret = CVertexDeclarationOGL::CreateVertexDeclaration(
            m_Allocator,
            pElem,
            nNum);
        return ret;
    }

    // メモリからテクスチャ作成
    CTexture* CGraphicsDeviceOGL::CreateTextureFromMemory(
        void* data,
        IZ_UINT dataSize,
        E_GRAPH_PIXEL_FMT fmt)
    {
        auto* tex = CTextureOGL::CreateTextureFromMemory(
            this,
            m_Allocator,
            data,
            dataSize,
            fmt);

        return tex;
    }

    // テクスチャ作成
    CTexture* CGraphicsDeviceOGL::CreateTexture(
        IZ_UINT width,
        IZ_UINT height,
        IZ_UINT mipLevel,
        E_GRAPH_PIXEL_FMT fmt,
        E_GRAPH_RSC_USAGE rscType)
    {
        auto* pTexture = CTextureOGL::CreateTexture(
            this,
            m_Allocator,
            width, height,
            mipLevel,
            fmt,
            rscType);

        return pTexture;
    }

    // 頂点バッファセット
    IZ_BOOL CGraphicsDeviceOGL::SetVertexBuffer(
        IZ_UINT nStreamIdx,
        IZ_UINT nOffsetByte,
        IZ_UINT nStride,
        CVertexBuffer* pVB)
    {
        if (m_RenderState.curVB[nStreamIdx] == pVB) {
            // すでに設定されている
            return IZ_TRUE;
        }

#if 0
        CVertexBufferOGL* oglVB = reinterpret_cast<CVertexBufferOGL*>(pVB);

        if (oglVB != IZ_NULL) {
            CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, oglVB->GetRawInterface()));
            oglVB->Initialize(this);
        }
        else {
            CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
#else
        if (!pVB) {
            CALL_GL_API(::glBindBuffer(GL_ARRAY_BUFFER, 0));
        }
#endif

        // 現在設定されているものとして保持
        SAFE_REPLACE(m_RenderState.curVB[nStreamIdx], pVB);

        m_dirty.isDirtyVB[nStreamIdx] = IZ_TRUE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::SetIndexBuffer(CIndexBuffer* pIB)
    {
        if (m_RenderState.curIB == pIB) {
            // すでに設定されている
            return IZ_TRUE;
        }

        CIndexBufferGLES2* gles2IB = reinterpret_cast<CIndexBufferGLES2*>(pIB);

        if (!pIB) {
            CALL_GL_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

        // 現在設定されているものとして保持
        SAFE_REPLACE(m_RenderState.curIB, pIB);

        m_dirty.isDirtyIB = IZ_TRUE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::SetVertexBufferInstanced(
        IZ_UINT streamIdx,
        IZ_UINT offsetByte,
        IZ_UINT stride,
        E_GRAPH_VB_USAGE usage,
        IZ_UINT divisor,
        CVertexBuffer* vb)
    {
        if (m_RenderState.curVB[streamIdx] == vb) {
            // すでに設定されている
            return IZ_TRUE;
        }

        IZ_ASSERT(streamIdx < COUNTOF(m_instancingParams));

        if (usage == E_GRAPH_VB_USAGE::E_GRAPH_VB_USAGE_INDEXEDDATA) {
            m_numInstancingPrim = divisor;
            m_instancingParams[streamIdx].divisor = 0;
        }
        else {
            m_instancingParams[streamIdx].divisor = divisor;
        }

        m_instancingParams[streamIdx].offset = offsetByte;
        m_instancingParams[streamIdx].stride = stride;

        // 現在設定されているものとして保持
        SAFE_REPLACE(m_RenderState.curVB[streamIdx], vb);

        return IZ_TRUE;
    }

    void CGraphicsDeviceOGL::setTexToSampler()
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);

        // NOTE
        // ShaderCompilerによってsamplerレジスタに応じたユニフォーム名が設定されている
        static const char* samplerName[TEX_STAGE_NUM] =
        {
            "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
        };

        for (IZ_UINT i = 0; i < TEX_STAGE_NUM; i++) {
            if (m_IsDirtyTex[i]
                && m_Texture[i] != IZ_NULL)
            {
                if (m_IsDirtyShaderProgram) {
                    m_SamplerHandle[i] = gles2Program->GetHandleByName(samplerName[i]);
                    //IZ_ASSERT(m_SamplerHandle[i] >= 0);
                    if (m_SamplerHandle[i] < 0) {
                        m_IsDirtyTex[i] = IZ_FALSE;
                        continue;
                    }
                }

                CALL_GL_API(::glActiveTexture(GL_TEXTURE0 + i));

                GLenum type = (m_Texture[i]->GetTexType() == E_GRAPH_TEX_TYPE_PLANE
                    ? GL_TEXTURE_2D
                    : GL_TEXTURE_CUBE_MAP);

                GLuint handle = m_Texture[i]->GetTexHandle();

                CALL_GL_API(::glBindTexture(type, handle));

                gles2Program->SetInt(this, m_SamplerHandle[i], i);
            }

            m_IsDirtyTex[i] = IZ_FALSE;
        }
    }

    CGraphicsDeviceOGL::IndexexDrawParam CGraphicsDeviceOGL::getIndexParam(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        IZ_UINT idxNum = 0;

        // プリミティブタイプからインデックス数を計算
        switch (prim_type)
        {
        case E_GRAPH_PRIM_TYPE_POINTLIST:
            idxNum = nPrimCnt;
            break;
        case E_GRAPH_PRIM_TYPE_LINELIST:
            idxNum = nPrimCnt * 2;
            break;
        case E_GRAPH_PRIM_TYPE_LINESTRIP:
            idxNum = nPrimCnt + 1;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLELIST:
            idxNum = nPrimCnt * 3;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLESTRIP:
            idxNum = nPrimCnt + 2;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLEFAN:
            idxNum = nPrimCnt + 2;
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        GLenum mode = CParamValueConverterGLES2::ConvAbstractToTarget_PrimType(prim_type);

        IZ_ASSERT(m_RenderState.curIB != IZ_NULL);

#if 1
        E_GRAPH_INDEX_BUFFER_FMT idxFmt = m_RenderState.curIB->GetFormat();
        GLenum type = CTargetParamValueConverter::ConvAbstractToTarget_IdxBufFmt(idxFmt);

        IZ_UINT offset = idxFmt == E_GRAPH_INDEX_BUFFER_FMT_INDEX16
            ? idxOffset * sizeof(IZ_UINT16)
            : idxOffset * sizeof(IZ_UINT32);
#else
        GLenum type = GL_UNSIGNED_SHORT;

        // TODO
        IZ_ASSERT(m_RenderState.curIB->GetIdxNum() <= IZ_UINT16_MAX);

        // NOTE
        // glDrawElements に渡すオフセットはバイト数
        // しかし、ここにわたってくるオフセットはインデックス位置のオフセット
        // そのため、バイトオフセットに変換する
        IZ_UINT offset = idxOffset * sizeof(IZ_USHORT);
#endif

        return IndexexDrawParam(idxNum, mode, type, offset);
    }

    CGraphicsDeviceOGL::DrawParam CGraphicsDeviceOGL::getDrawParam(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT nPrimCnt)
    {
        IZ_UINT vtxNum = 0;

        // プリミティブタイプからインデックス数を計算
        switch (prim_type)
        {
        case E_GRAPH_PRIM_TYPE_POINTLIST:
            vtxNum = nPrimCnt;
            break;
        case E_GRAPH_PRIM_TYPE_LINELIST:
            vtxNum = nPrimCnt * 2;
            break;
        case E_GRAPH_PRIM_TYPE_LINESTRIP:
            vtxNum = nPrimCnt + 1;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLELIST:
            vtxNum = nPrimCnt * 3;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLESTRIP:
            vtxNum = nPrimCnt + 2;
            break;
        case E_GRAPH_PRIM_TYPE_TRIANGLEFAN:
            vtxNum = nPrimCnt + 2;
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        GLenum mode = CParamValueConverterGLES2::ConvAbstractToTarget_PrimType(prim_type);

        return DrawParam(vtxNum, mode);
    }

    void CGraphicsDeviceOGL::setIndexBuffer()
    {
        if (m_dirty.isDirtyIB) {
            CIndexBufferGLES2* ib = reinterpret_cast<CIndexBufferGLES2*>(m_RenderState.curIB);
            if (ib != IZ_NULL) {
                CALL_GL_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->GetRawInterface()));
                ib->Initialize(this);
            }

            m_dirty.isDirtyIB = IZ_FALSE;
        }
    }

    IZ_BOOL CGraphicsDeviceOGL::DrawIndexedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT vtxOffset,
        IZ_UINT nVtxNum,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationOGL* vd = reinterpret_cast<CVertexDeclarationOGL*>(m_RenderState.curVD);
        CVertexBufferOGL* vb = reinterpret_cast<CVertexBufferOGL*>(m_RenderState.curVB[0]);

        // NOTE
        // ShaderProgramがセットされないとシェーダユニフォームの取得、設定ができないので
        // 頂点宣言の反映、テクスチャのセットなどをこのタイミングでやる

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        vd->begin(
            this,
            gles2Program,
            vtxOffset,
            m_dirty.isDirtyVB[0] ? vb : nullptr,
            vb->GetStride());

        m_dirty.isDirtyVB[0] = IZ_FALSE;

        setIndexBuffer();

        setTexToSampler();

        auto idxParam = getIndexParam(prim_type, idxOffset, nPrimCnt);

        IZ_UINT idxNum = std::get<0>(idxParam);
        GLenum mode = std::get<1>(idxParam);
        GLenum type = std::get<2>(idxParam);
        IZ_UINT offset = std::get<3>(idxParam);

        CALL_GL_API(::glDrawElements(
            mode,
            idxNum,
            type,
            (const GLvoid*)offset));

        vd->end();

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::DrawPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationOGL* vd = reinterpret_cast<CVertexDeclarationOGL*>(m_RenderState.curVD);
        CVertexBufferOGL* vb = reinterpret_cast<CVertexBufferOGL*>(m_RenderState.curVB[0]);

        // NOTE
        // ShaderProgramがセットされないとシェーダユニフォームの取得、設定ができないので
        // 頂点宣言の反映、テクスチャのセットなどをこのタイミングでやる

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        vd->begin(
            this,
            gles2Program,
            0,
            m_dirty.isDirtyVB[0] ? vb : nullptr,
            vb->GetStride());

        m_dirty.isDirtyVB[0] = IZ_FALSE;

        setTexToSampler();

        auto drawParam = getDrawParam(prim_type, nPrimCnt);

        IZ_UINT vtxNum = std::get<0>(drawParam);
        GLenum mode = std::get<1>(drawParam);

        CALL_GL_API(::glDrawArrays(mode, idxOffset, vtxNum));

        vd->end();

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::DrawIndexedInstancedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT vtxOffset,
        IZ_UINT vtxNum,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationOGL* vd = reinterpret_cast<CVertexDeclarationOGL*>(m_RenderState.curVD);

        // NOTE
        // ShaderProgramがセットされないとシェーダユニフォームの取得、設定ができないので
        // 頂点宣言の反映、テクスチャのセットなどをこのタイミングでやる

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        m_instancingParams[0].offset += vtxOffset * m_instancingParams[0].stride;

        CVertexBuffer* vbs[MAX_STREAM_NUM] = {
            m_RenderState.curVB[0],
            m_RenderState.curVB[1],
            m_RenderState.curVB[2],
            m_RenderState.curVB[3],
        };

        vd->beginInstancing(
            gles2Program,
            m_instancingParams,
            vbs);

        m_dirty.isDirtyVB[0] = IZ_FALSE;
        m_dirty.isDirtyVB[1] = IZ_FALSE;
        m_dirty.isDirtyVB[2] = IZ_FALSE;
        m_dirty.isDirtyVB[3] = IZ_FALSE;

        setIndexBuffer();

        setTexToSampler();

        auto idxParam = getIndexParam(prim_type, idxOffset, nPrimCnt);

        IZ_UINT idxNum = std::get<0>(idxParam);
        GLenum mode = std::get<1>(idxParam);
        GLenum type = std::get<2>(idxParam);
        IZ_UINT offset = std::get<3>(idxParam);

        IZ_ASSERT(m_numInstancingPrim > 0);

        CALL_GL_API(::glDrawElementsInstanced(
            mode,
            idxNum,
            type,
            (const GLvoid*)offset,
            m_numInstancingPrim));

        vd->end();

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::DrawInstancedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationOGL* vd = reinterpret_cast<CVertexDeclarationOGL*>(m_RenderState.curVD);

        // NOTE
        // ShaderProgramがセットされないとシェーダユニフォームの取得、設定ができないので
        // 頂点宣言の反映、テクスチャのセットなどをこのタイミングでやる

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        CVertexBuffer* vbs[MAX_STREAM_NUM] = {
            m_RenderState.curVB[0],
            m_RenderState.curVB[1],
            m_RenderState.curVB[2],
            m_RenderState.curVB[3],
        };

        vd->beginInstancing(
            gles2Program,
            m_instancingParams,
            vbs);

        m_dirty.isDirtyVB[0] = IZ_FALSE;
        m_dirty.isDirtyVB[1] = IZ_FALSE;
        m_dirty.isDirtyVB[2] = IZ_FALSE;
        m_dirty.isDirtyVB[3] = IZ_FALSE;

        setTexToSampler();

        auto drawParam = getDrawParam(prim_type, nPrimCnt);

        IZ_UINT vtxNum = std::get<0>(drawParam);
        GLenum mode = std::get<1>(drawParam);

        IZ_ASSERT(m_numInstancingPrim > 0);

        CALL_GL_API(::glDrawArraysInstanced(
            mode,
            idxOffset,
            vtxNum,
            m_numInstancingPrim));

        vd->end();

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }

    IZ_BOOL CGraphicsDeviceOGL::DrawPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        std::function<void(IZ_UINT)> funcRenderer)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationOGL* vd = reinterpret_cast<CVertexDeclarationOGL*>(m_RenderState.curVD);
        CVertexBufferOGL* vb = reinterpret_cast<CVertexBufferOGL*>(m_RenderState.curVB[0]);

        // NOTE
        // ShaderProgramがセットされないとシェーダユニフォームの取得、設定ができないので
        // 頂点宣言の反映、テクスチャのセットなどをこのタイミングでやる

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        vd->begin(
            this,
            gles2Program,
            0,
            m_dirty.isDirtyVB[0] ? vb : nullptr,
            vb->GetStride());

        m_dirty.isDirtyVB[0] = IZ_FALSE;

        setTexToSampler();

        auto drawParam = getDrawParam(prim_type, 0);

        IZ_UINT vtxNum = std::get<0>(drawParam);
        GLenum mode = std::get<1>(drawParam);

        funcRenderer(mode);

        vd->end();

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
