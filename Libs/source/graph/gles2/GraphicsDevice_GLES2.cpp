#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/2DRenderer_GLES2.h"
#include "graph/gles2/FrameBufferObject.h"
#include "graph/gles2/RenderTarget_GLES2.h"

namespace izanagi
{
namespace graph
{
    CGraphicsDevice* CGraphicsDevice::s_Instance = IZ_NULL;

    /**
    * �C���X�^���X�쐬
    */
    CGraphicsDevice* CGraphicsDevice::CreateGraphicsDevice(IMemoryAllocator* allocator)
    {
        if (s_Instance != IZ_NULL) {
            // �쐬�ς݂Ȃ̂ŉ������Ȃ�
            return s_Instance;
        }

        IZ_ASSERT(allocator != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CGraphicsDeviceGLES2* instance = IZ_NULL;

        // �������m��
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CGraphicsDeviceGLES2));
        VGOTO(buf != IZ_NULL, __EXIT__);

        // �C���X�^���X�쐬
        instance = new(buf) CGraphicsDeviceGLES2;
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

    // �R���X�g���N�^
    CGraphicsDeviceGLES2::CGraphicsDeviceGLES2()
    {
        m_Display = EGL_NO_DISPLAY;
        m_Surface = EGL_NO_SURFACE;
        m_Context = EGL_NO_CONTEXT;

        m_ScreenWidth = 0;
        m_ScreenHeight = 0;

        m_IsDirtyShaderProgram = IZ_FALSE;
        
        for (IZ_UINT i = 0; i < COUNTOF(m_IsDirtyTex); i++) {
            m_IsDirtyTex[i] = 0;
        }
        
        ::memset(m_SamplerHandle, 0xff, sizeof(m_SamplerHandle));

        FILL_ZERO(m_TexSamplerParamInitialized, sizeof(m_TexSamplerParamInitialized));

        m_FBO = IZ_NULL;
    }

    // �f�X�g���N�^
    CGraphicsDeviceGLES2::~CGraphicsDeviceGLES2()
    {
        // TODO

        ClearRenderState();

        SAFE_RELEASE(m_FBO);
    }

    void CGraphicsDeviceGLES2::ClearRenderState()
    {
        SetShaderProgram(IZ_NULL);
        SetVertexBuffer(0, 0, 0, IZ_NULL);
        SetIndexBuffer(IZ_NULL);

        SAFE_RELEASE(m_RenderState.curVD);

        for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
            // �`�F�b�N
            IZ_ASSERT(m_RTMgr[i].IsEmpty());

            SAFE_RELEASE(m_RenderState.curRT[i]);
        }

        // �`�F�b�N
        IZ_ASSERT(m_DepthMgr.IsEmpty());

        SAFE_RELEASE(m_RenderState.curDepth);

        for (IZ_UINT i = 0; i < TEX_STAGE_NUM; ++i) {
            SetTexture(i, IZ_NULL);
        }
    }

    /**
    * ���Z�b�g
    */
    IZ_BOOL CGraphicsDeviceGLES2::Reset(const void* initialParam)
    {
        const SGraphicsDeviceInitParams& param = *(reinterpret_cast<const SGraphicsDeviceInitParams*>(initialParam));

        IZ_BOOL ret = IZ_FALSE;

        if (m_Display == EGL_NO_DISPLAY) {
            // �{�̍쐬
            ret = CreateBody(param);

            if (ret) {
                // 2D�`�揉����
                m_2DRenderer = C2DRendererGLES2::Create2DRenderer(this, m_Allocator);
                VRETURN(m_2DRenderer != IZ_NULL);

                // FBO
                m_FBO = CFrameBufferObject::CreateFBO(this, m_Allocator);
                VRETURN(m_FBO != IZ_NULL);
            }
        }

        if (ret) {
            // NOTE
            // izanagi�ł͍�����W�n�Ȃ̂�
            // �J�����O�̕W����CounterClockWise�ɂ���
            CALL_GLES2_API(::glFrontFace(GL_CCW));

            // �r���[�|�[�g
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

            // �����_�[�X�e�[�g�̌��ݒl���擾
            m_RenderState.GetParamsFromGraphicsDevice(this);

            // �X�e�[�g
            // �����ݒ�
            m_Flags.is_force_set_state = IZ_TRUE;
            SetDefaultRenderState();
            m_Flags.is_force_set_state = IZ_FALSE;

            // DummyRenderTarget
            {
                m_RT = CRenderTargetGLES2::CreateDummyRenderTarget(
                    m_Allocator,
                    param.screenWidth, param.screenHeight);
                VRETURN(m_RT != IZ_NULL);
                
                // ���݂̃����_�[�^�[�Q�b�g�Ƃ��ĕێ�
                SAFE_REPLACE(m_RenderState.curRT[0], m_RT);
            }
        }

        return ret;
    }

    // �{�̍쐬
    IZ_BOOL CGraphicsDeviceGLES2::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        // Get Display
        m_Display = ::eglGetDisplay(sParams.display);
        VRETURN(m_Display != EGL_NO_DISPLAY);

        // Initialize EGL
        EGLint major, minor;
        EGLBoolean result = ::eglInitialize(m_Display, &major, &minor);
        VRETURN(result);

        // Get configs
        EGLint numConfigs;
        result = ::eglGetConfigs(m_Display, IZ_NULL, 0, &numConfigs);
        VRETURN(result);

        EGLint configAttribList[] =
        {
            EGL_RED_SIZE,       sParams.rgba[0],
            EGL_GREEN_SIZE,     sParams.rgba[1],
            EGL_BLUE_SIZE,      sParams.rgba[2],
            EGL_ALPHA_SIZE,     (sParams.rgba[3] > 0 ? sParams.rgba[3] : EGL_DONT_CARE),
            EGL_DEPTH_SIZE,     (sParams.depth > 0 ? sParams.depth : EGL_DONT_CARE),
            EGL_STENCIL_SIZE,   (sParams.stencil > 0 ? sParams.stencil : EGL_DONT_CARE),
            EGL_SAMPLE_BUFFERS, (sParams.enableMultiSample ? 1 : 0),
            EGL_NONE,
        };

        // Choose config
        EGLConfig config;
        result = ::eglChooseConfig(
                m_Display,
                configAttribList,
                &config,
                1,
                &numConfigs);
        VRETURN(result);

        static const EGLint surfaceAttribList[] =
        {
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_NONE,
        };

        // Create a surface
        m_Surface = ::eglCreateWindowSurface(
            m_Display,
            config,
            sParams.window,
            surfaceAttribList);
        VRETURN(m_Surface != EGL_NO_SURFACE);

        static const EGLint contextAtrribs[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE, EGL_NONE,
        };

        // Create a GL context
        m_Context = ::eglCreateContext(
            m_Display,
            config,
            EGL_NO_CONTEXT,
            contextAtrribs);
        VRETURN(m_Context != EGL_NO_CONTEXT);

        // Make the context current
        result = ::eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);
        VRETURN(result);

        m_ScreenWidth = sParams.screenWidth;
        m_ScreenHeight = sParams.screenHeight;

        return IZ_TRUE;
    }

    /**
    * �`��J�n
    */
    IZ_BOOL CGraphicsDeviceGLES2::BeginRender(
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor,
        IZ_FLOAT fClearZ,
        IZ_DWORD nClearStencil)
    {
        m_Flags.is_call_begin = IZ_TRUE;

        Clear(
            nClearFlags,
            nClearColor,
            fClearZ,
            nClearStencil);

        // 2D�����J�n
        m_2DRenderer->BeginFrame();

        return IZ_TRUE;
    }

    /**
    * �`��I��
    */
    void CGraphicsDeviceGLES2::EndRender()
    {
        // �O�̂���
        EndScene();

        m_Flags.is_call_begin = IZ_FALSE;

        // �N���A���Ă݂�
        ClearRenderState();
    }

    #ifndef _IS_CLEAR
    #define _IS_CLEAR(flag, mask)   (((flag) & (mask)) > 0)
    #else
    IZ_C_ASSERT(IZ_FALSE);
    #endif

    static const IZ_FLOAT clr_div = 1.0f / 255.0f;

    /**
    * �N���A
    */
    void CGraphicsDeviceGLES2::Clear(
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor,
        IZ_FLOAT fClearZ,
        IZ_DWORD nClearStencil)
    {
        if (nClearFlags > 0) {
            GLbitfield flag = 0;

            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_COLOR))
            {
                CALL_GLES2_API(
                    ::glClearColor(
                        IZ_COLOR_R(nClearColor) * clr_div,
                        IZ_COLOR_G(nClearColor) * clr_div,
                        IZ_COLOR_B(nClearColor) * clr_div,
                        IZ_COLOR_A(nClearColor) * clr_div));

                flag |= GL_COLOR_BUFFER_BIT;
            }

            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_DEPTH))
            {
                CALL_GLES2_API(::glClearDepthf(fClearZ));

                flag |= GL_DEPTH_BUFFER_BIT;
            }

            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_STENCIL))
            {
                CALL_GLES2_API(::glClearStencil(nClearStencil));

                flag |= GL_STENCIL_BUFFER_BIT;
            }

            if (flag > 0) {
                CALL_GLES2_API(::glClear(flag));
            }
        }
    }

    IZ_BOOL CGraphicsDeviceGLES2::BeginScene(
        CRenderTarget** pRT,
        IZ_UINT nCount,
        CRenderTarget* pDepth,
        IZ_DWORD nClearFlags,
        IZ_COLOR nClearColor/*= 0*/,
        IZ_FLOAT fClearZ/*= 1.0f*/,
        IZ_DWORD nClearStencil/*= 0*/)
    {
        // TODO
        // MRT�͖����ŁE�E�E
        IZ_ASSERT(nCount <= 1);

        IZ_BOOL ret = IZ_TRUE;

        if ((nCount > 0) && (pRT != IZ_NULL)) {
            // �����_�[�^�[�Q�b�g�Z�b�g
            PushRenderTarget(pRT, nCount);
        }
        if (pDepth != IZ_NULL) {
            // �[�x�E�X�e���V���Z�b�g
            PushDepthStencil(pDepth);
        }

        // �I�t�X�N���[���J�n�i�ݒ肳��Ă���΁j
        IZ_ASSERT(m_FBO != IZ_NULL);
        m_FBO->StartOffScreen();

        // �N���A
        Clear(
            nClearFlags,
            nClearColor,
            fClearZ,
            nClearStencil);

        return ret;
    }

    /**
    * �V�[���`��I��
    */
    void CGraphicsDeviceGLES2::EndScene(IZ_UINT flag/* = 0xffffffff*/)
    {
        // �I�t�X�N���[���I��
        IZ_ASSERT(m_FBO != IZ_NULL);
        m_FBO->EndOffScreen();

        CRenderTarget* pRTList[MAX_MRT_NUM];

        IZ_UINT nRTNum = 0;

        // �����_�[�^�[�Q�b�g
        // �����_�[�^�[�Q�b�g
        for (IZ_UINT i = 0; i < MAX_MRT_NUM; ++i) {
            CRenderTarget* rt = m_RTMgr[i].GetCurrent();

            if ((flag & (1 << i)) > 0) {
                CRenderTarget* tmp = m_RTMgr[i].Pop();
                if (tmp != IZ_NULL) {
                    rt = tmp;
                }
            }

            pRTList[i] = rt;
            nRTNum = (rt != IZ_NULL ? nRTNum + 1 : nRTNum);
        }

        if (pRTList[0] == IZ_NULL) {
            pRTList[0] = m_RT;
            nRTNum += 1;
        }

        if (nRTNum > 0) {
            // TODO
            // MRT�͂P���܂łɐ�������Ă���
            SetRenderTarget(pRTList, 1);
        }

        if ((flag & E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL) > 0) {
            // �[�x�E�X�e���V��
            CRenderTarget* pDepth = m_DepthMgr.Pop();
            if (pDepth != IZ_NULL) {
                SetDepthStencil(pDepth);
            }
        }
    }

    /**
    * ����
    */
    IZ_BOOL CGraphicsDeviceGLES2::Present()
    {
        ::eglSwapBuffers(m_Display, m_Surface);
        return IZ_TRUE;
    }

    void CGraphicsDeviceGLES2::SetDepthStencil(CRenderTarget* pSurface)
    {
        IZ_ASSERT(pSurface != IZ_NULL);
        IZ_ASSERT(m_FBO != IZ_NULL);

        if (m_RenderState.curDepth != pSurface) {
            // �����_�[�^�[�Q�b�g�����ւ���
            m_FBO->SetRenderTarget(pSurface, IZ_TRUE);
        }
    }

    /**
    * ���_�o�b�t�@�Z�b�g
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetVertexBuffer(
        IZ_UINT nStreamIdx,
        IZ_UINT nOffsetByte,
        IZ_UINT nStride,
        CVertexBuffer* pVB)
    {
        if (m_RenderState.curVB == pVB) {
            // ���łɐݒ肳��Ă���
            return IZ_TRUE;
        }

        CVertexBufferGLES2* gles2VB = reinterpret_cast<CVertexBufferGLES2*>(pVB);

        if (gles2VB != IZ_NULL) {
            CALL_GLES2_API(::glBindBuffer(GL_ARRAY_BUFFER, gles2VB->GetRawInterface()));
            gles2VB->Initialize();
        }
        else {
            CALL_GLES2_API(::glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curVB, pVB);

        return IZ_TRUE;
    }

    /**
    * �C���f�b�N�X�o�b�t�@�Z�b�g
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetIndexBuffer(CIndexBuffer* pIB)
    {
        if (m_RenderState.curIB == pIB) {
            // ���łɐݒ肳��Ă���
            return IZ_TRUE;
        }

        CIndexBufferGLES2* gles2IB = reinterpret_cast<CIndexBufferGLES2*>(pIB);

        if (gles2IB != IZ_NULL) {
            CALL_GLES2_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles2IB->GetRawInterface()));
            gles2IB->Initialize();
        }
        else {
            CALL_GLES2_API(::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curIB, pIB);

        return IZ_TRUE;
    }

    // �V�F�[�_�v���O�����Z�b�g
    IZ_BOOL CGraphicsDeviceGLES2::SetShaderProgram(CShaderProgram* program)
    {
        if (m_RenderState.curShader == program) {
            return IZ_TRUE;
        }

        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(program);

        if (gles2Program != IZ_NULL) {
            IZ_ASSERT(gles2Program->IsValid());
            CALL_GLES2_API(::glUseProgram(gles2Program->GetRawInterface()));
        }
        else {
            CALL_GLES2_API(::glUseProgram(0));
        }

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curShader, gles2Program);

        m_IsDirtyShaderProgram = IZ_TRUE;

        return IZ_TRUE;
    }

    /**
    * ���_�錾�Z�b�g
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetVertexDeclaration(CVertexDeclaration* pVD)
    {
        if (m_RenderState.curVD == pVD) {
            // ���łɐݒ肳��Ă���
            return IZ_TRUE;
        }

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curVD, pVD);

        return IZ_TRUE;
    }

    /**
    * �C���f�b�N�X�o�b�t�@�`��
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawIndexedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT vtxOffset,
        IZ_UINT nVtxNum,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationGLES2* vd = reinterpret_cast<CVertexDeclarationGLES2*>(m_RenderState.curVD);

        // NOTE
        // ShaderProgram���Z�b�g����Ȃ��ƃV�F�[�_���j�t�H�[���̎擾�A�ݒ肪�ł��Ȃ��̂�
        // ���_�錾�̔��f�A�e�N�X�`���̃Z�b�g�Ȃǂ����̃^�C�~���O�ł��

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        vd->Apply(
            gles2Program,
            vtxOffset,
            m_RenderState.curVB->GetStride());
        

        // NOTE
        // ShaderCompiler�ɂ����sampler���W�X�^�ɉ��������j�t�H�[�������ݒ肳��Ă���
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
                    IZ_ASSERT(m_SamplerHandle[i] >= 0);
                }

                CALL_GLES2_API(::glActiveTexture(GL_TEXTURE0 + i));

                GLenum type = (m_Texture[i]->GetTexType() == E_GRAPH_TEX_TYPE_PLANE
                    ? GL_TEXTURE_2D
                    : GL_TEXTURE_CUBE_MAP);

                GLuint handle = m_Texture[i]->GetTexHandle();

                CALL_GLES2_API(::glBindTexture(type, handle));

                gles2Program->SetInt(this, m_SamplerHandle[i], i);
            }

            m_IsDirtyTex[i] = IZ_FALSE;
        }

        IZ_UINT idxNum = 0;

        // �v���~�e�B�u�^�C�v����C���f�b�N�X�����v�Z
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
        // glDrawElements �ɓn���I�t�Z�b�g�̓o�C�g��
        // �������A�����ɂ킽���Ă���I�t�Z�b�g�̓C���f�b�N�X�ʒu�̃I�t�Z�b�g
        // ���̂��߁A�o�C�g�I�t�Z�b�g�ɕϊ�����
        IZ_UINT offset = idxOffset * sizeof(IZ_USHORT);
#endif

        CALL_GLES2_API(
            ::glDrawElements(
                mode,
                idxNum,
                type,
                (const GLvoid*)offset));

        m_IsDirtyShaderProgram = IZ_FALSE;

        return IZ_TRUE;
    }

    /**
    * �C���f�b�N�X�o�b�t�@�Ȃ��`��
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT idxOffset,
        IZ_UINT nPrimCnt)
    {
        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);
        CVertexDeclarationGLES2* vd = reinterpret_cast<CVertexDeclarationGLES2*>(m_RenderState.curVD);

        // NOTE
        // ShaderProgram���Z�b�g����Ȃ��ƃV�F�[�_���j�t�H�[���̎擾�A�ݒ肪�ł��Ȃ��̂�
        // ���_�錾�̔��f�A�e�N�X�`���̃Z�b�g�Ȃǂ����̃^�C�~���O�ł��

        IZ_ASSERT(m_RenderState.curVD != IZ_NULL);
        IZ_ASSERT(m_RenderState.curVB != IZ_NULL);

        vd->Apply(
            gles2Program,
            0,
            m_RenderState.curVB->GetStride());

        // NOTE
        // ShaderCompiler�ɂ����sampler���W�X�^�ɉ��������j�t�H�[�������ݒ肳��Ă���
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
                }

                if (m_SamplerHandle[i] >= 0) {
                    CALL_GLES2_API(::glActiveTexture(GL_TEXTURE0 + i));

                    GLenum type = (m_Texture[i]->GetTexType() == E_GRAPH_TEX_TYPE_PLANE
                        ? GL_TEXTURE_2D
                        : GL_TEXTURE_CUBE_MAP);

                    GLuint handle = m_Texture[i]->GetTexHandle();

                    CALL_GLES2_API(::glBindTexture(type, handle));

                    gles2Program->SetInt(this, m_SamplerHandle[i], i);
                }
            }

            m_IsDirtyTex[i] = IZ_FALSE;
        }

        IZ_UINT vtxNum = 0;

        // �v���~�e�B�u�^�C�v����C���f�b�N�X�����v�Z
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

        CALL_GLES2_API(::glDrawArrays(mode, idxOffset, vtxNum));

        m_IsDirtyShaderProgram = IZ_FALSE;
        
        return IZ_TRUE;
    }

    /**
    * �r���[�|�[�g�Z�b�g
    */
    IZ_BOOL CGraphicsDeviceGLES2::SetViewport(const SViewport& vp)
    {
        if (m_Flags.is_render_2d) {
            // TODO
            // 2D�`�撆�͕s��
            return IZ_TRUE;
        }

        IZ_BOOL ret = IZ_TRUE;

        if ((m_RenderState.vp.width != vp.width)
            || (m_RenderState.vp.height != vp.height)
            || (m_RenderState.vp.x != vp.x)
            || (m_RenderState.vp.y != vp.y)
            || (m_RenderState.vp.minZ != vp.minZ)
            || (m_RenderState.vp.maxZ != vp.maxZ))
        {
            CALL_GLES2_API(
                ::glViewport(
                    vp.x,
                    vp.y,
                    vp.width,
                    vp.height));

            CALL_GLES2_API(
                ::glDepthRangef(
                    vp.minZ,
                    vp.maxZ));
            
            if (ret) {
                memcpy(&m_RenderState.vp, &vp, sizeof(vp));
            }
        }

        IZ_ASSERT(ret);
        return ret;
    }

    /**
    * �f�t�H���g�̃����_�[�X�e�[�g��ݒ�
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
    }

    /**
    * �����_�[�X�e�[�g�ꊇ�Z�b�g
    */
    void CGraphicsDeviceGLES2::SetRenderState(const S_RENDER_STATE& sRS)
    {
        SetViewport(sRS.vp);

        for (IZ_UINT i = 0; i < E_GRAPH_RS_NUM; ++i) {
            CGraphicsDevice::SetRenderState(
                static_cast<E_GRAPH_RENDER_STATE>(i),
                sRS.dwRS[i]);
        }

        // �V�U�[��`
        SetScissorTestRect(sRS.rcScissor);
    }

    IZ_BOOL CGraphicsDeviceGLES2::SetTextureInternal(IZ_UINT nStage, CBaseTexture* pTex)
    {
        CALL_GLES2_API(::glActiveTexture(GL_TEXTURE0 + nStage));

        if (pTex != IZ_NULL) {
            IZ_BOOL isPlane = (pTex->GetTexType() == E_GRAPH_TEX_TYPE_PLANE);

            CALL_GLES2_API(
                ::glBindTexture(
                    isPlane ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP,
                    pTex->GetTexHandle()));

            if (pTex->GetTexType() == E_GRAPH_TEX_TYPE_PLANE) {
                ((CTextureGLES2*)pTex)->Initialize();
            }
            else if (pTex->GetTexType() == E_GRAPH_TEX_TYPE_CUBE) {
                ((CCubeTextureGLES2*)pTex)->Initialize();
            }
            else {
                // TODO
                IZ_ASSERT(IZ_FALSE);
            }

            IZ_BOOL orgValue = m_Flags.is_force_set_state;

            if (!m_TexSamplerParamInitialized[nStage])
            {
                // �ŏ������͋����ύX����
                m_Flags.is_force_set_state = IZ_TRUE;
            }

            // TODO
            // �X�e�[�g
            {
                // MIN_FILTER
                SetSamplerStateFilter(
                    isPlane,
                    E_GRAPH_SAMPLER_STATE_TYPE_MINFILTER,
                    m_SamplerState[nStage].minFilter,
                    pTex->GetState().minFilter);

                // MAG_FILTER
                SetSamplerStateFilter(
                    isPlane,
                    E_GRAPH_SAMPLER_STATE_TYPE_MAGFILTER,
                    m_SamplerState[nStage].magFilter,
                    pTex->GetState().magFilter);

                // MIP_FILTER
                if (pTex->GetMipMapNum() > 1) {
                    SetSamplerStateFilter(
                        isPlane,
                        E_GRAPH_SAMPLER_STATE_TYPE_MIPFILTER,
                        m_SamplerState[nStage].mipFilter,
                        pTex->GetState().mipFilter);
                }

                // ADDRESS_U
                SetSamplerStateAddr(
                    isPlane,
                    E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSU,
                    m_SamplerState[nStage].addressU,
                    pTex->GetState().addressU);

                // ADDRESS_V
                SetSamplerStateAddr(
                    isPlane,
                    E_GRAPH_SAMPLER_STATE_TYPE_ADDRESSV,
                    m_SamplerState[nStage].addressV,
                    pTex->GetState().addressV);
            }

            // ���ɖ߂�
            if (!m_TexSamplerParamInitialized[nStage])
            {
                m_Flags.is_force_set_state = orgValue;
                m_TexSamplerParamInitialized[nStage] = IZ_TRUE;
            }
        }
        else {
            CALL_GLES2_API(::glBindTexture(GL_TEXTURE_2D, 0));
            CALL_GLES2_API(::glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
        }

        // �ێ����Ă���
        SAFE_REPLACE(m_Texture[nStage], pTex);

        m_IsDirtyTex[nStage] = IZ_TRUE;

        return IZ_TRUE;
    }

    void CGraphicsDeviceGLES2::SetRenderTargetInternal(CRenderTarget** rt, IZ_UINT num)
    {
        // TODO
        // MRT�͖����ŁE�E�E
        IZ_ASSERT(num == 1);

        IZ_ASSERT(m_FBO != IZ_NULL);

        // �����_�[�^�[�Q�b�g�����ւ���
        for (IZ_UINT i = 0; i < num; ++i) {
            if (m_RenderState.curRT[i] != rt[i]) {
                m_FBO->SetRenderTarget(rt[i], IZ_FALSE);

                SAFE_REPLACE(m_RenderState.curRT[i], rt[i]);
            }
        }
    }
    
    // �T���v���X�e�[�g�ݒ�
    template <typename _SS, typename _T>
    void CGraphicsDeviceGLES2::SetSamplerStateAddr(
        IZ_BOOL isPlane,
        _SS nSSType,
        _T& old_val, _T new_val)
    {
        IZ_BOOL needUpdate = (m_Flags.is_force_set_state || (old_val != new_val));

        if (needUpdate) {
            CALL_GLES2_API(
                ::glTexParameteri(
                    isPlane ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP,
                    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
                    IZ_GET_TARGET_TEX_ADDR(new_val)));

            old_val = new_val;
        }
    }

    // �T���v���X�e�[�g�ݒ�
    template <typename _SS, typename _T>
    void CGraphicsDeviceGLES2::SetSamplerStateFilter(
        IZ_BOOL isPlane,
        _SS nSSType,
        _T& old_val, _T new_val)
    {
        IZ_BOOL needUpdate = (m_Flags.is_force_set_state || (old_val != new_val));

        if (needUpdate) {
            CALL_GLES2_API(
                ::glTexParameteri(
                    isPlane ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP,
                    IZ_GET_TARGET_SAMPLER_STATE_TYPE(nSSType),
                    IZ_GET_TARGET_TEX_FILTER(new_val)));

            old_val = new_val;
        }
    }
}   // namespace graph
}   // namespace izanagi
