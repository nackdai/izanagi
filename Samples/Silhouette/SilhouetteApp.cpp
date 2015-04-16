#include "SilhouetteApp.h"

CDebugMeshApp::CDebugMeshApp()
{
    FILL_ZERO(m_Mesh, sizeof(m_Mesh));

    m_Img = IZ_NULL;

    m_Shader = IZ_NULL;
}

CDebugMeshApp::~CDebugMeshApp()
{
}

// 初期化.
IZ_BOOL CDebugMeshApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
                    | izanagi::E_DEBUG_MESH_VTX_FORM_UV
                    | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
                    | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT;

    // Sphere
    {
        m_Mesh[MESH_TYPE_SPHERE] = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
                                    allocator,
                                    device,
                                    flag,
                                    IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                                    5.0f,
                                    10, 10);
        VGOTO(result = (m_Mesh[MESH_TYPE_SPHERE] != IZ_NULL), __EXIT__);
    }

    // Cube
    {
        m_Mesh[MESH_TYPE_CUBE] = izanagi::CDebugMeshBox::CreateDebugMeshBox(
                                    allocator,
                                    device,
                                    flag,
                                    IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                                    10.0f, 10.0f, 10.0f);
        VGOTO(result = (m_Mesh[MESH_TYPE_CUBE] != IZ_NULL), __EXIT__);
    }

    // Cylinder
    {
        m_Mesh[MESH_TYPE_CYLINDER] = izanagi::CDebugMeshCylinder::CreateDebugMeshCylinder(
                                        allocator,
                                        device,
                                        flag,
                                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                                        5.0f, 10.0f,
                                        10, 10);
        VGOTO(result = (m_Mesh[MESH_TYPE_CYLINDER] != IZ_NULL), __EXIT__);
    }

    // Torus
    {
        m_Mesh[MESH_TYPE_TORUS] = izanagi::CDebugMeshTorus::CreateDebugMeshTorus(
                                        allocator,
                                        device,
                                        flag,
                                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                                        2.0f, 5.0f,
                                        10, 10);
        VGOTO(result = (m_Mesh[MESH_TYPE_TORUS] != IZ_NULL), __EXIT__);
    }

    // Plane
    {
        m_Mesh[MESH_TYPE_PLANE] = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
                                        allocator,
                                        device,
                                        flag,
                                        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                                        10, 10,
                                        10.0f, 10.0f);
        VGOTO(result = (m_Mesh[MESH_TYPE_PLANE] != IZ_NULL), __EXIT__);
    }

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
        VGOTO(result = in.Open("data/SilhouetteShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 10.0f, 30.0f, 1.0f),
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
void CDebugMeshApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < MESH_TYPE_NUM; i++) {
        SAFE_RELEASE(m_Mesh[i]);
    }

    SAFE_RELEASE(m_Img);

    SAFE_RELEASE(m_Shader);
}

// 更新.
void CDebugMeshApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();
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

void CDebugMeshApp::RenderMeshes(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT techIdx)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    device->SetTexture(0, m_Img->GetTexture(0));

    // 通常描画
    m_Shader->Begin(device, techIdx, IZ_TRUE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(izanagi::math::SMatrix44));

            // Sphere
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh[MESH_TYPE_SPHERE]->Draw(device, isDrawTangentSpaceAxis);
            }

            // Cube
            {
                izanagi::math::SMatrix44::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector4(-15.0f, 0.0f, -15.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh[MESH_TYPE_CUBE]->Draw(device, isDrawTangentSpaceAxis);
            }

            // Cylinder
            {
                izanagi::math::SMatrix44::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector4(15.0f, 0.0f, -15.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh[MESH_TYPE_CYLINDER]->Draw(device, isDrawTangentSpaceAxis);
            }


            // Torus
            {
                izanagi::math::SMatrix44::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector4(15.0f, 0.0f, 15.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh[MESH_TYPE_TORUS]->Draw(device, isDrawTangentSpaceAxis);
            }

            // Plane
            {
                izanagi::math::SMatrix44::GetTrans(
                    mtxL2W,
                    izanagi::math::CVector4(-15.0f, 0.0f, 15.0f));

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_Mesh[MESH_TYPE_PLANE]->Draw(device, isDrawTangentSpaceAxis);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}

// 描画.
void CDebugMeshApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    RenderMeshes(device, 0);
    RenderMeshes(device, 1);
}
