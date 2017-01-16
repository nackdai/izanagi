#include "PointCloudApp.h"
#include "PlyReader.h"

#include "PCDFormat.h"

PointCloudApp::PointCloudApp()
{
}

PointCloudApp::~PointCloudApp()
{
}

// 初期化.
IZ_BOOL PointCloudApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 500.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        1000.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    //readPly(device);
    readPcd(device);

    {
        izanagi::graph::SVertexElement elems[] = {
            { 0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0 },
            { 0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR, 0 },
        };

        m_vd = device->CreateVertexDeclaration(elems, COUNTOF(elems));
    }

    readShaders(device);

    m_rtDepth = device->CreateRenderTarget(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
    m_rtWeightedColor = device->CreateRenderTarget(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA16F);
    
__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

void PointCloudApp::readPly(izanagi::graph::CGraphicsDevice* device)
{
    PlyReader reader;
    reader.open("data/bunny.ply");

    m_pointNum = reader.getVtxNum();

    m_vb = device->CreateVertexBuffer(
        sizeof(Vertex),
        m_pointNum,
        izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

    Vertex* vtx;
    auto pitch = m_vb->Lock(device, 0, 0, (void**)&vtx, IZ_FALSE);

    PlyReader::Vertex plyVtx;

    while (reader.readVtx(plyVtx)) {
        vtx->pos[0] = plyVtx.pos[0] * 1000.0f;
        vtx->pos[1] = plyVtx.pos[1] * 1000.0f;
        vtx->pos[2] = plyVtx.pos[2] * 1000.0f;
        vtx->pos[3] = 1.0f;

        vtx->color = IZ_COLOR_RGBA(
            plyVtx.color[0],
            plyVtx.color[1],
            plyVtx.color[2],
            plyVtx.color[3]);

        vtx++;
    }

    m_vb->Unlock(device);
}

void PointCloudApp::readPcd(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CFileInputStream in;
    auto res = in.Open("data/Stairs.pcd");
    IZ_ASSERT(res);

    PCDHeader header;

    in.Read(&header, 0, sizeof(header));

    m_pointNum = header.vtxNum;

    m_vb = device->CreateVertexBuffer(
        sizeof(Vertex),
        m_pointNum,
        izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

    Vertex* vtx;
    auto pitch = m_vb->Lock(device, 0, 0, (void**)&vtx, IZ_FALSE);

    struct {
        float pos[3];
        IZ_COLOR color;
    } tmp;

    static const float scale = 100.0f;

    for (uint32_t i = 0; i < m_pointNum; i++) {
        in.Read(&tmp, 0, sizeof(tmp));

        vtx->pos[0] = -tmp.pos[0] * scale;
        vtx->pos[1] = tmp.pos[1] * scale;
        vtx->pos[2] = tmp.pos[2] * scale;
        vtx->pos[3] = 1.0f;

        vtx->color = tmp.color;

        vtx++;
    }

    m_vb->Unlock(device);
}

void PointCloudApp::readShaders(izanagi::graph::CGraphicsDevice* device)
{
    m_shdDefault = initShader(device, "shader/vs.glsl", "shader/ps.glsl");
    m_shdInterp = initShader(device, "shader/vs.glsl", "shader/psInterp.glsl");
    m_shdDepth = initShader(device, "shader/vs.glsl", "shader/psDepth.glsl");
    m_shdSplat = initShader(device, "shader/vs.glsl", "shader/psWeightSplat.glsl");
    m_shdNml = initShader(device, "shader/vsNormalize.glsl", "shader/psNormalize.glsl");
}

izanagi::graph::CShaderProgram* PointCloudApp::initShader(
    izanagi::graph::CGraphicsDevice* device,
    const char* vsPath,
    const char* psPath)
{
    izanagi::graph::CVertexShader* vs = nullptr;
    izanagi::graph::CPixelShader* ps = nullptr;

    {
        izanagi::CFileInputStream in;
        in.Open(vsPath);

        std::vector<IZ_BYTE> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, buf.size());

        buf[buf.size() - 1] = 0;

        vs = device->CreateVertexShader(&buf[0]);
    }

    {
        izanagi::CFileInputStream in;
        in.Open(psPath);

        std::vector<IZ_BYTE> buf(in.GetSize() + 1);
        in.Read(&buf[0], 0, buf.size());

        buf[buf.size() - 1] = 0;

        ps = device->CreatePixelShader(&buf[0]);
    }

    auto shd = device->CreateShaderProgram();
    shd->AttachVertexShader(vs);
    shd->AttachPixelShader(ps);

    SAFE_RELEASE(vs);
    SAFE_RELEASE(ps);

    return shd;
}

// 解放.
void PointCloudApp::ReleaseInternal()
{
    SAFE_RELEASE(m_vb);
    SAFE_RELEASE(m_vd);

    SAFE_RELEASE(m_shdDefault);
    SAFE_RELEASE(m_shdInterp);
    SAFE_RELEASE(m_shdDepth);
    SAFE_RELEASE(m_shdSplat);
    SAFE_RELEASE(m_shdNml);

    SAFE_RELEASE(m_rtDepth);
    SAFE_RELEASE(m_rtWeightedColor);
}

// 更新.
void PointCloudApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();
    
    camera.Update();
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

// 描画.
void PointCloudApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_mode == Mode::Default) {
        renderScene(device, m_shdDefault);
    }
    else if (m_mode == Mode::Interp) {
        renderScene(device, m_shdInterp);
    }
    else {
        device->SaveRenderState();

        // Draw linear depth to the render target.
        renderDepth(device);

        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
            IZ_FALSE);
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZENABLE,
            IZ_FALSE);

        renderWeightedColor(device);

        renderNormalize(device);

        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
            IZ_TRUE);
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZENABLE,
            IZ_TRUE);

        device->LoadRenderState();

#if 0
        if (device->Begin2D()) {
            device->SetTexture(0, m_rtDepth);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(0, 100, 320, 180));

            device->SetTexture(0, m_rtWeightedColor);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(0, 280, 320, 180));

            device->End2D();
        }
#endif
    }

#if 1
    if (device->Begin2D()) {
        static const char* mode[Mode::None] = {
            "Default",
            "Interp",
            "WeightSplat",
        };

        auto font = GetDebugFont();

        font->Begin(device);

        font->DBPrint(
            device,
            0, 40,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            mode[m_mode]);

        font->DBPrint(
            device,
            0, 60,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            "Size:%f", m_PointSize);

        font->End();

        device->End2D();
    }
#endif
}

void PointCloudApp::renderScene(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::graph::CShaderProgram* shd,
    izanagi::graph::CTexture* tex)
{
    CALL_GL_API(::glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
    CALL_GL_API(::glEnable(GL_POINT_SPRITE));

    device->SetShaderProgram(shd);

    auto& camera = GetCamera();

    const izanagi::math::SMatrix44& mtxW2V = camera.GetParam().mtxW2V;
    const izanagi::math::SMatrix44& mtxV2C = camera.GetParam().mtxV2C;
    const izanagi::math::SMatrix44& mtxW2C = camera.GetParam().mtxW2C;

    auto fov = camera.GetParam().verticalFOV;
    auto screenHeight = SCREEN_HEIGHT;
    auto farClip = camera.GetParam().cameraFar;

    izanagi::math::CVector4 screen(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    auto hSize = shd->GetHandleByName("size");
    shd->SetFloat(device, hSize, m_PointSize);

    auto hMtxW2C = shd->GetHandleByName("mtxW2C");
    shd->SetMatrixArrayAsVectorArray(device, hMtxW2C, &mtxW2C, 4);

    auto hMtxW2V = shd->GetHandleByName("mtxW2V");
    shd->SetMatrixArrayAsVectorArray(device, hMtxW2V, &mtxW2V, 4);

    auto hMtxV2C = shd->GetHandleByName("mtxV2C");
    shd->SetMatrixArrayAsVectorArray(device, hMtxV2C, &mtxV2C, 4);

    auto hScreenHeight = shd->GetHandleByName("screenHeight");
    shd->SetFloat(device, hScreenHeight, screenHeight);

    auto hFOV = shd->GetHandleByName("fov");
    shd->SetFloat(device, hFOV, fov);

    auto hFar = shd->GetHandleByName("farClip");
    shd->SetFloat(device, hFar, farClip);

    auto hScreen = shd->GetHandleByName("screen");
    shd->SetVector(device, hScreen, screen);

    device->SetVertexBuffer(0, 0, sizeof(Vertex), m_vb);
    device->SetVertexDeclaration(m_vd);

    device->SetTexture(0, IZ_NULL);
    if (tex) {
        CALL_GL_API(::glActiveTexture(GL_TEXTURE0));

        GLuint handle = tex->GetTexHandle();

        CALL_GL_API(::glBindTexture(GL_TEXTURE_2D, handle));

        auto hDepthMap = shd->GetHandleByName("depthMap");

        CALL_GL_API(glUniform1i(hDepthMap, 0));
    }

    device->DrawPrimitive(
        izanagi::graph::E_GRAPH_PRIM_TYPE_POINTLIST,
        0,
        m_pointNum);

    CALL_GL_API(::glBindTexture(GL_TEXTURE_2D, 0));
}

void PointCloudApp::renderDepth(izanagi::graph::CGraphicsDevice* device)
{
    device->BeginScene(
        &m_rtDepth,
        1,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0x00));

    // Draw linear depth.
    renderScene(device, m_shdDepth);

    device->EndScene();
}

void PointCloudApp::renderWeightedColor(izanagi::graph::CGraphicsDevice* device)
{
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_BLENDMETHOD,
        izanagi::graph::E_GRAPH_ALPHA_BLEND_CsAs_Cd);

    device->BeginScene(
        &m_rtWeightedColor,
        1,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0x00));

    renderScene(device, m_shdSplat, m_rtDepth);

    device->EndScene();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_BLENDMETHOD,
        izanagi::graph::E_GRAPH_ALPHA_BLEND_NORMAL);
}

void PointCloudApp::renderNormalize(izanagi::graph::CGraphicsDevice* device)
{
    // For rendering full screen quad without vertex buffer.
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    device->SetShaderProgram(m_shdNml);

    izanagi::math::CVector4 invScreen(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0.0f);
    auto hInvScreen = m_shdNml->GetHandleByName("invScreen");
    m_shdNml->SetVector(device, hInvScreen, invScreen);

#if 0
    {
        CALL_GL_API(::glActiveTexture(GL_TEXTURE0));
        GLuint handle = m_rtDepth->GetTexHandle();
        CALL_GL_API(::glBindTexture(GL_TEXTURE_2D, handle));

        auto hDepthMap = m_shdNml->GetHandleByName("depthMap");
        CALL_GL_API(glUniform1i(hDepthMap, 0));
    }
    {
        CALL_GL_API(::glActiveTexture(GL_TEXTURE1));
        GLuint handle = m_rtWeightedColor->GetTexHandle();
        CALL_GL_API(::glBindTexture(GL_TEXTURE_2D, handle));

        auto hImage = m_shdNml->GetHandleByName("image");
        CALL_GL_API(glUniform1i(hImage, 1));
    }
#else
    // TODO
    // なぜか、SetTexture でテクスチャをセットいないと正しくテクスチャがセットされない.
    // SetTexture内部でやっている、filterやaddressの設定の問題？.

    device->SetTexture(0, m_rtDepth);
    auto hDepthMap = m_shdNml->GetHandleByName("depthMap");
    CALL_GL_API(glUniform1i(hDepthMap, 0));

    device->SetTexture(1, m_rtWeightedColor);
    auto hImage = m_shdNml->GetHandleByName("image");
    CALL_GL_API(glUniform1i(hImage, 1));
#endif

    // NOTE
    // 頂点バッファを使わず全画面に描画する頂点シェーダ.
    CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
}

IZ_BOOL PointCloudApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    IZ_INT mode = m_mode;

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        mode++;
        m_mode = (Mode)(mode >= Mode::None ? 0 : mode);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        mode--;
        m_mode = (Mode)(mode < 0 ? Mode::None - 1 : mode);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_LEFT) {
        m_PointSize -= 0.1f;
        m_PointSize = (m_PointSize < 0.5f ? 0.5f : m_PointSize);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_RIGHT) {
        m_PointSize += 0.1f;
        m_PointSize = (m_PointSize > 7.5f ? 7.5f : m_PointSize);
    }

    return IZ_TRUE;
}
