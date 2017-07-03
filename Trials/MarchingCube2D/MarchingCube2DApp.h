#if !defined(__MARCHING_CUBE_2D_APP_H__)
#define __MARCHING_CUBE_2D_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class MarchingCube2DApp : public izanagi::sample::CSampleApp {
public:
    MarchingCube2DApp();
    virtual ~MarchingCube2DApp();

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
    izanagi::sample::CSampleEnvBox* m_Cube;

    izanagi::CImage* m_Img;
    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::math::SMatrix44 m_L2W;
};

#endif    // #if !defined(__MARCHING_CUBE_2D_APP_H__)