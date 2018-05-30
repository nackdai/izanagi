#pragma once

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"

#include "GBuffer.h"
#include "Scene.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class NPRLineApp : public izanagi::sample::CSampleApp {
public:
    NPRLineApp();
    virtual ~NPRLineApp();

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

protected:
    Scene m_scene;

    izanagi::sample::Shader m_shdGeometryPass;
    izanagi::sample::Shader m_shdFinalPass;

    GBuffer m_gbuffer;

    izanagi::CDebugMesh* m_screenFillPlane{ nullptr };

	izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};
