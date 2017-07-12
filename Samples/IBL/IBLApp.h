#if !defined(__IBL_APP_H__)
#define __IBL_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class IBLApp : public izanagi::sample::CSampleApp {
public:
    IBLApp();
    virtual ~IBLApp();

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
    izanagi::CDebugMesh* m_Mesh;

    izanagi::CDebugMeshAxis* m_Axis;
    izanagi::CDebugMeshGrid* m_Grid;

    izanagi::sample::Shader m_shd;
};

#endif    // #if !defined(__IBL_APP_H__)