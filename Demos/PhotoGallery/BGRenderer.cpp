#include "BGRenderer.h"
#include "Seat.h"
#include "BG.h"
#include "Utility.h"
#include "Environment.h"

BGRenderer BGRenderer::instance;

BGRenderer& BGRenderer::Instance()
{
    return instance;
}

static izanagi::math::SVector4 samples[32];

BGRenderer::BGRenderer()
{
    m_Light = IZ_NULL;
    m_Depth = IZ_NULL;
    m_Position = IZ_NULL;
    m_Normal = IZ_NULL;
    
    m_VB = IZ_NULL;
    m_VD = IZ_NULL;
    
    m_Shader = IZ_NULL;

    const float SAMPLERADIUS = 2.75f;

    for (int i = 0; i < COUNTOF(samples); ++i) {
        IZ_FLOAT r = SAMPLERADIUS * (IZ_FLOAT)rand() / (IZ_FLOAT)RAND_MAX;
        IZ_FLOAT t = IZ_MATH_PI2 * (IZ_FLOAT)rand() / ((IZ_FLOAT)RAND_MAX + 1.0f);
        IZ_FLOAT cp = 2.0f * (IZ_FLOAT)rand() / (IZ_FLOAT)RAND_MAX - 1.0f;
        IZ_FLOAT sp = sqrt(1.0f - cp * cp);
        IZ_FLOAT ct = cos(t);
        IZ_FLOAT st = sin(t);

        samples[i].x = r * sp * ct;
        samples[i].y = r * sp * st;
        samples[i].z = r * cp;
        samples[i].w = 0.0f;
    }

    m_EnableSSAO = IZ_TRUE;
}

BGRenderer::~BGRenderer()
{
}

IZ_BOOL BGRenderer::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL result = IZ_TRUE;

    // Shader.
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/SSAOShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    // RenderTarget for SSAO.
    {
        m_Light = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
        m_Depth = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
        m_Position = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32F);
        m_Normal = device->CreateRenderTarget(
            device->GetBackBufferWidth(),
            device->GetBackBufferHeight(),
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);

        m_RT[0] = m_Light;
        m_RT[1] = m_Normal;
        m_RT[2] = m_Depth;
        m_RT[3] = m_Position;
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

    m_Seat = Seat::Create(allocator, device);
    VGOTO(result = (m_Seat != IZ_NULL), __EXIT__);

    m_BG = BG::Create(allocator, device);
    VGOTO(result = (m_BG != IZ_NULL), __EXIT__);

__EXIT__:
    if (!result) {
        Terminate();
    }

    return result;
}

void BGRenderer::Terminate()
{
    SAFE_RELEASE(m_Light);
    SAFE_RELEASE(m_Depth);
    SAFE_RELEASE(m_Position);
    SAFE_RELEASE(m_Normal);

    m_RT[0] = m_RT[1] = m_RT[2] = m_RT[3] = IZ_NULL;
    
    SAFE_RELEASE(m_VB);
    SAFE_RELEASE(m_VD);
    
    SAFE_RELEASE(m_Shader);

    SAFE_RELEASE(m_Seat);
    SAFE_RELEASE(m_BG);
}

void BGRenderer::PrepareToRender(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    if (m_EnableSSAO) {
        RenderToMRT(device, camera);
    }
}

void BGRenderer::Render(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    //RenderDebug(device);
    //RenderDebugPointLight(device);

    if (m_EnableSSAO) {
        RenderSSAO(device, camera);
    }
    else {
        RenderNoSSAO(device, camera);
    }
}

void BGRenderer::ToggleSSAO()
{
    m_EnableSSAO = !m_EnableSSAO;
}

// Material for point light.
static const izanagi::math::SVector4 mtrlPointLight[] = {
    { 1.0f, 1.0f, 1.0f, 1.0f },
    { 0.0f, 0.0f, 0.0f, 0.0f },
};

// Material for ambient light.
static const izanagi::math::SVector4 mtrlAmbientLight[] = {
    { 0.0f, 0.0f, 0.0f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 1.0f },
};

void BGRenderer::RenderToMRT(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    device->BeginScene(
        m_RT,
        COUNTOF(m_RT),
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0));

    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::SetUnit(mtx);

    // Render to MRT.
    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            Utility::SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&camera.GetParam().mtxW2V,
                sizeof(camera.GetParam().mtxW2V));
            Utility::SetShaderParam(
                m_Shader,
                "g_mV2C",
                (void*)&camera.GetParam().mtxV2C,
                sizeof(camera.GetParam().mtxV2C));

            Utility::SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtx,
                sizeof(mtx));

            Environment::Instance().SetPointLightParam(m_Shader);
            Environment::Instance().SetAmbientLightParam(m_Shader);

            {
                Utility::SetShaderParam(
                    m_Shader,
                    "g_PointLightMeterial",
                    (void*)&mtrlPointLight[0],
                    sizeof(mtrlPointLight[0]));
                Utility::SetShaderParam(
                    m_Shader,
                    "g_vMtrlAmbient",
                    (void*)&mtrlAmbientLight[0],
                    sizeof(mtrlAmbientLight[0]));

                m_Shader->CommitChanges(device);

                m_Seat->Render(device);
            }

            {
                Utility::SetShaderParam(
                    m_Shader,
                    "g_PointLightMeterial",
                    (void*)&mtrlPointLight[1],
                    sizeof(mtrlPointLight[1]));
                Utility::SetShaderParam(
                    m_Shader,
                    "g_vMtrlAmbient",
                    (void*)&mtrlAmbientLight[1],
                    sizeof(mtrlAmbientLight[1]));

                m_BG->SetShaderParam(m_Shader, camera);

                m_Shader->CommitChanges(device);

                m_BG->Render(device);
            }
        }
        m_Shader->EndPass();
    }
    m_Shader->End(device);

    device->EndScene();
}

void BGRenderer::RenderSSAO(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    m_Shader->Begin(device, 1, IZ_TRUE);
    {
        if (m_Shader->BeginPass(0)) {
            m_Shader->SetTexture("texNormal", m_Normal);
            m_Shader->SetTexture("texPosition", m_Position);
            m_Shader->SetTexture("texDepth", m_Depth);
            m_Shader->SetTexture("texLight", m_Light);

            Utility::SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&camera.GetParam().mtxW2V,
                sizeof(camera.GetParam().mtxW2V));
            Utility::SetShaderParam(
                m_Shader,
                "g_mV2C",
                (void*)&camera.GetParam().mtxV2C,
                sizeof(camera.GetParam().mtxV2C));

            Utility::SetShaderParam(
                m_Shader,
                "samples",
                (void*)samples,
                sizeof(samples));

            m_Shader->CommitChanges(device);

            device->SetVertexBuffer(0, 0, m_VB->GetStride(), m_VB);
            device->SetVertexDeclaration(m_VD);

            device->DrawPrimitive(
                izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
                0,
                2);

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}

void BGRenderer::RenderNoSSAO(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::CCamera& camera)
{
    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::SetUnit(mtx);

    m_Shader->Begin(device, 2, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            Utility::SetShaderParam(
                m_Shader,
                "g_mW2V",
                (void*)&camera.GetParam().mtxW2V,
                sizeof(camera.GetParam().mtxW2V));
            Utility::SetShaderParam(
                m_Shader,
                "g_mV2C",
                (void*)&camera.GetParam().mtxV2C,
                sizeof(camera.GetParam().mtxV2C));

            Utility::SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtx,
                sizeof(mtx));

            Environment::Instance().SetPointLightParam(m_Shader);
            Environment::Instance().SetAmbientLightParam(m_Shader);

            {
                Utility::SetShaderParam(
                    m_Shader,
                    "g_PointLightMeterial",
                    (void*)&mtrlPointLight[0],
                    sizeof(mtrlPointLight[0]));
                Utility::SetShaderParam(
                    m_Shader,
                    "g_vMtrlAmbient",
                    (void*)&mtrlAmbientLight[0],
                    sizeof(mtrlAmbientLight[0]));

                m_Shader->CommitChanges(device);

                m_Seat->Render(device);
            }

            {
                Utility::SetShaderParam(
                    m_Shader,
                    "g_PointLightMeterial",
                    (void*)&mtrlPointLight[1],
                    sizeof(mtrlPointLight[1]));
                Utility::SetShaderParam(
                    m_Shader,
                    "g_vMtrlAmbient",
                    (void*)&mtrlAmbientLight[1],
                    sizeof(mtrlAmbientLight[1]));

                m_BG->SetShaderParam(m_Shader, camera);

                m_Shader->CommitChanges(device);

                m_BG->Render(device);
            }
        }
        m_Shader->EndPass();
    }
    m_Shader->End(device);
}

void BGRenderer::RenderDebug(izanagi::graph::CGraphicsDevice* device)
{
    device->Begin2D();
    {
        // Light
        device->SetTexture(0, m_RT[0]);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 0, 640, 360));

        // Notmal
        device->SetTexture(0, m_RT[1]);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 360, 640, 360));

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA);

        // Depth
        device->SetTexture(0, m_RT[2]);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(640, 360, 640, 360));

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_NO_TEX_ALPHA);

        // Position
        device->SetTexture(0, m_RT[3]);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(640, 0, 640, 360));
    }
    device->End2D();
}

void BGRenderer::RenderDebugPointLight(izanagi::graph::CGraphicsDevice* device)
{
    device->Begin2D();
    {
        device->SetTexture(0, m_Light);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 0, 640, 360));
    }
    device->End2D();
}
