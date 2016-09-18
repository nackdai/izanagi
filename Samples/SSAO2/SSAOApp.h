#if !defined(__SSAO_APP_H__)
#define __SSAO_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SSAOApp : public izanagi::sample::CSampleApp {
public:
    SSAOApp();
    virtual ~SSAOApp();

public:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera) override;

    // 解放.
    virtual void ReleaseInternal() override;

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) override;

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) override;

protected:
    izanagi::sample::ObjModel* m_obj{ nullptr };

    izanagi::sample::Shader m_shd;
};

#endif    // #if !defined(__SSAO_APP_H__)
