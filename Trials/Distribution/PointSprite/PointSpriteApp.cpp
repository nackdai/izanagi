#include "PointSpriteApp.h"

PointSpriteApp::PointSpriteApp()
{
}

PointSpriteApp::~PointSpriteApp()
{
}

// 初期化.
IZ_BOOL PointSpriteApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    {
        m_vb = device->CreateVertexBuffer(
            sizeof(Vertex),
            POINT_NUM,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

        Vertex* vtx;
        auto pitch = m_vb->Lock(0, 0, (void**)&vtx, IZ_FALSE);

        static const IZ_COLOR colors[] = {
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff),
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff),
            IZ_COLOR_RGBA(0, 0, 0xff, 0xff),
        };

        izanagi::math::CMathRand::Init(0);

        for (IZ_UINT i = 0; i < POINT_NUM; i++) {
            vtx[i].pos[0] = izanagi::math::CMathRand::GetRandFloat() * 100.0f;
            vtx[i].pos[1] = izanagi::math::CMathRand::GetRandFloat() * 100.0f;
            vtx[i].pos[2] = izanagi::math::CMathRand::GetRandFloat() * 100.0f;

            vtx[i].pos[3] = 1.0f;

            vtx[i].color = colors[i % COUNTOF(colors)];
        }

        m_vb->Unlock();
    }

    {
        izanagi::graph::SVertexElement elems[] = {
            { 0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0 },
            { 0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR, 0 },
        };

        m_vd = device->CreateVertexDeclaration(elems, COUNTOF(elems));
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
void PointSpriteApp::ReleaseInternal()
{
    SAFE_RELEASE(m_vb);
    SAFE_RELEASE(m_vd);

    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_ps);
    SAFE_RELEASE(m_shd);
}

// 更新.
void PointSpriteApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
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
void PointSpriteApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    CALL_GL_API(::glEnable(GL_VERTEX_PROGRAM_POINT_SIZE));
    CALL_GL_API(::glEnable(GL_POINT_SPRITE));

    device->SetShaderProgram(m_shd);

    auto& camera = GetCamera();

    izanagi::math::SMatrix44 mtxW2C;
    izanagi::math::SMatrix44::Copy(mtxW2C, camera.GetParam().mtxW2C);

    IZ_FLOAT pointSize = 100.0f;
    auto hSize = m_shd->GetHandleByName("size");
    m_shd->SetFloat(device, hSize, pointSize);

    auto hMtxW2C = m_shd->GetHandleByName("mtxW2C");
    m_shd->SetMatrixArrayAsVectorArray(device, hMtxW2C, &mtxW2C, 4);

    device->SetVertexBuffer(0, 0, sizeof(Vertex), m_vb);
    device->SetVertexDeclaration(m_vd);

    device->DrawPrimitive(
        izanagi::graph::E_GRAPH_PRIM_TYPE_POINTLIST,
        0,
        POINT_NUM);
}

IZ_BOOL PointSpriteApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
