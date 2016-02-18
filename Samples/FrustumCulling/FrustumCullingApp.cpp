#include "FrustumCullingApp.h"

FrustumCullingApp::FrustumCullingApp()
{
    m_Shader = IZ_NULL;
}

FrustumCullingApp::~FrustumCullingApp()
{
}

// 初期化.
IZ_BOOL FrustumCullingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // Axis
    m_Axis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
                allocator,
                device);
    VGOTO(result = (m_Axis != IZ_NULL), __EXIT__);

    // Grid
    m_Grid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
                allocator,
                device,
                IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
                10, 10,
                5.0f);
    VGOTO(result = (m_Grid != IZ_NULL), __EXIT__);

    // Frustum.
    m_FrustumMesh = izanagi::CDebugMeshFrustum::CreateDebugMeshFrustum(
        allocator,
        device,
        IZ_COLOR_RGBA(0xff, 0, 0, 0x80),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight(),
        izanagi::math::CMath::Deg2Rad(60.0f),
        1.0f,
        500.0f);

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

    // Cube
    for (IZ_UINT i = 0; i < CUBE_NUM; i++)
    {
        m_cubes[i].mesh = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            10.0f, 10.0f, 10.0f);
        VGOTO(result = (m_cubes[i].mesh != IZ_NULL), __EXIT__);

        izanagi::math::SVector4 pos;
        pos.Set(20.0f * i, 0.0f, 0.0f);

        m_cubes[i].aabb.initialize(pos, 10.0f, 10.0f, 10.0f);

        izanagi::math::SMatrix44::GetTrans(
            m_cubes[i].mtxL2W,
            pos);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 00.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    const auto& mtxW2V = camera.GetParam().mtxW2V;
    izanagi::math::SMatrix44::Copy(m_mtxW2V, mtxW2V);
    izanagi::math::SMatrix44::Inverse(m_mtxL2W_Frustum, mtxW2V);

#if 0
    m_frustum.initialize(
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
#else
    m_frustum.initialize(camera.GetParam().mtxW2C);
#endif

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void FrustumCullingApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < CUBE_NUM; i++)
    {
        SAFE_RELEASE(m_cubes[i].mesh);
    }

    SAFE_RELEASE(m_Axis);
    SAFE_RELEASE(m_Grid);
    SAFE_RELEASE(m_FrustumMesh);

    SAFE_RELEASE(m_Shader);
}

// 更新.
void FrustumCullingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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

// 描画.
void FrustumCullingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    const auto& mtxW2V = camera.GetParam().mtxW2V;

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    m_Shader->Begin(device, 1, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(mtxL2W));

                // シェーダ設定
                m_Shader->CommitChanges(device);

                m_Grid->Draw(device);
                m_Axis->Draw(device);

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_mtxL2W_Frustum,
                    sizeof(m_mtxL2W_Frustum));

                // シェーダ設定
                m_Shader->CommitChanges(device);

                m_FrustumMesh->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            for (IZ_UINT i = 0; i < CUBE_NUM; i++)
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_cubes[i].mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                auto isIntersect = m_frustum.isIntersect(
                    &m_cubes[i].aabb,
                    m_mtxW2V);

                izanagi::math::SVector4 color;
                if (isIntersect) {
                    color.Set(1.0f, 0.0f, 0.0f);
                }
                else {
                    color.Set(1.0f, 1.0f, 1.0f);
                }

                _SetShaderParam(
                    m_Shader,
                    "g_Color",
                    (void*)&color,
                    sizeof(color));

                m_Shader->CommitChanges(device);

                m_cubes[i].mesh->Draw(device, isDrawTangentSpaceAxis);
            }
            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}
