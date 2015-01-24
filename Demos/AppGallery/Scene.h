#if !defined(__SCENE_H__)
#define __SCENE_H__

#include "izSceneGraph.h"

class Ring;

class Scene {
private:
    static Scene instance;

public:
    static Scene& Instance();

private:
    Scene();
    ~Scene();

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void Terminate();

    void Render(
        const izanagi::CCamera& camera,
        izanagi::graph::CGraphicsDevice* device);

private:
    void RenderByBevelShader(
        const izanagi::CCamera& camera,
        izanagi::graph::CGraphicsDevice* device);

    void RenderRing(
        const izanagi::CCamera& camera,
        izanagi::graph::CGraphicsDevice* device);

    void RenderByBasicShader(
        const izanagi::CCamera& camera,
        izanagi::graph::CGraphicsDevice* device);

private:
    Ring* m_Ring;

    izanagi::shader::CShaderBasic* m_BevelShader;
    izanagi::shader::CShaderBasic* m_BasicShader;
};

#endif    // #if !defined(__SCENE_H__)
