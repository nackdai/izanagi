#include "FrustumCullingApp.h"

#define FRUSTUM_INTERSECT_EX

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

    IZ_FLOAT fNear = 1.0f;
    IZ_FLOAT fFar = 500.0f;
    IZ_FLOAT verticalFOV = izanagi::math::CMath::Deg2Rad(60.0f);
    IZ_FLOAT aspect = (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight();

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
        aspect,
        verticalFOV,
        fNear,
        fFar);

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
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        fNear,
        fFar,
        verticalFOV,
        aspect);
    camera.Update();

    m_internalCamera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, 30.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        fNear,
        fFar,
        verticalFOV,
        aspect);
    m_internalCamera.Update();

#ifndef FRUSTUM_INTERSECT_EX
    m_frustum.initialize(
        fNear,
        fFar,
        verticalFOV,
        aspect);
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
    m_internalCamera.Update();
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

#ifdef FRUSTUM_INTERSECT_EX
            izanagi::math::SMatrix44 mtxW2C = m_internalCamera.GetParam().mtxW2C;
            m_frustum.computePlane(mtxW2C);
#endif

            for (IZ_UINT i = 0; i < CUBE_NUM; i++)
            {
                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&m_cubes[i].mtxL2W,
                    sizeof(izanagi::math::SMatrix44));

#ifdef FRUSTUM_INTERSECT_EX
                auto isIntersect = m_frustum.isIntersect(&m_cubes[i].aabb);
#else
                const izanagi::math::SMatrix44& mtxW2V = m_internalCamera.GetParam().mtxW2V;
                auto isIntersect = m_frustum.isIntersect(
                    &m_cubes[i].aabb,
                    mtxW2V);
#endif

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

                izanagi::math::SMatrix44 mtxL2W_Frustum = m_internalCamera.GetParam().mtxW2V;
                izanagi::math::SMatrix44::Inverse(mtxL2W_Frustum, mtxL2W_Frustum);

                _SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtxL2W_Frustum,
                    sizeof(mtxL2W_Frustum));

                // シェーダ設定
                m_Shader->CommitChanges(device);

                m_FrustumMesh->Draw(device);
            }

            m_Shader->EndPass();
        }
    }
    m_Shader->End(device);
}

IZ_BOOL FrustumCullingApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON::E_KEYBOARD_BUTTON_CONTROL) {
        setEnableCamera(IZ_FALSE);
        m_isEnableInternalCamera = IZ_TRUE;
    }

    return IZ_TRUE;
}

void FrustumCullingApp::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON::E_KEYBOARD_BUTTON_CONTROL) {
        setEnableCamera(IZ_TRUE);
        m_isEnableInternalCamera = IZ_FALSE;
    }
}

namespace {
    inline IZ_FLOAT _NormalizeHorizontal(
        izanagi::sample::CSampleApp* app,
        IZ_UINT x)
    {
        IZ_UINT width = app->GetScreenWidth();
        IZ_FLOAT ret = (2.0f * x - width) / (IZ_FLOAT)width;
        return ret;
    }

    inline IZ_FLOAT _NormalizeVertical(
        izanagi::sample::CSampleApp* app,
        IZ_UINT y)
    {
        IZ_UINT height = app->GetScreenHeight();
        IZ_FLOAT ret = (height - 2.0f * y) / (IZ_FLOAT)height;
        return ret;
    }
}

void FrustumCullingApp::OnMouseMoveInternal(const izanagi::CIntPoint& point)
{
    if (m_isEnableInternalCamera) {
        if (isOnMouseLBtn()) {
            m_internalCamera.Rotate(
                izanagi::CFloatPoint(
                    _NormalizeHorizontal(this, m_PrevPoint.x),
                    _NormalizeVertical(this, m_PrevPoint.y)),
                izanagi::CFloatPoint(
                    _NormalizeHorizontal(this, point.x),
                    _NormalizeVertical(this, point.y))
                );
        }
        else if (isOnMouseRBtn()) {
            float fOffsetX = (float)(m_PrevPoint.x - point.x);
            fOffsetX *= 0.5f;

            float fOffsetY = (float)(m_PrevPoint.y - point.y);
            fOffsetY *= 0.5f;

            m_internalCamera.Move(fOffsetX, fOffsetY);
        }
    }
}