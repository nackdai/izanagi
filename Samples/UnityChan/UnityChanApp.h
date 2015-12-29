#if !defined(__UNITY_CHAN_APP_H__)
#define __UNITY_CHAN_APP_H__

#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class UnityChanApp : public izanagi::sample::CSampleApp {
public:
    UnityChanApp();
    virtual ~UnityChanApp();

public:
    // キーボード押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

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

#endif    // #if !defined(__UNITY_CHAN_APP_H__)
