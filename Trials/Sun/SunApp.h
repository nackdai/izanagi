#if !defined(__SUN_APP_H__)
#define __SUN_APP_H__

#include "izSampleKit.h"
#include "izDebugUtil.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CSunApp : public izanagi::sample::CSampleApp {
public:
    CSunApp();
    virtual ~CSunApp();

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
    izanagi::CShaderBasic* m_Shader;
    izanagi::CDebugMeshSphere* m_Sphere;
};

#endif    // #if !defined(__SUN_APP_H__)