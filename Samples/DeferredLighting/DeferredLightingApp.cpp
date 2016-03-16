#include "DeferredLightingApp.h"

// NOTE
// http://learnopengl.com/#!Advanced-Lighting/Deferred-Shading
// http://enginetrouble.net/2014/10/classic-deferred-shading-2014.html
// http://ogldev.atspace.co.uk/www/tutorial35/tutorial35.html
// http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html
// http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html

DeferredLightingApp::DeferredLightingApp()
{
}

DeferredLightingApp::~DeferredLightingApp()
{
}

// 初期化.
IZ_BOOL DeferredLightingApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
                    | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
                    | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    // Sphere
    for (IZ_UINT i = 0; i < MESH_NUM; i++)
    {
        m_spheres[i].mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            10.0f, 10, 10);
        VGOTO(result = (m_spheres[i].mesh != IZ_NULL), __EXIT__);

        izanagi::math::SVector4 pos;
        pos.Set(20.0f * i, 0.0f, 0.0f);

        izanagi::math::SMatrix44::GetTrans(
            m_spheres[i].mtxL2W,
            pos);
    }

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/GBuffer.shd"), __EXIT__);

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/earth.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(allocator, device, &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
    }

    // GBuffer.
    result = m_gbuffer.initialize(allocator, device);
    VGOTO(result, __EXIT__);

    IZ_FLOAT fNear = 1.0f;
    IZ_FLOAT fFar = 500.0f;
    IZ_FLOAT verticalFOV = izanagi::math::CMath::Deg2Rad(60.0f);
    IZ_FLOAT aspect = (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight();

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        fNear,
        fFar,
        verticalFOV,
        aspect);
    camera.Update();

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void DeferredLightingApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < MESH_NUM; i++)
    {
        SAFE_RELEASE(m_spheres[i].mesh);
    }

    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Img);

    m_gbuffer.release();
}

// 更新.
void DeferredLightingApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void DeferredLightingApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    m_gbuffer.begin(device);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            m_Shader->SetTexture("tex", m_Img->GetTexture(0));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            for (IZ_UINT i = 0; i < MESH_NUM; i++)
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_spheres[i].mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_spheres[i].mesh->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    m_gbuffer.end(device);

    m_gbuffer.drawBuffers(device);
}
