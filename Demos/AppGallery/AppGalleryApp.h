#if !defined(__APP_GALLERY_APP_H__)
#define __APP_GALLERY_APP_H__

#include "izSampleKit.h"

class AppGalleryApp : public izanagi::sample::CSampleApp {
public:
    AppGalleryApp();
    virtual ~AppGalleryApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
    virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);
    virtual void OnMouseMove(const izanagi::CIntPoint& point);

private:
    izanagi::sys::CTimer m_Timer;
};

#endif    // #if !defined(__APP_GALLERY_APP_H__)