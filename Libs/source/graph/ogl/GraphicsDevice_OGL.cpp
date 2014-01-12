#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/FrameBufferObject.h"
#include "graph/gles2/RenderTarget_GLES2.h"
#include "graph/2d/2DRenderer.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceOGL : public CGraphicsDeviceGLES2 {
    public:
        CGraphicsDeviceOGL();
        virtual ~CGraphicsDeviceOGL();

    public:
        virtual IZ_BOOL Reset(const void* initialParam);
        virtual IZ_BOOL Present();

    private:
        IZ_BOOL CreateBody(const SGraphicsDeviceInitParams& sParams);
    };

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
            // 2D描画初期化
            m_2DRenderer = C2DRenderer::Create2DRenderer(this, m_Allocator);
            VRETURN(m_2DRenderer != IZ_NULL);

            // FBO
            m_FBO = CGraphicsDeviceGLES2::CreateFBO(this, m_Allocator);
            VRETURN(m_FBO != IZ_NULL);
        }

        if (ret) {
            // NOTE
            // izanagiでは左手座標系なので
            // カリングの標準はCounterClockWiseにする
            CALL_GLES2_API(::glFrontFace(GL_CCW));

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

    // 本体作成
    IZ_BOOL CGraphicsDeviceOGL::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        ::glewInit();

        m_ScreenWidth = sParams.screenWidth;
        m_ScreenHeight = sParams.screenHeight;

        return IZ_TRUE;
    }

    // 同期
    IZ_BOOL CGraphicsDeviceOGL::Present()
    {
        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
