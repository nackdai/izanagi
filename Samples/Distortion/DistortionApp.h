#if !defined(__DISTORTION_APP_H__)
#define __DISTORTION_APP_H__

#include "izSampleKit.h"
#include "izShader.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class DistortionApp : public izanagi::sample::CSampleApp {
public:
    DistortionApp();
    virtual ~DistortionApp();

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
    izanagi::CImage* m_Img{ nullptr };

    izanagi::shader::CShaderBasic* m_shd{ nullptr };
};

#endif  // #if !defined(__DISTORTION_APP_H__)
