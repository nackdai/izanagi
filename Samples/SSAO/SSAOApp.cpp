#include "SSAOApp.h"
#include "izGraph.h"
#include "izSystem.h"

static izanagi::math::SVector4 samples[32];

SSAOApp::SSAOApp()
{
    const float SAMPLERADIUS = 0.75f;

#if 0
    izanagi::math::CMathRand::Init((IZ_UINT)app->GetTimer(0).GetCurTime());

    for (IZ_UINT i = 0; i < COUNTOF(samples); i++) {
        float r = SAMPLERADIUS * izanagi::math::CMathRand::GetRandFloat();

        // u : [0, 1)
        float u = izanagi::math::CMathRand::GetRandBetween<IZ_FLOAT>(0.0f, 0.9999999f);

        float t = IZ_MATH_PI2 * u;

        // v : [-1, 1]
        float v = izanagi::math::CMathRand::GetRandBetween<IZ_FLOAT>(-1.0f, 1.0f);

        float p = ::sqrtf(1.0f - v * v);

        float sin = izanagi::math::CMath::SinF(t);
        float cos = izanagi::math::CMath::CosF(t);

        samples[i].x = r * p * cos;
        samples[i].y = r * p * sin;
        samples[i].z = r * v;
        samples[i].w = 0.0f;
    }
#else
    srand((IZ_UINT)GetTimer(0).GetCurTime());

    for (int i = 0; i < COUNTOF(samples); ++i) {
        float r = SAMPLERADIUS * (float)rand() / (float)RAND_MAX;
        float t = 6.2831853f * (float)rand() / ((float)RAND_MAX + 1.0f);
        float cp = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
        float sp = sqrt(1.0f - cp * cp);
        float ct = cos(t), st = sin(t);

        samples[i].x = r * sp * ct;
        samples[i].y = r * sp * st;
        samples[i].z = r * cp;
        samples[i].w = 0.0f;
    }
#endif
}

SSAOApp::~SSAOApp()
{
}

// 開始
IZ_BOOL SSAOApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    result = initMeshes(allocator, device);
    VGOTO(result, __EXIT__);

    // シェーダ
    {
        izanagi::CFileInputStream in;
        //VRETURN(in.Open("data/PhongShader.shd"));
        VRETURN(in.Open("data/SSAOShader.shd"));

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
        Release();
    }

    return result;
}

IZ_BOOL SSAOApp::initMeshes(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    // 球
    {
        m_meshes[0].mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            5.0f,
            20, 20);
        VRETURN(m_meshes[0].mesh != IZ_NULL);

        m_meshes[0].mtxL2W.SetTrans(10.0f, 5.0f, 10.0f);
    }
    {
        m_meshes[1].mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            5.0f,
            20, 20);
        VRETURN(m_meshes[1].mesh != IZ_NULL);

        m_meshes[1].mtxL2W.SetTrans(10.0f, 5.0f, -10.0f);
    }

    // キューブ
    {
        m_meshes[2].mesh = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            10.0f, 10.0f, 10.0f);
        VRETURN(m_meshes[2].mesh != IZ_NULL);

        m_meshes[2].mtxL2W.SetTrans(-10.0f, 5.0f, -10.0f);
    }
    {
        m_meshes[3].mesh = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
            10.0f, 10.0f, 10.0f);
        VRETURN(m_meshes[3].mesh != IZ_NULL);

        m_meshes[3].mtxL2W.SetTrans(-10.0f, 5.0f, 10.0f);
    }

    // 地面
    {
        m_meshes[4].mesh = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
            10, 10,
            50.0f, 50.0f);
        VRETURN(m_meshes[4].mesh != IZ_NULL);
    }

    // For SSAOPass, FinalPass.
    {
        m_screenFillPlane = izanagi::CDebugMeshScreenFillPlane::create(
            allocator,
            device,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff));
        VRETURN(m_screenFillPlane != IZ_NULL);
    }

    return IZ_TRUE;
}

// 更新.
void SSAOApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
        //IZ_ASSERT(handle != 0);

        if (handle > 0) {
            shader->SetParamValue(
                handle,
                value,
                bytes);
        }
    }
}

// 描画.
void SSAOApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    renderGeometryPass(device);
    renderSSAOPass(device);

    //renderFinalPass(device);

    device->SetTexture(0, IZ_NULL);

    m_gbuffer.drawBuffers(device);
}

void SSAOApp::renderGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    m_gbuffer.beginGeometryPass(device);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            m_Shader->SetTexture("tex", m_Img->GetTexture(0));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));
            _SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&camera.GetParam().mtxW2V,
                sizeof(camera.GetParam().mtxW2V));
            _SetShaderParam(
                m_Shader,
                "g_farClip",
                (void*)&camera.GetParam().cameraFar,
                sizeof(camera.GetParam().cameraFar));

            for (IZ_UINT i = 0; i < MESH_NUM; i++)
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_meshes[i].mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                m_Shader->CommitChanges(device);

                m_meshes[i].mesh->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    m_gbuffer.endGeometryPass(device);
}

void SSAOApp::renderSSAOPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxC2V;
    izanagi::math::SMatrix44::Inverse(mtxC2V, camera.GetParam().mtxV2C);

    izanagi::math::SMatrix44 mtxV2W;
    izanagi::math::SMatrix44::Inverse(mtxV2W, camera.GetParam().mtxW2V);

    izanagi::math::CVector4 invScreenSize(
        1.0f / device->GetBackBufferWidth(),
        1.0f / device->GetBackBufferHeight(),
        0.0f);

    m_gbuffer.beginSSAOPass(device, m_Shader);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        m_Shader->SetTexture("texAlbedo", m_gbuffer.getBuffer(GBuffer::Type::Albedo));
        m_Shader->SetTexture("texDepth", m_gbuffer.getBuffer(GBuffer::Type::Depth));

        if (m_Shader->BeginPass(1)) {
            _SetShaderParam(
                m_Shader,
                "g_vInvScreen",
                (void*)&invScreenSize,
                sizeof(invScreenSize));
            
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));
            _SetShaderParam(
                m_Shader,
                "g_farClip",
                (void*)&camera.GetParam().cameraFar,
                sizeof(camera.GetParam().cameraFar));

            _SetShaderParam(
                m_Shader,
                "g_mtxC2V",
                (void*)&mtxC2V,
                sizeof(mtxC2V));
            _SetShaderParam(
                m_Shader,
                "g_mtxV2W",
                (void*)&mtxV2W,
                sizeof(mtxV2W));

            _SetShaderParam(
                m_Shader,
                "samples",
                (void*)&samples,
                sizeof(samples));

            m_Shader->CommitChanges(device);

            m_screenFillPlane->Draw(device);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

    m_gbuffer.endSSAOPass(device, m_Shader);
}

void SSAOApp::ReleaseInternal()
{
}

IZ_BOOL SSAOApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
