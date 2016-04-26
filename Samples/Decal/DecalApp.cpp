#include "DecalApp.h"

CDecalApp::CDecalApp()
{
    m_Sphere = IZ_NULL;
    m_Decal = IZ_NULL;
    m_Img = IZ_NULL;
    m_Shader = IZ_NULL;
}

CDecalApp::~CDecalApp()
{
}

// 初期化.
IZ_BOOL CDecalApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, -30.0, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    // 球
    {
#if 1
        m_Sphere = CSphere::Create(
            allocator,
            device,
            10.0f,
            15, 15);
#else
        m_Sphere = CCube::Create(
            allocator,
            device,
            10.0f);
#endif
        VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
    }

    // デカール
    {
        // デカールを貼る位置を計算する
        izanagi::math::SVector4 decalPoint;
        izanagi::math::SVector4 decalNormal;

        const izanagi::math::SVector4& pos = camera.GetPos();
        const izanagi::math::SVector4& ref = camera.GetParam().ref;

        // 適当にカメラ位置から原点へのレイを飛ばす
        IZ_BOOL isCross = m_Sphere->GetCrossPoint(
            izanagi::math::CRay(
                pos,
                izanagi::math::CVector4(ref, pos, izanagi::math::CVector4::INIT_SUB)),
            decalPoint,
            &decalNormal);
        IZ_ASSERT(isCross);

        // TODO
        decalNormal.Set(0.0f, 0.0f, -1.0f, 0.0f);

        m_Decal = izanagi::scenegraph::CDecalSTL::Create(
            allocator,
            decalPoint,
            decalNormal,
            2.0f, 2.0f);
        VGOTO(result = (m_Decal != IZ_NULL), __EXIT__);
    }

    // テクスチャ
    {
        izanagi::CFileInputStream in;
        VGOTO(result = in.Open("data/asset.img"), __EXIT__);

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
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

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void CDecalApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Decal);
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Shader);
}

// 更新.
void CDecalApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    m_Decal->ComputeDecalMesh(
        m_Sphere->GetTriangles(),
        m_Sphere->GetTriNum());
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
void CDecalApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sample::CSampleCamera& camera = GetCamera();

    m_Decal->CreateGraphicsObject(device);

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    izanagi::math::SMatrix44 mtx;
    izanagi::math::SMatrix44::SetUnit(mtx);

    izanagi::math::SMatrix44 mtxDecalW2C;
    camera.GetOffsetV2C(
        mtxDecalW2C,
        m_Decal->GetCenter(),
        1e-3f);
    izanagi::math::SMatrix44::Mul(
        mtxDecalW2C,
        camera.GetParam().mtxW2V,
        mtxDecalW2C);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {        
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mL2W",
                (void*)&mtx,
                sizeof(mtx));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            // 球
            device->SetTexture(0, m_Img->GetTexture(0));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            // 球
            m_Sphere->Draw(device);

            // デカール
            device->SetTexture(0, m_Img->GetTexture(1));

            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&mtxDecalW2C,
                sizeof(mtxDecalW2C));

            // シェーダ設定
            m_Shader->CommitChanges(device);

            m_Decal->Draw(device);

            m_Shader->EndPass();
        }
    }

    m_Shader->End(device);
}

IZ_BOOL CDecalApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}

void CDecalApp::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
}

IZ_BOOL CDecalApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    CSampleApp::OnMouseLBtnDown(point);

    return IZ_TRUE;
}
