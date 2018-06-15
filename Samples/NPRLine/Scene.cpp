#include "Scene.h"

IZ_BOOL Scene::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    IZ_COLOR colors[] = {
        IZ_COLOR_RGBA(0xff,    0,    0, 0xff),
        IZ_COLOR_RGBA(   0, 0xff,    0, 0xff),
        IZ_COLOR_RGBA(   0,    0, 0xff, 0xff),
        IZ_COLOR_RGBA(0xff, 0xff,    0, 0xff),
        IZ_COLOR_RGBA(   0, 0xff, 0xff, 0xff),
    };

    // Sphere
    m_Mesh[MESH_TYPE_SPHERE] = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
        allocator,
        device,
        flag,
        colors[MESH_TYPE_SPHERE],
        5.0f,
        10, 10);
    VGOTO(result = (m_Mesh[MESH_TYPE_SPHERE] != IZ_NULL), __EXIT__);

    // Cube
    m_Mesh[MESH_TYPE_CUBE] = izanagi::CDebugMeshBox::CreateDebugMeshBox(
        allocator,
        device,
        flag,
        colors[MESH_TYPE_CUBE],
        10.0f, 10.0f, 10.0f);
    VGOTO(result = (m_Mesh[MESH_TYPE_CUBE] != IZ_NULL), __EXIT__);

#if 0
    // Cylinder
    m_Mesh[MESH_TYPE_CYLINDER] = izanagi::CDebugMeshCylinder::CreateDebugMeshCylinder(
        allocator,
        device,
        flag,
        colors[MESH_TYPE_CYLINDER],
        5.0f, 10.0f,
        10, 10);
    VGOTO(result = (m_Mesh[MESH_TYPE_CYLINDER] != IZ_NULL), __EXIT__);
#endif

    // Torus
    m_Mesh[MESH_TYPE_TORUS] = izanagi::CDebugMeshTorus::CreateDebugMeshTorus(
        allocator,
        device,
        flag,
        colors[MESH_TYPE_TORUS],
        2.0f, 5.0f,
        10, 10);
    VGOTO(result = (m_Mesh[MESH_TYPE_TORUS] != IZ_NULL), __EXIT__);

    // Plane
    m_Mesh[MESH_TYPE_PLANE] = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        flag,
        colors[MESH_TYPE_PLANE],
        10, 10,
        10.0f, 10.0f);
    VGOTO(result = (m_Mesh[MESH_TYPE_PLANE] != IZ_NULL), __EXIT__);

__EXIT__:
    if (!result) {
        release();
    }

    return result;
}

// 解放.
void Scene::release()
{
    for (IZ_UINT i = 0; i < MESH_TYPE_NUM; i++) {
        SAFE_RELEASE(m_Mesh[i]);
    }
}

// 描画.
void Scene::render(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::graph::CShaderProgram* shd)
{
    izanagi::math::CVector4 pos[] = {
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f),
        izanagi::math::CVector4(-15.0f, 0.0f, -15.0f),
        izanagi::math::CVector4(15.0f, 0.0f, -15.0f),
        izanagi::math::CVector4(15.0f, 0.0f, 15.0f),
        izanagi::math::CVector4(-15.0f, 0.0f, 15.0f),
    };

    for (IZ_UINT i = 0; i < MESH_TYPE_NUM; i++) {
        izanagi::math::CMatrix44 mtxL2W;
        mtxL2W.SetTrans(pos[i]);

        auto hL2W = shd->GetHandleByName("mtxL2W");
        shd->SetMatrix(device, hL2W, mtxL2W);

		// NOTE
		// id = 0 は背景に使う.
        auto hId = shd->GetHandleByName("id");
        shd->SetInt(device, hId, i + 1);

        m_Mesh[i]->Draw(device);
    }
}
