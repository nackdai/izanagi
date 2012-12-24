#if !defined(__SCENE_RENDERER_H__)
#define __SCENE_RENDERER_H__

#include "izStd.h"
#include "izShader.h"
#include "izDebugUtil.h"
#include "izSceneGraph.h"
#include "izSampleKit.h"

class CSceneRenderer
{
private:
    static CSceneRenderer sInstance;

public:
    static CSceneRenderer* GetInstance()
    {
        return &sInstance;
    }

private:
    CSceneRenderer();
    ~CSceneRenderer();

    NO_COPIABLE(CSceneRenderer);

public:
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void Render(
        const izanagi::SCameraParam& camera,
        izanagi::graph::CGraphicsDevice* device);

    void Release();

private:
    IZ_BOOL InitTorus(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL InitEnvBox(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void RenderTorus(
        const izanagi::SCameraParam& camera,
        izanagi::graph::CGraphicsDevice* device);

    void RenderEnvBox(
        const izanagi::SCameraParam& camera,
        izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::CDebugMesh* m_Torus;
    izanagi::CShaderBasic* m_Shader;

    izanagi::sample::CSampleEnvBox* m_EnvBox;
    izanagi::CImage* m_EnvImg;
    izanagi::CShaderBasic* m_EnvShader;

    izanagi::math::SMatrix m_L2W;
    izanagi::SParallelLightParam m_ParallelLight;
};

#endif    // #if !defined(__SCENE_RENDERER_H__)
