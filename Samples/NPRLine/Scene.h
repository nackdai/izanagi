#pragma once

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

class Scene {
public:
    Scene() {}
    ~Scene() {}

public:
    // 初期化.
    IZ_BOOL init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    void release();

    // 描画.
    void render(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::graph::CShaderProgram* shd);

protected:
    enum MESH_TYPE {
        MESH_TYPE_SPHERE = 0,
        MESH_TYPE_CUBE,
        //MESH_TYPE_CYLINDER,
        MESH_TYPE_TORUS,
        MESH_TYPE_PLANE,

        MESH_TYPE_NUM,
    };

    izanagi::CDebugMesh* m_Mesh[MESH_TYPE_NUM];
};
