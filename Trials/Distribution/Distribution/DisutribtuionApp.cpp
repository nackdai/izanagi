#include "DistributionApp.h"

// NOTE
// DirectX3D 9 でスクリーンショットを撮る方法
//  - IDirect3DDevice9::GetRenderTargetData メソッドを使う
// http://katze.hatenablog.jp/entry/2013/06/17/184457

static const IZ_FLOAT POS_X = 0.0f;
static const IZ_FLOAT RADIUS = 5.0f;
static const IZ_FLOAT DISTANCE = RADIUS * 2.0f;
static const IZ_UINT ORDER = 20;
static const IZ_UINT THREAD_NUM = 6;

CDistributionApp::CDistributionApp()
{
    m_Mesh = IZ_NULL;

    m_Img = IZ_NULL;

    m_Shader = IZ_NULL;
}

CDistributionApp::~CDistributionApp()
{
}

// 初期化.
IZ_BOOL CDistributionApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
                    | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    m_Mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
        allocator,
        device,
        flag,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
        RADIUS,
        100, 100);
    VGOTO(result = (m_Mesh != IZ_NULL), __EXIT__);

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/earth.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/Shader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    for (IZ_UINT i = 0; i < MAX_MESH_NUM; i++) {
        izanagi::math::SMatrix44::GetTrans(
            m_objects[i].mtxL2W,
            POS_X + DISTANCE * (i % ORDER),
            0.0f,
            -DISTANCE * (i / ORDER));

        m_objects[i].points[0].Set( RADIUS,  RADIUS,  RADIUS);
        m_objects[i].points[1].Set(-RADIUS,  RADIUS,  RADIUS);
        m_objects[i].points[2].Set(-RADIUS,  RADIUS, -RADIUS);
        m_objects[i].points[3].Set( RADIUS,  RADIUS, -RADIUS);

        m_objects[i].points[4].Set( RADIUS, -RADIUS,  RADIUS);
        m_objects[i].points[5].Set(-RADIUS, -RADIUS,  RADIUS);
        m_objects[i].points[6].Set(-RADIUS, -RADIUS, -RADIUS);
        m_objects[i].points[7].Set( RADIUS, -RADIUS, -RADIUS);

        for (IZ_UINT n = 0; n < COUNTOF(m_objects[i].points); n++) {
            izanagi::math::SMatrix44::Apply(
                m_objects[i].points[n],
                m_objects[i].points[n],
                m_objects[i].mtxL2W);
        }

        m_objects[i].point2D[0][0] = SCREEN_WIDTH;
        m_objects[i].point2D[0][1] = SCREEN_HEIGHT;

        m_objects[i].point2D[1][0] = 0.0f;
        m_objects[i].point2D[1][1] = 0.0f;
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    // Compute Clip -> Screen matrix.
    izanagi::CSceneGraphUtil::ComputeC2S(
        m_mtxC2S,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        camera.GetParam().cameraNear,
        camera.GetParam().cameraFar);

    // Allocate threads.
    m_theadPool.Init(allocator, THREAD_NUM);
    m_allocator = allocator;    // TODO

#ifdef __IZ_OGL__
#if 0
    GLuint size = SCREEN_WIDTH * SCREEN_HEIGHT * 4;

    CALL_GL_API(::glGenBuffers(COUNTOF(m_PBO), m_PBO));

    CALL_GL_API(::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[0]));
    CALL_GL_API(::glBufferData(GL_PIXEL_PACK_BUFFER, size, 0, GL_STREAM_READ));

    CALL_GL_API(::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[1]));
    CALL_GL_API(::glBufferData(GL_PIXEL_PACK_BUFFER, size, 0, GL_STREAM_READ));

    CALL_GL_API(::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0));
#else
    initScreenCapture();
#endif
#endif  // #ifdef __IZ_OGL__

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CDistributionApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);

    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_Shader);

    m_theadPool.Terminate();

#if __IZ_OGL__
    CALL_GL_API(::glDeleteBuffers(COUNTOF(m_PBO), m_PBO));
#endif
}

// 更新.
void CDistributionApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();
    
    camera.Update();

    if (m_enabled2DRender) {
        const auto& mtxW2C = camera.GetParam().mtxW2C;

#if 0
        for (IZ_UINT i = 0; i < MAX_MESH_NUM; i++) {
            m_objects[i].point2D[0][0] = SCREEN_WIDTH;
            m_objects[i].point2D[0][1] = SCREEN_HEIGHT;

            m_objects[i].point2D[1][0] = 0.0f;
            m_objects[i].point2D[1][1] = 0.0f;

            izanagi::math::SVector4 vec2D;

            for (IZ_UINT n = 0; n < COUNTOF(m_objects[i].points); n++) {
                // World -> Clip
                izanagi::math::SMatrix44::Apply(
                    vec2D,
                    m_objects[i].points[n],
                    mtxW2C);

                izanagi::math::SVector4::Div(vec2D, vec2D, vec2D.w);

                // Clip -> Screen
                izanagi::math::SMatrix44::Apply(
                    vec2D,
                    vec2D,
                    m_mtxC2S);

                m_objects[i].point2D[0][0] = IZ_MIN(m_objects[i].point2D[0][0], vec2D.x);
                m_objects[i].point2D[0][1] = IZ_MIN(m_objects[i].point2D[0][1], vec2D.y);

                m_objects[i].point2D[1][0] = IZ_MAX(m_objects[i].point2D[1][0], vec2D.x);
                m_objects[i].point2D[1][1] = IZ_MAX(m_objects[i].point2D[1][1], vec2D.y);
            }
        }
#else
        izanagi::math::SMatrix44 mtxW2S;

        izanagi::math::SMatrix44::Mul(
            mtxW2S,
            mtxW2C,
            m_mtxC2S);

        izanagi::threadmodel::CParallel::For(
            m_theadPool,
            m_allocator,
            0, MAX_MESH_NUM,
            [&](IZ_UINT i) {
            m_objects[i].point2D[0][0] = SCREEN_WIDTH;
            m_objects[i].point2D[0][1] = SCREEN_HEIGHT;

            m_objects[i].point2D[1][0] = 0.0f;
            m_objects[i].point2D[1][1] = 0.0f;

            izanagi::math::SVector4 vec2D;

            for (IZ_UINT n = 0; n < COUNTOF(m_objects[i].points); n++) {
#if 0
                // World -> Clip
                izanagi::math::SMatrix44::Apply(
                    vec2D,
                    m_objects[i].points[n],
                    mtxW2C);

                izanagi::math::SVector4::Div(vec2D, vec2D, vec2D.w);

                // Clip -> Screen
                izanagi::math::SMatrix44::Apply(
                    vec2D,
                    vec2D,
                    m_mtxC2S);
#else
                izanagi::math::SMatrix44::Apply(
                    vec2D,
                    m_objects[i].points[n],
                    mtxW2S);

                izanagi::math::SVector4::Div(vec2D, vec2D, vec2D.w);
#endif

                m_objects[i].point2D[0][0] = IZ_MIN(m_objects[i].point2D[0][0], vec2D.x);
                m_objects[i].point2D[0][1] = IZ_MIN(m_objects[i].point2D[0][1], vec2D.y);

                m_objects[i].point2D[1][0] = IZ_MAX(m_objects[i].point2D[1][0], vec2D.x);
                m_objects[i].point2D[1][1] = IZ_MAX(m_objects[i].point2D[1][1], vec2D.y);
            }
        });
#endif
    }
}

namespace {
    inline void _SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes)
    {
        izanagi::shader::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
        IZ_ASSERT(handle != 0);

        shader->SetParamValue(
            handle,
            value,
            bytes);
    }
}

#ifdef __IZ_OGL__
// NOTE
// https://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-AsynchronousBufferTransfers.pdf
// 28.4 Download
// 28.5 Copy

void CDistributionApp::initScreenCapture()
{
    const IZ_UINT screenSize = SCREEN_WIDTH * SCREEN_HEIGHT * 4;

    for (IZ_UINT i = 0; i < COUNTOF(m_SD); i++) {
        auto& sd = m_SD[i];

        glGenBuffers(1, &sd.buffer);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, sd.buffer);
        glBufferData(GL_PIXEL_PACK_BUFFER, screenSize, 0, GL_STREAM_READ);

        glGenBuffers(1, &sd.tmpBuffer);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, sd.tmpBuffer);
        glBufferData(GL_PIXEL_PACK_BUFFER, screenSize, 0, GL_STREAM_COPY);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
}

void CDistributionApp::procScreenCapture()
{
    const IZ_UINT screenSize = SCREEN_WIDTH * SCREEN_HEIGHT * 4;

    while (m_RBTail != m_RBHead) {
        auto tmpTail = (m_RBTail + 1) % COUNTOF(m_SD);

        auto& sd = m_SD[tmpTail];

        GLenum res = glClientWaitSync(sd.fence, 0, 0);
        if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
            glBindBuffer(GL_COPY_READ_BUFFER, sd.buffer);

            IZ_BYTE* src = (IZ_BYTE*)glMapBufferRange(
                GL_COPY_READ_BUFFER,
                0,
                screenSize,
                GL_MAP_READ_BIT);

            // TODO

            glUnmapBuffer(GL_COPY_READ_BUFFER);

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            m_RBTail = tmpTail;
        }
        else {
            break;
        }
    }
}

void CDistributionApp::captureScreen()
{
    const IZ_UINT screenSize = SCREEN_WIDTH * SCREEN_HEIGHT * 4;

    auto tmpHead = (m_RBHead + 1) % COUNTOF(m_SD);

    if (tmpHead == m_RBTail) {
        IZ_PRINTF("Too fast\n");
    }
    else {
        auto& sd = m_SD[tmpHead];

        glReadBuffer(GL_BACK);

        glBindBuffer(
            GL_PIXEL_PACK_BUFFER,
            sd.tmpBuffer);
        glReadPixels(
            0,
            0,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        glBindBuffer(GL_COPY_READ_BUFFER, sd.tmpBuffer);
        glBindBuffer(GL_COPY_WRITE_BUFFER, sd.buffer);
        glCopyBufferSubData(
            GL_COPY_READ_BUFFER,
            GL_COPY_WRITE_BUFFER,
            0,
            0,
            screenSize);

        sd.fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        m_RBHead = tmpHead;
    }
}
#endif

// 描画.
void CDistributionApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
#if 0
    izanagi::sys::CTimer timer;
    timer.Begin();
    {
#ifdef __IZ_OGL__
#if 0
        static int index = 0;

        index = (index + 1) % 2;
        auto nextIndex = (index + 1) % 2;

        ::glReadBuffer(GL_FRONT);

        ::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[index]);
        ::glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        ::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[nextIndex]);

        auto src = (GLubyte*)::glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
        IZ_ASSERT(src);

        ::glUnmapBuffer(GL_PIXEL_PACK_BUFFER);

        ::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        ::glDrawBuffer(GL_BACK);
#else
        static IZ_UINT64 frame_number = 0;
        const IZ_UINT64 buffer_number = frame_number++ % COUNTOF(m_fence);

        static int index = 0;

        index = (index + 1) % 2;
        auto nextIndex = (index + 1) % 2;

        m_fence[buffer_number] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        CALL_GL_API(auto res = ::glClientWaitSync(m_fence[buffer_number], 0, 0));
        if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
            CALL_GL_API(::glDeleteSync(m_fence[buffer_number]));

            CALL_GL_API(::glReadBuffer(GL_FRONT));

            CALL_GL_API(::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[index]));

            ::glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            ::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_PBO[nextIndex]);

            auto src = (GLubyte*)::glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
            IZ_ASSERT(src);

            CALL_GL_API(::glUnmapBuffer(GL_PIXEL_PACK_BUFFER));

            CALL_GL_API(::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0));

            CALL_GL_API(::glDrawBuffer(GL_BACK));
        }
#endif
#else
        auto d3d9device = (D3D_DEVICE*)device->GetPlatformInterface();

        // ロック可能なサーフェイスを作成.
        D3D_SURFACE* surface;
        auto hr = d3d9device->CreateOffscreenPlainSurface(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            D3DFMT_X8R8G8B8,
            //1920, 1080,
            //D3DFMT_A8R8G8B8,
            D3DPOOL_SYSTEMMEM,
            &surface,
            NULL);
        IZ_ASSERT(SUCCEEDED(hr));

#if 1
        // バックバッファの取得.
        D3D_SURFACE* backBufffer;
        hr = d3d9device->GetRenderTarget(0, &backBufffer);
        IZ_ASSERT(SUCCEEDED(hr));

        // バックバッファデータ転送.
        hr = d3d9device->GetRenderTargetData(backBufffer, surface);
        IZ_ASSERT(SUCCEEDED(hr));

        backBufffer->Release();
#else
        hr = d3d9device->GetFrontBufferData(0, surface);
        IZ_ASSERT(SUCCEEDED(hr));
#endif
        surface->Release();
#endif
    }
    auto time = timer.End();
    IZ_PRINTF("%f\n", time);
#endif

    procScreenCapture();

    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    device->SetTexture(0, m_Img->GetTexture(0));

    IZ_UINT techIdx = 1;

    m_Shader->Begin(device, techIdx, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix44));

            izanagi::math::SMatrix44 mtxL2W;
            izanagi::math::SMatrix44::SetUnit(mtxL2W);

            for (IZ_UINT i = 0; i < MAX_MESH_NUM; i++) {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_objects[i].mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    captureScreen();

    if (m_enabled2DRender) {
        if (device->Begin2D()) {
            static const IZ_COLOR colors[] = {
                IZ_COLOR_RGBA(0xff, 0x00, 0x00, 0xff),
                IZ_COLOR_RGBA(0x00, 0xff, 0x00, 0xff),
                IZ_COLOR_RGBA(0x00, 0x00, 0xff, 0xff),
            };

            for (IZ_UINT i = 0; i < MAX_MESH_NUM; i++) {
                izanagi::CIntRect rc(
                    m_objects[i].point2D[0][0],
                    m_objects[i].point2D[0][1],
                    m_objects[i].point2D[1][0] - m_objects[i].point2D[0][0],
                    m_objects[i].point2D[1][1] - m_objects[i].point2D[0][1]);

                device->Draw2DRect(rc, colors[i % COUNTOF(colors)]);
            }

            device->End2D();
        }
    }

    izanagi::CDebugFont* debugFont = GetDebugFont();

    if (device->Begin2D()) {
        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        debugFont->DBPrint(
            device,
            "%d\n",
            m_curNum);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL CDistributionApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
#if 0
    static const IZ_UINT MESH_NUM_ORDER = 100;

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        m_curNum += MESH_NUM_ORDER;
    }
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        if (m_curNum > MESH_NUM_ORDER) {
            m_curNum -= MESH_NUM_ORDER;
        }
    }
#endif

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_enabled2DRender = !m_enabled2DRender;
    }

    return IZ_TRUE;
}
