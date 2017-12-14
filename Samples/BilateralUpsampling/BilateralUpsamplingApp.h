#if !defined(__BILATERAL_UPSAMPLING_APP_H__)
#define __BILATERAL_UPSAMPLING_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

#include "GBuffer.h"
#include "Scene.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class BilateralUpsamplingApp : public izanagi::sample::CSampleApp {
public:
    BilateralUpsamplingApp();
    virtual ~BilateralUpsamplingApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    void renderGeometryPass(izanagi::graph::CGraphicsDevice* device);
    void renderColorPass(izanagi::graph::CGraphicsDevice* device);
    void renderFinalPass(izanagi::graph::CGraphicsDevice* device);

protected:
    Scene m_scene;

    izanagi::sample::Shader m_shdGeometryPass;
    izanagi::sample::Shader m_shdColorPass;
    izanagi::sample::Shader m_shdFinalPass;

    GBuffer m_gbuffer;

    izanagi::CDebugMesh* m_screenFillPlane{ nullptr };

	enum Type : int {
		Upscale,
		Reference,
		Non,

		Num,
	};

	Type m_type{ Type::Upscale };

	izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};

#endif    // #if !defined(__BILATERAL_UPSAMPLING_APP_H__)
