#if !defined(__MATERIAL_APP_H__)
#define __MATERIAL_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMaterialApp : public izanagi::sample::CSampleApp {
public:
    CMaterialApp();
    virtual ~CMaterialApp();

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
    izanagi::CImage* m_Img;
    izanagi::CModel* m_Mdl;
    izanagi::CMesh* m_Msh;
    izanagi::CSkeleton* m_Skl;
    izanagi::shader::CShaderBasic* m_Shd;

    izanagi::CMaterial* m_Mtrl[4];

    izanagi::SParallelLightParam m_ParallelLight;

    izanagi::CRenderGraph* m_RenderGraph;
    izanagi::CSceneRenderer* m_Renderer;

    izanagi::math::SMatrix44 m_L2W;
};

#endif    // #if !defined(__MODEL_APP_H__)
