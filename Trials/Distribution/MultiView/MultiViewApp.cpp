#include "MultiViewApp.h"

// NOTE
// http://ramemiso.hateblo.jp/entry/2013/10/18/214033

MultiViewApp::MultiViewApp()
{
}

MultiViewApp::~MultiViewApp()
{
}

// 初期化.
IZ_BOOL MultiViewApp::InitInternal(
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
        5.0f,
        10, 10);
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
void MultiViewApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Mesh);
    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_ps);
    SAFE_RELEASE(m_shd);
}

// 更新.
void MultiViewApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();
    
    camera.Update();
}

// 描画.
void MultiViewApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    const auto& camera = GetCamera();

    const auto& mtxW2C = camera.GetParam().mtxW2C;

    device->SetTexture(0, m_Img->GetTexture(0));

    device->SetShaderProgram(m_shd);

    auto handleMtxW2C = m_shd->GetHandleByName("mtxW2C");
    m_shd->SetMatrix(device, handleMtxW2C, mtxW2C);

#if 0
    izanagi::math::SVector4 vEyeClipEdge[2];
    vEyeClipEdge[0].Set(-1.0f, 0.0f, 0.0f, 1.0f);
    vEyeClipEdge[1].Set( 1.0f, 0.0f, 0.0f, 1.0f);

    auto handleEyeClipEdge = m_shd->GetHandleByName("EyeClipEdge");
    m_shd->SetVectorArray(device, handleEyeClipEdge, vEyeClipEdge, COUNTOF(vEyeClipEdge));
#endif

    glEnable(GL_CLIP_DISTANCE0);

    device->SetIndexBuffer(m_Mesh->GetIB());

    device->SetVertexDeclaration(m_Mesh->GetVD());

    device->SetVertexBufferInstanced(
        0,
        0,
        m_Mesh->GetVB()->GetStride(),
        izanagi::graph::E_GRAPH_VB_USAGE_INDEXEDDATA,
        2,
        m_Mesh->GetVB());

    device->DrawIndexedInstancedPrimitive(
        m_Mesh->GetPrimitiveType(),
        0,
        m_Mesh->GetVB()->GetVtxNum(),
        0,
        m_Mesh->GetPrimitiveCount());
}

IZ_BOOL MultiViewApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
