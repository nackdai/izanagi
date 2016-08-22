#include "RayHitApp.h"

#define FRUSTUM_INTERSECT_EX

RayHitApp::RayHitApp()
{
    m_Shader = IZ_NULL;

    m_mousePoint.x = -1;
    m_mousePoint.y = -1;
}

RayHitApp::~RayHitApp()
{
}

// 初期化.
IZ_BOOL RayHitApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
                    | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

    // Cube
    {
        m_hitable[0].mesh = izanagi::CDebugMeshBox::CreateDebugMeshBox(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff),
            10.0f, 10.0f, 10.0f);

        izanagi::math::CVector4 pos(0.0f, 0.0f, 0.0f);

        m_cube.initialize(
            izanagi::math::CVector4(0.0f, 0.0f, 0.0f),
            10.0f, 10.0f, 10.0f,
            IZ_TRUE);

        m_cube.getL2W().SetTrans(pos.x, pos.y, pos.z);

        m_hitable[0].bound = &m_cube;
    }

    // Sphere
    {
        m_hitable[1].mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff),
            10.0f, 10, 10);

        izanagi::math::CVector4 pos(20.0f, 0.0f, 0.0f);

        m_sphere.initialize(
            10.0f,
            izanagi::math::CVector4(0.0f, 0.0f, 0.0f));

        m_sphere.getL2W().SetTrans(pos.x, pos.y, pos.z);

        m_hitable[1].bound = &m_sphere;
    }

    // Rectangle
    {
        m_hitable[2].mesh = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
            allocator,
            device,
            flag,
            IZ_COLOR_RGBA(0xff, 0xff, 0, 0xff),
            1, 1, 
            10.0f, 10.0f);

        izanagi::math::CVector4 pos(-20.0f, 0.0f, 0.0f);

        m_rect.Set(
            izanagi::math::CVector4(-5.0f, 0.0f,  5.0f),
            izanagi::math::CVector4( 5.0f, 0.0f,  5.0f),
            izanagi::math::CVector4(-5.0f, 0.0f, -5.0f),
            izanagi::math::CVector4( 5.0f, 0.0f, -5.0f),
            IZ_TRUE);

        m_rect.getL2W().SetTrans(pos.x, pos.y, pos.z);

        m_hitable[2].bound = &m_rect;
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
void RayHitApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < HitableNum; i++)
    {
        m_hitable[i].release();
    }

    SAFE_RELEASE(m_Shader);
}

// 更新.
void RayHitApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    if (m_mousePoint.x >= 0
        && m_mousePoint.y >= 0)
    {
        izanagi::math::CVector4 dir;

        izanagi::CSceneGraphUtil::Point2Ray(
            dir,
            GetCamera().GetParam(),
            device->GetViewport(),
            m_mousePoint.x,
            m_mousePoint.y);

        izanagi::col::Ray ray(
            GetCamera().GetParam().pos,
            dir);

        for (IZ_UINT i = 0; i < HitableNum; i++)
        {
            izanagi::col::HitResult res;
            m_hitable[i].isHit = m_hitable[i].bound->isHit(ray, res);
        }
    }
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
void RayHitApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_BOOL isDrawTangentSpaceAxis = IZ_FALSE;

    izanagi::sample::CSampleCamera& camera = GetCamera();

    izanagi::math::SMatrix44 mtxL2W;
    izanagi::math::SMatrix44::SetUnit(mtxL2W);

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    m_Shader->Begin(device, 0, IZ_FALSE);
    {
        if (m_Shader->BeginPass(0)) {
            _SetShaderParam(
                m_Shader,
                "g_mW2C",
                (void*)&camera.GetParam().mtxW2C,
                sizeof(camera.GetParam().mtxW2C));

            for (IZ_UINT i = 0; i < HitableNum; i++)
            {
                const auto& mtxL2W = m_hitable[i].bound->getL2W();

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

                izanagi::math::CVector4 color(1.0f, 1.0f, 1.0f, 1.0f);

                if (m_hitable[i].isHit) {
                    color.a = 0.5f;
                }

                _SetShaderParam(
                    m_Shader,
                    "g_Color",
                    (void*)&color,
                    sizeof(color));

                m_Shader->CommitChanges(device);

                m_hitable[i].mesh->Draw(device, isDrawTangentSpaceAxis);
            }
            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);

}

IZ_BOOL RayHitApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_CONTROL) {
        m_isPressedKey = IZ_TRUE;
    }
    return IZ_TRUE;
}

void RayHitApp::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_CONTROL) {
        m_isPressedKey = IZ_FALSE;

        for (IZ_UINT i = 0; i < HitableNum; i++)
        {
            m_hitable[i].isHit = IZ_FALSE;
        }

        m_mousePoint.x = -1;
        m_mousePoint.y = -1;
    }
}

IZ_BOOL RayHitApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    izanagi::sample::CSampleApp::OnMouseLBtnDown(point);

    if (m_isPressedKey) {
        m_mousePoint = point;
    }

    return IZ_TRUE;
}
