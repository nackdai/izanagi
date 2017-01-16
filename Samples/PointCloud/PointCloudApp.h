#if !defined(__POINT_CLOUD_APP_H__)
#define __POINT_CLOUD_APP_H__

#include "izSampleKit.h"
#include "izDebugUtil.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class PointCloudApp : public izanagi::sample::CSampleApp {
public:
    PointCloudApp();
    virtual ~PointCloudApp();

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
    void readPly(izanagi::graph::CGraphicsDevice* device);

    void readPcd(izanagi::graph::CGraphicsDevice* device);

    void readShaders(izanagi::graph::CGraphicsDevice* device);

    izanagi::graph::CShaderProgram* initShader(
        izanagi::graph::CGraphicsDevice* device,
        const char* vsPath,
        const char* psPath);

    void renderScene(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::graph::CShaderProgram* shd,
        izanagi::graph::CTexture* tex = nullptr);

    void renderDepth(izanagi::graph::CGraphicsDevice* device);

    void renderWeightedColor(izanagi::graph::CGraphicsDevice* device);

    void renderNormalize(izanagi::graph::CGraphicsDevice* device);

private:
    struct Vertex {
        IZ_FLOAT pos[4];
        IZ_COLOR color;
    };

    IZ_UINT m_pointNum{ 0 };

    izanagi::graph::CVertexBuffer* m_vb{ nullptr };
    izanagi::graph::CVertexDeclaration* m_vd{ nullptr };

    izanagi::graph::CShaderProgram* m_shdDefault{ nullptr };
    izanagi::graph::CShaderProgram* m_shdInterp{ nullptr };
    izanagi::graph::CShaderProgram* m_shdDepth{ nullptr };
    izanagi::graph::CShaderProgram* m_shdSplat{ nullptr };
    izanagi::graph::CShaderProgram* m_shdNml{ nullptr };

    izanagi::graph::CRenderTarget* m_rtDepth{ nullptr };
    izanagi::graph::CRenderTarget* m_rtWeightedColor{ nullptr };

    enum Mode : int {
        Default,
        Interp,
        WeightSplat,
    };
    Mode m_mode{ Mode::Default };

    float m_PointSize{ 1.0f };

    izanagi::debugutil::ImGuiProc* m_imgui{ nullptr };
};

#endif    // #if !defined(__POINT_CLOUD_APP_H__)