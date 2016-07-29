#if !defined(__SINGLE_PASS_STEREO_RENDERING_APP_H__)
#define __SINGLE_PASS_STEREO_RENDERING_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SinglePassStereoRenderingApp : public izanagi::sample::CSampleApp {
public:
    SinglePassStereoRenderingApp();
    virtual ~SinglePassStereoRenderingApp();

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

    void RenderDefaultStereo(izanagi::graph::CGraphicsDevice* device);
    void RenderSinglePassStereo(izanagi::graph::CGraphicsDevice* device);

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    static const IZ_UINT MeshNum = 10000;

    izanagi::CDebugMesh* m_Mesh[MeshNum];
    izanagi::math::SMatrix44 m_L2W[MeshNum];

    izanagi::CImage* m_Img[10];

    izanagi::graph::CVertexShader* m_vs{ nullptr };
    izanagi::graph::CPixelShader* m_ps{ nullptr };
    izanagi::graph::CShaderProgram* m_shd{ nullptr };

    izanagi::graph::CVertexShader* m_vs_default{ nullptr };
    izanagi::graph::CPixelShader* m_ps_default{ nullptr };
    izanagi::graph::CShaderProgram* m_shd_default{ nullptr };

    IZ_BOOL m_enableAdvanced{ IZ_FALSE };
    IZ_INT m_curNum{ 5000 };
};

#endif    // #if !defined(__SINGLE_PASS_STEREO_RENDERING_APP_H__)