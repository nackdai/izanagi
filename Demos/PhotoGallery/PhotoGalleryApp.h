#if !defined(__PHOTO_GALLERY_APP_H__)
#define __PHOTO_GALLERY_APP_H__

#include "izSampleKit.h"

class Seat;
class PhotoItem;

class PhotoGalleryApp : public izanagi::sample::CSampleApp {
public:
    PhotoGalleryApp();
    virtual ~PhotoGalleryApp();

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
    Seat* m_Seat;
};

#endif    // #if !defined(__PHOTO_GALLERY_APP_H__)