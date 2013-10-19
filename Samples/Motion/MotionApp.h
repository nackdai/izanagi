#if !defined(__ANIMATION_APP_H__)
#define __ANIMATION_APP_H__

#include "izSampleKit.h"
#include "izAnimation.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CMotionApp : public izanagi::sample::CSampleApp {
public:
    CMotionApp();
    virtual ~CMotionApp();

public:
    // キーボード押下
    virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

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

    // 背景色取得.
    virtual IZ_COLOR GetBgColor() const;
};

#endif    // #if !defined(__ANIMATION_APP_H__)
