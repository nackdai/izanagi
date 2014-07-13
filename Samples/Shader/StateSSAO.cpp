#include "StateSSAO.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

static izanagi::math::SVector samples[32];

CStateSSAO::CStateSSAO(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;
    m_Sphere = IZ_NULL;
    m_Cube = IZ_NULL;
    m_Plane = IZ_NULL;

    m_Mode = SSAO;

#if 0
    izanagi::math::CMathRand::Init((IZ_UINT)app->GetTimer(0).GetCurTime());

    for (IZ_UINT i = 0; i < COUNTOF(samples); i++) {
        float r = 0.1f * izanagi::math::CMathRand::GetRandFloat();

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
    srand((IZ_UINT)app->GetTimer(0).GetCurTime());

    const float SAMPLERADIUS = 0.75f;

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

CStateSSAO::~CStateSSAO()
{
    Destroy();
}

// 描画.
IZ_BOOL CStateSSAO::Render(izanagi::graph::CGraphicsDevice* device)
{
    if (m_Mode != Ambient) {
        device->BeginScene(
            m_RT,
            COUNTOF(m_RT),
            izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
            IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0));
    }

    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::SetUnit(mtxL2W);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtxL2W,
                sizeof(mtxL2W));

            SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&m_Camera.mtxW2V,
                sizeof(m_Camera.mtxW2V));

            SetShaderParam(
                m_Shader,
                "g_mV2C",
                (void*)&m_Camera.mtxV2C,
                sizeof(m_Camera.mtxV2C));

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

                SetShaderParam(m_Shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));
                SetShaderParam(m_Shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
            }

            // 地面
            RenderScene(
                device, 
                m_Plane,
                izanagi::math::CVector(0.0f, 0.0f, 0.0f));

            // 球
            RenderScene(
                device, 
                m_Sphere,
                izanagi::math::CVector(10.0f, 5.0f, 10.0f));
            RenderScene(
                device, 
                m_Sphere,
                izanagi::math::CVector(10.0f, 5.0f, -10.0f));

            // キューブ
            RenderScene(
                device, 
                m_Cube,
                izanagi::math::CVector(-10.0f, 5.0f, -10.0f));
            RenderScene(
                device, 
                m_Cube,
                izanagi::math::CVector(-10.0f, 5.0f, 10.0f));
        }
    }
    m_Shader->End(device);

    device->EndScene();

    if (m_Mode == Textures) {
        device->Begin2D();
        {
            device->SetTexture(0, m_RT[0]);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(0, 0, 640, 360));

            device->SetTexture(0, m_RT[1]);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(0, 360, 640, 360));

            device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA);

            device->SetTexture(0, m_RT[2]);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(640, 360, 640, 360));

            device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA);

            device->SetTexture(0, m_RT[3]);
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(640, 0, 640, 360));
        }
        device->End2D();
    }
    else if (m_Mode == SSAO) {
        izanagi::graph::CShaderProgram* program = m_Shader->GetShaderProgram(1, 0);
        device->SetShaderProgram(program);

        device->SetTexture(0, m_RT[0]);
        device->SetTexture(1, m_RT[1]);
        device->SetTexture(2, m_RT[2]);
        device->SetTexture(3, m_RT[3]);

        SHADER_PARAM_HANDLE h0 = program->GetHandleByName("g_mW2V");
        SHADER_PARAM_HANDLE h1 = program->GetHandleByName("g_mV2C");
        SHADER_PARAM_HANDLE h2 = program->GetHandleByName("samples");

        program->SetMatrix(device, h0, m_Camera.mtxW2V);
        program->SetMatrix(device, h1, m_Camera.mtxV2C);
        program->SetValue(device, h2, samples, sizeof(samples));

        device->SetVertexBuffer(0, 0, m_VB->GetStride(), m_VB);
        device->SetVertexDeclaration(m_VD);

        device->DrawPrimitive(
            izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
            0,
            2);
    }
    else {
        izanagi::graph::CShaderProgram* program = m_Shader->GetShaderProgram(2, 0);
        device->SetShaderProgram(program);

        device->SetTexture(0, m_RT[0]);

        device->SetVertexBuffer(0, 0, m_VB->GetStride(), m_VB);
        device->SetVertexDeclaration(m_VD);

        device->DrawPrimitive(
            izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
            0,
            2);
    }

    RenderName(device, "SSAO");

    return IZ_TRUE;
}

void CStateSSAO::RenderScene(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CDebugMesh* mesh,
    const izanagi::math::SVector& position)
{
    izanagi::math::SMatrix mtxL2W;
    izanagi::math::SMatrix::GetTrans(mtxL2W, position);

    SetShaderParam(
        m_Shader,
        "g_mL2W",
        (void*)&mtxL2W,
        sizeof(mtxL2W));

    m_Shader->CommitChanges(device);

    mesh->Draw(device);
}

// 開始
IZ_BOOL CStateSSAO::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL result = IZ_TRUE;

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
        m_RT[0] = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
        m_RT[1] = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
        m_RT[2] = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
        m_RT[3] = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32F);
    }

    {
        IZ_FLOAT u = 0.5f / 1280.0f;
        IZ_FLOAT v = 0.5f / 720.0f;

        struct SVtx {
            IZ_FLOAT pos[4];
            IZ_FLOAT uv[2];
        } vtx[] = {
            {
                {0.0f, 0.0f, 0.0f, 1.0f},
                {0.0f + u, 0.0f + v},
            },
            {
                {1.0f, 0.0f, 0.0f, 1.0f},
                {1.0f + u, 0.0f + v},
            },
            {
                {0.0f, 1.0f, 0.0f, 1.0f},
                {0.0f + u, 1.0f + v},
            },
            {
                {1.0f, 1.0f, 0.0f, 1.0f},
                {1.0f + u, 1.0f + v},
            },
        };

        m_VB = device->CreateVertexBuffer(sizeof(SVtx), COUNTOF(vtx), izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        m_VB->LockBuffer(0, 0, IZ_FALSE);
        m_VB->WriteBuffer(vtx, sizeof(vtx));
        m_VB->UnlockBuffer();

        izanagi::graph::SVertexElement elem[] = {
            {0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},
            {0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},
        };

        m_VD = device->CreateVertexDeclaration(elem, COUNTOF(elem));
    }

    izanagi::math::SMatrix::SetUnit(m_L2W);

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateSSAO::Leave()
{
    SAFE_RELEASE(m_Shader);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Plane);

    for (IZ_UINT i = 0; i < COUNTOF(m_RT); i++) {
        SAFE_RELEASE(m_RT[i]);
    }

    SAFE_RELEASE(m_VB);
    SAFE_RELEASE(m_VD);

    return IZ_TRUE;
}

IZ_BOOL CStateSSAO::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    switch (key) {
    case izanagi::sys::E_KEYBOARD_BUTTON_LEFT:
        {
            IZ_INT mode = m_Mode - 1;
            m_Mode = (RenderMode)(mode < 0 ? RenderModeNum - 1 : mode);
        }
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_RIGHT:
        {
            IZ_INT mode = m_Mode + 1;
            m_Mode = (RenderMode)(mode % RenderModeNum);
        }
        break;
    default:
        return CStateBase::OnKeyDown(key);
        break;
    }

    return IZ_TRUE;
}
