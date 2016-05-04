#if !defined(__STEMCIL_TEST_APP_H__)
#define __STEMCIL_TEST_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class StencilTestApp : public izanagi::sample::CSampleApp {
public:
    StencilTestApp();
    virtual ~StencilTestApp();

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
    izanagi::CDebugMesh* m_Mesh{ nullptr };

    izanagi::CDebugMeshAxis* m_Axis{ nullptr };
    izanagi::CDebugMeshGrid* m_Grid{ nullptr };

    izanagi::CImage* m_Img{ nullptr };

    izanagi::shader::CShaderBasic* m_Shader{ nullptr };
};

#endif    // #if !defined(__STEMCIL_TEST_APP_H__)