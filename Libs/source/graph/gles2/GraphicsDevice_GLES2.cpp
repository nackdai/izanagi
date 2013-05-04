#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/2DRenderer_GLES2.h"

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
        // TODO
    }

    // �f�X�g���N�^
    CGraphicsDeviceGLES2::~CGraphicsDeviceGLES2()
    {
        // TODO

        ClearRenderState();
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
        // NOTE
        // izanagi�ł͍�����W�n�Ȃ̂�
        // �J�����O�̕W����CounterClockWise�ɂ���
        ::glFrontFace(GL_CCW);

        const SGraphicsDeviceInitParams& param = *(reinterpret_cast<const SGraphicsDeviceInitParams*>(initialParam));

        IZ_BOOL ret = IZ_FALSE;

        if (m_Display == EGL_NO_DISPLAY) {
            // �{�̍쐬
            ret = CreateBody(param);

            if (ret) {
                // 2D�`�揉����
                m_2DRenderer = C2DRendererGLES2::Create2DRenderer(this, m_Allocator);
                ret = (m_2DRenderer != IZ_NULL);
                IZ_ASSERT(ret);
            }
        }

        if (ret) {
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

            // TODO
            // ����H
            // �T�[�t�F�X�̃��Z�b�g
            //�i�t���[���o�b�t�@�̃��Z�b�g�j
        }

        return ret;
    }

    // �{�̍쐬
    IZ_BOOL CGraphicsDeviceGLES2::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        // Get Display
        m_Display = ::eglGetDisplay(sParams.display);
        VRETURN(m_Display == EGL_NO_DISPLAY);

        // Initialize EGL
        EGLint major, minor;
        VRETURN(::eglInitialize(m_Display, &major, &minor));

        // Get configs
        EGLint numConfigs;
        VRETURN(::eglGetConfigs(m_Display, IZ_NULL, 0, &numConfigs));

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
        VRETURN(
            ::eglChooseConfig(
                m_Display,
                configAttribList,
                &config,
                1,
                &numConfigs));

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
        VRETURN(::eglMakeCurrent(m_Display, m_Surface, EGL_NO_SURFACE, m_Context));

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
            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_COLOR))
            {
                ::glClearColor(
                    IZ_COLOR_R(nClearColor) * clr_div,
                    IZ_COLOR_G(nClearColor) * clr_div,
                    IZ_COLOR_B(nClearColor) * clr_div,
                    IZ_COLOR_A(nClearColor) * clr_div);

                ::glClear(GL_COLOR_BUFFER_BIT);
            }

            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_DEPTH))
            {
                ::glClearDepthf(fClearZ);

                ::glClear(GL_DEPTH_BUFFER_BIT);
            }

            if (_IS_CLEAR(nClearFlags, E_GRAPH_CLEAR_FLAG_STENCIL))
            {
                ::glClearStencil(nClearStencil);

                ::glClear(GL_STENCIL_BUFFER_BIT);
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
        CRenderTarget* pRTList[MAX_MRT_NUM];
        memset(pRTList, 0, sizeof(pRTList));

        IZ_UINT nRTNum = 0;

        // �����_�[�^�[�Q�b�g
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

        if (m_RenderState.curDepth != pSurface) {
            // �����_�[�^�[�Q�b�g�����ւ���
            // TODO
            IZ_ASSERT(IZ_FALSE);
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
            ::glBindBuffer(GL_ARRAY_BUFFER, gles2VB->GetRawInterface());
        }
        else {
            ::glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curVB, pVB);

        m_IsDirtyVB = IZ_TRUE;

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
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gles2IB->GetRawInterface());
        }
        else {
            ::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

        IZ_ASSERT(gles2Program->IsValid());

        if (gles2Program != IZ_NULL) {
            if (!gles2Program->IsLinked()) {
                gles2Program->Link();
            }

            ::glUseProgram(gles2Program->GetRawInterface());
        }
        else {
            ::glUseProgram(0);
        }

        m_IsDirtyShaderProgram = IZ_TRUE;

        // ���ݐݒ肳��Ă�����̂Ƃ��ĕێ�
        SAFE_REPLACE(m_RenderState.curShader, gles2Program);

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

        m_IsDirtyVD = IZ_TRUE;

        return IZ_TRUE;
    }

    /**
    * �C���f�b�N�X�o�b�t�@�`��
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawIndexedPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT nBaseIdx,
        IZ_UINT nMinIdx,
        IZ_UINT nVtxNum,
        IZ_UINT nStartIdx,
        IZ_UINT nPrimCnt)
    {
        // NOTE
        // ShaderProgram���Z�b�g����Ȃ��ƃV�F�[�_���j�t�H�[���̎擾�A�ݒ肪�ł��Ȃ��̂�
        // ���_�錾�̔��f�A�e�N�X�`���̃Z�b�g�Ȃǂ����̃^�C�~���O�ł��

        IZ_BOOL isDirty = (m_IsDirtyVB
            || m_IsDirtyVD
            || m_IsDirtyShaderProgram);

        if (isDirty) {
            IZ_ASSERT(m_RenderState.curVD != IZ_NULL);

            ((CVertexDeclarationGLES2*)m_RenderState.curVD)->Apply(this);
        }

        m_IsDirtyVB = IZ_FALSE;
        m_IsDirtyVD = IZ_FALSE;
        m_IsDirtyShaderProgram = IZ_FALSE;

        CShaderProgramGLES2* gles2Program = reinterpret_cast<CShaderProgramGLES2*>(m_RenderState.curShader);

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
                if (isDirty) {
                    m_SamplerHandle[i] = ::glGetUniformLocation(gles2Program->GetRawInterface(), samplerName[i]);
                }

                ::glActiveTexture(GL_TEXTURE0 + i);

                GLenum type = (m_Texture[i]->GetTexType() == E_GRAPH_TEX_TYPE_PLANE
                    ? GL_TEXTURE_2D
                    : GL_TEXTURE_CUBE_MAP);

                GLuint handle = m_Texture[i]->GetTexHandle();

                ::glBindTexture(type, handle);

                ::glUniform1i(m_SamplerHandle[i], i);
            }

            m_IsDirtyTex[i] = IZ_FALSE;
        }

        // TODO
        //::glDrawElements(


        return IZ_TRUE;
    }

    /**
    * �C���f�b�N�X�o�b�t�@�Ȃ��`��
    */
    IZ_BOOL CGraphicsDeviceGLES2::DrawPrimitive(
        E_GRAPH_PRIM_TYPE prim_type,
        IZ_UINT nStartIdx,
        IZ_UINT nPrimCnt)
    {
        // TODO
        IZ_ASSERT(IZ_FALSE);

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
            ::glViewport(
                vp.x,
                vp.y,
                vp.width,
                vp.height);

            ::glDepthRangef(
                vp.minZ,
                vp.maxZ);
            
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

        // ���[��E�E�E
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
        HRESULT hr = m_Device->SetTexture(
            nStage,
            pTex != NULL ? pTex->GetTexHandle() : NULL);
        VRETURN(SUCCEEDED(hr));

        // �ێ����Ă���
        SAFE_REPLACE(m_Texture[nStage], pTex);

        // ���[��E�E�E
        // �X�e�[�g
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
        // �����_�[�^�[�Q�b�g�����ւ���
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
