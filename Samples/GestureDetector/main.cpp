#include "izSampleKit.h"
#include "izDebugUtil.h"
#include <imgui.h>

class ImGuiApp : public izanagi::sample::CSampleApp {
public:
    ImGuiApp();
    virtual ~ImGuiApp();

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
    izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};

ImGuiApp::ImGuiApp()
{
}

ImGuiApp::~ImGuiApp()
{
}

// 初期化.
IZ_BOOL ImGuiApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_imgui = izanagi::debugutil::ImGuiProc::init(allocator, device);
    IZ_ASSERT(m_imgui);
    return IZ_TRUE;
}

// 解放.
void ImGuiApp::ReleaseInternal()
{
    SAFE_RELEASE(m_imgui);
}

// 更新.
void ImGuiApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    m_imgui->beginFrame();

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        //ImGui::ColorEdit3("clear color", (float*)&clear_col);
        if (ImGui::Button("Test Window")) {
            //show_test_window ^= 1;
        }
        if (ImGui::Button("Another Window")) {
            //show_another_window ^= 1;
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}

// 描画.
void ImGuiApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    ImGui::Render();
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

ImGuiApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "GestureDetector",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
