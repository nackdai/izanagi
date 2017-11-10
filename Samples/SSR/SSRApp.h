#if !defined(__SSR_APP_H__)
#define __SSR_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SSRApp : public izanagi::sample::CSampleApp {
public:
    SSRApp();
    virtual ~SSRApp();

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

private:
    void renderToGBufferPass(izanagi::graph::CGraphicsDevice* device);
	void renderToScreenPass(izanagi::graph::CGraphicsDevice* device);
    void renderSSRPass(izanagi::graph::CGraphicsDevice* device);

protected:
	izanagi::CDebugMesh* m_sphere{ nullptr };
	izanagi::CDebugMesh* m_cube{ nullptr };

	izanagi::CDebugMesh* m_plane{ nullptr };

    izanagi::sample::Shader m_shdGeometryPass;
    izanagi::sample::Shader m_shdSSRPass;

    GBuffer m_gbuffer;

	izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};

#endif    // #if !defined(__SSR_APP_H__)
