#include "SinglePassStereoRenderingApp.h"

// NOTE
// http://ramemiso.hateblo.jp/entry/2013/10/18/214033

static const IZ_FLOAT POS_X = -50.0f;
static const IZ_FLOAT DISTANCE = 10.0f;
static const IZ_UINT ORDER = 20;

SinglePassStereoRenderingApp::SinglePassStereoRenderingApp()
{
}

SinglePassStereoRenderingApp::~SinglePassStereoRenderingApp()
{
}

// 初期化.
IZ_BOOL SinglePassStereoRenderingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    //PFNWGLSWAPINTERVALEXTPROC proc = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    //proc(0);

    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    for (IZ_UINT i = 0; i < MeshNum; i++) {
        m_Mesh[i] = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            5.0f,
            10, 10);
        VGOTO(result = (m_Mesh[i] != IZ_NULL), __EXIT__);

        izanagi::math::SMatrix44::SetUnit(m_L2W[i]);
        izanagi::math::SMatrix44::GetTrans(
            m_L2W[i],
            POS_X + DISTANCE * (i % ORDER),
            0.0f,
            -DISTANCE * (i / ORDER));
    }

    // テクスチャ
    for (IZ_UINT i = 0; i < 10; i++) {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/earth.img"), __EXIT__);

        m_Img[i] = izanagi::CImage::CreateImage(
            allocator,
            device,
            &in);
        VGOTO(result = (m_Img[i] != IZ_NULL), __EXIT__);
    }

    {
        {
            izanagi::CFileInputStream in;
            in.Open("shader/vs.glsl");

            std::vector<IZ_BYTE> buf(in.GetSize() + 1);
            in.Read(&buf[0], 0, buf.size());

            buf[buf.size() - 1] = 0;

            m_vs = device->CreateVertexShader(&buf[0]);
        }

        {
            izanagi::CFileInputStream in;
            in.Open("shader/ps.glsl");

            std::vector<IZ_BYTE> buf(in.GetSize() + 1);
            in.Read(&buf[0], 0, buf.size());

            buf[buf.size() - 1] = 0;

            m_ps = device->CreatePixelShader(&buf[0]);
        }

        m_shd = device->CreateShaderProgram();
        m_shd->AttachVertexShader(m_vs);
        m_shd->AttachPixelShader(m_ps);
    }

    {
        {
            izanagi::CFileInputStream in;
            in.Open("shader/vs_default.glsl");

            std::vector<IZ_BYTE> buf(in.GetSize() + 1);
            in.Read(&buf[0], 0, buf.size());

            buf[buf.size() - 1] = 0;

            m_vs_default = device->CreateVertexShader(&buf[0]);
        }

        {
            izanagi::CFileInputStream in;
            in.Open("shader/ps.glsl");

            std::vector<IZ_BYTE> buf(in.GetSize() + 1);
            in.Read(&buf[0], 0, buf.size());

            buf[buf.size() - 1] = 0;

            m_ps_default = device->CreatePixelShader(&buf[0]);
        }

        m_shd_default = device->CreateShaderProgram();
        m_shd_default->AttachVertexShader(m_vs_default);
        m_shd_default->AttachPixelShader(m_ps_default);
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

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void SinglePassStereoRenderingApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < MeshNum; i++) {
        SAFE_RELEASE(m_Mesh[i]);

        SAFE_RELEASE(m_Img[i]);
    }

    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_ps);
    SAFE_RELEASE(m_shd);

    SAFE_RELEASE(m_vs_default);
    SAFE_RELEASE(m_ps_default);
    SAFE_RELEASE(m_shd_default);
}

// 更新.
void SinglePassStereoRenderingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();
    
    camera.Update();
}

// 描画.
void SinglePassStereoRenderingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_enableAdvanced) {
        RenderAdvancedStereo(device);
    }
    else {
        RenderDefaultStereo(device);
    }

    if (device->Begin2D()) {
        auto font = GetDebugFont();

        font->Begin(device);

        font->DBPrint(
            device,
            0, 40,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            m_enableAdvanced ? "Advance" : "Default");

        font->DBPrint(
            device,
            0, 60,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            "Num:%d", m_curNum);

        font->End();

        device->End2D();
    }
}

void SinglePassStereoRenderingApp::RenderDefaultStereo(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;

    static const izanagi::graph::SViewport vp[2] = {
        {
            0, 0,
            SCREEN_WIDTH / 2, SCREEN_HEIGHT,
            0.0f, 1.0f
        },
        {
            SCREEN_WIDTH / 2, 0,
            SCREEN_WIDTH / 2, SCREEN_HEIGHT,
            0.0f, 1.0f
        },
    };

    auto& defaultVP = device->GetViewport();

    for (IZ_UINT n = 0; n < 2; n++) {
        device->SetViewport(vp[n]);

        for (IZ_UINT i = 0; i < m_curNum; i++) {
            auto pos = i % COUNTOF(m_Img);
            device->SetTexture(0, m_Img[pos]->GetTexture(0));

            device->SetShaderProgram(m_shd_default);

            auto handleMtxW2C = m_shd->GetHandleByName("mtxW2C");
            m_shd->SetMatrix(device, handleMtxW2C, mtxW2C);

            auto handleMtxL2W = m_shd->GetHandleByName("mtxL2W");
            m_shd->SetMatrix(device, handleMtxL2W, m_L2W[i]);

#if 0
            izanagi::math::SVector4 vEyeClipEdge[2];
            vEyeClipEdge[0].Set(-1.0f, 0.0f, 0.0f, 1.0f);
            vEyeClipEdge[1].Set(1.0f, 0.0f, 0.0f, 1.0f);

            auto handleEyeClipEdge = m_shd->GetHandleByName("EyeClipEdge");
            m_shd->SetVectorArray(device, handleEyeClipEdge, vEyeClipEdge, COUNTOF(vEyeClipEdge));
#endif

            device->SetIndexBuffer(m_Mesh[i]->GetIB());

            device->SetVertexDeclaration(m_Mesh[i]->GetVD());

            device->SetVertexBuffer(
                0,
                0,
                m_Mesh[i]->GetVB()->GetStride(),
                m_Mesh[i]->GetVB());

            device->DrawIndexedPrimitive(
                m_Mesh[i]->GetPrimitiveType(),
                0,
                m_Mesh[i]->GetVB()->GetVtxNum(),
                0,
                m_Mesh[i]->GetPrimitiveCount());
        }
    }

    device->SetViewport(defaultVP);
}

void SinglePassStereoRenderingApp::RenderAdvancedStereo(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;

    for (IZ_UINT i = 0; i < m_curNum; i++) {
        auto pos = i % COUNTOF(m_Img);
        device->SetTexture(0, m_Img[pos]->GetTexture(0));

        device->SetShaderProgram(m_shd);

        auto handleMtxW2C = m_shd->GetHandleByName("mtxW2C");
        m_shd->SetMatrix(device, handleMtxW2C, mtxW2C);

        auto handleMtxL2W = m_shd->GetHandleByName("mtxL2W");
        m_shd->SetMatrix(device, handleMtxL2W, m_L2W[i]);

#if 0
        izanagi::math::SVector4 vEyeClipEdge[2];
        vEyeClipEdge[0].Set(-1.0f, 0.0f, 0.0f, 1.0f);
        vEyeClipEdge[1].Set( 1.0f, 0.0f, 0.0f, 1.0f);

        auto handleEyeClipEdge = m_shd->GetHandleByName("EyeClipEdge");
        m_shd->SetVectorArray(device, handleEyeClipEdge, vEyeClipEdge, COUNTOF(vEyeClipEdge));
#endif

        glEnable(GL_CLIP_DISTANCE0);

        device->SetIndexBuffer(m_Mesh[i]->GetIB());

        device->SetVertexDeclaration(m_Mesh[i]->GetVD());

        device->SetVertexBufferInstanced(
            0,
            0,
            m_Mesh[i]->GetVB()->GetStride(),
            izanagi::graph::E_GRAPH_VB_USAGE_INDEXEDDATA,
            2,
            m_Mesh[i]->GetVB());

        device->DrawIndexedInstancedPrimitive(
            m_Mesh[i]->GetPrimitiveType(),
            0,
            m_Mesh[i]->GetVB()->GetVtxNum(),
            0,
            m_Mesh[i]->GetPrimitiveCount());
    }
}

IZ_BOOL SinglePassStereoRenderingApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        m_enableAdvanced = !m_enableAdvanced;
    }

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        m_curNum += 10;
        m_curNum = (m_curNum > MeshNum ? MeshNum : m_curNum);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        m_curNum -= 10;
        m_curNum = (m_curNum < 1 ? 1 : m_curNum);
    }

    return IZ_TRUE;
}
