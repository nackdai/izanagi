#include "SSAOApp.h"
#include "izGraph.h"
#include "izSystem.h"

static izanagi::math::SVector4 samples[32];

SSAOApp::SSAOApp()
{
    m_Shader = IZ_NULL;
    m_Sphere = IZ_NULL;
    m_Cube = IZ_NULL;
    m_Plane = IZ_NULL;

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

    m_gbuffer.initialize(allocator, device);

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

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;
    IZ_COLOR color = IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff);

    // 球
    {
        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            color,
            5.0f,
            20, 20);
        VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
    }

    // キューブ
    {
        m_Cube = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            color,
            10.0f, 10.0f, 10.0f);
        VGOTO(result = (m_Cube != IZ_NULL), __EXIT__);
    }

    // 地面
    {
        m_Plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            color,
            10, 10,
            50.0f, 50.0f);
        VGOTO(result = (m_Plane != IZ_NULL), __EXIT__);
    }

    {
        IZ_FLOAT u = 0.5f / 1280.0f;
        IZ_FLOAT v = 0.5f / 720.0f;

        struct SVtx {
            IZ_FLOAT pos[4];
            IZ_FLOAT uv[2];
        } vtx[] = {
            {
                { 0.0f, 0.0f, 0.0f, 1.0f },
                { 0.0f + u, 0.0f + v },
            },
            {
                { 1.0f, 0.0f, 0.0f, 1.0f },
                { 1.0f + u, 0.0f + v },
            },
            {
                { 0.0f, 1.0f, 0.0f, 1.0f },
                { 0.0f + u, 1.0f + v },
            },
            {
                { 1.0f, 1.0f, 0.0f, 1.0f },
                { 1.0f + u, 1.0f + v },
            },
        };

        m_VB = device->CreateVertexBuffer(sizeof(SVtx), COUNTOF(vtx), izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        m_VB->LockBuffer(device, 0, 0, IZ_FALSE);
        m_VB->WriteBuffer(device, vtx, sizeof(vtx));
        m_VB->UnlockBuffer(device);

        izanagi::graph::SVertexElement elem[] = {
            { 0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0 },
            { 0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0 },
        };

        m_VD = device->CreateVertexDeclaration(elem, COUNTOF(elem));
    }

    izanagi::math::SMatrix44::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return result;
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
    const auto& camera = GetCamera();

    m_gbuffer.beginGeometryPass(device);

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    const auto& mtxW2V = camera.GetParam().mtxW2V;
    const auto& mtxW2C = camera.GetParam().mtxW2C;
    const auto& mtxV2C = camera.GetParam().mtxV2C;
    IZ_FLOAT farClip = camera.GetParam().cameraFar;

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            _SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&mtxW2V,
                sizeof(mtxW2V));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&mtxW2C,
                sizeof(mtxW2C));

            _SetShaderParam(
                m_Shader,
                "g_farClip",
                (void*)&farClip,
                sizeof(farClip));

            // ライトパラメータ
            {
                // Ambient Light Color
                izanagi::SAmbientLightParam ambient;
                ambient.color.Set(0.75f, 0.75f, 0.75f);

                // マテリアル
                izanagi::SMaterialParam mtrl;
                {
                    mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
                }

                _SetShaderParam(m_Shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));
                _SetShaderParam(m_Shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
            }

            // 地面
            RenderScene(
                device,
                m_Plane,
                izanagi::math::CVector4(0.0f, 0.0f, 0.0f));

            // 球
            RenderScene(
                device,
                m_Sphere,
                izanagi::math::CVector4(10.0f, 5.0f, 10.0f));
            RenderScene(
                device,
                m_Sphere,
                izanagi::math::CVector4(10.0f, 5.0f, -10.0f));

            // キューブ
            RenderScene(
                device,
                m_Cube,
                izanagi::math::CVector4(-10.0f, 5.0f, -10.0f));
            RenderScene(
                device,
                m_Cube,
                izanagi::math::CVector4(-10.0f, 5.0f, 10.0f));
        }
    }
    m_Shader->End(device);

    m_gbuffer.endGeometryPass(device);

#if 1
    m_gbuffer.beginSSAOPass(device, m_Shader);

    izanagi::graph::CShaderProgram* program = m_Shader->GetShaderProgram(1, 0);
    device->SetShaderProgram(program);

    device->SetTexture(0, m_gbuffer.getBuffer(GBuffer::Type::Albedo));
    device->SetTexture(1, m_gbuffer.getBuffer(GBuffer::Type::Normal));
    device->SetTexture(2, m_gbuffer.getBuffer(GBuffer::Type::Depth));

    izanagi::math::SMatrix44 mtxC2V;
    izanagi::math::SMatrix44::Inverse(mtxC2V, mtxV2C);

    izanagi::math::SMatrix44 mtxV2W;
    izanagi::math::SMatrix44::Inverse(mtxV2W, mtxW2V);

    izanagi::SHADER_PARAM_HANDLE hMtxC2V = program->GetHandleByName("g_mC2V");
    izanagi::SHADER_PARAM_HANDLE hMtxV2W = program->GetHandleByName("g_mV2W");
    izanagi::SHADER_PARAM_HANDLE hMtxW2C = program->GetHandleByName("g_mW2C");
    izanagi::SHADER_PARAM_HANDLE hSamples = program->GetHandleByName("samples");
    izanagi::SHADER_PARAM_HANDLE hFarClip = program->GetHandleByName("g_farClip");

    program->SetMatrix(device, hMtxC2V, mtxC2V);
    program->SetMatrix(device, hMtxV2W, mtxV2W);
    program->SetMatrix(device, hMtxW2C, mtxW2C);
    program->SetValue(device, hSamples, samples, sizeof(samples));
    program->SetFloat(device, hFarClip, farClip);

    device->SetVertexBuffer(0, 0, m_VB->GetStride(), m_VB);
    device->SetVertexDeclaration(m_VD);

    device->DrawPrimitive(
        izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
        0,
        2);

    m_gbuffer.endSSAOPass(device, m_Shader);
#endif

    m_gbuffer.drawBuffers(device);
}

void SSAOApp::RenderScene(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CDebugMesh* mesh,
    const izanagi::math::SVector4& position)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::GetTrans(mtxL2W, position);

    _SetShaderParam(
        m_Shader,
        "g_mL2W",
        (void*)&mtxL2W,
        sizeof(mtxL2W));

    m_Shader->CommitChanges(device);

    mesh->Draw(device);
}

void SSAOApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Plane);

    SAFE_RELEASE(m_VB);
    SAFE_RELEASE(m_VD);

    m_gbuffer.release();
}

IZ_BOOL SSAOApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
