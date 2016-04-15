#if !defined(__DEBUG_MESH_APP_H__)
#define __DEBUG_MESH_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CDebugMeshApp : public izanagi::sample::CSampleApp {
public:
    CDebugMeshApp();
    virtual ~CDebugMeshApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    enum MESH_TYPE {
        MESH_TYPE_SPHERE = 0,
        MESH_TYPE_CUBE,
        MESH_TYPE_CYLINDER,
        MESH_TYPE_TORUS,
        MESH_TYPE_PLANE,
        MESH_TYPE_WIRED_BOX,

        MESH_TYPE_NUM,
    };

    izanagi::CDebugMesh* m_Mesh[MESH_TYPE_NUM];

    izanagi::CDebugMeshAxis* m_Axis;
    izanagi::CDebugMeshGrid* m_Grid;

    izanagi::CImage* m_Img;

    izanagi::shader::CShaderBasic* m_Shader;
};

#endif    // #if !defined(__DEBUG_MESH_APP_H__)