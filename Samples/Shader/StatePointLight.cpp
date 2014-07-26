#include "StatePointLight.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStatePointLight::CStatePointLight(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
    m_Shader = IZ_NULL;

    m_Light = IZ_NULL;
    
    m_Sphere = IZ_NULL;
    m_Cube = IZ_NULL;
    m_Plane = IZ_NULL;
}

CStatePointLight::~CStatePointLight()
{
    Destroy();
}

// 描画.
IZ_BOOL CStatePointLight::Render(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            // パラメータ設定
            SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Camera.mtxW2C,
                sizeof(m_Camera.mtxW2C));

            SetShaderParam(
                m_Shader,
                "g_vEye",
                (void*)&m_Camera.pos,
                sizeof(m_Camera.pos));

            SetShaderParam(
                m_Shader,
                "g_PointLightPos",
                (void*)&m_PointLight.vPos,
                sizeof(m_PointLight.vPos));
            SetShaderParam(
                m_Shader,
                "g_PointLight",
                (void*)&m_PointLight.attn,
                sizeof(m_PointLight.attn));
            SetShaderParam(
                m_Shader,
                "g_PointLightColor",
                (void*)&m_PointLight.color,
                sizeof(m_PointLight.color));

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

        if (m_Shader->BeginPass(1)) {
            // パラメータ設定
            SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&m_Camera.mtxW2C,
                sizeof(m_Camera.mtxW2C));

            SetShaderParam(
                m_Shader,
                "g_vEye",
                (void*)&m_Camera.pos,
                sizeof(m_Camera.pos));

            // ライト
            RenderScene(
                device, 
                m_Light,
                m_PointLight.vPos);
        }
    }
    m_Shader->End(device);

    RenderName(device, "PointLight");

    return IZ_TRUE;
}

void CStatePointLight::RenderScene(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CDebugMesh* mesh,
    const izanagi::math::SVector4& position)
{
    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::GetTrans(mtxL2W, position);

    SetShaderParam(
        m_Shader,
        "g_mL2W",
        (void*)&mtxL2W,
        sizeof(mtxL2W));

    m_Shader->CommitChanges(device);

    mesh->Draw(device);
}

// 開始
IZ_BOOL CStatePointLight::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL result = IZ_TRUE;

    // シェーダ
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/PointLightShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
                    allocator,
                    device,
                    &in);
        VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
    }

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

    // ライト
    {
        m_Light = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0x00, 0xff),
            0.5f, 10, 10);
        VGOTO(result = (m_Light != IZ_NULL), __EXIT__);
    }

    // 球
    {
        m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
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
            IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
            10.0f, 10.0f, 10.0f);
        VGOTO(result = (m_Cube != IZ_NULL), __EXIT__);
    }

    // 地面
    {
        m_Plane = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff),
            10, 10,
            50.0f, 50.0f);
        VGOTO(result = (m_Plane != IZ_NULL), __EXIT__);
    }

    // ポイントライト
    {
        m_PointLight.vPos.Set(0.0f, 5.0f, 0.0f);
        m_PointLight.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
        m_PointLight.attn.Set(0.0f, 0.0f, 0.01f, 0.0f);
    }

__EXIT__:
    if (!result) {
        Leave();
    }

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStatePointLight::Leave()
{
    SAFE_RELEASE(m_Shader);

    SAFE_RELEASE(m_Light);
    SAFE_RELEASE(m_Sphere);
    SAFE_RELEASE(m_Cube);
    SAFE_RELEASE(m_Plane);

    return IZ_TRUE;
}
